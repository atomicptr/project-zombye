#version 330

in vec2 texel;
in vec3 n;
in vec3 v;
in vec3 world_position;

out vec4 fragcolor;

uniform int light_count;
uniform vec3 light_position[16];
uniform vec3 light_color[16];
uniform float light_intensity[16];

uniform sampler2D color_texture;
uniform sampler2D specular_texture;

vec3 blinn_phong(vec3 n, vec3 l, vec3 v, vec3 lc, float ld, vec3 kc, float kd, float ks) {
    vec3 h = normalize(v + l);
    float ndotl = max(0.0, dot(n, l));
    float ndoth = max(0.0, dot(n, h));

    vec3 diffuse_term = lc * kc * ld * kd * ndotl;
    vec3 specular_term = lc * kc * pow(ndoth, 100.0);
    vec3 final_color = diffuse_term + specular_term;
    return final_color;
}

void main() {
    vec4 color = texture(color_texture, texel);
    float specularity = texture(specular_texture, texel).r;

    vec3 nn = normalize(n);
    vec3 nv = normalize(v);
    vec3 final_color;

    for (int i = 0; i < light_count; ++i) {
        vec3 l = light_position[i] - world_position;
        vec3 nl = normalize(l);
        final_color += blinn_phong(nn, nl, nv, light_color[i], light_intensity[i], color.rbg, 1.0, specularity);
    }

    fragcolor = vec4(final_color, color.a);
}