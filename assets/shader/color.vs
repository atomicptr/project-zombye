#version 330

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texel;

out vec4 out_color;

uniform mat4 vp;

void main() {
    out_color = vec4(in_texel, 1.0, 1.0);
    gl_Position = vp * vec4(in_position, 1.0);
}