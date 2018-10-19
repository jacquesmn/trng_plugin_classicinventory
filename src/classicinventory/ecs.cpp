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

#include "ecs.h"

#include <memory>

namespace classicinventory {
namespace ecs {

std::unique_ptr<EntityManager> entity_manager;
std::unique_ptr<SystemManager> system_manager;

EntityManager& new_entity_manager()
{
	entity_manager = std::unique_ptr<EntityManager>(new EntityManager());

	return *entity_manager;
}

EntityManager& get_entity_manager()
{
	if (!entity_manager) {
		return new_entity_manager();
	}
	return *entity_manager;
}

SystemManager& new_system_manager()
{
	system_manager = std::unique_ptr<SystemManager>(new SystemManager());

	return *system_manager;
}

SystemManager& get_system_manager()
{
	if (!system_manager) {
		return new_system_manager();
	}
	return *system_manager;
}

void test()
{
	auto &system_manager = new_system_manager();

	system_manager.add_system<System>(nullptr);
}

}
}