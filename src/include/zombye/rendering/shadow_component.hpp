#ifndef __ZOMBYE_SHADOW_COMPONENT_HPP__
#define __ZOMBYE_SHADOW_COMPONENT_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    class shadow_component : public reflective<shadow_component, component> {
    private:
        friend class reflective<shadow_component, component>;
        glm::mat4 projection_;

    public:
        shadow_component(game& game, entity& owner, const glm::mat4& projection) noexcept;
        ~shadow_component();

        auto& projection() const  {
            return projection_;
        }

        void projection(const glm::mat4& projection) {
            projection_ = projection;
        }

        auto view() const {
            auto position = owner_.position();
            position = glm::normalize(position);
            auto& rotation = owner_.rotation();
            auto up_vector = glm::rotate(rotation, glm::vec3{0.f, 1.f, 0.f});
            up_vector = glm::normalize(up_vector);
            return glm::lookAt(position, glm::vec3{0.f}, up_vector);
        }

        auto view_projection() const {
            return projection_ * view();
        }

        static void register_at_script_engine(game& game);

    private:
        shadow_component(game& game, entity& owner) noexcept;
        static void register_reflection() { }
    };
}

#endif
