#include <zombye/physics/physics_system.hpp>

zombye::physics_system::physics_system() {
    broadphase_ = std::unique_ptr<btDbvtBroadphase>(
        new btDbvtBroadphase());
    collision_config_ = std::unique_ptr<btDefaultCollisionConfiguration>(
        new btDefaultCollisionConfiguration());
    dispatcher_ = std::unique_ptr<btCollisionDispatcher>(
        new btCollisionDispatcher(collision_config_.get()));
    solver_ = std::unique_ptr<btSequentialImpulseConstraintSolver>(
        new btSequentialImpulseConstraintSolver());

    world_ = std::unique_ptr<btDiscreteDynamicsWorld>(
        new btDiscreteDynamicsWorld(
            dispatcher_.get(),
            broadphase_.get(),
            solver_.get(),
            collision_config_.get()));

    world_->setGravity(btVector3(0, -10, 0));
}

zombye::physics_system::~physics_system() {

}

btDiscreteDynamicsWorld* zombye::physics_system::world() {
    return world_.get();
}

void zombye::physics_system::init() {
}

void zombye::physics_system::update(float delta_time) {
    world_->stepSimulation(delta_time);

    for(auto comp : components_) {
        comp->sync();
    }
}

void zombye::physics_system::register_component(physics_component* comp) {
    components_.push_back(comp);
}

void zombye::physics_system::unregister_component(physics_component* comp) {
    auto it = std::find(components_.begin(), components_.end(), comp);

    if(it != components_.end()) {
        auto last = components_.end() - 1;

        if(it != last) {
            *it = std::move(*last);
        }

        components_.pop_back();
    }
}
