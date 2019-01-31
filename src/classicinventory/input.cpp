/*
* Tomb Raider Next Generation Plugin - Classic Inventory
* Copyright (C) 2018 Jacques Niemand
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <StdAfx.h>

#include "input.h"

#include <algorithm>

#include <trng_core.h>
#include "core.h"
#include "camera.h"
#include "state.h"

extern TYPE_S_UpdateInput S_UpdateInput;

namespace classicinventory {
namespace input {

void InputUpdateSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	// this will call tr4 code which in turn will call callback function via trng
	S_UpdateInput();
}

InventoryInputSystem::InventoryInputSystem(
	state::StateMachine &state_machine,
	InputState &input_state
)
	:
	state_machine(state_machine),
	input_state(input_state)
{}

void InventoryInputSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	if (input_state.has_input()) {
		state_machine.input(input_state, entity_manager);
	}
}

void InputState::update(uint32_t commands, std::vector<bool> keys)
{
	commands_active = commands;
	keys_active = keys;

	// update state of repeating input
	commands_repeating &= commands;

	for (auto key_it = keys.begin(); key_it != keys.end(); ++key_it) {
		auto &key_active = *key_it;
		auto &key_repeating = keys_repeating[key_it - keys.begin()];

		key_repeating = key_active && key_repeating;
	}
}

bool InputState::has_input() const
{
	return commands_active != 0
		|| std::find(keys_active.begin(), keys_active.end(), true) != keys_active.end();
}

bool InputState::command_active(uint32_t command) const
{
	return core::bit_set(commands_active, command, true);
}

bool InputState::key_active(uint8_t scancode) const
{
	if (scancode < keys_active.size()) {
		return keys_active.at(scancode);
	}

	return false;
}

bool InputState::command_first_press(uint32_t command)
{
	const auto just_pressed = command_active(command) && !core::bit_set(commands_repeating, command, true);

	if (just_pressed) {
		// this is the first press, potentially repeating
		core::set_bit(commands_repeating, command);
	}

	return just_pressed;
}

bool InputState::key_first_press(uint8_t scancode)
{
	const auto just_pressed = key_active(scancode) && !keys_repeating.at(scancode);

	if (just_pressed) {
		// this is the first press, potentially repeating
		keys_repeating.at(scancode) = true;
	}

	return just_pressed;
}

}
}