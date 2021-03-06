#ifndef __ZOMBYE_ANIMATION_COMPONENT_HPP__
#define __ZOMBYE_ANIMATION_COMPONENT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
    class skeleton;
    class skinned_mesh;
    class texture;
    struct track;
}

namespace zombye {
    class animation_component : public reflective<animation_component, component> {
        struct frame {
            int tkey = 0;
            int qkey = 0;
            int skey = 0;
        };

        friend class reflective<animation_component, component>;

        std::shared_ptr<const skinned_mesh> mesh_;
        std::shared_ptr<const zombye::skeleton> skeleton_;
        std::string current_state_;
        const char* c_str_;
        std::string next_state_;
        float elapsed_time_;
        std::vector<frame> current_keyframes_;
        std::vector<frame> next_keyframes_;
        std::vector<glm::mat4> pose_;
        bool blend_;
        bool blend_next_;
    public:
        animation_component(game& game, entity& owner, const std::string& mesh, const std::string& skeleton);
        ~animation_component() noexcept;

        void draw() const noexcept;

        void update(float delta_time);

        auto mesh() const noexcept {
            return mesh_;
        }

        void load(const std::string& mesh);

        auto skeleton() const noexcept {
            return skeleton_;
        }

        void load_skeleton(const std::string& skeleton);

        auto& pose() const noexcept {
            return pose_;
        }

        void change_state(const std::string& state) {
            current_state_ = state;
            c_str_ = current_state_.c_str();
            current_keyframes_.assign(current_keyframes_.size(), frame{});
            next_keyframes_.assign(next_keyframes_.size(), frame{});
            elapsed_time_ = 0.f;
        }

        void change_state_blend(const std::string& state_b) {
            next_state_ = state_b;
            next_keyframes_.assign(next_keyframes_.size(), frame{});
            blend_next_ = true;
        }

        bool is_playing(const std::string& state) const {
            return current_state_ == state;
        }

        bool is_blending() const {
            return blend_;
        }

        static void register_at_script_engine(game& game);

    private:
        glm::vec3 interpolate_translation_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps);
        glm::quat interpolate_quaternion_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps);

        animation_component(game& game, entity& owner);
        static void register_reflection();
    };
}

#endif
