#ifndef __ZOMBYE_CONVEX_HULL_SHAPE_HPP__
#define __ZOMBYE_CONVEX_HULL_SHAPE_HPP__

#include <memory>

#include <btBulletDynamicsCommon.h>

#include <zombye/physics/collision_shape.hpp>
#include <zombye/rendering/mesh.hpp>

namespace zombye {

    class convex_hull_shape : public collision_shape {
    public:
        convex_hull_shape(mesh*);

        btCollisionShape* shape();

    private:
        std::unique_ptr<btCollisionShape> shape_;
    };
}

#endif
