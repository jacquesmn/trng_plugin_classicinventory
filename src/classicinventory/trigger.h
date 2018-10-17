#pragma once
#pragma pack(push,1)

#include "ecs.h"

namespace classicinventory {
namespace trigger {

namespace ItemSelectType {
enum Enum {
	SELECT = 1,
	ACTIVATE
};
}

namespace ItemMissingResponse {
enum Enum {
	SILENCE = 1,
	NO
};
}

void increase_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

void decrease_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

void set_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

void popup_inventory_at_item(
	int32_t item_id,
	ItemSelectType::Enum select_type,
	ItemMissingResponse::Enum missing_response,
	ecs::EntityManager &entity_manager
);

void show_pickup_notifier(int32_t item_id, ecs::EntityManager &entity_manager);

}
}

#pragma pack(pop)