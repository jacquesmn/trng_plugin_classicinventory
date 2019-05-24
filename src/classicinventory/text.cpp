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

#include <StdAfx.h>

#include "text.h"

#include <regex>
#include <sstream>

#include "render.h"
#include "script.h"

namespace classicinventory {
namespace text {

TextConfig* get_text_config(
	TextType::Enum text_type,
	ecs::EntityManager &entity_manager
)
{
	const auto inventory = entity_manager.find_entity_with_component<TextConfig>();

	if (inventory) {
		return inventory->get_component<TextConfig>([&](TextConfig &config) -> bool {
			return config.type == text_type;
		});
	}

	return nullptr;
}

void add_text(
	ecs::Entity &entity,
	TextType::Enum text_type,
	const std::string &text,
	ecs::EntityManager &entity_manager
)
{
	if (text.empty()) {
		return;
	}

	const auto text_config = get_text_config(text_type, entity_manager);
	if (!text_config || !text_config->enabled) {
		return;
	}

	const auto screen_x = text_config->x;
	auto screen_y = text_config->y;

	// shift down if overlapping existing text
	// very basic at the moment, just checking if screen coords are the same
	const auto find_overlapping_text = [&entity](uint32_t x, uint32_t y) -> render::ScreenText* {
		return entity.get_component<render::ScreenText>([&](render::ScreenText &screen_text) -> bool {
			return x == screen_text.x && y == screen_text.y && screen_text.line_height > 0;
		});
	};

	render::ScreenText *overlapping_text = nullptr;
	do {
		overlapping_text = find_overlapping_text(screen_x, screen_y);

		if (overlapping_text) {
			screen_y = overlapping_text->y + overlapping_text->line_height;
		}
	} while (overlapping_text);

	entity.add_component(new render::ScreenText(
		text,
		screen_x,
		screen_y,
		text_config->size,
		text_config->colour,
		text_config->align,
		text_config->line_height
	));
}

std::string build_item_text(const std::string& item_name, const item::ItemQuantity *item_qty, bool force_qty)
{
	if (item_qty) {
		const auto qty = item_qty->get();

		if (qty == item::ITEM_QTY_UNLIMITED) {
			const auto unlimited = script::ScriptString(script::StringIndex::UNLIMITED);

			return std::regex_replace(unlimited.get_string(), std::regex("%s"), item_name);;
		}
		else if (qty > 1 || force_qty) {
			std::ostringstream item_text;
			item_text << qty << " x " << item_name;
			return item_text.str();
		}
	}

	return item_name;
}

}
}