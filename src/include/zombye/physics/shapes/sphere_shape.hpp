#ifndef __ZOMBYE_SPHERE_SHAPE_HPP__
#define __ZOMBYE_SPHERE_SHAPE_HPP__

#include <memory>
#include <zombye/physics/collision_shape.hpp>
#include <btBulletDynamicsCommon.h>

namespace zombye {

    class sphere_shape : public collision_shape {
    public:
        sphere_shape(float);

        btCollisionShape* shape();

    private:
        std::unique_ptr<btCollisionShape> shape_;
    };
}

#endif
