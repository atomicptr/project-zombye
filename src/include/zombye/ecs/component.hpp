#ifndef __ZOMBYE_COMPONENT_HPP__
#define __ZOMBYE_COMPONENT_HPP__

#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class game;
    class entity;
    class component {
    protected:
        game& game_;
        entity& owner_;
    public:
        component(game& game, entity& owner) noexcept;
        component(const component& other) = delete;
        component(component&& other) = delete;
        virtual ~component() noexcept = 0;
        entity& owner() const noexcept {
            return owner_;
        }
        component& operator= (const component& other) = delete;
        component& operator= (component&& other) = delete;
    };
}

#endif
