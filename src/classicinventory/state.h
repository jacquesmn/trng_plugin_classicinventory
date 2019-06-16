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

#include <functional>
#include <vector>

#include "ecs.h"
#include "input.h"
#include "inventory.h"
#include "item.h"
#include "ring.h"
#include "special.h"
#include "sound.h"

namespace classicinventory {
namespace state {

class StateMachine;
class State;

namespace GamePhase {
enum Enum {
	UNKNOWN,
	MAIN,
	INVENTORY
};
}

class StateSystem : public ecs::System {
private:
	StateMachine &state_machine;

public:
	StateSystem(StateMachine &state_machine);

	void update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;

	void cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager) override;
};

class StateMachine {
private:
	GamePhase::Enum current_phase;
	std::unique_ptr<State> current_state;
	ecs::EntityManager &entity_manager;

	void set_phase(GamePhase::Enum phase);

public:
	StateMachine(State *state, ecs::EntityManager &entity_manager);

	void update(ecs::EntityManager &entity_manager);

	void input(input::InputState &input_state, ecs::EntityManager &entity_manager);

	GamePhase::Enum get_phase() const;

	void set_state(State *state);

	State* transition(State *state) const;
};

class State {
public:
	virtual ~State() {}

	virtual State* start(ecs::EntityManager &entity_manager) { return this; }

	virtual void end(ecs::EntityManager &entity_manager) {}

	virtual State* update(ecs::EntityManager &entity_manager) { return this; }

	virtual State* input(input::InputState &input_state, ecs::EntityManager &entity_manager) { return this; }

	virtual GamePhase::Enum get_phase() { return GamePhase::UNKNOWN; }
};

class ClosedState : public State {
public:
	GamePhase::Enum get_phase() override;
};

class OpeningState : public State {
public:
	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

	GamePhase::Enum get_phase() override;
};

class IdleState : public State {
public:
	State* start(ecs::EntityManager &entity_manager) override;
	void end(ecs::EntityManager &entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class RingRotateState : public State {
public:
	enum Direction {
		RIGHT,
		LEFT
	};

	RingRotateState(
		RingRotateState::Direction direction,
		std::function<State*()> get_next_state
	);

	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

private:
	RingRotateState::Direction direction;
	std::function<State*()> get_next_state;
};

class RingChangeState : public State {
public:
	enum Type {
		UP,
		DOWN,
		FADE,
		INSTANT
	};

	RingChangeState(
		RingChangeState::Type type,
		std::function<inventory::InventoryRing*()> get_ring_target,
		std::function<State*()> get_next_state,
		bool play_sound = true
	);

	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

private:
	RingChangeState::Type type;
	std::function<inventory::InventoryRing*()> get_ring_target;
	std::function<State*()> get_next_state;
	bool play_sound;

	bool halfway;
};

class ItemActivateState : public State {
private:
	bool motions_done;

public:
	ItemActivateState();

	State* start(ecs::EntityManager& entity_manager) override;
	void end(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;
};

class ItemActiveState : public State {
private:
	bool enable_auto_cancel;
	bool loading_or_saving;

	State* do_action(
		ecs::Entity &item_active,
		ecs::Entity &inventory,
		std::function<State*()> get_default_state,
		ecs::EntityManager& entity_manager
	);

public:
	ItemActiveState(bool enable_auto_cancel);

	State* start(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class ItemCancelState : public State {
private:
	std::function<State*()> get_next_state;
	bool play_sfx;
	bool play_animations;
	bool restore_orientation;
	bool restore_model;

	bool motions_done;

public:
	ItemCancelState(
		std::function<State*()> get_next_state,
		bool play_sfx = true,
		bool play_animations = true,
		bool restore_orientation = true,
		bool restore_model = true
	);

	State* start(ecs::EntityManager& entity_manager) override;
	void end(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;
};

class AmmoContextState : public State {
private:
	item::ItemAction &item_action;

public:
	AmmoContextState(item::ItemAction &item_action);

	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class AmmoLoadState : public State {
public:
	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;
};

class ComboContextState : public State {
private:
	item::ItemAction &item_action;

public:
	ComboContextState(item::ItemAction &item_action);

	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class ComboTryState : public State {
public:
	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;
};

class SeparateState : public State {
public:
	State* update(ecs::EntityManager& entity_manager) override;
};

class ExamineState : public State {
public:
	State* start(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class StatisticsState : public State {
public:
	State* start(ecs::EntityManager& entity_manager) override;
	void end(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class OptionsState : public State {
private:
	bool controls;

public:
	OptionsState(bool controls = false);

	State* start(ecs::EntityManager& entity_manager) override;
	void end(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;
};

class PassportState : public State {
private:
	std::function<State*()> get_post_closing_state;
	bool loading_or_saving;
	bool page_changed;
	bool closing;

public:
	PassportState();

	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager &entity_manager) override;
};

class MapState : public State {
private:
	void update_map_marker(
		ecs::Entity &item,
		item::ItemDisplay &item_display,
		special::MapData &map_data,
		uint32_t duration_frames,
		ecs::EntityManager &entity_manager
	);

public:
	State* start(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class CheatState : public State {
private:
	int32_t entity_id;

public:
	CheatState(int32_t entity_id);

	State* update(ecs::EntityManager& entity_manager) override;
};

class DebugState : public State {
private:
	ecs::Entity &item;
	item::ItemDisplayType::Enum display_type;
	std::function<State*()> get_next_state;

public:
	DebugState(
		ecs::Entity &item,
		item::ItemDisplayType::Enum display_type,
		std::function<State*()> get_next_state
	);

	State* start(ecs::EntityManager& entity_manager) override;
	void end(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;

	State* input(input::InputState &input_state, ecs::EntityManager& entity_manager) override;
};

class ClosingState : public State {
private:
	bool play_sfx;

public:
	ClosingState(bool play_sfx = true);

	State* start(ecs::EntityManager& entity_manager) override;

	State* update(ecs::EntityManager& entity_manager) override;
};

// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
ring::RingItem* get_selected_item(ecs::EntityManager &entity_manager);

ring::RingItem* get_active_item(ecs::EntityManager &entity_manager);

void clear_hud(ecs::EntityManager &entity_manager);

void add_sfx(sound::SfxType::Enum type, bool stop, ecs::Entity &inventory, ecs::Entity *item = nullptr);

}
}

#pragma pack(pop)