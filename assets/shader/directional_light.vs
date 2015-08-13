#version 140

in vec3 _position;
in vec2 _texcoord;

out vec2 texcoord_;

uniform mat4 projection;

void main() {
    texcoord_ = _texcoord;
    gl_Position = projection * vec4(_position, 1.f);
}
