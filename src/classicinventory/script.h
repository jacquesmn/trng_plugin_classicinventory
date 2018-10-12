#pragma once
#pragma pack(push,1)

#include <string>

namespace classicinventory {
namespace script {

namespace StringIndex {
enum Enum {
	USE = 40,
	CHOOSE_AMMO,
	COMBINE,
	SEPARATE,
	EQUIP,
	COMBINE_WITH,
	LOAD_GAME,
	SAVE_GAME,
	EXAMINE
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