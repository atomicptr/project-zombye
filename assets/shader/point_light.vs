#version 140

in vec3 _position;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(_position, 1.f);
}
