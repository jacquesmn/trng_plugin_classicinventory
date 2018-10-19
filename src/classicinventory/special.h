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