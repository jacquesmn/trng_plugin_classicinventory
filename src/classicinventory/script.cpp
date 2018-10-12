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