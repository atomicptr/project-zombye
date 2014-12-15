#version 140

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 f_texcoord;

uniform mat4 mvp;

void main() {
    f_texcoord = texcoord;
    gl_Position = mvp * vec4(position, 1.0);
}