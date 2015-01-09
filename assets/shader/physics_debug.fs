#version 140

in vec3 f_color;

out vec4 fragcolor;

void main() {
	fragcolor = vec4(f_color, 1.0);
}