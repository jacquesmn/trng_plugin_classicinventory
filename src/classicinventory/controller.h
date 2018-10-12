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