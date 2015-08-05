#version 140

in vec3 color_;

out vec4 albedo_color;
out vec4 normal_color;
out vec4 specular_color;

void main() {
    albedo_color = vec4(color_, 1.0);
    normal_color = vec4(0.33333, 0.33333, 0.33333, 1.0);
    specular_color = vec4(0.0, 1.0, 0.0, 1.0);
}
