void enter(entity_impl& out owner) {
    owner.get_animation_component().play_ani("stand");
    owner.get_character_physics_component().velocity(0);
    owner.get_character_physics_component().angular_velocity(0);
}

void update(float delta_time, entity_impl& out owner) {

}

void leave(entity_impl& out owner) {

}
