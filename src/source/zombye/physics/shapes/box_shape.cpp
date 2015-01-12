#include <zombye/physics/shapes/box_shape.hpp>

zombye::box_shape::box_shape(glm::vec3 &v) : box_shape(v.x, v.y, v.z) {
}

zombye::box_shape::box_shape(float x, float y, float z) {
    shape_ = std::unique_ptr<btCollisionShape>(new btBoxShape(btVector3(x, y, z)));
}

btCollisionShape* zombye::box_shape::shape() {
    return shape_.get();
}
