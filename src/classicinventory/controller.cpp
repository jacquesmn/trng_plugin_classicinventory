#include <StdAfx.h>

#include "controller.h"

#include <algorithm>
#include <ctime>
#include <memory>
#include <vector>

#include <trng_core.h>
#include "action.h"
#include "cheat.h"
#include "item.h"
#include "motion.h"
#include "render.h"
#include "sound.h"
#include "special.h"
#include "state.h"
#include "camera.h"

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
	system_manager.add_system(new special::MediShortcutSystem(input_state));
	system_manager.add_system(new cheat::CheatSystem(input_state));
	system_manager.add_system(new camera::InventoryCameraSystem());
	system_manager.add_system(new render::InventoryRenderSystem());
	system_manager.add_system(new render::GameRenderSystem());
}

void Controller::do_main()
{
	// get list of applicable systems in desired order
	std::vector<ecs::System*> systems;
	systems.push_back(system_manager.get_system<special::PickupSystem>());
	systems.push_back(system_manager.get_system<special::MediShortcutSystem>());

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
	// this is to prevent re-opening inventory if player keeps keys pressed
	if (!input_state.command_first_press(enumCMD.INVENTORY) && !input_state.command_first_press(enumCMD.ACTION)) {
		// clear requested item to stop game from trying to re-open inventory
		*Trng.pGlobTomb4->pAdr->pInventoryRequiredSlotItem = -1;
		return;
	}

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
	const int32_t fps = 30;
	const int32_t frame_budget = 1000 / fps;

	while (inventory_active()) {
		const auto frame_start = clock();

		// update systems
		for (auto system_it = systems.begin(); system_it != systems.end(); ++system_it) {
			(*system_it)->update(entity_manager, system_manager);

			if (!inventory_active()) {
				break;
			}
		}

		// frame rate
		const auto frame_spent = clock() - frame_start;
		if (frame_budget > frame_spent) {
			Sleep(frame_budget - frame_spent);
		}
	}

	// cleanup systems
	std::for_each(systems.rbegin(), systems.rend(), [&](ecs::System *system) -> void {
		system->cleanup(entity_manager, system_manager);
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
