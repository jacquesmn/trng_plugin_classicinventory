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

namespace classicinventory {
namespace motion {

class MotionSystem : public ecs::System {
public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class Motion : public ecs::Component {
public:
	enum Direction {
		FORWARD = 1,
		BACKWARD = -1
	};

	float &value;
	float start;
	float end;
	uint32_t frames;
	uint32_t frame;
	float rate;
	Motion::Direction direction;
	bool loop;
	bool restoring;
	bool background;
	bool active;

	Motion(
		float &value,
		float start,
		float end,
		uint32_t frames,
		float rate = 0,
		Motion::Direction direction = Motion::FORWARD,
		bool loop = false,
		bool background = false
	);

	void init();

	void accelerate(float speed = 1);

	void restore(uint32_t frames = 0);

	void resume(Motion::Direction direction, uint32_t frames);

	void reverse();

	void recalculate();

	void finish();
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
std::vector<ecs::Entity*> get_entities_in_motion(ecs::EntityManager &entity_manager);

// Adds rotation motion through shortest route.
void add_motion_rot(ecs::Entity &entity, float &value, float start, float end, int32_t frames);

}
}

#pragma pack(pop)