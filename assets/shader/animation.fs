#version 330

in vec2 f_texcoord;
in vec3 f_normal;

out vec4 albedo_color;
out vec3 normal_color;
out vec4 specular_color;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

void main() {
    normal_color = f_normal;//normalize(f_normal * (2.0 * texture(normal_texture, f_texcoord).xyz - 1.0));
    albedo_color = texture(color_texture, f_texcoord);
    specular_color = texture(specular_texture, f_texcoord);
}