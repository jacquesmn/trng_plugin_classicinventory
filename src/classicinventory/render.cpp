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

#include "render.h"

#include <sstream>
#include <iomanip>

#include "camera.h"
#include "cheat.h"
#include "core.h"
#include "inventory.h"
#include "item.h"
#include "ring.h"
#include "special.h"

extern TYPE_CreateMonoScreen CreateMonoScreen;
extern TYPE_FreeMonoScreen FreeMonoScreen;
extern TYPE_S_InitialisePolyList S_InitialisePolyList;
extern TYPE_S_DisplayMonoScreen S_DisplayMonoScreen;
extern TYPE_S_OutputPolyList S_OutputPolyList;
extern TYPE_S_DumpScreen S_DumpScreen;
extern TYPE_phd_PushMatrix phd_PushMatrix;
extern TYPE_phd_TranslateAbs phd_TranslateAbs;
extern TYPE_phd_TranslateRel phd_TranslateRel;
extern TYPE_phd_RotYXZ phd_RotYXZ;
extern TYPE_phd_RotX phd_RotX;
extern TYPE_phd_RotY phd_RotY;
extern TYPE_phd_RotZ phd_RotZ;
extern TYPE_gar_RotYXZsuperpack gar_RotYXZsuperpack;
extern TYPE_phd_PutPolygons phd_PutPolygons;
extern TYPE_phd_PutPolygons_train phd_PutPolygons_train;
extern TYPE_phd_PutPolygonsInv phd_PutPolygonsInv;
extern TYPE_ScaleCurrentMatrix ScaleCurrentMatrix;
extern TYPE_UpdateTextPulse UpdateTextPulse;
extern TYPE_CalculateObjectLightingLara CalculateObjectLightingLara;
extern TYPE_DoBar DoBar;
extern TYPE_DoStatScreen DoStatScreen;
extern TYPE_DoOptions DoOptions;
extern TYPE_GetFloor GetFloor;
extern TYPE_DrawRooms DrawRooms;
extern TYPE_phd_GetVectorAngles phd_GetVectorAngles;
extern TYPE_phd_GenerateW2V phd_GenerateW2V;
extern TYPE_AlterFOV AlterFOV;
extern TYPE_DrawHealthBar DrawHealthBar;
extern TYPE_DrawAirBar DrawAirBar;
extern void BackupLara(StrBackupLara *pBack, StrItemTr4 *pOggetto);
extern void RestoreLara(StrBackupLara *pBack, StrItemTr4 *pOggetto);
extern StrMyData MyData;

namespace classicinventory {
namespace render {

const auto poly_alpha = reinterpret_cast<uint32_t*>(0x4AB7B8);

void InventoryRenderSystem::init(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	// create background image
	*Trng.pGlobTomb4->pAdr->pTestGrayScreen = true;
	CreateMonoScreen();
}

void InventoryRenderSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	// begin drawing cycle
	S_InitialisePolyList();

	// draw background
	S_DisplayMonoScreen();

	draw_inventory(entity_manager);
	draw_texts(entity_manager);
	draw_bars(entity_manager);
	draw_statistics(entity_manager);
	draw_options(entity_manager);

	// finish drawing cycle
	S_OutputPolyList();
	S_DumpScreen();
}

void InventoryRenderSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	// free background image
	FreeMonoScreen();
	*Trng.pGlobTomb4->pAdr->pTestGrayScreen = false;
}

void InventoryRenderSystem::draw_inventory(ecs::EntityManager &entity_manager) const {
	const auto inventory = entity_manager.find_entity_with_component<inventory::InventoryState>();
	if (!inventory) {
		return;
	}
	auto &inventory_state = *inventory->get_component<inventory::InventoryState>();

	if (!inventory->has_component<inventory::InventoryDisplay>()) {
		return;
	}
	auto &inventory_display = *inventory->get_component<inventory::InventoryDisplay>();

	if (!inventory_state.ring) {
		return;
	}
	auto &ring = inventory_state.ring->ring;

	const auto ring_display = ring.get_component<ring::RingDisplay>();
	const auto ring_item_head = ring.get_component<ring::RingItem>([](ring::RingItem &ring_item) -> bool {
		return ring_item.item_index == 0;
	});
	if (!ring_display || !ring_item_head) {
		return;
	}

	// draw all items on the active ring except the active item which will be drawn separately
	// this will allow the active item to be drawn even when its ring isn't active
	const auto ring_item_active = inventory_state.item_active;
	auto ring_item = ring_item_head;
	do {
		if (!ring_item_active || ring_item_active != ring_item) {
			draw_ring_item(*ring_item, inventory_display, entity_manager);
		}

		ring_item = ring_item->next;
	} while (ring_item->item_index != ring_item_head->item_index);

	if (ring_item_active) {
		draw_ring_item(*ring_item_active, inventory_display, entity_manager);
	}
}

void InventoryRenderSystem::draw_ring_item(
	const ring::RingItem &ring_item,
	const inventory::InventoryDisplay &inventory_display,
	ecs::EntityManager &entity_manager
) const
{
	auto &ring = ring_item.ring;
	if (!ring.has_component<ring::RingDisplay>()) {
		return;
	}
	auto &ring_display = *ring.get_component<ring::RingDisplay>();

	const auto ring_radius = core::round(ring_display.radius);
	const auto ring_pos_x = core::round(ring_display.position.x);
	const auto ring_pos_y = core::round(ring_display.position.y);
	const auto ring_pos_z = core::round(ring_display.position.z);
	const auto ring_rot_x = core::degrees_to_tr4_angle(ring_display.rotation.x);
	const auto ring_rot_y = core::degrees_to_tr4_angle(ring_display.rotation.y);
	const auto ring_rot_z = core::degrees_to_tr4_angle(ring_display.rotation.z);

	auto &item = ring_item.item;

	const auto item_display = item.get_component<item::ItemDisplay>();
	if (!item_display) {
		return;
	}

	phd_PushMatrix();

	// set transparency
	uint8_t alpha = 255;
	if (ring_display.alpha < 255 && item_display->alpha_enabled) {
		alpha = uint8_t(ring_display.alpha);
	}
	*poly_alpha = alpha << 24;

	// do all transforms relative to world origin
	phd_TranslateAbs(0, 0, 0);

	// start off at center of ring
	phd_TranslateRel(ring_pos_x, ring_pos_y, ring_pos_z);
	phd_RotYXZ(ring_rot_y, ring_rot_x, ring_rot_z);

	// position item on ring
	const auto ring_angle_degrees = (ring_item.item_index * ring_display.sector) + 90;
	const auto ring_angle = core::degrees_to_tr4_angle(ring_angle_degrees);
	const auto item_tilt = core::degrees_to_tr4_angle(item_display->tilt);

	phd_PushMatrix();
	phd_RotYXZ(ring_angle, 0, 0);				// angle around centre of ring
	phd_TranslateRel(ring_radius, 0, 0);		// put on edge of ring
	phd_RotY(-core::degrees_to_tr4_angle(90));	// face outwards
	phd_RotX(item_tilt);						// tilt

	set_lighting(entity_manager, ring_display.rotation.y + ring_angle_degrees);

	draw_item(item, inventory_display, entity_manager);

	restore_lighting(entity_manager);

	// pop pop
	core::jmn_PopMatrix(2);
}

void InventoryRenderSystem::draw_item(
	ecs::Entity &item,
	const inventory::InventoryDisplay &inventory_display,
	ecs::EntityManager &entity_manager
) const {
	const auto item_display = item.get_component<item::ItemDisplay>();
	if (!item_display || !item_display->config) {
		return;
	}

	const auto item_model = item.get_component<item::ItemModel>();
	if (!item_model || !item_model->config) {
		return;
	}

	if (!item_model->config->slot) {
		return;
	}
	const auto &slot_tr4 = *item_model->config->slot;

	const auto item_anim = item.get_component<item::ItemAnimation>([](const item::ItemAnimation &item_animation) -> bool {
		return item_animation.active;
	});
	const auto tr4_anim = Trng.pGlobTomb4->pAdr->pVetAnimations[slot_tr4.IndexFirstAnim + (item_anim ? item_anim->anim_index : 0)];

	const auto anim_frame = reinterpret_cast<int16_t*>(tr4_anim.FrameOffset) + (item_anim ? core::round(item_anim->frame) * tr4_anim.FrameSize : 0);
	const int32_t frame_trans_x = *(anim_frame + 6);
	const int32_t frame_trans_y = *(anim_frame + 7);
	const int32_t frame_trans_z = *(anim_frame + 8);
	auto frame_rot = (anim_frame + 9);
	// TODO: interpolate based on frame rate

	const auto item_pos_x = core::round(item_display->pos.x);
	const auto item_pos_y = core::round(item_display->pos.y);
	const auto item_pos_z = core::round(item_display->pos.z);
	const auto item_orient_x = core::degrees_to_tr4_angle(item_display->orient.x);
	const auto item_orient_y = core::degrees_to_tr4_angle(item_display->orient.y);
	const auto item_orient_z = core::degrees_to_tr4_angle(item_display->orient.z);
	const auto item_rot_x = core::degrees_to_tr4_angle(item_display->rot.x);
	const auto item_rot_y = core::degrees_to_tr4_angle(item_display->rot.y);
	const auto item_rot_z = core::degrees_to_tr4_angle(item_display->rot.z);

	// set base position
	phd_TranslateRel(item_pos_x, item_pos_y, item_pos_z);

	// orientate in XYZ order for user-friendly scripting
	phd_RotX(item_rot_x);
	phd_RotY(item_rot_y);
	phd_RotZ(item_rot_z);
	phd_RotX(item_orient_x);
	phd_RotY(item_orient_y);
	phd_RotZ(item_orient_z);

	// scale item
	StrTripla scale;
	scale.SizeX = core::round(inventory_display.item_base_size * item_display->scale);
	scale.SizeY = core::round(inventory_display.item_base_size * item_display->scale);
	scale.SizeZ = core::round(inventory_display.item_base_size * item_display->scale);
	ScaleCurrentMatrix(&scale);

	// start rendering mesh tree 
	phd_PushMatrix();

	// transform root mesh according to animation frame
	if (!item_display->config->pos_ignore_anim) {
		phd_TranslateRel(frame_trans_x, frame_trans_y, frame_trans_z); // TODO: causes jitter
	}
	if (!item_display->config->orient_ignore_anim) {
		gar_RotYXZsuperpack(&frame_rot, 0);
	}

	// draw meshes
	const uint32_t poly_alpha_backup = *poly_alpha;
	const auto mesh_tree = reinterpret_cast<int32_t*>(0x533958);
	auto bone = reinterpret_cast<int32_t*>(*mesh_tree) + slot_tr4.IndexFirstTree;
	uint16_t mesh_bit = 1;

	for (int32_t mesh_index = 0; mesh_index < slot_tr4.TotMesh; ++mesh_index) {
		// draw mesh if mesh-mask bit is set
		bool draw_mesh = core::bit_set(mesh_bit, item_model->config->mesh_mask, true);

		if (mesh_index > 0) {
			const auto mesh_poppush = *bone;
			const auto mesh_trans_x = *(bone + 1);
			const auto mesh_trans_y = *(bone + 2);
			const auto mesh_trans_z = *(bone + 3);

			if (mesh_poppush & 1) {
				core::jmn_PopMatrix();
			}
			if (mesh_poppush & 2) {
				phd_PushMatrix();
			}

			// transform mesh according to animation frame
			phd_TranslateRel(mesh_trans_x, mesh_trans_y, mesh_trans_z); // TODO: causes jitter
			gar_RotYXZsuperpack(&frame_rot, 0);

			bone += 4;
		}

		// do special stuff, e.g. compass needle, stopwatch hands, etc.
		// TODO: make this more generic
		int16_t mesh_rot_special = 0;
		core::Axis::Enum mesh_rot_special_axis = core::Axis::X;

		if (item.has_component<special::CompassData>()) {
			auto &compass_data = *item.get_component<special::CompassData>();

			auto &pointers = compass_data.pointers;
			for (auto pointer_it = pointers.begin(); pointer_it != pointers.end(); ++pointer_it) {
				auto &pointer = *pointer_it;

				if (pointer.mesh_index == mesh_index) {
					// rotate compass needle
					mesh_rot_special = core::degrees_to_tr4_angle(pointer.angle);
					mesh_rot_special_axis = pointer.mesh_axis;

					// make needle transparent if cheats are enabled
					if (cheat::cheats_enabled()) {
						const auto cheat_entity = entity_manager.find_entity_with_component <cheat::CheatConfig>([](const cheat::CheatConfig &config) -> bool {
							return config.enabled()
								&& config.hint_type == cheat::CheatHintType::COMPASS_TRANSPARENT;
						});
						if (cheat_entity) {
							*poly_alpha = min(128, *poly_alpha >> 24) << 24;
						}
					}

					break;
				}
			}
		}

		// rotate stopwatch hands
		if (item.has_component<special::StopwatchData>()) {
			auto &stopwatch_data = *item.get_component<special::StopwatchData>();

			if (stopwatch_data.hour_hand_mesh_index == mesh_index) {
				mesh_rot_special = core::degrees_to_tr4_angle(-stopwatch_data.hour_hand_angle);
				mesh_rot_special_axis = stopwatch_data.hour_hand_mesh_axis;
			}
			else if (stopwatch_data.minute_hand_mesh_index == mesh_index) {
				mesh_rot_special = core::degrees_to_tr4_angle(-stopwatch_data.minute_hand_angle);
				mesh_rot_special_axis = stopwatch_data.minute_hand_mesh_axis;
			}
			else if (stopwatch_data.second_hand_mesh_index == mesh_index) {
				mesh_rot_special = core::degrees_to_tr4_angle(-stopwatch_data.second_hand_angle);
				mesh_rot_special_axis = stopwatch_data.second_hand_mesh_axis;
			}
		}

		// only draw the active map marker
		if (item.has_component<special::MapData>()) {
			auto &map_data = *item.get_component<special::MapData>();

			for (uint32_t i = 0; i < map_data.markers.size(); ++i) {
				auto &marker = map_data.markers.at(i);

				if (marker.mesh_index == mesh_index && (i != map_data.marker_index || !map_data.marker_active)) {
					draw_mesh = false;
					break;
				}
			}
		}

		if (mesh_rot_special != 0) {
			switch (mesh_rot_special_axis) {
			case core::Axis::X:
				phd_RotX(mesh_rot_special);
				break;
			case core::Axis::Y:
				phd_RotY(mesh_rot_special);
				break;
			case core::Axis::Z:
				phd_RotZ(mesh_rot_special);
				break;
			}
		}

		if (draw_mesh) {
			const auto mesh_tr4 = Trng.pGlobTomb4->pAdr->VetMeshPointer[slot_tr4.IndexFirstMesh + mesh_index * 2];

			phd_PutPolygons(mesh_tr4, 0);
		}

		mesh_bit <<= 1;

		*poly_alpha = poly_alpha_backup;
	}

	core::jmn_PopMatrix();
}

void InventoryRenderSystem::draw_texts(ecs::EntityManager &entity_manager) const {
	auto entities = entity_manager.find_entities_with_component<ScreenText>();

	for (auto entity_it = entities.begin(); entity_it != entities.end(); ++entity_it) {
		auto &entity = **entity_it;

		auto texts = entity.get_components<ScreenText>([](ScreenText &screen_text) -> bool {
			return !screen_text.text.empty();
		});
		for (auto text_it = texts.begin(); text_it != texts.end(); ++text_it) {
			auto &screen_text = **text_it;

			draw_text(
				screen_text.text,
				screen_text.x,
				screen_text.y,
				screen_text.flags_size,
				screen_text.flags_colour,
				screen_text.flags_align
			);
		}
	}

	UpdateTextPulse();
}

void InventoryRenderSystem::draw_bars(ecs::EntityManager &entity_manager) const
{
	auto health_entities = entity_manager.find_entities_with_component<HealthBar>();
	for (auto entity_it = health_entities.begin(); entity_it != health_entities.end(); ++entity_it) {
		auto &entity = **entity_it;

		DrawHealthBar(entity.get_component<HealthBar>()->percent);
		
		break;
	}

	auto air_entities = entity_manager.find_entities_with_component<AirBar>();
	for (auto entity_it = air_entities.begin(); entity_it != air_entities.end(); ++entity_it) {
		auto& entity = **entity_it;

		DrawAirBar(entity.get_component<AirBar>()->percent);

		break;
	}
}

void InventoryRenderSystem::draw_statistics(
	ecs::EntityManager &entity_manager
) const
{
	const auto entity = entity_manager.find_entity_with_component<StatisticsScreen>();

	if (entity) {
		//DoStatScreen();
		draw_stats();
	}
}

void InventoryRenderSystem::draw_options(ecs::EntityManager &entity_manager) const
{
	const auto entity = entity_manager.find_entity_with_component<OptionsScreen>([](const OptionsScreen &screen) -> bool {
		return !screen.finished;
	});

	if (entity) {
		auto &options_screen = *entity->get_component<OptionsScreen>();

		auto &alternate_input_flags = *reinterpret_cast<int32_t*>(0x535784);
		auto &inventory_active = *reinterpret_cast<bool*>(0x7FE040);
		auto &control_screen_active = *reinterpret_cast<bool*>(0x5B7368);
		auto &waiting_for_new_key = *reinterpret_cast<bool*>(0x5B736C);
		auto &control_scheme_type = *reinterpret_cast<int32_t*>(0x5B7360); // keyboard, joystick, reset

		if (!options_screen.started) {
			options_screen.started = true;

			// set some flags for tr4 options code to work
			alternate_input_flags = 0;
			control_scheme_type = 0;
			inventory_active = true;

			// only show the controls screen
			if (options_screen.controls_only) {
				control_screen_active = true;
			}
		}
		else {
			// mark as finished after exiting
			if (core::bit_set(alternate_input_flags, enumCMD.INVENTORY)
				&& (!control_screen_active || (options_screen.controls_only && !waiting_for_new_key))) {
				options_screen.finished = true;

				control_scheme_type = 0;
				waiting_for_new_key = false;
				control_screen_active = false;
				inventory_active = false;

				return;
			}
		}

		DoOptions();
	}
}

void InventoryRenderSystem::set_lighting(ecs::EntityManager &entity_manager, float ring_angle_degrees) const
{
	/*
	* Lighting is based on Lara's current position.
	* If a lighting location is configured, we'll move Lara there to get the lighting data and
	* afterwards restore Lara again to her original location.
	* Lara will also be moved around the point to allow different lighting for different sections of the ring.
	*/

	const auto entity = entity_manager.find_entity_with_component<LightingLocation>();
	if (!entity) {
		return;
	}
	auto &light_loc = *entity->get_component<LightingLocation>();

	if (light_loc.room < 0
		|| light_loc.x < 0
		|| light_loc.z < 0
		|| light_loc.radius < 0) {
		return;
	}

	// calculate point around location based on ring angle
	core::Vector2D light_loc_ring;

	core::point_on_circle(
		core::Vector2D(float(light_loc.x), float(light_loc.z)),
		float(light_loc.radius),
		ring_angle_degrees,
		light_loc_ring
	);

	// backup Lara's location
	const auto lara = Trng.pGlobTomb4->pAdr->pLara;
	bool &lara_in_water = *reinterpret_cast<bool*>(0x80EBB0);
	bool &camera_underwater = *reinterpret_cast<bool*>(0x7FE73C);

	light_loc.backup_lara = StrBackupLara();
	light_loc.backup_lara_in_water = lara_in_water;
	light_loc.backup_camera_underwater = camera_underwater;
	BackupLara(&light_loc.backup_lara, lara);

	// move Lara to lighting location
	lara->Room = light_loc.room;
	lara->CordX = core::round(light_loc_ring.x);
	lara->CordY = light_loc.y;
	lara->CordZ = core::round(light_loc_ring.y);
	lara->AnimationNow = 103; // standing idle, prevents animation poses from influencing lighting
	lara->FrameNow = 0;

	// set to false to prevent water effect when Lara touches water 
	lara_in_water = false;

	// set to false to prevent water effect when camera is underwater
	camera_underwater = false;

	calculate_lighting();
}

void InventoryRenderSystem::restore_lighting(ecs::EntityManager &entity_manager) const
{
	const auto entity = entity_manager.find_entity_with_component<LightingLocation>();
	if (!entity) {
		return;
	}
	auto &light_loc = *entity->get_component<LightingLocation>();

	if (light_loc.room < 0
		|| light_loc.x < 0
		|| light_loc.z < 0
		|| light_loc.radius < 0) {
		return;
	}

	// restore Lara's location
	const auto lara = Trng.pGlobTomb4->pAdr->pLara;
	bool &lara_in_water = *reinterpret_cast<bool*>(0x80EBB0);
	bool &camera_underwater = *reinterpret_cast<bool*>(0x7FE73C);

	RestoreLara(&light_loc.backup_lara, lara);
	lara_in_water = light_loc.backup_lara_in_water;
	camera_underwater = light_loc.backup_camera_underwater;

	calculate_lighting();
}

void InventoryRenderSystem::calculate_lighting() const
{
	// change is gradual so perform a few times to make sure lighting is correct
	for (int32_t i = 0; i < 32; ++i) {
		CalculateObjectLightingLara();
	}
}

void GameRenderSystem::update(
	ecs::EntityManager &entity_manager,
	ecs::SystemManager &system_manager
)
{
	Get(enumGET.INFO_LARA, 0, 0);

	// cleanup if any undesired phase is active
	const auto undesired_phases = enumSKIP.LOADING_LEVEL
		| enumSKIP.TITLE_LEVEL
		| enumSKIP.GRAY_SCREEN
		| enumSKIP.NO_VIEW_OGGETTI
		| enumSKIP.BINOCULARS
		| enumSKIP.LASER_SIGHT
		| enumSKIP.FULL_IMAGE;

	if (core::bit_set(GET.LaraInfo.SkipPhaseFlags, undesired_phases, true)) {
		cleanup(entity_manager, system_manager);
		return;
	}

	// only draw during main game phase and fixed views
	if (core::bit_set(GET.LaraInfo.SkipPhaseFlags, undesired_phases | enumSKIP.FLY_CAMERA, true)) {
		return;
	}

	// we should be inside a drawing cycle already
	draw_pickups(entity_manager);
}

void GameRenderSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	entity_manager.remove_components<PickupDisplay>([](const PickupDisplay &pickup_display) -> bool {
		if (pickup_display.param_show_sprite_id >= 0) {
			PerformFlipeffect(nullptr, 358, pickup_display.param_show_sprite_id, 0);
		}
		
		return true;
	});
}

void GameRenderSystem::draw_pickups(ecs::EntityManager &entity_manager)
{
	// pick first pickup
	const auto pickup = entity_manager.find_entity_with_component<PickupDisplay>([](const PickupDisplay &pickup_display) -> bool {
		return pickup_display.active;
	});
	if (!pickup) {
		return;
	}
	auto &pickup_display = *pickup->get_component<PickupDisplay>();

	const auto item = entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == pickup_display.item_id;
	});

	bool pickup_done = false;

	if (item) {
		const auto item_model_pickup = item::get_item_model_config(*item, item::ItemModelType::PICKUP);
		const auto item_display_pickup = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP);

		if (!item_model_pickup || !item_display_pickup) {
			pickup_done = true;
		}
		else {
			// fade in and out
			if (!item_display_pickup->fade) {
				pickup_display.alpha = 255;
			}

			const auto poly_alpha = reinterpret_cast<int32_t*>(0x4AB7B8);
			*poly_alpha = core::round(pickup_display.alpha) << 24;

			if (item_display_pickup->fade) {
				if (pickup_display.rotation.y <= 45 && pickup_display.alpha < 255) {
					pickup_display.alpha = min(255, pickup_display.alpha + 17);
				}
				else if (pickup_display.rotation.y >= 315 && pickup_display.alpha > 0) {
					pickup_display.alpha = max(0, pickup_display.alpha - 17);
				}
			}

			// spin
			pickup_display.rotation.y += 3;

			// draw pickup on screen
			if (item_model_pickup->sprite_id >= 0
				&& item_model_pickup->size_x > 0
				&& item_model_pickup->size_y > 0) {
				if (pickup_display.param_show_sprite_id < 0) {
					RECT screen_pos;
					screen_pos.left = core::round(item_display_pickup->pos.x);
					screen_pos.top = core::round(item_display_pickup->pos.y);
					screen_pos.right = item_model_pickup->size_x;
					screen_pos.bottom = item_model_pickup->size_y;
					
					pickup_display.param_show_sprite_id = Service(
						enumSRV.CREATE_PARAM_COMMAND,
						true,
						PARAM_SHOW_SPRITE,
						item_display_pickup->fade ? FSS_EFFECT_ZOOM : -1,
						screen_pos.left,
						screen_pos.top,
						screen_pos.right,
						screen_pos.bottom,
						item_model_pickup->slot_id,
						item_model_pickup->sprite_id,
						-1,
						-1,
						-1,
						item_display_pickup->fade ? 15 : -1,
						END_LIST
					);
					PerformFlipeffect(nullptr, 357, pickup_display.param_show_sprite_id, 0);
				}
			}
			else {
				RECT screen_pos;
				screen_pos.left = core::round(item_display_pickup->pos.x);
				screen_pos.top = core::round(item_display_pickup->pos.y);
				ConvertMicroUnits(&screen_pos);

				DrawObject2D(
					item_model_pickup->slot_id,
					screen_pos.left,
					screen_pos.top,
					core::degrees_to_tr4_angle(item_display_pickup->orient.y + pickup_display.rotation.y),
					core::degrees_to_tr4_angle(item_display_pickup->orient.x),
					core::degrees_to_tr4_angle(item_display_pickup->orient.z),
					core::round(0x300 / item_display_pickup->scale)
				);
			}

			if (pickup_display.rotation.y >= 360) {
				pickup_done = true;
			}
		}
	}
	else {
		pickup_done = true;
	}

	// mark as done
	if (pickup_done) {
		if (pickup_display.param_show_sprite_id >= 0) {
			PerformFlipeffect(nullptr, 358, pickup_display.param_show_sprite_id, 0);
		}
		
		pickup_display.active = false;
	}

	// clear pickups that are done
	entity_manager.remove_components<PickupDisplay>([](const PickupDisplay &pickup_display) -> bool {
		return !pickup_display.active;
	});
}

void StatisticsRenderSystem::update(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	draw_stats();
}

void draw_text(
	std::string text,
	uint32_t screen_x,
	uint32_t screen_y,
	int32_t flags_size,
	int32_t flags_colour,
	int32_t flags_align
) {
	RECT text_area;
	text_area.left = screen_x;
	text_area.top = screen_y;
	ConvertMicroUnits(&text_area);

	PrintText(
		text_area.left,
		text_area.top,
		&text[0],
		flags_size,
		flags_colour,
		flags_align
	);
}

void draw_stats()
{
	const auto &statistics_global = MyData.Save.Global.statistics;
	const auto &statistics_local = MyData.Save.Local.statistics;
	const auto &statistics = statistics_global;
	
	const auto font_size = 0;
	const auto line_height = 68;
	const auto div_width = 500;
	const auto text_x = 130;
	auto text_y = 100;

	draw_text(script::ScriptString(script::StringIndex::STATISTICS).get_string(), 500, text_y, 0, enumFC.GOLD, enumFTS.ALIGN_CENTER);

	// Level Name
	text_y += int(line_height * 1.25);
	const auto level_index = *Trng.pGlobTomb4->pAdr->pLevelNow;
	const auto level_name_stridex = reinterpret_cast<BYTE*>(0x7FD1A0)[level_index];
	draw_text(script::ScriptString(level_name_stridex).get_string(), 500, text_y, 0, enumFC.WHITE, enumFTS.ALIGN_CENTER);

	// Time Taken
	text_y += int(line_height * 1.25);
	draw_text(script::ScriptString(script::StringIndex::TIME_TAKEN).get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto game_seconds = statistics.time_taken_seconds;
	const auto d = game_seconds / 86400;
	const auto h = game_seconds % 86400 / 3600;
	const auto m = game_seconds % 3600 / 60;
	const auto s = game_seconds % 60;

	std::ostringstream stream_time;
	if (d) {
		stream_time << d;
		stream_time << " " << script::ScriptString(script::StringIndex::DAYS).get_string() << " ";
	}
	stream_time << std::setfill('0') << std::setw(2) << h;
	stream_time << ":";
	stream_time << std::setfill('0') << std::setw(2) << m;
	stream_time << ":";
	stream_time << std::setfill('0') << std::setw(2) << s;
	draw_text(std::string(stream_time.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Distance Travelled
	text_y += line_height;
	draw_text(script::ScriptString(script::StringIndex::DISTANCE_TRAVELLED).get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto distance_meters = statistics.distance_travelled_meters;
	
	std::ostringstream stream_distance;
	if (distance_meters < 1000) {
		stream_distance << distance_meters << "m";
	}
	else {
		stream_distance << std::setprecision(2) << std::fixed << distance_meters / 1000.f << "km";
	}
	draw_text(std::string(stream_distance.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Ammo Used
	text_y += line_height;
	draw_text(script::ScriptString(script::StringIndex::AMMO_USED).get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto ammo_used = statistics.ammo_used;

	std::ostringstream stream_ammo;
	stream_ammo << ammo_used;
	draw_text(std::string(stream_ammo.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Hits
	text_y += line_height;
	draw_text("Hits", text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto hits = statistics.hits;

	std::ostringstream stream_hits;
	stream_hits << hits;
	draw_text(std::string(stream_hits.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Kills
	text_y += line_height;
	draw_text("Kills", text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto kills = statistics.kills;

	std::ostringstream stream_kills;
	stream_kills << kills;
	draw_text(std::string(stream_kills.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Pickups
	text_y += line_height;
	draw_text("Pickups", text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto pickups = statistics.pickups;

	std::ostringstream stream_pickups;
	stream_pickups << pickups;
	draw_text(std::string(stream_pickups.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Health Packs Used
	text_y += line_height;
	draw_text(script::ScriptString(script::StringIndex::HEALTH_PACKS_USED).get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto health_packs_used = statistics.health_packs_used;

	std::ostringstream stream_health_packs;
	stream_health_packs << health_packs_used;
	draw_text(std::string(stream_health_packs.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Health Lost
	text_y += line_height;
	draw_text(script::ScriptString("Health Lost").get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto health_lost = statistics.health_lost;

	std::ostringstream stream_injuries;
	stream_injuries << health_lost << " HP";
	draw_text(std::string(stream_injuries.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Flares Used
	text_y += line_height;
	draw_text(script::ScriptString("Flares Used").get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto flares_used = statistics.flares_used;

	std::ostringstream stream_flares;
	stream_flares << flares_used;
	draw_text(std::string(stream_flares.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Times Saved
	text_y += line_height;
	draw_text(script::ScriptString("Times Saved").get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto times_saved = statistics.times_saved;

	std::ostringstream stream_saved;
	stream_saved << times_saved;
	draw_text(std::string(stream_saved.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);

	// Secrets Found
	text_y += line_height;
	draw_text(script::ScriptString(script::StringIndex::SECRETS_FOUND).get_string(), text_x, text_y, font_size, enumFC.WHITE, enumFTS.ALIGN_LEFT);
	const auto secrets_found = statistics.secrets_found;

	std::ostringstream stream_secrets;
	stream_secrets << secrets_found;
	stream_secrets << " " << script::ScriptString(script::StringIndex::OF).get_string() << " ";
	stream_secrets << Trng.pGlobTomb4->pBaseCustomize->SecretsAmount;
	draw_text(std::string(stream_secrets.str()), text_x + div_width, text_y, font_size, enumFC.GOLD, enumFTS.ALIGN_LEFT);
}

}
}