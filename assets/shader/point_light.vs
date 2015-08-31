#version 140

in vec3 _position;
in vec2 _texcoord;
in vec3 _normal;
in vec3 _tangent;
in vec3 position;
in vec3 color;
in float radius;
in float exponent;

uniform mat4 view_projection;

flat out vec3 point_light_position;
flat out vec3 point_light_color;
flat out float point_light_radius;
flat out float point_light_exponent;

void main() {
    point_light_position = position;
    point_light_color = color;
    point_light_radius = radius;
    point_light_exponent = exponent;
    gl_Position = view_projection * vec4(_position * 2.f * radius + position, 1.f);
}
