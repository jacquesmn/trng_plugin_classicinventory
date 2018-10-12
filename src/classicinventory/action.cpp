#include "StdAfx.h"

#include "action.h"

#include <trng_core.h>
#include "item.h"

extern TYPE_ShowSaveScreen ShowSaveScreen;
extern TYPE_SoundEffect SoundEffect;
extern TYPE_SetFade SetFade;
extern StrMyData MyData;

namespace classicinventory {
namespace action {

void ActionSystem::init(ecs::EntityManager & entity_manager, ecs::SystemManager &system_manager)
{}

void ActionSystem::update(ecs::EntityManager & entity_manager, ecs::SystemManager &system_manager)
{}

void ActionSystem::cleanup(ecs::EntityManager & entity_manager, ecs::SystemManager &system_manager)
{}

bool use_health(ecs::Entity & item)
{
	if (!item.has_component<item::HealthData>()) {
		return false;
	}
	auto &health_data = *item.get_component<item::HealthData>();

	if (!item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	auto &lara = *Trng.pGlobTomb4->pAdr->pLara;
	auto &lara_health = lara.Health;

	if (lara_health <= 0) {
		// lara ded
		return false;
	}

	if ((lara_health >= 1000 && *Trng.pGlobTomb4->pAdr->pPoison1 == 0)
		|| (!item_quantity.decrement() && item_quantity.get_quantity() != item::ITEM_QTY_UNLIMITED)
		) {
		// lara says no
		SoundEffect(2, nullptr, 0);
		return false;
	}

	const auto lara_health_before = lara_health;
	lara_health = min(1000, lara_health + health_data.health_points);
	lara_health = max(0, lara_health);

	if (health_data.poison_points > 0) {
		*Trng.pGlobTomb4->pAdr->pPoison1 = health_data.poison_points;
		*Trng.pGlobTomb4->pAdr->pPoison2 = health_data.poison_points;
	}

	if (health_data.cure_poison) {
		*Trng.pGlobTomb4->pAdr->pPoison1 = 0;
		*Trng.pGlobTomb4->pAdr->pPoison2 = 0;
	}

	if (lara_health > lara_health_before) {
		// heal sfx
		if (health_data.heal_sound_id >= 0) {
			SoundEffect(health_data.heal_sound_id, nullptr, 0);
		}
	}
	else if (lara_health < lara_health_before) {
		// hurt sfx
		if (health_data.hurt_sound_id >= 0) {
			SoundEffect(health_data.hurt_sound_id, nullptr, 0);
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

	if (!lara_info.TestIsHoldingWeapon && hand_flags != 0) {
		// lara is busy doing something important
		SoundEffect(2, nullptr, 0);
		return false;
	}

	if (!item_quantity.decrement() && item_quantity.get_quantity() != item::ITEM_QTY_UNLIMITED) {
		return false;
	}

	in_hands_next = enumHOLD.FLARE;

	if (!lara_info.TestIsHoldingWeapon) {
		in_hands_now = enumHOLD.FLARE;
		hand_flags = enumFLH.IS_EXTRACTING;
	}

	return true;
}

bool use_binoculars(ecs::Entity & item)
{
	if (!item.has_component<item::ItemQuantity>()) {
		return false;
	}
	auto &item_quantity = *item.get_component<item::ItemQuantity>();

	if (item_quantity.get_quantity() == 0) {
		return false;
	}

	Get(enumGET.LARA, 0, 0);
	auto &lara = *GET.pLara;

	Get(enumGET.INFO_LARA, 0, 0);
	auto &lara_info = GET.LaraInfo;

	auto &in_hands_now = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNow;
	auto &hand_flags = *Trng.pGlobTomb4->pAdr->pFlagsLaraHands;

	if (lara.StateIdCurrent != 2
		|| hand_flags == enumFLH.IS_EXTRACTING) {
		// lara is busy doing something important
		SoundEffect(2, nullptr, 0);
		return false;
	}

	if (lara_info.TestIsHoldingWeapon
		|| in_hands_now == enumHOLD.FLARE
		|| in_hands_now == enumHOLD.ANY_TORCH) {
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

	if (item_quantity.get_quantity() == 0) {
		return false;
	}

	if (Trng.pGlobTomb4->TestTakeAwayWeapons) {
		return false;
	}

	Get(enumGET.INFO_LARA, 0, 0);
	auto &lara_info = GET.LaraInfo;

	auto &in_hands_next = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNext;
	auto &in_hands_now = *Trng.pGlobTomb4->pAdr->pObjInLaraHandsNow;
	auto &hand_flags = *Trng.pGlobTomb4->pAdr->pFlagsLaraHands;

	if (!lara_info.TestIsHoldingWeapon
		&& in_hands_now != enumHOLD.FLARE
		&& in_hands_now != enumHOLD.ANY_TORCH
		&& hand_flags != 0) {
		// lara is busy doing something important
		SoundEffect(2, nullptr, 0);
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
		if (lara_info.TestIsHoldingWeapon) {
			// already holding selected weapon
			return false;
		}

		// extract current weapon
		hand_flags = enumFLH.IS_EXTRACTING;

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

	if (item_quantity.get_quantity() == 0) {
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

	MyData.Save.Local.inventory_item_id_selected = exchanged_waterskin_item_id;

	return true;
}

bool load_game()
{
	const int save_number = ShowSaveScreen(0x800000, true);

	if (save_number >= 0) {
		// loaded game
		*Trng.pGlobTomb4->pAdr->pTestLoadOrNewLevel = true;

		return true;
	}

	return false;
}

bool save_game()
{
	const int save_number = ShowSaveScreen(0x400000, true);

	if (save_number >= 0) {
		// saved game
		return true;
	}

	return false;
}

bool exit_to_title()
{
	*Trng.pGlobTomb4->pAdr->pTestLoadOrNewLevel = true;

	return true;
}

}
}
