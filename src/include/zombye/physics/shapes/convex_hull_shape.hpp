#ifndef __ZOMBYE_CONVEX_HULL_SHAPE_HPP__
#define __ZOMBYE_CONVEX_HULL_SHAPE_HPP__

#include <memory>
#include <string>

#include <btBulletDynamicsCommon.h>

#include <zombye/physics/collision_shape.hpp>

namespace zombye {
    class collision_mesh;
}

namespace zombye {

    class convex_hull_shape : public collision_shape {
    public:
        convex_hull_shape(std::shared_ptr<const collision_mesh>);

        btCollisionShape* shape();

    private:
        std::unique_ptr<btCollisionShape> shape_;
        std::shared_ptr<const collision_mesh> mesh_;
    };
}

#endif
