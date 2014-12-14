#version 140

in vec3 in_position;
in vec3 in_color;

out vec4 out_color;

void main() {
    out_color = vec4(in_color, 1.0);
    gl_Position = vec4(in_position, 1.0);
}
