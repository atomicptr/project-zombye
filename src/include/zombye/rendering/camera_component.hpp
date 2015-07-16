#ifndef __ZOMBYE_CAMERA_COMPONENT_HPP__
#define __ZOMBYE_CAMERA_COMPONENT_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class game;
    struct aabb;
}

namespace zombye {
    class camera_component : public reflective<camera_component, component> {
        friend class reflective<camera_component, component>;

    private:
        float near_;
        float far_;
        glm::mat4 projection_;
        std::vector<float> split_planes_;
        std::vector<aabb> sub_frusta_aabbs_;

    public:
        camera_component(game& game, entity& owner, float fov, float width, float height, float near, float far) noexcept;
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

        auto near() const  {
            return near_;
        }

        auto far() const {
            return far_;
        }

        auto& split_planes() const {
            return split_planes_;
        }

        auto& sub_frusta_aabbs() const {
            return sub_frusta_aabbs_;
        }

        static void register_at_script_engine(game& game);

    private:
        camera_component(game& game, entity& owner) noexcept;
    };
}

#endif
