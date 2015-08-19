#version 140

in vec3 _position;
in vec2 _texcoord;
in vec3 _normal;
in vec3 _tangent;
in mat4 mvp;
in vec3 position;
in vec3 color;
in float radius;

out flat vec3 point_light_position;
out flat vec3 point_light_color;
out flat float point_light_radius;

void main() {
    point_light_position = position;
    point_light_color = color;
    point_light_radius = radius;
    gl_Position = mvp * vec4(_position, 1.f);
}
