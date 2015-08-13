#include <glm/gtc/matrix_transform.hpp>

#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/entity_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/gameplay/camera_follow_component.hpp>
#include <zombye/gameplay/gameplay_system.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    camera_follow_component::camera_follow_component(game& game, entity& owner, unsigned long target,
        float elevation, float azimuth, float distance, float min_distance, float max_distance,
        float spring_constant, float mass)
    : reflective{game, owner}, target_{target}, elevation_{elevation}, azimuth_{azimuth}, distance_{distance},
    min_distance_{min_distance}, max_distance_{max_distance}, spring_constant_{spring_constant}, mass_{mass},
    velocity_{0.f} {
        game_.gameplay()->register_component(this);
    }

    camera_follow_component::~camera_follow_component() {
        game_.gameplay()->unregister_component(this);
    }

    void camera_follow_component::update(float delta_time) {
        if (target_ == 0) {
            return;
        }

        auto target = game_.entity_manager().resolve(target_);
        if (!target) {
            return;
        }

        auto look_at = target->position();

        auto target_orientation = glm::toMat3(target->rotation());
        auto local_x = target_orientation[0];
        auto local_y = target_orientation[1];
        auto local_z = target_orientation[2];
        auto camera_pos = look_at;
        camera_pos += (distance_ * glm::sin(elevation_) * glm::cos(azimuth_)) * local_y;
        camera_pos += (distance_ * glm::cos(elevation_)) * local_z;
        camera_pos += (distance_ * glm::sin(elevation_) * glm::sin(azimuth_)) * local_x;
        auto dest_pos = camera_pos;

        auto src_dest = owner_.position();

        auto spring_force = spring_constant_ * (dest_pos - src_dest);
        auto damping = 2.f * glm::sqrt(spring_constant_);
        auto damping_force = -damping * velocity_;

        auto accel = (spring_force + damping_force) / mass_;

        auto velocity = velocity_ + delta_time * accel;
        auto pos = owner_.position() + delta_time * velocity;

        spring_force = spring_constant_ * (dest_pos - pos);
        damping = 2.f * glm::sqrt(spring_constant_);
        damping_force = -damping * velocity;

        accel = (spring_force + damping_force) / mass_;

        auto pred_velocity = velocity + delta_time * accel;

        velocity_ = 0.5f * (velocity + pred_velocity);

        auto pred_pos = owner_.position() + delta_time * velocity_;

        auto new_pos = pred_pos;
        auto local_vec = pred_pos - look_at;

        auto current_dist = glm::length(local_vec);
        if (current_dist > max_distance_) {
            new_pos = look_at + local_vec * (max_distance_ / current_dist);
        }
        owner_.position(new_pos);

        auto view = glm::lookAt(owner_.position(), look_at, glm::vec3{0.f, 1.f, 0.f});
        auto rot = glm::quat_cast(view);
        owner_.rotation(glm::inverse(rot));
    }

    void camera_follow_component::first_position(const glm::vec3& offset) {
        if (target_ == 0) {
            return;
        }

        auto target = game_.entity_manager().resolve(target_);
        if (!target) {
            return;
        }

        auto look_at = target->position();

        auto target_orientation = glm::toMat3(target->rotation());
        auto local_x = target_orientation[0];
        auto local_y = target_orientation[1];
        auto local_z = target_orientation[2];
        auto camera_pos = look_at;
        camera_pos += (distance_ * glm::sin(elevation_) * glm::cos(azimuth_)) * local_y;
        camera_pos += (distance_ * glm::cos(elevation_)) * local_z;
        camera_pos += (distance_ * glm::sin(elevation_) * glm::sin(azimuth_)) * local_x;

        owner_.position(camera_pos + offset);

        auto view = glm::lookAt(owner_.position(), look_at, target_orientation[1]);
        auto rot = glm::quat_cast(view);
        owner_.rotation(glm::inverse(rot));
    }

    void camera_follow_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<camera_follow_component>("camera_follow_component");

        scripting_system.register_member_function("camera_follow_component", "void target(uint64)",
            +[](camera_follow_component& component, unsigned long id) { component.target(id); });
        scripting_system.register_member_function("camera_follow_component", "void initial_position(const glm::vec3& in)",
            +[](camera_follow_component& component, const glm::vec3& offset) { component.first_position(offset); });

        scripting_system.register_member_function("entity_impl",
            "camera_follow_component& add_camera_follow_component()",
            +[](entity& owner) -> camera_follow_component& {
                return owner.emplace<camera_follow_component>();
            });
        scripting_system.register_member_function("entity_impl", "camera_follow_component@ get_camera_follow_component()",
            +[](entity& owner) { return owner.component<camera_follow_component>(); });
    }
}
