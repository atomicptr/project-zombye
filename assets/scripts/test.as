void main() {
    entity e(glm::vec3(0), glm::quat(glm::radians(45), glm::vec3(1, 0, 0)), glm::vec3(1));
    print(e.position());
}
