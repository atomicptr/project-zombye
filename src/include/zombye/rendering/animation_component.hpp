#ifndef __ZOMBYE_ANIMATION_COMPONENT_HPP__
#define __ZOMBYE_ANIMATION_COMPONENT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/skinned_mesh.hpp>
#include <zombye/rendering/skeleton.hpp>
#include <zombye/rendering/texture.hpp>

namespace zombye {
    class entity;
    class game;
}

namespace zombye {
    class animation_component : public reflective<animation_component, component> {
        friend class reflective<animation_component, component>;

        std::shared_ptr<const skinned_mesh> mesh_;
        std::vector<material> materials_;
        std::shared_ptr<const zombye::skeleton> skeleton_;
        std::string current_state_;
        float elapsed_time_;
        int current_frame_;
        std::vector<glm::mat4> pose_;
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
            current_frame_ = 0;
            elapsed_time_ = 0.f;
        }
    private:
        animation_component(game& game, entity& owner);
        static void register_reflection();
    };
}

#endif