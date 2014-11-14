#ifndef __ZOMBYE_STATICMESH_COMPONENT_HPP__
#define __ZOMBYE_STATICMESH_COMPONENT_HPP__

#include <memory>
#include <string>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/rendering/texture.hpp>

namespace zombye {
    class game;
    class staticmesh_component : public reflective<staticmesh_component, component> {
        friend class reflective<staticmesh_component, component>;
        std::shared_ptr<const zombye::mesh> mesh_;
        std::shared_ptr<const texture> diffuse_texture_;
        static void register_reflection();
        staticmesh_component(game& game, entity& owner) noexcept;
    public:
        staticmesh_component(game& game, entity& owner, const std::string& mesh,
        const std::string& diffuse_texture);
        ~staticmesh_component() noexcept;

        auto mesh() noexcept {
            return mesh_;
        }

        void set_mesh(const std::string& name);

        auto diffuse_texture() {
            return diffuse_texture_;
        }

        void set_diffuse_texture(const std::string& name);
    };
}

#endif