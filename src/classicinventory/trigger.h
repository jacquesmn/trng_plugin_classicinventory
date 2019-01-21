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

namespace classicinventory {
namespace trigger {

namespace ItemSelectType {
enum Enum {
	SELECT = 1,
	ACTIVATE
};
}

namespace ItemMissingResponse {
enum Enum {
	SILENCE = 1,
	NO
};
}

void flipeffect_increase_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

void flipeffect_decrease_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

void flipeffect_set_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

void flipeffect_open_inventory_at_item(
	int32_t item_id,
	ItemSelectType::Enum select_type,
	ItemMissingResponse::Enum missing_response,
	bool open_now,
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
);

void flipeffect_show_pickup_notifier(int32_t item_id, ecs::EntityManager &entity_manager);

bool condition_item_qty_at_least(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

bool condition_item_qty_less_than(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

bool condition_item_is_selected(int32_t item_id, ecs::EntityManager &entity_manager);

bool condition_item_last_used(int32_t item_id, ecs::EntityManager &entity_manager);

bool condition_inventory_enabled(bool enabled, ecs::EntityManager &entity_manager);

}
}

#pragma pack(pop)