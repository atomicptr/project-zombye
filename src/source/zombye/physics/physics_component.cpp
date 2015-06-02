#include <zombye/ecs/entity.hpp>
#include <zombye/core/game.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/physics/collision_shape.hpp>
#include <zombye/physics/physics_system.hpp>

zombye::physics_component::physics_component(game& game, entity& owner)
: reflective{game, owner}, body_{nullptr}, motion_state_{nullptr}, colshape_{nullptr} {
    physics_ = game.physics();
    world_ = physics_->world();
}

zombye::physics_component::physics_component(game &g, entity &owner, collision_shape* col_shape, bool isstatic) : reflective(g, owner) {
    physics_ = g.physics();
    world_ = physics_->world();
    colshape_ = std::unique_ptr<collision_shape>(col_shape);

    physics_->register_component(this);

    auto position = owner.position();
    auto rotation = owner.rotation();

    auto mass = isstatic ? 0.0f : 1.0f;

    auto bt_position = btVector3(position.x, position.y, position.z);
    auto bt_rotation = btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w);

    auto transform = btTransform(bt_rotation, bt_position);

    motion_state_ = std::make_unique<btDefaultMotionState>(transform);

    auto inertia = btVector3{0, 0, 0};

    if(!isstatic) {
        colshape_->shape()->calculateLocalInertia(mass, inertia);
    }

    auto bodyinfo = btRigidBody::btRigidBodyConstructionInfo{mass, motion_state_.get(), colshape_->shape(), inertia};

    body_ = std::make_unique<btRigidBody>(bodyinfo);

    world_->addRigidBody(body_.get());

    sync();
}

zombye::physics_component::~physics_component() {
    physics_->unregister_component(this);

    world_->removeRigidBody(body_.get());
}

void zombye::physics_component::sync() const {
    static glm::vec3 pos{};
    static glm::quat rot{};

    btTransform trans;
    body_->getMotionState()->getWorldTransform(trans);

    auto origin = trans.getOrigin();
    auto rotation = trans.getRotation();

    pos.x = origin.x();
    pos.y = origin.y();
    pos.z = origin.z();

    rot.x = rotation.getX();
    rot.y = rotation.getY();
    rot.z = rotation.getZ();
    rot.w = rotation.getW();

    owner().position(pos);
    owner().rotation(rot);
}
