#version 330

in vec2 texcoord_;

out vec4 frag_color;

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;
uniform sampler2D depth_texture;
uniform sampler2D shadow_texture[5];
uniform mat4 inv_view_projection;
uniform vec3 view_vector;
uniform int point_light_num;
// uniform arry size is a magic magic number. super magic.
uniform vec3 point_light_positions[40];
uniform vec3 point_light_colors[40];
uniform float point_light_radii[40];
uniform int directional_light_num;
// uniform arry size is a magic magic number. super magic.
uniform vec3 directional_light_directions[40];
uniform vec3 directional_light_colors[40];
uniform float directional_light_energy[40];
uniform mat4 shadow_projection;
uniform mat4 view;
uniform int num_sub_projections;
uniform mat4 sub_projections[5];
uniform vec3 ambient_term;

uniform int num_splits;
uniform float split_planes[6];

vec3 blinn_phong(vec3 N, vec3 L, vec3 V, vec3 light_color, vec3 diff_color, vec3 spec_color, float shininess) {
	vec3 H = normalize(L + V);

	float NdotL = max(0.0, dot(N, L));
	float NdotH = max(0.0, dot(N, H));

	return ambient_term + light_color * diff_color * NdotL + light_color * spec_color * pow(NdotH, shininess);
}

float sample_shadow(sampler2D shadow_map, vec2 texcoord, float compare) {
	return step(compare, texture(shadow_map, texcoord).r);
}

float linstep(float low, float high, float v) {
	return clamp((v - low) / (high - low), 0.0, 1.0);
}

float sample_variance_shadow(sampler2D shadow_map, vec2 texcoord, float compare) {
	vec2 moments = texture(shadow_map, texcoord).xy;

	float p = step(compare, moments.x);
	float variance = max(moments.y - moments.x * moments.x, 0.000002);

	float d = compare - moments.x;
	float p_max = linstep(0.2, 1.0, variance / (variance + d * d));

	return min(max(p, p_max), 1.0);
}

float calculate_shadow_amount(sampler2D shadow_map, vec4 initial_shadow_coord) {
	vec3 shadow_coord = initial_shadow_coord.xyz / initial_shadow_coord.w;
	return sample_variance_shadow(shadow_map, shadow_coord.xy, shadow_coord.z);
}

float sample_pssm(sampler2D shadow_maps[5], mat4 shadow_projections[5], float splits[6], float depth_view_space, vec3 p) {
	float light = 1.f;
	mat4 bias = mat4(0.5);
	bias[3] = vec4(0.5, 0.5, 0.5, 1.0);

	if (depth_view_space < splits[1]) {
		vec4 position_shadow = bias * sub_projections[0] * vec4(p, 1.0);
		light = calculate_shadow_amount(shadow_maps[0], position_shadow);
	} else if (depth_view_space < splits[2]) {
		vec4 position_shadow = bias * sub_projections[1] * vec4(p, 1.0);
		light = calculate_shadow_amount(shadow_maps[1], position_shadow);
	} else if (depth_view_space < splits[3]) {
		vec4 position_shadow = bias * sub_projections[2] * vec4(p, 1.0);
		light = calculate_shadow_amount(shadow_maps[2], position_shadow);
	} else if (depth_view_space < splits[4]) {
		vec4 position_shadow = bias * sub_projections[3] * vec4(p, 1.0);
		light = calculate_shadow_amount(shadow_maps[3], position_shadow);
	} else if (depth_view_space < splits[5]) {
		vec4 position_shadow = bias * sub_projections[4] * vec4(p, 1.0);
		light = calculate_shadow_amount(shadow_maps[4], position_shadow);
	}

	return light;
}

vec4 render_split_frusta(float depth, vec4 final_color) {
	vec4 frag_color = final_color;

	for (int i = 0; i < num_splits - 1; ++i) {
		float near_split_plane = split_planes[i];
		float far_split_plane = split_planes[i + 1];
		if (depth >= near_split_plane && depth < far_split_plane) {
			if (i == 4) {
				frag_color[0] *= 0.5;
				frag_color[1] = 0;
				frag_color[2] *= 0.5;
				break;
			}
			frag_color[(i + 1) % 3] = 0.0;
			frag_color[(i + 2) % 3] = 0.0;
			break;
		} else {
			continue;
		}
	}

	return frag_color;
}

void main() {
	float depth = 2.0 * texture(depth_texture, texcoord_).x - 1.0;
	vec3 clip_space;
	clip_space.xy = 2.0 * texcoord_ - 1.0;
	clip_space.z = depth;
	vec4 world_space = inv_view_projection *  vec4(clip_space,1.0);
	vec3 p = world_space.xyz / world_space.w;

	vec4 view_space = view * vec4(p, 1.f);
	float shadow_amount = sample_pssm(shadow_texture, sub_projections, split_planes, view_space.z, p);

	vec3 N = normalize(texture(normal_texture, texcoord_).xyz);
	vec3 V = normalize(view_vector - p);
	vec3 diffuse_color = texture(albedo_texture, texcoord_).rgb;
	vec3 spec_color = texture(specular_texture, texcoord_).rrr;
	float emission = texture(specular_texture, texcoord_).g;
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

	frag_color = vec4(mix(final_color, diffuse_color, emission), 1.0);

	//frag_color = render_split_frusta(view_space.z, frag_color);
}
