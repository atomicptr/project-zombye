#version 140

out vec4 albedo_color;
out vec4 normal_color;
out vec4 material_color;

uniform vec3 color;

void main() {
    albedo_color = vec4(color * 2, 1.0);
    normal_color = vec4(0.33333, 0.33333 , 0.33333, 1.0);
    material_color = vec4(1.0, 1.0, 0.0, 1.0);
}
