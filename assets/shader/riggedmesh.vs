#version 330

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texel;
in ivec4 in_bone_index;
in vec4 in_weight;

out vec2 texel;

uniform mat4 mvp;

void main() {
    texel = in_texel;
    gl_Position = mvp * vec4(in_position, 1.0);
}