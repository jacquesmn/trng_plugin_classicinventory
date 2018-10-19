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
#include "core.h"

namespace classicinventory {
namespace camera {

class InventoryCameraSystem : public ecs::System {
public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;
};

struct CameraView : public ecs::Component {
	core::Vector3D position;
	core::Vector3D target;
	core::Vector3D rotation;
	float fov;

	CameraView(
		core::Vector3D position = core::Vector3D(),
		core::Vector3D target = core::Vector3D(),
		core::Vector3D rotation = core::Vector3D(),
		float fov = 80
	)
		:
		position(position),
		target(target),
		rotation(rotation),
		fov(fov)
	{}
};

}
}

#pragma pack(pop)