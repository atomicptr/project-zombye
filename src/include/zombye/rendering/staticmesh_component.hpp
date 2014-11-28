#ifndef __ZOMBYE_STATICMESH_COMPONENT_HPP__
#define __ZOMBYE_STATICMESH_COMPONENT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/reflective.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/rendering/texture.hpp>

namespace zombye {
    struct material {
        std::shared_ptr<const texture> color;
        std::shared_ptr<const texture> specular;
    };
    class game;
    class staticmesh_component : public reflective<staticmesh_component, component> {
        friend class reflective<staticmesh_component, component>;
        std::shared_ptr<const zombye::mesh> mesh_;
        std::vector<material> materials_;
        static void register_reflection();
        staticmesh_component(game& game, entity& owner) noexcept;
    public:
        staticmesh_component(game& game, entity& owner, const std::string& mesh);
        ~staticmesh_component() noexcept;

        void draw() const;

        auto mesh() noexcept {
            return mesh_;
        }

        void set_mesh(const std::string& name);
    };
}

#endif