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
        std::unique_ptr<collision_shape> collision_shape_;
        std::unique_ptr<btKinematicCharacterController> character_controller_;
        float max_velocity_;
        float max_angular_velocity_;
        float current_velocity_;
        float current_angular_velocity_;
        glm::vec3 direction_;

    public:
        character_physics_component(game& game, entity& owner, collision_shape* collision_shape, float max_velocity, float max_angular_velocity);
        ~character_physics_component();

        void update(float delta_time);
        void sync();
        void linear_velocity(const glm::vec3& velocity);

        float max_velocity() const {
            return max_velocity_;
        }

        void max_velocity(float max_velocity) {
            max_velocity_ = max_velocity;
        }

        float max_angular_velocity() const {
            return max_angular_velocity_;
        }

        void max_angular_velocity(float max_angular_velocity) {
            max_angular_velocity_ = max_angular_velocity;
        }

        float velocity() const {
            return current_velocity_;
        }

        void velocity(float velocity) {
            current_velocity_ = velocity < max_velocity_ ? velocity : max_velocity_;
        }

        float angular_velocity() const {
            return current_angular_velocity_;
        }

        void angular_velocity(float angular_velocity) {
            current_angular_velocity_ = angular_velocity < max_angular_velocity_ ? angular_velocity : max_angular_velocity_;
        }

        const glm::vec3& direction() const {
            return direction_;
        }

        static void register_at_script_engine(game& game);

    private:
        character_physics_component(game& game, entity& owner);
    };
}

#endif
