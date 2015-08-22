#ifndef __ZOMBYE_PHYSICS_SYSTEM_HPP__
#define __ZOMBYE_PHYSICS_SYSTEM_HPP__

#include <btBulletDynamicsCommon.h>

#include <zombye/physics/collision_mesh_manager.hpp>

namespace zombye {
    class game;
    class debug_renderer;
    class debug_render_bridge;
    class physics_component;
    class character_physics_component;
}

namespace zombye {
    class physics_system {
        friend class physics_component;
        friend class character_physics_component;
    public:
        physics_system(game& game);
        ~physics_system();

        btDiscreteDynamicsWorld* world();

        void update(float);
        void debug_draw();

        void toggle_debug();
        void enable_debug();
        void disable_debug();

        auto& broadphase() noexcept {
            return *broadphase_;
        }

        auto& collision_mesh_manager() noexcept {
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
        std::vector<character_physics_component*> character_physics_components_;

        std::unique_ptr<debug_render_bridge> bt_debug_drawer_;
        std::unique_ptr<debug_renderer> debug_renderer_;

        void register_component(physics_component*);
        void unregister_component(physics_component*);
        void register_component(character_physics_component* component);
        void unregister_component(character_physics_component* component);
    };
}

#endif
