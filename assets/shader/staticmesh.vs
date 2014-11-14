#version 330

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texel;

out vec2 out_texel;

uniform mat4 mvp;

void main() {
    out_texel = in_texel;
    gl_Position = mvp * vec4(in_position, 1.0);
}