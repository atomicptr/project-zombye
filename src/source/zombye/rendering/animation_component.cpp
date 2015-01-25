#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include <zombye/core/game.hpp>
#include <zombye/ecs/component.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    animation_component::animation_component(game& game, entity& owner, const std::string& mesh, const std::string& skeleton)
    : reflective{game, owner}, current_state_{""}, elapsed_time_{0.f}, current_frame_{0} {
        game_.rendering_system().register_component(this);
        game_.animation_system().register_component(this);
        load(mesh);
        load_skeleton(skeleton);
    }

    animation_component::~animation_component() noexcept {
        game_.rendering_system().unregister_component(this);
        game_.animation_system().unregister_component(this);
    }

    void animation_component::update(float delta_time) {
        static std::vector<glm::mat4> pose(skeleton_->bones().size(), glm::mat4{1.f});

        if (current_state_ != "") {
            auto& animation = skeleton_->animation(current_state_);
            auto& bones = skeleton_->bones();
            auto length = animation.length;
            auto max_frames = animation.tracks.at(0).keyframes.size();
            auto time_slot = length / max_frames;
            elapsed_time_ += delta_time;
            if (elapsed_time_ <= length) {
                auto next_frame = current_frame_ + 1;
                auto t1 = current_frame_ * time_slot;
                auto t2 = next_frame * time_slot;
                if (elapsed_time_ >= t2 && next_frame < (max_frames - 1)) {
                    ++current_frame_;
                    auto next_frame = current_frame_ + 1;
                    t1 = current_frame_ * time_slot;
                    t2 = next_frame * time_slot;
                }
                
                for (auto i = 0; i < bones.size(); ++i) {
                    auto v1 = animation.tracks.at(i).keyframes[current_frame_].translate;
                    auto v2 = animation.tracks.at(i).keyframes[next_frame].translate;

                    auto q1 = animation.tracks.at(i).keyframes[current_frame_].rotate;
                    auto q2 = animation.tracks.at(i).keyframes[next_frame].rotate;

                    if (elapsed_time_ < t2) {
                        auto delta = (elapsed_time_ - t1) / time_slot;
                        delta = delta > 1.f ? 1.f : delta;

                        auto iv = glm::lerp(v1, v2, delta);

                        auto iq = glm::normalize(glm::lerp(q1, q2, delta));

                        auto p = glm::toMat4(iq);
                        p[3].x = iv.x;
                        p[3].y = iv.y;
                        p[3].z = iv.z;

                        pose[i] = glm::mat4{1.f};
                        auto parent = animation.tracks.at(i).parent;
                        if (parent > -1) {
                            pose[i] = pose[parent];
                        }
                        pose[i] *= bones[i].relative_transform * p;
                    }
                }
            } else {
                elapsed_time_ = 0.f;
                current_frame_ = 0;
            }
        } else {
            auto& bones = skeleton_->bones();
            for (auto i = 0u; i < bones.size(); ++i) {
                pose[i] = glm::inverse(bones[i].absolute_transform);
            }
        }

        auto& bones = skeleton_->bones();

        for (auto i = 0; i < pose_.size(); ++i) {
            pose_[i] = pose[i] * bones[i].absolute_transform;
        }
    }

    void animation_component::draw() const noexcept {
        mesh_->vao().bind();
        for (auto i = 0u; i < materials_.size(); ++i) {
            materials_[i].color->bind(0);
            materials_[i].specular->bind(1);
            mesh_->draw(i);
        }
    }

    void animation_component::load(const std::string& mesh) {
        auto& rendering_system = game_.rendering_system();

        mesh_ = rendering_system.skinned_mesh_manager().load(mesh);
        if (!mesh_) {
            log(LOG_FATAL, "could not load mesh from file " + mesh);
        }

        auto asset = game_.asset_manager().load(mesh);
        if (!asset) {
            log(LOG_FATAL, "could not load asset " + mesh);
        }

        auto data_ptr = asset->content().data();
        auto size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(skinned_vertex);
        data_ptr += sizeof(size_t);
        data_ptr += size;
        size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(unsigned int);
        data_ptr += sizeof(size_t);
        data_ptr += size;
        size = *reinterpret_cast<const size_t*>(data_ptr) * sizeof(submesh);
        data_ptr += sizeof(size_t);
        data_ptr += size;

        auto material_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        auto& texture_manager = rendering_system.texture_manager();
        for (auto i = 0u; i < material_count; ++i) {
            size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            auto name = std::string{data_ptr, size};
            data_ptr += size;

            material m;
            auto color = texture_manager.load("texture/" + name + "_color.dds");
            if (!color) {
                log(LOG_FATAL, "could not load texture " + name + "_color.dds");
            }
            m.color = color;

            auto specular = texture_manager.load("texture/" + name + "_specular.dds");
            if (!color) {
                log(LOG_FATAL, "could not load texture " + name + "_specular.dds");
            }
            m.specular = specular;

            materials_.emplace_back(m);
        }
    }

    void animation_component::load_skeleton(const std::string& skeleton) {
        skeleton_ = game_.rendering_system().skeleton_manager().load(skeleton);
        if (!skeleton_) {
            log(LOG_FATAL, "could not load skeleton " + skeleton);
        }

        auto& bones = skeleton_->bones();
        for (auto i = 0u; i < bones.size(); ++i) {
            pose_.emplace_back(glm::inverse(bones[i].absolute_transform));
        }
    }

    animation_component::animation_component(game& game, entity& owner)
    : reflective(game, owner), current_state_{""}, elapsed_time_{0.f}, current_frame_{0} {
        game_.rendering_system().register_component(this);
        game_.animation_system().register_component(this);
    }

    void animation_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &animation_component::load);
        register_property<std::string>("skeleton", nullptr, &animation_component::load_skeleton);
    }
}