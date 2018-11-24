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
#include "motion.h"
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

void CompassSystem::init(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	auto compasses = entity_manager.find_entities_with_component<CompassData>();
	for (auto compass_it = compasses.begin(); compass_it != compasses.end(); ++compass_it) {
		auto &compass = **compass_it;
		auto &compass_data = *compass.get_component<CompassData>();

		compass_data.bearing = core::tr4_angle_to_degrees(-Trng.pGlobTomb4->pAdr->pLara->OrientationH) + 180;

		// swing needle
		compass_data.needle_acceleration = 0.f;
		compass_data.needle_velocity = 0.f;
		compass_data.needle_oscill_angle = core::random(45.f, 179.f) * core::random_sign();
	}
}

void CompassSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	auto compasses = entity_manager.find_entities_with_component<CompassData>();
	for (auto compass_it = compasses.begin(); compass_it != compasses.end(); ++compass_it) {
		auto &compass = **compass_it;
		auto &compass_data = *compass.get_component<CompassData>();

		// settle needle
		if (compass_data.needle_friction == 1) {
			compass_data.needle_oscill_angle = 0;
		}
		else {
			compass_data.needle_acceleration = -compass_data.needle_attraction * sin(core::degrees_to_radians(compass_data.needle_oscill_angle));
			compass_data.needle_velocity += compass_data.needle_acceleration;
			compass_data.needle_oscill_angle += compass_data.needle_velocity;
			compass_data.needle_velocity *= (1 - compass_data.needle_friction);
		}

		compass_data.needle_angle = compass_data.bearing + compass_data.needle_oscill_angle;
	}
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

	for (uint8_t i = 0; i < 128; i += 4) {
		if (*reinterpret_cast<uint16_t*>(tr4_pickup_buffer_offset + i) == UINT16_MAX) {
			break;
		}
		clear_tr4_pickup_buffer = true;

		// skip if not in desired phase
		// ^ we still want to clear the tr4 buffer though
		if (static_cast<int32_t>(GET.LaraInfo.SkipPhaseFlags) != enumSKIP.NONE
			&& !core::bit_set(GET.LaraInfo.SkipPhaseFlags, enumSKIP.FLY_CAMERA, true)) {
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
	// skip if not in desired phase
	Get(enumGET.INFO_LARA, 0, 0);
	if (static_cast<int32_t>(GET.LaraInfo.SkipPhaseFlags) != enumSKIP.NONE
		&& !core::bit_set(GET.LaraInfo.SkipPhaseFlags, enumSKIP.FIXED_CAMERA, true)) {
		return;
	}

	// replace shortcut functions for medipacks
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
}

}
}
