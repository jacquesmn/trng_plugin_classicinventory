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

#include "trigger.h"

#include <trng_core.h>
#include "ecs.h"
#include "inventory.h"
#include "input.h"
#include "item.h"
#include "render.h"

extern TYPE_SoundEffect SoundEffect;
extern StrMyData MyData;

namespace classicinventory {
namespace trigger {

void flipeffect_increase_item_qty(
	int32_t item_id,
	int32_t qty,
	ecs::EntityManager &entity_manager
)
{
	auto item = item::get_item_by_item_id(item_id, entity_manager);
	if (!item) {
		return;
	}

	if (!item->has_component<item::ItemQuantity>()) {
		return;
	}
	auto &item_qty = *item->get_component<item::ItemQuantity>();

	item_qty.increase(qty);
}

void flipeffect_decrease_item_qty(
	int32_t item_id,
	int32_t qty,
	ecs::EntityManager &entity_manager
)
{
	auto item = item::get_item_by_item_id(item_id, entity_manager);
	if (!item) {
		return;
	}

	if (!item->has_component<item::ItemQuantity>()) {
		return;
	}
	auto &item_qty = *item->get_component<item::ItemQuantity>();

	item_qty.decrease(qty);
}

void flipeffect_set_item_qty(
	int32_t item_id,
	int32_t qty,
	ecs::EntityManager &entity_manager
)
{
	auto item = item::get_item_by_item_id(item_id, entity_manager);
	if (!item) {
		return;
	}

	if (!item->has_component<item::ItemQuantity>()) {
		return;
	}
	auto &item_qty = *item->get_component<item::ItemQuantity>();

	item_qty.set(qty);
}

void flipeffect_open_inventory_at_item(
	int32_t item_id,
	ItemSelectType::Enum select_type,
	ItemMissingResponse::Enum missing_response,
	bool open_now,
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	const auto item = item::get_item_by_item_id(item_id, entity_manager);

	item::ItemQuantity *item_qty = nullptr;
	item::ItemRing *item_ring = nullptr;
	if (item) {
		item_qty = item->get_component<item::ItemQuantity>();
		item_ring = item->get_component<item::ItemRing>();
	}

	if (!item
		|| !item_qty
		|| item_qty->get() == 0
		|| !item_ring) {
		// item not found
		if (missing_response == ItemMissingResponse::NO) {
			SoundEffect(2, nullptr, 0);
		}
		return;
	}

	MyData.Save.Local.inventory_data.ring_id_selected = item_ring->ring_id;
	MyData.Save.Local.inventory_data.item_id_selected = item_id;

	if (open_now) {
		auto inventory_state = inventory::get_inventory_state(entity_manager);

		if (inventory_state) {
			if (select_type == ItemSelectType::ACTIVATE) {
				inventory_state->activate_selected_item_on_open = true;
			}
			inventory_state->force_open = true;
		}

		// reset inventory key status, won't work if debounced by engine
		PerformFlipeffect(nullptr, 51, 14, 0);
		system_manager.get_system<input::InputUpdateSystem>()->update(entity_manager, system_manager);
		PerformFlipeffect(nullptr, 52, 14, 0);

		// press inventory key
		PerformFlipeffect(nullptr, 53, 14, 0);
	}
}

void flipeffect_show_pickup_notifier(
	int32_t item_id,
	ecs::EntityManager &entity_manager
)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	auto &pickup_display = inventory->add_component(new render::PickupDisplay(item_id));
	pickup_display.alpha = 0;
}

bool condition_item_qty_at_least(
	int32_t item_id,
	int32_t qty,
	ecs::EntityManager &entity_manager
)
{
	auto item = item::get_item_by_item_id(item_id, entity_manager);
	if (item) {
		if (item->has_component<item::ItemQuantity>()) {
			const auto item_qty = item->get_component<item::ItemQuantity>()->get();

			return item_qty == item::ITEM_QTY_UNLIMITED || item_qty >= qty;
		}
	}

	return false;
}

bool condition_item_qty_less_than(
	int32_t item_id,
	int32_t qty,
	ecs::EntityManager &entity_manager
)
{
	auto item = item::get_item_by_item_id(item_id, entity_manager);
	if (item) {
		if (item->has_component<item::ItemQuantity>()) {
			const auto item_qty = item->get_component<item::ItemQuantity>()->get();

			return item_qty != item::ITEM_QTY_UNLIMITED && item_qty < qty;
		}
	}

	return false;
}

bool condition_item_is_selected(
	int32_t item_id,
	ecs::EntityManager &entity_manager
)
{
	return MyData.Save.Local.inventory_data.item_id_selected == item_id;
}

bool condition_item_last_used(
	int32_t item_id,
	ecs::EntityManager &entity_manager
)
{
	return MyData.Save.Local.inventory_data.item_id_used == item_id;
}

bool condition_inventory_enabled(
	bool enabled,
	ecs::EntityManager& entity_manager
)
{
	return enabled == inventory::inventory_enabled(entity_manager);
}

}
}
