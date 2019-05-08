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

#include <trng_core.h>
#include "ecs.h"

namespace classicinventory {
namespace sound {

namespace SfxType {
enum Enum {
	INVENTORY_OPEN = 1,
	INVENTORY_CLOSE,
	RING_ROTATE,
	RING_CHANGE,
	ITEM_ACTIVATE,
	ITEM_ACTIVE,
	ITEM_CANCEL,
	MENU_CHANGE,
	LOAD_AMMO,
	COMBINE,
	SEPARATE,
	FAIL,
	NONE
};
}

class SoundSystem : public ecs::System {
public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

struct SoundFX : public ecs::Component {
	int32_t sound_id;
	bool stop;

	SoundFX(
		int32_t sound_id,
		bool stop = false
	)
		:
		sound_id(sound_id),
		stop(stop)
	{}
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
StrSoundInfos* find_tr4_sound_info(int32_t sound_id);

}
}

#pragma pack(pop)