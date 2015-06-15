#version 330

in vec2 texcoord_;

uniform sampler2D shadow_texture;
uniform vec2 blur_scale;

out vec4 frag_color;

void main() {
    vec4 color = vec4(0.0);

    color += texture2D(shadow_texture, texcoord_ + (vec2(-3.0) * blur_scale.xy)) * (1.0 / 64.0);
    color += texture2D(shadow_texture, texcoord_ + (vec2(-2.0) * blur_scale.xy)) * (6.0 / 64.0);
    color += texture2D(shadow_texture, texcoord_ + (vec2(-1.0) * blur_scale.xy)) * (15.0 / 64.0);
    color += texture2D(shadow_texture, texcoord_ + (vec2(0.0) * blur_scale.xy)) * (20.0 / 64.0);
    color += texture2D(shadow_texture, texcoord_ + (vec2(1.0) * blur_scale.xy)) * (15.0 / 64.0);
    color += texture2D(shadow_texture, texcoord_ + (vec2(2.0) * blur_scale.xy)) * (6.0 / 64.0);
    color += texture2D(shadow_texture, texcoord_ + (vec2(3.0) * blur_scale.xy)) * (1.0 / 64.0);

    frag_color = color;
}
