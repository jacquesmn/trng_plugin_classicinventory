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
#include "item.h"

namespace classicinventory {
namespace text {

namespace TextType {
enum Enum {
	RING_NAME = 1,
	ITEM_NAME_IDLE,
	ITEM_DESC_IDLE,
	ITEM_AMMO_IDLE,
	ITEM_NAME_ACTIVE,
	ITEM_DESC_ACTIVE,
	ITEM_AMMO_ACTIVE,
	EXAMINE_1,
	EXAMINE_2,
	EXAMINE_3,
	ACTION_MENU,
	ACTION_MENU_HIGHLIGHT,
	CONTEXT_ACTION,
	SPECIAL_ACTION,
	NAV_UP_LEFT,
	NAV_UP_RIGHT,
	NAV_DOWN_LEFT,
	NAV_DOWN_RIGHT,
	NAV_LEFT,
	NAV_RIGHT,
	NONE
};
}

struct TextConfig : public ecs::Component {
	const TextType::Enum type;
	uint32_t x;
	uint32_t y;
	int32_t size;
	int32_t colour;
	int32_t align;
	uint32_t line_height;
	bool enabled;

	TextConfig(
		TextType::Enum type,
		uint32_t x = 0,
		uint32_t y = 0,
		int32_t size = 0,
		int32_t colour = enumFC.WHITE,
		int32_t align = enumFTS.ALIGN_CENTER,
		uint32_t line_height = 0
	)
		:
		type(type),
		x(x),
		y(y),
		size(size),
		colour(colour),
		align(align),
		line_height(line_height),
		enabled(true)
	{}
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------

TextConfig* get_text_config(TextType::Enum text_type, ecs::EntityManager &entity_manager);

void add_text(ecs::Entity &entity, TextType::Enum text_type, const std::string &text, ecs::EntityManager &entity_manager);

std::string build_item_text(const std::string& item_name, const item::ItemQuantity *item_qty = nullptr, bool force_qty = false);

}
}

#pragma pack(pop)