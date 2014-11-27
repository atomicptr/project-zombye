#include <zombye/core/game.hpp>
#include <zombye/physics/physics_system.hpp>

zombye::physics_system::physics_system(game& game)
: game_{game} {
    broadphase_ = std::make_unique<btDbvtBroadphase>();
    collision_config_ = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher_ = std::make_unique<btCollisionDispatcher>(collision_config_.get());
    solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();

    world_ = std::make_unique<btDiscreteDynamicsWorld>(
        dispatcher_.get(),
        broadphase_.get(),
        solver_.get(),
        collision_config_.get());

    world_->setGravity(btVector3(0, -9.81, 0));

    debug_drawer_ = std::make_unique<debug_renderer>(game_); // TODO: change this
    bt_debug_drawer_ = std::make_unique<debug_render_bridge>(debug_drawer_.get());

    world_->setDebugDrawer(bt_debug_drawer_.get());

#ifdef ZOMBYE_DEBUG
    bt_debug_drawer_->setDebugMode(1);
#endif
}

zombye::physics_system::~physics_system() {

}

btDiscreteDynamicsWorld* zombye::physics_system::world() {
    return world_.get();
}

void zombye::physics_system::update(float delta_time) {
    world_->stepSimulation(delta_time);

    for(auto comp : components_) {
        comp->sync();
    }
}

void zombye::physics_system::debug_draw() {
    world_->debugDrawWorld();
}

void zombye::physics_system::toggle_debug() {
    if(bt_debug_drawer_->getDebugMode() == 1) {
        disable_debug();
    } else {
        enable_debug();
    }
}

void zombye::physics_system::enable_debug() {
    bt_debug_drawer_->setDebugMode(1);
}

void zombye::physics_system::disable_debug() {
    bt_debug_drawer_->setDebugMode(0);
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
