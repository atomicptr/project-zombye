#version 330

in vec2 out_texel;

out vec4 fragcolor;

uniform sampler2D color_texture;

void main() {
    fragcolor = texture(color_texture, out_texel);
}