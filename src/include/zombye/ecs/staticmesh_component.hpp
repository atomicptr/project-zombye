#ifndef __ZOMBYE_STATICMESH_COMPONENT_HPP__
#define __ZOMBYE_STATICMESH_COMPONENT_HPP__

#include <string>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class staticmesh_component : public reflective<staticmesh_component, component> {
        std::string file_;
    public:
        staticmesh_component(game& game, entity& owner) noexcept : reflective(game, owner), file_() { }
        staticmesh_component(game& game, entity& owner, const std::string& file)
        : reflective(game, owner), file_(file) { }
        const std::string& file() const noexcept {
            return file_;
        }
        static void register_reflection() {
            register_property<std::string>("file", &staticmesh_component::file, nullptr);
        }
    };
}

#endif