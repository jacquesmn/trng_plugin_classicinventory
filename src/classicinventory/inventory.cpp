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

#include "inventory.h"

#include <algorithm>
#include <sstream>

#include "camera.h"
#include "motion.h"
#include "render.h"
#include "special.h"

namespace classicinventory {
namespace inventory {

void build_inventory(
	int32_t ring_id,
	int32_t item_id,
	InventoryState &inventory_state,
	ecs::EntityManager &entity_manager
)
{
	const auto rings = entity_manager.find_entities_with_component<ring::RingData>([](const ring::RingData &ring_data) -> bool {
		return ring_data.ring_id >= ring::MIN_INVENTORY_RING_ID;
	});
	if (rings.empty()) {
		return;
	}

	const auto inventory = entity_manager.find_entity_with_component<InventoryData>();
	if (!inventory) {
		return;
	}
	auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

	// keep track of ammos for which we already have the weapon
	std::vector<const ecs::Entity*> ammos_with_weapon;
	if (!inventory_display.always_show_ammo) {
		const auto weapons = entity_manager.find_entities_with_component<item::ItemAmmo>();
		for (auto weapon_it = weapons.begin(); weapon_it != weapons.end(); ++weapon_it) {
			auto& weapon = **weapon_it;

			const auto item_qty = weapon.get_component<item::ItemQuantity>();
			if (!item_qty || item_qty->get() == 0) {
				continue;
			}

			auto item_ammos = weapon.get_components<item::ItemAmmo>();
			for (auto ammo_it = item_ammos.begin(); ammo_it != item_ammos.end(); ++ammo_it) {
				ammos_with_weapon.push_back(&(*ammo_it)->ammo_item);
			}
		}
	}

	// add carried items to rings
	for (auto ring_it = rings.begin(); ring_it != rings.end(); ++ring_it) {
		auto &ring = **ring_it;

		if (!ring.has_component<ring::RingData>()
			|| !ring.has_component<ring::RingState>()) {
			continue;
		}

		auto &ring_data = *ring.get_component<ring::RingData>();
		auto &ring_state = *ring.get_component<ring::RingState>();

		auto items = entity_manager.find_entities_with_component<item::ItemRing>([&](const item::ItemRing &item_ring) -> bool {
			return item_ring.ring_id == ring_data.ring_id;
		});

		build_ring(ring, ring_state, items, &item_id, &ammos_with_weapon);

		fade_in_ring(ring, 0);
	}

	// now link the rings together for easy navigation
	// context ring will sit separately and built dynamically based on state
	inventory->remove_components<InventoryRing>();

	inventory_state.ring = nullptr;
	inventory_state.ring_target = nullptr;
	inventory_state.item_active = nullptr;

	InventoryRing *inventory_ring_prev = nullptr;
	for (auto ring_it = rings.begin(); ring_it != rings.end(); ++ring_it) {
		auto &ring = **ring_it;

		const auto &ring_data = *ring.get_component<ring::RingData>();
		const auto ring_state = ring.get_component<ring::RingState>();

		if (!ring_state || ring_state->item_count == 0) {
			continue;
		}

		auto &inventory_ring = inventory->add_component(new InventoryRing(ring, inventory_ring_prev));
		if (inventory_ring_prev) {
			inventory_ring_prev->next = &inventory_ring;
		}
		inventory_ring_prev = &inventory_ring;

		if (ring_data.ring_id == ring_id
			|| (!inventory_state.ring && ring_data.ring_id == ring::RingId::INVENTORY)) {
			inventory_state.ring = &inventory_ring;
			inventory_state.ring_target = &inventory_ring;
		}
	}
}

void build_ring(
	ecs::Entity &ring,
	ring::RingState &ring_state,
	std::vector<ecs::Entity*> &items,
	const int32_t *item_id_selected,
	const std::vector<const ecs::Entity*> *items_excluded
)
{
	ring.remove_components<ring::RingItem>();

	ring_state.item = nullptr;
	ring_state.item_target = nullptr;
	ring_state.item_count = 0;

	if (items.empty()) {
		return;
	}

	// sort items
	std::stable_sort(items.begin(), items.end(), sort_items_by_sort_index);

	ring::RingItem *ring_item_head = nullptr;
	ring::RingItem *ring_item_prev = nullptr;
	uint32_t item_index = 0;
	for (auto it = items.begin(); it != items.end(); ++it) {
		auto &item = **it;

		// exclude items with zero quantity
		const auto item_qty = item.get_component<item::ItemQuantity>();
		if (!item_qty || item_qty->get() == 0) {
			continue;
		}

		// exclude desired items
		if (items_excluded) {
			if (std::find_if(items_excluded->begin(), items_excluded->end(), [&](const ecs::Entity *ammo_item) -> bool {
				return ammo_item->get_id() == item.get_id();
			}) != items_excluded->end()) {
				continue;
			}
		}

		// add item to ring and link together to form a loop
		auto &ring_item = ring.add_component(new ring::RingItem(ring, item, item_index++));

		if (!ring_item_head) {
			ring_item_head = &ring_item;
		}

		ring_item.prev = ring_item_prev;
		if (ring_item_prev) {
			ring_item_prev->next = &ring_item;
		}
		ring_item_prev = &ring_item;

		++ring_state.item_count;

		if (item_id_selected
			&& item.get_component<item::ItemData>([&](item::ItemData &item_data) -> bool {
			return item_data.item_id == *item_id_selected;
		})) {
			ring_state.item = &ring_item;
		}

		// change display
		item::change_item_display(item, item::ItemDisplayType::IDLE);

		// reset animations
		item::reset_item_animations(item);
	}

	// close item loop
	if (ring_item_head) {
		ring_item_head->prev = ring_item_prev;
		ring_item_prev->next = ring_item_head;
	}

	if (!ring_state.item) {
		ring_state.item = ring_item_head;
	}
	ring_state.item_target = ring_state.item;

	const auto ring_display = ring.get_component<ring::RingDisplay>();
	if (ring_display) {
		ring_display->sector = ring_state.item_count == 0 ? 0 : 360.f / ring_state.item_count;
	}
}

InventoryRing* build_ammo_ring(
	ecs::Entity &item,
	ecs::Entity &inventory,
	ecs::EntityManager &entity_manager
)
{
	auto context_ring = entity_manager.find_entity_with_component<ring::RingData>([](const ring::RingData &ring_data) -> bool {
		return ring_data.ring_id == ring::RingId::CONTEXT;
	});
	if (context_ring) {
		if (context_ring->has_component<ring::RingState>()) {
			auto &context_ring_state = *context_ring->get_component<ring::RingState>();

			std::vector<ecs::Entity*> ammo_items;
			int32_t *ammo_loaded_item_id = nullptr;

			auto item_ammos = item.get_components<item::ItemAmmo>();
			for (auto it = item_ammos.begin(); it != item_ammos.end(); ++it) {
				auto &item_ammo = **it;
				auto &ammo_item = item_ammo.ammo_item;

				ammo_items.push_back(&ammo_item);

				if (!ammo_loaded_item_id
					&& item_ammo.loaded()
					&& ammo_item.has_component<item::ItemData>()) {
					auto &item_data = *ammo_item.get_component<item::ItemData>();

					ammo_loaded_item_id = &item_data.item_id;
				}
			}

			build_ring(*context_ring, context_ring_state, ammo_items, ammo_loaded_item_id);

			if (context_ring_state.item_count > 0) {
				auto inventory_context_ring = inventory.get_component<InventoryRing>([&](InventoryRing &inventory_ring) -> bool {
					return &inventory_ring.ring == context_ring;
				});
				if (!inventory_context_ring) {
					inventory_context_ring = &inventory.add_component(new InventoryRing(*context_ring));
				}

				return inventory_context_ring;
			}
		}
	}

	return nullptr;
}

InventoryRing* build_combo_ring(
	ecs::Entity &item,
	ecs::Entity &inventory,
	ecs::EntityManager &entity_manager
)
{
	auto context_ring = entity_manager.find_entity_with_component<ring::RingData>([](const ring::RingData &ring_data) -> bool {
		return ring_data.ring_id == ring::RingId::CONTEXT;
	});

	if (context_ring) {
		if (context_ring->has_component<ring::RingState>()) {
			auto &context_ring_state = *context_ring->get_component<ring::RingState>();

			std::vector<ecs::Entity*> combo_items;

			auto combos = inventory.get_components<item::ComboData>();
			for (auto it = combos.begin(); it != combos.end(); ++it) {
				auto &combo = **it;

				// skip if disabled
				if (!combo.enabled) {
					continue;
				}

				auto add_to_list = [&](ecs::Entity *combo_item) -> void {
					// skip if zero quantity
					if (!combo_item
						|| !combo_item->has_component<item::ItemQuantity>()
						|| !combo_item->get_component<item::ItemQuantity>([](item::ItemQuantity &item_qty) -> bool {
						return item_qty.get() != 0;
					})) {
						return;
					}

					// skip if active item
					if (combo_item->get_id() == item.get_id()) {
						return;
					}

					// skip if already in list
					if (std::find(combo_items.begin(), combo_items.end(), combo_item) != combo_items.end()) {
						return;
					}

					combo_items.push_back(combo_item);
				};

				add_to_list(&combo.item_first);
				add_to_list(&combo.item_second);
			}

			build_ring(*context_ring, context_ring_state, combo_items);

			if (context_ring_state.item_count > 0) {
				auto inventory_context_ring = inventory.get_component<InventoryRing>([&](InventoryRing &inventory_ring) -> bool {
					return &inventory_ring.ring == context_ring;
				});
				if (!inventory_context_ring) {
					inventory_context_ring = &inventory.add_component(new InventoryRing(*context_ring));
				}

				return inventory_context_ring;
			}
		}
	}

	return nullptr;
}

void open_camera(
	ecs::Entity &camera,
	const InventoryDisplay &inventory_display,
	const uint32_t *duration_frames
)
{
	auto camera_view = camera.get_component<camera::CameraView>();
	if (!camera_view) {
		return;
	}

	if (duration_frames && *duration_frames) {
		camera.add_component(new motion::Motion(
			camera_view->position.x,
			inventory_display.camera_pos_closed.x,
			inventory_display.camera_pos_opened.x,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->position.y,
			inventory_display.camera_pos_closed.y,
			inventory_display.camera_pos_opened.y,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->position.z,
			inventory_display.camera_pos_closed.z,
			inventory_display.camera_pos_opened.z,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->target.x,
			inventory_display.camera_tgt_closed.x,
			inventory_display.camera_tgt_opened.x,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->target.y,
			inventory_display.camera_tgt_closed.y,
			inventory_display.camera_tgt_opened.y,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->target.z,
			inventory_display.camera_tgt_closed.z,
			inventory_display.camera_tgt_opened.z,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->fov,
			inventory_display.camera_fov_closed,
			inventory_display.camera_fov_opened,
			*duration_frames)
		);
	}
	else {
		camera_view->position.x = inventory_display.camera_pos_opened.x;
		camera_view->position.y = inventory_display.camera_pos_opened.y;
		camera_view->position.z = inventory_display.camera_pos_opened.z;
		camera_view->target.x = inventory_display.camera_tgt_opened.x;
		camera_view->target.y = inventory_display.camera_tgt_opened.y;
		camera_view->target.z = inventory_display.camera_tgt_opened.z;
		camera_view->fov = inventory_display.camera_fov_opened;
	}
}

void close_camera(
	ecs::Entity &camera,
	const InventoryDisplay &inventory_display,
	const uint32_t *duration_frames
)
{
	auto camera_view = camera.get_component<camera::CameraView>();
	if (!camera_view) {
		return;
	}

	if (duration_frames && *duration_frames) {
		camera.add_component(new motion::Motion(
			camera_view->position.x,
			inventory_display.camera_pos_opened.x,
			inventory_display.camera_pos_closed.x,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->position.y,
			inventory_display.camera_pos_opened.y,
			inventory_display.camera_pos_closed.y,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->position.z,
			inventory_display.camera_pos_opened.z,
			inventory_display.camera_pos_closed.z,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->target.x,
			inventory_display.camera_tgt_opened.x,
			inventory_display.camera_tgt_closed.x,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->target.y,
			inventory_display.camera_tgt_opened.y,
			inventory_display.camera_tgt_closed.y,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->target.z,
			inventory_display.camera_tgt_opened.z,
			inventory_display.camera_tgt_closed.z,
			*duration_frames)
		);
		camera.add_component(new motion::Motion(
			camera_view->fov,
			inventory_display.camera_fov_opened,
			inventory_display.camera_fov_closed,
			*duration_frames)
		);
	}
	else {
		camera_view->position.x = inventory_display.camera_pos_closed.x;
		camera_view->position.y = inventory_display.camera_pos_closed.y;
		camera_view->position.z = inventory_display.camera_pos_closed.z;
		camera_view->target.x = inventory_display.camera_tgt_closed.x;
		camera_view->target.y = inventory_display.camera_tgt_closed.y;
		camera_view->target.z = inventory_display.camera_tgt_closed.z;
		camera_view->fov = inventory_display.camera_fov_closed;
	}
}

void open_ring(
	ecs::Entity &ring,
	const InventoryDisplay &inventory_display,
	const uint32_t *duration_frames
)
{
	auto ring_display = ring.get_component<ring::RingDisplay>();
	if (!ring_display) {
		return;
	}

	const auto ring_state = ring.get_component<ring::RingState>();
	if (!ring_state || ring_state->item_count == 0) {
		return;
	}

	if (duration_frames) {
		ring.add_component(new motion::Motion(
			ring_display->radius,
			inventory_display.ring_radius_closed,
			inventory_display.ring_radius_opened,
			*duration_frames)
		);
		ring.add_component(new motion::Motion(
			ring_display->rotation.y,
			(ring_state->item->item_index * -ring_display->sector),
			(ring_state->item->item_index * -ring_display->sector) - inventory_display.ring_orient_opened.y,
			*duration_frames)
		);
	}
	else {
		ring_display->radius = inventory_display.ring_radius_opened;
		ring_display->rotation.y = (ring_state->item->item_index * -ring_display->sector) - inventory_display.ring_orient_opened.y;

		if (ring_display->alpha != 255) {
			ring_display->alpha = 255;
		}
	}
}

void close_ring(
	ecs::Entity &ring,
	const InventoryDisplay &inventory_display,
	const uint32_t *duration_frames
)
{
	auto ring_display = ring.get_component<ring::RingDisplay>();
	if (!ring_display) {
		return;
	}

	const auto ring_state = ring.get_component<ring::RingState>();
	if (!ring_state || ring_state->item_count == 0) {
		return;
	}

	if (duration_frames && *duration_frames) {
		ring.add_component(new motion::Motion(
			ring_display->radius,
			inventory_display.ring_radius_opened,
			inventory_display.ring_radius_closed,
			*duration_frames)
		);
		ring.add_component(new motion::Motion(
			ring_display->rotation.y,
			(ring_state->item->item_index * -ring_display->sector) + inventory_display.ring_orient_opened.y,
			(ring_state->item->item_index * -ring_display->sector),
			*duration_frames)
		);
	}
	else {
		ring_display->radius = inventory_display.ring_radius_closed;
		ring_display->rotation.y = (ring_state->item->item_index * -ring_display->sector);

		if (ring_display->alpha != 255) {
			ring_display->alpha = 0;
		}
	}
}

void fade_in_ring(
	ecs::Entity &ring,
	uint32_t duration_frames
)
{
	const auto ring_state = ring.get_component<ring::RingState>();
	if (!ring_state) {
		return;
	}

	auto ring_display = ring.get_component<ring::RingDisplay>();
	if (!ring_display) {
		return;
	}

	ring.add_component(new motion::Motion(
		ring_display->alpha,
		0,
		255,
		duration_frames)
	);
}

void fade_out_ring(
	ecs::Entity &ring,
	uint32_t duration_frames
)
{
	const auto ring_state = ring.get_component<ring::RingState>();
	if (!ring_state) {
		return;
	}

	auto ring_display = ring.get_component<ring::RingDisplay>();
	if (!ring_display) {
		return;
	}

	ring.add_component(new motion::Motion(
		ring_display->alpha,
		255,
		0,
		duration_frames)
	);
}

void rotate_ring(
	ecs::Entity &ring,
	uint32_t duration_frames,
	bool clockwise
)
{
	auto ring_display = ring.get_component<ring::RingDisplay>();
	if (!ring_display) {
		return;
	}

	auto ring_state = ring.get_component<ring::RingState>();
	if (!ring_state || ring_state->item_count == 0) {
		return;
	}

	if (clockwise) {
		ring_state->item_target = ring_state->item->prev;

		ring.add_component(new motion::Motion(
			ring_display->rotation.y,
			ring_display->rotation.y,
			ring_display->rotation.y + ring_display->sector,
			duration_frames)
		);
	}
	else {
		ring_state->item_target = ring_state->item->next;

		ring.add_component(new motion::Motion(
			ring_display->rotation.y,
			ring_display->rotation.y,
			ring_display->rotation.y - ring_display->sector,
			duration_frames)
		);
	}

	item::restore_item_spin(ring_state->item->item, 0, 1.5);
}

bool sort_items_by_sort_index(const ecs::Entity *item_a, const ecs::Entity *item_b)
{
	auto &item_ring_a = *item_a->get_component<item::ItemRing>();
	auto &item_ring_b = *item_b->get_component<item::ItemRing>();

	return item_ring_a.sort_index < item_ring_b.sort_index;
}

void add_health_bar(ecs::Entity& item)
{
	if (item.has_component<special::HealthData>()) {
		const auto &health_data = *item.get_component<special::HealthData>();

		if (health_data.health_points != 0 || health_data.poison_points != 0 || health_data.cure_poison) {
			item.add_component(new render::HealthBar(Trng.pGlobTomb4->pAdr->pLara->Health * 100 / 1000));
		}

		if (health_data.air_points != 0) {
			item.add_component(new render::AirBar(*Trng.pGlobTomb4->pAdr->pAirAvailable * 100 / 1800));
		}
	}
}

InventoryData* get_inventory_data(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<InventoryData>();
	if (inventory) {
		return inventory->get_component<InventoryData>();
	}

	return nullptr;
}

InventoryState* get_inventory_state(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<InventoryState>();
	if (inventory) {
		return inventory->get_component<InventoryState>();
	}

	return nullptr;
}

InventoryDuration* get_inventory_duration(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<InventoryDuration>();
	if (inventory) {
		return inventory->get_component<InventoryDuration>();
	}

	return nullptr;
}

bool inventory_enabled(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<InventoryData>();
	if (inventory) {
		const auto &inventory_data = *inventory->get_component<InventoryData>();

		return inventory_data.enabled;
	}

	return false;
}

bool debug_enabled(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<InventoryDebug>();
	if (inventory) {
		const auto &inventory_debug = *inventory->get_component<InventoryDebug>();

		return inventory_debug.enabled;
	}

	return false;
}

bool stats_enabled(ecs::EntityManager& entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<InventoryData>();
	if (inventory) {
		const auto &inventory_data = *inventory->get_component<InventoryData>();

		return inventory_data.enable_stats;
	}

	return false;
}

}
}
