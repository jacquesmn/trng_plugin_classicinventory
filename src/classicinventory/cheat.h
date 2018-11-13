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

#include <functional>

#include "ecs.h"
#include "input.h"

namespace classicinventory {
namespace cheat {

namespace CheatHintType {
enum Enum {
	NONE = 1,
	COMPASS_TRANSPARENT
};
}

class CheatSystem : public ecs::System {
private:
	input::InputState &input_state;

public:
	CheatSystem(input::InputState &input_state);

	void init(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;

	void cleanup(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;
};

struct CheatConfig : public ecs::Component {
	int32_t key_1;
	int32_t key_2;
	int32_t key_3;
	int32_t key_4;
	int32_t key_5;
	int32_t key_6;
	int32_t key_7;
	int32_t key_8;
	int32_t key_9;
	int32_t key_10;
	std::function<void()> action;
	std::function<bool()> enabled;
	CheatHintType::Enum hint_type;
	bool active;

	CheatConfig(
		int32_t key_1 = 0,
		int32_t key_2 = 0,
		int32_t key_3 = 0,
		int32_t key_4 = 0,
		int32_t key_5 = 0,
		int32_t key_6 = 0,
		int32_t key_7 = 0,
		int32_t key_8 = 0,
		int32_t key_9 = 0,
		int32_t key_10 = 0
	)
		:
		key_1(key_1),
		key_2(key_2),
		key_3(key_3),
		key_4(key_4),
		key_5(key_5),
		key_6(key_6),
		key_7(key_7),
		key_8(key_8),
		key_9(key_9),
		key_10(key_10),
		action([]() -> void {}),
		enabled([]() -> bool { return true; }),
		hint_type(CheatHintType::COMPASS_TRANSPARENT),
		active(false)
	{}
};

struct Cheat : public ecs::Component {
	CheatConfig &config;

	Cheat(CheatConfig &config)
		:
		config(config)
	{}
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
bool add_active_cheats(ecs::Entity &entity, input::InputState &input_state);

void do_active_cheats(ecs::Entity &entity);

bool facing_north();

bool cheats_enabled();

}
}

#pragma pack(pop)