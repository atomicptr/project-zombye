void main() {
    for (auto i = 0; i < 10; ++i) {
        print(formatInt(i, 'l', 10));
    }
    auto some_string = "this is some string";
    print(some_string);
    glm::vec3 pos(0.f, 10.f, -0.5f);
    glm::vec3 pos2(0.f, 5.f, 1.f);
    print(pos);
    print(pos2);
    auto pos3 = pos2;
    pos3 += pos;
    print(pos3);
    auto pos4 = pos + pos2;
    print(pos4);
    print(pos3 - pos4);
    auto zero = glm::vec3(0.f, 0.f, 0.f);
    print(zero - pos3);
    print(20.556464 * glm::vec3(1.f, 0.f, 0.f) * -5);
    print(glm::vec3(5.f, 10.f, 15.f) / 5.f);
    auto pos5 = glm::vec3(5, 10, 15);
    pos5 /= 5;
    print(pos5);
    pos5 *= 5;
    print(pos5);
    print(formatFloat(glm::length(pos5), '0', 10, 10));
    print(formatFloat(glm::dot(glm::vec3(1.f, 0.f, 5.f), glm::vec3(3.f, 0.f, 1.2f)), '0', 10, 10));
    print(glm::cross(glm::vec3(1,0,0), glm::vec3(0,1,0)));
    print(glm::normalize(pos));
    print(formatFloat(glm::length(glm::normalize(pos)), '0', 10, 10));
}
