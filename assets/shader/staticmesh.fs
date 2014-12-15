#version 140

in vec2 f_texcoord;

out vec4 fragcolor;

uniform sampler2D diffuse;

void main() {
    fragcolor = texture2D(diffuse, f_texcoord);
}