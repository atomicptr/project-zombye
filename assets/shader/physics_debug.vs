#version 130

in vec3 in_position;
in vec3 in_color;

out vec4 out_color;

uniform mat4 vp;

void main() {
    out_color = vec4(in_color, 1.0);
    gl_Position = vp * vec4(in_position, 1.0);
}
