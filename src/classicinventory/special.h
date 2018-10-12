#pragma once
#pragma pack(push, 1)

#include "ecs.h"

namespace classicinventory {
namespace input {
class InputState;
}
}

namespace classicinventory {
namespace special {

class TimeSystem : public ecs::System {
public:
	void update(ecs::EntityManager& entity_manager, ecs::SystemManager &system_manager) override;
};

class CompassSystem : public ecs::System {
public:
	void init(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class StopwatchSystem : public ecs::System {
private:
	bool first_update;

public:
	StopwatchSystem();

	void init(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class PickupSystem : public ecs::System {
public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class MediShortcutSystem : public ecs::System {
private:
	input::InputState &input_state;

public:
	MediShortcutSystem(input::InputState &input_state);

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};



struct GameTime : public ecs::Component {
	uint32_t frames;
	float hours;
	float minutes;
	float seconds;
	float milliseconds;

	GameTime()
		:
		frames(0),
		hours(0),
		minutes(0),
		seconds(0),
		milliseconds(0)
	{}
};

}
}

#pragma pack(pop)