#version 330

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in ivec4 index;
in vec4 weight;

out vec2 f_texcoord;
out vec3 f_normal;
out vec3 f_world_position;

uniform mat4 m;
uniform mat4 mit;
uniform mat4 mvp;
uniform mat4 pose[250];

void main() {
    f_texcoord = texcoord;

    vec4 pos = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 nor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < 4; ++i) {
        pos += weight[i] * pose[index[i]] * vec4(position, 1.0);
        nor += weight[i] * pose[index[i]] * vec4(normal, 0.0);
    }

    f_normal = (mit * nor).xyz;
    f_world_position = (m * pos).xyz;

    gl_Position = mvp * vec4(pos.xyz, 1.0);
}