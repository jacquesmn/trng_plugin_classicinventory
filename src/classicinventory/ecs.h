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

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace classicinventory {
namespace ecs {

class EntityManager;
class SystemManager;

inline int32_t next_entity_id()
{
	static int32_t id = 0;
	return id++;
}

inline int32_t next_component_type_id()
{
	static int32_t id = 0;
	return id++;
}

inline int32_t next_system_id()
{
	static int32_t id = 0;
	return id++;
}

// this will return a unique id for component type T
template <class T>
int32_t get_component_type_id()
{
	static auto id = next_component_type_id();
	return id;
}

// this will return a unique id for system type T
template <class T>
int32_t get_system_id()
{
	static auto id = next_system_id();
	return id;
}

class Entity {
	friend class EntityManager;

private:
	int32_t id;
	std::bitset<1024> component_type_bitset;
	EntityManager &entity_manager;

	Entity(
		const int32_t id,
		EntityManager &entity_manager
	)
		:
		id(id),
		entity_manager(entity_manager)
	{}

public:
	int32_t get_id() const { return id; }

	template <class T>
	T& add_component(T *component)
	{
		return entity_manager.add_component<T>(*this, component);
	}

	template <class T>
	void remove_components(std::function<bool(const T&)> component_predicate = nullptr)
	{
		return entity_manager.remove_components<T>(*this, component_predicate);
	}

	template <class T>
	std::vector<T*> get_components(std::function<bool(T&)> component_filter = nullptr) const
	{
		return entity_manager.get_components<T>(*this, component_filter);
	}

	template <class T>
	T* get_component(std::function<bool(T&)> component_filter = nullptr) const
	{
		return entity_manager.get_component<T>(*this, component_filter);
	}

	template <class T>
	bool has_component()
	{
		return entity_manager.has_component<T>(*this);
	}

	template <class T>
	void sort_components(std::function<bool(const T&, const T&)> component_sorter)
	{
		entity_manager.sort_components<T>(*this, component_sorter);
	}
};

struct Component {

	Component() {}
	virtual ~Component() {}
};

class System {
public:
	System() {}
	virtual ~System() {}

	virtual void init(EntityManager &entity_manager, SystemManager &system_manager) {};

	virtual void update(EntityManager &entity_manager, SystemManager &system_manager) {};

	virtual void cleanup(EntityManager &entity_manager, SystemManager &system_manager) {};
};

class EntityManager {
private:
	std::vector<std::unique_ptr<Entity>> entity_store;
	std::vector<std::vector<std::vector<std::unique_ptr<Component>>>> component_store;

public:
	Entity& new_entity()
	{
		const auto entity_id = next_entity_id();

		if (entity_id >= static_cast<int32_t>(entity_store.size())) {
			entity_store.resize(entity_id * 2 + 1);
		}
		entity_store[entity_id] = std::unique_ptr<Entity>(new Entity(entity_id, *this));

		return *entity_store[entity_id];
	}

	Entity* get_entity(const int32_t entity_id) const
	{
		if (entity_id >= static_cast<int32_t>(entity_store.size())) {
			return nullptr;
		}
		return entity_store[entity_id].get();
	}

	template <class T>
	std::vector<Entity*> find_entities_with_component(std::function<bool(const T&)> component_filter = nullptr)
	{
		std::vector<Entity*> results;

		auto component_type_id = get_component_type_id<T>();

		if (!component_store.empty() && component_type_id < static_cast<int32_t>(component_store.size())) {
			auto &entities = component_store[component_type_id];

			for (int32_t entity_id = 0; entity_id < static_cast<int32_t>(entities.size()); ++entity_id) {
				auto &components = entities[entity_id];

				if (components.empty()) {
					continue;
				}

				if (component_filter) {
					auto found = false;

					for (auto it = components.begin(); it != components.end(); ++it) {
						if (component_filter(*static_cast<T*>(it->get()))) {
							found = true;
							break;
						}
					}

					if (!found) {
						continue;
					}
				}

				results.push_back(entity_store[entity_id].get());
			}
		}

		return results;
	}

	template <class T>
	Entity* find_entity_with_component(std::function<bool(const T&)> component_filter = nullptr)
	{
		auto entities = find_entities_with_component<T>(component_filter);
		if (entities.empty()) {
			return nullptr;
		}
		return entities.at(0);
	}

	template <class T>
	T& add_component(Entity &entity, T *component)
	{
		auto component_type_id = get_component_type_id<T>();

		if (component_type_id >= static_cast<int32_t>(component_store.size())) {
			component_store.resize(component_type_id * 2 + 1);
		}
		auto &entities = component_store[component_type_id];

		if (entity.get_id() >= static_cast<int32_t>(entities.size())) {
			entities.resize(entity.get_id() * 2 + 1);
		}
		auto &components = entities[entity.get_id()];

		components.push_back(std::unique_ptr<Component>(component));

		entity.component_type_bitset.set(component_type_id, true);

		return *component;
	}

	template <class T>
	void remove_components(Entity &entity, std::function<bool(const T&)> component_predicate = nullptr)
	{
		auto component_type_id = get_component_type_id<T>();

		if (!component_store.empty() && component_type_id < static_cast<int32_t>(component_store.size())) {
			auto &entities = component_store[component_type_id];

			if (!entities.empty() && entity.get_id() < static_cast<int32_t>(entities.size())) {
				auto &components = entities[entity.get_id()];

				components.erase(
					std::remove_if(
						components.begin(),
						components.end(),
						[&](const std::unique_ptr<Component> &c) -> bool {return !component_predicate ? true : component_predicate(*static_cast<T*>(c.get())); }
					),
					components.end()
				);

				if (components.empty()) {
					entity.component_type_bitset.set(component_type_id, false);
				}
			}
		}
	}

	template <class T>
	void remove_components(std::function<bool(const T&)> component_predicate = nullptr)
	{
		auto entities = find_entities_with_component<T>(component_predicate);
		if (entities.empty()) {
			return;
		}

		for (auto entity_it = entities.begin(); entity_it != entities.end(); ++entity_it) {
			auto &entity = **entity_it;

			remove_components<T>(entity, component_predicate);
		}
	}

	template <class T>
	std::vector<T*> get_components(
		const Entity &entity,
		std::function<bool(T&)> component_filter = nullptr
	) const
	{
		std::vector<T*> results;

		auto component_type_id = get_component_type_id<T>();

		if (entity.component_type_bitset[component_type_id]
			&& !component_store.empty()
			&& component_type_id < static_cast<int32_t>(component_store.size())) {
			auto &entities = component_store[component_type_id];

			if (!entities.empty() && entity.get_id() < static_cast<int32_t>(entities.size())) {
				auto &components = entities[entity.get_id()];

				for (auto it = components.begin(); it != components.end(); ++it) {
					if (component_filter && !component_filter(*static_cast<T*>(it->get())))
					{
						continue;
					}
					results.push_back(static_cast<T*>(it->get()));
				}
			}
		}

		return results;
	}

	template <class T>
	T* get_component(
		const Entity &entity,
		std::function<bool(T&)> component_filter = nullptr
	) const
	{
		auto components = get_components<T>(entity, component_filter);
		if (components.empty()) {
			return nullptr;
		}
		return components.at(0);
	}

	template <class T>
	bool has_component(const Entity &entity) const {
		auto component_type_id = get_component_type_id<T>();

		return entity.component_type_bitset[component_type_id];
	}

	template <class T>
	void sort_components(
		const Entity &entity,
		std::function<bool(const T&, const T&)> component_sorter
	)
	{
		auto component_type_id = get_component_type_id<T>();

		if (entity.component_type_bitset[component_type_id]
			&& !component_store.empty()
			&& component_type_id < static_cast<int32_t>(component_store.size())) {
			auto &entities = component_store[component_type_id];

			if (!entities.empty() && entity.get_id() < static_cast<int32_t>(entities.size())) {
				auto &components = entities[entity.get_id()];

				std::stable_sort(
					components.begin(),
					components.end(),
					[=](const std::unique_ptr<Component> &a, const std::unique_ptr<Component> &b) -> bool {
					return component_sorter(*static_cast<T*>(a.get()), *static_cast<T*>(b.get()));
				});
			}
		}
	}
};

class SystemManager {
private:
	std::vector<std::unique_ptr<System>> system_store;

public:
	template <class T>
	T& add_system(T *system)
	{
		auto system_id = get_system_id<T>();

		if (system_id >= static_cast<int32_t>(system_store.size())) {
			system_store.resize(system_id * 2 + 1);
		}

		system_store[system_id] = std::unique_ptr<System>(system);

		return *system;
	}

	std::vector<System*> get_systems()
	{
		std::vector<System*> results;

		for (auto it = system_store.begin(); it != system_store.end(); ++it) {
			auto *system = it->get();
			if (!system) {
				continue;
			}

			results.push_back(system);
		}

		return results;
	}

	template <class T>
	T* get_system()
	{
		auto system_id = get_system_id<T>();

		if (!system_store.empty() && system_id < static_cast<int32_t>(system_store.size())) {
			return static_cast<T*>(system_store.at(system_id).get());
		}

		return nullptr;
	}

	template <class T>
	void remove_system()
	{
		auto system_id = get_system_id<T>();

		if (!system_store.empty() && system_id < static_cast<int32_t>(system_store.size())) {
			system_store.erase(system_store.begin() + system_id);
		}
	}
};


EntityManager& new_entity_manager();
EntityManager& get_entity_manager();
SystemManager& new_system_manager();
SystemManager& get_system_manager();

}
}

#pragma pack(pop)