#ifndef __ZOMBYE_LIGHT_COMPONENT_HPP__
#define __ZOMBYE_LIGHT_COMPONENT_HPP__

#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
}

namespace zombye {
    class light_component : public reflective<light_component, component> {
        friend class reflective<light_component, component>;

        glm::vec3 color_;
        glm::vec3 specular_color_;
    public:
        light_component(game& game, entity& owner, const glm::vec3& color, const glm::vec3& specular_color) noexcept;
        ~light_component() noexcept;

        auto color() const noexcept {
            return color_;
        }

        void set_color(const glm::vec3& color) noexcept {
            color_ = color;
        }

        auto specular_color() const noexcept {
            return specular_color_;
        }

        void set_specular_color(const glm::vec3& specular_color) noexcept {
            specular_color_ = specular_color;
        }

    private:
        light_component(game& game, entity& owner) noexcept;
        static void register_reflection();
    };
}

#endif