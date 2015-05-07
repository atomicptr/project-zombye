#include <zombye/physics/debug_renderer.hpp>
#include <zombye/physics/debug_render_bridge.hpp>
#include <zombye/utils/logger.hpp>

zombye::debug_render_bridge::debug_render_bridge(debug_renderer& debug_renderer)
: debug_mode_(0), debug_renderer_{debug_renderer} {}

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

    if(debug_mode_ != 0) {
        debug_renderer_.buffer_line(from, to, color);
    }
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

    if(debug_mode_ != 0) {
        debug_renderer_.buffer_contact_point(point, normal, distance, lifetime, color);
    }
}

void zombye::debug_render_bridge::drawTransform(const btTransform &transform, btScalar ortho_length) {
    auto start = transform.getOrigin();

    drawLine(start, start + transform.getBasis() * btVector3(ortho_length, 0, 0), btVector3(0.7f, 0, 0));
    drawLine(start, start + transform.getBasis() * btVector3(0, ortho_length, 0), btVector3(0, 0.7f, 0));
    drawLine(start, start + transform.getBasis() * btVector3(0, 0, ortho_length), btVector3(0, 0, 0.7f));
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
