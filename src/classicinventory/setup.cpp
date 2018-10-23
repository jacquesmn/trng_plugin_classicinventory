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

#include "setup.h"

#include <algorithm>

#include <trng_core.h>
#include "action.h"
#include "camera.h"
#include "cheat.h"
#include "inventory.h"
#include "ring.h"
#include "item.h"
#include "render.h"
#include "script.h"
#include "sound.h"
#include "special.h"
#include "text.h"

extern TYPE_convert_obj_to_invobj convert_obj_to_invobj;
extern TYPE_SoundEffect SoundEffect;

namespace classicinventory {
namespace setup {

void add_to_ring(
	ecs::Entity &item,
	int32_t ring_id,
	ecs::EntityManager &entity_manager
)
{
	const auto ring = entity_manager.find_entity_with_component<ring::RingData>([=](const ring::RingData &ring_data) -> bool {
		return ring_data.ring_id == ring_id;
	});
	if (!ring) {
		return;
	}

	auto ring_items = entity_manager.find_entities_with_component<item::ItemRing>([&](const item::ItemRing &item_ring) -> bool {
		return item_ring.ring_id == ring_id;
	});

	item.add_component(new item::ItemRing(ring_id, ring_items.size()));
}

void set_default_item_displays(ecs::Entity &item)
{
	auto &item_display_idle = item.add_component(new item::ItemDisplayConfig(item::ItemDisplayType::IDLE));

	auto &item_display_active = item.add_component(new item::ItemDisplayConfig(item::ItemDisplayType::ACTIVE, item_display_idle));
	item_display_active.pos.z = -256;
	item_display_active.orient.x += 40;
	item_display_active.tilt = -45;

	auto &item_display_context = item.add_component(new item::ItemDisplayConfig(item::ItemDisplayType::CONTEXT, item_display_idle));
	item_display_context.pos.z = 256;
	item_display_context.orient.x += -40;
	item_display_context.tilt = 45;

	auto &item_display_examine = item.add_component(new item::ItemDisplayConfig(item::ItemDisplayType::EXAMINE, item_display_active));

	auto &item_display_pickup = item.add_component(new item::ItemDisplayConfig(item::ItemDisplayType::PICKUP, item_display_idle));
	item_display_pickup.pos.x = 850;
	item_display_pickup.pos.y = 850;

	item.add_component(new item::ItemDisplay(item_display_idle));
}

void set_tr4_pickup_orientation(ecs::Entity &item, int32_t slot_id)
{
	auto display_pickup = item::get_item_display_config(item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (!display_pickup) {
		return;
	}

	const auto &tr4_invobj = Trng.pGlobTomb4->pAdr->pVetStructInventoryItems[convert_obj_to_invobj(slot_id)];

	display_pickup->orient.x = core::tr4_angle_to_degrees(tr4_invobj.OrientY);
	display_pickup->orient.y = core::tr4_angle_to_degrees(tr4_invobj.OrientX);
	display_pickup->orient.z = core::tr4_angle_to_degrees(tr4_invobj.OrientZ);
}

item::ItemAction& add_item_action(
	ecs::Entity &item,
	script::ScriptString name = script::ScriptString(),
	item::ItemActionType::Enum type = item::ItemActionType::USE
)
{
	auto &item_action = item.add_component(new item::ItemAction(name, type));

	item_action.sort_index = item.get_components<item::ItemAction>().size() - 1;

	return item_action;
}

ecs::Entity* setup_tr4_item(
	int32_t item_id,
	int32_t slot_id,
	int32_t ring_id,
	ecs::EntityManager &entity_manager
)
{
	// skip if not present
	Get(enumGET.SLOT, slot_id, 0);
	const auto slot = GET.pSlot;
	if (!slot || !core::bit_set(slot->Flags, enumFSLOT.PRESENT)) {
		return nullptr;
	}

	auto &tr4_inv_obj = Trng.pGlobTomb4->pAdr->pVetStructInventoryItems[convert_obj_to_invobj(slot_id)];

	auto &item = entity_manager.new_entity();
	item.add_component(new item::ItemData(item_id, script::ScriptString(tr4_inv_obj.IndiceStringa)));

	// set default models
	const auto &item_model_idle = item.add_component(new item::ItemModelConfig(item::ItemModelType::IDLE, *slot, slot_id, tr4_inv_obj.Mistero));
	item.add_component(new item::ItemModel(item_model_idle));

	set_default_item_displays(item);

	add_to_ring(item, ring_id, entity_manager);

	return &item;
}

void setup_tr4_ammo(
	int32_t item_id,
	int32_t ammo_item_id,
	uint8_t *weapon_value,
	uint8_t ammo_bit,
	ecs::EntityManager &entity_manager
)
{
	auto item = entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});

	const auto ammo_item = entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
		return item_data.item_id == ammo_item_id;
	});

	if (!item || !ammo_item) {
		return;
	}

	item->add_component(new item::ItemAmmo(
		*ammo_item,
		[=]() -> bool { return core::bit_set(*weapon_value, ammo_bit); },
		[=]() -> void {
		// unload previous ammo
		core::set_bit(*weapon_value, (FWEAP_AMMO_NORMAL | FWEAP_AMMO_SUPER | FWEAP_AMMO_EXPLOSIVE), false);
		// load new ammo
		core::set_bit(*weapon_value, ammo_bit);
	}
	));
}

void setup_MEMCARD_LOAD_INV(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::MEMCARD_LOAD_INV, enumSLOT.MEMCARD_LOAD_INV_ITEM, ring::RingId::OPTIONS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return 1; },
		[](int32_t quantity) -> void {}
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::LOAD_GAME), item::ItemActionType::LOAD_GAME);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->scale = 0.25f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->scale = 0.25f;
	}
}

void setup_MEMCARD_SAVE_INV(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::MEMCARD_SAVE_INV, enumSLOT.MEMCARD_SAVE_INV_ITEM, ring::RingId::OPTIONS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return 1; },
		[](int32_t quantity) -> void {}
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::SAVE_GAME), item::ItemActionType::SAVE_GAME);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->scale = 0.25f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->scale = 0.25f;
	}
}

void setup_COMPASS(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::COMPASS, enumSLOT.COMPASS_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return 1; },
		[](int32_t quantity) -> void {}
	));

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 100;
	}

	auto &compass_data = item->add_component(new item::CompassData(1, core::Axis::Y));
	compass_data.needle_oscill_amplitude_min = 3;
	compass_data.needle_oscill_amplitude_max = 30;
	compass_data.needle_oscill_amplitude_settle_frames = 120;
	compass_data.needle_oscill_period_frames = 60;
}

void setup_SMALLMEDI(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::SMALLMEDI, enumSLOT.SMALLMEDI_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->MediPackSmall; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->MediPackSmall = quantity; }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() {
		if (action::use_health(*item)) {
			DetectedGlobalTriggerEvent(GT_USED_LITTLE_MEDIPACK, -1, false);
		}
	};

	item->add_component(new item::HealthData(500, 0, 116, 31));

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.y = 135;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SMALLMEDI_ITEM);
}

void setup_BIGMEDI(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::BIGMEDI, enumSLOT.BIGMEDI_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->MediPackLarge; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->MediPackLarge = quantity; }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() {
		if (action::use_health(*item)) {
			DetectedGlobalTriggerEvent(GT_USED_BIG_MEDIPACK, -1, false);
		}
	};

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.y = -35;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.BIGMEDI_ITEM);

	item->add_component(new item::HealthData(1000, 0, 116, 31));
}

void setup_FLARE_INV(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::FLARE_INV, enumSLOT.FLARE_INV_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->Flares; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->Flares = quantity; }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() {
		action::use_flare(*item);
	};

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.y = -90;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->pos.y = 800;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.FLARE_INV_ITEM);
}

void setup_BINOCULARS(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::BINOCULARS, enumSLOT.BINOCULARS_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->Binoculars; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->Binoculars = quantity; },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() {
		action::use_binoculars(*item);
	};

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 30;
		item_display_active->orient.y = 35;
		item_display_active->orient.z = 6;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.BINOCULARS_ITEM);
}

void setup_CROWBAR(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CROWBAR, enumSLOT.CROWBAR_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->Crowbar; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->Crowbar = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = 90;
		item_display_active->orient.y = 35;
		item_display_active->orient.z = -85;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->pos.y = 800;
		item_display_pickup->scale = 0.75f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CROWBAR_ITEM);
}

void setup_PISTOLS(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PISTOLS, enumSLOT.PISTOLS_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->WeaponPistols, FWEAP_PRESENT) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->WeaponPistols, FWEAP_PRESENT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->scale = 0.8f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = -5;
		item_display_active->pos.y = -20;
		item_display_active->orient.x = 120;
		item_display_active->orient.y = -210;
		item_display_active->tilt = -35;
		item_display_active->scale = 0.8f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = -5;
		item_display_context->pos.y = -20;
		item_display_context->orient.x = 45;
		item_display_context->orient.y = -210;
		item_display_context->scale = 0.8f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.75f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.PISTOLS_ITEM);
}

void setup_SHOTGUN(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::SHOTGUN, enumSLOT.SHOTGUN_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->WeaponShotGun, FWEAP_PRESENT) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->WeaponShotGun, FWEAP_PRESENT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->pos.y = 20;
		item_display_active->pos.z = -350;
		item_display_active->orient.x = -75;
		item_display_active->orient.y = -2;
		item_display_active->orient.z = -35;
		item_display_active->tilt = -35;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 25;
		item_display_context->pos.y = 20;
		item_display_context->orient.x = -150;
		item_display_context->orient.y = -2;
		item_display_context->orient.z = -45;
		item_display_context->tilt = 35;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SHOTGUN_ITEM);
}

void setup_UZI(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::UZI, enumSLOT.UZI_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->WeaponUZI, FWEAP_PRESENT) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->WeaponUZI, FWEAP_PRESENT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->scale = 0.8f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 35;
		item_display_active->pos.z = -270;
		item_display_active->orient.x = -100;
		item_display_active->orient.y = -220;
		item_display_active->orient.z = 40;
		item_display_active->scale = 0.8f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 35;
		item_display_context->orient.x = -170;
		item_display_context->orient.y = -220;
		item_display_context->orient.z = 40;
		item_display_context->scale = 0.8f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->pos.y = 800;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.UZI_ITEM);
}

void setup_REVOLVER(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::REVOLVER, enumSLOT.SIXSHOOTER_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	const auto weapon_qty = &Trng.pGlobTomb4->pAdr->pInventory->WeaponRevolver;
	item->add_component(new item::ItemQuantity(
		[=]() -> int32_t {return core::bit_set(*weapon_qty, FWEAP_PRESENT) && !core::bit_set(*weapon_qty, FWEAP_LASERSIGHT) ? 1 : 0; },
		[=](int32_t quantity) -> void {core::set_bit(*weapon_qty, FWEAP_PRESENT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 30;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->pos.z = -280;
		item_display_active->orient.x = 135;
		item_display_active->orient.y = -18;
		item_display_active->orient.z = 225;
		item_display_active->tilt = -35;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 25;
		item_display_context->orient.x = 70;
		item_display_context->orient.y = -18;
		item_display_context->orient.z = 225;
		item_display_context->tilt = 35;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SIXSHOOTER_ITEM);
}

void setup_REVOLVER_LASERSIGHT_COMBO(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::REVOLVER_LASERSIGHT_COMBO, enumSLOT.SIXSHOOTER_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->get_component<item::ItemData>()->name = script::ScriptString(57);

	const auto item_models = item->get_components<item::ItemModelConfig>();
	std::for_each(item_models.begin(), item_models.end(), [](item::ItemModelConfig *config) -> void {
		config->mesh_mask = 0x0B;
	});

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->WeaponRevolver, FWEAP_PRESENT | FWEAP_LASERSIGHT) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->WeaponRevolver, FWEAP_PRESENT | FWEAP_LASERSIGHT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 30;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->pos.z = -280;
		item_display_active->orient.x = 135;
		item_display_active->orient.y = -18;
		item_display_active->orient.z = 225;
		item_display_active->tilt = -35;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 25;
		item_display_context->orient.x = 70;
		item_display_context->orient.y = -18;
		item_display_context->orient.z = 225;
		item_display_context->tilt = 35;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SIXSHOOTER_ITEM);
}

void setup_CROSSBOW(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CROSSBOW, enumSLOT.CROSSBOW_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	const auto weapon_qty = &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow;
	item->add_component(new item::ItemQuantity(
		[=]() -> int32_t {return core::bit_set(*weapon_qty, FWEAP_PRESENT) && !core::bit_set(*weapon_qty, FWEAP_LASERSIGHT) ? 1 : 0; },
		[=](int32_t quantity) -> void {core::set_bit(*weapon_qty, FWEAP_PRESENT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 22;
		item_display_active->orient.x = 5;
		item_display_active->orient.y = -145;
		item_display_active->orient.z = -20;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 22;
		item_display_context->orient.x = -85;
		item_display_context->orient.y = -145;
		item_display_context->orient.z = -20;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CROSSBOW_ITEM);
}

void setup_CROSSBOW_LASERSIGHT_COMBO(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CROSSBOW_LASERSIGHT_COMBO, enumSLOT.CROSSBOW_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->get_component<item::ItemData>()->name = script::ScriptString(59);

	const auto item_models = item->get_components<item::ItemModelConfig>();
	std::for_each(item_models.begin(), item_models.end(), [](item::ItemModelConfig *config) -> void {
		config->mesh_mask = 0x0B;
	});

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_PRESENT | FWEAP_LASERSIGHT) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_PRESENT | FWEAP_LASERSIGHT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 22;
		item_display_active->orient.x = 5;
		item_display_active->orient.y = -145;
		item_display_active->orient.z = -20;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 22;
		item_display_context->orient.x = -85;
		item_display_context->orient.y = -145;
		item_display_context->orient.z = -20;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CROSSBOW_ITEM);
}

void setup_GRENADE_GUN(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::GRENADE_GUN, enumSLOT.GRENADE_GUN_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->WeaponGrenadeGun, FWEAP_PRESENT) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->WeaponGrenadeGun, FWEAP_PRESENT, quantity != 0); }
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::EQUIP), item::ItemActionType::EQUIP);
	item_action.action = [=]() {
		action::equip_weapon(*item);
	};

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.x = 75;
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 30;
		item_display_active->pos.z = -214;
		item_display_active->orient.x = 20;
		item_display_active->orient.y = -135;
		item_display_active->orient.z = 70;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 30;
		item_display_context->orient.x = -65;
		item_display_context->orient.y = -135;
		item_display_context->orient.z = 70;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.GRENADE_GUN_ITEM);
}

void setup_PISTOLS_AMMO(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PISTOLS_AMMO, enumSLOT.PISTOLS_AMMO_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoPistols; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoPistols = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 60;
		item_display_active->orient.y = -128;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -30;
		item_display_context->orient.y = -128;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.PISTOLS_AMMO_ITEM);
}

void setup_SHOTGUN_AMMO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::SHOTGUN_AMMO1, enumSLOT.SHOTGUN_AMMO1_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoShotgunNormals; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoShotgunNormals = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 50;
		item_display_active->orient.y = 90;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 90;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SHOTGUN_AMMO1_ITEM);
}

void setup_SHOTGUN_AMMO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::SHOTGUN_AMMO2, enumSLOT.SHOTGUN_AMMO2_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoShotgunWideShot; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoShotgunWideShot = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 50;
		item_display_active->orient.y = 90;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 90;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SHOTGUN_AMMO2_ITEM);
}

void setup_UZI_AMMO(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::UZI_AMMO, enumSLOT.UZI_AMMO_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoUZI; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoUZI = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 60;
		item_display_active->orient.y = -180;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -30;
		item_display_context->orient.y = -180;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.UZI_AMMO_ITEM);
}

void setup_REVOLVER_AMMO(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::REVOLVER_AMMO, enumSLOT.SIXSHOOTER_AMMO_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoRevolver; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoRevolver = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 60;
		item_display_active->orient.y = 180;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -30;
		item_display_context->orient.y = 180;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.SIXSHOOTER_AMMO_ITEM);
}

void setup_CROSSBOW_AMMO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CROSSBOW_AMMO1, enumSLOT.CROSSBOW_AMMO1_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoCrossBowNormals; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoCrossBowNormals = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 55;
		item_display_active->orient.y = 90;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 90;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->pos.y = 800;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CROSSBOW_AMMO1_ITEM);
}

void setup_CROSSBOW_AMMO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CROSSBOW_AMMO2, enumSLOT.CROSSBOW_AMMO2_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoCrossBowPoison; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoCrossBowPoison = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 55;
		item_display_active->orient.y = 90;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 90;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->pos.y = 800;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CROSSBOW_AMMO2_ITEM);
}

void setup_CROSSBOW_AMMO3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CROSSBOW_AMMO3, enumSLOT.CROSSBOW_AMMO3_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoCrossBowExplosive; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoCrossBowExplosive = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 55;
		item_display_active->orient.y = 90;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 90;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->pos.y = 800;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CROSSBOW_AMMO3_ITEM);
}

void setup_GRENADE_GUN_AMMO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::GRENADE_GUN_AMMO1, enumSLOT.GRENADE_GUN_AMMO1_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoGrenadeNormals; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoGrenadeNormals = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 55;
		item_display_active->orient.y = 60;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 60;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.GRENADE_GUN_AMMO1_ITEM);
}

void setup_GRENADE_GUN_AMMO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::GRENADE_GUN_AMMO2, enumSLOT.GRENADE_GUN_AMMO2_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoGrenadeSuper; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoGrenadeSuper = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 55;
		item_display_active->orient.y = 60;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 60;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.GRENADE_GUN_AMMO2_ITEM);
}

void setup_GRENADE_GUN_AMMO3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::GRENADE_GUN_AMMO3, enumSLOT.GRENADE_GUN_AMMO3_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->AmmoGrenadeFlash; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->AmmoGrenadeFlash = quantity; }
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 55;
		item_display_active->orient.y = 60;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 60;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.GRENADE_GUN_AMMO3_ITEM);
}

void setup_LASERSIGHT(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::LASERSIGHT, enumSLOT.LASERSIGHT_ITEM, ring::RingId::INVENTORY, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->LaserSight; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->LaserSight = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 15;
		item_display_active->orient.x = 30;
		item_display_active->orient.y = -140;
		item_display_active->orient.z = -10;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 15;
		item_display_context->orient.x = -60;
		item_display_context->orient.y = -140;
		item_display_context->orient.z = -10;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.LASERSIGHT_ITEM);
}

void setup_PUZZLE1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE1, enumSLOT.PUZZLE_ITEM1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem1; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem1 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE2, enumSLOT.PUZZLE_ITEM2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem2; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem2 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE3, enumSLOT.PUZZLE_ITEM3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem3; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem3 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE4(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE4, enumSLOT.PUZZLE_ITEM4, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem4; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem4 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE5(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE5, enumSLOT.PUZZLE_ITEM5, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem5; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem5 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE6(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE6, enumSLOT.PUZZLE_ITEM6, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem6; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem6 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE7(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE7, enumSLOT.PUZZLE_ITEM7, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem7; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem7 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE8(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE8, enumSLOT.PUZZLE_ITEM8, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem8; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem8 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE9(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE9, enumSLOT.PUZZLE_ITEM9, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem9; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem9 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE10(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE10, enumSLOT.PUZZLE_ITEM10, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem10; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem10 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE11(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE11, enumSLOT.PUZZLE_ITEM11, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem11; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem11 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE12(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE12, enumSLOT.PUZZLE_ITEM12, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem12; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->PuzzleItem12 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE1_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE1_COMBO1, enumSLOT.PUZZLE_ITEM1_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE1_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE1_COMBO2, enumSLOT.PUZZLE_ITEM1_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE2_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE2_COMBO1, enumSLOT.PUZZLE_ITEM2_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE2_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE2_COMBO2, enumSLOT.PUZZLE_ITEM2_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE3_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE3_COMBO1, enumSLOT.PUZZLE_ITEM3_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE3_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE3_COMBO2, enumSLOT.PUZZLE_ITEM3_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE4_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE4_COMBO1, enumSLOT.PUZZLE_ITEM4_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE4_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE4_COMBO2, enumSLOT.PUZZLE_ITEM4_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE5_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE5_COMBO1, enumSLOT.PUZZLE_ITEM5_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_5) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_5, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE5_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE5_COMBO2, enumSLOT.PUZZLE_ITEM5_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_5) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_5, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE6_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE6_COMBO1, enumSLOT.PUZZLE_ITEM6_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_6) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_6, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE6_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE6_COMBO2, enumSLOT.PUZZLE_ITEM6_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_6) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_6, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE7_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE7_COMBO1, enumSLOT.PUZZLE_ITEM7_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_7) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_7, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE7_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE7_COMBO2, enumSLOT.PUZZLE_ITEM7_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_7) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_7, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE8_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE8_COMBO1, enumSLOT.PUZZLE_ITEM8_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_8) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM1_FOR_PUZZLE_8, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PUZZLE8_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PUZZLE8_COMBO2, enumSLOT.PUZZLE_ITEM8_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_8) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ComboItems, COMBO_ITEM2_FOR_PUZZLE_8, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY1, enumSLOT.KEY_ITEM1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY2, enumSLOT.KEY_ITEM2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY3, enumSLOT.KEY_ITEM3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY4(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY4, enumSLOT.KEY_ITEM4, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY5(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY5, enumSLOT.KEY_ITEM5, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_5) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_5, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY6(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY6, enumSLOT.KEY_ITEM6, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_6) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_6, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY7(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY7, enumSLOT.KEY_ITEM7, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_7) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_7, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY8(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY8, enumSLOT.KEY_ITEM8, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_8) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_8, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY9(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY9, enumSLOT.KEY_ITEM9, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_9) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_9, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY10(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY10, enumSLOT.KEY_ITEM10, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_10) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_10, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY11(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY11, enumSLOT.KEY_ITEM11, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_11) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_11, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY12(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY12, enumSLOT.KEY_ITEM12, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_12) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->Keys, EX16_EXIST_12, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY1_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY1_COMBO1, enumSLOT.KEY_ITEM1_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY1_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY1_COMBO2, enumSLOT.KEY_ITEM1_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY2_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY2_COMBO1, enumSLOT.KEY_ITEM2_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY2_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY2_COMBO2, enumSLOT.KEY_ITEM2_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY3_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY3_COMBO1, enumSLOT.KEY_ITEM3_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY3_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY3_COMBO2, enumSLOT.KEY_ITEM3_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY4_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY4_COMBO1, enumSLOT.KEY_ITEM4_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY4_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY4_COMBO2, enumSLOT.KEY_ITEM4_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY5_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY5_COMBO1, enumSLOT.KEY_ITEM5_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_5) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_5, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY5_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY5_COMBO2, enumSLOT.KEY_ITEM5_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_5) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_5, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY6_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY6_COMBO1, enumSLOT.KEY_ITEM6_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_6) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_6, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY6_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY6_COMBO2, enumSLOT.KEY_ITEM6_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_6) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_6, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY7_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY7_COMBO1, enumSLOT.KEY_ITEM7_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_7) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_7, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY7_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY7_COMBO2, enumSLOT.KEY_ITEM7_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_7) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_7, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY8_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY8_COMBO1, enumSLOT.KEY_ITEM8_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_8) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM1_FOR_KEY_8, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_KEY8_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::KEY8_COMBO2, enumSLOT.KEY_ITEM8_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_8) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->KeyCombo, KCOMBO_ITEM2_FOR_KEY_8, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP1, enumSLOT.PICKUP_ITEM1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP2, enumSLOT.PICKUP_ITEM2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP3, enumSLOT.PICKUP_ITEM3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP4(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP4, enumSLOT.PICKUP_ITEM4, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupItems, EX16_EXIST_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP1_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP1_COMBO1, enumSLOT.PICKUP_ITEM1_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP1_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP1_COMBO2, enumSLOT.PICKUP_ITEM1_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP2_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP2_COMBO1, enumSLOT.PICKUP_ITEM2_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP2_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP2_COMBO2, enumSLOT.PICKUP_ITEM2_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP3_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP3_COMBO1, enumSLOT.PICKUP_ITEM3_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP3_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP3_COMBO2, enumSLOT.PICKUP_ITEM3_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP4_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP4_COMBO1, enumSLOT.PICKUP_ITEM4_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM1_FOR_PICKUP_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_PICKUP4_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::PICKUP4_COMBO2, enumSLOT.PICKUP_ITEM4_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->PickupCombo, PCOMBO_ITEM2_FOR_PICKUP_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
}

void setup_QUEST1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::QUEST1, enumSLOT.QUEST_ITEM1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_1) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_1, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.z = -200;
		item_display_active->orient.x = 130;
		item_display_active->orient.y = 180;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.QUEST_ITEM1);
}

void setup_QUEST2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::QUEST2, enumSLOT.QUEST_ITEM2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_2) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_2, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.z = -200;
		item_display_active->orient.x = 40;
		item_display_active->orient.y = 150;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.QUEST_ITEM2);
}

void setup_QUEST3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::QUEST3, enumSLOT.QUEST_ITEM3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_3) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_3, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.x = -180;
		item_display_idle->orient.y = -10;
		item_display_idle->orient.z = 92;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = -140;
		item_display_active->orient.y = 20;
		item_display_active->orient.z = 92;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.QUEST_ITEM3);
}

void setup_QUEST4(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::QUEST4, enumSLOT.QUEST_ITEM4, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_4) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_4, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.x = -180;
		item_display_idle->orient.y = -10;
		item_display_idle->orient.z = 92;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = -140;
		item_display_active->orient.y = 20;
		item_display_active->orient.z = 92;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.QUEST_ITEM4);
}

void setup_QUEST5(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::QUEST5, enumSLOT.QUEST_ITEM5, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_5) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_5, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 50;
		item_display_active->orient.y = 180;
		item_display_active->orient.z = 70;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.QUEST_ITEM5);
}

void setup_QUEST6(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::QUEST6, enumSLOT.QUEST_ITEM6, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_6) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->QuestItems, EX16_EXIST_6, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 20;
		item_display_active->orient.y = -70;
		item_display_active->orient.z = 55;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.QUEST_ITEM6);
}

void setup_EXAMINE1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::EXAMINE1, enumSLOT.EXAMINE1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->ExamineItem1; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->ExamineItem1 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::EXAMINE), item::ItemActionType::EXAMINE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.z = -156;
		item_display_active->orient.x = 120;
		item_display_active->orient.y = 180;
	}

	auto item_display_examine = item::get_item_display_config(*item, item::ItemDisplayType::EXAMINE, item::ItemDisplayType::EXAMINE);
	if (item_display_examine) {
		item_display_examine->pos.z = -156;
		item_display_examine->orient.x = 120;
		item_display_examine->orient.y = 180;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.EXAMINE1);
}

void setup_EXAMINE2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::EXAMINE2, enumSLOT.EXAMINE2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->ExamineItem2; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->ExamineItem2 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::EXAMINE), item::ItemActionType::EXAMINE);

	item->add_component(new item::ExamineData(script::ScriptString(201), script::ScriptString(), script::ScriptString(202)));

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.x = -90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.z = -156;
		item_display_active->orient.x = 40;
	}

	auto item_display_examine = item::get_item_display_config(*item, item::ItemDisplayType::EXAMINE, item::ItemDisplayType::EXAMINE);
	if (item_display_examine) {
		item_display_examine->pos.z = -156;
		item_display_examine->orient.x = 40;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}
}

void setup_EXAMINE3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::EXAMINE3, enumSLOT.EXAMINE3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->ExamineItem3; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->ExamineItem3 = quantity; },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::EXAMINE), item::ItemActionType::EXAMINE);

	item->add_component(new item::ExamineData(script::ScriptString(), script::ScriptString(203), script::ScriptString()));

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.z = -156;
		item_display_active->orient.x = 120;
		item_display_active->orient.y = 180;
	}

	auto item_display_examine = item::get_item_display_config(*item, item::ItemDisplayType::EXAMINE, item::ItemDisplayType::EXAMINE);
	if (item_display_examine) {
		item_display_examine->pos.z = -156;
		item_display_examine->orient.x = 120;
		item_display_examine->orient.y = 180;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.EXAMINE3);
}

void setup_WATERSKIN1_EMPTY(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN1_EMPTY, enumSLOT.WATERSKIN1_EMPTY, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall == 1 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall = (quantity == 1 ? 1 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 10;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN1_EMPTY);
}

void setup_WATERSKIN1_1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN1_1, enumSLOT.WATERSKIN1_1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall == 2 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall = (quantity == 1 ? 2 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 10;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN1_1);
}

void setup_WATERSKIN1_2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN1_2, enumSLOT.WATERSKIN1_2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall == 3 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall = (quantity == 1 ? 3 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 10;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN1_2);
}

void setup_WATERSKIN1_3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN1_3, enumSLOT.WATERSKIN1_3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall == 4 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinSmall = (quantity == 1 ? 4 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 10;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN1_3);
}

void setup_WATERSKIN2_EMPTY(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN2_EMPTY, enumSLOT.WATERSKIN2_EMPTY, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge == 1 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge = (quantity == 1 ? 1 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN2_EMPTY);
}

void setup_WATERSKIN2_1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN2_1, enumSLOT.WATERSKIN2_1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge == 2 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge = (quantity == 1 ? 2 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN2_1);
}

void setup_WATERSKIN2_2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN2_2, enumSLOT.WATERSKIN2_2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge == 3 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge = (quantity == 1 ? 3 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN2_2);
}

void setup_WATERSKIN2_3(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN2_3, enumSLOT.WATERSKIN2_3, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge == 4 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge = (quantity == 1 ? 4 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN2_3);
}

void setup_WATERSKIN2_4(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN2_4, enumSLOT.WATERSKIN2_4, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge == 5 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge = (quantity == 1 ? 5 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN2_4);
}

void setup_WATERSKIN2_5(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::WATERSKIN2_5, enumSLOT.WATERSKIN2_5, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge == 6 ? 1 : 0; },
		[](int32_t quantity) -> void {Trng.pGlobTomb4->pAdr->pInventory->WaterSkinLarge = (quantity == 1 ? 6 : 0); },
		1,
		0
	));

	auto &item_action = add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);
	item_action.action = [=]() -> void {
		action::exchange_waterskins(*item);
	};
	item_action.replace_default = false;

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = -140;
		item_display_idle->scale = 0.5f;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->pos.x = 25;
		item_display_active->orient.x = -60;
		item_display_active->orient.z = -45;
		item_display_active->scale = 0.5f;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->pos.x = 10;
		item_display_context->orient.x = -130;
		item_display_context->orient.z = -45;
		item_display_context->scale = 0.5f;
	}

	auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP, item::ItemDisplayType::PICKUP);
	if (item_display_pickup) {
		item_display_pickup->scale = 0.5f;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.WATERSKIN2_5);
}

void setup_CLOCKWORK_BEETLE(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CLOCKWORK_BEETLE, enumSLOT.CLOCKWORK_BEETLE, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ClockWorkBeetle, CBEET_BEETLE_WITH_KEY) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ClockWorkBeetle, CBEET_BEETLE_WITH_KEY, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 30;
		item_display_active->orient.y = 150;
		item_display_active->tilt = -40;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = 30;
		item_display_context->orient.y = 150;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CLOCKWORK_BEETLE);
}

void setup_CLOCKWORK_BEETLE_COMBO1(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CLOCKWORK_BEETLE_COMBO1, enumSLOT.CLOCKWORK_BEETLE_COMBO1, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ClockWorkBeetle, CBEET_KEY_SINGLE) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ClockWorkBeetle, CBEET_KEY_SINGLE, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 90;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 30;
		item_display_active->orient.y = 45;
		item_display_active->tilt = -30;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -40;
		item_display_context->orient.y = 45;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CLOCKWORK_BEETLE_COMBO1);
}

void setup_CLOCKWORK_BEETLE_COMBO2(ecs::EntityManager &entity_manager)
{
	const auto item = setup_tr4_item(item::ItemId::CLOCKWORK_BEETLE_COMBO2, enumSLOT.CLOCKWORK_BEETLE_COMBO2, ring::RingId::ITEMS, entity_manager);
	if (!item) {
		return;
	}

	item->add_component(new item::ItemQuantity(
		[]() -> int32_t {return core::bit_set(Trng.pGlobTomb4->pAdr->pInventory->ClockWorkBeetle, CBEET_BEETLE_SINGLE) ? 1 : 0; },
		[](int32_t quantity) -> void {core::set_bit(Trng.pGlobTomb4->pAdr->pInventory->ClockWorkBeetle, CBEET_BEETLE_SINGLE, quantity != 0); },
		1,
		0
	));

	add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

	auto item_display_idle = item::get_item_display_config(*item, item::ItemDisplayType::IDLE);
	if (item_display_idle) {
		item_display_idle->orient.y = 180;
	}

	auto item_display_active = item::get_item_display_config(*item, item::ItemDisplayType::ACTIVE, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = 30;
		item_display_active->orient.y = 150;
		item_display_active->tilt = -40;
	}

	auto item_display_context = item::get_item_display_config(*item, item::ItemDisplayType::CONTEXT, item::ItemDisplayType::CONTEXT);
	if (item_display_context) {
		item_display_context->orient.x = -50;
		item_display_context->orient.y = 150;
	}

	set_tr4_pickup_orientation(*item, enumSLOT.CLOCKWORK_BEETLE_COMBO2);
}

void setup_items(ecs::EntityManager &entity_manager)
{
	setup_MEMCARD_LOAD_INV(entity_manager);
	setup_MEMCARD_SAVE_INV(entity_manager);

	setup_COMPASS(entity_manager);
	setup_PISTOLS(entity_manager);
	setup_PISTOLS_AMMO(entity_manager);
	setup_SHOTGUN(entity_manager);
	setup_SHOTGUN_AMMO1(entity_manager);
	setup_SHOTGUN_AMMO2(entity_manager);
	setup_UZI(entity_manager);
	setup_UZI_AMMO(entity_manager);
	setup_REVOLVER(entity_manager);
	setup_REVOLVER_LASERSIGHT_COMBO(entity_manager);
	setup_REVOLVER_AMMO(entity_manager);
	setup_CROSSBOW(entity_manager);
	setup_CROSSBOW_LASERSIGHT_COMBO(entity_manager);
	setup_CROSSBOW_AMMO1(entity_manager);
	setup_CROSSBOW_AMMO2(entity_manager);
	setup_CROSSBOW_AMMO3(entity_manager);
	setup_GRENADE_GUN(entity_manager);
	setup_GRENADE_GUN_AMMO1(entity_manager);
	setup_GRENADE_GUN_AMMO2(entity_manager);
	setup_GRENADE_GUN_AMMO3(entity_manager);
	setup_LASERSIGHT(entity_manager);
	setup_CROWBAR(entity_manager);
	setup_BINOCULARS(entity_manager);
	setup_FLARE_INV(entity_manager);
	setup_BIGMEDI(entity_manager);
	setup_SMALLMEDI(entity_manager);

	setup_PUZZLE1(entity_manager);
	setup_PUZZLE2(entity_manager);
	setup_PUZZLE3(entity_manager);
	setup_PUZZLE4(entity_manager);
	setup_PUZZLE5(entity_manager);
	setup_PUZZLE6(entity_manager);
	setup_PUZZLE7(entity_manager);
	setup_PUZZLE8(entity_manager);
	setup_PUZZLE9(entity_manager);
	setup_PUZZLE10(entity_manager);
	setup_PUZZLE11(entity_manager);
	setup_PUZZLE12(entity_manager);
	setup_PUZZLE1_COMBO1(entity_manager);
	setup_PUZZLE1_COMBO2(entity_manager);
	setup_PUZZLE2_COMBO1(entity_manager);
	setup_PUZZLE2_COMBO2(entity_manager);
	setup_PUZZLE3_COMBO1(entity_manager);
	setup_PUZZLE3_COMBO2(entity_manager);
	setup_PUZZLE4_COMBO1(entity_manager);
	setup_PUZZLE4_COMBO2(entity_manager);
	setup_PUZZLE5_COMBO1(entity_manager);
	setup_PUZZLE5_COMBO2(entity_manager);
	setup_PUZZLE6_COMBO1(entity_manager);
	setup_PUZZLE6_COMBO2(entity_manager);
	setup_PUZZLE7_COMBO1(entity_manager);
	setup_PUZZLE7_COMBO2(entity_manager);
	setup_PUZZLE8_COMBO1(entity_manager);
	setup_PUZZLE8_COMBO2(entity_manager);
	setup_KEY1(entity_manager);
	setup_KEY2(entity_manager);
	setup_KEY3(entity_manager);
	setup_KEY4(entity_manager);
	setup_KEY5(entity_manager);
	setup_KEY6(entity_manager);
	setup_KEY7(entity_manager);
	setup_KEY8(entity_manager);
	setup_KEY9(entity_manager);
	setup_KEY10(entity_manager);
	setup_KEY11(entity_manager);
	setup_KEY12(entity_manager);
	setup_KEY1_COMBO1(entity_manager);
	setup_KEY1_COMBO2(entity_manager);
	setup_KEY2_COMBO1(entity_manager);
	setup_KEY2_COMBO2(entity_manager);
	setup_KEY3_COMBO1(entity_manager);
	setup_KEY3_COMBO2(entity_manager);
	setup_KEY4_COMBO1(entity_manager);
	setup_KEY4_COMBO2(entity_manager);
	setup_KEY5_COMBO1(entity_manager);
	setup_KEY5_COMBO2(entity_manager);
	setup_KEY6_COMBO1(entity_manager);
	setup_KEY6_COMBO2(entity_manager);
	setup_KEY7_COMBO1(entity_manager);
	setup_KEY7_COMBO2(entity_manager);
	setup_KEY8_COMBO1(entity_manager);
	setup_KEY8_COMBO2(entity_manager);
	setup_PICKUP1(entity_manager);
	setup_PICKUP2(entity_manager);
	setup_PICKUP3(entity_manager);
	setup_PICKUP4(entity_manager);
	setup_PICKUP1_COMBO1(entity_manager);
	setup_PICKUP1_COMBO2(entity_manager);
	setup_PICKUP2_COMBO1(entity_manager);
	setup_PICKUP2_COMBO2(entity_manager);
	setup_PICKUP3_COMBO1(entity_manager);
	setup_PICKUP3_COMBO2(entity_manager);
	setup_PICKUP4_COMBO1(entity_manager);
	setup_PICKUP4_COMBO2(entity_manager);
	setup_QUEST1(entity_manager);
	setup_QUEST2(entity_manager);
	setup_QUEST3(entity_manager);
	setup_QUEST4(entity_manager);
	setup_QUEST5(entity_manager);
	setup_QUEST6(entity_manager);
	setup_EXAMINE1(entity_manager);
	setup_EXAMINE2(entity_manager);
	setup_EXAMINE3(entity_manager);
	setup_WATERSKIN1_EMPTY(entity_manager);
	setup_WATERSKIN1_1(entity_manager);
	setup_WATERSKIN1_2(entity_manager);
	setup_WATERSKIN1_3(entity_manager);
	setup_WATERSKIN2_EMPTY(entity_manager);
	setup_WATERSKIN2_1(entity_manager);
	setup_WATERSKIN2_2(entity_manager);
	setup_WATERSKIN2_3(entity_manager);
	setup_WATERSKIN2_4(entity_manager);
	setup_WATERSKIN2_5(entity_manager);
	setup_CLOCKWORK_BEETLE(entity_manager);
	setup_CLOCKWORK_BEETLE_COMBO1(entity_manager);
	setup_CLOCKWORK_BEETLE_COMBO2(entity_manager);
}

void setup_ammo(ecs::EntityManager &entity_manager)
{
	setup_tr4_ammo(item::ItemId::PISTOLS, item::ItemId::PISTOLS_AMMO, &Trng.pGlobTomb4->pAdr->pInventory->WeaponPistols, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::SHOTGUN, item::ItemId::SHOTGUN_AMMO1, &Trng.pGlobTomb4->pAdr->pInventory->WeaponShotGun, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::SHOTGUN, item::ItemId::SHOTGUN_AMMO2, &Trng.pGlobTomb4->pAdr->pInventory->WeaponShotGun, FWEAP_AMMO_SUPER, entity_manager);
	setup_tr4_ammo(item::ItemId::UZI, item::ItemId::UZI_AMMO, &Trng.pGlobTomb4->pAdr->pInventory->WeaponUZI, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::REVOLVER, item::ItemId::REVOLVER_AMMO, &Trng.pGlobTomb4->pAdr->pInventory->WeaponRevolver, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::REVOLVER_LASERSIGHT_COMBO, item::ItemId::REVOLVER_AMMO, &Trng.pGlobTomb4->pAdr->pInventory->WeaponRevolver, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::CROSSBOW, item::ItemId::CROSSBOW_AMMO1, &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::CROSSBOW, item::ItemId::CROSSBOW_AMMO2, &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_AMMO_SUPER, entity_manager);
	setup_tr4_ammo(item::ItemId::CROSSBOW, item::ItemId::CROSSBOW_AMMO3, &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_AMMO_EXPLOSIVE, entity_manager);
	setup_tr4_ammo(item::ItemId::CROSSBOW_LASERSIGHT_COMBO, item::ItemId::CROSSBOW_AMMO1, &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::CROSSBOW_LASERSIGHT_COMBO, item::ItemId::CROSSBOW_AMMO2, &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_AMMO_SUPER, entity_manager);
	setup_tr4_ammo(item::ItemId::CROSSBOW_LASERSIGHT_COMBO, item::ItemId::CROSSBOW_AMMO3, &Trng.pGlobTomb4->pAdr->pInventory->WeaponCrossBow, FWEAP_AMMO_EXPLOSIVE, entity_manager);
	setup_tr4_ammo(item::ItemId::GRENADE_GUN, item::ItemId::GRENADE_GUN_AMMO1, &Trng.pGlobTomb4->pAdr->pInventory->WeaponGrenadeGun, FWEAP_AMMO_NORMAL, entity_manager);
	setup_tr4_ammo(item::ItemId::GRENADE_GUN, item::ItemId::GRENADE_GUN_AMMO2, &Trng.pGlobTomb4->pAdr->pInventory->WeaponGrenadeGun, FWEAP_AMMO_SUPER, entity_manager);
	setup_tr4_ammo(item::ItemId::GRENADE_GUN, item::ItemId::GRENADE_GUN_AMMO3, &Trng.pGlobTomb4->pAdr->pInventory->WeaponGrenadeGun, FWEAP_AMMO_EXPLOSIVE, entity_manager);
}

void combo_combine(item::ComboData &combo_data)
{
	auto &item_first = combo_data.item_first;
	auto &item_second = combo_data.item_second;
	auto item_final = combo_data.item_final;
	auto item_extra = combo_data.item_extra;

	if (!item_first.has_component<item::ItemQuantity>()) {
		return;
	}
	if (!item_second.has_component<item::ItemQuantity>()) {
		return;
	}
	const auto &item_a_qty = *item_first.get_component<item::ItemQuantity>();
	const auto &item_b_qty = *item_second.get_component<item::ItemQuantity>();

	item_a_qty.decrement();
	item_b_qty.decrement();

	if (item_final && item_final->has_component<item::ItemQuantity>()) {
		item_final->get_component<item::ItemQuantity>()->increment();
	}
	if (item_extra && item_extra->has_component<item::ItemQuantity>()) {
		item_extra->get_component<item::ItemQuantity>()->increment();
	}
}

void combo_separate(item::ComboData &combo_data)
{
	auto &item_first = combo_data.item_first;
	auto &item_second = combo_data.item_second;
	auto item_final = combo_data.item_final;

	if (!combo_data.separable) {
		return;
	}
	if (!item_first.has_component<item::ItemQuantity>()) {
		return;
	}
	if (!item_second.has_component<item::ItemQuantity>()) {
		return;
	}
	if (!item_final || !item_final->has_component<item::ItemQuantity>()) {
		return;
	}
	const auto &item_first_qty = *item_first.get_component<item::ItemQuantity>();
	const auto &item_second_qty = *item_second.get_component<item::ItemQuantity>();
	const auto &item_final_qty = *item_final->get_component<item::ItemQuantity>();

	item_final_qty.decrement();
	item_first_qty.increment();
	item_second_qty.increment();
}

void setup_tr4_combo(
	int32_t item_id_first,
	int32_t item_id_second,
	int32_t item_id_final,
	int32_t item_id_extra,
	bool vice_versa,
	bool separable,
	ecs::EntityManager &entity_manager
)
{
	auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	const auto find_item_by_id = [&](int32_t item_id) -> ecs::Entity* {
		return entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
			return item_data.item_id == item_id;
		});
	};

	const auto item_first = find_item_by_id(item_id_first);
	const auto item_second = find_item_by_id(item_id_second);
	const auto item_final = find_item_by_id(item_id_final);
	const auto item_extra = find_item_by_id(item_id_extra);

	if (!item_first || !item_second) {
		return;
	}

	auto &combo_data = inventory->add_component(new item::ComboData(
		*item_first,
		*item_second,
		item_final,
		item_extra,
		vice_versa,
		separable
	));
	combo_data.combine = combo_combine;
	combo_data.separate = combo_separate;
}

void setup_combos(ecs::EntityManager &entity_manager)
{
	setup_tr4_combo(item::ItemId::REVOLVER, item::ItemId::LASERSIGHT, item::ItemId::REVOLVER_LASERSIGHT_COMBO, item::ItemId::NONE, true, true, entity_manager);
	setup_tr4_combo(item::ItemId::CROSSBOW, item::ItemId::LASERSIGHT, item::ItemId::CROSSBOW_LASERSIGHT_COMBO, item::ItemId::NONE, true, true, entity_manager);

	setup_tr4_combo(item::ItemId::PUZZLE1_COMBO1, item::ItemId::PUZZLE1_COMBO2, item::ItemId::PUZZLE1, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE2_COMBO1, item::ItemId::PUZZLE2_COMBO2, item::ItemId::PUZZLE2, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE3_COMBO1, item::ItemId::PUZZLE3_COMBO2, item::ItemId::PUZZLE3, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE4_COMBO1, item::ItemId::PUZZLE4_COMBO2, item::ItemId::PUZZLE4, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE5_COMBO1, item::ItemId::PUZZLE5_COMBO2, item::ItemId::PUZZLE5, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE6_COMBO1, item::ItemId::PUZZLE6_COMBO2, item::ItemId::PUZZLE6, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE7_COMBO1, item::ItemId::PUZZLE7_COMBO2, item::ItemId::PUZZLE7, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PUZZLE8_COMBO1, item::ItemId::PUZZLE8_COMBO2, item::ItemId::PUZZLE8, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY1_COMBO1, item::ItemId::KEY1_COMBO2, item::ItemId::KEY1, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY2_COMBO1, item::ItemId::KEY2_COMBO2, item::ItemId::KEY2, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY3_COMBO1, item::ItemId::KEY3_COMBO2, item::ItemId::KEY3, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY4_COMBO1, item::ItemId::KEY4_COMBO2, item::ItemId::KEY4, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY5_COMBO1, item::ItemId::KEY5_COMBO2, item::ItemId::KEY5, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY6_COMBO1, item::ItemId::KEY6_COMBO2, item::ItemId::KEY6, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY7_COMBO1, item::ItemId::KEY7_COMBO2, item::ItemId::KEY7, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::KEY8_COMBO1, item::ItemId::KEY8_COMBO2, item::ItemId::KEY8, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PICKUP1_COMBO1, item::ItemId::PICKUP1_COMBO2, item::ItemId::PICKUP1, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PICKUP2_COMBO1, item::ItemId::PICKUP2_COMBO2, item::ItemId::PICKUP2, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PICKUP3_COMBO1, item::ItemId::PICKUP3_COMBO2, item::ItemId::PICKUP3, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::PICKUP4_COMBO1, item::ItemId::PICKUP4_COMBO2, item::ItemId::PICKUP4, item::ItemId::NONE, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::CLOCKWORK_BEETLE_COMBO1, item::ItemId::CLOCKWORK_BEETLE_COMBO2, item::ItemId::CLOCKWORK_BEETLE, item::ItemId::NONE, true, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN1_EMPTY, item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_EMPTY, item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_EMPTY, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_EMPTY, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_1, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_EMPTY, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_2, true, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_EMPTY, item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN1_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_3, true, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_EMPTY, item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN1_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_1, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_4, true, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_EMPTY, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN1_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN1_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_EMPTY, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_1, true, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN2_5, item::ItemId::WATERSKIN1_2, true, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN2_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN2_1, item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_EMPTY, false, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_1, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN2_3, item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_2, false, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_EMPTY, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN2_2, item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_1, false, false, entity_manager);

	setup_tr4_combo(item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN1_1, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_2, false, false, entity_manager);
	setup_tr4_combo(item::ItemId::WATERSKIN2_4, item::ItemId::WATERSKIN1_2, item::ItemId::WATERSKIN1_3, item::ItemId::WATERSKIN2_3, false, false, entity_manager);
}

bool action_enabled_ammo(ecs::Entity &item) {
	return !item.get_components<item::ItemAmmo>([](item::ItemAmmo &item_ammo) -> bool {
		return !item_ammo.ammo_item.get_components<item::ItemQuantity>([](item::ItemQuantity &item_qty) -> bool {
			return item_qty.get_quantity() != 0;
		}).empty();
	}).empty();
}

bool action_enabled_combine(
	ecs::Entity &item,
	ecs::Entity &inventory
)
{
	auto combos = inventory.get_components<item::ComboData>();

	for (auto it = combos.begin(); it != combos.end(); ++it) {
		auto &combo = **it;

		auto &item_first = combo.item_first;
		auto &item_second = combo.item_second;

		if (item_first.get_id() != item.get_id() && item_second.get_id() != item.get_id()) {
			continue;
		}

		if (combo.vice_versa && item_first.get_id() != item.get_id() && item_first.has_component<item::ItemQuantity>()) {
			if (item_first.get_component<item::ItemQuantity>([](item::ItemQuantity &item_qty) -> bool {
				return item_qty.get_quantity() != 0;
			})) {
				return true;
			}
		}

		if (item_second.get_id() != item.get_id() && item_second.has_component<item::ItemQuantity>()) {
			if (item_second.get_component<item::ItemQuantity>([](item::ItemQuantity &item_qty) -> bool {
				return item_qty.get_quantity() != 0;
			})) {
				return true;
			}
		}
	}

	return false;
}

bool action_enabled_separate(
	ecs::Entity &item,
	ecs::Entity &inventory
)
{
	return inventory.get_component<item::ComboData>([&](item::ComboData &combo) -> bool {
		return combo.item_final
			&& combo.item_final->get_id() == item.get_id()
			&& combo.separable;
	}) != nullptr;
}

void setup_ammo_and_combo_actions(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	auto items = entity_manager.find_entities_with_component<item::ItemData>();
	for (auto item_it = items.begin(); item_it != items.end(); ++item_it) {
		auto &item = **item_it;

		auto &item_action_ammo = add_item_action(item, script::ScriptString(script::StringIndex::CHOOSE_AMMO), item::ItemActionType::CHOOSE_AMMO);
		item_action_ammo.action = nullptr;
		item_action_ammo.enabled = [&item]() -> bool {
			return action_enabled_ammo(item);
		};

		auto &item_action_combo = add_item_action(item, script::ScriptString(script::StringIndex::COMBINE), item::ItemActionType::COMBINE);
		item_action_combo.action = nullptr;
		item_action_combo.enabled = [&, inventory]() -> bool {
			return action_enabled_combine(item, *inventory);
		};

		auto &item_action_separate = add_item_action(item, script::ScriptString(script::StringIndex::SEPARATE), item::ItemActionType::SEPARATE);
		item_action_separate.action = nullptr;
		item_action_separate.enabled = [&, inventory]() -> bool {
			return action_enabled_separate(item, *inventory);
		};
	}
}

ecs::Entity& setup_ring(
	ring::RingId::Enum ring_id,
	script::ScriptString ring_name,
	ecs::EntityManager &entity_manager
)
{
	auto &ring = entity_manager.new_entity();

	ring.add_component(new ring::RingData(ring_id, ring_name));
	ring.add_component(new ring::RingDisplay());
	ring.add_component(new ring::RingState());

	return ring;
}

void setup_rings(ecs::EntityManager &entity_manager)
{
	setup_ring(ring::RingId::CONTEXT, script::ScriptString(), entity_manager);
	setup_ring(ring::RingId::OPTIONS, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[0])), entity_manager);
	setup_ring(ring::RingId::DOCUMENTS, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[1])), entity_manager);
	setup_ring(ring::RingId::INVENTORY, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[2])), entity_manager);
	setup_ring(ring::RingId::WEAPONS, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[3])), entity_manager);
	setup_ring(ring::RingId::ITEMS, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[4])), entity_manager);
	setup_ring(ring::RingId::MISC1, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[5])), entity_manager);
	setup_ring(ring::RingId::MISC2, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[6])), entity_manager);
	setup_ring(ring::RingId::MISC3, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[7])), entity_manager);
	setup_ring(ring::RingId::MISC4, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[8])), entity_manager);
	setup_ring(ring::RingId::MISC5, script::ScriptString(const_cast<char*>(ring::DEFAULT_RING_NAMES[9])), entity_manager);
}

void setup_camera(ecs::EntityManager &entity_manager)
{
	auto &camera = entity_manager.new_entity();

	camera.add_component(new camera::CameraView());
}

void setup_text(ecs::Entity &inventory)
{
	inventory.add_component(new text::TextConfig(
		text::TextType::RING_NAME,
		500,
		100,
		0,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER
	));

	inventory.add_component(new text::TextConfig(
		text::TextType::ITEM_NAME_IDLE,
		500,
		900,
		0,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER,
		45
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::ITEM_DESC_IDLE,
		500,
		945,
		enumFT.SIZE_ATOMIC_CHAR,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER,
		40
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::ITEM_AMMO_IDLE,
		500,
		945,
		enumFT.LITTLE_ALWAYS,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER,
		40
	));

	inventory.add_component(new text::TextConfig(
		text::TextType::ITEM_NAME_ACTIVE,
		500,
		100,
		0,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER,
		50
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::ITEM_DESC_ACTIVE,
		500,
		150,
		enumFT.SIZE_ATOMIC_CHAR,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER,
		50
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::ITEM_AMMO_ACTIVE,
		500,
		150,
		enumFT.LITTLE_ALWAYS,
		enumFC.GOLD,
		enumFTS.ALIGN_CENTER,
		50
	));

	inventory.add_component(new text::TextConfig(
		text::TextType::EXAMINE_1,
		500,
		100,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER,
		300
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::EXAMINE_2,
		500,
		400,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER,
		300
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::EXAMINE_3,
		500,
		700,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER,
		300
	));

	inventory.add_component(new text::TextConfig(
		text::TextType::ACTION_MENU,
		500,
		600,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER,
		70
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::ACTION_MENU_HIGHLIGHT,
		500,
		600,
		0,
		enumFC.WHITE_PULSE,
		enumFTS.ALIGN_CENTER,
		70
	));

	inventory.add_component(new text::TextConfig(
		text::TextType::CONTEXT_ACTION,
		500,
		180,
		0,
		enumFC.WHITE_PULSE,
		enumFTS.ALIGN_CENTER,
		70
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::SPECIAL_ACTION,
		500,
		900,
		0,
		enumFC.WHITE_PULSE,
		enumFTS.ALIGN_CENTER,
		50
	));

	inventory.add_component(new text::TextConfig(
		text::TextType::NAV_UP_LEFT,
		50,
		100,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::NAV_UP_RIGHT,
		950,
		100,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::NAV_DOWN_LEFT,
		50,
		900,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::NAV_DOWN_RIGHT,
		950,
		900,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::NAV_LEFT,
		50,
		900,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER
	));
	inventory.add_component(new text::TextConfig(
		text::TextType::NAV_RIGHT,
		950,
		900,
		0,
		enumFC.LIGHT_GRAY,
		enumFTS.ALIGN_CENTER
	));
}

void setup_lighting(ecs::Entity &inventory)
{
	// find the brightest room to use as default lighting location
	StrRoomTr4 *room_brightest = nullptr;
	int32_t room_brightest_index = -1;

	auto &room_count = *Trng.pGlobTomb4->pAdr->pTotRooms;
	for (int32_t i = 0; i < room_count; ++i) {
		auto &room = Trng.pGlobTomb4->pAdr->pVetRooms[i];

		if (!room_brightest || room.ColorIntensityLight > room_brightest->ColorIntensityLight) {
			room_brightest = &room;
			room_brightest_index = i;
		}
	}

	if (room_brightest && room_brightest_index >= 0) {
		inventory.add_component(new render::LightingLocation(
			room_brightest_index,
			room_brightest->OriginX + 512,
			room_brightest->OrigYBottom,
			room_brightest->OriginZ + 512
		));
	}
}

void setup_cheats(ecs::EntityManager &entity_manager)
{
	const auto set_unlimited = [](ecs::Entity *item) -> void {
		if (item && item->has_component<item::ItemQuantity>()) {
			auto &item_qty = *item->get_component<item::ItemQuantity>();

			item_qty.unlimited();
		}
	};
	const auto give_weapon = [](ecs::Entity *item) -> void {
		if (item && item->has_component<item::ItemQuantity>()) {
			auto &item_qty = *item->get_component<item::ItemQuantity>();

			item_qty.set_quantity(1);
		}
	};

	auto item_BIGMEDI = item::get_item_by_item_id(item::ItemId::BIGMEDI, entity_manager);
	auto item_SMALLMEDI = item::get_item_by_item_id(item::ItemId::SMALLMEDI, entity_manager);
	auto item_MEMCARD_LOAD_INV = item::get_item_by_item_id(item::ItemId::MEMCARD_LOAD_INV, entity_manager);

	cheat::CheatConfig *cheat_WEAPON = nullptr;

	if (item_BIGMEDI) {
		cheat_WEAPON = &item_BIGMEDI->add_component(new cheat::CheatConfig(17, 18, 30, 25, 24, 49));

		cheat_WEAPON->enabled = []() -> bool { return false; }; // disabled until GUNS has been performed

		cheat_WEAPON->action = [=, &entity_manager]() -> void {
			const auto item_PISTOLS = item::get_item_by_item_id(item::ItemId::PISTOLS, entity_manager);
			const auto item_SHOTGUN = item::get_item_by_item_id(item::ItemId::SHOTGUN, entity_manager);
			const auto item_UZI = item::get_item_by_item_id(item::ItemId::UZI, entity_manager);
			const auto item_REVOLVER = item::get_item_by_item_id(item::ItemId::REVOLVER, entity_manager);
			const auto item_CROSSBOW = item::get_item_by_item_id(item::ItemId::CROSSBOW, entity_manager);
			const auto item_GRENADE_GUN = item::get_item_by_item_id(item::ItemId::GRENADE_GUN, entity_manager);

			const auto item_PISTOLS_AMMO = item::get_item_by_item_id(item::ItemId::PISTOLS_AMMO, entity_manager);
			const auto item_SHOTGUN_AMMO1 = item::get_item_by_item_id(item::ItemId::SHOTGUN_AMMO1, entity_manager);
			const auto item_SHOTGUN_AMMO2 = item::get_item_by_item_id(item::ItemId::SHOTGUN_AMMO2, entity_manager);
			const auto item_UZI_AMMO = item::get_item_by_item_id(item::ItemId::UZI_AMMO, entity_manager);
			const auto item_REVOLVER_AMMO = item::get_item_by_item_id(item::ItemId::REVOLVER_AMMO, entity_manager);
			const auto item_CROSSBOW_AMMO1 = item::get_item_by_item_id(item::ItemId::CROSSBOW_AMMO1, entity_manager);
			const auto item_CROSSBOW_AMMO2 = item::get_item_by_item_id(item::ItemId::CROSSBOW_AMMO2, entity_manager);
			const auto item_CROSSBOW_AMMO3 = item::get_item_by_item_id(item::ItemId::CROSSBOW_AMMO3, entity_manager);
			const auto item_GRENADE_GUN_AMMO1 = item::get_item_by_item_id(item::ItemId::GRENADE_GUN_AMMO1, entity_manager);
			const auto item_GRENADE_GUN_AMMO2 = item::get_item_by_item_id(item::ItemId::GRENADE_GUN_AMMO2, entity_manager);
			const auto item_GRENADE_GUN_AMMO3 = item::get_item_by_item_id(item::ItemId::GRENADE_GUN_AMMO3, entity_manager);

			give_weapon(item_PISTOLS);
			give_weapon(item_SHOTGUN);
			give_weapon(item_UZI);
			give_weapon(item_REVOLVER);
			give_weapon(item_CROSSBOW);
			give_weapon(item_GRENADE_GUN);

			set_unlimited(item_PISTOLS_AMMO);
			set_unlimited(item_SHOTGUN_AMMO1);
			set_unlimited(item_SHOTGUN_AMMO2);
			set_unlimited(item_UZI_AMMO);
			set_unlimited(item_REVOLVER_AMMO);
			set_unlimited(item_CROSSBOW_AMMO1);
			set_unlimited(item_CROSSBOW_AMMO2);
			set_unlimited(item_CROSSBOW_AMMO3);
			set_unlimited(item_GRENADE_GUN_AMMO1);
			set_unlimited(item_GRENADE_GUN_AMMO2);
			set_unlimited(item_GRENADE_GUN_AMMO3);
		};
	}

	if (item_SMALLMEDI) {
		auto &cheat_GUNS = item_SMALLMEDI->add_component(new cheat::CheatConfig(34, 22, 49, 31));

		cheat_GUNS.enabled = cheat::facing_north;

		cheat_GUNS.action = [=, &entity_manager]() -> void {
			const auto item_FLARE_INV = item::get_item_by_item_id(item::ItemId::FLARE_INV, entity_manager);

			set_unlimited(item_SMALLMEDI);
			set_unlimited(item_BIGMEDI);
			set_unlimited(item_FLARE_INV);

			if (cheat_WEAPON) {
				cheat_WEAPON->enabled = cheat::facing_north;
			}
		};
	}

	if (item_MEMCARD_LOAD_INV) {
		auto &cheat_HELP = item_MEMCARD_LOAD_INV->add_component(new cheat::CheatConfig(35, 18, 38, 25));

		cheat_HELP.enabled = cheat::facing_north;

		cheat_HELP.action = []() -> void {
			// jump to next level
			PerformFlipeffect(nullptr, 4, 0, 0);
		};
	}
}

void customize_ring(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 2) {
		return;
	}

	int32_t cust_index = -1;

	const auto ring_id = customize.pVetArg[++cust_index];
	const auto name_stridex = customize.pVetArg[++cust_index];

	auto ring = entity_manager.find_entity_with_component<ring::RingData>([&](const ring::RingData &ring_data) -> bool {
		return ring_data.ring_id == ring_id;
	});
	if (!ring) {
		ring = &setup_ring(static_cast<ring::RingId::Enum>(ring_id), script::ScriptString(), entity_manager);
	}
	auto &ring_data = *ring->get_component<ring::RingData>();

	if (name_stridex != -1) {
		ring_data.name = script::ScriptString(name_stridex);
	}
}

void customize_item(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 3) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto name_stridex = customize.pVetArg[++cust_index];
	const auto desc_stridex = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		item = &entity_manager.new_entity();
		item->add_component(new item::ItemData(item_id));

		// set some defaults
		Get(enumGET.SLOT, enumSLOT.LARA_START_POS, 0); // dummy model
		const auto &item_model_idle = item->add_component(new item::ItemModelConfig(item::ItemModelType::IDLE, *GET.pSlot, enumSLOT.LARA_START_POS));
		item->add_component(new item::ItemModel(item_model_idle));

		set_default_item_displays(*item);

		add_to_ring(*item, ring::RingId::INVENTORY, entity_manager);
	}

	auto &item_data = *item->get_component<item::ItemData>();

	if (name_stridex != -1) {
		item_data.name = script::ScriptString(name_stridex);
	}
	if (desc_stridex != -1) {
		item_data.description = script::ScriptString(desc_stridex);
	}
}

void customize_item_ring(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments != 3) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto ring_id = customize.pVetArg[++cust_index];
	const auto sort_index = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	if (ring_id < ring::MIN_INVENTORY_RING_ID) {
		return;
	}

	const auto ring = entity_manager.find_entity_with_component<ring::RingData>([&](const ring::RingData &rd) -> bool {
		return rd.ring_id == ring_id;
	});
	if (!ring) {
		return;
	}

	auto item_ring = item->get_component<item::ItemRing>();
	if (!item_ring) {
		item_ring = &item->add_component(new item::ItemRing(ring_id));
	}
	else {
		item_ring->ring_id = ring_id;
	}

	if (sort_index >= 0) {
		item_ring->sort_index = sort_index;
	}
}

void customize_item_model(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 7) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	for (int32_t i = 1; i < customize.NArguments; i += 4) {
		const auto type_id = customize.pVetArg[++cust_index];
		const auto slot_id = customize.pVetArg[++cust_index];
		const auto sprite_id = customize.pVetArg[++cust_index];
		const auto size_x = customize.pVetArg[++cust_index];
		const auto size_y = customize.pVetArg[++cust_index];
		const auto mesh_mask = customize.pVetArg[++cust_index];

		if (type_id < 1) {
			continue;
		}
		const auto type = static_cast<item::ItemModelType::Enum>(type_id);

		if (slot_id < 0 || !Get(enumGET.SLOT, slot_id, 0)) {
			continue;
		}
		const auto slot = GET.pSlot;

		if (!core::bit_set(slot->Flags, enumFSLOT.PRESENT)) {
			continue;
		}

		auto model_config = item->get_component<item::ItemModelConfig>([&](item::ItemModelConfig &model) -> bool {
			return model.type == type;
		});
		if (!model_config) {
			model_config = &item->add_component(new item::ItemModelConfig(type, *slot, slot_id));
		}

		model_config->slot = slot;
		model_config->slot_id = slot_id;

		if (sprite_id >= 0) {
			model_config->sprite_id = sprite_id;
		}
		if (size_x > 0) {
			model_config->size_x = size_x;
		}
		if (size_y > 0) {
			model_config->size_y = size_y;
		}
		if (mesh_mask >= 0) {
			model_config->mesh_mask = mesh_mask;
		}
	}
}

void customize_item_display(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 12) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	for (int32_t i = 1; i < customize.NArguments; i += 11) {
		const auto type_id = customize.pVetArg[++cust_index];
		const auto pos_x = customize.pVetArg[++cust_index];
		const auto pos_y = customize.pVetArg[++cust_index];
		const auto pos_z = customize.pVetArg[++cust_index];
		const auto pos_ignore_anim = customize.pVetArg[++cust_index];
		const auto orient_x = customize.pVetArg[++cust_index];
		const auto orient_y = customize.pVetArg[++cust_index];
		const auto orient_z = customize.pVetArg[++cust_index];
		const auto orient_ignore_anim = customize.pVetArg[++cust_index];
		const auto tilt = customize.pVetArg[++cust_index];
		const auto scale = customize.pVetArg[++cust_index];

		if (type_id < 1) {
			continue;
		}
		const auto type = static_cast<item::ItemDisplayType::Enum>(type_id);

		auto item_display_config = item->get_component<item::ItemDisplayConfig>([&](item::ItemDisplayConfig &display_config) -> bool {
			return display_config.type == type;
		});
		if (!item_display_config) {
			item_display_config = &item->add_component(new item::ItemDisplayConfig(type));
		}

		if (pos_x != -1) {
			item_display_config->pos.x = pos_x;
		}

		if (pos_y != -1) {
			item_display_config->pos.y = pos_y;
		}

		if (pos_z != -1) {
			item_display_config->pos.z = pos_z;
		}

		item_display_config->pos_ignore_anim = pos_ignore_anim == CINV_TRUE;

		if (orient_x != -1) {
			item_display_config->orient.x = orient_x;
		}

		if (orient_y != -1) {
			item_display_config->orient.y = orient_y;
		}

		if (orient_z != -1) {
			item_display_config->orient.z = orient_z;
		}

		item_display_config->orient_ignore_anim = orient_ignore_anim == CINV_TRUE;

		if (tilt != -1) {
			item_display_config->tilt = tilt;
		}

		if (scale != -1) {
			item_display_config->scale = scale / 100.f;
		}
	}
}

void customize_item_quantity(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 5) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto qty_min = customize.pVetArg[++cust_index];
	const auto qty_max = customize.pVetArg[++cust_index];
	const auto qty_get_tgroup = customize.pVetArg[++cust_index];
	const auto qty_set_tgroup = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto item_qty = item->get_component<item::ItemQuantity>();
	if (!item_qty) {
		item_qty = &item->add_component(new item::ItemQuantity());
	}

	item_qty->quantity_min = qty_min;
	item_qty->quantity_max = qty_max;

	if (qty_get_tgroup != -1) {
		item_qty->get_quantity = [=]()-> int32_t {
			PerformTriggerGroup(qty_get_tgroup);

			return Trng.pGlobTomb4->pBaseVariableTRNG->Globals.CurrentValue;
		};
	}

	if (qty_set_tgroup != -1) {
		item_qty->set_quantity = [=](int32_t qty)-> void {
			Trng.pGlobTomb4->pBaseVariableTRNG->Globals.CurrentValue = qty;

			PerformTriggerGroup(qty_set_tgroup);
		};
	}
}

void customize_item_animation(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 6) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	for (int32_t i = 1; i < customize.NArguments; i += 5) {
		const auto type = customize.pVetArg[++cust_index];
		const auto anim_index = customize.pVetArg[++cust_index];
		const auto frame_start = customize.pVetArg[++cust_index];
		const auto frame_end = customize.pVetArg[++cust_index];
		const auto wait_for_motions = customize.pVetArg[++cust_index];

		auto item_anim = item->get_component<item::ItemAnimation>([&](const item::ItemAnimation &item_animation) -> bool {
			return item_animation.type == type;
		});
		if (!item_anim) {
			item_anim = &item->add_component(new item::ItemAnimation(static_cast<item::ItemAnimationType::Enum>(type)));
		}
		if (anim_index != -1) {
			item_anim->anim_index = anim_index;
		}
		if (frame_start != -1) {
			item_anim->frame_start = frame_start;
		}
		if (frame_end != -1) {
			item_anim->frame_end = frame_end;
		}
		if (wait_for_motions != -1) {
			item_anim->wait_for_motions = wait_for_motions == CINV_TRUE;
		}
	}
}

void customize_item_action(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 7) {
		return;
	}

	const auto item_id = customize.pVetArg[0];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	for (int32_t i = 1; i < customize.NArguments; i += 6) {
		const auto type = customize.pVetArg[i];
		const auto name_stridex = customize.pVetArg[i + 1];
		const auto sort_index = customize.pVetArg[i + 2];
		const auto replace = customize.pVetArg[i + 3];
		const auto enabled_cgroup = customize.pVetArg[i + 4];
		const auto action_tgroup = customize.pVetArg[i + 5];

		if (type == -1) {
			continue;
		}

		item::ItemAction *item_action = nullptr;
		if (type != item::ItemActionType::CUSTOM) { // can have multiple custom actions
			item_action = item->get_component<item::ItemAction>([&](const item::ItemAction &iaction) -> bool {
				return iaction.type == type;
			});
		}
		if (!item_action) {
			item_action = &add_item_action(*item);
		}

		item_action->type = static_cast<item::ItemActionType::Enum>(type);

		if (name_stridex >= 0) {
			item_action->name = script::ScriptString(name_stridex);
		}

		if (sort_index >= 0) {
			item_action->sort_index = sort_index;
		}

		if (replace >= 0 && !item_action->action) {
			item_action->replace_default = replace == CINV_TRUE;
		}

		if (enabled_cgroup >= 0) {
			const auto enabled_proc = item_action->enabled;

			item_action->enabled = [=]() -> bool {
				bool enabled = true;

				if (enabled_proc && replace != CINV_TRUE) {
					enabled = enabled_proc();
				}

				return enabled && PerformTriggerGroup(enabled_cgroup) > 0;
			};
		}

		if (action_tgroup >= 0) {
			const auto action_proc = item_action->action;

			item_action->action = [=]() -> void {
				if (action_proc && replace != CINV_TRUE) {
					action_proc();
				}

				PerformTriggerGroup(action_tgroup);
			};
		}
	}
}

void customize_item_sfx(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 3) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	for (int32_t i = 1; i < customize.NArguments; i += 2) {
		const auto type_id = customize.pVetArg[++cust_index];
		const auto sound_id = customize.pVetArg[++cust_index];

		if (type_id < 1 || type_id >= sound::SfxType::NONE) {
			return;
		}
		const auto type = sound::SfxType::Enum(type_id);

		item::ItemSfx *item_sfx = nullptr;
		if (item->has_component<item::ItemSfx>()) {
			item_sfx = item->get_component<item::ItemSfx>([&](item::ItemSfx &sfx) -> bool {
				return sfx.type == type;
			});
		}
		if (!item_sfx) {
			item_sfx = &item->add_component(new item::ItemSfx(type));
		}

		item_sfx->sound_id = sound_id;
	}
}

void customize_ammo(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 4) {
		return;
	}

	int32_t cust_index = -1;

	const auto weapon_item_id = customize.pVetArg[++cust_index];

	auto weapon = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == weapon_item_id;
	});
	if (!weapon) {
		return;
	}

	for (int32_t i = 1; i < customize.NArguments; i += 3) {
		const auto ammo_item_id = customize.pVetArg[++cust_index];
		const auto loaded_cgroup = customize.pVetArg[++cust_index];
		const auto load_tgroup = customize.pVetArg[++cust_index];

		auto ammo = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
			return item_data.item_id == ammo_item_id;
		});
		if (!ammo) {
			return;
		}

		auto item_ammo = weapon->get_component<item::ItemAmmo>([&](item::ItemAmmo &ia) -> bool {
			return ia.ammo_item.get_id() == ammo->get_id();
		});
		if (!item_ammo) {
			item_ammo = &weapon->add_component(new item::ItemAmmo(*ammo));
		}

		if (loaded_cgroup >= 0) {
			item_ammo->loaded = [=]()->bool {
				return PerformTriggerGroup(loaded_cgroup) > 0;
			};
		}
		if (load_tgroup >= 0) {
			item_ammo->load = [=]() -> void {
				PerformTriggerGroup(load_tgroup);
			};
		}
	}
}

void customize_examine(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 4) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto text1 = customize.pVetArg[++cust_index];
	const auto text2 = customize.pVetArg[++cust_index];
	const auto text3 = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto item_action = item->get_component<item::ItemAction>([&](const item::ItemAction &iaction) -> bool {
		return iaction.type == item::ItemActionType::EXAMINE;
	});
	if (!item_action) {
		item_action = &add_item_action(*item, script::ScriptString(script::StringIndex::EXAMINE), item::ItemActionType::EXAMINE);
	}

	auto examine_data = item->get_component<item::ExamineData>();
	if (!examine_data) {
		examine_data = &item->add_component(new item::ExamineData());
	}

	if (text1 >= 0) {
		examine_data->text_1 = script::ScriptString(text1);
	}
	if (text2 >= 0) {
		examine_data->text_2 = script::ScriptString(text2);
	}
	if (text3 >= 0) {
		examine_data->text_3 = script::ScriptString(text3);
	}
}

void customize_health(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 7) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto health_points = customize.pVetArg[++cust_index];
	const auto poison_points = customize.pVetArg[++cust_index];
	const auto heal_sound_id = customize.pVetArg[++cust_index];
	const auto hurt_sound_id = customize.pVetArg[++cust_index];
	const auto cure_poison = customize.pVetArg[++cust_index];
	const auto increase_usage_stats = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto item_action = item->get_component<item::ItemAction>([&](const item::ItemAction &iaction) -> bool {
		return iaction.type == item::ItemActionType::USE;
	});
	if (!item_action) {
		item_action = &add_item_action(*item, script::ScriptString(script::StringIndex::USE), item::ItemActionType::USE);

		// make it the first action
		item_action->sort_index = -1;
	}

	auto health_data = item->get_component<item::HealthData>();
	if (!health_data) {
		health_data = &item->add_component(new item::HealthData(0, 0, 116, 31));

		// only perform existing action if first time adding health data
		// this is to allow existing medi-actions to be performed only once
		const auto action_proc = item_action->action;
		item_action->action = [=]() -> void {
			if (action_proc) {
				action_proc();
			}

			action::use_health(*item);
		};
	}

	if (health_points != -1) {
		health_data->health_points = health_points;
	}
	if (poison_points >= 0) {
		health_data->poison_points = poison_points;
	}
	if (heal_sound_id >= 0) {
		health_data->heal_sound_id = heal_sound_id;
	}
	if (hurt_sound_id >= 0) {
		health_data->hurt_sound_id = hurt_sound_id;
	}
	if (cure_poison >= 0) {
		health_data->cure_poison = cure_poison == CINV_TRUE;
	}
	if (increase_usage_stats >= 0) {
		health_data->increase_usage_stats = increase_usage_stats == CINV_TRUE;
	}
}

void customize_compass(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 7) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto needle_mesh_index = customize.pVetArg[++cust_index];
	const auto needle_mesh_axis = customize.pVetArg[++cust_index];
	const auto needle_oscill_amp_max = customize.pVetArg[++cust_index];
	const auto needle_oscill_amp_min = customize.pVetArg[++cust_index];
	const auto needle_oscill_amp_settle_frames = customize.pVetArg[++cust_index];
	const auto needle_oscill_period_frames = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto compass_data = item->get_component<item::CompassData>();
	if (!compass_data) {
		compass_data = &item->add_component(new item::CompassData(0, core::Axis::Y));
	}

	if (needle_mesh_index >= 0) {
		compass_data->needle_mesh_index = needle_mesh_index;
	}
	if (needle_mesh_axis >= 0) {
		compass_data->needle_mesh_axis = static_cast<core::Axis::Enum>(needle_mesh_axis);
	}
	if (needle_oscill_amp_max >= 0) {
		compass_data->needle_oscill_amplitude_max = needle_oscill_amp_max;
	}
	if (needle_oscill_amp_min >= 0) {
		compass_data->needle_oscill_amplitude_min = needle_oscill_amp_min;
	}
	if (needle_oscill_amp_settle_frames >= 0) {
		compass_data->needle_oscill_amplitude_settle_frames = needle_oscill_amp_settle_frames;
	}
	if (needle_oscill_period_frames >= 0) {
		compass_data->needle_oscill_period_frames = needle_oscill_period_frames;
	}
}

void customize_stopwatch(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 8) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto hour_hand_mesh_index = customize.pVetArg[++cust_index];
	const auto hour_hand_mesh_axis = customize.pVetArg[++cust_index];
	const auto minute_hand_mesh_index = customize.pVetArg[++cust_index];
	const auto minute_hand_mesh_axis = customize.pVetArg[++cust_index];
	const auto second_hand_mesh_index = customize.pVetArg[++cust_index];
	const auto second_hand_mesh_axis = customize.pVetArg[++cust_index];
	const auto frequency_frames = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto stopwatch_data = item->get_component<item::StopwatchData>();
	if (!stopwatch_data) {
		stopwatch_data = &item->add_component(new item::StopwatchData(0, core::Axis::Y, 0, core::Axis::Y, 0, core::Axis::Y));
	}

	if (hour_hand_mesh_index >= 0) {
		stopwatch_data->hour_hand_mesh_index = hour_hand_mesh_index;
	}
	if (hour_hand_mesh_axis >= 0) {
		stopwatch_data->hour_hand_mesh_axis = static_cast<core::Axis::Enum>(hour_hand_mesh_axis);
	}

	if (minute_hand_mesh_index >= 0) {
		stopwatch_data->minute_hand_mesh_index = minute_hand_mesh_index;
	}
	if (minute_hand_mesh_axis >= 0) {
		stopwatch_data->minute_hand_mesh_axis = static_cast<core::Axis::Enum>(minute_hand_mesh_axis);
	}

	if (second_hand_mesh_index >= 0) {
		stopwatch_data->second_hand_mesh_index = second_hand_mesh_index;
	}
	if (second_hand_mesh_axis >= 0) {
		stopwatch_data->second_hand_mesh_axis = static_cast<core::Axis::Enum>(second_hand_mesh_axis);
	}

	if (frequency_frames >= 0) {
		stopwatch_data->frequency_frames = frequency_frames;
	}
}

void customize_passport(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 2) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto page_sound_id = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto passport_data = item->get_component<item::PassportData>();
	if (!passport_data) {
		passport_data = &item->add_component(new item::PassportData());
	}

	if (page_sound_id >= 0) {
		passport_data->page_sound_id = page_sound_id;
	}
}

void customize_map(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 2) {
		return;
	}

	int32_t cust_index = -1;

	const auto item_id = customize.pVetArg[++cust_index];
	const auto cancelable = customize.pVetArg[++cust_index];

	auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
	if (!item) {
		return;
	}

	auto map_data = item->get_component<item::MapData>();
	if (!map_data) {
		map_data = &item->add_component(new item::MapData());
	}

	if (cancelable >= 0) {
		map_data->cancelable = cancelable == CINV_TRUE;
	}

	for (int32_t i = 2; i < customize.NArguments; i += 4) {
		const auto marker_mesh_index = customize.pVetArg[++cust_index];
		const auto map_orient_x = customize.pVetArg[++cust_index];
		const auto map_orient_y = customize.pVetArg[++cust_index];
		const auto map_orient_z = customize.pVetArg[++cust_index];

		if (marker_mesh_index < 0) {
			continue;
		}

		map_data->markers.push_back(item::MapMarker(marker_mesh_index));

		auto &marker = map_data->markers.back();

		if (map_orient_x != -1) {
			marker.map_orient.x = map_orient_x;
		}
		if (map_orient_y != -1) {
			marker.map_orient.y = map_orient_y;
		}
		if (map_orient_z != -1) {
			marker.map_orient.z = map_orient_z;
		}
	}
}

void customize_duration(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 2) {
		return;
	}

	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryDuration>();
	if (!inventory) {
		return;
	}
	auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();

	int32_t cust_index = -1;

	const auto type_id = customize.pVetArg[++cust_index];
	const auto duration_frames = customize.pVetArg[++cust_index];

	if (type_id <= 0) {
		return;
	}
	const auto type = inventory::DurationType::Enum(type_id);

	if (duration_frames >= 0) {
		switch (type) {
		case inventory::DurationType::INVENTORY_OPEN:
			inventory_duration.inventory_open_frames = duration_frames;
			break;
		case inventory::DurationType::RING_ROTATE:
			inventory_duration.ring_rotate_frames = duration_frames;
			break;
		case inventory::DurationType::RING_CHANGE:
			inventory_duration.ring_change_frames = duration_frames;
			break;
		case inventory::DurationType::ITEM_SELECT:
			inventory_duration.item_select_frames = duration_frames;
			break;
		case inventory::DurationType::ITEM_SPIN:
			inventory_duration.item_spin_frames = duration_frames;
			break;
		}
	}
}

void customize_display(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 2) {
		return;
	}

	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryDisplay>();
	if (!inventory) {
		return;
	}
	auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

	int32_t cust_index = -1;

	const auto ring_radius = customize.pVetArg[++cust_index];
	const auto item_base_size = customize.pVetArg[++cust_index];

	if (ring_radius >= 0) {
		inventory_display.ring_radius_opened = ring_radius;
	}
	if (item_base_size >= 0) {
		inventory_display.item_base_size = item_base_size;
	}
}

void customize_sfx(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 2) {
		return;
	}

	const auto inventory = entity_manager.find_entity_with_component<inventory::InventorySfx>();
	if (!inventory) {
		return;
	}
	auto &inventory_sfx = *inventory->get_component<inventory::InventorySfx>();

	int32_t cust_index = -1;

	const auto type_id = customize.pVetArg[++cust_index];
	const auto sound_id = customize.pVetArg[++cust_index];

	if (type_id < 1 || type_id >= sound::SfxType::NONE) {
		return;
	}
	const auto type = sound::SfxType::Enum(type_id);

	switch (type) {
	case sound::SfxType::INVENTORY_OPEN:
		inventory_sfx.opening_sound_id = sound_id;
		break;
	case sound::SfxType::INVENTORY_CLOSE:
		inventory_sfx.closing_sound_id = sound_id;
		break;
	case sound::SfxType::RING_ROTATE:
		inventory_sfx.ring_rotate_sound_id = sound_id;
		break;
	case sound::SfxType::RING_CHANGE:
		inventory_sfx.ring_change_sound_id = sound_id;
		break;
	case sound::SfxType::ITEM_SELECT:
		inventory_sfx.item_select_sound_id = sound_id;
		break;
	case sound::SfxType::ITEM_CANCEL:
		inventory_sfx.item_cancel_sound_id = sound_id;
		break;
	case sound::SfxType::MENU_CHANGE:
		inventory_sfx.menu_change_sound_id = sound_id;
		break;
	case sound::SfxType::LOAD_AMMO:
		inventory_sfx.ammo_load_sound_id = sound_id;
		break;
	case sound::SfxType::COMBINE:
		inventory_sfx.combine_sound_id = sound_id;
		break;
	case sound::SfxType::SEPARATE:
		inventory_sfx.separate_sound_id = sound_id;
		break;
	case sound::SfxType::FAIL:
		inventory_sfx.fail_sound_id = sound_id;
		break;
	}
}

void customize_text(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 8) {
		return;
	}

	auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	int32_t cust_index = -1;

	const auto type_id = customize.pVetArg[++cust_index];
	const auto screen_x = customize.pVetArg[++cust_index];
	const auto screen_y = customize.pVetArg[++cust_index];
	const auto size = customize.pVetArg[++cust_index];
	const auto colour = customize.pVetArg[++cust_index];
	const auto align = customize.pVetArg[++cust_index];
	const auto line_height = customize.pVetArg[++cust_index];
	const auto enabled = customize.pVetArg[++cust_index];

	if (type_id < 1) {
		return;
	}
	const auto type = static_cast<text::TextType::Enum>(type_id);

	auto text_config = inventory->get_component<text::TextConfig>([&](text::TextConfig &config) -> bool {
		return config.type == type;
	});
	if (!text_config) {
		text_config = &inventory->add_component(new text::TextConfig(type));
	}

	if (screen_x >= 0 && screen_x <= 1000) {
		text_config->x = screen_x;
	}
	if (screen_y >= 0 && screen_y <= 1000) {
		text_config->y = screen_y;
	}
	if (size != -1) {
		text_config->size = size;
	}
	if (colour != -1) {
		text_config->colour = colour;
	}
	if (align != -1) {
		text_config->align = align;
	}
	if (line_height >= 0) {
		text_config->line_height = line_height;
	}
	if (enabled != -1
		&& type != text::TextType::ACTION_MENU
		&& type != text::TextType::ACTION_MENU_HIGHLIGHT) {
		text_config->enabled = enabled == CINV_TRUE;
	}
}

void customize_combo(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 8) {
		return;
	}

	auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	int32_t cust_index = -1;

	for (int32_t i = 0; i < customize.NArguments; i += 8) {
		const auto first_item_id = customize.pVetArg[++cust_index];
		const auto second_item_id = customize.pVetArg[++cust_index];
		const auto final_item_id = customize.pVetArg[++cust_index];
		const auto extra_item_id = customize.pVetArg[++cust_index];
		const auto vice_versa = customize.pVetArg[++cust_index];
		const auto separable = customize.pVetArg[++cust_index];
		const auto combine_tgroup = customize.pVetArg[++cust_index];
		const auto separate_tgroup = customize.pVetArg[++cust_index];

		if (first_item_id == item::ItemId::NONE || second_item_id == item::ItemId::NONE) {
			continue;
		}

		const auto find_item_by_id = [](int32_t item_id, ecs::EntityManager &entity_manager) -> ecs::Entity* {
			return entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
				return item_data.item_id == item_id;
			});
		};

		const auto first_item = find_item_by_id(first_item_id, entity_manager);
		const auto second_item = find_item_by_id(second_item_id, entity_manager);
		const auto final_item = find_item_by_id(final_item_id, entity_manager);
		const auto extra_item = find_item_by_id(extra_item_id, entity_manager);

		if (!first_item || !second_item) {
			continue;
		}

		auto combo_data = inventory->get_component<item::ComboData>([&](item::ComboData &combo) -> bool {
			return ((combo.item_first.get_id() == first_item->get_id()
				&& combo.item_second.get_id() == second_item->get_id())
				|| (combo.vice_versa
					&& combo.item_first.get_id() == second_item->get_id()
					&& combo.item_second.get_id() == first_item->get_id()));
		});

		if (!combo_data) {
			combo_data = &inventory->add_component(new item::ComboData(*first_item, *second_item));
			combo_data->combine = combo_combine;
			combo_data->separate = combo_separate;
		}

		combo_data->item_final = final_item;
		combo_data->item_extra = extra_item;

		if (vice_versa >= 0) {
			combo_data->vice_versa = vice_versa == CINV_TRUE;
		}
		if (separable >= 0) {
			combo_data->separable = separable == CINV_TRUE;
		}
		if (combine_tgroup >= 0) {
			combo_data->combine = [=](item::ComboData &cd) -> void {
				PerformTriggerGroup(combine_tgroup);
			};
		}
		if (separate_tgroup >= 0) {
			combo_data->separate = [=](item::ComboData &cd) -> void {
				PerformTriggerGroup(separate_tgroup);
			};
		}
	}
}

void customize_camera(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 14) {
		return;
	}

	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryDisplay>();
	if (!inventory) {
		return;
	}
	auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

	int32_t cust_index = -1;

	const auto fov = customize.pVetArg[++cust_index];
	const auto pos_x_closed = customize.pVetArg[++cust_index];
	const auto pos_y_closed = customize.pVetArg[++cust_index];
	const auto pos_z_closed = customize.pVetArg[++cust_index];
	const auto tgt_x_closed = customize.pVetArg[++cust_index];
	const auto tgt_y_closed = customize.pVetArg[++cust_index];
	const auto tgt_z_closed = customize.pVetArg[++cust_index];
	const auto pos_x_opened = customize.pVetArg[++cust_index];
	const auto pos_y_opened = customize.pVetArg[++cust_index];
	const auto pos_z_opened = customize.pVetArg[++cust_index];
	const auto tgt_x_opened = customize.pVetArg[++cust_index];
	const auto tgt_y_opened = customize.pVetArg[++cust_index];
	const auto tgt_z_opened = customize.pVetArg[++cust_index];
	const auto ring_change_pitch = customize.pVetArg[++cust_index];

	if (fov >= 0) {
		inventory_display.camera_fov_opened = fov;
	}
	if (pos_x_closed != -1) {
		inventory_display.camera_pos_closed.x = pos_x_closed;
	}
	if (pos_y_closed != -1) {
		inventory_display.camera_pos_closed.y = pos_y_closed;
	}
	if (pos_z_closed != -1) {
		inventory_display.camera_pos_closed.z = pos_z_closed;
	}
	if (tgt_x_closed != -1) {
		inventory_display.camera_tgt_closed.x = tgt_x_closed;
	}
	if (tgt_y_closed != -1) {
		inventory_display.camera_tgt_closed.y = tgt_y_closed;
	}
	if (tgt_z_closed != -1) {
		inventory_display.camera_tgt_closed.z = tgt_z_closed;
	}
	if (pos_x_opened != -1) {
		inventory_display.camera_pos_opened.x = pos_x_opened;
	}
	if (pos_y_opened != -1) {
		inventory_display.camera_pos_opened.y = pos_y_opened;
	}
	if (pos_z_opened != -1) {
		inventory_display.camera_pos_opened.z = pos_z_opened;
	}
	if (tgt_x_opened != -1) {
		inventory_display.camera_tgt_opened.x = tgt_x_opened;
	}
	if (tgt_y_opened != -1) {
		inventory_display.camera_tgt_opened.y = tgt_y_opened;
	}
	if (tgt_z_opened != -1) {
		inventory_display.camera_tgt_opened.z = tgt_z_opened;
	}
	if (ring_change_pitch != -1) {
		inventory_display.camera_ring_change_pitch = ring_change_pitch;
	}
}

void customize_lighting(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 4) {
		return;
	}

	const auto entity = entity_manager.find_entity_with_component<render::LightingLocation>();
	if (!entity) {
		return;
	}
	auto &light_loc = *entity->get_component<render::LightingLocation>();

	int32_t cust_index = -1;

	const auto room_index = customize.pVetArg[++cust_index];
	const auto cord_x = customize.pVetArg[++cust_index];
	const auto cord_y = customize.pVetArg[++cust_index];
	const auto cord_z = customize.pVetArg[++cust_index];

	if (room_index != -1) {
		light_loc.room = room_index;
	}
	if (cord_x != -1) {
		light_loc.x = cord_x;
	}
	if (cord_y != -1) {
		light_loc.y = cord_y;
	}
	if (cord_z != -1) {
		light_loc.z = cord_z;
	}
}

void customize_cheats(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 9) {
		return;
	}

	int32_t cust_index = -1;

	for (int32_t i = 0; i < customize.NArguments; i += 9) {
		const auto item_id = customize.pVetArg[++cust_index];
		const auto key1_scancode = customize.pVetArg[++cust_index];
		const auto key2_scancode = customize.pVetArg[++cust_index];
		const auto key3_scancode = customize.pVetArg[++cust_index];
		const auto key4_scancode = customize.pVetArg[++cust_index];
		const auto key5_scancode = customize.pVetArg[++cust_index];
		const auto hint_type_id = customize.pVetArg[++cust_index];
		const auto enabled_cgroup = customize.pVetArg[++cust_index];
		const auto action_tgroup = customize.pVetArg[++cust_index];

		auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
			return item_data.item_id == item_id;
		});
		if (!item) {
			continue;
		}

		auto cheat_config = item->get_component<cheat::CheatConfig>([&](cheat::CheatConfig &config) -> bool {
			return config.key_1 == key1_scancode
				&& config.key_2 == key2_scancode
				&& config.key_3 == key3_scancode
				&& config.key_4 == key4_scancode
				&& config.key_5 == key5_scancode
				&& config.key_6 == 0
				&& config.key_7 == 0
				&& config.key_8 == 0
				&& config.key_9 == 0
				&& config.key_10 == 0;
		});
		if (!cheat_config) {
			cheat_config = &item->add_component(new cheat::CheatConfig());
			cheat_config->enabled = cheat::facing_north;
		}

		if (key1_scancode > 0) {
			cheat_config->key_1 = key1_scancode;
		}
		if (key2_scancode > 0) {
			cheat_config->key_2 = key2_scancode;
		}
		if (key3_scancode > 0) {
			cheat_config->key_3 = key3_scancode;
		}
		if (key4_scancode > 0) {
			cheat_config->key_4 = key4_scancode;
		}
		if (key5_scancode > 0) {
			cheat_config->key_5 = key5_scancode;
		}
		if (hint_type_id >= 0) {
			cheat_config->hint_type = static_cast<cheat::CheatHintType::Enum>(hint_type_id);
		}
		if (enabled_cgroup > 0) {
			cheat_config->enabled = [=]() -> bool {
				return PerformTriggerGroup(enabled_cgroup) > 0;
			};
		}
		if (action_tgroup > 0) {
			cheat_config->action = [=]() -> void {
				PerformTriggerGroup(action_tgroup);
			};
		}
	}
}

void customize_debug(
	const StrGenericCustomize &customize,
	ecs::EntityManager &entity_manager
)
{
	if (customize.NArguments < 1) {
		return;
	}

	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryDebug>();
	if (!inventory) {
		return;
	}
	auto &inventory_debug = *inventory->get_component<inventory::InventoryDebug>();

	int32_t cust_index = -1;

	const auto enabled = customize.pVetArg[++cust_index];

	if (enabled >= 0) {
		inventory_debug.enabled = enabled == CINV_TRUE;
	}
}

void customize_inventory(ecs::EntityManager &entity_manager)
{
	for (int32_t ring_id = ring::MIN_INVENTORY_RING_ID; ring_id <= ring::MAX_INVENTORY_RING_ID; ++ring_id) {
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_RING, ring_id)) {
			customize_ring(*GET.pCust, entity_manager);
		}
	}

	for (int32_t item_id = item::MIN_INVENTORY_ITEM_ID; item_id <= item::MAX_INVENTORY_ITEM_ID; ++item_id) {
		if (item_id == item::ItemId::NONE) {
			continue;
		}

		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM, item_id)) {
			customize_item(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_RING, item_id)) {
			customize_item_ring(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_MODEL, item_id)) {
			customize_item_model(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_DISPLAY, item_id)) {
			customize_item_display(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_QUANTITY, item_id)) {
			customize_item_quantity(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_ANIMATION, item_id)) {
			customize_item_animation(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_ACTION, item_id)) {
			customize_item_action(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_ITEM_SFX, item_id)) {
			customize_item_sfx(*GET.pCust, entity_manager);
		}

		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_AMMO, item_id)) {
			customize_ammo(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_EXAMINE, item_id)) {
			customize_examine(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_HEALTH, item_id)) {
			customize_health(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_COMPASS, item_id)) {
			customize_compass(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_STOPWATCH, item_id)) {
			customize_stopwatch(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_PASSPORT, item_id)) {
			customize_passport(*GET.pCust, entity_manager);
		}
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_MAP, item_id)) {
			customize_map(*GET.pCust, entity_manager);
		}
	}

	for (int32_t type = 1; type < inventory::DurationType::NONE; ++type) {
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_DURATION, type)) {
			customize_duration(*GET.pCust, entity_manager);
		}
	}
	for (int32_t type = 1; type < sound::SfxType::NONE; ++type) {
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_SFX, type)) {
			customize_sfx(*GET.pCust, entity_manager);
		}
	}
	for (int32_t type = 1; type < text::TextType::NONE; ++type) {
		if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_TEXT, type)) {
			customize_text(*GET.pCust, entity_manager);
		}
	}

	if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_COMBO, -1)) {
		customize_combo(*GET.pCust, entity_manager);
	}
	if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_DISPLAY, -1)) {
		customize_display(*GET.pCust, entity_manager);
	}
	if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_CAMERA, -1)) {
		customize_camera(*GET.pCust, entity_manager);
	}
	if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_LIGHTING, -1)) {
		customize_lighting(*GET.pCust, entity_manager);
	}
	if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_CHEATS, -1)) {
		customize_cheats(*GET.pCust, entity_manager);
	}
	if (Get(enumGET.MY_CUSTOMIZE_COMMAND, CUST_CINV_DEBUG, -1)) {
		customize_debug(*GET.pCust, entity_manager);
	}
}

void setup_inventory(ecs::EntityManager &entity_manager)
{
	auto &inventory = entity_manager.new_entity();

	inventory.add_component(new inventory::InventoryData());

	auto &inventory_display = inventory.add_component(new inventory::InventoryDisplay());
	inventory_display.ring_radius_closed = 0;
	inventory_display.ring_radius_opened = 800;
	inventory_display.ring_orient_closed = core::Vector3D(0, 0, 0);
	inventory_display.ring_orient_opened = core::Vector3D(0, 180, 0);
	inventory_display.camera_pos_closed = core::Vector3D(0, -2000, 800);
	inventory_display.camera_pos_opened = core::Vector3D(0, -210, 700);
	inventory_display.camera_tgt_closed = core::Vector3D(0, -70, 0);
	inventory_display.camera_tgt_opened = core::Vector3D(0, -70, 0);
	inventory_display.camera_ring_change_pitch = 40;
	inventory_display.camera_fov_closed = 80;
	inventory_display.camera_fov_opened = 80;
	inventory_display.item_base_size = 16384;

	auto &inventory_duration = inventory.add_component(new inventory::InventoryDuration());
	inventory_duration.inventory_open_frames = 18;
	inventory_duration.ring_change_frames = 14;
	inventory_duration.ring_rotate_frames = 10;
	inventory_duration.item_select_frames = 10;
	inventory_duration.item_spin_frames = 120;

	auto &inventory_sfx = inventory.add_component(new inventory::InventorySfx());
	inventory_sfx.opening_sound_id = 109;
	inventory_sfx.closing_sound_id = 111;
	inventory_sfx.ring_rotate_sound_id = 108;
	inventory_sfx.ring_change_sound_id = 109;
	inventory_sfx.item_select_sound_id = 109;
	inventory_sfx.item_cancel_sound_id = 111;
	inventory_sfx.menu_change_sound_id = 108;
	inventory_sfx.ammo_load_sound_id = 9;
	inventory_sfx.combine_sound_id = 114;
	inventory_sfx.separate_sound_id = 124;
	inventory_sfx.fail_sound_id = 2;

	inventory.add_component(new inventory::InventoryState());
	inventory.add_component(new inventory::InventoryDebug());
	inventory.add_component(new special::GameTime());

	setup_text(inventory);
	setup_lighting(inventory);
}

void setup(ecs::EntityManager &entity_manager)
{
	setup_inventory(entity_manager);
	setup_camera(entity_manager);
	setup_rings(entity_manager);
	setup_items(entity_manager);
	setup_ammo(entity_manager);
	setup_combos(entity_manager);
	setup_ammo_and_combo_actions(entity_manager);
	setup_cheats(entity_manager);

	customize_inventory(entity_manager);
}

}
}