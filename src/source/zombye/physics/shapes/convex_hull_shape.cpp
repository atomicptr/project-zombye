#include <zombye/physics/collision_mesh.hpp>
#include <zombye/physics/shapes/convex_hull_shape.hpp>
#include <zombye/utils/logger.hpp>

zombye::convex_hull_shape::convex_hull_shape(std::shared_ptr<const collision_mesh> mesh) : mesh_(mesh) {
    auto shape = new btConvexHullShape();

    auto vertices = mesh->vertices();

    log("Create convex hull shape from: " + std::to_string(vertices.size()) + " vertices");

    for(auto &vertex : vertices) {
        auto &pos = vertex.pos;
        shape->addPoint(btVector3(pos.x, pos.y, pos.z));
    }

    shape_ = std::unique_ptr<btCollisionShape>(shape);
}

btCollisionShape* zombye::convex_hull_shape::shape() {
    return shape_.get();
}
