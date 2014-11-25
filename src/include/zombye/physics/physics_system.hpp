#ifndef __ZOMBYE_PHYSICS_SYSTEM_HPP__
#define __ZOMBYE_PHYSICS_SYSTEM_HPP__

#include <btBulletDynamicsCommon.h>

#include <zombye/physics/physics_component.hpp>

namespace zombye {
    class physics_component;

    class physics_system {
        friend class physics_component;
    public:
        physics_system();
        ~physics_system();

        btDiscreteDynamicsWorld* world();

        void init();

        void update(float);

    private:
        std::unique_ptr<btDbvtBroadphase> broadphase_;
        std::unique_ptr<btDefaultCollisionConfiguration> collision_config_;
        std::unique_ptr<btCollisionDispatcher> dispatcher_;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;

        std::unique_ptr<btDiscreteDynamicsWorld> world_;

        std::vector<physics_component*> components_;

        void register_component(physics_component*);
        void unregister_component(physics_component*);
    };
}

#endif
