#pragma once
#pragma pack(push,1)

#include "ecs.h"

namespace classicinventory {
namespace trigger {

void flipeffect_increase_item_qty(int32_t item_id, int32_t qty, ecs::EntityManager &entity_manager);

}
}

#pragma pack(pop)