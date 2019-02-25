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
#pragma pack(push, 1)

#include <trng_core.h>
#include "core.h"
#include "ecs.h"

namespace classicinventory {
namespace inventory {
struct InventoryDisplay;
}
}

namespace classicinventory {
namespace ring {
struct RingItem;
}
}

namespace classicinventory {
namespace render {

class InventoryRenderSystem : public ecs::System {
private:
	void draw_inventory(ecs::EntityManager &entity_manager) const;

	void draw_ring_item(
		const ring::RingItem &ring_item,
		const inventory::InventoryDisplay &inventory_display,
		ecs::EntityManager &entity_manager
	) const;

	void draw_item(
		ecs::Entity &item,
		const inventory::InventoryDisplay &inventory_display,
		ecs::EntityManager &entity_manager
	) const;

	void draw_texts(ecs::EntityManager &entity_manager) const;
	void draw_text(
		std::string text,
		uint32_t screen_x,
		uint32_t screen_y,
		int32_t flags_size,
		int32_t flags_colour,
		int32_t flags_align
	) const;

	void draw_bars(ecs::EntityManager &entity_manager) const;
	void draw_bar(
		uint32_t screen_x,
		uint32_t screen_y,
		uint32_t width,
		uint32_t height,
		uint32_t percent,
		uint32_t colour1,
		uint32_t colour2
	) const;

	void draw_statistics(ecs::EntityManager &entity_manager) const;
	void draw_options(ecs::EntityManager &entity_manager) const;

	void set_lighting(ecs::EntityManager &entity_manager, float ring_angle_degrees) const;
	void restore_lighting(ecs::EntityManager &entity_manager) const;
	void calculate_lighting() const;

public:
	void init(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class GameRenderSystem : public ecs::System {
private:
	void draw_pickups(ecs::EntityManager &entity_manager);

public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;
};

struct LightingLocation : public ecs::Component {
	int32_t room;
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t radius;

	StrBackupLara backup_lara;
	bool backup_lara_in_water;

	LightingLocation(
		int32_t room = 0,
		int32_t x = 0,
		int32_t y = 0,
		int32_t z = 0,
		int32_t radius = 0
	)
		:
		room(room),
		x(x),
		y(y),
		z(z),
		radius(radius)
	{}
};

struct ScreenText : public ecs::Component {
	std::string text;
	uint32_t x;
	uint32_t y;
	int32_t flags_size;
	int32_t flags_colour;
	int32_t flags_align;
	uint32_t line_height;

	ScreenText(
		std::string text,
		uint32_t x,
		uint32_t y,
		int32_t flags_size,
		int32_t flags_colour,
		int32_t flags_align,
		uint32_t line_height = 0
	)
		:
		text(text),
		x(x),
		y(y),
		flags_size(flags_size),
		flags_colour(flags_colour),
		flags_align(flags_align),
		line_height(line_height)
	{}
};

struct ScreenBar : public ecs::Component {
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	uint32_t percent;
	uint32_t colour1;
	uint32_t colour2;

	ScreenBar(
		uint32_t x,
		uint32_t y,
		uint32_t width,
		uint32_t height,
		uint32_t percent,
		uint32_t colour1,
		uint32_t colour2
	)
		:
		x(x),
		y(y),
		width(width),
		height(height),
		percent(percent),
		colour1(colour1),
		colour2(colour2)
	{}
};

struct PickupDisplay : public ecs::Component {
	int32_t item_id;
	core::Vector3D rotation;
	float alpha;
	bool active;

	PickupDisplay(
		int32_t item_id
	)
		:
		item_id(item_id),
		rotation(core::Vector3D()),
		alpha(255),
		active(true)
	{}
};

struct StatisticsScreen : public ecs::Component {
};

struct OptionsScreen : public ecs::Component {
	bool controls_only;
	bool started;
	bool finished;

	OptionsScreen(bool controls_only = false)
		:
		controls_only(controls_only),
		started(false),
		finished(false)
	{}
};

}
}

#pragma pack(pop)