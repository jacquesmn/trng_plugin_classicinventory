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

#include "StdAfx.h"

#include "action.h"

#include <trng_core.h>
#include "item.h"
#include "special.h"

extern TYPE_ShowSaveScreen ShowSaveScreen;
extern TYPE_SoundEffect SoundEffect;
extern TYPE_SayNo SayNo;
extern TYPE_SetFade SetFade;
extern StrMyData MyData;

namespace classicinventory {
namespace action {

bool use_health(ecs::Entity &item, bool silent)
{
	if (!item.has_component<special::HealthData>()) {
		return false;
	}
	auto &health_data = *item.get_component<special::HealthData>();

	if (!item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	auto &lara = *Trng.pGlobTomb4->pAdr->pLara;
	auto &lara_health = lara.Health;
	auto &lara_air = *Trng.pGlobTomb4->pAdr->pAirAvailable;
	auto &lara_poison1 = *Trng.pGlobTomb4->pAdr->pPoison1;
	auto &lara_poison2 = *Trng.pGlobTomb4->pAdr->pPoison2;

	if (lara_health <= 0) {
		// lara ded
		return false;
	}

	const auto can_use = (health_data.health_points > 0 && lara_health < 1000)
		|| (health_data.air_points > 0 && lara_air < 1800)
		|| (health_data.cure_poison && (lara_poison1 > 0 || lara_poison2 > 0))
		|| health_data.health_points < 0
		|| health_data.air_points < 0
		|| health_data.poison_points > 0;

	if (!can_use
		|| (!item_quantity.decrease() && item_quantity.get() != item::ITEM_QTY_UNLIMITED)) {
		if (!silent) {
			// lara says no
			SayNo();
		}
		return false;
	}

	const auto lara_health_before = lara_health;
	const auto lara_air_before = lara_air;
	const auto poison1_before = lara_poison1;
	const auto poison2_before = lara_poison2;

	lara_health = min(1000, lara_health + health_data.health_points);
	lara_health = max(0, lara_health);

	lara_air = min(1800, lara_air + health_data.air_points);
	lara_air = max(0, lara_air);

	if (health_data.cure_poison) {
		lara_poison1 = 0;
		lara_poison2 = 0;
	}
	else {
		lara_poison1 = min(4097, lara_poison1 + health_data.poison_points);;
		lara_poison2 = min(4097, lara_poison2 + health_data.poison_points);;
	}

	if (lara_health < lara_health_before
		|| lara_air < lara_air_before
		|| lara_poison1 > poison1_before
		|| lara_poison2 > poison2_before) {
		// hurt sfx
		if (health_data.hurt_sound_id >= 0) {
			SoundEffect(health_data.hurt_sound_id, nullptr, 2);
		}
	}
	else if (lara_health > lara_health_before
		|| lara_air > lara_air_before
		|| lara_poison1 < poison1_before
		|| lara_poison2 < poison2_before) {
		// heal sfx
		if (health_data.heal_sound_id >= 0) {
			SoundEffect(health_data.heal_sound_id, nullptr, 2);
		}
	}

	if (health_data.increase_usage_stats) {
		// increase number of health items used
		(*reinterpret_cast<uint32_t*>(0x7F7745))++;
	}

	return true;
}

bool use_flare(ecs::Entity &item)
{
	if (!item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	Get(enumGET.INFO_LARA, 0, 0);
	auto &lara_info = GET.LaraInfo;

	auto &in_hands_next = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNext;
	auto &in_hands_now = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNow;
	auto &hand_flags = *Trng.pGlobTomb4->pAdr->pFlagsLaraHands;

	if (in_hands_now == enumHOLD.FLARE) {
		// already holding flare
		return false;
	}

	if (hand_flags != 0) {
		// lara is busy doing something important
		SayNo();
		return false;
	}

	if (!item_quantity.decrease() && item_quantity.get() != item::ITEM_QTY_UNLIMITED) {
		return false;
	}

	in_hands_next = enumHOLD.FLARE;

	return true;
}

bool use_binoculars(ecs::Entity &item)
{
	if (!item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	if (item_quantity.get() == 0) {
		return false;
	}

	Get(enumGET.LARA, 0, 0);
	auto &lara = *GET.pLara;

	Get(enumGET.INFO_LARA, 0, 0);
	auto &lara_info = GET.LaraInfo;

	auto &in_hands_now = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNow;
	auto &hand_flags = *Trng.pGlobTomb4->pAdr->pFlagsLaraHands;

	if (lara.StateIdCurrent != 2) {
		// lara is busy doing something important
		SayNo();
		return false;
	}

	if (lara_info.TestIsHoldingWeapon
		|| hand_flags == enumFLH.IS_EXTRACTING) {
		hand_flags = enumFLH.IS_THROWING;
	}

	*Trng.pGlobTomb4->pAdr->pZoomFactor = 0x80;
	core::set_bit(*Trng.pGlobTomb4->pAdr->pFlagsLara2, enumFL2.IS_USING_LASER_SIGHT_OR_BINOCULARS);

	return true;
}

bool equip_weapon(ecs::Entity &item)
{
	if (!item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	if (!item.has_component<item::ItemData>()) {
		return false;
	}
	auto &item_data = *item.get_component<item::ItemData>();

	if (item_quantity.get() == 0) {
		return false;
	}

	Get(enumGET.INFO_LARA, 0, 0);
	auto &lara_info = GET.LaraInfo;

	const auto weapons_disabled = core::bit_set(Trng.pGlobTomb4->StatusNG, SNG_DISABLE_WEAPONS);
	auto &in_hands_next = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNext;
	auto &in_hands_now = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNow;
	auto &hand_flags = *Trng.pGlobTomb4->pAdr->pFlagsLaraHands;

	if (weapons_disabled
		|| (!lara_info.TestIsHoldingWeapon
			&& in_hands_now != enumHOLD.FLARE
			&& in_hands_now != enumHOLD.ANY_TORCH
			&& in_hands_now != enumHOLD.OUT_TORCH
			&& in_hands_now != enumHOLD.FIRED_TORCH
			&& hand_flags != enumFLH.IS_EXTRACTING
			&& hand_flags != enumFLH.IS_THROWING
			&& hand_flags != 0)) {
		// lara can't equip weapon now
		SayNo();
		return false;
	}

	int32_t hold_flag = 0;
	switch (item_data.item_id) {
	case item::ItemId::PISTOLS:
		hold_flag = enumHOLD.PISTOLS;
		break;
	case item::ItemId::SHOTGUN:
		hold_flag = enumHOLD.SHOTGUN;
		break;
	case item::ItemId::UZI:
		hold_flag = enumHOLD.UZI;
		break;
	case item::ItemId::REVOLVER:
	case item::ItemId::REVOLVER_LASERSIGHT_COMBO:
		hold_flag = enumHOLD.REVOLVER;
		break;
	case item::ItemId::CROSSBOW:
	case item::ItemId::CROSSBOW_LASERSIGHT_COMBO:
		hold_flag = enumHOLD.CROSSBOW;
		break;
	case item::ItemId::GRENADE_GUN:
		hold_flag = enumHOLD.GRENADEGUN;
		break;
	default:
		return false;
	}

	if (in_hands_now == hold_flag) {
		if (lara_info.TestIsHoldingWeapon || hand_flags == enumFLH.IS_EXTRACTING) {
			// already holding/extracting selected weapon
			return false;
		}

		// extract current weapon
		if (hand_flags == enumFLH.IS_THROWING) {
			in_hands_next = hold_flag;
		}
		else {
			hand_flags = enumFLH.IS_EXTRACTING;
		}

		return true;
	}

	// switch to weapon
	in_hands_next = hold_flag;

	return true;
}

bool exchange_waterskins(ecs::Entity &item)
{
	if (!item.has_component<item::ItemData>()
		|| !item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_data = *item.get_component<item::ItemData>();
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	if (item_quantity.get() == 0) {
		return false;
	}

	Get(enumGET.LARA, 0, 0);
	auto &lara = *GET.pLara;
	const bool &lara_in_water = *reinterpret_cast<bool*>(0x80EBB0);

	if (lara.StateIdCurrent != 2) {
		// lara is busy doing something important
		return false;
	}

	// set corresponding empty or filled waterskin as selected
	item::ItemId::Enum exchanged_waterskin_item_id = item::ItemId::NONE;
	switch (item_data.item_id) {
	case item::ItemId::WATERSKIN1_EMPTY:
	case item::ItemId::WATERSKIN1_1:
	case item::ItemId::WATERSKIN1_2:
		exchanged_waterskin_item_id = lara_in_water ? item::ItemId::WATERSKIN1_3 : item::ItemId::WATERSKIN1_EMPTY;
		break;
	case item::ItemId::WATERSKIN1_3:
		exchanged_waterskin_item_id = item::ItemId::WATERSKIN1_EMPTY;
		break;
	case item::ItemId::WATERSKIN2_EMPTY:
	case item::ItemId::WATERSKIN2_1:
	case item::ItemId::WATERSKIN2_2:
	case item::ItemId::WATERSKIN2_3:
	case item::ItemId::WATERSKIN2_4:
		exchanged_waterskin_item_id = lara_in_water ? item::ItemId::WATERSKIN2_5 : item::ItemId::WATERSKIN2_EMPTY;
		break;
	case item::ItemId::WATERSKIN2_5:
		exchanged_waterskin_item_id = item::ItemId::WATERSKIN2_EMPTY;
		break;
	default:
		return false;
	}

	MyData.Save.Local.inventory_data.item_id_selected = exchanged_waterskin_item_id;

	return true;
}

bool load_game()
{
	const int save_number = ShowSaveScreen(0x800000, true);

	if (save_number >= 0) {
		// loaded game
		*Trng.pGlobTomb4->pAdr->pTestLoadOrNewLevel = 1;

		return true;
	}

	return false;
}

bool save_game()
{
	const int save_number = ShowSaveScreen(0x400000, true);

	if (save_number >= 0) {
		// saved game

		// Clear internal trng variable to fix issue with savegame being loaded after exiting inventory.
		// Only happens with CUST_KEEP_DEAD_ENEMIES enabled, so perhaps memory zone is corrupted?
		*reinterpret_cast<int32_t*>(0x10657B68) = 0;

		return true;
	}

	return false;
}

bool exit_to_title()
{
	int32_t &Savegame_1E7_NumeroLivello = *reinterpret_cast<int32_t*>(0x7F7730);

	Savegame_1E7_NumeroLivello = 0;

	*Trng.pGlobTomb4->pAdr->pTestLoadOrNewLevel = 1;

	return true;
}

void pickup_item(ecs::Entity &item, ecs::EntityManager &entity_manager)
{
	auto update_qty = [](item::ItemQuantity &item_qty, int32_t qty) {
		if (qty >= 0) {
			item_qty.increase(qty);
		}
		else {
			item_qty.unlimited();
		}
	};

	if (item.has_component<item::ItemData>()
		&& item.has_component<item::ItemQuantity>()) {
		const auto item_data = item.get_component<item::ItemData>();
		const auto item_qty = item.get_component<item::ItemQuantity>();

		update_qty(*item_qty, item_qty->pickup);

		// if weapon, add ammo and load if unloaded
		if (item.has_component<item::ItemAmmo>()) {
			auto item_ammos = item.get_components<item::ItemAmmo>();
			for (auto ammo_it = item_ammos.begin(); ammo_it != item_ammos.end(); ++ammo_it) {
				auto &item_ammo = **ammo_it;
				auto &ammo_item = item_ammo.ammo_item;

				if (item_ammo.qty_with_weapon == 0) {
					continue;
				}

				if (ammo_item.has_component<item::ItemData>()
					&& ammo_item.has_component<item::ItemQuantity>()) {
					const auto ammo_data = ammo_item.get_component<item::ItemData>();
					const auto ammo_qty = ammo_item.get_component<item::ItemQuantity>();

					update_qty(*ammo_qty, item_ammo.qty_with_weapon);

					if (!item::get_loaded_ammo(item)) {
						item::load_ammo(item, ammo_data->item_id);
					}
				}
			}
		}

		// if ammo, load weapon if unloaded
		const auto weapons = entity_manager.find_entities_with_component<item::ItemAmmo>([&](const item::ItemAmmo &item_ammo) -> bool {
			auto &ammo_item = item_ammo.ammo_item;

			if (ammo_item.has_component<item::ItemData>()) {
				const auto ammo_data = ammo_item.get_component<item::ItemData>();

				if (ammo_data->item_id == item_data->item_id) {
					return true;
				}
			}

			return false;
		});
		for (auto weapon_it = weapons.begin(); weapon_it != weapons.end(); ++weapon_it) {
			auto& weapon = **weapon_it;

			if (!item::get_loaded_ammo(weapon)) {
				item::load_ammo(weapon, item_data->item_id);
			}
		}
	}
}

}
}
