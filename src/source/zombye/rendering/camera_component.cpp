#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <zombye/config/config_system.hpp>
#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    camera_component::camera_component(game& game, entity& owner, float fov, float width, float height, float near, float far) noexcept
    : reflective{game, owner}, near_{near}, far_{far} {
        game_.rendering_system().register_component(this);

        projection_ = glm::perspectiveFov(fov, width, height, near, far);

        auto config = game.config();
        auto quality_level = config->get("main", "quality").asString();
        auto quality = config->get("quality", quality_level);

        auto splits = quality["shadow_frustum_splits"].asInt();
        auto m = splits + 1;
        split_planes_.resize(m + 1);

        auto lambda = config->get("main", "shadow_frustum_split_weight").asFloat();
        for (auto i = 0ul; i < split_planes_.size(); ++i) {
            auto c_i_log = near_ * pow((far_ / near_), (static_cast<float>(i) / static_cast<float>(m)));
            auto c_i_uni = near_ + (far_ - near_) * (static_cast<float>(i) / static_cast<float>(m));
            split_planes_[i] = lambda * c_i_log + (1.f - lambda) * c_i_uni;
            std::cout << split_planes_[i] << std::endl;
        }

        auto view_matrix = view();
        auto top_right = glm::vec4{1.f, 1.f, 1.f, 1.f};
        auto bottom_left = glm::vec4{-1.f, -1.f, -1.f, 1.f};

        sub_frusta_bbs_.resize(m);
        for (auto i = 0; i < m; ++i) {
            auto projection = glm::perspectiveFov(fov, width, height, split_planes_[i], split_planes_[i + 1]);
            auto projection_inv = glm::inverse(projection);

            auto top_right_p = projection_inv * top_right;
            top_right_p /= glm::vec4{top_right_p.w};

            auto bottom_left_p = projection_inv * bottom_left;
            bottom_left_p /= glm::vec4{bottom_left_p.w};
            bottom_left_p.x = -top_right_p.x;
            bottom_left_p.y = -top_right_p.y;

            bounding_box bb;
            bb.points[0] = glm::vec3{bottom_left_p};
            bb.points[1] = glm::vec3{bottom_left_p} + glm::vec3{top_right_p.x, 0.f, 0.f};
            bb.points[2] = glm::vec3{bottom_left_p} + glm::vec3{0.f, top_right_p.y, 0.f};
            bb.points[3] = glm::vec3{bottom_left_p} + glm::vec3{top_right_p.x, top_right_p.y, 0.f};
            bb.points[4] = glm::vec3{top_right_p};
            bb.points[5] = glm::vec3{top_right_p} + glm::vec3{bottom_left_p.x, 0.f, 0.f};
            bb.points[6] = glm::vec3{top_right_p} + glm::vec3{0.f, bottom_left_p.y, 0.f};
            bb.points[7] = glm::vec3{top_right_p} + glm::vec3{bottom_left_p.x, bottom_left_p.y, 0.f};

            std::cout << glm::to_string(bottom_left_p) << std::endl;
            std::cout << glm::to_string(top_right_p) << std::endl << std::endl;

            sub_frusta_bbs_[i] = bb;
        }
    }

    camera_component::camera_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, near_{0.f}, far_{0.f}, projection_{glm::mat4{1.f}} {}

    camera_component::~camera_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void camera_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<camera_component>("camera_component");

        scripting_system.register_member_function("entity_impl",
            "camera_component& add_camera_component(float fov, float width, float height, float near, float far)",
            +[](entity& owner, float fov, float width, float height, float near, float far) -> camera_component& {
                return owner.emplace<camera_component>(fov, width, height, near, far);
            });
        scripting_system.register_member_function("entity_impl", "camera_component@ get_camera_component()",
            +[](entity& owner) { return owner.component<camera_component>(); });
    }
}
