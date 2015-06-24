void enter(entity_impl& out owner) {
    owner.get_animation_component().play_ani("run");
}

void update(float delta_time, entity_impl& out owner) {
    //glm::vec3 pos = owner.position();
    //pos += glm::vec3(1, 0, 0) * 5.f * delta_time;
    //owner.position(pos);
}

void leave(entity_impl& out owner) {
    print("leave state");
}
