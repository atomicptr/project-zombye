#ifndef __ZOMBYE_CAMERA_COMPONENT_HPP__
#define __ZOMBYE_CAMERA_COMPONENT_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    class camera_component : public reflective<camera_component, component> {
        friend class reflective<camera_component, component>;
        glm::mat4 projection_;
    public:
        camera_component(game& game, entity& owner, const glm::mat4& projection) noexcept;
        ~camera_component() noexcept;

        auto& projection() const {
            return projection_;
        }

        void projection(const glm::mat4 projection) {
            projection_ = projection;
        }

        auto view() const {
            return glm::inverse(owner_.transform());
        }

        auto projection_view() const {
            return projection() * view();
        }

        static void register_at_script_engine(game& game);

    private:
        camera_component(game& game, entity& owner) noexcept;
    };
}

#endif
