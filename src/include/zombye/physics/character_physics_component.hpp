#ifndef __ZOMBYE_CHARACTER_PHYSICS_COMPONENT_HPP__
#define __ZOMBYE_CHARACTER_PHYSICS_COMPONENT_HPP__

#include <memory>

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class collision_shape;
    class entity;
    class game;
    class physics_system;
}

namespace zombye {
    class character_physics_component : public reflective<character_physics_component, component> {
        friend class reflective<character_physics_component, component>;

    private:
        physics_system& physics_;
        btDiscreteDynamicsWorld& world_;
        std::unique_ptr<btPairCachingGhostObject> ghost_object_;
        std::unique_ptr<collision_shape> collision_shape_;
        std::unique_ptr<btKinematicCharacterController> character_controller_;

    public:
        character_physics_component(game& game, entity& owner, collision_shape* collision_shape);
        ~character_physics_component();

        void update(float delta_time);
        void sync();
        void linear_velocity(const glm::vec3& velocity);

        static void register_at_script_engine(game& game);

    private:
        character_physics_component(game& game, entity& owner);
    };
}

#endif
