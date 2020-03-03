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

#include <string>

namespace classicinventory {
namespace script {

namespace StringIndex {
enum Enum {
	UNLIMITED = 39,
	USE,
	CHOOSE_AMMO,
	COMBINE,
	SEPARATE,
	EQUIP,
	COMBINE_WITH,
	LOAD_GAME,
	SAVE_GAME,
	EXAMINE,
	STATISTICS = 183,
	DISTANCE_TRAVELLED,
	AMMO_USED,
	SECRETS_FOUND,
	HEALTH_PACKS_USED = 188,
	TIME_TAKEN,
	DAYS,
	OF
};
}

class ScriptString {
private:
	int32_t index;
	std::string string;

	void init();

public:
	ScriptString(int32_t index = -1);
	ScriptString(std::string string);

	int32_t get_index() const { return index; }
	std::string get_string() const { return string; };
};

}
}

#pragma pack(pop)