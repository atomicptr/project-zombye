#version 330

in vec2 texcoord_;
in vec3 normal_;
in vec3 tangent_;

out vec4 albedo_color;
out vec3 normal_color;
out vec4 specular_color;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

vec3 calc_normal() {
	vec3 normal = normalize(normal_);
	vec3 tangent = normalize(tangent_);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	vec3 map_normal = normalize(2.0 * texture(normal_texture, texcoord_).xyz - vec3(1.0, 1.0, 1.0));
	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 new_normal = normalize(tbn * map_normal);
	return new_normal;
}

void main() {
    normal_color = calc_normal();
    albedo_color = texture(color_texture, texcoord_);
    specular_color = texture(specular_texture, texcoord_);
}
