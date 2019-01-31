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

#include "sound.h"

#include <algorithm>

#include <trng_core.h>

extern TYPE_SoundEffect SoundEffect;
extern TYPE_StopSoundEffect StopSoundEffect;

namespace classicinventory {
namespace sound {

void SoundSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	auto entities = entity_manager.find_entities_with_component<SoundFX>();

	for (auto entity_it = entities.begin(); entity_it != entities.end(); ++entity_it) {
		auto &entity = **entity_it;

		auto sfxs = entity.get_components<SoundFX>();
		for (auto sfx_it = sfxs.begin(); sfx_it != sfxs.end(); ++sfx_it) {
			auto &sfx = **sfx_it;

			if (sfx.sound_id < 0) {
				continue;
			}

			if (sfx.stop) {
				StopSoundEffect(sfx.sound_id);
			}
			else {
				SoundEffect(sfx.sound_id, nullptr, 2);

				// sfx might be looping so keep track of sfx played and stop on cleanup
				played_sound_ids.insert(sfx.sound_id);
			}
		}
	}

	entity_manager.remove_components<SoundFX>();
}

void SoundSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	entity_manager.remove_components<SoundFX>();

	// stop all sfx played by system
	// this will insure any looping sfx is stopped
	// can't use SoundStopAllSamples as that will stop other sfx as well
	std::for_each(played_sound_ids.begin(), played_sound_ids.end(), [](const int32_t &sound_id) -> void {
		StopSoundEffect(sound_id);
	});
}

}
}
