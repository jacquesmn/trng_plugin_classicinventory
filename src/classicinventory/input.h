#pragma once
#pragma pack(push, 1)

#include "ecs.h"

namespace classicinventory {
namespace state {
class StateMachine;
}
}

namespace classicinventory {
namespace input {

class InputState;

/*
Use this system to update current active input.
Only to be used in closed loops e.g. inventory, not during main game.
 */
class InputUpdateSystem : public ecs::System {
public:
	void update(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;
};

class InventoryInputSystem : public ecs::System {
private:
	state::StateMachine &state_machine;
	InputState &input_state;

public:
	InventoryInputSystem(state::StateMachine &state_machine, InputState &input_state);

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class InputState {
private:
	uint32_t commands_active;
	uint32_t commands_repeating;
	std::vector<bool> keys_active;
	std::vector<bool> keys_repeating;

public:
	InputState()
		:
		commands_active(0),
		commands_repeating(0),
		keys_active(255),
		keys_repeating(255)
	{}

	void update(uint32_t commands, std::vector<bool> keys);

	bool has_input() const;

	bool command_active(uint32_t command) const;
	bool key_active(uint8_t scancode) const;

	bool command_first_press(uint32_t command);
	bool key_first_press(uint8_t scancode);
};

}
}

#pragma pack(pop)