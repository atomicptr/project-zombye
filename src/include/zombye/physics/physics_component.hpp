#ifndef __ZOMBYE_PHYSICS_COMPONENT_HPP__
#define __ZOMBYE_PHYSICS_COMPONENT_HPP__

#include <memory>
#include <math.h>

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include <zombye/core/game.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>

#include <zombye/physics/physics_system.hpp>

namespace zombye {
    class physics_system;

    class physics_component : public component {
    public:
        physics_component(game&, entity&, btCollisionShape*, bool isstatic=false);
        ~physics_component();

        void sync() const;
    private:
        physics_system* physics_;
        btDiscreteDynamicsWorld* world_;

        std::unique_ptr<btRigidBody> body_;
        std::unique_ptr<btDefaultMotionState> motion_state_;
    };
}

#endif
