#version 140

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 f_texcoord;
out vec3 f_normal;
out vec3 f_world_position;

uniform mat4 m;
uniform mat4 mit;
uniform mat4 mvp;

void main() {
    f_texcoord = texcoord;
    f_normal = (mit * vec4(normal, 0.0)).xyz;
    f_world_position = (m * vec4(position, 1.0)).xyz;
    gl_Position = mvp * vec4(position, 1.0);
}