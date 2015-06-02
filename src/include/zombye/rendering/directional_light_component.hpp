#ifndef __ZOMBYE_DIRECTIONAL_LIGHT_COMPONENT_HPP__
#define __ZOMBYE_DIRECTIONAL_LIGHT_COMPONENT_HPP__

#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
}

namespace zombye {
    class directional_light_component : public reflective<directional_light_component, component> {
        friend class reflective<directional_light_component, component>;

        glm::vec4 direction_;
        glm::vec3 color_;
        float energy_;
    public:
        directional_light_component(game& game, entity& owner, const glm::vec3& color, float energy) noexcept;
        ~directional_light_component() noexcept;

        auto& color() const noexcept {
            return color_;
        }

        void color(const glm::vec3& value) noexcept {
            color_ = value;
        }

        auto& energy() const noexcept {
            return energy_;
        }

        void energy(float value) {
            energy_ = value;
        }

        static void register_at_script_engine(game& game);

    private:
        directional_light_component(game& game, entity& owner) noexcept;
        static void register_reflection();
    };
}

#endif
