#version 330

in vec3 _position;
in vec3 _normal;
in vec3 _tangent;
in vec2 _texcoord;

out vec2 texcoord_;
out vec3 normal_;
out vec3 tangent_;

uniform mat4 m;
uniform mat4 mit;
uniform mat4 mvp;

void main() {
    texcoord_ = _texcoord;
    normal_ = (mit * vec4(_normal, 0.0)).xyz;
    tangent_ = (mit * vec4(_tangent, 0.0)).xyz;
    gl_Position = mvp * vec4(_position, 1.0);
}
