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

#pragma once
#pragma pack(push,1)

#include "ecs.h"
#include "input.h"
#include "state.h"

namespace classicinventory {
namespace controller {

class Controller {
private:
	ecs::EntityManager &entity_manager;
	ecs::SystemManager &system_manager;

	state::StateMachine state_machine;
	input::InputState input_state;

	bool inventory_active() const;

public:
	Controller(
		ecs::EntityManager &entity_manager,
		ecs::SystemManager &system_manager
	);

	void init();

	void do_main();

	void do_main_draw();

	void do_inventory();

	void do_statistics();

	void do_input(
		uint8_t VetInputKeyboard[],
		uint32_t *pInputExtGameCommands
	);
};



Controller& new_controller(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager);
Controller& get_controller(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager);

}
}

#pragma pack(pop)