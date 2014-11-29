#ifndef __ZOMBYE_BOX_SHAPE_HPP__
#define __ZOMBYE_BOX_SHAPE_HPP__

#include <memory>

#include <zombye/physics/collision_shape.hpp>

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

namespace zombye {
    class box_shape : public collision_shape {
    public:
        box_shape(float, float, float);
        box_shape(glm::vec3&);

        btCollisionShape* shape();

    private:
        std::unique_ptr<btCollisionShape> shape_;
    };
}

#endif
