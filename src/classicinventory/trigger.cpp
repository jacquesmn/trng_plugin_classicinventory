#include "StdAfx.h"

#include "trigger.h"

#include "ecs.h"
#include "inventory.h"
#include "item.h"
#include "render.h"

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

}
}
