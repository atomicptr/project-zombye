#version 330

in vec3 position;
in vec2 texcoord;

out vec2 texcoord_;

uniform mat4 projection;

void main() {
	texcoord_ = texcoord;
	gl_Position = projection * vec4(position, 1.0);
}
