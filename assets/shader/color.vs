#version 330

in vec3 in_position;
in vec4 in_color;

out vec4 out_color;

void main() {
    out_color = in_color;
    gl_Position = vec4(in_position, 1.0);
}