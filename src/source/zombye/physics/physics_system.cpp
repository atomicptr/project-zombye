#include <algorithm>

#include <zombye/config/config_system.hpp>
#include <zombye/core/game.hpp>
#include <zombye/physics/character_physics_component.hpp>
#include <zombye/physics/debug_renderer.hpp>
#include <zombye/physics/debug_render_bridge.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/physics/physics_system.hpp>
#include <zombye/utils/component_helper.hpp>

#define ZDBG_DRAW_WIREFRAME 1
#define ZDBG_DRAW_AAB 2
#define ZDBG_DRAW_CONTACT_POINTS 8
#define ZDBG_DRAW_CONSTRAINTS 2048
#define ZDBG_DRAW_CONSTRAINTS_LIMITS 4096
#define ZDBG_DRAW_NORMALS 16384

#define DEBUG_DRAW_CONFIG (ZDBG_DRAW_WIREFRAME | ZDBG_DRAW_AAB | ZDBG_DRAW_CONTACT_POINTS | ZDBG_DRAW_CONSTRAINTS | ZDBG_DRAW_CONSTRAINTS_LIMITS | ZDBG_DRAW_NORMALS)

zombye::physics_system::physics_system(game& game)
: game_{game}, collision_mesh_manager_{game_} {
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

    debug_renderer_ = std::make_unique<debug_renderer>(game);
    bt_debug_drawer_ = std::make_unique<debug_render_bridge>(*debug_renderer_);

    world_->setDebugDrawer(bt_debug_drawer_.get());

    auto debug_draw = game_.config()->get("main", "physics_debug_draw").asBool();
    if (debug_draw) {
        bt_debug_drawer_->setDebugMode(DEBUG_DRAW_CONFIG);
    }
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

    for (auto& cp : character_physics_components_) {
        cp->update(delta_time);
        cp->sync();
    }
}

void zombye::physics_system::debug_draw() {
    world_->debugDrawWorld();
    debug_renderer_->draw();
}

void zombye::physics_system::toggle_debug() {
    if(bt_debug_drawer_->getDebugMode() != 0) {
        disable_debug();
    } else {
        enable_debug();
    }
}

void zombye::physics_system::enable_debug() {
    bt_debug_drawer_->setDebugMode(DEBUG_DRAW_CONFIG);
}

void zombye::physics_system::disable_debug() {
    bt_debug_drawer_->setDebugMode(0);
}

void zombye::physics_system::register_component(physics_component* comp) {
    components_.push_back(comp);
}

void zombye::physics_system::unregister_component(physics_component* comp) {
    //auto it = std::find(components_.begin(), components_.end(), comp);

    auto it = std::find_if(components_.begin(), components_.end(), [comp](auto other) {
        return comp == other;
    });

    if(it != components_.end()) {
        auto last = components_.end() - 1;

        if(it != last) {
            *it = std::move(*last);
        }

        components_.pop_back();
    }
}

void zombye::physics_system::register_component(character_physics_component* component) {
    character_physics_components_.emplace_back(component);
}

void zombye::physics_system::unregister_component(character_physics_component* component) {
    remove(character_physics_components_, component);
}
