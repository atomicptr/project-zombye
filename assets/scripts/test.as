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
    glm::quat q0;
    glm::quat q1(1, 2, 3, 4);
    glm::quat q2 = q1;
    print(formatFloat(q2.x, '0', 10, 10));
    print(formatFloat(q2.y, '0', 10, 10));
    print(formatFloat(q2.z, '0', 10, 10));
    print(formatFloat(q2.w, '0', 10, 10));
    glm::quat q3(0.5, glm::vec3(1, 0, 0));
    print(formatFloat(q3.x, '0', 10, 10));
    print(formatFloat(q3.y, '0', 10, 10));
    print(formatFloat(q3.z, '0', 10, 10));
    print(formatFloat(q3.w, '0', 10, 10));
}
