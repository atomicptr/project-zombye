#ifndef __ZOMBYE_STATICMESH_COMPONENT_HPP__
#define __ZOMBYE_STATICMESH_COMPONENT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
    class mesh;
}

namespace zombye {
    class staticmesh_component : public reflective<staticmesh_component, component> {
        friend class reflective<staticmesh_component, component>;

        std::shared_ptr<const zombye::mesh> mesh_;
    public:
        staticmesh_component(game& game, entity& owner, const std::string& mesh);
        ~staticmesh_component() noexcept;

        void draw() const noexcept;

        auto mesh() const noexcept {
            return mesh_;
        }

        void load(const std::string& mesh);
    private:
        staticmesh_component(game& game, entity& owner);
        static void register_reflection();
    };
}

#endif
