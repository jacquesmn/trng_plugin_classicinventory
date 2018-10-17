#include "StdAfx.h"

#include "trigger.h"

#include <trng_core.h>
#include "ecs.h"
#include "inventory.h"
#include "item.h"
#include "render.h"

extern TYPE_SoundEffect SoundEffect;
extern StrMyData MyData;

namespace classicinventory {
namespace trigger {

void increase_item_qty(
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

	item_qty.increment(qty);
}

void decrease_item_qty(
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

	item_qty.decrement(qty);
}

void set_item_qty(
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

void popup_inventory_at_item(
	int32_t item_id,
	ItemSelectType::Enum select_type,
	ItemMissingResponse::Enum missing_response,
	ecs::EntityManager &entity_manager
)
{
	const auto item = item::get_item_by_item_id(item_id, entity_manager);
	const auto item_qty = item->get_component<item::ItemQuantity>();
	const auto item_ring = item->get_component<item::ItemRing>();

	if (!item
		|| !item_qty
		|| item_qty->get_quantity() == 0
		|| !item_ring) {
		// item not found
		if (missing_response == ItemMissingResponse::NO) {
			SoundEffect(2, nullptr, 0);
		}
		return;
	}

	MyData.Save.Local.inventory_data.ring_id_selected = item_ring->ring_id;
	MyData.Save.Local.inventory_data.item_id_selected = item_id;

	if (select_type == ItemSelectType::ACTIVATE) {
		auto inventory_state = inventory::get_inventory_state(entity_manager);
		if (inventory_state) {
			inventory_state->activate_selected_item_on_open = true;
		}
	}

	// press inventory key
	PerformFlipeffect(nullptr, 53, 14, 0);
}

void show_pickup_notifier(
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

}
}
