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

#include <functional>

#include "core.h"
#include "ecs.h"
#include "script.h"
#include "sound.h"

struct StrSlot;

namespace classicinventory {
namespace item {

const int32_t MIN_INVENTORY_ITEM_ID = -120; // let user defined IDs start at 0, -1 is used for IGNORE
const int32_t MAX_INVENTORY_ITEM_ID = 127;
const int32_t ITEM_QTY_UNLIMITED = -1;

namespace ItemId {
enum Enum {
	UZI = MIN_INVENTORY_ITEM_ID,
	PISTOLS,
	SHOTGUN,
	REVOLVER,
	REVOLVER_LASERSIGHT_COMBO,
	CROSSBOW,
	CROSSBOW_LASERSIGHT_COMBO,
	GRENADE_GUN,
	SHOTGUN_AMMO1,
	SHOTGUN_AMMO2,
	GRENADE_GUN_AMMO1,
	GRENADE_GUN_AMMO2,
	GRENADE_GUN_AMMO3,
	CROSSBOW_AMMO1,
	CROSSBOW_AMMO2,
	CROSSBOW_AMMO3,
	REVOLVER_AMMO,
	UZI_AMMO,
	PISTOLS_AMMO,
	LASERSIGHT,
	BIGMEDI,
	SMALLMEDI,
	BINOCULARS,
	FLARE_INV,
	COMPASS,
	MEMCARD_LOAD_INV,
	MEMCARD_SAVE_INV,
	WATERSKIN1_EMPTY,
	WATERSKIN1_1,
	WATERSKIN1_2,
	WATERSKIN1_3,
	WATERSKIN2_EMPTY,
	WATERSKIN2_1,
	WATERSKIN2_2,
	WATERSKIN2_3,
	WATERSKIN2_4,
	WATERSKIN2_5,
	PUZZLE1,
	PUZZLE2,
	PUZZLE3,
	PUZZLE4,
	PUZZLE5,
	PUZZLE6,
	PUZZLE7,
	PUZZLE8,
	PUZZLE9,
	PUZZLE10,
	PUZZLE11,
	PUZZLE12,
	PUZZLE1_COMBO1,
	PUZZLE1_COMBO2,
	PUZZLE2_COMBO1,
	PUZZLE2_COMBO2,
	PUZZLE3_COMBO1,
	PUZZLE3_COMBO2,
	PUZZLE4_COMBO1,
	PUZZLE4_COMBO2,
	PUZZLE5_COMBO1,
	PUZZLE5_COMBO2,
	PUZZLE6_COMBO1,
	PUZZLE6_COMBO2,
	PUZZLE7_COMBO1,
	PUZZLE7_COMBO2,
	PUZZLE8_COMBO1,
	PUZZLE8_COMBO2,
	KEY1,
	KEY2,
	KEY3,
	KEY4,
	KEY5,
	KEY6,
	KEY7,
	KEY8,
	KEY9,
	KEY10,
	KEY11,
	KEY12,
	KEY1_COMBO1,
	KEY1_COMBO2,
	KEY2_COMBO1,
	KEY2_COMBO2,
	KEY3_COMBO1,
	KEY3_COMBO2,
	KEY4_COMBO1,
	KEY4_COMBO2,
	KEY5_COMBO1,
	KEY5_COMBO2,
	KEY6_COMBO1,
	KEY6_COMBO2,
	KEY7_COMBO1,
	KEY7_COMBO2,
	KEY8_COMBO1,
	KEY8_COMBO2,
	PICKUP1,
	PICKUP2,
	PICKUP3,
	PICKUP4,
	PICKUP1_COMBO1,
	PICKUP1_COMBO2,
	PICKUP2_COMBO1,
	PICKUP2_COMBO2,
	PICKUP3_COMBO1,
	PICKUP3_COMBO2,
	PICKUP4_COMBO1,
	PICKUP4_COMBO2,
	QUEST1,
	QUEST2,
	QUEST3,
	QUEST4,
	QUEST5,
	QUEST6,
	BURNING_TORCH,
	CROWBAR,
	CLOCKWORK_BEETLE,
	CLOCKWORK_BEETLE_COMBO1,
	CLOCKWORK_BEETLE_COMBO2,
	EXAMINE1,
	EXAMINE2,
	EXAMINE3,
	NONE
};
}

namespace ItemModelType {
enum Enum {
	IDLE = 1,
	ACTIVE,
	PICKUP
};
}

namespace ItemDisplayType {
enum Enum {
	IDLE = 1,
	ACTIVE,
	CONTEXT,
	EXAMINE,
	PICKUP
};
}

namespace ItemAnimationType {
enum Enum {
	IDLE = 1,
	ACTIVATE,
	CANCEL,
	PASSPORT_PAGE2,
	PASSPORT_PAGE3
};
}

namespace ItemActionType {
enum Enum {
	USE = 1,
	EQUIP,
	CHOOSE_AMMO,
	COMBINE,
	SEPARATE,
	EXAMINE,
	LOAD_GAME,
	SAVE_GAME,
	STATISTICS,
	OPTIONS,
	CONTROLS,
	EXIT_TO_TITLE,
	CUSTOM
};
}

struct ItemData : public ecs::Component {
	int32_t item_id;
	script::ScriptString name;
	script::ScriptString description;

	ItemData(
		int32_t item_id,
		script::ScriptString name = script::ScriptString(),
		script::ScriptString description = script::ScriptString()
	)
		:
		item_id(item_id),
		name(name),
		description(description)
	{}
};

struct ItemModelConfig : public ecs::Component {
	const ItemModelType::Enum type;

	StrSlot *slot;
	int32_t slot_id;
	int32_t sprite_id;
	int32_t size_x;
	int32_t size_y;
	uint32_t mesh_mask;

	ItemModelConfig(
		const ItemModelType::Enum type,
		StrSlot &slot,
		int32_t slot_id,
		uint32_t mesh_mask = UINT32_MAX
	)
		:
		type(type),
		slot(&slot),
		slot_id(slot_id),
		sprite_id(-1),
		size_x(0),
		size_y(0),
		mesh_mask(mesh_mask)
	{}

	ItemModelConfig(
		const ItemModelType::Enum type,
		const ItemModelConfig &config
	)
		:
		type(type),
		slot(config.slot),
		slot_id(config.slot_id),
		sprite_id(config.sprite_id),
		size_x(config.size_x),
		size_y(config.size_y),
		mesh_mask(config.mesh_mask)
	{}
};

struct ItemModel : public ecs::Component {
	const ItemModelConfig *config;

	ItemModel(const ItemModelConfig &config)
		:
		config(&config)
	{}
};

struct ItemDisplayConfig : public ecs::Component {
	const ItemDisplayType::Enum type;

	core::Vector3D pos;
	core::Vector3D orient;

	float tilt;
	float scale;

	bool pos_ignore_anim;
	bool orient_ignore_anim;
	bool spin;
	bool fade;

	ItemDisplayConfig(const ItemDisplayType::Enum type)
		:
		type(type),
		pos(core::Vector3D()),
		orient(core::Vector3D()),
		tilt(0),
		scale(1),
		pos_ignore_anim(false),
		orient_ignore_anim(false),
		spin(true),
		fade(true)
	{}

	ItemDisplayConfig(
		const ItemDisplayType::Enum type,
		const ItemDisplayConfig &config
	)
		:
		type(type),
		pos(config.pos),
		orient(config.orient),
		tilt(config.tilt),
		scale(config.scale),
		pos_ignore_anim(config.pos_ignore_anim),
		orient_ignore_anim(config.orient_ignore_anim),
		spin(config.spin),
		fade(config.fade)
	{}
};

struct ItemDisplay : public ecs::Component {
	const ItemDisplayConfig *config;

	core::Vector3D pos;
	core::Vector3D orient;
	core::Vector3D rot;

	float tilt;
	float scale;
	bool spin;

	bool alpha_enabled;

	ItemDisplay(const ItemDisplayConfig &config)
		:
		config(&config),
		pos(config.pos),
		orient(config.orient),
		rot(core::Vector3D()),
		tilt(config.tilt),
		scale(config.scale),
		spin(config.spin),
		alpha_enabled(true)
	{}
};

class ItemQuantity : public ecs::Component {
public:
	std::function<int32_t(void)> get_quantity;
	std::function<void(int32_t)> set_quantity;
	int32_t quantity_max;
	int32_t quantity_min;
	int32_t divider;
	bool supports_unlimited;

	ItemQuantity(
		std::function<int32_t(void)> get_quantity = []() -> int32_t {return 0; },
		std::function<void(int32_t)> set_quantity = [](int32_t) -> void {},
		int32_t quantity_max = INT16_MAX,
		int32_t quantity_min = 0
	)
		:
		get_quantity(get_quantity),
		set_quantity(set_quantity),
		quantity_max(quantity_max),
		quantity_min(quantity_min),
		divider(1),
		supports_unlimited(false)
	{}

	int32_t get() const;
	bool set(int32_t qty) const;
	bool increase(int32_t qty = 1) const;
	bool decrease(int32_t qty = 1) const;
	bool unlimited() const;
	bool zero() const;
};

struct ItemAnimation : public ecs::Component {
	ItemAnimationType::Enum type;
	int32_t anim_index;
	float frame;
	float frame_start;
	float frame_end;
	bool wait_for_motions;

	bool active;

	ItemAnimation(
		ItemAnimationType::Enum type = ItemAnimationType::IDLE,
		int32_t anim_index = 0,
		float frame = 0,
		float frame_start = 0,
		float frame_end = 0,
		bool wait_for_motions = false
	)
		:
		type(type),
		anim_index(anim_index),
		frame(frame),
		frame_start(frame_start),
		frame_end(frame_end),
		wait_for_motions(wait_for_motions),
		active(false)
	{}
};

struct ItemAction : public ecs::Component {
	script::ScriptString name;
	int32_t sort_index;
	ItemActionType::Enum type;
	bool replace_default;
	std::function<void(void)> action;
	std::function<bool(void)> enabled;

	bool active;

	ItemAction(
		script::ScriptString name = script::ScriptString(),
		ItemActionType::Enum type = ItemActionType::USE,
		std::function<void(void)> action = nullptr,
		std::function<bool(void)> enabled = []() -> bool {return true; }
	)
		:
		name(name),
		sort_index(0),
		type(type),
		replace_default(true),
		action(action),
		enabled(enabled),
		active(false)
	{}
};

struct ItemSfx : public ecs::Component {
	sound::SfxType::Enum type;
	int32_t sound_id;

	ItemSfx(
		sound::SfxType::Enum type,
		int32_t sound_id = -1
	)
		:
		type(type),
		sound_id(sound_id)
	{}
};

struct ItemAmmo : public ecs::Component {
	ecs::Entity &ammo_item;
	std::function<bool(void)> loaded;
	std::function<void(void)> load;

	ItemAmmo(
		ecs::Entity &ammo_item,
		const std::function<bool()> loaded = []() -> bool { return false; },
		const std::function<void()> load = []() -> void {}
	)
		:
		ammo_item(ammo_item),
		loaded(loaded),
		load(load)
	{}
};

struct ItemRing : public ecs::Component {
	int32_t ring_id;
	int32_t sort_index;

	ItemRing(
		int32_t ring_id,
		int32_t sort_index = 0
	)
		:
		ring_id(ring_id),
		sort_index(sort_index)
	{}
};

struct ComboData : public ecs::Component {
	ecs::Entity &item_first;
	ecs::Entity &item_second;
	ecs::Entity *item_final;
	ecs::Entity *item_extra;
	bool vice_versa;
	bool separable;

	std::function<void(ComboData&)> combine;
	std::function<void(ComboData&)> separate;

	ComboData(
		ecs::Entity &item_first,
		ecs::Entity &item_second,
		ecs::Entity *item_final = nullptr,
		ecs::Entity *item_extra = nullptr,
		bool vice_versa = true,
		bool separable = false,
		std::function<void(ComboData&)> combine = [](ComboData&) -> void {},
		std::function<void(ComboData&)> separate = [](ComboData&) -> void {}
	)
		:
		item_first(item_first),
		item_second(item_second),
		item_final(item_final),
		item_extra(item_extra),
		vice_versa(vice_versa),
		separable(separable),
		combine(combine),
		separate(separate)
	{}
};

struct ExamineData : public ecs::Component {
	script::ScriptString text_1;
	script::ScriptString text_2;
	script::ScriptString text_3;

	ExamineData(
		script::ScriptString text_1 = script::ScriptString(),
		script::ScriptString text_2 = script::ScriptString(),
		script::ScriptString text_3 = script::ScriptString()
	)
		:
		text_1(text_1),
		text_2(text_2),
		text_3(text_3)
	{}
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
int32_t tr4_slot_to_item_id(uint32_t tr4_slot);
int32_t tr4_invobj_to_item_id(uint32_t tr4_invobj);
uint32_t item_id_to_tr4_slot(int32_t item_id);
uint32_t item_id_to_tr4_invobj(int32_t item_id);
uint32_t item_id_to_item_index(int32_t item_id);
uint32_t item_index_to_item_id(int32_t item_index);

ecs::Entity* get_item_by_item_id(int32_t item_id, ecs::EntityManager &entity_manager);

item::ItemModelConfig* get_item_model_config(
	ecs::Entity &item,
	item::ItemModelType::Enum type,
	item::ItemModelType::Enum type_default = item::ItemModelType::IDLE
);

item::ItemDisplayConfig* get_item_display_config(
	ecs::Entity &item,
	item::ItemDisplayType::Enum type,
	item::ItemDisplayType::Enum type_default = item::ItemDisplayType::IDLE
);

void change_item_model(ecs::Entity &item, item::ItemModelType::Enum type);

void change_item_display(
	ecs::Entity &item,
	item::ItemDisplayType::Enum type,
	bool keep_pos = false,
	bool keep_orient = false,
	bool keep_rot = false,
	bool keep_tilt = false,
	bool keep_scale = false,
	bool keep_motion = false,
	bool keep_alpha = false
);

void remove_item_motion(ecs::Entity &item, bool keep_background = true);
void deactivate_item_actions(ecs::Entity &item);
void reset_item_animation(ecs::Entity &item);
void spin_item(ecs::Entity &item, uint32_t frames);
void restore_item_spin(ecs::Entity &item, uint32_t frames = 0, float speed = 1);

}
}

#pragma pack(pop)