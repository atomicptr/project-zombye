#include <zombye/core/game.hpp>
#include <zombye/physics/shapes/box_shape.hpp>
#include <zombye/physics/character_physics_component.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/scripting/scripting_system.hpp>

zombye::box_shape::box_shape(glm::vec3 &v) : box_shape(v.x, v.y, v.z) {
}

zombye::box_shape::box_shape(float x, float y, float z) {
    shape_ = std::unique_ptr<btCollisionShape>(new btBoxShape(btVector3(x, y, z)));
}

btCollisionShape* zombye::box_shape::shape() {
    return shape_.get();
}

void zombye::box_shape::register_at_script_engine(game& game) {
    auto& scripting_system = game.scripting_system();

    scripting_system.register_type<box_shape>("box_shape");

    scripting_system.register_factory("box_shape", "box_shape@ f(float x, float y, float z)",
        +[](float x, float y, float z) { return new box_shape(x, y, z); });
    scripting_system.register_factory("box_shape", "box_shape@ f(const glm::vec3& in dimensions)",
        +[](const glm::vec3& dimensions) { return new box_shape(dimensions.x, dimensions.y, dimensions.z); });

    scripting_system.register_member_function("entity_impl",
        "physics_component& add_physics_component(box_shape@ collison_shape, bool is_static = false)",
        +[](entity& owner, box_shape* collison_shape, bool is_static) -> physics_component& {
            return owner.emplace<physics_component>(collison_shape, is_static);
        });

    scripting_system.register_member_function("entity_impl",
        "character_physics_component& add_character_physics_component(box_shape@ collison_shape)",
        +[](entity& owner, box_shape* collison_shape) -> character_physics_component& {
            return owner.emplace<character_physics_component>(collison_shape);
        });
}
