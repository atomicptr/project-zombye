#include <zombye/physics/physics_component.hpp>

zombye::physics_component::physics_component(game &g, entity &owner, btCollisionShape* col_shape, bool isstatic) : component(g, owner) {
    physics_ = g.physics();
    world_ = physics_->world();

    physics_->register_component(this);

    auto position = owner.position();
    auto rotation = owner.rotation();

    auto mass = isstatic ? 0.0f : 1.0f;

    auto bt_position = btVector3(position.x, position.y, position.z);
    auto bt_rotation = btQuaternion(btVector3(rotation.x, rotation.y, rotation.z), rotation.w);

    auto transform = btTransform(bt_rotation, bt_position);

    motion_state_ = std::make_unique<btDefaultMotionState>(transform);

    auto inertia = btVector3{0, 0, 0};

    if(!isstatic) {
        col_shape->calculateLocalInertia(mass, inertia);
    }

    auto bodyinfo = btRigidBody::btRigidBodyConstructionInfo{mass, motion_state_.get(), col_shape, inertia};

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

    auto axis = rotation.getAxis();
    auto angle = rotation.getAngle();

    rot.x = axis.x();
    rot.y = axis.y();
    rot.z = axis.z();
    rot.w = angle;

    owner().position(pos);
    owner().rotation(rot);
}