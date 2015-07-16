#include <zombye/ecs/entity.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/aabb.hpp>
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

    std::vector<glm::mat4>&& shadow_component::calculate_crop_matricies(const std::vector<aabb> aabbs) const {
        std::vector<glm::mat4> crop_matricies;
        crop_matricies.resize(aabbs.size());

        for (auto i = size_t{0}; i < aabbs.size(); ++i) {
            auto crop_aabb = aabbs[i];
            crop_aabb.min = glm::vec3{view_projection() * glm::vec4{crop_aabb.min, 1.f}};
            crop_aabb.max = glm::vec3{view_projection() * glm::vec4{crop_aabb.max, 1.f}};

            crop_aabb.min.z = 0.f;

            auto scale_x = 2.f / (crop_aabb.max.x - crop_aabb.min.x);
            auto scale_y = 2.f / (crop_aabb.max.y - crop_aabb.min.y);
            auto scale_z = 1.f / (crop_aabb.max.z - crop_aabb.min.z);
            auto offset_x = -0.5f * (crop_aabb.max.x + crop_aabb.min.x) * scale_x;
            auto offset_y = -0.5f * (crop_aabb.max.y + crop_aabb.min.y) * scale_y;
            auto offset_z = -crop_aabb.min.z * scale_z;

            auto crop_matrix = glm::mat4{
                scale_x, 0.f, 0.f, 0.f,
                0.f, scale_y, 0.f, 0.f,
                0.f, 0.f, scale_z, 0.f,
                offset_x, offset_y, offset_z, 1.f
            };

            crop_matricies[i] = crop_matrix;
        }

        return std::move(crop_matricies);
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
