#version 140

in vec3 position;
in vec3 color;

out vec3 f_color;

uniform mat4 vp;

void main() {
	gl_PointSize = 100.0;
	f_color = color;
	gl_Position = vp * vec4(position, 1.0);
}