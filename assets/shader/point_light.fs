#version 140

in vec3 point_light_position;
in vec3 point_light_color;
in float point_light_radius;
in float point_light_exponent;

out vec4 frag_color;

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;
uniform sampler2D depth_texture;
uniform mat4 inv_view_projection;
uniform vec3 view_vector;
uniform vec2 resolution;

vec3 blinn_phong(vec3 N, vec3 L, vec3 V, vec3 light_color, vec3 diff_color, vec3 spec_color, float shininess) {
	vec3 H = normalize(L + V);

	float NdotL = max(0.0, dot(N, L));
	float NdotH = max(0.0, dot(N, H));

	return light_color * diff_color * NdotL + light_color * spec_color * pow(NdotH, shininess);
}

void main() {
    float depth = 2.0 * texture(depth_texture, gl_FragCoord.xy / resolution).x - 1.0;
    vec3 clip_space;
    clip_space.xy = 2.0 * gl_FragCoord.xy / resolution - 1.0;
    clip_space.z = depth;
    vec4 world_space = inv_view_projection *  vec4(clip_space,1.0);
    vec3 p = world_space.xyz / world_space.w;

    vec3 N = normalize(texture(normal_texture, gl_FragCoord.xy / resolution).xyz);
    vec3 V = normalize(view_vector - p);
    vec3 diffuse_color = texture(albedo_texture, gl_FragCoord.xy / resolution).rgb;
    vec3 spec_color = texture(specular_texture, gl_FragCoord.xy / resolution).rrr;
    float emission = texture(specular_texture, gl_FragCoord.xy / resolution).g;

    vec3 L = point_light_position - p;

	float d = length(L);
	float r = point_light_radius;
	if (d <= r) {
		float attenuation = 1.0 - (pow(d / r, point_light_exponent));

		L = normalize(L);
		vec3 final_color = blinn_phong(N, L, V, point_light_color, diffuse_color, spec_color, 50) * attenuation;

		frag_color = vec4(mix(final_color, vec3(0.f, 0.f, 0.f), emission), 1.0);
	} else {
		discard;
	}
}
