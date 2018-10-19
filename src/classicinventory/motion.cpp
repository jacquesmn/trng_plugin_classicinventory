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

#include "motion.h"

#include "core.h"

namespace classicinventory {
namespace motion {

void MotionSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	auto entities = entity_manager.find_entities_with_component<Motion>();

	for (auto entity_it = entities.begin(); entity_it != entities.end(); ++entity_it) {
		auto &entity = **entity_it;

		auto motions = entity.get_components<Motion>([](const Motion &motion) -> bool {
			return motion.active;
		});

		for (auto motion_it = motions.begin(); motion_it != motions.end(); ++motion_it) {
			auto &motion = **motion_it;

			if (motion.loop && motion.frame >= motion.frames) {
				motion.value = motion.direction == Motion::FORWARD ? motion.start : motion.end;
				motion.frame = 0;
			}

			++motion.frame;
			motion.value += (motion.rate * motion.direction);

			if (!motion.loop && (motion.frame >= motion.frames || motion.rate == 0)) {
				motion.finish();
			}
		}

		entity.remove_components<Motion>([](const Motion &motion) -> bool {
			return !motion.active;
		});
	}
}

void MotionSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	// finish and remove all motions
	auto entities = entity_manager.find_entities_with_component<Motion>();
	for (auto entity_it = entities.begin(); entity_it != entities.end(); ++entity_it) {
		auto &entity = **entity_it;

		auto motions = entity.get_components<Motion>();
		for (auto motion_it = motions.begin(); motion_it != motions.end(); ++motion_it) {
			(*motion_it)->finish();
		}

		entity.remove_components<Motion>();
	}
}

Motion::Motion(
	float& value,
	float start,
	float end,
	uint32_t frames,
	float rate,
	Motion::Direction direction,
	bool loop,
	bool background
)
	:
	value(value),
	start(start),
	end(end),
	frames(frames),
	rate(abs(rate)),
	direction(direction),
	loop(loop),
	background(background)
{
	init();
}

void Motion::init()
{
	value = start;
	frame = 0;
	restoring = false;
	active = true;

	if (rate == 0) {
		recalculate();
	}
}

void Motion::accelerate(float speed)
{
	speed = abs(speed);

	rate *= speed;
	frame = uint32_t(frame / speed);
	this->frames = uint32_t(this->frames / speed);
}

void Motion::restore(uint32_t frames)
{
	loop = false;
	restoring = true;

	// restore via shortest route
	if (frame < (this->frames * 0.5)) {
		reverse();
	}

	// restore in given frames
	if (frames > 0) {
		const auto speed = float(this->frames - frame) / float(frames);
		rate *= speed;
		frame = 0;
		this->frames = frames;
	}
}

void Motion::resume(Motion::Direction direction, uint32_t frames)
{
	restoring = false;

	this->direction = direction;
	this->frames = frames;

	frame = core::round(abs((value - start) / (end - start)) * frames);

	recalculate();
}

void Motion::reverse()
{
	direction = Direction(direction * -1);
	frame = this->frames - frame;
}

void Motion::recalculate()
{
	rate = frames > 0 ? (end - start) / frames : 0;
}

void Motion::finish()
{
	value = (direction == FORWARD ? end : start);
	frame = frames;
	loop = false;
	restoring = false;
	active = false;
}

}
}