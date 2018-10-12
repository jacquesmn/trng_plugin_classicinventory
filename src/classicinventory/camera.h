#pragma once
#pragma pack(push,1)

#include "ecs.h"
#include "core.h"

namespace classicinventory {
namespace camera {

class InventoryCameraSystem : public ecs::System {
public:
	void init(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager& entity_manager, ecs::SystemManager& system_manager) override;
};

struct CameraView : public ecs::Component {
	core::Vector3D position;
	core::Vector3D target;
	core::Vector3D rotation;
	float fov;

	CameraView(
		core::Vector3D position = core::Vector3D(),
		core::Vector3D target = core::Vector3D(),
		core::Vector3D rotation = core::Vector3D(),
		float fov = 80
	)
		:
		position(position),
		target(target),
		rotation(rotation),
		fov(fov)
	{}
};

}
}

#pragma pack(pop)