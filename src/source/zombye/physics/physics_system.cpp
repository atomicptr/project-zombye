#include <algorithm>

#include <zombye/config/config_system.hpp>
#include <zombye/core/game.hpp>
#include <zombye/physics/character_physics_component.hpp>
#include <zombye/physics/debug_renderer.hpp>
#include <zombye/physics/debug_render_bridge.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/physics/physics_system.hpp>
#include <zombye/utils/component_helper.hpp>
#include <zombye/ecs/entity.hpp>

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

    check_collisions();

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

void zombye::physics_system::register_collision_callback(entity* a, entity* b, std::function<void(entity*, entity*)> callback) {
    auto id_a = a->id();
    auto id_b = b->id();

    set_user_pointer(a);
    set_user_pointer(b);

    collision_listeners_[std::make_pair(id_a, id_b)] = callback;
}

bool zombye::physics_system::has_collision_callback(entity* a, entity* b) {
    auto id_a = a->id();
    auto id_b = b->id();

    auto it = collision_listeners_.find(std::make_pair(id_a, id_b));

    return it != collision_listeners_.end();
}

void zombye::physics_system::fire_collision_callback(entity* a, entity* b) {
    auto id_a = a->id();
    auto id_b = b->id();

    if(has_collision_callback(a, b)) {
        collision_listeners_[std::make_pair(id_a, id_b)](a, b);
    }
}

void zombye::physics_system::check_collisions() {
    auto num = world_->getDispatcher()->getNumManifolds();

    for(auto i = 0u; i < num; i++) {
        auto contact = world_->getDispatcher()->getManifoldByIndexInternal(i);

        auto a = static_cast<const btCollisionObject*>(contact->getBody0());
        auto b = static_cast<const btCollisionObject*>(contact->getBody1());

        auto num_contacts = contact->getNumContacts();

        for(auto j = 0u; j < num_contacts; j++) {
            auto& point = contact->getContactPoint(j);

            if(point.getDistance() < 0.f) {
                auto entity_a = static_cast<entity*>(a->getUserPointer());
                auto entity_b = static_cast<entity*>(b->getUserPointer());

                // both were set as user pointer
                if(entity_a != nullptr && entity_b != nullptr) {

                    if(has_collision_callback(entity_a, entity_b)) {
                        fire_collision_callback(entity_a, entity_b);
                    } else if(has_collision_callback(entity_b, entity_a)) {
                        fire_collision_callback(entity_b, entity_a);
                    }
                }
            }
        }
    }
}

void zombye::physics_system::set_user_pointer(entity* en) {
    auto physics_comp = en->component<physics_component>();

    btCollisionObject* obj = nullptr;

    // has no physics component? Maybe it's a character_physics_component
    if(physics_comp == nullptr) {
        auto character_physics_comp = en->component<character_physics_component>();

        if(character_physics_comp != nullptr) {
            obj = character_physics_comp->collision_object();
        } else {
            // this means the character has no kind of physics thing Oo
            zombye::log(LOG_ERROR, "Entity (id: " + std::to_string(en->id()) + ") has no physics or character controller component!");
        }
    } else {
        obj = physics_comp->collision_object();
    }

    if(obj != nullptr) {
        obj->setUserPointer((void*)en);
    } else {
        zombye::log(LOG_ERROR, "Entity (id: " + std::to_string(en->id()) + ") can't set user pointer...");
    }
}
