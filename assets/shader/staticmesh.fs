#version 330

in vec2 texcoord_;
in vec3 normal_;
in vec3 tangent_;
in vec3 world_pos_;

out vec4 albedo_color;
out vec3 normal_color;
out vec4 specular_color;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;
uniform vec3 view_vector;
uniform float disp_map_scale;
uniform float disp_map_bias;

vec3 calc_normal(sampler2D normal_map, vec2 texcoord, mat3 tbn) {
	vec3 normal = normalize(2.0 * texture(normal_map, texcoord).xyz - vec3(1.0, 1.0, 1.0));
	normal = normalize(tbn * normal);
	return normal;
}

void main() {
	vec3 normal = normalize(normal_);
	vec3 tangent = normalize(tangent_);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);

	vec3 direction_to_view = normalize(view_vector - world_pos_);

	vec2 texcoord = texcoord_ + (tbn * direction_to_view).xy * (texture(specular_texture, texcoord_).b
		* disp_map_scale + disp_map_bias);

    normal_color = calc_normal(normal_texture, texcoord, tbn);
    albedo_color = texture(color_texture, texcoord);
    specular_color = vec4(texture(specular_texture, texcoord).rg, 0.0, 1.0);
}
