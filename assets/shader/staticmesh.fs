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
uniform vec3 view;

vec3 blinn_phong(vec3 N, vec3 L, vec3 V, vec3 light_color, vec3 diffuse_color, vec3 specular_color, float gloss, float shininess) {
    vec3 H = normalize(V + L);

    float NdotL = max(0.0, dot(N, L));
    float NdotH = max(0.0, dot(N, H));

    vec3 diffuse_term = light_color * diffuse_color * NdotL;
    vec3 specular_term = light_color * gloss * pow(NdotH, 50);

    return diffuse_term + specular_term;
}

void main() {
    vec3 N = normalize(f_normal);
    vec3 V = normalize(view - f_world_position);

    vec4 diffuse_color = texture2D(diffuse_sampler, f_texcoord);
    float gloss = texture2D(specular_sampler, f_texcoord).r;

    vec3 final_color = vec3(0, 0, 0);
    for (int i = 0; i < light_count; ++i) {
        vec3 L = normalize(light_position[i] - f_world_position);
        final_color += blinn_phong(N, L, V, light_color[i], diffuse_color.rgb, diffuse_color.rgb, gloss, 50);
    }

    fragcolor = vec4(final_color, diffuse_color.a);
}