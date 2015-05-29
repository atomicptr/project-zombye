void main() {
    glm::vec3 v0;
    glm::vec3 v1(1, 5, 2);
    glm::vec3 v2 = 5 * v1;
    glm::vec3 v3 = v2 / 5;
    print(v1);
    print(v2);
    print(v3);
    print(formatFloat(v2.x, '0', 10, 10));
    print(formatFloat(v2.y, '0', 10, 10));
    print(formatFloat(v2.z, '0', 10, 10));
}
