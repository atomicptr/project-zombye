#include <zombye/core/game.hpp>
#include <zombye/physics/collision_mesh.hpp>
#include <zombye/physics/collision_shape.hpp>
#include <zombye/physics/shapes/triangle_mesh_shape.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/physics/physics_system.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
	triangle_mesh_shape::triangle_mesh_shape(game& game, const std::string& file_name) {
		auto mesh = game.physics()->collision_mesh_manager().load(file_name);
		if (!mesh) {
			throw std::runtime_error{"could not load collision mesh " + file_name};
		}

		vertices_ = mesh->vertices();
		indices_ = mesh->indices();

		auto collision_mesh = new btTriangleMesh{true};
		for (auto i = 0ul; i < indices_.size(); i += 3) {
			auto& v1 = vertices_[indices_[i]].pos;
			auto& v2 = vertices_[indices_[i + 1]].pos;
			auto& v3 = vertices_[indices_[i + 2]].pos;
			collision_mesh->addTriangle(
				btVector3{v1.x, v1.y, v1.z},
				btVector3{v2.x, v2.y, v2.z},
				btVector3{v3.x, v3.y, v3.z},
				true
			);
		}

		shape_ = std::unique_ptr<btCollisionShape>{
			new btBvhTriangleMeshShape{
				collision_mesh,
				false,
			}
		};
	}

	void triangle_mesh_shape::register_at_script_engine(game& game) {
		auto& scripting_system = game.scripting_system();

		scripting_system.register_type<triangle_mesh_shape>("triangle_mesh_shape");

		static std::function<triangle_mesh_shape*(const std::string&)> factory = [&](const std::string& file_name) {
			return new triangle_mesh_shape{game, file_name};
		};
		scripting_system.register_factory("triangle_mesh_shape", "triangle_mesh_shape@ f(const string& in)",
			factory);

		scripting_system.register_member_function("entity_impl",
	        "physics_component& add_physics_component(triangle_mesh_shape@ collison_shape, bool is_static = false)",
	        +[](entity& owner, triangle_mesh_shape* collison_shape, bool is_static) -> physics_component& {
	            return owner.emplace<physics_component>(collison_shape, is_static);
	        });
	}
}
