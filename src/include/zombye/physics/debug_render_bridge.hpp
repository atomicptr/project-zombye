#ifndef __ZOMBYE_DEBUG_RENDER_BRIDGE_HPP__
#define __ZOMBYE_DEBUG_RENDER_BRIDGE_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>

#include <zombye/physics/debug_renderer.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {

    class debug_render_bridge : public btIDebugDraw {
    public:
        debug_render_bridge(debug_renderer*);
        ~debug_render_bridge();

        void drawLine(const btVector3&, const btVector3&, const btVector3&);
        void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&);
        void draw3dText(const btVector3&, const char*);
        void drawTransform(const btTransform&, btScalar);

        void reportErrorWarning(const char*);

        void setDebugMode(int);
        int getDebugMode() const;

    private:
        debug_renderer* renderer_;

        int debug_mode_;
    };
}

#endif
