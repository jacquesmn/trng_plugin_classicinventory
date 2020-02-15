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

std::string trim(std::string text)
{
	text.erase(text.find_last_not_of("\t\n\v\f\r ") + 1);
	text.erase(0, text.find_first_not_of("\t\n\v\f\r "));

	return text;
}

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

std::string build_item_text(
	TextType::Enum text_type,
	ecs::EntityManager &entity_manager,
	const std::string& item_name,
	const item::ItemQuantity *item_qty,
	bool force_qty
)
{
	const auto text_config = get_text_config(text_type, entity_manager);
	if (!text_config || !text_config->enabled) {
		return std::string();
	}

	const auto text_template = text_config->text_template.get_string();

	const auto regex_name = std::regex("%s|%name");
	const auto regex_qty = std::regex("%qty");

	auto template_name = std::string("%name");
	auto template_qty = std::string("%qty x");
	auto template_unlimited = script::ScriptString(script::StringIndex::UNLIMITED).get_string();

	TextConfig *text_config_qty = nullptr;
	switch (text_type) {
	case TextType::ITEM_NAME_IDLE:
	case TextType::ITEM_AMMO_IDLE:
		text_config_qty = get_text_config(TextType::ITEM_QTY_IDLE, entity_manager);
		break;
	case TextType::ITEM_NAME_ACTIVE:
	case TextType::ITEM_AMMO_ACTIVE:
		text_config_qty = get_text_config(TextType::ITEM_QTY_ACTIVE, entity_manager);
		break;
	case TextType::ITEM_QTY_IDLE:
	case TextType::ITEM_QTY_ACTIVE:
		text_config_qty = text_config;
	default:
		break;
	}
	if (text_config_qty) {
		template_qty = text_config_qty->text_template.get_string();
	}

	auto text_name = trim(std::regex_replace(template_name, regex_name, item_name));
	
	auto text_qty = std::string();
	if (item_qty) {
		const auto qty = item_qty->get();
		
		if (qty == item::ITEM_QTY_UNLIMITED) {
			text_qty = trim(std::regex_replace(template_unlimited, regex_name, std::string()));
		}
		else if (qty > 1 || force_qty) {
			std::ostringstream stream_qty;
			stream_qty << qty;

			text_qty = template_qty;
			text_qty = trim(std::regex_replace(text_qty, regex_qty, stream_qty.str()));
			text_qty = trim(std::regex_replace(text_qty, regex_name, std::string()));
		}
	}

	auto text = text_template;
	text = std::regex_replace(text, regex_name, text_name);
	text = std::regex_replace(text, regex_qty, text_qty);

	text = trim(text);

	return text;
}

}
}