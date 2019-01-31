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

#include "item.h"

#include <trng_core.h>
#include "motion.h"

extern TYPE_convert_obj_to_invobj convert_obj_to_invobj;

namespace classicinventory {
namespace item {

int32_t ItemQuantity::get() const
{
	if (get_quantity) {
		const auto qty = get_quantity();

		if (qty == ITEM_QTY_UNLIMITED) {
			return qty;
		}

		return core::round(float(qty) / divider);
	}

	return 0;
}

bool ItemQuantity::set(int32_t qty) const
{
	if (qty != ITEM_QTY_UNLIMITED) {
		qty = divider * qty;
	}

	if (set_quantity
		&& ((qty >= quantity_min && qty <= quantity_max) || (qty == ITEM_QTY_UNLIMITED && supports_unlimited))) {
		set_quantity(qty);

		return true;
	}

	return false;
}

bool ItemQuantity::increase(int32_t qty) const {
	const auto qty_old = get();

	if (qty_old != ITEM_QTY_UNLIMITED) {
		set(min(quantity_max, qty_old + qty));

		return get() != qty_old;
	}

	return false;
}

bool ItemQuantity::decrease(int32_t qty) const {
	const auto qty_old = get();

	if (qty_old != ITEM_QTY_UNLIMITED) {
		set(max(quantity_min, qty_old - qty));

		return get() != qty_old;
	}

	return false;
}

bool ItemQuantity::unlimited() const {
	return set(ITEM_QTY_UNLIMITED);
}

bool ItemQuantity::zero() const {
	return set(0);
}

int32_t tr4_slot_to_item_id(uint32_t tr4_slot)
{
	if (tr4_slot == UINT32_MAX) {
		return ItemId::NONE;
	}

	return tr4_invobj_to_item_id(convert_obj_to_invobj(tr4_slot));
}

int32_t tr4_invobj_to_item_id(uint32_t tr4_invobj)
{
	if (tr4_invobj > 118) {
		return ItemId::NONE;
	}

	return tr4_invobj - abs(MIN_INVENTORY_ITEM_ID);
}

uint32_t item_id_to_tr4_slot(int32_t item_id)
{
	const auto tr4_invobj = item_id_to_tr4_invobj(item_id);
	if (tr4_invobj > 118) {
		return UINT32_MAX;
	}

	return Trng.pGlobTomb4->pAdr->pVetStructInventoryItems[tr4_invobj].Slot;
}

uint32_t item_id_to_tr4_invobj(int32_t item_id)
{
	if (item_id >= ItemId::NONE) {
		return UINT32_MAX;
	}

	return item_id + abs(MIN_INVENTORY_ITEM_ID);
}

uint32_t item_id_to_item_index(int32_t item_id)
{
	const uint32_t item_id_count = MAX_INVENTORY_ITEM_ID - MIN_INVENTORY_ITEM_ID;

	return (item_id - MAX_INVENTORY_ITEM_ID) + item_id_count;
}

uint32_t item_index_to_item_id(int32_t item_index)
{
	const uint32_t item_id_count = MAX_INVENTORY_ITEM_ID - MIN_INVENTORY_ITEM_ID;

	return (item_index + MAX_INVENTORY_ITEM_ID) - item_id_count;
}

ecs::Entity* get_item_by_item_id(int32_t item_id, ecs::EntityManager &entity_manager)
{
	return entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
}

item::ItemModelConfig* get_item_model_config(
	ecs::Entity &item,
	item::ItemModelType::Enum type,
	item::ItemModelType::Enum type_default
)
{
	auto item_model = item.get_component<item::ItemModelConfig>([&](item::ItemModelConfig &config) -> bool {
		return config.type == type;
	});
	if (!item_model) {
		item_model = item.get_component<item::ItemModelConfig>([&](item::ItemModelConfig &config) -> bool {
			return config.type == type_default;
		});
	}

	return item_model;
}

item::ItemDisplayConfig* get_item_display_config(
	ecs::Entity &item,
	item::ItemDisplayType::Enum type,
	item::ItemDisplayType::Enum type_default
)
{
	auto item_display = item.get_component<item::ItemDisplayConfig>([&](item::ItemDisplayConfig &config) -> bool {
		return config.type == type;
	});
	if (!item_display) {
		item_display = item.get_component<item::ItemDisplayConfig>([&](item::ItemDisplayConfig &config) -> bool {
			return config.type == type_default;
		});
	}

	return item_display;
}

void change_item_model(
	ecs::Entity &item,
	item::ItemModelType::Enum type
)
{
	const auto item_model = item.get_component<item::ItemModel>();
	if (!item_model) {
		return;
	}

	const auto model_config = get_item_model_config(item, type);
	if (!model_config) {
		return;
	}

	item_model->config = model_config;
}

void change_item_display(
	ecs::Entity &item,
	item::ItemDisplayType::Enum type,
	bool keep_pos,
	bool keep_orient,
	bool keep_rot,
	bool keep_tilt,
	bool keep_scale,
	bool keep_motion,
	bool keep_alpha
)
{
	const auto item_display = item.get_component<item::ItemDisplay>();
	if (!item_display) {
		return;
	}

	const auto display_config = item::get_item_display_config(item, type);
	if (!display_config) {
		return;
	}

	item_display->config = display_config;

	if (!keep_pos) {
		item_display->pos = display_config->pos;
	}
	if (!keep_orient) {
		item_display->orient = display_config->orient;
	}
	if (!keep_rot) {
		item_display->rot = core::Vector3D();
	}
	if (!keep_tilt) {
		item_display->tilt = display_config->tilt;
	}
	if (!keep_scale) {
		item_display->scale = display_config->scale;
	}
	if (!keep_motion) {
		remove_item_motion(item);
	}
	if (!keep_alpha) {
		item_display->alpha_enabled = true;
	}

	item_display->spin = display_config->spin;
}

void remove_item_motion(ecs::Entity &item, bool keep_background)
{
	item.remove_components<motion::Motion>([&](const motion::Motion &motion) -> bool {
		if (keep_background) {
			return !motion.background;
		}
		return true;
	});
}

void deactivate_item_actions(ecs::Entity &item)
{
	auto item_actions = item.get_components<item::ItemAction>();

	std::for_each(item_actions.begin(), item_actions.end(), [](item::ItemAction *action) -> void {
		action->active = false;
	});
}

void reset_item_animation(ecs::Entity &item)
{
	const auto item_anims = item.get_components<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
		return item_animation.active;
	});

	std::for_each(item_anims.begin(), item_anims.end(), [](item::ItemAnimation *item_animation) -> void {
		item_animation->active = false;
	});
}

void spin_item(ecs::Entity &item, uint32_t frames)
{
	if (item.has_component<item::ItemDisplay>()) {
		auto &item_display = *item.get_component<item::ItemDisplay>();

		if (!item_display.spin) {
			return;
		}

		// check if already spinning
		auto motion_spin = item.get_component<motion::Motion>([&](motion::Motion &motion) -> bool {
			return &motion.value == &item_display.rot.y;
		});

		if (motion_spin) {
			// resume spin
			if (motion_spin->restoring) {
				motion_spin->resume(motion::Motion::FORWARD, frames);

				motion_spin->loop = true;
			}
		}
		else {
			// start spin
			item.add_component(new motion::Motion(
				item_display.rot.y,
				0,
				360,
				frames,
				0,
				motion::Motion::FORWARD,
				true)
			);
		}
	}
}

void restore_item_spin(ecs::Entity &item, uint32_t frames, float speed)
{
	auto item_motion = item.get_component<motion::Motion>([](const motion::Motion &motion) -> bool {
		return motion.loop && !motion.background;
	});

	if (item_motion) {
		item_motion->restore(frames);
		if (speed != 1) {
			item_motion->accelerate(speed);
		}
	}
}

ItemAmmo* get_loaded_ammo(ecs::Entity &weapon_item)
{
	return weapon_item.get_component<item::ItemAmmo>([](item::ItemAmmo &item_ammo) -> bool {
		return item_ammo.loaded();
	});
}

bool load_ammo(ecs::Entity &weapon_item, int32_t ammo_item_id)
{
	const auto ammo = weapon_item.get_component<item::ItemAmmo>([&](item::ItemAmmo &item_ammo) -> bool {
		return item_ammo.ammo_item.get_component<item::ItemData>([&](item::ItemData &item_data) -> bool {
			return item_data.item_id == ammo_item_id;
		}) != nullptr;
	});

	if (ammo) {
		ammo->load();

		return true;
	}

	return false;
}

void unload_ammo(ecs::Entity &weapon_item)
{
	const auto ammos = weapon_item.get_components<item::ItemAmmo>([&](item::ItemAmmo &ammo) -> bool {
		return ammo.loaded();
	});
	
	std::for_each(ammos.begin(), ammos.end(), [](item::ItemAmmo *ammo) -> void {
		ammo->unload();
	});
}

}
}