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

void flipeffect_increase_item_qty(
	int32_t item_id,
	int32_t qty,
	ecs::EntityManager &entity_manager
)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	auto item = entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	if (!item->has_component<item::ItemQuantity>()) {
		return;
	}
	auto &item_qty = *item->get_component<item::ItemQuantity>();

	if (item_qty.increment(qty)) {
		auto &pickup_display = inventory->add_component(new render::PickupDisplay(item_id));
		pickup_display.position.x = 850;
		pickup_display.position.y = 850;
	}
}

void flipeffect_popup_inventory_activate_item(
	int32_t item_id,
	ecs::EntityManager &entity_manager
)
{
	auto item = item::get_item_by_item_id(item_id, entity_manager);

	if (item
		&& item->has_component<item::ItemQuantity>()
		&& item->has_component<item::ItemRing>()) {
		auto &item_qty = *item->get_component<item::ItemQuantity>();
		auto &item_ring = *item->get_component<item::ItemRing>();

		if (item_qty.get_quantity() != 0) {
			MyData.Save.Local.inventory_ring_id_selected = item_ring.ring_id;
			MyData.Save.Local.inventory_item_id_selected = item_id;

			auto inventory_state = inventory::get_inventory_state(entity_manager);
			if (inventory_state) {
				inventory_state->activate_selected_item_on_open = true;
			}

			// press inventory key
			PerformFlipeffect(nullptr, 53, 14, 0);

			return;
		}
	}

	// item not found
	SoundEffect(2, nullptr, 0);
}

}
}
