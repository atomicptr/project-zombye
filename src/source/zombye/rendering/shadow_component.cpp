#include <limits>

#include <glm/gtx/string_cast.hpp>

#include <zombye/ecs/entity.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/bounding_box.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/shadow_component.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    shadow_component::shadow_component(game& game, entity& owner, const glm::mat4& projection) noexcept
    : reflective{game, owner}, projection_{projection} {
        game_.rendering_system().register_component(this);
    }

    shadow_component::~shadow_component() {
        game_.rendering_system().unregister_component(this);
    }

    std::vector<glm::mat4> shadow_component::calculate_crop_matricies(const std::vector<bounding_box> bbs) {
        std::vector<glm::mat4> crop_matricies;
        crop_matricies.resize(bbs.size());

        for (auto i = size_t{0}; i < bbs.size(); ++i) {
            auto min_x = std::numeric_limits<float>::max();
            auto max_x = std::numeric_limits<float>::min();
            auto min_y = std::numeric_limits<float>::max();
            auto max_y = std::numeric_limits<float>::min();
            auto min_z = std::numeric_limits<float>::max();
            auto max_z = std::numeric_limits<float>::min();

            auto crop_bb = bbs[i];
            for (auto& p : crop_bb.points) {
                auto point = view_projection() * glm::vec4{p, 1.f};
                point /= point.w;
                if (point.x < min_x) {
                    min_x = point.x;
                }
                if (point.x > max_x) {
                    max_x = point.x;
                }
                if (point.y < min_y) {
                    min_y = point.y;
                }
                if (point.y > max_y) {
                    max_y = point.y;
                }
                if (point.z < min_z) {
                    min_z = point.z;
                }
                if (point.z > max_z) {
                    max_z = point.z;
                }
            }

            auto min = glm::vec3{min_x, min_y, -1.f};
            auto max = glm::vec3{max_x, max_y, max_z};

            auto scale_x = 2.f / (max.x - min.x);
            auto scale_y = 2.f / (max.y - min.y);
            auto scale_z = 2.f / (max.z - min.z);
            auto offset_x = -0.5f * (max.x + min.x) * scale_x;
            auto offset_y = -0.5f * (max.y + min.y) * scale_y;
            auto offset_z = -0.5f * (max.z + min.z) * scale_z;

            crop_matricies[i] = glm::mat4{
                scale_x, 0.f, 0.f, 0.f,
                0.f, scale_y, 0.f, 0.f,
                0.f, 0.f, scale_z, 0.f,
                offset_x, offset_y, offset_z, 1.f
            };
        }

        return crop_matricies;
    }

    void shadow_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<shadow_component>("shadow_component");

        scripting_system.register_member_function("entity_impl",
            "shadow_component& add_shadow_component(const glm::mat4& in)",
            +[](entity& owner, const glm::mat4& projection) -> shadow_component& {
                return owner.emplace<shadow_component>(projection);
            });
        scripting_system.register_member_function("entity_impl", "shadow_component@ get_shadow_component()",
            +[](entity& owner) { return owner.component<shadow_component>(); });
    }

    shadow_component::shadow_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, projection_{1.f} {
        game_.rendering_system().register_component(this);
    }
}
