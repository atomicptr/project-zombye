#ifndef __ZOMBYE_COLLISION_SHAPE_HPP__
#define __ZOMBYE_COLLISION_SHAPE_HPP__

#include <memory>
#include <utility>

#include <btBulletDynamicsCommon.h>

namespace zombye {

    class collision_shape {
    public:
        virtual btCollisionShape* shape() = 0;
    };
}

#endif
