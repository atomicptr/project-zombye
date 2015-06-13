#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <zombye/config/config_system.hpp>
#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/directional_light_component.hpp>
#include <zombye/rendering/framebuffer.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/rendering/program.hpp>
#include <zombye/rendering/screen_quad.hpp>
#include <zombye/rendering/skinned_mesh.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/shadow_component.hpp>
#include <zombye/utils/component_helper.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
	rendering_system::rendering_system(game& game, SDL_Window* window)
	: game_{game}, window_{window}, mesh_manager_{game_}, shader_manager_{game_}, skinned_mesh_manager_{game_},
	skeleton_manager_{game_}, texture_manager_{game_}, active_camera_{0}, projection_{1.f}, view_{1.f},
	shadow_resolution_{3072} {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		context_ = SDL_GL_CreateContext(window_);
		auto error = std::string{SDL_GetError()};
		if (error != "") {
			log(LOG_FATAL, "could not create OpenGL context with version 3.1: " + error);
		}
		SDL_ClearError();

		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			log(LOG_FATAL, "could not initialize glew");
		}

		auto version = std::string{reinterpret_cast<const char*>(glGetString(GL_VERSION))};
		log("OpenGL version " + version);

		glEnable(GL_DEPTH_TEST);
		clear_color(0.4, 0.5, 0.9, 1.0);

		auto vertex_shader = shader_manager_.load("shader/staticmesh.vs", GL_VERTEX_SHADER);
		auto fragment_shader = shader_manager_.load("shader/staticmesh.fs", GL_FRAGMENT_SHADER);

		staticmesh_program_ = std::make_unique<program>();
		staticmesh_program_->attach_shader(vertex_shader);
		staticmesh_program_->attach_shader(fragment_shader);

		staticmesh_layout_.emplace_back("_position", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
		staticmesh_layout_.emplace_back("_texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 3 * sizeof(float));
		staticmesh_layout_.emplace_back("_normal", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 5 * sizeof(float));
		staticmesh_layout_.emplace_back("_tangent", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 8 * sizeof(float));

		staticmesh_layout_.setup_program(*staticmesh_program_, "albedo_color");
		staticmesh_program_->bind_frag_data_location("normal_color", 1);
		staticmesh_program_->bind_frag_data_location("specular_color", 2);
		staticmesh_program_->link();

		vertex_shader = shader_manager_.load("shader/animation.vs", GL_VERTEX_SHADER);
		fragment_shader = shader_manager_.load("shader/animation.fs", GL_FRAGMENT_SHADER);

		animation_program_ = std::make_unique<program>();
		animation_program_->attach_shader(vertex_shader);
		animation_program_->attach_shader(fragment_shader);

		skinnedmesh_layout_.emplace_back("_position", 3, GL_FLOAT, GL_FALSE, sizeof(skinned_vertex), 0);
		skinnedmesh_layout_.emplace_back("_texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(skinned_vertex), 3 * sizeof(float));
		skinnedmesh_layout_.emplace_back("_normal", 3, GL_FLOAT, GL_FALSE, sizeof(skinned_vertex), 5 * sizeof(float));
		skinnedmesh_layout_.emplace_back("_tangent", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 8 * sizeof(float));
		skinnedmesh_layout_.emplace_back("_index", 4, GL_INT, GL_FALSE, sizeof(skinned_vertex), 11 * sizeof(float));
		skinnedmesh_layout_.emplace_back("_weight", 4, GL_FLOAT, GL_FALSE, sizeof(skinned_vertex), 15 * sizeof(float));

		skinnedmesh_layout_.setup_program(*animation_program_, "albedo_color");
		animation_program_->bind_frag_data_location("normal_color", 1);
		animation_program_->bind_frag_data_location("specular_color", 2);
		animation_program_->link();

		float fovy = 45.f * 3.1415f / 180.f;
		width_ = float(game_.width());
		height_ = float(game_.height());
		float aspect = width_ / height_;
		float near = 0.01f;
		float far = 1000.f;
		projection_ = glm::perspective(fovy, aspect, near, far);
		ortho_projection_ = glm::ortho(0.f, width_, 0.f, height_);

		g_buffer_ = std::make_unique<framebuffer>();
		g_buffer_->attach(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, width_, height_, GL_DEPTH_COMPONENT, GL_FLOAT);
		g_buffer_->attach(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_RGBA32F, width_, height_, GL_RGBA, GL_FLOAT);
		g_buffer_->attach(GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GL_RGBA32F, width_, height_, GL_RGBA, GL_FLOAT);
		g_buffer_->attach(GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GL_RGBA32F, width_, height_, GL_RGBA, GL_FLOAT);

		g_buffer_->bind();
		glClearColor(0.4f, 0.6f, 0.9f, 1.f);
		GLenum buf[4] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_NONE
		};
		glDrawBuffers(4, buf);
		g_buffer_->bind_default();

		screen_quad_program_ = std::make_unique<program>();
		vertex_shader = shader_manager_.load("shader/screen_quad.vs", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load screen_quad.vs"};
		}
		screen_quad_program_->attach_shader(vertex_shader);
		fragment_shader = shader_manager_.load("shader/screen_quad.fs", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load screen_quad.fs"};
		}
		screen_quad_program_->attach_shader(fragment_shader);
		staticmesh_layout_.setup_program(*screen_quad_program_, "frag_color");
		screen_quad_program_->link();

		for (auto i = 0; i < 4; ++i) {
			debug_screen_quads_.emplace_back(std::make_unique<screen_quad>(
				staticmesh_layout_,
				glm::vec2{(i * width_ / 4.f) + 0.01f * width_, (1 * height_ / 4) + 0.01f * height_},
				glm::vec2{((i + 1) * width_ / 4.f) - 0.01f * width_, 0.01f * height_}
			));
		}

		composition_program_ = std::make_unique<program>();
		vertex_shader = shader_manager_.load("shader/deferred.vs", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load deferred.vs"};
		}
		composition_program_->attach_shader(vertex_shader);
		fragment_shader = shader_manager_.load("shader/deferred.fs", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load deferred.fs"};
		}
		composition_program_->attach_shader(fragment_shader);
		staticmesh_layout_.setup_program(*composition_program_, "frag_color");
		composition_program_->link();

		screen_quad_ = std::make_unique<screen_quad>(staticmesh_layout_, glm::vec2(0.f, height_), glm::vec2(width_, 0.f));

		auto config = game.config();
		shadow_resolution_ = config->get("main", "shadow_resolution").asInt();
		shadow_map_ = std::make_unique<framebuffer>();
		shadow_map_->attach(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_RG32F, shadow_resolution_, shadow_resolution_, GL_RGBA, GL_FLOAT);
		shadow_map_->bind();
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		shadow_map_->bind_default();

		shadow_staticmesh_program_ = std::make_unique<program>();
		vertex_shader = shader_manager_.load("shader/staticmesh.vs", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load staticmesh.vs"};
		}
		shadow_staticmesh_program_->attach_shader(vertex_shader);
		fragment_shader = shader_manager_.load("shader/shadow.fs", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load shadow.fs"};
		}
		shadow_staticmesh_program_->attach_shader(fragment_shader);
		staticmesh_layout_.setup_program(*shadow_staticmesh_program_, "frag_color");
		shadow_staticmesh_program_->link();

		shadow_animation_program_ = std::make_unique<program>();
		vertex_shader = shader_manager_.load("shader/animation.vs", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load animation.vs"};
		}
		shadow_animation_program_->attach_shader(vertex_shader);
		fragment_shader = shader_manager_.load("shader/shadow.fs", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load shadow.fs"};
		}
		shadow_animation_program_->attach_shader(fragment_shader);
		skinnedmesh_layout_.setup_program(*shadow_animation_program_, "frag_color");
		shadow_animation_program_->link();
	}

	rendering_system::~rendering_system() {
		window_ = nullptr;
		SDL_GL_DeleteContext(context_);
	}

	void rendering_system::begin_scene() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void rendering_system::end_scene() {
		SDL_GL_SwapWindow(window_);
	}

	void rendering_system::update(float delta_time) {
		auto camera = camera_components_.find(active_camera_);
		view_ = glm::mat4{1.f};
		auto camera_position = glm::vec3{0.f};
		if (camera != camera_components_.end()) {
			view_ = camera->second->transform();
			camera_position = camera->second->owner().position();
		}

		render_shadowmap();

		glEnable(GL_DEPTH_TEST);
		g_buffer_->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		staticmesh_program_->use();
		staticmesh_program_->uniform("diffuse_texture", 0);
		staticmesh_program_->uniform("specular_texture", 1);
		staticmesh_program_->uniform("normal_texture", 2);
		for (auto& s : staticmesh_components_) {
			auto model = s->owner().transform();
			auto model_it = glm::inverse(glm::transpose(model));
			staticmesh_program_->uniform("m", false, model);
			staticmesh_program_->uniform("mit", false, model_it);
			staticmesh_program_->uniform("mvp", false, projection_ * view_ * model);
			s->draw();
		}

		animation_program_->use();
		animation_program_->uniform("diffuse_texture", 0);
		animation_program_->uniform("specular_texture", 1);
		animation_program_->uniform("normal_texture", 2);
		for (auto& a: animation_components_) {
			auto model = a->owner().transform();
			auto model_it = glm::inverse(glm::transpose(model));
			animation_program_->uniform("m", false, model);
			animation_program_->uniform("mit", false, model_it);
			animation_program_->uniform("mvp", false, projection_ * view_ * model);
			animation_program_->uniform("pose", a->pose().size(), false, a->pose());
			a->draw();
		}

		glDisable(GL_DEPTH_TEST);
		g_buffer_->bind_default();

		static auto debug_mode = true;
		render_screen_quad();
		if (debug_mode) {
			render_debug_screen_quads();
		}
	}

	void rendering_system::render_debug_screen_quads() const {
		const static GLenum attachments[4] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_DEPTH_ATTACHMENT
		};

		screen_quad_program_->use();
		screen_quad_program_->uniform("projection", false, ortho_projection_);
		screen_quad_program_->uniform("near_plane", 0.1f);
		screen_quad_program_->uniform("far_plane", 1000.f);
		screen_quad_program_->uniform("color_texture", 0);

		for (auto i = 0; i < 4; ++i) {
			g_buffer_->attachment(attachments[i]).bind(0);

			screen_quad_program_->uniform("linearize", false);
			if (attachments[i] == GL_DEPTH_ATTACHMENT) {
				screen_quad_program_->uniform("linearize", false);
				shadow_map_->attachment(GL_COLOR_ATTACHMENT0).bind(0);
			}

			debug_screen_quads_[i]->draw();
		}
	}

	void rendering_system::render_screen_quad()  {
		const static GLenum attachments[5] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_DEPTH_ATTACHMENT
		};

		std::vector<glm::vec3> point_light_positions;
		std::vector<glm::vec3> point_light_colors;
		std::vector<float> point_light_radii;
		for (auto& l : light_components_) {
			point_light_positions.emplace_back(l->owner().position());
			point_light_colors.emplace_back(l->color());
			point_light_radii.emplace_back(l->distance());
		}

		std::vector<glm::vec3> directional_light_directions;
		std::vector<glm::vec3> directional_light_colors;
		std::vector<float> directional_light_energy;
		for (auto& l : directional_light_components_) {
			directional_light_directions.emplace_back(l->owner().position());
			directional_light_colors.emplace_back(l->color());
			directional_light_energy.emplace_back(l->energy());
		}

		auto camera = camera_components_.find(active_camera_);
		auto camera_position = glm::vec3{0.f};
		if (camera != camera_components_.end()) {
			view_ = camera->second->transform();
			camera_position = camera->second->owner().position();
		}

		composition_program_->use();
		composition_program_->uniform("projection", false, ortho_projection_);
		composition_program_->uniform("albedo_texture", 0);
		composition_program_->uniform("normal_texture", 1);
		composition_program_->uniform("specular_texture", 2);
		composition_program_->uniform("depth_texture", 3);
		composition_program_->uniform("shadow_texture", 4);
		composition_program_->uniform("inv_view_projection", false, glm::inverse(projection_ * view_));
		composition_program_->uniform("view_vector", camera_position);
		composition_program_->uniform("point_light_num", static_cast<int32_t>(light_components_.size()));
		composition_program_->uniform("point_light_positions", light_components_.size(), point_light_positions);
		composition_program_->uniform("point_light_colors", light_components_.size(), point_light_colors);
		composition_program_->uniform("point_light_radii", light_components_.size(), point_light_radii);
		composition_program_->uniform("directional_light_num", static_cast<int32_t>(directional_light_components_.size()));
		composition_program_->uniform("directional_light_directions", directional_light_components_.size(), directional_light_directions);
		composition_program_->uniform("directional_light_colors", directional_light_components_.size(), directional_light_colors);
		composition_program_->uniform("directional_light_energy", directional_light_components_.size(), directional_light_energy);
		composition_program_->uniform("shadow_projection", false, shadow_projection_);

		for (auto i = 0; i < 4; ++i) {
			g_buffer_->attachment(attachments[i]).bind(i);
		}
		shadow_map_->attachment(GL_COLOR_ATTACHMENT0).bind(4);
		screen_quad_->draw();
	}

	void rendering_system::render_shadowmap()  {
		if (directional_light_components_.size() == 0) {
			return;
		}
		auto dir_light = directional_light_components_.front();
		auto& owner = dir_light->owner();
		auto dir_light_shadow = owner.component<shadow_component>();
		if (!dir_light_shadow) {
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glViewport(0, 0, shadow_resolution_, shadow_resolution_);

		shadow_map_->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 proj_z = owner.position();
		glm::vec3 up{0.f, 1.f, 0.f};
		glm::vec3 proj_x = glm::cross(up, proj_z);
		glm::vec3 proj_y = glm::cross(proj_z, proj_x);
		float r = 20;
		proj_x = glm::normalize(proj_x) * r;
		proj_y = glm::normalize(proj_y) * r;
		proj_z = glm::normalize(proj_z) * r;
		glm::mat3 proj_inv_tmp{proj_x, proj_y, proj_z};
		glm::mat4 proj_inv{proj_inv_tmp};
		proj_inv[3].x = 0.f;
		proj_inv[3].y = 0.f;
		proj_inv[3].z = 0.f;
		shadow_projection_ = glm::inverse(proj_inv);

		shadow_projection_ = glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);
		shadow_projection_ *= glm::lookAt(glm::vec3{0.f, 1.f, 1.f}, glm::vec3{0.f}, glm::vec3{0.f, 1.f, 0.f});

		shadow_staticmesh_program_->use();
		shadow_staticmesh_program_->uniform("diffuse_texture", 0);
		shadow_staticmesh_program_->uniform("specular_texture", 1);
		shadow_staticmesh_program_->uniform("normal_texture", 2);
		shadow_staticmesh_program_->uniform("m", false, glm::mat4{1.f});
		shadow_staticmesh_program_->uniform("mit", false, glm::mat4{1.f});
		for (auto& s : staticmesh_components_) {
			auto model = s->owner().transform();
			shadow_staticmesh_program_->uniform("mvp", false, shadow_projection_ * model);
			s->draw();
		}

		shadow_animation_program_->use();
		shadow_animation_program_->uniform("diffuse_texture", 0);
		shadow_animation_program_->uniform("specular_texture", 1);
		shadow_animation_program_->uniform("normal_texture", 2);
		shadow_animation_program_->uniform("m", false, glm::mat4{1.f});
		shadow_animation_program_->uniform("mit", false, glm::mat4{1.f});
		for (auto& a : animation_components_) {
			auto model = a->owner().transform();
			shadow_animation_program_->uniform("mvp", false, shadow_projection_ * model);
			shadow_animation_program_->uniform("pose", a->pose().size(), false, a->pose());
			a->draw();
		}

		shadow_map_->bind_default();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glViewport(0, 0, width_, height_);
	}

	void rendering_system::clear_color(float red, float green, float blue, float alpha) {
		glClearColor(red, green, blue, alpha);
	}

	void rendering_system::register_component(animation_component* component) {
		animation_components_.emplace_back(component);
	}

	void rendering_system::unregister_component(animation_component* component) {
		remove(animation_components_, component);
	}

	void rendering_system::register_component(camera_component* component) {
		camera_components_.insert(std::make_pair(component->owner().id(), component));
	}

	void rendering_system::unregister_component(camera_component* component) {
		auto it = camera_components_.find(component->owner().id());
		if (it != camera_components_.end()) {
			camera_components_.erase(it);
		}
	}

	void rendering_system::register_component(directional_light_component* component) {
		directional_light_components_.emplace_back(component);
	}

	void rendering_system::unregister_component(directional_light_component* component) {
		remove(directional_light_components_, component);
	}

	void rendering_system::register_component(light_component* component) {
		light_components_.emplace_back(component);
	}

	void rendering_system::unregister_component(light_component* component) {
		remove(light_components_, component);
	}

	void rendering_system::register_component(staticmesh_component* component) {
		staticmesh_components_.emplace_back(component);
	}

	void rendering_system::unregister_component(staticmesh_component* component) {
		remove(staticmesh_components_, component);
	}

	void rendering_system::register_component(shadow_component* component) {
		shadow_components_.emplace_back(component);
	}

	void rendering_system::unregister_component(shadow_component* component) {
		remove(shadow_components_, component);
	}
}
