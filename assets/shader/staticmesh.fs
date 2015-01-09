#version 140

in vec2 f_texcoord;
in vec3 f_normal;
in vec3 f_world_position;

out vec4 fragcolor;

uniform sampler2D diffuse_sampler;
uniform sampler2D specular_sampler;

uniform int light_count;
uniform vec3 light_position[16];
uniform vec3 light_color[16];
uniform float light_distance[16];
uniform vec3 view;

vec3 blinn_phong(vec3 N, vec3 L, vec3 V, vec3 light_color, vec3 diffuse_color, vec3 specular_color, float shininess) {
    vec3 H = normalize(V + L);

    float NdotL = max(0.0, dot(N, L));
    float NdotH = max(0.0, dot(N, H));

    vec3 color = light_color * diffuse_color;
    vec3 diffuse_term = color * NdotL;
    vec3 specular_term = color * pow(NdotH, shininess);

    return diffuse_term + specular_term;
}

void main() {
    vec3 N = normalize(f_normal);
    vec3 V = normalize(view - f_world_position);

    vec4 diffuse_color = texture2D(diffuse_sampler, f_texcoord);
    float shininess = texture2D(specular_sampler, f_texcoord).r * 128.f;

    vec3 final_color = vec3(0, 0, 0);
    for (int i = 0; i < light_count; ++i) {
        vec3 light = light_position[i] - f_world_position;
        vec3 L = normalize(light);
        float ld = length(light);
        float contribution = ld / light_distance[i];
        if (contribution <= 1.0) {
            contribution = 1.0 - contribution;
        } else {
            contribution = 0.0;
        }
        final_color += blinn_phong(N, L, V, contribution * light_color[i], diffuse_color.rgb, diffuse_color.rgb, shininess);
    }

    fragcolor = vec4(final_color, diffuse_color.a);
}