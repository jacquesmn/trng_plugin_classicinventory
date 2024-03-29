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

#include "cheat.h"

#include <algorithm>

#include <trng_core.h>
#include "core.h"
#include "inventory.h"
#include "item.h"
#include "state.h"

namespace classicinventory {
namespace cheat {

CheatSystem::CheatSystem(input::InputState &input_state)
	:
	input_state(input_state)
{}

void CheatSystem::init(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	if (!cheats_enabled()) {
		return;
	}

	const auto cheat_entities = entity_manager.find_entities_with_component<CheatConfig>();

	// turn on cheats
	for (auto it = cheat_entities.begin(); it != cheat_entities.end(); ++it) {
		auto &cheat_entity = **it;
		auto cheat_configs = cheat_entity.get_components<CheatConfig>();

		std::for_each(cheat_configs.begin(), cheat_configs.end(), [](CheatConfig *config) -> void {
			config->active = true;
		});
	}
}

void CheatSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	entity_manager.remove_components<Cheat>();
}

bool add_active_cheats(
	ecs::Entity &entity,
	input::InputState &input_state
)
{
	// find all cheat configs for entity and key_combo
	const auto key_active_or_zero = [&](int32_t key) -> bool {
		return key == 0 || input_state.key_active(key);
	};

	auto cheat_configs = entity.get_components<CheatConfig>([&](CheatConfig &config) -> bool {
		return key_active_or_zero(config.key_1)
			&& key_active_or_zero(config.key_2)
			&& key_active_or_zero(config.key_3)
			&& key_active_or_zero(config.key_4)
			&& key_active_or_zero(config.key_5)
			&& key_active_or_zero(config.key_6)
			&& key_active_or_zero(config.key_7)
			&& key_active_or_zero(config.key_8)
			&& key_active_or_zero(config.key_9)
			&& key_active_or_zero(config.key_10)
			&& config.active
			&& config.action
			&& config.enabled
			&& config.enabled();
	});

	// activate found cheats
	std::for_each(cheat_configs.begin(), cheat_configs.end(), [&](CheatConfig *config) -> void {
		entity.add_component(new Cheat(*config));
	});

	return !cheat_configs.empty();
}

void do_active_cheats(ecs::Entity &entity)
{
	auto cheats = entity.get_components<cheat::Cheat>();

	std::for_each(cheats.begin(), cheats.end(), [](cheat::Cheat *cheat) -> void {
		cheat->config.action();

		cheat->config.active = false;
	});

	entity.remove_components<cheat::Cheat>();
}

bool facing_north()
{
	return Trng.pGlobTomb4->pAdr->pLara->OrientationH == 0;
}

bool cheats_enabled()
{
	return !core::bit_set(Trng.pGlobTomb4->Settings, 1);
}

}
}