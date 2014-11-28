#version 330

in vec3 in_position;
in vec3 in_normal;
in vec2 in_texel;

out vec2 out_texel;
out vec3 n;
out vec3 l;
out vec3 v;

uniform vec3 light_position;
uniform vec3 view;

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 mit;

void main() {
    n = (mit * vec4(in_normal, 0.0)).xyz;
    vec3 world_position = (m * vec4(in_position, 1.0)).xyz;
    l = (light_position - world_position);
    v = (view - world_position);

    out_texel = in_texel;
    gl_Position = mvp * vec4(in_position, 1.0);
}