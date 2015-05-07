#ifndef __ZOMBYE_PHYSICS_COMPONENT_HPP__
#define __ZOMBYE_PHYSICS_COMPONENT_HPP__

#include <memory>
#include <math.h>

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class collision_shape;
    class entity;
    class game;
    class physics_system;
}

namespace zombye {
    class physics_component : public reflective<physics_component, component> {
        friend class reflective<physics_component, component>;

        physics_component(game& game, entity& owner);
    public:
        physics_component(game&, entity&, collision_shape*, bool isstatic=false);
        ~physics_component();

        void sync() const;
    private:
        physics_system* physics_;
        btDiscreteDynamicsWorld* world_;

        std::unique_ptr<btRigidBody> body_;
        std::unique_ptr<btDefaultMotionState> motion_state_;
        std::unique_ptr<collision_shape> colshape_;
    };
}

#endif
