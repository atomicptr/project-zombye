#version 330

out vec4 frag_color;

void main() {
    frag_color = vec4(gl_FragCoord.z, gl_FragCoord.z * gl_FragCoord.z, 0.0, 0.0);
}
