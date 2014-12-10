#version 130

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texel;
in ivec4 in_bone_index;
in vec4 in_weight;

out vec2 texel;
out vec3 n;
out vec3 v;
out vec3 world_position;

uniform int light_count;
uniform vec3 light_position[16];
uniform vec3 view;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 mit;

void main() {
    n = (mit * vec4(in_normal, 0.0)).xyz;
    world_position = (m * vec4(in_position, 1.0)).xyz;
    v = view - world_position;

    texel = in_texel;
    gl_Position = mvp * vec4(in_position, 1.0);
}
