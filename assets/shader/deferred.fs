#version 330

in vec2 texcoord_;

out vec4 frag_color;

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;
uniform sampler2D depth_texture;
uniform sampler2D shadow_texture;
uniform mat4 inv_view_projection;
uniform vec3 view_vector;
uniform int point_light_num;
// uniform arry size is a magic magic number. super magic.
uniform vec3 point_light_positions[83];
uniform vec3 point_light_colors[83];
uniform float point_light_radii[83];
uniform int directional_light_num;
// uniform arry size is a magic magic number. super magic.
uniform vec3 directional_light_directions[83];
uniform vec3 directional_light_colors[83];
uniform float directional_light_energy[83];
uniform mat4 shadow_projection;

vec3 blinn_phong(vec3 N, vec3 L, vec3 V, vec3 light_color, vec3 diff_color, vec3 spec_color, float shininess) {
	vec3 H = normalize(L + V);

	float NdotL = max(0.0, dot(N, L));
	float NdotH = max(0.0, dot(N, H));

	return light_color * diff_color * NdotL + light_color * spec_color * pow(NdotH, shininess);
}

float sample_shadow(sampler2D shadow_map, vec2 texcoord, float compare) {
	return step(compare, texture2D(shadow_map, texcoord).r);
}

float calculate_shadow_amount(sampler2D shadow_map, vec4 initial_shadow_coord) {
	vec3 shadow_coord = initial_shadow_coord.xyz / initial_shadow_coord.w;
	return sample_shadow(shadow_map, shadow_coord.xy, shadow_coord.z);
}

void main() {
	float depth = 2.0 * texture(depth_texture, texcoord_).x - 1.0;
	vec3 clip_space;
	clip_space.xy = 2.0 * texcoord_ - 1.0;
	clip_space.z = depth;
	vec4 world_space = inv_view_projection *  vec4(clip_space,1.0);
	vec3 p = world_space.xyz / world_space.w;

	mat4 bias = mat4(0.5);
	bias[3] = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 position_shadow = bias * shadow_projection * vec4(p, 1.0);
	float shadow_amount = 1.0;
	shadow_amount = calculate_shadow_amount(shadow_texture, position_shadow);

	vec3 N = normalize(texture(normal_texture, texcoord_).xyz);
	vec3 V = normalize(view_vector - p);
	vec3 diffuse_color = texture(albedo_texture, texcoord_).rgb;
	vec3 spec_color = texture(specular_texture, texcoord_).rrr;
	vec3 final_color = vec3(0.0);
	for (int i = 0; i < point_light_num; ++i) {
		vec3 L = point_light_positions[i] - p;

		float d = length(L);
		float r = point_light_radii[i];
		float ac = 1;
		float al = 2 / r;
		float ae = 1 / pow(r, 2.0);
		float attenuation_denominator = ac + al * d + ae * pow(d, 2.0);

		L = normalize(L);
		final_color += blinn_phong(N, L, V, point_light_colors[i], diffuse_color, spec_color, 50)
			/ attenuation_denominator;
	}

	for (int i = 0; i < directional_light_num; ++i) {
		vec3 L = normalize(directional_light_directions[i]);
		float energy = directional_light_energy[i];

		final_color += blinn_phong(N, L, V, directional_light_colors[i], diffuse_color, spec_color, 50)
			* energy * shadow_amount;
	}

	//final_color = texture2D(shadow_texture, position_shadow.xy).rgb;

	frag_color = vec4(final_color, 1.0);
}
