#include "StdAfx.h"

#include "render.h"

#include <algorithm>

#include <trng_core.h>
#include "core.h"
#include "camera.h"
#include "cheat.h"
#include "inventory.h"
#include "ring.h"
#include "item.h"

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

namespace classicinventory {
namespace render {

const auto poly_alpha = reinterpret_cast<int32_t*>(0x4AB7B8);

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

	set_lighting(entity_manager);

	draw_inventory(entity_manager);

	restore_lighting(entity_manager);

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

	const auto item_tilt = core::degrees_to_tr4_angle(item_display->tilt);
	const auto ring_angle = core::degrees_to_tr4_angle(90 + (ring_display.sector * ring_item.item_index));

	phd_PushMatrix();

	// set transparency
	uint8_t alpha = 255;
	if (ring_display.alpha < 255 && item_display->alpha_enabled) {
		alpha = uint8_t(ring_display.alpha);
	}
	*poly_alpha = alpha << 24;

	// lets do all transforms relative to world origin
	phd_TranslateAbs(0, 0, 0);

	// position center of ring
	phd_TranslateRel(ring_pos_x, ring_pos_y, ring_pos_z);
	phd_RotYXZ(ring_rot_y, ring_rot_x, ring_rot_z);

	// position item on ring
	phd_PushMatrix();
	phd_RotYXZ(ring_angle, 0, 0);			// angle around centre of ring
	phd_TranslateRel(ring_radius, 0, 0);	// translate by radius
	phd_RotY(-0x4000);						// face outwards
	phd_RotX(item_tilt);					// tilt

	draw_item(item, inventory_display, entity_manager);

	// pop pop
	core::jmn_PopMatrix();
	core::jmn_PopMatrix();
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

	// TODO: get correct frame based on frame rate
	const auto anim_frame = reinterpret_cast<int16_t*>(tr4_anim.FrameOffset) + (item_anim ? core::round(item_anim->frame) * tr4_anim.FrameSize : 0);
	const int32_t root_trans_x = *(anim_frame + 6);
	const int32_t root_trans_y = *(anim_frame + 7);
	const int32_t root_trans_z = *(anim_frame + 8);
	auto root_rot = (anim_frame + 9);

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
		phd_TranslateRel(root_trans_x, root_trans_y, root_trans_z); // TODO: causes jitter
	}
	if (!item_display->config->orient_ignore_anim) {
		gar_RotYXZsuperpack(&root_rot, 0);
	}

	// draw meshes
	const int32_t poly_alpha_backup = *poly_alpha;
	const auto mesh_tree = reinterpret_cast<int32_t*>(0x533958);
	auto bone = reinterpret_cast<int32_t*>(*mesh_tree) + slot_tr4.IndexFirstTree;
	uint16_t mesh_bit = 1;

	for (int32_t mesh_index = 0; mesh_index < slot_tr4.TotMesh; ++mesh_index) {
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
			gar_RotYXZsuperpack(&root_rot, 0);

			bone += 4;
		}

		// do special stuff, e.g. compass needle, stopwatch hands, etc.
		// TODO: make this more generic
		int16_t mesh_rot_special = 0;
		core::Axis::Enum mesh_rot_special_axis = core::Axis::X;

		if (item.has_component<item::CompassData>()) {
			auto &compass_data = *item.get_component<item::CompassData>();

			if (compass_data.needle_mesh_index == mesh_index) {
				// rotate compass needle
				mesh_rot_special = core::degrees_to_tr4_angle(compass_data.needle_angle);
				mesh_rot_special_axis = compass_data.needle_mesh_axis;

				// make needle transparent if cheats enabled
				const auto cheat_entity = entity_manager.find_entity_with_component <cheat::CheatConfig>([](const cheat::CheatConfig &config) -> bool {
					return config.enabled()
						&& config.hint_type == cheat::CheatHintType::COMPASS_TRANSPARENT;
				});
				if (cheat_entity) {
					*poly_alpha = 128 << 24;
				}
			}
		}

		// rotate stopwatch hands
		if (item.has_component<item::StopwatchData>()) {
			auto &stopwatch_data = *item.get_component<item::StopwatchData>();

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
		if (item.has_component<item::MapData>()) {
			auto &map_data = *item.get_component<item::MapData>();

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

		// draw mesh if mesh-mask bit is set
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

void InventoryRenderSystem::draw_text(
	std::string text,
	uint32_t screen_x,
	uint32_t screen_y,
	int32_t flags_size,
	int32_t flags_colour,
	int32_t flags_align
) const {
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

void InventoryRenderSystem::draw_bars(ecs::EntityManager & entity_manager) const
{
	auto entities = entity_manager.find_entities_with_component<ScreenBar>();

	for (auto entity_it = entities.begin(); entity_it != entities.end(); ++entity_it) {
		auto &entity = **entity_it;

		auto bars = entity.get_components<ScreenBar>();
		for (auto bar_it = bars.begin(); bar_it != bars.end(); ++bar_it) {
			auto &screen_bar = **bar_it;

			draw_bar(
				screen_bar.x,
				screen_bar.y,
				screen_bar.width,
				screen_bar.height,
				screen_bar.percent,
				screen_bar.colour1,
				screen_bar.colour2
			);
		}
	}
}

void InventoryRenderSystem::draw_bar(
	uint32_t screen_x,
	uint32_t screen_y,
	uint32_t width,
	uint32_t height,
	uint32_t percent,
	uint32_t colour1,
	uint32_t colour2
) const
{
	// position and size needs to be based on resolution of 640x480
	// game will adjust proportionally to fit current resolution

	screen_x = core::round((screen_x / 1000.f) * 640.f);
	screen_y = core::round((screen_y / 1000.f) * Trng.pGlobTomb4->ScreenSizeY); // not computed from 480, bug?

	width = core::round((width / 1000.f) * 640.f);
	height = core::round((height / 1000.f) * 480.f);

	DoBar(screen_x, screen_y, width, height, percent, colour1, colour2);
}

void InventoryRenderSystem::draw_statistics(
	ecs::EntityManager &entity_manager
) const
{
	const auto entity = entity_manager.find_entity_with_component<StatisticsScreen>();

	if (entity) {
		DoStatScreen();
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

void InventoryRenderSystem::set_lighting(ecs::EntityManager & entity_manager) const
{
	/*
	* Lighting is based on Lara's current position.
	* If a lighting location is configured, we'll move Lara there to get the lighting data and
	* afterwards restore Lara again to her original location.
	*/

	const auto entity = entity_manager.find_entity_with_component<LightingLocation>();
	if (!entity) {
		return;
	}
	auto &light_loc = *entity->get_component<LightingLocation>();

	if (light_loc.x == 0
		&& light_loc.y == 0
		&& light_loc.z == 0
		&& light_loc.room == 0) {
		return;
	}

	// backup Lara's location
	const auto lara = Trng.pGlobTomb4->pAdr->pLara;
	bool &lara_in_water = *reinterpret_cast<bool*>(0x80EBB0);

	light_loc.backup_x = lara->CordX;
	light_loc.backup_y = lara->CordY;
	light_loc.backup_z = lara->CordZ;
	light_loc.backup_room = lara->Room;
	light_loc.backup_lara_in_water = lara_in_water;

	// move Lara to lighting location
	lara->CordX = light_loc.x;
	lara->CordY = light_loc.y;
	lara->CordZ = light_loc.z;
	lara->Room = light_loc.room;

	// set to false to prevent water effect when Lara touches water
	lara_in_water = false;

	// TODO: there's still a blue tint when camera is underwater

	calculate_lighting();
}

void InventoryRenderSystem::restore_lighting(ecs::EntityManager & entity_manager) const
{
	const auto entity = entity_manager.find_entity_with_component<LightingLocation>();
	if (!entity) {
		return;
	}
	auto &light_loc = *entity->get_component<LightingLocation>();

	if (light_loc.x == 0
		&& light_loc.y == 0
		&& light_loc.z == 0
		&& light_loc.room == 0) {
		return;
	}

	// restore Lara's location
	const auto lara = Trng.pGlobTomb4->pAdr->pLara;
	bool &lara_in_water = *reinterpret_cast<bool*>(0x80EBB0);

	lara->CordX = light_loc.backup_x;
	lara->CordY = light_loc.backup_y;
	lara->CordZ = light_loc.backup_z;
	lara->Room = light_loc.backup_room;

	lara_in_water = light_loc.backup_lara_in_water;

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

	// cleanup if not in a desired phase
	if (static_cast<int32_t>(GET.LaraInfo.SkipPhaseFlags) != enumSKIP.NONE
		&& !core::bit_set(GET.LaraInfo.SkipPhaseFlags, enumSKIP.FLY_CAMERA, true)) {
		cleanup(entity_manager, system_manager);
		return;
	}

	// only draw during main game phase
	if (static_cast<int32_t>(GET.LaraInfo.SkipPhaseFlags) != enumSKIP.NONE) {
		return;
	}

	// we should be inside a drawing cycle already
	draw_pickups(entity_manager);
}

void GameRenderSystem::cleanup(ecs::EntityManager &entity_manager, ecs::SystemManager &system_manager)
{
	entity_manager.remove_components<render::PickupDisplay>();
}

void GameRenderSystem::draw_pickups(ecs::EntityManager & entity_manager)
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
		const auto item_model = item::get_item_model_config(*item, item::ItemModelType::PICKUP);
		const auto item_display = item::get_item_display_config(*item, item::ItemDisplayType::PICKUP);

		if (!item_model || !item_display) {
			pickup_done = true;
		}
		else {
			// fade in and out
			const auto poly_alpha = reinterpret_cast<int32_t*>(0x4AB7B8);
			*poly_alpha = core::round(pickup_display.alpha) << 24;

			if (pickup_display.rotation.y <= 45 && pickup_display.alpha < 255) {
				pickup_display.alpha = min(255, pickup_display.alpha + 17);
			}
			else if (pickup_display.rotation.y >= 315 && pickup_display.alpha > 0) {
				pickup_display.alpha = max(0, pickup_display.alpha - 17);
			}

			// spin
			pickup_display.rotation.y += 3;

			// draw pickup on screen
			RECT screen_pos;
			screen_pos.left = core::round(pickup_display.position.x + item_display->pos.x);
			screen_pos.top = core::round(pickup_display.position.y + item_display->pos.y);
			ConvertMicroUnits(&screen_pos);

			DrawObject2D(
				item_model->slot_id,
				screen_pos.left,
				screen_pos.top,
				core::degrees_to_tr4_angle(item_display->orient.y + pickup_display.rotation.y),
				core::degrees_to_tr4_angle(item_display->orient.x),
				core::degrees_to_tr4_angle(item_display->orient.z),
				core::round(0x300 / item_display->scale)
			);

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
		pickup_display.active = false;
	}

	// clear pickups that are done
	entity_manager.remove_components<PickupDisplay>([](const PickupDisplay &pickup_display) -> bool {
		return !pickup_display.active;
	});
}

}
}