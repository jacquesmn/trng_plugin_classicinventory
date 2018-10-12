#include <StdAfx.h>

#include "ecs.h"

#include <memory>

namespace classicinventory {
namespace ecs {

std::unique_ptr<EntityManager> entity_manager;
std::unique_ptr<SystemManager> system_manager;

EntityManager& new_entity_manager()
{
	entity_manager = std::unique_ptr<EntityManager>(new EntityManager());

	return *entity_manager;
}

EntityManager& get_entity_manager()
{
	if (!entity_manager) {
		return new_entity_manager();
	}
	return *entity_manager;
}

SystemManager& new_system_manager()
{
	system_manager = std::unique_ptr<SystemManager>(new SystemManager());

	return *system_manager;
}

SystemManager& get_system_manager()
{
	if (!system_manager) {
		return new_system_manager();
	}
	return *system_manager;
}

void test()
{
	auto &system_manager = new_system_manager();

	system_manager.add_system<System>(nullptr);
}

}
}