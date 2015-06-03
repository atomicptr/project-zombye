#ifndef __ZOMBYE_SHADOW_COMPONENT_HPP__
#define __ZOMBYE_SHADOW_COMPONENT_HPP__

#include <glm/glm.hpp>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
}

namespace zombye {
    class shadow_component : public reflective<shadow_component, component> {
    private:
        friend class reflective<shadow_component, component>;
        glm::mat4 projection_;

    public:
        shadow_component(game& game, entity& owner, const glm::mat4& projection) noexcept;
        ~shadow_component();

        auto& projection() const  {
            return projection_;
        }

        void projection(const glm::mat4& projection) {
            projection_ = projection;
        }

        static void register_at_script_engine(game& game);

    private:
        shadow_component(game& game, entity& owner) noexcept;
        static void register_reflection() { }
    };
}

#endif
