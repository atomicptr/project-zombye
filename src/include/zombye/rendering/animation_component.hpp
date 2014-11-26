#ifndef __ZOMBYE_ANIMATION_COMPONENT_HPP__
#define __ZOMBYE_ANIMATION_COMPONENT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>
#include <zombye/rendering/animation.hpp>
#include <zombye/rendering/rigged_mesh.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/texture.hpp>

namespace zombye {
    class game;
    class animation_component : public reflective<animation_component, component> {
        friend class reflective<animation_component, component>;
        std::shared_ptr<const animation> animation_;
        std::shared_ptr<const zombye::rigged_mesh> mesh_;
        std::vector<material> materials_;
        static void register_reflection();
        animation_component(game& game, entity& owner) noexcept;
    public:
        animation_component(game& game, entity& owner, const std::string& mesh);
        ~animation_component() noexcept;

        void draw() const;

        auto mesh() noexcept {
            return mesh_;
        }

        void set_rigged_mesh(const std::string& name);

        auto get_animation() noexcept {
            return animation_;
        }

        void set_animation(const std::string& name);
    };
}

#endif