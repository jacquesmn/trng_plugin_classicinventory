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

#include <trng_core.h>
#include "script.h"

namespace classicinventory {
namespace script {

ScriptString::ScriptString(int32_t index)
	:
	index(index)
{
	init();
}

ScriptString::ScriptString(std::string string)
	:
	index(-1),
	string(string)
{
	init();
}

void ScriptString::init()
{
	if (index < 0) {
		return;
	}

	const auto string_type = index <= 358 ? enumGET.STRING : enumGET.STRINGNG;
	if (index > 358) {
		index -= 359;
	}

	Get(string_type, index, 0);
	string = string_type == enumGET.STRING ? GET.pString : GET.pStringNG;
}

}
}