#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/physics/collision_shape.hpp>
#include <zombye/physics/physics_system.hpp>
#include <zombye/physics/character_physics_component.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    character_physics_component::character_physics_component(game& game, entity& owner, collision_shape* shape)
    : reflective{game, owner}, physics_{*game_.physics()}, world_{*physics_.world()} {
        physics_.register_component(this);

        collision_shape_ = std::unique_ptr<collision_shape>(shape);

        auto position = owner_.position();
        auto rotation = owner_.rotation();

        auto bt_position = btVector3(position.x, position.y, position.z);
        auto bt_rotation = btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w);

        auto transform = btTransform(bt_rotation, bt_position);

        ghost_object_ = std::make_unique<btPairCachingGhostObject>();
        ghost_object_->setWorldTransform(transform);
        auto convex_collision_shape = static_cast<btConvexShape*>(collision_shape_->shape());
        ghost_object_->setCollisionShape(convex_collision_shape);
        ghost_object_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

        character_controller_ = std::make_unique<btKinematicCharacterController>(
            ghost_object_.get(), convex_collision_shape, btScalar{0.35f});

        physics_.broadphase().getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback{});

        world_.addCollisionObject(ghost_object_.get(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
        world_.addAction(character_controller_.get());

        sync();
    }

    character_physics_component::~character_physics_component() {
        physics_.unregister_component(this);
    }

    void character_physics_component::update(float delta_time) {
        character_controller_->updateAction(&world_, delta_time);
    }

    void character_physics_component::sync() {
        static glm::vec3 pos{};
        static glm::quat rot{};

        auto trans = ghost_object_->getWorldTransform();

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

    void character_physics_component::linear_velocity(const glm::vec3& velocity) {
        character_controller_->setWalkDirection(btVector3{velocity.x, velocity.y, velocity.z});
    }

    void character_physics_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<character_physics_component>("character_physics_component");

        scripting_system.register_member_function("character_physics_component", "void linear_velocity(const glm::vec3& in)",
            +[](character_physics_component& component, const glm::vec3& velocity) { component.linear_velocity(velocity); });

        scripting_system.register_member_function("entity_impl", "character_physics_component@ get_character_physics_component()",
            +[](entity& owner) { return owner.component<character_physics_component>(); });
    }

    character_physics_component::character_physics_component(game& game, entity& owner)
    : reflective{game, owner}, physics_{*game_.physics()}, world_{*physics_.world()} {
        physics_.register_component(this);
    }
}
