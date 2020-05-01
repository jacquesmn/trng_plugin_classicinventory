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

#include "special.h"

#include <trng_core.h>
#include "action.h"
#include "input.h"
#include "inventory.h"
#include "render.h"

extern TYPE_InitialisePickUpDisplay InitialisePickUpDisplay;

namespace classicinventory {
namespace special {

void TimeSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	auto &game_frames = *reinterpret_cast<uint32_t*>(0x7FD138);

	// extract time parts
	auto milliseconds = game_frames / 0.03f;
	const auto hours = milliseconds / 3600000.f;

	milliseconds = float(int32_t(milliseconds) % 3600000);
	const auto minutes = milliseconds / 60000.f;

	milliseconds = float(int32_t(milliseconds) % 60000);
	const auto seconds = milliseconds / 1000.f;

	milliseconds = float(int32_t(milliseconds) % 1000);

	auto times = entity_manager.find_entities_with_component<GameTime>();
	for (auto time_it = times.begin(); time_it != times.end(); ++time_it) {
		auto &time = **time_it;
		auto &game_time = *time.get_component<GameTime>();

		game_time.frames = game_frames;
		game_time.hours = hours;
		game_time.minutes = minutes;
		game_time.seconds = seconds;
		game_time.milliseconds = milliseconds;
	}

	game_frames++;
}

CompassSystem::CompassSystem()
	:
	first_update(true)
{}

void CompassSystem::init(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	first_update = true;

	auto compasses = entity_manager.find_entities_with_component<CompassData>();
	for (auto compass_it = compasses.begin(); compass_it != compasses.end(); ++compass_it) {
		auto &compass = **compass_it;
		auto &compass_data = *compass.get_component<CompassData>();

		auto &pointers = compass_data.pointers;
		for (auto pointer_it = pointers.begin(); pointer_it != pointers.end(); ++pointer_it) {
			auto &pointer = *pointer_it;

			// swing pointer
			pointer.acceleration = 0.f;
			pointer.velocity = 0.f;
			pointer.oscill_angle = core::random(45.f, 179.f) * core::random_sign();
		}
	}
}

void CompassSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	const auto time = entity_manager.find_entity_with_component<GameTime>();
	if (!time) {
		return;
	}
	auto &game_time = *time->get_component<GameTime>();

	auto compasses = entity_manager.find_entities_with_component<CompassData>();
	for (auto compass_it = compasses.begin(); compass_it != compasses.end(); ++compass_it) {
		auto &compass = **compass_it;
		auto &compass_data = *compass.get_component<CompassData>();

		auto &pointers = compass_data.pointers;
		for (auto pointer_it = pointers.begin(); pointer_it != pointers.end(); ++pointer_it) {
			auto &pointer = *pointer_it;

			if (first_update || game_time.frames % pointer.frequency_frames == 0) {
				pointer.bearing = pointer.get_bearing() - pointer.offset;
			}

			// settle pointer
			if (pointer.friction == 1) {
				pointer.oscill_angle = 0;
			}
			else {
				pointer.acceleration = -pointer.attraction * sin(core::degrees_to_radians(pointer.oscill_angle));
				pointer.velocity += pointer.acceleration;
				pointer.oscill_angle += pointer.velocity;
				pointer.velocity *= (1 - pointer.friction);
			}

			pointer.angle = pointer.bearing + pointer.oscill_angle;
		}
	}

	first_update = false;
}

StopwatchSystem::StopwatchSystem()
	:
	first_update(true)
{}

void StopwatchSystem::init(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	first_update = true;
}

void StopwatchSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	const auto time = entity_manager.find_entity_with_component<GameTime>();
	if (!time) {
		return;
	}
	auto &game_time = *time->get_component<GameTime>();

	// update stopwatch hands
	auto stopwatches = entity_manager.find_entities_with_component<StopwatchData>();
	for (auto stopwatch_it = stopwatches.begin(); stopwatch_it != stopwatches.end(); ++stopwatch_it) {
		auto &stopwatch = **stopwatch_it;
		auto &stopwatch_data = *stopwatch.get_component<StopwatchData>();

		if (!first_update && game_time.frames % stopwatch_data.frequency_frames != 0) {
			continue;
		}

		stopwatch_data.hour_hand_angle = (game_time.hours / 12.f) * 360.f;
		stopwatch_data.minute_hand_angle = (game_time.minutes / 60.f) * 360.f;
		stopwatch_data.second_hand_angle = (game_time.seconds / 60.f) * 360.f;
	}

	first_update = false;
}

void PickupSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();

	// replace original tr4 pickup display
	auto clear_tr4_pickup_buffer = false;
	const int32_t tr4_pickup_buffer_offset = 0x7FD100;

	for (uint8_t i = 0; i < 32; i += 4) {
		if (*reinterpret_cast<uint16_t*>(tr4_pickup_buffer_offset + i) == UINT16_MAX) {
			break;
		}
		clear_tr4_pickup_buffer = true;

		// skip if any undesired phase is active
		// ^ we still want to always clear the tr4 buffer though
		const auto undesired_phases = enumSKIP.LOADING_LEVEL
			| enumSKIP.TITLE_LEVEL
			| enumSKIP.GRAY_SCREEN
			| enumSKIP.NO_VIEW_OGGETTI
			| enumSKIP.BINOCULARS
			| enumSKIP.LASER_SIGHT
			| enumSKIP.FULL_IMAGE;

		if (core::bit_set(GET.LaraInfo.SkipPhaseFlags, undesired_phases, true)) {
			break;
		}

		if (!inventory) {
			break;
		}

		const auto slot_id = *reinterpret_cast<uint16_t*>(tr4_pickup_buffer_offset + i + 2);
		const auto item_id = item::tr4_slot_to_item_id(slot_id);

		if (!entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
			return item_data.item_id == item_id;
		})) {
			continue;
		}

		auto &pickup_display = inventory->add_component(new render::PickupDisplay(item_id));
		pickup_display.alpha = 0;
	}

	if (clear_tr4_pickup_buffer) {
		InitialisePickUpDisplay();
	}
}

ShortcutSystem::ShortcutSystem(input::InputState &input_state)
	:
	input_state(input_state)
{}

void ShortcutSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	// skip if any undesired phase is active
	const auto undesired_phases = enumSKIP.LOADING_LEVEL
		| enumSKIP.FADE
		| enumSKIP.TITLE_LEVEL
		| enumSKIP.GRAY_SCREEN
		| enumSKIP.NO_VIEW_OGGETTI
		| enumSKIP.BINOCULARS
		| enumSKIP.LASER_SIGHT
		| enumSKIP.FULL_IMAGE
		| enumSKIP.FLY_CAMERA;

	Get(enumGET.INFO_LARA, 0, 0);
	if (core::bit_set(GET.LaraInfo.SkipPhaseFlags, undesired_phases, true)) {
		return;
	}

	// replace shortcut functions:
	// medipacks
	int32_t medi_item_id = item::ItemId::NONE;

	if (input_state.key_first_press(10)) {
		medi_item_id = item::ItemId::BIGMEDI;
	}
	else if (input_state.key_first_press(11)) {
		medi_item_id = item::ItemId::SMALLMEDI;
	}

	if (medi_item_id != item::ItemId::NONE) {
		const auto item = entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
			return item_data.item_id == medi_item_id;
		});

		if (item) {
			action::use_health(*item, true);
		}
	}

	// flares
	if (input_state.command_first_press(enumCMD.USE_FLARE)) {
		const auto item = entity_manager.find_entity_with_component<item::ItemData>([=](const item::ItemData &item_data) -> bool {
			return item_data.item_id == item::ItemId::FLARE_INV;
		});
		action::use_flare(*item, true, true);
	}

	// savegame
	if (input_state.key_first_press(63)) {
		action::save_game(true);
	}
}

StatisticsSystem::StatisticsSystem(
	StrStatistics& statistics_local,
	StrStatistics& statistics_global
)
	:
	statistics_local(statistics_local),
	statistics_global(statistics_global),
	health_before(0)
{}

void StatisticsSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	// skip if any undesired phase is active
	const auto undesired_phases = enumSKIP.LOADING_LEVEL
		| enumSKIP.TITLE_LEVEL;

	Get(enumGET.INFO_LARA, 0, 0);
	if (core::bit_set(GET.LaraInfo.SkipPhaseFlags, undesired_phases, true)) {
		return;
	}
	
	// Time Taken
	const auto game_frames = *reinterpret_cast<uint32_t*>(0x7FD138);
	const auto game_seconds = game_frames / 30;
	const auto game_seconds_diff = game_seconds - statistics_global.time_taken_seconds;

	statistics_global.time_taken_seconds += game_seconds_diff;
	statistics_local.time_taken_seconds += game_seconds_diff;

	// Distance Travelled
	const auto distance_travelled_meters = *reinterpret_cast<uint32_t*>(0x7F7736) / 419;
	const auto distance_travelled_meters_diff = distance_travelled_meters - statistics_global.distance_travelled_meters;

	statistics_global.distance_travelled_meters += distance_travelled_meters_diff;
	statistics_local.distance_travelled_meters += distance_travelled_meters_diff;

	// Ammo Used
	const auto ammo_used = int(*reinterpret_cast<uint32_t*>(0x7F773A));
	const auto ammo_used_diff = ammo_used - statistics_global.ammo_used;

	statistics_global.ammo_used += ammo_used_diff;
	statistics_local.ammo_used += ammo_used_diff;

	// Hits
	const auto hits = int(*reinterpret_cast<uint32_t*>(0x7F773E));
	const auto hits_diff = hits - statistics_global.hits;

	statistics_global.hits += hits_diff;
	statistics_local.hits += hits_diff;

	// Kills
	auto &kills = *reinterpret_cast<uint32_t*>(0x7F7756);

	// keep kills between levels
	if (!kills && statistics_global.kills) {
		kills = statistics_global.kills;
	}
	
	const auto kills_diff = kills - statistics_global.kills;

	statistics_global.kills += kills_diff;
	statistics_local.kills += kills_diff;

	// Health Packs Used
	const auto health_packs_used = int(*reinterpret_cast<uint8_t*>(0x7F7745));
	const auto health_packs_used_diff = health_packs_used - statistics_global.health_packs_used;

	statistics_global.health_packs_used += health_packs_used_diff;
	statistics_local.health_packs_used += health_packs_used_diff;

	// Health Lost
	const auto health = max(0, Trng.pGlobTomb4->pAdr->pLara->Health);
	const auto health_diff = health - health_before;
	health_before = health;

	if (health_diff < 0) {
		statistics_global.health_lost += abs(health_diff);
		statistics_local.health_lost += abs(health_diff);
	}

	// Secrets Found
	const auto secrets_found = int(*reinterpret_cast<uint8_t*>(0x7F7744));
	const auto secrets_found_diff = secrets_found - statistics_global.secrets_found;

	statistics_global.secrets_found += secrets_found_diff;
	statistics_local.secrets_found += secrets_found_diff;
}

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
float get_lara_bearing(bool realistic_north)
{
	return core::tr4_angle_to_degrees((realistic_north ? -1 : 1) * Trng.pGlobTomb4->pAdr->pLara->OrientationH);
}

float get_lara_item_bearing(int32_t ngle_index, bool realistic_north, uint32_t jitter)
{
	uint32_t item_x = 0;
	uint32_t item_z = 0;

	if (Get(enumGET.ITEM, ngle_index | NGLE_INDEX, 0)) {
		const auto item_movable = GET.pItem;
		item_x = item_movable->CordX;
		item_z = item_movable->CordZ;

		// TODO: check if active/visible
	}
	else if (Get(enumGET.STATIC, ngle_index | NGLE_INDEX, 0)) {
		const auto item_static = GET.pStatic;
		item_x = item_static->x;
		item_z = item_static->z;
	}
	else {
		return get_lara_bearing(realistic_north);
	}

	if (item_x == 0 && item_z == 0) {
		return 0;
	}

	Get(enumGET.LARA, 0, 0);
	auto &lara = *GET.pLara;

	uint32_t lara_x = lara.CordX;
	uint32_t lara_z = lara.CordZ;

	if (jitter > 0) {
		const auto lara_x_jittered = int(lara_x) + int(core::random(-float(jitter), float(jitter)));
		const auto lara_z_jittered = int(lara_z) + int(core::random(-float(jitter), float(jitter)));

		lara_x = lara_x_jittered < 0 ? lara_x : uint32_t(lara_x_jittered);
		lara_z = lara_z_jittered < 0 ? lara_z : uint32_t(lara_z_jittered);
	}

	return core::tr4_angle_to_degrees(GetDirection(lara_x, lara_z, item_x, item_z))
		+ core::tr4_angle_to_degrees(-lara.OrientationH);
}

}
}
