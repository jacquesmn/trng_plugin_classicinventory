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

#include <string>

#include <vector>

#include <trng_core.h>
#include "core.h"
#include "ecs.h"
#include "item.h"
#include "ring.h"

namespace classicinventory {
namespace inventory {

namespace DurationType {
enum Enum {
	INVENTORY_OPEN = 1,
	RING_ROTATE,
	RING_CHANGE,
	ITEM_ACTIVATE,
	ITEM_SPIN,
	NONE
};
}

struct InventoryData : public ecs::Component {
	bool enabled;

	InventoryData()
		:
		enabled(true)
	{}
};

struct InventoryDisplay : public ecs::Component {
	float ring_radius_closed;
	float ring_radius_opened;
	core::Vector3D ring_orient_closed;
	core::Vector3D ring_orient_opened;
	core::Vector3D camera_pos_closed;
	core::Vector3D camera_pos_opened;
	core::Vector3D camera_tgt_closed;
	core::Vector3D camera_tgt_opened;
	float camera_ring_change_pitch;
	float camera_fov_closed;
	float camera_fov_opened;
	float item_base_size;
	bool ring_rotate_lock;

	InventoryDisplay()
		:
		ring_radius_closed(0),
		ring_radius_opened(0),
		ring_orient_closed(core::Vector3D()),
		ring_orient_opened(core::Vector3D()),
		camera_pos_closed(core::Vector3D()),
		camera_pos_opened(core::Vector3D()),
		camera_tgt_closed(core::Vector3D()),
		camera_tgt_opened(core::Vector3D()),
		camera_ring_change_pitch(0),
		camera_fov_closed(80),
		camera_fov_opened(80),
		item_base_size(0),
		ring_rotate_lock(true)
	{}
};

struct InventoryDuration : public ecs::Component {
	uint32_t inventory_open_frames;
	uint32_t ring_change_frames;
	uint32_t ring_rotate_frames;
	uint32_t item_activate_frames;
	uint32_t item_spin_frames;

	InventoryDuration()
		:
		inventory_open_frames(0),
		ring_change_frames(0),
		ring_rotate_frames(0),
		item_activate_frames(0),
		item_spin_frames(0)
	{}
};

struct InventorySfx : public ecs::Component {
	int32_t opening_sound_id;
	int32_t closing_sound_id;
	int32_t ring_rotate_sound_id;
	int32_t ring_change_sound_id;
	int32_t item_select_sound_id;
	int32_t item_cancel_sound_id;
	int32_t menu_change_sound_id;
	int32_t fail_sound_id;
	int32_t combine_sound_id;
	int32_t separate_sound_id;
	int32_t ammo_load_sound_id;

	InventorySfx()
		:
		opening_sound_id(-1),
		closing_sound_id(-1),
		ring_rotate_sound_id(-1),
		ring_change_sound_id(-1),
		item_select_sound_id(-1),
		item_cancel_sound_id(-1),
		menu_change_sound_id(-1),
		fail_sound_id(-1),
		combine_sound_id(-1),
		separate_sound_id(-1),
		ammo_load_sound_id(-1)
	{}
};

struct InventoryRing : public ecs::Component {
	ecs::Entity &ring;
	InventoryRing *prev;
	InventoryRing *next;

	InventoryRing(
		ecs::Entity &ring,
		InventoryRing *prev = nullptr,
		InventoryRing *next = nullptr
	)
		:
		ring(ring),
		prev(prev),
		next(next)
	{}
};

struct InventoryState : public ecs::Component {
	InventoryRing *ring;
	InventoryRing *ring_target;

	ring::RingItem *item_active;
	ring::RingItem *item_used;

	bool activate_selected_item_on_open;
	bool force_open;

	InventoryState()
		:
		ring(nullptr),
		ring_target(nullptr),
		item_active(nullptr),
		item_used(nullptr),
		activate_selected_item_on_open(false),
		force_open(false)
	{}

	void change_ring() {
		if (ring != ring_target && ring_target != nullptr) {
			ring = ring_target;
		}
	}
};

struct InventoryDebug : public ecs::Component {
	bool enabled;

	InventoryDebug()
		:
		enabled(false)
	{}
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
void build_inventory(int32_t ring_id, int32_t item_id, InventoryState &inventory_state, ecs::EntityManager &entity_manager);

void build_ring(ecs::Entity &ring, ring::RingState &ring_state, std::vector<ecs::Entity*> &items, const int32_t *item_id_selected = nullptr, const std::vector<const ecs::Entity*> *items_excluded = nullptr);

InventoryRing* build_ammo_ring(ecs::Entity &item, ecs::Entity &inventory, ecs::EntityManager &entity_manager);

InventoryRing* build_combo_ring(ecs::Entity &item, ecs::Entity &inventory, ecs::EntityManager &entity_manager);

void open_ring(ecs::Entity &ring, const InventoryDisplay &inventory_display, const uint32_t *duration_frames = nullptr);

void close_ring(ecs::Entity &ring, const InventoryDisplay &inventory_display, const uint32_t *duration_frames = nullptr);

void fade_in_ring(ecs::Entity &ring, uint32_t duration_frames);

void fade_out_ring(ecs::Entity &ring, uint32_t duration_frames);

void open_camera(ecs::Entity &camera, const InventoryDisplay &inventory_display, const InventoryDuration &inventory_duration);

void close_camera(ecs::Entity &camera, const InventoryDisplay &inventory_display, const InventoryDuration &inventory_duration);

void rotate_ring(ecs::Entity &ring, uint32_t duration_frames, bool clockwise);

bool sort_items_by_sort_index(const ecs::Entity *item_a, const ecs::Entity *item_b);

void add_health_bar(ecs::Entity &item);

inventory::InventoryState* get_inventory_state(ecs::EntityManager &entity_manager);

inventory::InventoryDuration* get_inventory_duration(ecs::EntityManager &entity_manager);

bool inventory_enabled(ecs::EntityManager &entity_manager);

bool debug_enabled(ecs::EntityManager &entity_manager);

}
}

#pragma pack(pop)