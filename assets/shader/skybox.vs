#version 140

in vec3 _position;

out vec3 color_;

uniform mat4 mvp;
uniform float sun_intensity;

void main() {
    if (_position.y >= 1.f) {
        color_ = vec3(0.4, 0.6, 0.9);
    } else {
        color_ = vec3(0.57, 0.62, 0.69);
    }
    color_ *= sun_intensity;
    gl_Position = mvp * vec4(_position, 1.0);
}
