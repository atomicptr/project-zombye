#version 140

in vec2 texcoord_;

out vec4 frag_color;

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;
uniform sampler2D depth_texture;
uniform sampler2D shadow_texture;
uniform mat4 inv_view_projection;
uniform vec3 view_vector;
uniform vec3 directional_light_direction;
uniform vec3 directional_light_color;
uniform float directional_light_energy;
uniform mat4 shadow_projection;
uniform vec3 ambient_term;
uniform vec2 resolution;

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

void main() {
    float depth = 2.0 * texture(depth_texture, gl_FragCoord.xy / resolution).x - 1.0;
    vec3 clip_space;
    clip_space.xy = 2.0 * gl_FragCoord.xy / resolution - 1.0;
    clip_space.z = depth;
    vec4 world_space = inv_view_projection *  vec4(clip_space,1.0);
    vec3 p = world_space.xyz / world_space.w;

    mat4 bias = mat4(0.5);
    bias[3] = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 position_shadow = bias * shadow_projection * vec4(p, 1.0);
    float shadow_amount = 1.0;
    shadow_amount = calculate_shadow_amount(shadow_texture, position_shadow);

    vec3 N = normalize(texture(normal_texture, gl_FragCoord.xy / resolution).xyz);
    vec3 V = normalize(view_vector - p);
    vec3 diffuse_color = texture(albedo_texture, gl_FragCoord.xy / resolution).rgb;
    vec3 spec_color = texture(specular_texture, gl_FragCoord.xy / resolution).rrr;
    float emission = texture(specular_texture, gl_FragCoord.xy / resolution).g;

    vec3 L = normalize(directional_light_direction);

    vec3 final_color = blinn_phong(N, L, V, directional_light_color, diffuse_color, spec_color, 50)
        * directional_light_energy * shadow_amount;

    frag_color = vec4(mix(final_color, diffuse_color, emission), 1.0);
}
