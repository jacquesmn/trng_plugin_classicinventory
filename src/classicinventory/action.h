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
namespace action {

bool use_health(ecs::Entity &item, bool silent = false);
bool use_flare(ecs::Entity &item, bool throw_away = false, bool silent = false);
bool use_binoculars(ecs::Entity &item);
bool equip_weapon(ecs::Entity &item);
bool exchange_waterskins(ecs::Entity &item);
bool load_game(bool draw_background = false, int32_t *result = nullptr);
bool save_game(bool draw_background = false, int32_t *result = nullptr);
bool exit_to_title();
void pickup_item(ecs::Entity &item, ecs::EntityManager &entity_manager);

}
}

#pragma pack(pop)