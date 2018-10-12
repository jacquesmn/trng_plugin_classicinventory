#pragma once
#pragma pack(push,1)

#include <set>

#include "ecs.h"

namespace classicinventory {
namespace sound {

namespace SfxType {
enum Enum {
	INVENTORY_OPEN = 1,
	INVENTORY_CLOSE,
	RING_ROTATE,
	RING_CHANGE,
	ITEM_SELECT,
	ITEM_ACTIVE,
	ITEM_CANCEL,
	MENU_CHANGE,
	LOAD_AMMO,
	COMBINE,
	SEPARATE,
	FAIL,
	NONE
};
}

class SoundSystem : public ecs::System {
private:
	std::set<int32_t> played_sound_ids;

public:
	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

struct SoundFX : public ecs::Component {
	int32_t sound_id;
	bool stop;

	SoundFX(
		int32_t sound_id,
		bool stop = false
	)
		:
		sound_id(sound_id),
		stop(stop)
	{}
};

}
}

#pragma pack(pop)