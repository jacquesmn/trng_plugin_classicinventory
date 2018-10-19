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

#include "ecs.h"
#include "core.h"
#include "script.h"

namespace classicinventory {
namespace ring {

const int32_t MIN_INVENTORY_RING_ID = 1;
const int32_t MAX_INVENTORY_RING_ID = 10;

namespace RingId {
enum Enum {
	CONTEXT = -2,
	NONE,
	OPTIONS = MIN_INVENTORY_RING_ID,
	DOCUMENTS,
	INVENTORY,
	WEAPONS,
	ITEMS,
	MISC1,
	MISC2,
	MISC3,
	MISC4,
	MISC5
};
}

static const char *DEFAULT_RING_NAMES[MAX_INVENTORY_RING_ID] = {
	"Options",
	"Documents",
	"Inventory",
	"Weapons",
	"Items",
	"Misc 1",
	"Misc 2",
	"Misc 3",
	"Misc 4",
	"Misc 5"
};

struct RingData : public ecs::Component {
	int32_t ring_id;
	script::ScriptString name;

	RingData(
		int32_t ring_id,
		script::ScriptString name = script::ScriptString()
	)
		:
		ring_id(ring_id),
		name(name)
	{}
};

struct RingDisplay : public ecs::Component {
	core::Vector3D position;
	core::Vector3D rotation;
	float radius;
	float sector;
	float alpha;

	RingDisplay(
		core::Vector3D position = core::Vector3D(),
		core::Vector3D rotation = core::Vector3D(),
		float radius = 0,
		float sector = 0
	)
		:
		position(position),
		rotation(rotation),
		radius(radius),
		sector(sector),
		alpha(255)
	{}
};

struct RingItem : public ecs::Component {
	ecs::Entity &ring;
	ecs::Entity &item;
	uint32_t item_index;

	RingItem *next;
	RingItem *prev;

	RingItem(
		ecs::Entity &ring,
		ecs::Entity &item,
		uint32_t item_index,
		RingItem *next = nullptr,
		RingItem *prev = nullptr
	)
		:
		ring(ring),
		item(item),
		item_index(item_index),
		next(next),
		prev(prev)
	{}
};

struct RingState : public ecs::Component {
	RingItem *item;
	RingItem *item_target;

	uint32_t item_count;

	RingState()
		:
		item(nullptr),
		item_target(nullptr),
		item_count(0)
	{}

	void change_item() {
		if (item != item_target && item_target != nullptr) {
			item = item_target;
		}
	}
};

}
}

#pragma pack(pop)