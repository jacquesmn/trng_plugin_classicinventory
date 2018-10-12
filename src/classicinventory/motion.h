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

}
}

#pragma pack(pop)