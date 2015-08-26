void enter(entity_impl& out owner) {
    owner.get_animation_component().blend_ani("walk_backward");
}

void update(float delta_time, entity_impl& out owner) {

}

void leave(entity_impl& out owner) {

}
