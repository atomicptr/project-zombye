#version 330

out float frag_color;

void main() {
    float depth = gl_FragCoord.z;

    frag_color = exp(80.0 * depth);
}
