#version 330

in vec2 texcoord_;

uniform sampler2D shadow_texture;

out vec4 frag_color;

void main() {
    frag_color = texture2D(shadow_texture, texcoord_);
}
