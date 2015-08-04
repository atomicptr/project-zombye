#include "../../assets/scripts/entities/light_cube.as"

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

void throw_light_begin() {
	auto player = get_entity(player_id);
	glm::vec3 direction = player.get_character_physics_component().direction();
	glm::vec3 position = player.position();
	light_cube lc(position + direction * 2.f, player.rotation());
	lc.impl().get_physics_component().apply_central_impulse((direction + glm::vec3(0.f, 0.2f, 0.f)) * 5.f);
}

void throw_light_end() {

}
