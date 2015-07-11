void move_forward_begin() {
	auto player = get_entity(player_id);
	auto player_state = player.get_state_component();
	if (player_state.current_state() != "run") {
		player_state.change_state("run");
	}
	player.get_character_physics_component().velocity(3.f);
}

void move_forward_end() {
	auto player = get_entity(player_id);
	auto player_state = player.get_state_component();
	if (player_state.current_state() == "run") {
		player_state.change_state("stand");
		player.get_character_physics_component().velocity(0.f);
	}
}

void move_backward_begin() {
	auto player = get_entity(player_id);
	auto player_state = player.get_state_component();
	if (player_state.current_state() != "walk_backward") {
		player_state.change_state("walk_backward");
	}
	player.get_character_physics_component().velocity(-1.f);
}

void move_backward_end() {
	auto player = get_entity(player_id);
	auto player_state = player.get_state_component();
	if (player_state.current_state() == "walk_backward") {
		player_state.change_state("stand");
		player.get_character_physics_component().velocity(0.f);
	}
}

void move_right_begin() {
	auto player = get_entity(player_id);
	player.get_character_physics_component().angular_velocity(-1.0f);
	if (player.get_state_component().current_state() == "stand") {
		if (!player.get_animation_component().is_playing("turn_right")) {
			player.get_animation_component().play_ani("turn_right");
		}
	}
}

void move_right_end() {
	auto player = get_entity(player_id);
	player.get_character_physics_component().angular_velocity(0.0f);
	if (player.get_state_component().current_state() == "stand") {
		player.get_animation_component().play_ani("stand");
	}
}

void move_left_begin() {
	auto player = get_entity(player_id);
	player.get_character_physics_component().angular_velocity(1.0f);
	if (player.get_state_component().current_state() == "stand") {
		if (!player.get_animation_component().is_playing("turn_right")) {
			player.get_animation_component().play_ani("turn_right");
		}
	}
}

void move_left_end() {
	auto player = get_entity(player_id);
	player.get_character_physics_component().angular_velocity(0.0f);
	if (player.get_state_component().current_state() == "stand") {
		player.get_animation_component().play_ani("stand");
	}
}

void stand_begin() {
	auto player = get_entity(player_id);
	auto player_state = player.get_state_component();
	if (player_state.current_state() != "stand") {
		player_state.change_state("stand");
	}
}
