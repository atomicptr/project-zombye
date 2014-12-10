#version 140

in vec2 out_texel;
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

vec3 blinn_phong(vec3 n, vec3 l, vec3 v, vec3 diffuse_color, vec3 specular_color, float id, float is,
float kd, float ks, float s) {
    vec3 h = normalize(v + l);
    float ndotl = max(0.0, dot(n, l));
    float ndoth = max(0.0, dot(n, h));

    vec3 diffuse_term = id * kd * ndotl * diffuse_color;
    vec3 specular_term = is * ks * pow(ndoth, s) * specular_color;
    vec3 final_color = diffuse_term + specular_term;
    return final_color;
}

void main() {
    vec4 color = texture(color_texture, out_texel);
    float ks = texture(specular_texture, out_texel).r;

    vec3 nn = normalize(n);
    vec3 nv = normalize(v);
    vec3 final_color;

    for (int i = 0; i < light_count; ++i) {
        vec3 l = light_position[i] - world_position;
        vec3 nl = normalize(l);
        final_color += blinn_phong(nn, nl, nv, color.rgb, color.rgb, light_intensity[i], 1.0, 1.0, ks, 50);
    }

    fragcolor = vec4(final_color, color.a);
}
