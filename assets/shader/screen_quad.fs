#version 330

in vec2 texcoord_;

out vec4 frag_color;

uniform sampler2D color_texture;
uniform bool linearize;
uniform float near_plane;
uniform float far_plane;

float linearize_depth(float depth) {
	return (2.f * near_plane) / (far_plane + near_plane - depth * (far_plane - near_plane));
}

void main() {
	vec4 color = texture(color_texture, texcoord_);
	if (linearize == true) {
		float z_linear = linearize_depth(color.r);
		color = vec4(z_linear, z_linear, z_linear, 1.0);
	}
	frag_color = color;
}
