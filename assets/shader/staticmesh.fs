#version 330

in vec2 out_texel;
in vec3 n;
in vec3 l;
in vec3 v;

out vec4 fragcolor;

uniform vec3 light_color;
uniform float light_intensity;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;

vec3 blinn_phong(vec3 n, vec3 l, vec3 v, vec3 lc, float ld, vec3 kc, float kd, float ks) {
    vec3 h = normalize(v + l);
    float ndotl = max(0.0, dot(n, l));
    float ndoth = max(0.0, dot(n, h));

    vec3 diffuse_term = lc * kc * ld * kd * ndotl;
    vec3 specular_term = lc * kc * pow(ndoth, 50.0);
    vec3 final_color = diffuse_term + specular_term;
    return final_color;
}

void main() {
    vec4 color = texture(color_texture, out_texel);
    float specularity = texture(specular_texture, out_texel).r;

    vec3 nn = normalize(n);
    vec3 nl = normalize(l);
    vec3 nv = normalize(v);

    fragcolor = vec4(blinn_phong(nn, nl, nv, light_color, light_intensity, color.rbg, 1.0, specularity), color.a);
}