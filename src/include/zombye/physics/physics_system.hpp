#ifndef __ZOMBYE_PHYSICS_SYSTEM_HPP__
#define __ZOMBYE_PHYSICS_SYSTEM_HPP__

#include <btBulletDynamicsCommon.h>

#include <zombye/physics/collision_mesh_manager.hpp>
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

        void toggle_debug();
        void enable_debug();
        void disable_debug();

        auto& collision_mesh_manager() const noexcept {
            return collision_mesh_manager_;
        }

    private:
        game& game_;
        zombye::collision_mesh_manager collision_mesh_manager_;
        std::unique_ptr<btDbvtBroadphase> broadphase_;
        std::unique_ptr<btDefaultCollisionConfiguration> collision_config_;
        std::unique_ptr<btCollisionDispatcher> dispatcher_;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;

        std::unique_ptr<btDiscreteDynamicsWorld> world_;

        std::vector<physics_component*> components_;

        std::unique_ptr<debug_render_bridge> bt_debug_drawer_;
        std::unique_ptr<debug_renderer> debug_renderer_;

        void register_component(physics_component*);
        void unregister_component(physics_component*);
    };
}

#endif
