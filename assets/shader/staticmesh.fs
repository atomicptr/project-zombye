#version 330

in vec2 out_texel;

out vec4 fragcolor;

uniform sampler2D diffuse;

void main() {
    fragcolor = texture(diffuse, out_texel);
}