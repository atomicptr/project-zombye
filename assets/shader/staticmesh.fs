#version 330

in vec2 f_texcoord;
in vec3 f_normal;
in vec3 f_tangent;

out vec4 albedo_color;
out vec3 normal_color;
out vec4 specular_color;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

vec3 calc_normal() {
	vec3 normal = normalize(f_normal);
	vec3 tangent = normalize(f_tangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	vec3 map_normal = normalize(2.0 * texture(normal_texture, f_texcoord).xyz - vec3(1.0, 1.0, 1.0));
	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 new_normal = normalize(tbn * map_normal);
	return new_normal;
}

void main() {
    normal_color = calc_normal();
    albedo_color = texture(color_texture, f_texcoord);
    specular_color = texture(specular_texture, f_texcoord);
}
