#include <zombye/physics/shapes/convex_hull_shape.hpp>

zombye::convex_hull_shape::convex_hull_shape(mesh *m) {
    auto shape = new btConvexHullShape();

    shape->addPoint(btVector3(2, 2, 1));
    shape->addPoint(btVector3(-2, 2, 1));
    shape->addPoint(btVector3(0, 2, -1));

    shape_ = std::unique_ptr<btCollisionShape>(shape);
}

btCollisionShape* zombye::convex_hull_shape::shape() {
    return shape_.get();
}
