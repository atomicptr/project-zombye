#version 330

in vec3 _position;
in vec2 _texcoord;
in vec3 _normal;
in vec3 _tangent;
in ivec4 _index;
in vec4 _weight;

out vec2 texcoord_;
out vec3 normal_;
out vec3 tangent_;
out vec3 world_pos_;

uniform mat4 m;
uniform mat4 mit;
uniform mat4 mvp;
uniform mat4 pose[250];

void main() {
    texcoord_ = _texcoord;

    vec4 pos = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 nor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 tan = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < 4; ++i) {
        pos += _weight[i] * pose[_index[i]] * vec4(_position, 1.0);
        nor += _weight[i] * pose[_index[i]] * vec4(_normal, 0.0);
        tan += _weight[i] * pose[_index[i]] * vec4(_tangent, 0.0);
    }

    normal_ = (mit * nor).xyz;
    tangent_ = (mit * tan).xyz;
    world_pos_ = (m * vec4(_position, 1.0)).xyz;

    gl_Position = mvp * vec4(pos.xyz, 1.0);
}
