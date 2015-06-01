class entity {
    protected entity_impl@ impl_;

    entity(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale) {
        @impl_ = @entity_impl(position, rotation, scale);
    }

    entity_impl@ impl() {
        return impl_;
    }

    uint64 id() const {
        return impl_.id();
    }

    glm::vec3 position() {
        return impl_.position();
    }

    void position(const glm::vec3& in value) {
        impl_.position(value);
    }

    glm::quat rotation() const  {
        return impl_.rotation();
    }

    void rotation(const glm::quat& in value) {
        impl_.rotation(value);
    }

    glm::vec3 scale() const  {
        return impl_.scale();
    }

    void scale(const glm::vec3& in value) {
        impl_.scale(value);
    }
}
