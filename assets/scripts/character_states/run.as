void enter(entity_impl& out owner) {
    owner.get_animation_component().play_ani("run");
}

void update(float delta_time, entity_impl& out owner) {
    owner.get_character_physics_component().linear_velocity(glm::vec3(1, 0, 0) * 3.f * delta_time);
}

void leave(entity_impl& out owner) {

}
