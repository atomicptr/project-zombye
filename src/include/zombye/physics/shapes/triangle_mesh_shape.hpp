#ifndef __ZOMBYE_TRIANGLE_MESH_SHAPE_HPP__
#define __ZOMBYE_TRIANGLE_MESH_SHAPE_HPP__

#include <memory>
#include <vector>

#include <btBulletCollisionCommon.h>

#include <zombye/physics/collision_shape.hpp>

namespace zombye {
	class collision_mesh;
	class game;
	struct collision_vertex;
}

namespace zombye {
	class triangle_mesh_shape : public collision_shape {
	private:
		std::unique_ptr<btCollisionShape> shape_;
		std::vector<collision_vertex> vertices_;
		std::vector<unsigned int> indices_;

	public:
		triangle_mesh_shape(game& game, const std::string& file_name);

		btCollisionShape* shape() {
			return shape_.get();
		}

		static void register_at_script_engine(game& game);
	};
}

#endif
