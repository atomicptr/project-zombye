#ifndef __ZOMBYE_DEBUG_RENDERER_HPP__
#define __ZOMBYE_DEBUG_RENDERER_HPP__

#include <glm/glm.hpp>

namespace zombye {
    class debug_renderer {
    public:
        virtual ~debug_renderer() {}

        // from, to, color
        virtual void draw_line(glm::vec3&, glm::vec3&, glm::vec3&) = 0;

        // pointOnB, normalOnB, distance, lifetime, color
        virtual void draw_contact_point(glm::vec3&, glm::vec3&, float, int, glm::vec3&) = 0;
    };
}

#endif
