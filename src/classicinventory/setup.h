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

#include <trng_core.h>
#include "ecs.h"

namespace classicinventory {
namespace setup {

void setup_phase1(ecs::EntityManager &entity_manager);

void setup_phase2(ecs::EntityManager &entity_manager);

void customize_phase1(const StrGenericCustomize &customize, ecs::EntityManager &entity_manager);

void customize_phase2(ecs::EntityManager &entity_manager);

}
}

#pragma pack(pop)