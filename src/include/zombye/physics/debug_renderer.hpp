#ifndef __ZOMBYE_DEBUG_RENDERER_HPP__
#define __ZOMBYE_DEBUG_RENDERER_HPP__

#include <glm/glm.hpp>

namespace zombye {
    class debug_renderer {
    public:
        virtual ~debug_renderer() {}

        // from, to, r, g, b
        virtual void draw_line(glm::vec3, glm::vec3, float, float, float) = 0;

        // pointOnB, normalOnB, distance, lifetime, r, g, b
        virtual void draw_contact_point(glm::vec3, glm::vec3, float, int, float, float, float) = 0;
    };
}

#endif
