#ifndef __ZOMBYE_PHYSICS_SYSTEM_HPP__
#define __ZOMBYE_PHYSICS_SYSTEM_HPP__

#include <btBulletDynamicsCommon.h>

#include <zombye/physics/physics_component.hpp>
#include <zombye/physics/debug_renderer.hpp>
#include <zombye/physics/debug_render_bridge.hpp>

namespace zombye {
    class game;
    class physics_component;

    class physics_system {
        friend class physics_component;
    public:
        physics_system(game& game);
        ~physics_system();

        btDiscreteDynamicsWorld* world();

        void update(float);
        void debug_draw();

    private:
        game& game_;
        std::unique_ptr<btDbvtBroadphase> broadphase_;
        std::unique_ptr<btDefaultCollisionConfiguration> collision_config_;
        std::unique_ptr<btCollisionDispatcher> dispatcher_;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;

        std::unique_ptr<btDiscreteDynamicsWorld> world_;

        std::vector<physics_component*> components_;

        std::unique_ptr<debug_renderer> debug_drawer_; // TODO: change this
        std::unique_ptr<debug_render_bridge> bt_debug_drawer_;

        void register_component(physics_component*);
        void unregister_component(physics_component*);
    };
}

#endif
