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

#include <StdAfx.h>

#include "state.h"

#include <algorithm>
#include <bitset>
#include <sstream>

#include <trng_core.h>
#include "action.h"
#include "cheat.h"
#include "ecs.h"
#include "camera.h"
#include "core.h"
#include "motion.h"
#include "render.h"
#include "script.h"
#include "text.h"

extern StrMyData MyData;
extern TYPE_use_current_item use_current_item;
extern TYPE_DoStatScreen DoStatScreen;
extern TYPE_DoOptions DoOptions;
extern TYPE_S_PauseMenu S_PauseMenu;

namespace classicinventory {
namespace state {

uint32_t *tr4_item_selected = reinterpret_cast<uint32_t*>(0x4AC05C); // tr4 pointer to item selected in inventory (invobj)

StateSystem::StateSystem(StateMachine &state_machine)
	:
	state_machine(state_machine)
{}

void StateSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	state_machine.update(entity_manager);
}

void StateSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	*Trng.pGlobTomb4->pAdr->pInventoryRequiredSlotItem = -1;
	*Trng.pGlobTomb4->pAdr->pInventoryChosenItem = -1;
	*tr4_item_selected = UINT32_MAX;

	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}
	auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

	// save state
	if (inventory_state.ring) {
		const auto ring_data = inventory_state.ring->ring.get_component<ring::RingData>();
		if (ring_data) {
			MyData.Save.Local.inventory_data.ring_id_selected = ring_data->ring_id;
		}

		const auto ring_state = inventory_state.ring->ring.get_component<ring::RingState>();
		if (ring_state && ring_state->item) {
			auto &item = ring_state->item->item;
			const auto item_data = item.get_component<item::ItemData>();
			if (item_data) {
				MyData.Save.Local.inventory_data.item_id_selected = item_data->item_id;

				*tr4_item_selected = item::item_id_to_tr4_invobj(item_data->item_id);
			}
		}
	}

	// perform action if item used
	if (inventory_state.item_used) {
		auto &item_used = inventory_state.item_used->item;

		if (item_used.has_component<item::ItemData>()) {
			auto &item_data = *item_used.get_component<item::ItemData>();

			MyData.Save.Local.inventory_data.item_id_used = item_data.item_id;

			const auto tr4_slot_id = item::item_id_to_tr4_slot(item_data.item_id);

			const auto item_action = item_used.get_component<item::ItemAction>([](item::ItemAction &action) -> bool {
				return action.active && action.action;
			});

			DetectedGlobalTriggerEvent(GT_SELECTED_INVENTORY_ITEM, tr4_slot_id, false);

			if (item_action) {
				item_action->action();
			}

			if (!item_action || !item_action->replace_tr4) {
				*Trng.pGlobTomb4->pAdr->pInventoryChosenItem = tr4_slot_id;
			}
		}
	}
}



// --------------------------------------
// ##### THE BIG MEAN STATE MACHINE #####
// --------------------------------------
// Instead of having a big switch statement, each state will be a separate class managed by the state machine.
// Each state is self-contained and will decide the next state and when to transition.

StateMachine::StateMachine(State *state, ecs::EntityManager &entity_manager)
	:
	entity_manager(entity_manager)
{
	set_state(state);
}

void StateMachine::update(ecs::EntityManager &entity_manager)
{
	set_state(current_state->update(entity_manager));
}

void StateMachine::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	set_state(current_state->input(input_state, entity_manager));
}

GamePhase::Enum StateMachine::get_phase() const
{
	return current_phase;
}

void StateMachine::set_phase(GamePhase::Enum phase)
{
	if (phase != GamePhase::UNKNOWN && phase != current_phase) {
		current_phase = phase;
	}
}

void StateMachine::set_state(State* state)
{
	if (state && state != current_state.get()) {
		if (current_state) {
			current_state->end(entity_manager);
		}

		state = transition(state);

		current_state = std::unique_ptr<State>(state);

		set_phase(current_state->get_phase());
	}
}

State* StateMachine::transition(State *state) const
{
	const auto next_state = state->start(entity_manager);

	if (next_state != state) {
		return transition(next_state);
	}

	return next_state;
}



// ---------------------------------
// ##### State Implementations #####
// ---------------------------------
GamePhase::Enum ClosedState::get_phase()
{
	return GamePhase::MAIN;
}

State* OpeningState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		// clear last used item
		MyData.Save.Local.inventory_data.item_id_used = item::ItemId::NONE;
		inventory_state.item_used = nullptr;

		// set requested item as selected
		const auto item_id = item::tr4_slot_to_item_id(*Trng.pGlobTomb4->pAdr->pInventoryRequiredSlotItem);
		auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
			return item_data.item_id == item_id;
		});
		if (item && item->has_component<item::ItemRing>()) {
			auto &item_data = *item->get_component<item::ItemData>();
			auto &item_ring = *item->get_component<item::ItemRing>();

			MyData.Save.Local.inventory_data.ring_id_selected = item_ring.ring_id;
			MyData.Save.Local.inventory_data.item_id_selected = item_data.item_id;
		}

		build_inventory(MyData.Save.Local.inventory_data.ring_id_selected, MyData.Save.Local.inventory_data.item_id_selected, inventory_state, entity_manager);

		// initialize opening motions
		if (inventory->has_component<inventory::InventoryDisplay>() && inventory->has_component<inventory::InventoryDuration>()) {
			const auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();
			const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();

			if (inventory_state.ring) {
				open_ring(inventory_state.ring->ring, inventory_display, &inventory_duration.inventory_open_frames);
			}

			const auto camera = entity_manager.find_entity_with_component<camera::CameraView>();
			if (camera) {
				open_camera(*camera, inventory_display, &inventory_duration.inventory_open_frames);
			}

		}

		// add sfx
		add_sfx(sound::SfxType::INVENTORY_OPEN, false, *inventory);

		clear_hud(entity_manager);
	}

	return this;
}

State* OpeningState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		return new IdleState();
	}

	return this;
}

GamePhase::Enum OpeningState::get_phase()
{
	return GamePhase::INVENTORY;
}

State* IdleState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory_state.ring) {
			auto &ring = inventory_state.ring->ring;
			const auto add_inv_text = !inventory->has_component<render::ScreenText>();

			if (ring.has_component<ring::RingData>()) {
				auto &ring_data = *ring.get_component<ring::RingData>();

				// add text for ring
				if (add_inv_text) {
					text::add_text(
						*inventory,
						text::TextType::RING_NAME,
						ring_data.name.get_string(),
						entity_manager
					);
				}

				if (ring.has_component<ring::RingState>()) {
					auto &ring_state = *ring.get_component<ring::RingState>();

					if (ring_state.item) {
						auto &item = ring_state.item->item;

						if (!item.has_component<render::ScreenText>()
							&& item.has_component<item::ItemData>()) {
							const auto &item_data = *item.get_component<item::ItemData>();
							const auto item_qty = item.get_component<item::ItemQuantity>();

							// add text for item
							const auto item_text = text::build_item_text(
								text::TextType::ITEM_NAME_IDLE,
								entity_manager,
								item_data.name.get_string(),
								item_qty,
								false
							);
							text::add_text(
								item,
								text::TextType::ITEM_NAME_IDLE,
								item_text,
								entity_manager
							);

							// add text for item qty
							const auto qty_text = text::build_item_text(
								text::TextType::ITEM_QTY_IDLE,
								entity_manager,
								item_data.name.get_string(),
								item_qty,
								false
							);
							text::add_text(
								item,
								text::TextType::ITEM_QTY_IDLE,
								qty_text,
								entity_manager
							);

							// add text for ammo 
							if (item.has_component<item::ItemAmmo>()) {
								auto item_ammo = item::get_loaded_ammo(item);

								if (item_ammo) {
									auto &ammo_item = item_ammo->ammo_item;
									if (ammo_item.has_component<item::ItemData>()
										&& ammo_item.has_component<item::ItemQuantity>()) {
										const auto &ammo_data = *ammo_item.get_component<item::ItemData>();
										const auto ammo_qty = ammo_item.get_component<item::ItemQuantity>();

										const auto ammo_text = text::build_item_text(
											text::TextType::ITEM_AMMO_IDLE,
											entity_manager,
											ammo_data.name.get_string(),
											ammo_qty,
											true
										);
										text::add_text(
											item,
											text::TextType::ITEM_AMMO_IDLE,
											ammo_text,
											entity_manager
										);

										// add text for ammo qty
										const auto ammo_qty_text = text::build_item_text(
											text::TextType::ITEM_QTY_IDLE,
											entity_manager,
											ammo_data.name.get_string(),
											ammo_qty,
											true
										);
										text::add_text(
											item,
											text::TextType::ITEM_QTY_IDLE,
											ammo_qty_text,
											entity_manager
										);
									}
								}
							}

							// add text for description
							text::add_text(
								item,
								text::TextType::ITEM_DESC_IDLE,
								item_data.description.get_string(),
								entity_manager
							);

							// add health bar
							inventory::add_health_bar(item);
						}
					}
				}
			}

			// add text for navigation
			if (add_inv_text) {
				if (inventory_state.ring->next) {
					const auto glyph_up = "\x83";

					text::add_text(
						*inventory,
						text::TextType::NAV_UP_LEFT,
						glyph_up,
						entity_manager
					);
					text::add_text(
						*inventory,
						text::TextType::NAV_UP_RIGHT,
						glyph_up,
						entity_manager
					);
				}
				if (inventory_state.ring->prev) {
					const auto glyph_down = "\x85";

					text::add_text(
						*inventory,
						text::TextType::NAV_DOWN_LEFT,
						glyph_down,
						entity_manager
					);
					text::add_text(
						*inventory,
						text::TextType::NAV_DOWN_RIGHT,
						glyph_down,
						entity_manager
					);
				}
			}
		}

		// change item models and displays
		auto items = entity_manager.find_entities_with_component<item::ItemDisplay>();
		std::for_each(items.begin(), items.end(), [](ecs::Entity *item) -> void {
			item::change_item_display(*item, item::ItemDisplayType::IDLE,
				false,
				false,
				true,
				false,
				false,
				true
			);

			item::change_item_model(*item, item::ItemModelType::IDLE);
		});

		const auto ring_item_selected = get_selected_item(entity_manager);
		if (ring_item_selected) {
			auto &item_selected = ring_item_selected->item;

			// spin selected item
			const auto inventory_duration = inventory::get_inventory_duration(entity_manager);
			if (inventory_duration) {
				item::spin_item(item_selected, inventory_duration->item_spin_frames);
			}

			// play selected animation
			if (item_selected.has_component<item::ItemAnimation>()) {
				const auto item_animation = item_selected.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
					return item_animation.type == item::ItemAnimationType::SELECTED;
				});
				if (item_animation) {
					item::start_item_animation(item_selected, *item_animation, true);
				}
			}
		}
	}

	return this;
}

void IdleState::end(ecs::EntityManager &entity_manager)
{
	const auto ring_item_selected = get_selected_item(entity_manager);

	if (ring_item_selected) {
		auto &item_selected = ring_item_selected->item;

		const auto item_animation = item_selected.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
			return item_animation.type == item::ItemAnimationType::SELECTED;
		});
		if (item_animation) {
			item::restore_item_animation(item_selected, *item_animation);
		}
	}
}

State* IdleState::update(ecs::EntityManager &entity_manager)
{
	const auto inventory_state = inventory::get_inventory_state(entity_manager);

	if (inventory_state && inventory_state->activate_selected_item_on_open) {
		inventory_state->activate_selected_item_on_open = false;

		return new ItemActivateState();
	}

	return this;
}

State* IdleState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	const auto inventory_state = inventory::get_inventory_state(entity_manager);

	if (input_state.command_first_press(enumCMD.ACTION) || input_state.command_first_press(enumCMD.ENTER)) {
		return new ItemActivateState();
	}

	if (input_state.command_first_press(enumCMD.INVENTORY)) {
		return new ClosingState();
	}

	if (input_state.command_active(enumCMD.RIGHT)) {
		return new RingRotateState(RingRotateState::RIGHT, []() -> State* {
			return new IdleState();
		});
	}

	if (input_state.command_active(enumCMD.LEFT)) {
		return new RingRotateState(RingRotateState::LEFT, []() -> State* {
			return new IdleState();
		});
	}

	if (input_state.command_first_press(enumCMD.UP)) {
		if (inventory_state && inventory_state->ring && inventory_state->ring->next) {
			const auto ring_target = inventory_state->ring->next;

			return new RingChangeState(RingChangeState::UP, [=]() -> inventory::InventoryRing* {return ring_target; }, []() -> State* {
				return new IdleState();
			});
		}
	}

	if (input_state.command_first_press(enumCMD.DOWN)) {
		if (inventory_state && inventory_state->ring && inventory_state->ring->prev) {
			const auto ring_target = inventory_state->ring->prev;

			return new RingChangeState(RingChangeState::DOWN, [=]() -> inventory::InventoryRing* {return ring_target; }, []() -> State* {
				return new IdleState();
			});
		}
	}

	if (input_state.command_first_press(enumCMD.ROLL)) {
		if (inventory::debug_enabled(entity_manager)) {
			auto ring_item_selected = get_selected_item(entity_manager);

			if (ring_item_selected) {
				return new DebugState(ring_item_selected->item, item::ItemDisplayType::IDLE, []() -> State* {
					return new IdleState();
				});
			}
		}
	}

	// handle active cheats
	auto ring_item_selected = get_selected_item(entity_manager);
	if (ring_item_selected) {
		auto &item_selected = ring_item_selected->item;

		if (cheat::add_active_cheats(item_selected, input_state)) {
			return new CheatState(item_selected.get_id());
		}
	}

	return this;
}

RingRotateState::RingRotateState(
	RingRotateState::Direction direction,
	std::function<State*()> get_next_state
)
	:
	direction(direction),
	get_next_state(get_next_state)
{}

State* RingRotateState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		const auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory_state.ring
			&& inventory_state.ring->ring.has_component<ring::RingState>()
			&& inventory->has_component<inventory::InventoryDuration>()
			&& inventory->has_component<inventory::InventoryDisplay>()) {
			auto &ring = inventory_state.ring->ring;
			auto &ring_state = *ring.get_component<ring::RingState>();

			const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();
			const auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

			if (!inventory_display.ring_rotate_lock || ring_state.item_count > 1) {
				inventory::rotate_ring(ring, inventory_duration.ring_rotate_frames, direction == RIGHT);

				// add sfx
				add_sfx(sound::SfxType::RING_ROTATE, false, *inventory);
			}
		}
	}

	return this;
}

State* RingRotateState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

		// switch item
		if (inventory && inventory->has_component<inventory::InventoryState>()) {
			const auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

			if (inventory_state.ring) {
				auto &ring = inventory_state.ring->ring;

				if (ring.has_component<ring::RingState>()) {
					auto &ring_state = *ring.get_component<ring::RingState>();

					// clear current item details
					if (ring_state.item) {
						ring_state.item->item.remove_components<render::ScreenText>();
						ring_state.item->item.remove_components<render::ScreenBar>();
						ring_state.item->item.remove_components<render::HealthBar>();
						ring_state.item->item.remove_components<render::AirBar>();
					}

					ring_state.change_item();
				}
			}
		}

		return get_next_state();
	}

	return this;
}

RingChangeState::RingChangeState(
	RingChangeState::Type type,
	std::function<inventory::InventoryRing*()> get_ring_target,
	std::function<State*()> get_next_state,
	bool play_sound
)
	:
	type(type),
	get_ring_target(get_ring_target),
	get_next_state(get_next_state),
	play_sound(play_sound),
	halfway(false)
{}

State* RingChangeState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory->has_component<inventory::InventoryDisplay>() && inventory->has_component<inventory::InventoryDuration>()) {
			const auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();
			const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();

			if (type == UP || type == DOWN) {
				close_ring(inventory_state.ring->ring, inventory_display, &inventory_duration.ring_change_frames);

				// pitch camera up or down respectively
				const auto camera = entity_manager.find_entity_with_component<camera::CameraView>();
				if (camera) {
					auto &camera_view = *camera->get_component<camera::CameraView>();

					camera->add_component(new motion::Motion(
						camera_view.rotation.x,
						0,
						type == UP ? inventory_display.camera_ring_change_pitch : -inventory_display.camera_ring_change_pitch,
						inventory_duration.ring_change_frames)
					);
				}
			}
			else if (type == FADE) {
				inventory::fade_out_ring(inventory_state.ring->ring, core::round(inventory_duration.ring_change_frames * 0.25f));
			}
		}

		// add sfx
		if (play_sound) {
			add_sfx(sound::SfxType::RING_CHANGE, false, *inventory);
		}

		clear_hud(entity_manager);
	}

	return this;
}

State* RingChangeState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (!halfway) {
		if (entities_in_motion.empty()) {
			halfway = true;

			const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

			if (inventory) {
				auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

				// switch ring
				inventory_state.ring_target = get_ring_target();
				inventory_state.change_ring();

				// change item displays
				auto items = entity_manager.find_entities_with_component<item::ItemDisplay>();
				std::for_each(items.begin(), items.end(), [&entity_manager](ecs::Entity *item) -> void {
					// except for active item
					const auto ring_item_active = get_active_item(entity_manager);
					if (ring_item_active
						&& ring_item_active->item.get_id() == item->get_id()) {
						return;
					}

					item::change_item_display(*item, item::ItemDisplayType::IDLE);
				});

				// add ring motions
				if (inventory->has_component<inventory::InventoryDisplay>() && inventory->has_component<inventory::InventoryDuration>()) {
					const auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();
					const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();

					if (type == UP || type == DOWN) {
						open_ring(inventory_state.ring->ring, inventory_display, &inventory_duration.ring_change_frames);

						// pitch camera up or down respectively, starting from opposite direction
						// this will make it appear as if the rings are stacked
						const auto camera = entity_manager.find_entity_with_component<camera::CameraView>();
						if (camera) {
							auto &camera_view = *camera->get_component<camera::CameraView>();

							camera->add_component(new motion::Motion(
								camera_view.rotation.x,
								-(type == UP ? inventory_display.camera_ring_change_pitch : -inventory_display.camera_ring_change_pitch),
								0,
								inventory_duration.ring_change_frames)
							);
						}
					}
					else if (type == FADE) {
						open_ring(inventory_state.ring->ring, inventory_display);
						inventory::fade_in_ring(inventory_state.ring->ring, core::round(inventory_duration.ring_change_frames * 0.25f));
					}
					else if (type == INSTANT) {
						open_ring(inventory_state.ring->ring, inventory_display);
						return get_next_state();
					}
				}
			}
		}
	}
	else if (entities_in_motion.empty()) {
		return get_next_state();
	}

	return this;
}

ItemActivateState::ItemActivateState()
	:
	motions_done(false)
{}

State* ItemActivateState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		const auto item_selected = get_selected_item(entity_manager);

		if (item_selected) {
			// set selected item as active
			inventory_state.item_active = item_selected;

			auto &item_active = inventory_state.item_active->item;

			uint32_t duration_frames = 0;
			const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
			if (inventory_duration) {
				duration_frames = inventory_duration->item_activate_frames;
			}

			// move to active position
			const auto item_display = item_active.get_component<item::ItemDisplay>();
			if (item_display) {
				item::change_item_display(item_active, item::ItemDisplayType::ACTIVE,
					true,
					true,
					true,
					true,
					true,
					true
				);
				const auto item_display_active = item_display->config;

				if (item_display_active) {
					item_active.add_component(new motion::Motion(
						item_display->pos.x,
						item_display->pos.x,
						item_display_active->pos.x,
						duration_frames
					));
					item_active.add_component(new motion::Motion(
						item_display->pos.y,
						item_display->pos.y,
						item_display_active->pos.y,
						duration_frames
					));
					item_active.add_component(new motion::Motion(
						item_display->pos.z,
						item_display->pos.z,
						item_display_active->pos.z,
						duration_frames
					));
					motion::add_motion_rot(
						item_active,
						item_display->orient.x,
						item_display->orient.x,
						item_display_active->orient.x,
						duration_frames
					);
					motion::add_motion_rot(
						item_active,
						item_display->orient.y,
						item_display->orient.y,
						item_display_active->orient.y,
						duration_frames
					);
					motion::add_motion_rot(
						item_active,
						item_display->orient.z,
						item_display->orient.z,
						item_display_active->orient.z,
						duration_frames
					);
					item_active.add_component(new motion::Motion(
						item_display->tilt,
						item_display->tilt,
						item_display_active->tilt,
						duration_frames
					));
					item_active.add_component(new motion::Motion(
						item_display->scale,
						item_display->scale,
						item_display_active->scale,
						duration_frames
					));
				}
			}

			item::restore_item_spin(item_active, duration_frames);

			item::change_item_model(item_active, item::ItemModelType::ACTIVE);

			// start animation, if any
			if (item_active.has_component<item::ItemAnimation>()) {
				const auto item_animation = item_active.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
					return item_animation.type == item::ItemAnimationType::ACTIVATE;
				});

				if (item_animation && !item_animation->wait_for_motions) {
					item::start_item_animation(item_active, *item_animation);
				}
			}

			// sort actions
			item_active.sort_components<item::ItemAction>([](const item::ItemAction &a, const item::ItemAction &b) -> bool {
				return a.sort_index < b.sort_index;
			});

			// activate first enabled action
			auto item_actions = item_active.get_components<item::ItemAction>([](item::ItemAction &item_action) -> bool {
				item_action.active = false;

				return item_action.enabled();
			});
			if (!item_actions.empty()) {
				item_actions.at(0)->active = true;
			}

			// add sfx
			add_sfx(sound::SfxType::ITEM_ACTIVATE, false, *inventory, &item_active);
		}

		clear_hud(entity_manager);
	}

	return this;
}

void ItemActivateState::end(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}

	const auto item_active = get_active_item(entity_manager);
	if (!item_active) {
		return;
	}

	// add active sfx
	add_sfx(sound::SfxType::ITEM_ACTIVE, false, *inventory, &item_active->item);
}

State* ItemActivateState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	auto item_active = get_active_item(entity_manager);

	if (!motions_done) {
		if (entities_in_motion.empty()) {
			motions_done = true;

			if (item_active) {
				// start animation, if any and not already
				if (item_active->item.has_component<item::ItemAnimation>()) {
					const auto item_animation = item_active->item.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
						return item_animation.type == item::ItemAnimationType::ACTIVATE;
					});

					if (item_animation && !item_animation->active) {
						item::start_item_animation(item_active->item, *item_animation);

						return this;
					}
				}
			}
		}
	}

	if (motions_done && entities_in_motion.empty()) {
		// go to special state depending on item components
		if (item_active) {
			if (item_active->item.has_component<special::PassportData>()) {
				return new PassportState();
			}
			if (item_active->item.has_component<special::MapData>()) {
				return new MapState();
			}
		}

		return new ItemActiveState(false);
	}

	return this;
}

ItemActiveState::ItemActiveState(bool enable_auto_cancel)
	:
	enable_auto_cancel(enable_auto_cancel),
	loading_or_saving(false)
{}

State* ItemActiveState::start(ecs::EntityManager &entity_manager)
{
	auto item = get_active_item(entity_manager);
	if (item) {
		auto &item_active = item->item;

		if (item_active.has_component<item::ItemData>()
			&& item_active.has_component<item::ItemQuantity>()
			&& item_active.has_component<item::ItemDisplay>()) {
			auto &item_data = *item_active.get_component<item::ItemData>();
			auto &item_display = *item_active.get_component<item::ItemDisplay>();
			const auto item_qty = item_active.get_component<item::ItemQuantity>();

			item_display.alpha_enabled = false;

			auto item_actions = item_active.get_components<item::ItemAction>([](item::ItemAction &item_action) -> bool {
				return item_action.enabled();
			});
			const auto item_actions_config = item_active.get_component<item::ItemActions>();

			// perform first action if single-action item and no-confirm
			if (item_actions.size() == 1
				&& (!item_actions_config || !item_actions_config->confirm_single_action)) {
				const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

				if (inventory) {
					const auto get_default_next_state = []() -> State* {
						return new ItemCancelState([]() -> State* {
							return new IdleState();
						});
					};

					if (enable_auto_cancel) {
						return get_default_next_state();
					}

					return do_action(
						item_active,
						*inventory,
						get_default_next_state,
						entity_manager);
				}
			}

			if (!item_active.has_component<render::ScreenText>()) {
				// add text for active item
				const auto item_text = text::build_item_text(
					text::TextType::ITEM_NAME_ACTIVE,
					entity_manager,
					item_data.name.get_string(),
					item_qty,
					false
				);
				text::add_text(
					item_active,
					text::TextType::ITEM_NAME_ACTIVE,
					item_text,
					entity_manager
				);

				const auto qty_text = text::build_item_text(
					text::TextType::ITEM_QTY_ACTIVE,
					entity_manager,
					item_data.name.get_string(),
					item_qty,
					false
				);
				auto ammo_qty_text = std::string();

				// add text for loaded ammo, if any
				if (item_active.has_component<item::ItemAmmo>()) {
					auto item_ammo = item::get_loaded_ammo(item_active);

					if (item_ammo) {
						auto &ammo_item = item_ammo->ammo_item;
						if (ammo_item.has_component<item::ItemData>()
							&& ammo_item.has_component<item::ItemQuantity>()) {
							auto &ammo_data = *ammo_item.get_component<item::ItemData>();
							const auto ammo_qty = ammo_item.get_component<item::ItemQuantity>();

							const auto ammo_text = text::build_item_text(
								text::TextType::ITEM_AMMO_ACTIVE,
								entity_manager,
								ammo_data.name.get_string(),
								ammo_qty,
								true
							);
							text::add_text(
								item_active,
								text::TextType::ITEM_AMMO_ACTIVE,
								ammo_text,
								entity_manager
							);

							ammo_qty_text = text::build_item_text(
								text::TextType::ITEM_QTY_ACTIVE,
								entity_manager,
								ammo_data.name.get_string(),
								ammo_qty,
								true
							);
						}
					}
				}

				// add text for description
				text::add_text(
					item_active,
					text::TextType::ITEM_DESC_ACTIVE,
					item_data.description.get_string(),
					entity_manager
				);

				// add text for qty
				text::add_text(
					item_active,
					text::TextType::ITEM_QTY_ACTIVE,
					qty_text,
					entity_manager
				);
				text::add_text(
					item_active,
					text::TextType::ITEM_QTY_ACTIVE,
					ammo_qty_text,
					entity_manager
				);

				// add health bar
				inventory::add_health_bar(item_active);

				// add text for action menu (add this last)
				if (!item_actions.empty()) {
					for (auto action_it = item_actions.begin(); action_it != item_actions.end(); ++action_it) {
						auto &item_action = **action_it;

						text::add_text(
							item_active,
							item_action.active ? text::TextType::ACTION_MENU_HIGHLIGHT : text::TextType::ACTION_MENU,
							item_action.name.get_string(),
							entity_manager
						);
					}
				}
			}
		}
	}

	return this;
}

State* ItemActiveState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		if (input_state.command_first_press(enumCMD.INVENTORY) && !loading_or_saving) {
			return new ItemCancelState([]() -> State* {
				return new IdleState();
			});
		}

		const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
		if (inventory) {
			auto ring_item_active = get_active_item(entity_manager);
			if (ring_item_active) {
				auto &item_active = ring_item_active->item;

				if ((input_state.command_first_press(enumCMD.UP) || input_state.command_first_press(enumCMD.DOWN)) && !loading_or_saving) {
					auto screen_texts = item_active.get_components<render::ScreenText>();

					auto item_actions = item_active.get_components<item::ItemAction>([](item::ItemAction &item_action) -> bool {
						return item_action.enabled();
					});
					for (auto action_it = item_actions.begin(); action_it != item_actions.end(); ++action_it) {
						const uint32_t index = action_it - item_actions.begin();
						auto &item_action = **action_it;

						// deactivate current action and activate next action based on direction
						// also update related text font
						if (item_action.active) {
							item::ItemAction *item_action_next = nullptr;
							auto index_next = index;

							if (input_state.command_active(enumCMD.UP)) {
								if (index == 0) {
									break;
								}
								item_action_next = *(action_it - 1);
								--index_next;
							}
							else if (input_state.command_active(enumCMD.DOWN)) {
								if (index == (item_actions.size() - 1)) {
									break;
								}
								item_action_next = *(action_it + 1);
								++index_next;
							}

							item_action.active = false;
							item_action_next->active = true;

							// action screen texts should be at end of list
							const uint32_t screen_text_offset = (screen_texts.size() - item_actions.size());
							auto &screen_text_now = *screen_texts.at(screen_text_offset + index);
							auto &screen_text_next = *screen_texts.at(screen_text_offset + index_next);

							// update font
							const auto normal_text_config = text::get_text_config(text::TextType::ACTION_MENU, entity_manager);
							if (normal_text_config) {
								screen_text_now.x = normal_text_config->x;
								screen_text_now.flags_size = normal_text_config->size;
								screen_text_now.flags_colour = normal_text_config->colour;
								screen_text_now.flags_align = normal_text_config->align;
							}

							const auto hightlight_text_config = text::get_text_config(text::TextType::ACTION_MENU_HIGHLIGHT, entity_manager);
							if (hightlight_text_config) {
								screen_text_next.x = hightlight_text_config->x;
								screen_text_next.flags_size = hightlight_text_config->size;
								screen_text_next.flags_colour = hightlight_text_config->colour;
								screen_text_next.flags_align = hightlight_text_config->align;
							}

							// add sfx
							add_sfx(sound::SfxType::MENU_CHANGE, false, *inventory, &item_active);

							break;
						}
					}
				}

				if (input_state.command_active(enumCMD.ACTION) || input_state.command_active(enumCMD.ENTER)) {
					return do_action(item_active, *inventory, [this]() -> State* { return this; }, entity_manager);
				}

				if (input_state.command_first_press(enumCMD.ROLL)) {
					if (inventory::debug_enabled(entity_manager)) {
						return new DebugState(item_active, item::ItemDisplayType::ACTIVE, []() -> State* {
							return new ItemActiveState(true);
						});
					}
				}
			}
		}
	}

	// input after saving or loading isn't properly debounced
	if (loading_or_saving) {
		loading_or_saving = false;
	}

	return this;
}

State* ItemActiveState::do_action(
	ecs::Entity& item_active,
	ecs::Entity& inventory,
	std::function<State*()> get_default_state,
	ecs::EntityManager& entity_manager
)
{
	const auto inventory_state = inventory::get_inventory_state(entity_manager);

	if (inventory_state) {
		const auto active_action = item_active.get_component<item::ItemAction>([](item::ItemAction &item_action) -> bool {
			return item_action.active;
		});

		if (active_action) {
			inventory::InventoryRing *context_ring = nullptr;
			std::function<State*()> context_state = nullptr;

			if (active_action->type == item::ItemActionType::CHOOSE_AMMO) {
				if (active_action->action) {
					active_action->action();
				}

				context_ring = inventory::build_ammo_ring(item_active, inventory, entity_manager);

				context_state = [=]() -> State* {
					return new AmmoContextState(*active_action);
				};
			}
			else if (active_action->type == item::ItemActionType::COMBINE) {
				if (active_action->action) {
					active_action->action();
				}

				context_ring = inventory::build_combo_ring(item_active, inventory, entity_manager);

				context_state = [=]() -> State* {
					return new ComboContextState(*active_action);
				};
			}
			else if (active_action->type == item::ItemActionType::SEPARATE) {
				if (active_action->action) {
					active_action->action();
				}

				return new SeparateState();
			}
			else if (active_action->type == item::ItemActionType::EXAMINE) {
				return new ExamineState();
			}
			else if (active_action->type == item::ItemActionType::LOAD_GAME) {
				loading_or_saving = true;

				if (action::load_game()) {
					return new ClosedState();
				}
			}
			else if (active_action->type == item::ItemActionType::SAVE_GAME) {
				loading_or_saving = true;

				if (action::save_game()) {
					return new ItemCancelState([]() -> State* {
						return new ClosingState(false);
					}, false, false, false, false);
				}
			}
			else if (active_action->type == item::ItemActionType::STATISTICS) {
				return new StatisticsState();
			}
			else if (active_action->type == item::ItemActionType::OPTIONS) {
				return new OptionsState();
			}
			else if (active_action->type == item::ItemActionType::CONTROLS) {
				return new OptionsState(true);
			}
			else if (active_action->type == item::ItemActionType::EXIT_TO_TITLE) {
				action::exit_to_title();

				if (active_action->instant) {
					return new ClosedState();
				}

				return new ItemCancelState([]() -> State* {
					return new ClosingState(false);
				}, false, false, false, false);
			}
			else {
				inventory_state->item_used = inventory_state->item_active;

				if (active_action->instant) {
					return new ClosedState();
				}

				return new ItemCancelState([]() -> State* {
					return new ClosingState(false);
				}, false, false, false, false);
			}

			// change to context ring for selected context action
			if (context_ring && context_state && inventory_state->ring) {
				uint32_t duration_frames = 0;
				const auto inventory_duration = inventory.get_component<inventory::InventoryDuration>();
				if (inventory_duration) {
					duration_frames = inventory_duration->item_activate_frames;
				}

				// move active item to context position
				const auto item_display = item_active.get_component<item::ItemDisplay>();
				if (item_display) {
					item::change_item_display(item_active, item::ItemDisplayType::CONTEXT,
						true,
						true,
						true,
						true,
						true,
						true,
						true
					);
					const auto item_display_context = item_display->config;

					if (item_display_context) {
						item_active.add_component(new motion::Motion(
							item_display->pos.x,
							item_display->pos.x,
							item_display_context->pos.x,
							duration_frames
						));
						item_active.add_component(new motion::Motion(
							item_display->pos.y,
							item_display->pos.y,
							item_display_context->pos.y,
							duration_frames
						));
						item_active.add_component(new motion::Motion(
							item_display->pos.z,
							item_display->pos.z,
							item_display_context->pos.z,
							duration_frames
						));
						motion::add_motion_rot(
							item_active,
							item_display->orient.x,
							item_display->orient.x,
							item_display_context->orient.x,
							duration_frames
						);
						motion::add_motion_rot(
							item_active,
							item_display->orient.y,
							item_display->orient.y,
							item_display_context->orient.y,
							duration_frames
						);
						motion::add_motion_rot(
							item_active,
							item_display->orient.z,
							item_display->orient.z,
							item_display_context->orient.z,
							duration_frames
						);
						item_active.add_component(new motion::Motion(
							item_display->tilt,
							item_display->tilt,
							item_display_context->tilt,
							duration_frames
						));
						item_active.add_component(new motion::Motion(
							item_display->scale,
							item_display->scale,
							item_display_context->scale,
							duration_frames
						));
					}
				}

				// remember ring to return to
				context_ring->prev = inventory_state->ring;

				return new RingChangeState(RingChangeState::FADE, [=]() -> inventory::InventoryRing* {return context_ring; }, context_state);
			}
		}
	}

	return get_default_state();
}

ItemCancelState::ItemCancelState(
	std::function<State*()> get_next_state,
	bool play_sfx,
	bool play_animations,
	bool restore_orientation,
	bool restore_model
)
	:
	get_next_state(get_next_state),
	play_sfx(play_sfx),
	play_animations(play_animations),
	restore_orientation(restore_orientation),
	restore_model(restore_model),
	motions_done(false)
{}

State* ItemCancelState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (inventory) {
		auto ring_item_active = get_active_item(entity_manager);
		if (ring_item_active) {
			auto &item_active = ring_item_active->item;

			// start animations, if any
			if (play_animations && item_active.has_component<item::ItemAnimation>()) {
				const auto item_animation = item_active.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
					return item_animation.type == item::ItemAnimationType::CANCEL;
				});

				if (item_animation && item_animation->wait_for_motions) {
					item::start_item_animation(item_active, *item_animation);
				}
			}

			if (play_sfx) {
				// add sfx
				add_sfx(sound::SfxType::ITEM_CANCEL, false, *inventory, &item_active);
			}

			// stop active sfx
			add_sfx(sound::SfxType::ITEM_ACTIVE, true, *inventory, &item_active);
		}

		clear_hud(entity_manager);
	}

	return this;
}

void ItemCancelState::end(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}
	auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

	auto ring_item_active = inventory_state.item_active;
	if (ring_item_active) {
		auto &item_active = ring_item_active->item;

		if (item_active.has_component<item::ItemDisplay>()) {
			item_active.get_component<item::ItemDisplay>()->alpha_enabled = true;
		}
	}

	// clear active item
	inventory_state.item_active = nullptr;

	// change item model
	if (restore_model) {
		auto ring_item_selected = get_selected_item(entity_manager);
		if (ring_item_selected) {
			auto &item_selected = ring_item_selected->item;

			item::reset_item_animations(item_selected);

			item::change_item_model(item_selected, item::ItemModelType::IDLE);
		}
	}
}

State* ItemCancelState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (!motions_done) {
		if (entities_in_motion.empty()) {
			motions_done = true;

			const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

			if (inventory) {
				auto ring_item_active = get_active_item(entity_manager);
				if (ring_item_active) {
					auto &item_active = ring_item_active->item;

					// start animations, if any and not already
					if (play_animations && item_active.has_component<item::ItemAnimation>()) {
						const auto item_animation = item_active.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation)->bool {
							return item_animation.type == item::ItemAnimationType::CANCEL;
						});

						if (item_animation && !item_animation->active) {
							item::start_item_animation(item_active, *item_animation);
						}
					}

					uint32_t duration_frames = 0;
					const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
					if (inventory_duration) {
						duration_frames = inventory_duration->item_activate_frames;
					}

					// move to idle position
					const auto item_display = item_active.get_component<item::ItemDisplay>();
					if (item_display) {
						item::change_item_display(item_active, item::ItemDisplayType::IDLE,
							true,
							true,
							true,
							true,
							true,
							true
						);
						const auto item_display_idle = item_display->config;

						if (item_display_idle) {
							item_active.add_component(new motion::Motion(
								item_display->pos.x,
								item_display->pos.x,
								item_display_idle->pos.x,
								duration_frames
							));
							item_active.add_component(new motion::Motion(
								item_display->pos.y,
								item_display->pos.y,
								item_display_idle->pos.y,
								duration_frames
							));
							item_active.add_component(new motion::Motion(
								item_display->pos.z,
								item_display->pos.z,
								item_display_idle->pos.z,
								duration_frames
							));
							if (restore_orientation) {
								motion::add_motion_rot(
									item_active,
									item_display->orient.x,
									item_display->orient.x,
									item_display_idle->orient.x,
									duration_frames
								);
								motion::add_motion_rot(
									item_active,
									item_display->orient.y,
									item_display->orient.y,
									item_display_idle->orient.y,
									duration_frames
								);
								motion::add_motion_rot(
									item_active,
									item_display->orient.z,
									item_display->orient.z,
									item_display_idle->orient.z,
									duration_frames
								);
								item_active.add_component(new motion::Motion(
									item_display->tilt,
									item_display->tilt,
									item_display_idle->tilt,
									duration_frames
								));
							}
							item_active.add_component(new motion::Motion(
								item_display->scale,
								item_display->scale,
								item_display_idle->scale,
								duration_frames
							));
						}
					}
				}
			}
		}
	}
	else if (entities_in_motion.empty()) {
		return get_next_state();
	}

	return this;
}

AmmoContextState::AmmoContextState(item::ItemAction &item_action)
	:
	item_action(item_action)
{}

State* AmmoContextState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		// add text for active item
		if (inventory_state.item_active) {
			auto &item_active = inventory_state.item_active->item;

			if (!item_active.has_component<render::ScreenText>()
				&& item_active.has_component<item::ItemData>()) {
				auto &item_data = *item_active.get_component<item::ItemData>();

				const auto item_text = text::build_item_text(
					text::TextType::ITEM_NAME_ACTIVE,
					entity_manager,
					item_data.name.get_string()
				);
				text::add_text(
					item_active,
					text::TextType::ITEM_NAME_ACTIVE,
					item_text,
					entity_manager
				);
			}
		}

		// add action text
		if (!inventory->has_component<render::ScreenText>()) {
			const auto context_string = item_action.context.get_index() < 0
				? script::ScriptString(script::StringIndex::CHOOSE_AMMO).get_string()
				: item_action.context.get_string();

			text::add_text(
				*inventory,
				text::TextType::CONTEXT_ACTION,
				context_string,
				entity_manager
			);
		}

		// add selected ammo text
		if (inventory_state.ring) {
			auto &ring = inventory_state.ring->ring;

			if (ring.has_component<ring::RingState>()) {
				auto &ring_state = *ring.get_component<ring::RingState>();

				if (ring_state.item) {
					auto &item = ring_state.item->item;

					if (!item.has_component<render::ScreenText>()
						&& item.has_component<item::ItemData>()
						&& item.has_component<item::ItemQuantity>()) {
						auto &item_data = *item.get_component<item::ItemData>();
						auto &item_qty = *item.get_component<item::ItemQuantity>();

						const auto item_text = item_action.context_hide_qty
							? text::build_item_text(text::TextType::ITEM_NAME_IDLE, entity_manager, item_data.name.get_string())
							: text::build_item_text(text::TextType::ITEM_NAME_IDLE, entity_manager, item_data.name.get_string(), &item_qty, true);

						text::add_text(
							item,
							text::TextType::ITEM_NAME_IDLE,
							item_text,
							entity_manager
						);

						if (!item_action.context_hide_qty) {
							// add text for item qty
							const auto qty_text = text::build_item_text(
								text::TextType::ITEM_QTY_IDLE,
								entity_manager,
								item_data.name.get_string(),
								&item_qty,
								true
							);
							text::add_text(
								item,
								text::TextType::ITEM_QTY_IDLE,
								qty_text,
								entity_manager
							);
						}
					}
				}
			}
		}
	}

	return this;
}

State* AmmoContextState::update(ecs::EntityManager &entity_manager)
{
	// spin selected item
	const auto inventory_duration = inventory::get_inventory_duration(entity_manager);
	const auto item_selected = get_selected_item(entity_manager);

	if (item_selected && inventory_duration) {
		item::spin_item(item_selected->item, inventory_duration->item_spin_frames);
	}

	return this;
}

State* AmmoContextState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	auto *item_action = &this->item_action;

	if (input_state.command_active(enumCMD.RIGHT)) {
		return new RingRotateState(RingRotateState::RIGHT, [=]() -> State* {
			return new AmmoContextState(*item_action);
		});
	}

	if (input_state.command_active(enumCMD.LEFT)) {
		return new RingRotateState(RingRotateState::LEFT, [=]() -> State* {
			return new AmmoContextState(*item_action);
		});
	}

	if (input_state.command_first_press(enumCMD.ACTION) || input_state.command_first_press(enumCMD.ENTER)) {
		return new AmmoLoadState();
	}

	if (input_state.command_first_press(enumCMD.INVENTORY)) {
		// switch back to main ring
		const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
		if (inventory) {
			auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

			const auto ring_context = inventory_state.ring;
			if (ring_context) {
				const auto ring_prev = ring_context->prev;

				if (ring_prev) {
					if (inventory_state.item_active) {
						auto &item_active = inventory_state.item_active->item;

						uint32_t duration_frames = 0;
						const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
						if (inventory_duration) {
							duration_frames = inventory_duration->item_activate_frames;
						}

						// move active item to active position
						const auto item_display = item_active.get_component<item::ItemDisplay>();
						if (item_display) {
							item::change_item_display(item_active, item::ItemDisplayType::ACTIVE,
								true,
								true,
								true,
								true,
								true,
								true,
								true
							);
							const auto item_display_context = item_display->config;

							if (item_display_context) {
								item_active.add_component(new motion::Motion(
									item_display->pos.x,
									item_display->pos.x,
									item_display_context->pos.x,
									duration_frames
								));
								item_active.add_component(new motion::Motion(
									item_display->pos.y,
									item_display->pos.y,
									item_display_context->pos.y,
									duration_frames
								));
								item_active.add_component(new motion::Motion(
									item_display->pos.z,
									item_display->pos.z,
									item_display_context->pos.z,
									duration_frames
								));
								motion::add_motion_rot(
									item_active,
									item_display->orient.x,
									item_display->orient.x,
									item_display_context->orient.x,
									duration_frames
								);
								motion::add_motion_rot(
									item_active,
									item_display->orient.y,
									item_display->orient.y,
									item_display_context->orient.y,
									duration_frames
								);
								motion::add_motion_rot(
									item_active,
									item_display->orient.z,
									item_display->orient.z,
									item_display_context->orient.z,
									duration_frames
								);
								item_active.add_component(new motion::Motion(
									item_display->tilt,
									item_display->tilt,
									item_display_context->tilt,
									duration_frames
								));
								item_active.add_component(new motion::Motion(
									item_display->scale,
									item_display->scale,
									item_display_context->scale,
									duration_frames
								));
							}
						}
					}

					return new RingChangeState(RingChangeState::FADE, [=]() -> inventory::InventoryRing* {return ring_prev; }, []() -> State* {
						return new ItemActiveState(true);
					});
				}
			}
		}
	}

	if (input_state.command_first_press(enumCMD.ROLL)) {
		if (inventory::debug_enabled(entity_manager)) {
			auto ring_item_active = get_active_item(entity_manager);

			if (ring_item_active) {
				return new DebugState(ring_item_active->item, item::ItemDisplayType::CONTEXT, [=]() -> State* {
					return new AmmoContextState(*item_action);
				});
			}
		}
	}

	return this;
}

State* AmmoLoadState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		const auto ring_context = inventory_state.ring;
		if (ring_context) {
			auto &ring = ring_context->ring;

			if (ring.has_component<ring::RingState>()) {
				auto &ring_state = *ring.get_component<ring::RingState>();

				if (ring_state.item) {
					auto &item_selected = ring_state.item->item;

					uint32_t duration_frames = 0;
					const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
					if (inventory_duration) {
						duration_frames = inventory_duration->item_activate_frames;
					}

					// move selected item to context position
					const auto item_display = item_selected.get_component<item::ItemDisplay>();
					if (item_display) {
						item::change_item_display(item_selected, item::ItemDisplayType::CONTEXT,
							true,
							true,
							true,
							true,
							true,
							true
						);
						const auto item_display_context = item_display->config;

						if (item_display_context) {
							item_selected.add_component(new motion::Motion(
								item_display->pos.x,
								item_display->pos.x,
								item_display_context->pos.x,
								duration_frames
							));
							item_selected.add_component(new motion::Motion(
								item_display->pos.y,
								item_display->pos.y,
								item_display_context->pos.y,
								duration_frames
							));
							item_selected.add_component(new motion::Motion(
								item_display->pos.z,
								item_display->pos.z,
								item_display_context->pos.z,
								duration_frames
							));
							motion::add_motion_rot(
								item_selected,
								item_display->orient.x,
								item_display->orient.x,
								item_display_context->orient.x,
								duration_frames
							);
							motion::add_motion_rot(
								item_selected,
								item_display->orient.y,
								item_display->orient.y,
								item_display_context->orient.y,
								duration_frames
							);
							motion::add_motion_rot(
								item_selected,
								item_display->orient.z,
								item_display->orient.z,
								item_display_context->orient.z,
								duration_frames
							);
							item_selected.add_component(new motion::Motion(
								item_display->tilt,
								item_display->tilt,
								item_display_context->tilt,
								duration_frames
							));
							item_selected.add_component(new motion::Motion(
								item_display->scale,
								item_display->scale,
								item_display_context->scale,
								duration_frames
							));
						}
					}

					item::restore_item_spin(item_selected, duration_frames);
				}
			}
		}
	}

	return this;
}

State* AmmoLoadState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
		if (inventory) {
			auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

			if (inventory_state.item_active) {
				auto &weapon_item = inventory_state.item_active->item;

				const auto ring_context = inventory_state.ring;
				if (ring_context) {
					// unload current ammo before loading
					item::unload_ammo(weapon_item);

					// load ammo
					if (ring_context->ring.has_component<ring::RingState>()) {
						auto &ring_state = *ring_context->ring.get_component<ring::RingState>();

						if (ring_state.item) {
							auto &ammo_item = ring_state.item->item;

							if (ammo_item.has_component<item::ItemData>()) {
								const auto ammo_item_id = ammo_item.get_component<item::ItemData>()->item_id;

								if (item::load_ammo(weapon_item, ammo_item_id)) {
									add_sfx(sound::SfxType::LOAD_AMMO, false, *inventory, &weapon_item);
								}
							}
						}
					}

					// switch back to main ring
					const auto ring_prev = ring_context->prev;
					if (ring_prev) {
						return new RingChangeState(RingChangeState::FADE, [ring_prev]() -> inventory::InventoryRing* {return ring_prev; }, [inventory, &entity_manager]() -> State* {
							auto ring_item_active = get_active_item(entity_manager);

							if (ring_item_active) {
								auto &item_active = ring_item_active->item;

								uint32_t duration_frames = 0;
								const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
								if (inventory_duration) {
									duration_frames = inventory_duration->item_activate_frames;
								}

								// move active item to active position
								const auto item_display = item_active.get_component<item::ItemDisplay>();
								if (item_display) {
									item::change_item_display(item_active, item::ItemDisplayType::ACTIVE,
										true,
										true,
										true,
										true,
										true,
										true,
										true
									);
									const auto item_display_context = item_display->config;

									if (item_display_context) {
										item_active.add_component(new motion::Motion(
											item_display->pos.x,
											item_display->pos.x,
											item_display_context->pos.x,
											duration_frames
										));
										item_active.add_component(new motion::Motion(
											item_display->pos.y,
											item_display->pos.y,
											item_display_context->pos.y,
											duration_frames
										));
										item_active.add_component(new motion::Motion(
											item_display->pos.z,
											item_display->pos.z,
											item_display_context->pos.z,
											duration_frames
										));
										motion::add_motion_rot(
											item_active,
											item_display->orient.x,
											item_display->orient.x,
											item_display_context->orient.x,
											duration_frames
										);
										motion::add_motion_rot(
											item_active,
											item_display->orient.y,
											item_display->orient.y,
											item_display_context->orient.y,
											duration_frames
										);
										motion::add_motion_rot(
											item_active,
											item_display->orient.z,
											item_display->orient.z,
											item_display_context->orient.z,
											duration_frames
										);
										item_active.add_component(new motion::Motion(
											item_display->tilt,
											item_display->tilt,
											item_display_context->tilt,
											duration_frames
										));
										item_active.add_component(new motion::Motion(
											item_display->scale,
											item_display->scale,
											item_display_context->scale,
											duration_frames
										));
									}
								}
							}

							return new ItemActiveState(true);
						}, false);
					}
				}
			}
		}
	}

	return this;
}

ComboContextState::ComboContextState(item::ItemAction &item_action)
	:
	item_action(item_action)
{}

State* ComboContextState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		// add text for active item
		if (inventory_state.item_active) {
			auto &item_active = inventory_state.item_active->item;

			if (!item_active.has_component<render::ScreenText>()
				&& item_active.has_component<item::ItemData>()) {
				auto &item_data = *item_active.get_component<item::ItemData>();

				const auto item_text = text::build_item_text(
					text::TextType::ITEM_NAME_ACTIVE,
					entity_manager,
					item_data.name.get_string()
				);
				text::add_text(
					item_active,
					text::TextType::ITEM_NAME_ACTIVE,
					item_text,
					entity_manager
				);
			}
		}

		// add action text
		if (!inventory->has_component<render::ScreenText>()) {
			const auto context_string = item_action.context.get_index() < 0
				? script::ScriptString(script::StringIndex::COMBINE_WITH).get_string()
				: item_action.context.get_string();

			text::add_text(
				*inventory,
				text::TextType::CONTEXT_ACTION,
				context_string,
				entity_manager
			);
		}

		// add selected item text
		if (inventory_state.ring) {
			auto &ring = inventory_state.ring->ring;

			if (ring.has_component<ring::RingState>()) {
				auto &ring_state = *ring.get_component<ring::RingState>();

				if (ring_state.item) {
					auto &item = ring_state.item->item;

					if (!item.has_component<render::ScreenText>()
						&& item.has_component<item::ItemData>()) {
						auto &item_data = *item.get_component<item::ItemData>();
						auto &item_qty = *item.get_component<item::ItemQuantity>();

						const auto item_text = item_action.context_hide_qty
							? text::build_item_text(text::TextType::ITEM_NAME_IDLE, entity_manager, item_data.name.get_string())
							: text::build_item_text(text::TextType::ITEM_NAME_IDLE, entity_manager, item_data.name.get_string(), &item_qty);

						text::add_text(
							item,
							text::TextType::ITEM_NAME_IDLE,
							item_text,
							entity_manager
						);

						if (!item_action.context_hide_qty) {
							// add text for item qty
							const auto qty_text = text::build_item_text(
								text::TextType::ITEM_QTY_IDLE,
								entity_manager,
								item_data.name.get_string(),
								&item_qty
							);
							text::add_text(
								item,
								text::TextType::ITEM_QTY_IDLE,
								qty_text,
								entity_manager
							);
						}
					}
				}
			}
		}
	}

	return this;
}

State* ComboContextState::update(ecs::EntityManager &entity_manager)
{
	// spin selected item
	const auto inventory_duration = inventory::get_inventory_duration(entity_manager);
	const auto item_selected = get_selected_item(entity_manager);

	if (item_selected && inventory_duration) {
		item::spin_item(item_selected->item, inventory_duration->item_spin_frames);
	}

	return this;
}

State* ComboContextState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	auto *item_action = &this->item_action;

	if (input_state.command_active(enumCMD.RIGHT)) {
		return new RingRotateState(RingRotateState::RIGHT, [=]() -> State* {
			return new ComboContextState(*item_action);
		});
	}

	if (input_state.command_active(enumCMD.LEFT)) {
		return new RingRotateState(RingRotateState::LEFT, [=]() -> State* {
			return new ComboContextState(*item_action);
		});
	}

	if (input_state.command_first_press(enumCMD.ACTION) || input_state.command_first_press(enumCMD.ENTER)) {
		return new ComboTryState();
	}

	if (input_state.command_first_press(enumCMD.INVENTORY)) {
		// switch back to main ring
		const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
		if (inventory) {
			auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

			const auto ring_context = inventory_state.ring;
			if (ring_context) {
				const auto ring_prev = ring_context->prev;

				if (ring_prev) {
					if (inventory_state.item_active) {
						auto &item_active = inventory_state.item_active->item;

						uint32_t duration_frames = 0;
						const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
						if (inventory_duration) {
							duration_frames = inventory_duration->item_activate_frames;
						}

						// move active item to active position
						const auto item_display = item_active.get_component<item::ItemDisplay>();
						if (item_display) {
							item::change_item_display(item_active, item::ItemDisplayType::ACTIVE,
								true,
								true,
								true,
								true,
								true,
								true,
								true
							);
							const auto item_display_context = item_display->config;

							if (item_display_context) {
								item_active.add_component(new motion::Motion(
									item_display->pos.x,
									item_display->pos.x,
									item_display_context->pos.x,
									duration_frames
								));
								item_active.add_component(new motion::Motion(
									item_display->pos.y,
									item_display->pos.y,
									item_display_context->pos.y,
									duration_frames
								));
								item_active.add_component(new motion::Motion(
									item_display->pos.z,
									item_display->pos.z,
									item_display_context->pos.z,
									duration_frames
								));
								motion::add_motion_rot(
									item_active,
									item_display->orient.x,
									item_display->orient.x,
									item_display_context->orient.x,
									duration_frames
								);
								motion::add_motion_rot(
									item_active,
									item_display->orient.y,
									item_display->orient.y,
									item_display_context->orient.y,
									duration_frames
								);
								motion::add_motion_rot(
									item_active,
									item_display->orient.z,
									item_display->orient.z,
									item_display_context->orient.z,
									duration_frames
								);
								item_active.add_component(new motion::Motion(
									item_display->tilt,
									item_display->tilt,
									item_display_context->tilt,
									duration_frames
								));
								item_active.add_component(new motion::Motion(
									item_display->scale,
									item_display->scale,
									item_display_context->scale,
									duration_frames
								));
							}
						}
					}

					return new RingChangeState(RingChangeState::FADE, [=]() -> inventory::InventoryRing* {return ring_prev; }, []() -> State* {
						return new ItemActiveState(true);
					});
				}
			}
		}
	}

	if (input_state.command_first_press(enumCMD.ROLL)) {
		if (inventory::debug_enabled(entity_manager)) {
			auto ring_item_active = get_active_item(entity_manager);

			if (ring_item_active) {
				return new DebugState(ring_item_active->item, item::ItemDisplayType::CONTEXT, [=]() -> State* {
					return new ComboContextState(*item_action);
				});
			}
		}
	}

	return this;
}

State* ComboTryState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		const auto ring_context = inventory_state.ring;
		if (ring_context) {
			auto &ring = ring_context->ring;

			if (ring.has_component<ring::RingState>()) {
				auto &ring_state = *ring.get_component<ring::RingState>();

				if (ring_state.item) {
					auto &item_selected = ring_state.item->item;

					uint32_t duration_frames = 0;
					const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
					if (inventory_duration) {
						duration_frames = inventory_duration->item_activate_frames;
					}

					// move selected item to context position
					const auto item_display = item_selected.get_component<item::ItemDisplay>();
					if (item_display) {
						item::change_item_display(item_selected, item::ItemDisplayType::CONTEXT,
							true,
							true,
							true,
							true,
							true,
							true
						);
						const auto item_display_context = item_display->config;

						if (item_display_context) {
							item_selected.add_component(new motion::Motion(
								item_display->pos.x,
								item_display->pos.x,
								item_display_context->pos.x,
								duration_frames
							));
							item_selected.add_component(new motion::Motion(
								item_display->pos.y,
								item_display->pos.y,
								item_display_context->pos.y,
								duration_frames
							));
							item_selected.add_component(new motion::Motion(
								item_display->pos.z,
								item_display->pos.z,
								item_display_context->pos.z,
								duration_frames
							));
							motion::add_motion_rot(
								item_selected,
								item_display->orient.x,
								item_display->orient.x,
								item_display_context->orient.x,
								duration_frames
							);
							motion::add_motion_rot(
								item_selected,
								item_display->orient.y,
								item_display->orient.y,
								item_display_context->orient.y,
								duration_frames
							);
							motion::add_motion_rot(
								item_selected,
								item_display->orient.z,
								item_display->orient.z,
								item_display_context->orient.z,
								duration_frames
							);
							item_selected.add_component(new motion::Motion(
								item_display->tilt,
								item_display->tilt,
								item_display_context->tilt,
								duration_frames
							));
							item_selected.add_component(new motion::Motion(
								item_display->scale,
								item_display->scale,
								item_display_context->scale,
								duration_frames
							));
						}
					}

					item::restore_item_spin(item_selected, duration_frames);
				}
			}
		}
	}

	return this;
}

State* ComboTryState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
		if (inventory) {
			auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

			if (inventory_state.item_active) {
				auto &item_active = inventory_state.item_active->item;

				const auto ring_context = inventory_state.ring;
				if (ring_context) {
					if (ring_context->ring.has_component<ring::RingState>()) {
						auto &ring_state = *ring_context->ring.get_component<ring::RingState>();

						if (ring_state.item) {
							// combine if combinable
							auto &item_selected = ring_state.item->item;

							const auto combo_data = inventory->get_component<item::ComboData>([&](item::ComboData &combo) -> bool {
								return (combo.enabled
									&& combo.combine
									&& ((combo.item_first.get_id() == item_active.get_id()
										&& combo.item_second.get_id() == item_selected.get_id())
										|| (combo.vice_versa
											&& combo.item_first.get_id() == item_selected.get_id()
											&& combo.item_second.get_id() == item_active.get_id())));
							});

							if (combo_data) {
								combo_data->combine(*combo_data);

								// change to ring of final item
								auto item_final = combo_data->item_final;
								int32_t ring_id = ring::RingId::INVENTORY;
								int32_t item_id = item::ItemId::NONE;

								if (item_final
									&& item_final->has_component<item::ItemData>()
									&& item_final->has_component<item::ItemRing>()) {
									auto &item_data = *item_final->get_component<item::ItemData>();
									auto &item_ring = *item_final->get_component<item::ItemRing>();
									ring_id = item_ring.ring_id;
									item_id = item_data.item_id;
								}

								const auto get_next_ring = [=, &inventory_state, &entity_manager]() -> inventory::InventoryRing* {
									auto ring_item_active = get_active_item(entity_manager);
									if (ring_item_active) {
										// add sfx
										add_sfx(sound::SfxType::COMBINE, false, *inventory, &ring_item_active->item);
									}

									// rebuild inventory
									build_inventory(ring_id, item_id, inventory_state, entity_manager);

									return inventory_state.ring;
								};

								if (item_selected.has_component<item::ItemDisplay>()) {
									item_selected.get_component<item::ItemDisplay>()->alpha_enabled = false;
								}

								return new RingChangeState(RingChangeState::INSTANT, get_next_ring, []() -> State* {
									return new IdleState();
								}, false);
							}
							else {
								if (ring_context->prev) {
									// change to previous ring
									const auto get_next_ring = [=]() -> inventory::InventoryRing* {
										// add sfx
										add_sfx(sound::SfxType::FAIL, false, *inventory);

										return ring_context->prev;
									};

									return new RingChangeState(RingChangeState::FADE, get_next_ring, [inventory, &entity_manager]() -> State* {
										auto ring_item_active = get_active_item(entity_manager);

										if (ring_item_active) {
											auto &item_active = ring_item_active->item;

											uint32_t duration_frames = 0;
											const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
											if (inventory_duration) {
												duration_frames = inventory_duration->item_activate_frames;
											}

											// move active item to active position
											const auto item_display = item_active.get_component<item::ItemDisplay>();
											if (item_display) {
												item::change_item_display(item_active, item::ItemDisplayType::ACTIVE,
													true,
													true,
													true,
													true,
													true,
													true,
													true
												);
												const auto item_display_context = item_display->config;

												if (item_display_context) {
													item_active.add_component(new motion::Motion(
														item_display->pos.x,
														item_display->pos.x,
														item_display_context->pos.x,
														duration_frames
													));
													item_active.add_component(new motion::Motion(
														item_display->pos.y,
														item_display->pos.y,
														item_display_context->pos.y,
														duration_frames
													));
													item_active.add_component(new motion::Motion(
														item_display->pos.z,
														item_display->pos.z,
														item_display_context->pos.z,
														duration_frames
													));
													motion::add_motion_rot(
														item_active,
														item_display->orient.x,
														item_display->orient.x,
														item_display_context->orient.x,
														duration_frames
													);
													motion::add_motion_rot(
														item_active,
														item_display->orient.y,
														item_display->orient.y,
														item_display_context->orient.y,
														duration_frames
													);
													motion::add_motion_rot(
														item_active,
														item_display->orient.z,
														item_display->orient.z,
														item_display_context->orient.z,
														duration_frames
													);
													item_active.add_component(new motion::Motion(
														item_display->tilt,
														item_display->tilt,
														item_display_context->tilt,
														duration_frames
													));
													item_active.add_component(new motion::Motion(
														item_display->scale,
														item_display->scale,
														item_display_context->scale,
														duration_frames
													));
												}
											}
										}

										return new ItemActiveState(true);
									}, false);
								}
							}
						}
					}
				}
			}
		}
	}

	return this;
}

State* SeparateState::update(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory_state.item_active) {
			auto &item_active = inventory_state.item_active->item;

			const auto combo_data = inventory->get_component<item::ComboData>([&](item::ComboData &combo) -> bool {
				return (combo.separable
					&& combo.separate
					&& combo.item_final
					&& combo.item_final->get_id() == item_active.get_id());
			});

			if (combo_data) {
				combo_data->separate(*combo_data);

				// change to ring of first item
				auto &item_first = combo_data->item_first;
				int32_t ring_id = ring::RingId::INVENTORY;
				int32_t item_id = item::ItemId::NONE;
				if (item_first.has_component<item::ItemData>()
					&& item_first.has_component<item::ItemRing>()) {
					auto &item_data = *item_first.get_component<item::ItemData>();
					auto &item_ring = *item_first.get_component<item::ItemRing>();
					ring_id = item_ring.ring_id;
					item_id = item_data.item_id;
				}

				const auto get_next_ring = [=, &inventory_state, &entity_manager]() -> inventory::InventoryRing* {
					auto ring_item_active = get_active_item(entity_manager);
					if (ring_item_active) {
						// add sfx
						add_sfx(sound::SfxType::SEPARATE, false, *inventory, &ring_item_active->item);
					}

					// rebuild inventory
					build_inventory(ring_id, item_id, inventory_state, entity_manager);

					return inventory_state.ring;
				};

				return new RingChangeState(RingChangeState::FADE, get_next_ring, []() -> State* {
					return new IdleState();
				}, false);
			}

		}
	}

	return this;
}

State* ExamineState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory_state.item_active) {
			auto &item_active = inventory_state.item_active->item;

			uint32_t duration_frames = 0;
			const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
			if (inventory_duration) {
				duration_frames = inventory_duration->item_activate_frames;
			}

			// move item to examine position
			const auto item_display = item_active.get_component<item::ItemDisplay>();
			if (item_display) {
				item::change_item_display(item_active, item::ItemDisplayType::EXAMINE,
					true,
					true,
					true,
					true,
					true,
					true,
					true
				);
				const auto item_display_examine = item_display->config;

				if (item_display_examine) {
					item_active.add_component(new motion::Motion(
						item_display->pos.x,
						item_display->pos.x,
						item_display_examine->pos.x,
						duration_frames
					));
					item_active.add_component(new motion::Motion(
						item_display->pos.y,
						item_display->pos.y,
						item_display_examine->pos.y,
						duration_frames
					));
					item_active.add_component(new motion::Motion(
						item_display->pos.z,
						item_display->pos.z,
						item_display_examine->pos.z,
						duration_frames
					));
					motion::add_motion_rot(
						item_active,
						item_display->orient.x,
						item_display->orient.x,
						item_display_examine->orient.x,
						duration_frames
					);
					motion::add_motion_rot(
						item_active,
						item_display->orient.y,
						item_display->orient.y,
						item_display_examine->orient.y,
						duration_frames
					);
					motion::add_motion_rot(
						item_active,
						item_display->orient.z,
						item_display->orient.z,
						item_display_examine->orient.z,
						duration_frames
					);
					item_active.add_component(new motion::Motion(
						item_display->tilt,
						item_display->tilt,
						item_display_examine->tilt,
						duration_frames
					));
					item_active.add_component(new motion::Motion(
						item_display->scale,
						item_display->scale,
						item_display_examine->scale,
						duration_frames
					));
				}
			}

			item::restore_item_spin(item_active, duration_frames);

			inventory::fade_out_ring(inventory_state.ring->ring, core::round(duration_frames * 0.25f));

			clear_hud(entity_manager);

			// add texts for examine
			if (item_active.has_component<item::ExamineData>()) {
				auto &examine_data = *item_active.get_component<item::ExamineData>();

				auto text_1 = examine_data.text_1.get_string();
				auto text_2 = examine_data.text_2.get_string();
				auto text_3 = examine_data.text_3.get_string();

				if (!text_1.empty()) {
					text::add_text(
						item_active,
						text::TextType::EXAMINE_1,
						text_1,
						entity_manager
					);
				}

				if (!text_2.empty()) {
					text::add_text(
						item_active,
						text::TextType::EXAMINE_2,
						text_2,
						entity_manager
					);
				}

				if (!text_3.empty()) {
					text::add_text(
						item_active,
						text::TextType::EXAMINE_3,
						text_3,
						entity_manager
					);
				}
			}
		}
	}

	return this;
}

State* ExamineState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		if (input_state.command_first_press(enumCMD.INVENTORY)) {
			const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

			if (inventory) {
				auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

				if (inventory_state.item_active) {
					auto &item_active = inventory_state.item_active->item;

					uint32_t duration_frames = 0;
					const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
					if (inventory_duration) {
						duration_frames = inventory_duration->item_activate_frames;
					}

					// restore item back to active position
					const auto item_display = item_active.get_component<item::ItemDisplay>();
					if (item_display) {
						item::change_item_display(item_active, item::ItemDisplayType::ACTIVE,
							true,
							true,
							true,
							true,
							true,
							false,
							true
						);
						const auto item_display_active = item_display->config;

						if (item_display_active) {
							item_active.add_component(new motion::Motion(
								item_display->pos.x,
								item_display->pos.x,
								item_display_active->pos.x,
								duration_frames
							));
							item_active.add_component(new motion::Motion(
								item_display->pos.y,
								item_display->pos.y,
								item_display_active->pos.y,
								duration_frames
							));
							item_active.add_component(new motion::Motion(
								item_display->pos.z,
								item_display->pos.z,
								item_display_active->pos.z,
								duration_frames
							));
							motion::add_motion_rot(
								item_active,
								item_display->orient.x,
								item_display->orient.x,
								item_display_active->orient.x,
								duration_frames
							);
							motion::add_motion_rot(
								item_active,
								item_display->orient.y,
								item_display->orient.y,
								item_display_active->orient.y,
								duration_frames
							);
							motion::add_motion_rot(
								item_active,
								item_display->orient.z,
								item_display->orient.z,
								item_display_active->orient.z,
								duration_frames
							);
							motion::add_motion_rot(
								item_active,
								item_display->rot.x,
								item_display->rot.x,
								0,
								duration_frames
							);
							motion::add_motion_rot(
								item_active,
								item_display->rot.y,
								item_display->rot.y,
								0,
								duration_frames
							);
							motion::add_motion_rot(
								item_active,
								item_display->rot.z,
								item_display->rot.z,
								0,
								duration_frames
							);
							item_active.add_component(new motion::Motion(
								item_display->tilt,
								item_display->tilt,
								item_display_active->tilt,
								duration_frames
							));
							item_active.add_component(new motion::Motion(
								item_display->scale,
								item_display->scale,
								item_display_active->scale,
								duration_frames
							));
						}
					}

					inventory::fade_in_ring(inventory_state.ring->ring, core::round(duration_frames * 0.25f));

					clear_hud(entity_manager);

					return new ItemActiveState(true);
				}
			}
		}

		auto ring_item_active = get_active_item(entity_manager);
		if (ring_item_active) {
			auto &item_active = ring_item_active->item;

			if (input_state.command_first_press(enumCMD.ROLL)) {
				if (inventory::debug_enabled(entity_manager)) {
					return new DebugState(item_active, item::ItemDisplayType::EXAMINE, []() -> State* {
						return new ExamineState();
					});
				}
			}

			if (item_active.has_component<item::ItemDisplay>()) {
				auto &item_display = *item_active.get_component<item::ItemDisplay>();

				// rotate/zoom item based on input
				if (input_state.command_active(enumCMD.LEFT)) {
					item_display.rot.y -= 5;
				}
				if (input_state.command_active(enumCMD.RIGHT)) {
					item_display.rot.y += 5;
				}
				if (input_state.command_active(enumCMD.UP)) {
					item_display.rot.x += 5;
				}
				if (input_state.command_active(enumCMD.DOWN)) {
					item_display.rot.x -= 5;
				}
				if (input_state.command_active(enumCMD.DUCK) && item_display.scale < 3.0) {
					item_display.scale += 0.05f;
				}
				if (input_state.command_active(enumCMD.DASH) && item_display.scale > 1.0) {
					item_display.scale -= 0.05f;
				}

				core::wrap_angle(item_display.rot.x);
				core::wrap_angle(item_display.rot.y);
				core::wrap_angle(item_display.rot.z);
			}
		}
	}

	return this;
}

State* StatisticsState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		uint32_t duration_frames = 0;
		const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
		if (inventory_duration) {
			duration_frames = inventory_duration->item_activate_frames;
		}

		inventory::fade_out_ring(inventory_state.ring->ring, core::round(duration_frames * 0.25f));

		clear_hud(entity_manager);

		inventory->add_component(new render::StatisticsScreen());
	}

	return this;
}

void StatisticsState::end(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}
	auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

	uint32_t duration_frames = 0;
	const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
	if (inventory_duration) {
		duration_frames = inventory_duration->item_activate_frames;
	}

	inventory::fade_in_ring(inventory_state.ring->ring, core::round(duration_frames * 0.25f));

	clear_hud(entity_manager);

	inventory->remove_components<render::StatisticsScreen>();
}

State* StatisticsState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	if (input_state.command_first_press(enumCMD.INVENTORY)) {
		return new ItemActiveState(true);
	}

	return this;
}

OptionsState::OptionsState(bool controls)
	:
	controls(controls)
{}

State* OptionsState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		uint32_t duration_frames = 0;
		const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
		if (inventory_duration) {
			duration_frames = inventory_duration->item_activate_frames;
		}

		inventory::fade_out_ring(inventory_state.ring->ring, core::round(duration_frames * 0.25f));

		clear_hud(entity_manager);

		inventory->add_component(new render::OptionsScreen(controls));
	}

	return this;
}

void OptionsState::end(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}
	auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

	uint32_t duration_frames = 0;
	const auto inventory_duration = inventory->get_component<inventory::InventoryDuration>();
	if (inventory_duration) {
		duration_frames = inventory_duration->item_activate_frames;
	}

	inventory::fade_in_ring(inventory_state.ring->ring, core::round(duration_frames * 0.25f));

	clear_hud(entity_manager);

	inventory->remove_components<render::OptionsScreen>();
}

State* OptionsState::update(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<render::OptionsScreen>();
	if (inventory) {
		auto &options_screen = *inventory->get_component<render::OptionsScreen>();

		// tr4 options screen will handle input and flag when done
		if (options_screen.finished) {
			return new ItemActiveState(true);
		}
	}

	return this;
}

PassportState::PassportState()
	:
	loading_or_saving(false),
	page_changed(false),
	closing(false)
{
	get_post_closing_state = []() -> State* {
		return new ItemCancelState([]() -> State* {
			return new IdleState();
		}, true, false);
	};
}

State* PassportState::start(ecs::EntityManager &entity_manager)
{
	auto ring_item_active = get_active_item(entity_manager);

	if (ring_item_active) {
		auto &item_active = ring_item_active->item;

		if (item_active.has_component<special::PassportData>()) {
			auto &passport_data = *item_active.get_component<special::PassportData>();

			// open on first page
			passport_data.page = 1;
		}
	}

	return this;
}

State* PassportState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		auto ring_item_active = get_active_item(entity_manager);

		if (ring_item_active) {
			auto &item_active = ring_item_active->item;

			if (item_active.has_component<item::ItemData>()
				&& item_active.has_component<special::PassportData>()) {
				auto &item_data = *item_active.get_component<item::ItemData>();
				auto &passport_data = *item_active.get_component<special::PassportData>();

				if (closing) {
					// passport is busy closing
					clear_hud(entity_manager);

					const auto find_item_anim = [](const ecs::Entity &item, item::ItemAnimationType::Enum anim_type) -> item::ItemAnimation* {
						return item.get_component<item::ItemAnimation>([&](item::ItemAnimation &anim) -> bool {
							return anim.type == anim_type;
						});
					};

					// page count is hardcoded for now
					// page 1 and 2 will reverse animations back to front cover
					// page 3 will play normal closing animation
					const auto passport_anim_open = find_item_anim(item_active, item::ItemAnimationType::ACTIVATE);
					const auto passport_anim_page2 = find_item_anim(item_active, item::ItemAnimationType::PASSPORT_PAGE2);
					const auto passport_anim_close = find_item_anim(item_active, item::ItemAnimationType::CANCEL);

					item::ItemAnimation *anim_to_play = nullptr;
					bool reverse = false;

					switch (passport_data.page) {
					case 1:
						anim_to_play = passport_anim_open;
						reverse = true;
						--passport_data.page;
						break;
					case 2:
						anim_to_play = passport_anim_page2;
						reverse = true;
						--passport_data.page;
						break;
					case 3:
						anim_to_play = passport_anim_close;
						passport_data.page = 0;
						break;
					}

					if (anim_to_play) {
						item::reset_item_animations(item_active);

						// add animation for page flip
						const auto frame_start = reverse ? anim_to_play->frame_end : anim_to_play->frame_start;
						const auto frame_end = reverse ? anim_to_play->frame_start : anim_to_play->frame_end;

						anim_to_play->active = true;
						item_active.add_component(new motion::Motion(
							anim_to_play->frame,
							frame_start,
							frame_end,
							abs(core::round(frame_end - frame_start))
						));

						// add sfx for page flip
						if (passport_data.page != 0 && passport_data.page_sound_id >= 0) {
							item_active.add_component(new sound::SoundFX(passport_data.page_sound_id));
						}

						return this;
					}

					// passport is closed
					return get_post_closing_state();
				}

				if (page_changed) {
					// remove text
					entity_manager.remove_components<render::ScreenText>();
				}

				if (!item_active.has_component<render::ScreenText>()) {
					// add text for item name
					const auto item_text = text::build_item_text(
						text::TextType::ITEM_NAME_ACTIVE,
						entity_manager,
						item_data.name.get_string()
					);
					text::add_text(
						item_active,
						text::TextType::ITEM_NAME_ACTIVE,
						item_text,
						entity_manager
					);

					// add text for page action
					auto passport_actions = item_active.get_components<item::ItemAction>([](item::ItemAction &item_action) -> bool {
						return item_action.enabled();
					});
					if (passport_data.page > 0 && static_cast<int32_t>(passport_actions.size()) >= passport_data.page) {
						const auto action = passport_actions.at(passport_data.page - 1);

						text::add_text(
							item_active,
							text::TextType::SPECIAL_ACTION,
							action->name.get_string(),
							entity_manager
						);
					}
				}

				const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
				if (inventory && !inventory->has_component<render::ScreenText>()) {
					// add text for navigation

					if (passport_data.page > 1) {
						const auto glyph_left = "\x84";

						text::add_text(
							*inventory,
							text::TextType::NAV_LEFT,
							glyph_left,
							entity_manager
						);
					}
					if (passport_data.page < 3) {
						const auto glyph_right = "\x86";

						text::add_text(
							*inventory,
							text::TextType::NAV_RIGHT,
							glyph_right,
							entity_manager
						);
					}
				}
			}
		}
	}

	return this;
}

State* PassportState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty() && !closing) {
		auto ring_item_active = get_active_item(entity_manager);

		if (ring_item_active) {
			auto &item_active = ring_item_active->item;

			if (item_active.has_component<special::PassportData>()) {
				auto &passport_data = *item_active.get_component<special::PassportData>();

				const auto find_item_anim = [](const ecs::Entity &item, item::ItemAnimationType::Enum anim_type) -> item::ItemAnimation* {
					return item.get_component<item::ItemAnimation>([&](item::ItemAnimation &anim) -> bool {
						return anim.type == anim_type;
					});
				};

				// page count is hardcoded for now
				const auto passport_anim_page2 = find_item_anim(item_active, item::ItemAnimationType::PASSPORT_PAGE2);
				const auto passport_anim_page3 = find_item_anim(item_active, item::ItemAnimationType::PASSPORT_PAGE3);

				item::ItemAnimation *anim_to_play = nullptr;
				bool reverse = false;

				// turn page depending on direction
				if (input_state.command_first_press(enumCMD.RIGHT)) {
					if (passport_data.page < 3) {
						switch (passport_data.page) {
						case 1:
							anim_to_play = passport_anim_page2;
							break;
						case 2:
							anim_to_play = passport_anim_page3;
							break;
						}

						++passport_data.page;
						page_changed = true;
					}
				}
				else if (input_state.command_first_press(enumCMD.LEFT)) {
					if (passport_data.page > 1) {
						switch (passport_data.page) {
						case 3:
							anim_to_play = passport_anim_page3;
							break;
						case 2:
							anim_to_play = passport_anim_page2;
							break;
						}
						reverse = true;

						--passport_data.page;
						page_changed = true;
					}
				}
				// perform action
				else if (input_state.command_first_press(enumCMD.ACTION) || input_state.command_first_press(enumCMD.ENTER)) {
					auto passport_actions = item_active.get_components<item::ItemAction>([](item::ItemAction &item_action) -> bool {
						return item_action.enabled();
					});

					if (passport_data.page > 0 && static_cast<int32_t>(passport_actions.size()) >= passport_data.page) {
						const auto action = passport_actions.at(passport_data.page - 1);

						if (action->type == item::ItemActionType::LOAD_GAME) {
							loading_or_saving = true;

							if (action::load_game()) {
								return new ClosedState();
							}
						}
						else if (action->type == item::ItemActionType::SAVE_GAME) {
							loading_or_saving = true;

							if (action::save_game()) {
								get_post_closing_state = []() -> State* {
									return new ItemCancelState([]() -> State* {
										return new ClosingState(false);
									}, false, false);
								};
								closing = true;
							}
						}
						else if (action->type == item::ItemActionType::EXIT_TO_TITLE) {
							action::exit_to_title();

							if (action->instant) {
								return new ClosedState();
							}

							get_post_closing_state = []() -> State* {
								return new ItemCancelState([]() -> State* {
									return new ClosingState(false);
								}, false, false);
							};
							closing = true;
						}
						else {
							item::deactivate_item_actions(item_active);

							action->active = true;

							const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
							if (inventory) {
								auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

								inventory_state.item_used = inventory_state.item_active;
							}

							if (action->instant) {
								return new ClosedState();
							}

							get_post_closing_state = []() -> State* {
								return new ItemCancelState([]() -> State* {
									return new ClosingState(false);
								}, false, false);
							};
							closing = true;
						}
					}
				}
				// close passport
				else if (input_state.command_first_press(enumCMD.INVENTORY)) {
					closing = true;
				}
				// debug
				else if (input_state.command_first_press(enumCMD.ROLL)) {
					if (inventory::debug_enabled(entity_manager)) {
						return new DebugState(item_active, item::ItemDisplayType::ACTIVE, []() -> State* {
							return new PassportState();
						});
					}
				}

				if (anim_to_play) {
					// add sfx for page flip
					if (passport_data.page_sound_id >= 0) {
						item_active.add_component(new sound::SoundFX(passport_data.page_sound_id));
					}

					// add animation for page flip
					item::reset_item_animations(item_active);

					const auto frame_start = reverse ? anim_to_play->frame_end : anim_to_play->frame_start;
					const auto frame_end = reverse ? anim_to_play->frame_start : anim_to_play->frame_end;

					anim_to_play->active = true;
					item_active.add_component(new motion::Motion(
						anim_to_play->frame,
						frame_start,
						frame_end,
						abs(core::round(frame_end - frame_start))
					));
				}
			}
		}
	}

	// input after saving or loading isn't properly debounced
	if (loading_or_saving) {
		loading_or_saving = false;
	}

	return this;
}

State* MapState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryDuration>();

	auto ring_item_active = get_active_item(entity_manager);
	if (ring_item_active) {
		auto &item_active = ring_item_active->item;

		if (item_active.has_component<item::ItemData>()
			&& item_active.has_component<special::MapData>()) {
			auto &item_data = *item_active.get_component<item::ItemData>();
			auto &map_data = *item_active.get_component<special::MapData>();

			// add text for item name
			if (inventory) {
				const auto item_text = text::build_item_text(
					text::TextType::ITEM_NAME_ACTIVE,
					entity_manager,
					item_data.name.get_string()
				);
				text::add_text(
					*inventory,
					text::TextType::ITEM_NAME_ACTIVE,
					item_text,
					entity_manager
				);
			}

			if (item_active.has_component<item::ItemDisplay>()) {
				auto &item_display = *item_active.get_component<item::ItemDisplay>();

				// associate markers with item actions
				auto item_actions = item_active.get_components<item::ItemAction>();
				const auto action_count = item_actions.size();
				const auto marker_count = map_data.markers.size();

				// load saved marker index
				const auto marker_index = MyData.Save.Global.inventory_data.map_marker_active[item::item_id_to_item_index(item_data.item_id)];
				if (marker_index >= 0 && marker_index < marker_count) {
					map_data.marker_index = marker_index;
				}

				item::deactivate_item_actions(item_active);

				for (uint32_t i = 0; i < marker_count; ++i) {
					auto &marker = map_data.markers.at(i);

					item::ItemAction *item_action = nullptr;
					if (i < action_count) {
						item_action = item_actions.at(i);

						item_action->active = map_data.marker_index == i;
					}
					marker.action = item_action;
				}

				uint32_t duration_frames = 0;
				if (inventory) {
					const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();
					duration_frames = inventory_duration.item_activate_frames;
				}

				update_map_marker(item_active, item_display, map_data, duration_frames, entity_manager);
			}
		}
	}

	return this;
}

State* MapState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		auto ring_item_active = get_active_item(entity_manager);
		if (ring_item_active) {
			auto &item_active = ring_item_active->item;

			if (item_active.has_component<special::MapData>()
				&& item_active.has_component<item::ItemDisplay>()) {
				auto &map_data = *item_active.get_component<special::MapData>();
				auto &item_display = *item_active.get_component<item::ItemDisplay>();

				const auto marker_count = map_data.markers.size();
				auto marker_index = map_data.marker_index;

				if (input_state.command_first_press(enumCMD.INVENTORY) && map_data.cancelable) {
					return new ItemCancelState([]() -> State* {
						return new IdleState();
					});
				}

				if (input_state.command_first_press(enumCMD.LEFT)) {
					// find prev enabled marker
					if (marker_count > 1) {
						do {
							if (marker_index == 0) {
								marker_index = marker_count - 1;
							}
							else {
								--marker_index;
							}

							auto &marker = map_data.markers.at(marker_index);
							if (marker.action && marker.action->action && marker.action->enabled()) {
								break;
							}
						} while (marker_index != map_data.marker_index);
					}
				}
				else if (input_state.command_first_press(enumCMD.RIGHT)) {
					// find next enabled marker
					if (marker_count > 1) {
						do {
							++marker_index;
							if (marker_index >= marker_count) {
								marker_index = 0;
							}

							auto &marker = map_data.markers.at(marker_index);
							if (marker.action && marker.action->action && marker.action->enabled()) {
								break;
							}
						} while (marker_index != map_data.marker_index);
					}
				}
				else if (input_state.command_first_press(enumCMD.ACTION) || input_state.command_first_press(enumCMD.ENTER)) {
					item::deactivate_item_actions(item_active);

					// perform action of active marker
					if (map_data.marker_active && marker_index < marker_count) {
						const auto &marker = map_data.markers.at(marker_index);
						auto &action = *marker.action;

						action.active = true;

						const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
						if (inventory) {
							auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

							inventory_state.item_used = inventory_state.item_active;

							if (action.instant) {
								return new ClosedState();
							}

							return new ItemCancelState([]() -> State* {
								return new ClosingState(false);
							}, false, false, false, false);
						}
					}
				}
				// debug
				else if (input_state.command_first_press(enumCMD.ROLL)) {
					if (inventory::debug_enabled(entity_manager)) {
						return new DebugState(item_active, item::ItemDisplayType::ACTIVE, []() -> State* {
							return new MapState();
						});
					}
				}

				// change marker
				if (marker_index != map_data.marker_index) {
					map_data.marker_index = marker_index;

					uint32_t duration_frames = 0;
					const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryDuration>();
					if (inventory) {
						const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();
						duration_frames = inventory_duration.item_activate_frames;
					}

					update_map_marker(item_active, item_display, map_data, duration_frames, entity_manager);

					return this;
				}
			}
		}
	}

	return this;
}

void MapState::update_map_marker(
	ecs::Entity &item,
	item::ItemDisplay &item_display,
	special::MapData &map_data,
	uint32_t duration_frames,
	ecs::EntityManager &entity_manager
)
{
	if (map_data.marker_index >= map_data.markers.size()) {
		return;
	}

	const auto &marker = map_data.markers.at(map_data.marker_index);

	map_data.marker_active = marker.action && marker.action->action && marker.action->enabled();

	if (!map_data.marker_active) {
		return;
	}

	item.remove_components<render::ScreenText>();

	// add text for marker
	text::add_text(
		item,
		text::TextType::SPECIAL_ACTION,
		marker.action->name.get_string(),
		entity_manager
	);

	// add text for navigation
	if (map_data.markers.size() > 1) {
		const auto glyph_left = "\x84";
		const auto glyph_right = "\x86";

		text::add_text(
			item,
			text::TextType::NAV_LEFT,
			glyph_left,
			entity_manager
		);
		text::add_text(
			item,
			text::TextType::NAV_RIGHT,
			glyph_right,
			entity_manager
		);
	}

	// orient map to selected marker
	motion::add_motion_rot(
		item,
		item_display.orient.x,
		item_display.orient.x,
		marker.map_orient.x,
		duration_frames
	);
	motion::add_motion_rot(
		item,
		item_display.orient.y,
		item_display.orient.y,
		marker.map_orient.y,
		duration_frames
	);
	motion::add_motion_rot(
		item,
		item_display.orient.z,
		item_display.orient.z,
		marker.map_orient.z,
		duration_frames
	);

	// set item_display configs to same orientation for future activations
	auto item_display_active = item::get_item_display_config(item, item::ItemDisplayType::ACTIVE);
	if (item_display_active) {
		item_display_active->orient.x = marker.map_orient.x;
		item_display_active->orient.y = marker.map_orient.y;
		item_display_active->orient.z = marker.map_orient.z;
	}

	// save the current marker index
	const auto item_data = item.get_component<item::ItemData>();
	if (item_data) {
		MyData.Save.Global.inventory_data.map_marker_active[item::item_id_to_item_index(item_data->item_id)] = map_data.marker_index;
	}
}

CheatState::CheatState(int32_t entity_id)
	:
	entity_id(entity_id)
{}

State* CheatState::update(ecs::EntityManager &entity_manager)
{
	auto inventory_state = inventory::get_inventory_state(entity_manager);

	if (inventory_state) {
		const auto entity = entity_manager.get_entity(entity_id);

		const auto get_next_ring = [=, &entity_manager]() -> inventory::InventoryRing* {
			// perform cheat(s)
			if (entity) {
				cheat::do_active_cheats(*entity);
			}

			// rebuild inventory
			int32_t ring_id = ring::RingId::INVENTORY;
			int32_t item_id = item::ItemId::NONE;

			const auto ring_item_selected = get_selected_item(entity_manager);
			if (ring_item_selected) {
				auto &ring = ring_item_selected->ring;
				auto &item = ring_item_selected->item;

				if (ring.has_component<ring::RingData>()) {
					ring_id = ring.get_component<ring::RingData>()->ring_id;
				}
				if (item.has_component<item::ItemData>()) {
					item_id = item.get_component<item::ItemData>()->item_id;
				}
			}

			inventory::build_inventory(ring_id, item_id, *inventory_state, entity_manager);

			return inventory_state->ring;
		};

		return new RingChangeState(RingChangeState::FADE, get_next_ring, []() -> State* {
			return new IdleState();
		}, false);
	}

	return this;
}

DebugState::DebugState(
	ecs::Entity &item,
	item::ItemDisplayType::Enum display_type,
	std::function<State*()> get_next_state)
	:
	item(item),
	display_type(display_type),
	get_next_state(get_next_state)
{}

State* DebugState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (inventory) {
		clear_hud(entity_manager);

		item::restore_item_spin(item, 5);

		const uint32_t line_height = 40;
		const uint32_t screen_x = 50;
		uint32_t screen_y = 100;

		inventory->add_component(new render::ScreenText(
			"Debug Mode",
			screen_x,
			screen_y,
			0,
			enumFC.YELLOW,
			enumFTS.ALIGN_LEFT)
		);

		// add empty debug lines to be filled on update
		screen_y += 10;
		for (uint8_t i = 0; i < 20; ++i) {
			screen_y += line_height;

			inventory->add_component(new render::ScreenText(
				"",
				screen_x,
				screen_y,
				enumFT.SIZE_ATOMIC_CHAR,
				enumFC.WHITE,
				enumFTS.ALIGN_LEFT)
			);
		}
	}

	return this;
}

void DebugState::end(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryData>();
	if (!inventory) {
		return;
	}

	clear_hud(entity_manager);
}

State* DebugState::update(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<render::ScreenText>();

	if (inventory) {
		auto screen_texts = inventory->get_components<render::ScreenText>();

		uint32_t text_index = 0;


		if (++text_index < screen_texts.size()) {
			auto screen_text = screen_texts.at(text_index);
			if (screen_text) {
				std::ostringstream text("");
				text << "Lighting Location:";

				const auto &lara = *Trng.pGlobTomb4->pAdr->pLara;
				auto light_loc_room = lara.Room;
				auto light_loc_x = lara.CordX;
				auto light_loc_y = lara.CordY;
				auto light_loc_z = lara.CordZ;
				auto light_loc_radius = 0;

				const auto light_loc = inventory->get_component<render::LightingLocation>();
				if (light_loc
					&& light_loc->room >= 0
					&& light_loc->x >= 0
					&& light_loc->z >= 0
					&& light_loc->radius >= 0) {
					light_loc_room = light_loc->room;
					light_loc_x = light_loc->x;
					light_loc_y = light_loc->y;
					light_loc_z = light_loc->z;
					light_loc_radius = light_loc->radius;
				}

				text << " Room=" << light_loc_room
					<< ", X=" << light_loc_x
					<< ", Y=" << light_loc_y
					<< ", Z=" << light_loc_z
					<< ", Radius=" << light_loc_radius;

				screen_text->text = text.str();
			}
		}

		const auto camera = entity_manager.find_entity_with_component<camera::CameraView>();
		if (camera) {
			auto &camera_view = *camera->get_component<camera::CameraView>();

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Camera Pos:"
						<< " X=" << camera_view.position.x << " (Left/Right)"
						<< ", Y=" << camera_view.position.y << " (Home/PgUp)"
						<< ", Z=" << camera_view.position.z << " (Up/Down)";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Camera Tgt:"
						<< " X=" << camera_view.target.x << " (Ctrl+Left/Right)"
						<< ", Y=" << camera_view.target.y << " (Ctrl+Home/PgUp)";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Camera FOV: " << camera_view.fov << " (Ins/Del)";
					screen_text->text = text.str();
				}
			}
		}

		if (inventory->has_component<inventory::InventoryState>()) {
			auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

			auto ring = inventory_state.ring;
			if (ring && ring->ring.has_component<ring::RingDisplay>()) {
				auto &ring_display = *ring->ring.get_component<ring::RingDisplay>();

				if (++text_index < screen_texts.size()) {
					auto screen_text = screen_texts.at(text_index);
					if (screen_text) {
						std::ostringstream text("");
						text << "Ring Radius: " << ring_display.radius << " (End/PgDn)";
						screen_text->text = text.str();
					}
				}
			}
		}

		if (inventory->has_component<inventory::InventoryDisplay>()) {
			auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Base Size: " << inventory_display.item_base_size << " (Z/X)";
					screen_text->text = text.str();
				}
			}
		}

		if (item.has_component<item::ItemData>()
			&& item.has_component<item::ItemRing>()
			&& item.has_component<item::ItemModel>()
			&& item.has_component<item::ItemDisplay>()) {
			auto &item_data = *item.get_component<item::ItemData>();
			auto &item_ring = *item.get_component<item::ItemRing>();
			auto &item_model = *item.get_component<item::ItemModel>();
			auto &item_display = *item.get_component<item::ItemDisplay>();

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Name: " << item_data.name.get_string();
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					short item_model_mesh_mask = min(INT16_MAX, item_model.config->mesh_mask);

					std::ostringstream text("");
					text << "Item Model:"
						<< " Slot=" << item_model.config->slot_id
						<< ", Mesh-Mask=" << item_model_mesh_mask
						<< " (" << std::bitset<15>(item_model_mesh_mask) << ")";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::string display_type_str;
					switch (display_type) {
					case item::ItemDisplayType::IDLE:
						display_type_str = "Idle";
						break;
					case item::ItemDisplayType::ACTIVE:
						display_type_str = "Active";
						break;
					case item::ItemDisplayType::CONTEXT:
						display_type_str = "Context";
						break;
					case item::ItemDisplayType::EXAMINE:
						display_type_str = "Examine";
						break;
					case item::ItemDisplayType::PICKUP:
						display_type_str = "Pickup";
						break;
					default:;
					}

					std::ostringstream text("");
					text << "Item Display: " << display_type_str;
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Pos:"
						<< " X=" << item_display.pos.x << " (A/D)"
						<< ", Y=" << item_display.pos.y << " (Q/E)"
						<< ", Z=" << item_display.pos.z << " (W/S)";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Orient:"
						<< " X=" << item_display.orient.x << " (Ctrl+W/S)"
						<< ", Y=" << item_display.orient.y << " (Ctrl+A/D)"
						<< ", Z=" << item_display.orient.z << " (Ctrl+Q/E)";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Tilt: " << item_display.tilt << " (R/F)";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Scale: " << core::round(item_display.scale * 100) << " (T/G)";
					screen_text->text = text.str();
				}
			}

			if (++text_index < screen_texts.size()) {
				auto screen_text = screen_texts.at(text_index);
				if (screen_text) {
					std::ostringstream text("");
					text << "Item Sort Index: " << item_ring.sort_index;
					screen_text->text = text.str();
				}
			}
		}
	}

	return this;
}

State* DebugState::input(input::InputState &input_state, ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	const auto camera = entity_manager.find_entity_with_component<camera::CameraView>();

	if (inventory && camera) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();
		auto &camera_view = *camera->get_component<camera::CameraView>();

		// use scan codes in case commands are mapped to same keys

		const bool ctrl_active = input_state.key_active(29);

		if (!ctrl_active) {
			// position camera
			if (input_state.key_active(77)) { // RIGHT arrow
				camera_view.position.x -= 10;
			}
			if (input_state.key_active(75)) { // LEFT arrow
				camera_view.position.x += 10;
			}
			if (input_state.key_active(73)) { // PAGE UP
				camera_view.position.y -= 10;
			}
			if (input_state.key_active(71)) { // HOME
				camera_view.position.y += 10;
			}
			if (input_state.key_active(72)) { // UP arrow
				camera_view.position.z -= 10;
			}
			if (input_state.key_active(80)) { // DOWN arrow
				camera_view.position.z += 10;
			}
		}
		else {
			// orientate camera
			if (input_state.key_active(77)) { // RIGHT arrow
				camera_view.target.x -= 10;
			}
			if (input_state.key_active(75)) { // LEFT arrow
				camera_view.target.x += 10;
			}
			if (input_state.key_active(73)) { // PAGE UP
				camera_view.target.y -= 10;
			}
			if (input_state.key_active(71)) { // HOME
				camera_view.target.y += 10;
			}
		}

		// alter camera fov
		if (input_state.key_active(83)) { // DELETE
			camera_view.fov -= 1;
		}
		if (input_state.key_active(82)) { // INSERT
			camera_view.fov += 1;
		}

		// resize ring
		auto ring = inventory_state.ring;
		if (ring && ring->ring.has_component<ring::RingDisplay>()) {
			auto &ring_display = *ring->ring.get_component<ring::RingDisplay>();

			if (input_state.key_active(79)) { // END
				ring_display.radius -= 10;
			}
			if (input_state.key_active(81)) { // PAGE DOWN
				ring_display.radius += 10;
			}
		}

		// resize items
		if (inventory->has_component<inventory::InventoryDisplay>()) {
			auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

			if (input_state.key_active(44)) { // Z
				inventory_display.item_base_size -= 100;
			}
			if (input_state.key_active(45)) { // X
				inventory_display.item_base_size += 100;
			}
		}

		auto item_display_config = item::get_item_display_config(item, display_type);
		if (item_display_config) {
			if (!ctrl_active) {
				// position item
				if (input_state.key_active(30)) { // A
					item_display_config->pos.x -= 1;
				}
				if (input_state.key_active(32)) { // D
					item_display_config->pos.x += 1;
				}
				if (input_state.key_active(18)) { // E
					item_display_config->pos.y -= 1;
				}
				if (input_state.key_active(16)) { // Q
					item_display_config->pos.y += 1;
				}
				if (input_state.key_active(31)) { // S
					item_display_config->pos.z -= 1;
				}
				if (input_state.key_active(17)) { // W
					item_display_config->pos.z += 1;
				}
			}
			else {
				// orientate item
				if (input_state.key_active(17)) { // W
					item_display_config->orient.x -= 1;
				}
				if (input_state.key_active(31)) { // S
					item_display_config->orient.x += 1;
				}
				if (input_state.key_active(30)) { // A
					item_display_config->orient.y -= 1;
				}
				if (input_state.key_active(32)) { // D
					item_display_config->orient.y += 1;
				}
				if (input_state.key_active(16)) { // Q
					item_display_config->orient.z -= 1;
				}
				if (input_state.key_active(18)) { // E
					item_display_config->orient.z += 1;
				}
			}

			// tilt item
			if (input_state.key_active(19)) { // R
				item_display_config->tilt -= 1;
			}
			if (input_state.key_active(33)) { // F
				item_display_config->tilt += 1;
			}

			// scale item
			if (input_state.key_active(20)) { // T
				item_display_config->scale -= 0.01f;
			}
			if (input_state.key_active(34)) { // G
				item_display_config->scale += 0.01f;
			}

			item::change_item_display(item, display_type,
				false,
				false,
				true,
				false,
				false,
				true,
				true
			);
		}
	}

	if (input_state.key_first_press(1) || input_state.key_first_press(15)) { // ESCAPE or TAB
		input_state.command_first_press(enumCMD.INVENTORY); // debounce for next state

		return get_next_state();
	}

	return this;
}

ClosingState::ClosingState(bool play_sfx)
	:
	play_sfx(play_sfx)
{}

State* ClosingState::start(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory->has_component<inventory::InventoryDisplay>() && inventory->has_component<inventory::InventoryDuration>()) {
			const auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();
			const auto &inventory_duration = *inventory->get_component<inventory::InventoryDuration>();

			if (inventory_state.ring) {
				close_ring(inventory_state.ring->ring, inventory_display, &inventory_duration.inventory_open_frames);
			}

			const auto camera = entity_manager.find_entity_with_component<camera::CameraView>();
			if (camera) {
				close_camera(*camera, inventory_display, &inventory_duration.inventory_open_frames);
			}
		}

		if (play_sfx) {
			// add sfx
			add_sfx(sound::SfxType::INVENTORY_CLOSE, false, *inventory);
		}

		clear_hud(entity_manager);
	}

	if (motion::get_entities_in_motion(entity_manager).empty()) {
		return new ClosedState();
	}

	return this;
}

State* ClosingState::update(ecs::EntityManager &entity_manager)
{
	// wait for all relevant motions to finish
	const auto entities_in_motion = motion::get_entities_in_motion(entity_manager);

	if (entities_in_motion.empty()) {
		return new ClosedState();
	}

	return this;
}



// ----------------------------
// ##### HELPER FUNCTIONS #####
// ----------------------------
ring::RingItem* get_selected_item(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		if (inventory_state.ring) {
			auto &ring = inventory_state.ring->ring;

			if (ring.has_component<ring::RingState>()) {
				const auto &ring_state = *ring.get_component<ring::RingState>();

				return ring_state.item;
			}
		}
	}

	return nullptr;
}

ring::RingItem* get_active_item(ecs::EntityManager &entity_manager)
{
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();

	if (inventory) {
		auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

		return inventory_state.item_active;
	}

	return nullptr;
}

void clear_hud(ecs::EntityManager &entity_manager)
{
	entity_manager.remove_components<render::ScreenText>();
	entity_manager.remove_components<render::ScreenBar>();
	entity_manager.remove_components<render::HealthBar>();
	entity_manager.remove_components<render::AirBar>();
}

void add_sfx(
	sound::SfxType::Enum type,
	bool stop,
	ecs::Entity &inventory,
	ecs::Entity *item
)
{
	if (item) {
		const auto item_sfx = item->get_component<item::ItemSfx>([&](item::ItemSfx &sfx) -> bool {
			return sfx.type == type;
		});
		if (item_sfx) {
			if (item_sfx->sound_id >= 0) {
				inventory.add_component(new sound::SoundFX(item_sfx->sound_id, stop));
			}
			return;
		}
	}

	// TODO: add sfx type to InventorySfx as well
	if (inventory.has_component<inventory::InventorySfx>()) {
		const auto &inventory_sfx = *inventory.get_component<inventory::InventorySfx>();

		int32_t default_sound_id = -1;
		switch (type) {
		case sound::SfxType::INVENTORY_OPEN:
			default_sound_id = inventory_sfx.opening_sound_id;
			break;
		case sound::SfxType::INVENTORY_CLOSE:
			default_sound_id = inventory_sfx.closing_sound_id;
			break;
		case sound::SfxType::RING_ROTATE:
			default_sound_id = inventory_sfx.ring_rotate_sound_id;
			break;
		case sound::SfxType::RING_CHANGE:
			default_sound_id = inventory_sfx.ring_change_sound_id;
			break;
		case sound::SfxType::ITEM_ACTIVATE:
			default_sound_id = inventory_sfx.item_select_sound_id;
			break;
		case sound::SfxType::ITEM_CANCEL:
			default_sound_id = inventory_sfx.item_cancel_sound_id;
			break;
		case sound::SfxType::MENU_CHANGE:
			default_sound_id = inventory_sfx.menu_change_sound_id;
			break;
		case sound::SfxType::LOAD_AMMO:
			default_sound_id = inventory_sfx.ammo_load_sound_id;
			break;
		case sound::SfxType::COMBINE:
			default_sound_id = inventory_sfx.combine_sound_id;
			break;
		case sound::SfxType::SEPARATE:
			default_sound_id = inventory_sfx.separate_sound_id;
			break;
		case sound::SfxType::FAIL:
			default_sound_id = inventory_sfx.fail_sound_id;
			break;
		}

		if (default_sound_id >= 0) {
			inventory.add_component(new sound::SoundFX(default_sound_id, stop));
		}
	}
}

}
}
