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

#include "controller.h"

#include <algorithm>
#include <ctime>
#include <memory>
#include <vector>
#include <trng_core.h>
#include "action.h"
#include "cheat.h"
#include "motion.h"
#include "render.h"
#include "sound.h"
#include "special.h"
#include "state.h"
#include "camera.h"

extern TYPE_S_DumpScreen S_DumpScreen;
extern StrMyData MyData;

namespace classicinventory {
namespace controller {

std::unique_ptr<Controller> controller;

Controller::Controller(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
	:
	entity_manager(entity_manager),
	system_manager(system_manager),
	state_machine(state::StateMachine(new state::ClosedState(), entity_manager)),
	input_state(input::InputState())
{}

void Controller::init()
{
	// register all systems
	system_manager.add_system(new state::StateSystem(state_machine));
	system_manager.add_system(new input::InputUpdateSystem());
	system_manager.add_system(new input::InventoryInputSystem(state_machine, input_state));
	system_manager.add_system(new motion::MotionSystem());
	system_manager.add_system(new sound::SoundSystem());
	system_manager.add_system(new special::TimeSystem());
	system_manager.add_system(new special::CompassSystem());
	system_manager.add_system(new special::StopwatchSystem());
	system_manager.add_system(new special::PickupSystem());
	system_manager.add_system(new special::ShortcutSystem(input_state));
	system_manager.add_system(new special::StatisticsSystem(MyData.Save.Local.statistics, MyData.Save.Global.statistics));
	system_manager.add_system(new cheat::CheatSystem(input_state));
	system_manager.add_system(new camera::InventoryCameraSystem());
	system_manager.add_system(new render::InventoryRenderSystem());
	system_manager.add_system(new render::GameRenderSystem());
	system_manager.add_system(new render::StatisticsRenderSystem());
}

void Controller::do_main()
{
	// get list of applicable systems in desired order
	std::vector<ecs::System*> systems;
	systems.push_back(system_manager.get_system<special::PickupSystem>());
	systems.push_back(system_manager.get_system<special::ShortcutSystem>());
	systems.push_back(system_manager.get_system<special::StatisticsSystem>());

	// update systems
	std::for_each(systems.begin(), systems.end(), [&](ecs::System *system) -> void {
		system->update(entity_manager, system_manager);
	});
}

void Controller::do_main_draw()
{
	// get list of applicable systems in desired order
	std::vector<ecs::System*> systems;
	systems.push_back(system_manager.get_system<render::GameRenderSystem>());

	// update systems
	std::for_each(systems.begin(), systems.end(), [&](ecs::System *system) -> void {
		system->update(entity_manager, system_manager);
	});
}

void Controller::do_inventory()
{
	// entry guards
	// this is to prevent game from re-opening inventory if player keeps keys pressed at puzzle hole
	const auto inventory_state = inventory::get_inventory_state(entity_manager);
	const auto first_press_inventory = input_state.command_first_press(enumCMD.INVENTORY);
	const auto first_press_action = input_state.command_first_press(enumCMD.ACTION);
	const auto first_press_enter = input_state.command_first_press(enumCMD.ENTER);
	const auto force_open = inventory_state && inventory_state->force_open;

	if (!first_press_inventory
		&& (!first_press_action || !first_press_enter)
		&& !force_open) {
		*Trng.pGlobTomb4->pAdr->pInventoryRequiredSlotItem = -1;
		return;
	}
	if (inventory_state) {
		inventory_state->force_open = false;
	}

	// debounce controls
	input_state.command_first_press(enumCMD.LEFT);
	input_state.command_first_press(enumCMD.RIGHT);
	input_state.command_first_press(enumCMD.UP);
	input_state.command_first_press(enumCMD.DOWN);

	// get list of applicable systems in desired order
	std::vector<ecs::System*> systems;
	systems.push_back(system_manager.get_system<state::StateSystem>());
	systems.push_back(system_manager.get_system<input::InputUpdateSystem>());
	systems.push_back(system_manager.get_system<input::InventoryInputSystem>());
	systems.push_back(system_manager.get_system<motion::MotionSystem>());
	systems.push_back(system_manager.get_system<special::TimeSystem>());
	systems.push_back(system_manager.get_system<special::CompassSystem>());
	systems.push_back(system_manager.get_system<special::StopwatchSystem>());
	systems.push_back(system_manager.get_system<cheat::CheatSystem>());
	systems.push_back(system_manager.get_system<camera::InventoryCameraSystem>());
	systems.push_back(system_manager.get_system<render::InventoryRenderSystem>());
	systems.push_back(system_manager.get_system<sound::SoundSystem>());

	// initialize systems
	std::for_each(systems.begin(), systems.end(), [&](ecs::System *system) -> void {
		system->init(entity_manager, system_manager);
	});

	// change state
	state_machine.set_state(new state::OpeningState());

	// inventory loop
	while (inventory_active()) {
		// update systems
		for (auto system_it = systems.begin(); system_it != systems.end(); ++system_it) {
			(*system_it)->update(entity_manager, system_manager);

			if (!inventory_active()) {
				break;
			}
		}

		S_DumpScreen();
	}

	// cleanup systems
	std::for_each(systems.rbegin(), systems.rend(), [&](ecs::System *system) -> void {
		system->cleanup(entity_manager, system_manager);
	});
}

void Controller::do_statistics()
{
	// get list of applicable systems in desired order
	std::vector<ecs::System*> systems;
	systems.push_back(system_manager.get_system<render::StatisticsRenderSystem>());

	// update systems
	std::for_each(systems.begin(), systems.end(), [&](ecs::System *system) -> void {
		system->update(entity_manager, system_manager);
	});
}

void Controller::do_input(
	uint8_t VetInputKeyboard[],
	uint32_t *pInputExtGameCommands
)
{
	// make key states easier to use
	std::vector<bool> keys;
	for (uint8_t i = 0; i < 255; ++i) {
		keys.push_back(VetInputKeyboard[i] != 0);
	}

	input_state.update(*pInputExtGameCommands, keys);

	// clear all inputs to be replaced:
	// medi shortcuts
	VetInputKeyboard[10] = 0;
	VetInputKeyboard[11] = 0;

	// flare shortcut
	//core::set_bit<uint32_t>(*pInputExtGameCommands, enumCMD.USE_FLARE, false);

	// save shortcut
	//VetInputKeyboard[63] = 0;
}

bool Controller::inventory_active() const
{
	const bool &game_closed = *reinterpret_cast<bool*>(0x753B8C);
	const int16_t &lara_health = Trng.pGlobTomb4->pAdr->pLara->Health;

	return state_machine.get_phase() == state::GamePhase::INVENTORY
		&& !game_closed
		&& lara_health > 0;
}



Controller& new_controller(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	controller = std::unique_ptr<Controller>(new Controller(entity_manager, system_manager));

	return *controller;
}

Controller& get_controller(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	if (!controller) {
		return new_controller(entity_manager, system_manager);
	}
	return *controller;
}
}
}
