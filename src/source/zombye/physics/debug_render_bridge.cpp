#include <zombye/physics/debug_render_bridge.hpp>

zombye::debug_render_bridge::debug_render_bridge(debug_renderer* renderer) : renderer_(renderer), debug_mode_(0) {}

zombye::debug_render_bridge::~debug_render_bridge() {}

void zombye::debug_render_bridge::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    auto glm_from = glm::vec3{from.x(), from.y(), from.z()};
    auto glm_to = glm::vec3{to.x(), to.y(), to.z()};
    auto r = float{color.x()};
    auto g = float{color.y()};
    auto b = float{color.z()};

    renderer_->draw_line(glm_from, glm_to, r, g, b);
}

void zombye::debug_render_bridge::drawContactPoint(const btVector3 &point, const btVector3 &normal, btScalar distance, int lifetime, const btVector3 &color) {
    auto glm_point = glm::vec3{point.x(), point.y(), point.z()};
    auto glm_normal = glm::vec3{normal.x(), normal.y(), normal.z()};
    auto dist = float{distance};
    auto r = float{color.x()};
    auto g = float{color.y()};
    auto b = float{color.z()};

    renderer_->draw_contact_point(glm_point, glm_normal, dist, lifetime, r, g, b);
}

void zombye::debug_render_bridge::reportErrorWarning(const char *msg) {
    log(LOG_ERROR, std::string{msg});
}

void zombye::debug_render_bridge::setDebugMode(int state) {
    debug_mode_ = state;
}

int zombye::debug_render_bridge::getDebugMode() const {
    return debug_mode_;
}
