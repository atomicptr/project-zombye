#include <zombye/physics/shapes/sphere_shape.hpp>

zombye::sphere_shape::sphere_shape(float radius) {
    shape_ = std::unique_ptr<btCollisionShape>(new btSphereShape(radius));
}

btCollisionShape* zombye::sphere_shape::shape() {
    return shape_.get();
}
