#ifndef __ZOMBYE_LIGHT_COMPONENT_HPP__
#define __ZOMBYE_LIGHT_COMPONENT_HPP__

#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class game;
    class light_component : public reflective<light_component, component> {
        friend class reflective<light_component, component>;
        glm::vec3 color_;
        float intensity_;
        static void register_reflection();
        light_component(game& game, entity& owner);
    public:
        light_component(game& game, entity& owner, const glm::vec3& color, float intensity);
        ~light_component() noexcept;

        auto color() const noexcept {
            return color_;
        }

        void set_color(const glm::vec3& color) {
            color_ = color;
        }

        auto intensity() const noexcept {
            return intensity_;
        }

        void set_intensity(const float& intensity) {
            intensity_ = intensity;
        }
    };
}

#endif