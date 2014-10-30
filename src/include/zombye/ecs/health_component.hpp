#ifndef __ZOMBYE_HEALTH_COMPONENT_HPP__
#define __ZOMBYE_HEALTH_COMPONENT_HPP__

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class health_component : public reflective<health_component, component> {
        float health_;
        float max_health_;
    public:
        health_component(game& game, entity& owner) noexcept
        : reflective(game, owner) { }
        health_component(game& game, entity& owner, float health, float max_health) noexcept
        : reflective(game, owner), health_(health), max_health_(max_health) { }
        const float& health() const {
            return health_;
        }
        void set_health(const float& health) {
            health_ = health;
        }
        const float& max_health() const {
            return max_health_;
        }
        void set_max_health(const float& max_health) {
            max_health_ = max_health;
        }
        static void register_reflection() {
            register_property<float>("health", &health_component::health, &health_component::set_health);
            register_property<float>("max_health", &health_component::max_health, &health_component::set_max_health);
        }
    };
}

#endif
