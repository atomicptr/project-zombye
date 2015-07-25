#ifndef __ZOMBYE_CAMERA_COMPONENT_HPP__
#define __ZOMBYE_CAMERA_COMPONENT_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>
#include <zombye/rendering/bounding_box.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    class camera_component : public reflective<camera_component, component> {
        friend class reflective<camera_component, component>;

    private:
        float near_;
        float far_;
        glm::mat4 projection_;
        std::vector<float> split_planes_;
        std::vector<bounding_box> sub_frusta_bbs_;

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

        auto sub_frusta_bbs() const {
            std::vector<bounding_box> sub_frusta_bbs = sub_frusta_bbs_;

            auto model_matrix = owner_.transform();
            for (auto& bb : sub_frusta_bbs) {
                for (auto& point : bb.points) {
                    point =  glm::vec3{model_matrix * glm::vec4{point, 1.f}};
                }
            }

            return sub_frusta_bbs;
        }

        static void register_at_script_engine(game& game);

    private:
        camera_component(game& game, entity& owner) noexcept;
    };
}

#endif
