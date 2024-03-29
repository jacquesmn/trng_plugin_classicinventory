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

#include "StdAfx.h"

#include "camera.h"

#include <trng_core.h>

#include "inventory.h"

extern TYPE_phd_GetVectorAngles phd_GetVectorAngles;
extern TYPE_phd_GenerateW2V phd_GenerateW2V;
extern TYPE_AlterFOV AlterFOV;

namespace classicinventory {

void camera::InventoryCameraSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}
	auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

	if (!inventory_state.ring) {
		return;
	}
	auto &ring = inventory_state.ring->ring;

	if (!ring.has_component<ring::RingDisplay>()) {
		return;
	}
	const auto &ring_display = *ring.get_component<ring::RingDisplay>();

	const auto camera = entity_manager.find_entity_with_component<CameraView>();
	if (!camera) {
		return;
	}
	auto &camera_view = *camera->get_component<CameraView>();

	const auto ring_radius = core::round(ring_display.radius); // this will be the Z target
	const auto camera_tgt_x = core::round(camera_view.target.x);
	const auto camera_tgt_y = core::round(camera_view.target.y);
	const auto camera_pos_x = core::round(camera_view.position.x);
	const auto camera_pos_y = core::round(camera_view.position.y);
	const auto camera_pos_z = core::round(camera_view.position.z) + ring_radius; // move with ring radius
	const auto camera_rot_x = core::degrees_to_tr4_angle(camera_view.rotation.x);
	const auto camera_rot_y = core::degrees_to_tr4_angle(camera_view.rotation.y);
	const auto camera_rot_z = core::degrees_to_tr4_angle(camera_view.rotation.z);

	if (camera_view.fov != 0) {
		// Based on JoeyQuint's auto FOV formula:

		const auto w = (float)(Trng.pGlobTomb4->ScreenSizeX);
		const auto h = (float)(Trng.pGlobTomb4->ScreenSizeY);
		const auto ratio = w / h;

		// default (4/3) FOV is 80
		// widescreen (16/9) FOV should be something around 93
		
		// formula that makes the FOV follow the rules mentioned above
		const auto fov = (int)(((ratio * 30) + 40) * (camera_view.fov / DEFAULT_FOV_DEGREES));

		AlterFOV(fov * 192);
	}

	// backup current poison levels
	auto &poison1 = *Trng.pGlobTomb4->pAdr->pPoison1;
	auto &poison2 = *Trng.pGlobTomb4->pAdr->pPoison2;

	const auto backup_poison1 = poison1;
	const auto backup_poison2 = poison2;

	// disable poison warp effect
	poison1 = 0;
	poison2 = 0;

	int16_t camera_angles[2];
	phd_GetVectorAngles(
		camera_tgt_x - camera_pos_x,
		camera_tgt_y - camera_pos_y,
		ring_radius - camera_pos_z,
		camera_angles
	);

	StrPos3d viewer;
	viewer.Posizione = StrPosizione();
	viewer.Posizione.OrgX = camera_pos_x;
	viewer.Posizione.OrgY = camera_pos_y;
	viewer.Posizione.OrgZ = camera_pos_z;
	viewer.OrientV = camera_angles[1] + camera_rot_x; // pitch
	viewer.OrientH = camera_angles[0] + camera_rot_y; // yaw
	viewer.OrientR = camera_rot_z; // roll

	phd_GenerateW2V(&viewer);

	// restore poison levels
	poison1 = backup_poison1;
	poison2 = backup_poison2;
}

void camera::InventoryCameraSystem::cleanup(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	// restore FOV
	AlterFOV(core::degrees_to_tr4_angle(DEFAULT_FOV_DEGREES));
}

}