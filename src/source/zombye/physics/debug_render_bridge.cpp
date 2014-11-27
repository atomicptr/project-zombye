#include <zombye/physics/debug_render_bridge.hpp>

zombye::debug_render_bridge::debug_render_bridge(debug_renderer* renderer) : renderer_(renderer), debug_mode_(0) {}

zombye::debug_render_bridge::~debug_render_bridge() {}

void zombye::debug_render_bridge::drawLine(const btVector3 &btfrom, const btVector3 &btto, const btVector3 &btcolor) {
    static auto from = glm::vec3{};
    static auto to = glm::vec3{};
    static auto color = glm::vec3{};

    from.x = btfrom.x();
    from.y = btfrom.y();
    from.z = btfrom.z();

    to.x = btto.x();
    to.y = btto.y();
    to.z = btto.z();

    color.x = btcolor.x();
    color.y = btcolor.y();
    color.z = btcolor.z();

    renderer_->draw_line(from, to, color);
}

void zombye::debug_render_bridge::drawContactPoint(const btVector3 &btpoint, const btVector3 &btnormal, btScalar btdistance, int lifetime, const btVector3 &btcolor) {
    static auto point = glm::vec3{};
    static auto normal = glm::vec3{};
    static auto color = glm::vec3{};

    auto distance = float{btdistance};

    point.x = btpoint.x();
    point.y = btpoint.y();
    point.z = btpoint.z();

    normal.x = btnormal.x();
    normal.y = btnormal.y();
    normal.z = btnormal.z();

    color.x = btcolor.x();
    color.y = btcolor.y();
    color.z = btcolor.z();

    renderer_->draw_contact_point(point, normal, distance, lifetime, color);
}

void zombye::debug_render_bridge::draw3dText(const btVector3&, const char*) {

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
