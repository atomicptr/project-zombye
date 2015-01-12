#ifndef __ZOMBYE_CAMERA_COMPONENT_HPP__
#define __ZOMBYE_CAMERA_COMPONENT_HPP__

#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
}

namespace zombye {
    class camera_component : public reflective<camera_component, component> {
        friend class reflective<camera_component, component>;
        glm::vec3 look_at_;
        glm::vec3 up_;
    public:
        camera_component(game& game, entity& owner, const glm::vec3& look_at, const glm::vec3& up) noexcept;
        ~camera_component() noexcept;

        glm::mat4 transform() const;

        glm::vec3 look_at() const {
            return look_at_;
        }

        void set_look_at(const glm::vec3& look_at) {
            look_at_ = look_at;
        }

        glm::vec3 up() const {
            return up_;
        }

        void set_up(const glm::vec3& up) {
            up_ = up;
        }

    private:
        camera_component(game& game, entity& owner) noexcept;
    };
}

#endif