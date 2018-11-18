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

#include "ecs.h"
#include "item.h"

namespace classicinventory {
namespace input {
class InputState;
}
}

namespace classicinventory {
namespace special {

class TimeSystem : public ecs::System {
public:
	void update(ecs::EntityManager& entity_manager, ecs::SystemManager &system_manager) override;
};

class CompassSystem : public ecs::System {
public:
	void init(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class StopwatchSystem : public ecs::System {
private:
	bool first_update;

public:
	StopwatchSystem();

	void init(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class PickupSystem : public ecs::System {
public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class ShortcutSystem : public ecs::System {
private:
	input::InputState &input_state;

public:
	ShortcutSystem(input::InputState &input_state);

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};



struct GameTime : public ecs::Component {
	uint32_t frames;
	float hours;
	float minutes;
	float seconds;
	float milliseconds;

	GameTime()
		:
		frames(0),
		hours(0),
		minutes(0),
		seconds(0),
		milliseconds(0)
	{}
};

struct HealthData : public ecs::Component {
	int32_t health_points;
	uint32_t poison_points;
	int32_t heal_sound_id;
	int32_t hurt_sound_id;
	bool cure_poison;
	bool increase_usage_stats;

	HealthData(
		int32_t health_points,
		uint32_t poison_points = 0,
		int32_t heal_sound_id = -1,
		int32_t hurt_sound_id = -1,
		bool cure_poison = true,
		bool increase_usage_stats = true
	)
		:
		health_points(health_points),
		poison_points(poison_points),
		heal_sound_id(heal_sound_id),
		hurt_sound_id(hurt_sound_id),
		cure_poison(cure_poison),
		increase_usage_stats(increase_usage_stats)
	{}
};

struct CompassData : public ecs::Component {
	int32_t needle_mesh_index;
	core::Axis::Enum needle_mesh_axis;
	float needle_angle;

	float needle_oscill_amplitude;
	float needle_oscill_amplitude_min;
	float needle_oscill_amplitude_max;
	float needle_oscill_angle;

	uint32_t needle_oscill_amplitude_settle_frames;
	uint32_t needle_oscill_period_frames;

	float bearing;

	CompassData(
		int32_t needle_mesh_index,
		core::Axis::Enum needle_mesh_axis = core::Axis::Y
	)
		:
		needle_mesh_index(needle_mesh_index),
		needle_mesh_axis(needle_mesh_axis),
		needle_angle(0),
		needle_oscill_amplitude(0),
		needle_oscill_amplitude_min(0),
		needle_oscill_amplitude_max(0),
		needle_oscill_angle(0),
		needle_oscill_amplitude_settle_frames(0),
		needle_oscill_period_frames(0),
		bearing(0)
	{}
};

struct StopwatchData : public ecs::Component {
	int32_t hour_hand_mesh_index;
	core::Axis::Enum hour_hand_mesh_axis;
	int32_t minute_hand_mesh_index;
	core::Axis::Enum minute_hand_mesh_axis;
	int32_t second_hand_mesh_index;
	core::Axis::Enum second_hand_mesh_axis;
	int32_t frequency_frames;

	float hour_hand_angle;
	float minute_hand_angle;
	float second_hand_angle;

	StopwatchData(
		int32_t hour_hand_mesh_index,
		core::Axis::Enum hour_hand_mesh_axis,
		int32_t minute_hand_mesh_index,
		core::Axis::Enum minute_hand_mesh_axis,
		int32_t second_hand_mesh_index,
		core::Axis::Enum second_hand_mesh_axis,
		int32_t frequency_frames = 30
	)
		:
		hour_hand_mesh_index(hour_hand_mesh_index),
		hour_hand_mesh_axis(hour_hand_mesh_axis),
		minute_hand_mesh_index(minute_hand_mesh_index),
		minute_hand_mesh_axis(minute_hand_mesh_axis),
		second_hand_mesh_index(second_hand_mesh_index),
		second_hand_mesh_axis(second_hand_mesh_axis),
		frequency_frames(frequency_frames),
		hour_hand_angle(0),
		minute_hand_angle(0),
		second_hand_angle(0)
	{}
};

struct PassportData : public ecs::Component {
	int32_t page;
	int32_t page_sound_id;

	PassportData(
		int32_t page = 0,
		int32_t page_sound_id = -1
	)
		:
		page(page),
		page_sound_id(page_sound_id)
	{}
};

struct MapMarker : public ecs::Component {
	int32_t mesh_index;
	core::Vector3D map_orient;
	item::ItemAction *action;

	MapMarker(
		int32_t mesh_index,
		core::Vector3D map_orient = core::Vector3D()
	)
		:
		mesh_index(mesh_index),
		map_orient(map_orient),
		action(nullptr)
	{}
};

struct MapData : public ecs::Component {
	std::vector<MapMarker> markers;
	uint32_t marker_index;
	bool marker_active;
	bool cancelable;

	MapData()
		:
		markers(std::vector<MapMarker>()),
		marker_index(0),
		marker_active(false),
		cancelable(true)
	{}
};

}
}

#pragma pack(pop)