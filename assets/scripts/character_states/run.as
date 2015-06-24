void enter(entity_impl& out owner) {
    owner.get_animation_component().play_ani("run");
}

void update(float delta_time, entity_impl& out owner) {
    owner.get_character_physics_component().velocity(3.f);
}

void leave(entity_impl& out owner) {

}
