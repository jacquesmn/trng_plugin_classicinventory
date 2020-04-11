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

#include <regex>

#include <trng_core.h>
#include "script.h"

extern char* ReadTextVariable(int Code);

namespace classicinventory {
namespace script {

ScriptString::ScriptString(int32_t index)
	:
	index(index)
{}

ScriptString::ScriptString(std::string string)
	:
	index(-1),
	string(string)
{}

std::string ScriptString::get_string() const
{
	auto value = string;
	
	if (index >= 0) {
		const auto string_type = index <= 358 ? enumGET.STRING : enumGET.STRINGNG;

		Get(string_type, index > 358 ? index - 359 : index, 0);

		value = std::string(string_type == enumGET.STRING ? GET.pString : GET.pStringNG);
	}

	// replace script code with text variable value
	const std::regex script_code_regex("#([0-9A-Fa-f]{4})");
	std::string script_code_subject = std::string(value);
	std::smatch script_code_match;

	while (std::regex_search(script_code_subject, script_code_match, script_code_regex)) {
		value = std::regex_replace(
			value,
			std::regex(script_code_match.str(0)),
			std::string(ReadTextVariable(std::stoi(script_code_match.str(1), 0, 16) | SCRIPT_CODE))
		);

		script_code_subject = script_code_match.suffix().str();
	}

	return value;
}

}
}