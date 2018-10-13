#pragma once
#pragma pack(push,1)

#include "ecs.h"

namespace classicinventory {
namespace action {

bool use_health(ecs::Entity &item);
bool use_flare(ecs::Entity &item);
bool use_binoculars(ecs::Entity &item);
bool equip_weapon(ecs::Entity &item);
bool exchange_waterskins(ecs::Entity &item);
bool load_game();
bool save_game();
bool exit_to_title();

}
}

#pragma pack(pop)