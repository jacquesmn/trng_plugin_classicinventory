#include <StdAfx.h>

#include "item.h"

#include <trng_core.h>
#include "motion.h"

extern TYPE_convert_obj_to_invobj convert_obj_to_invobj;

namespace classicinventory {
namespace item {

bool ItemQuantity::increment(int32_t qty) const {
	if (!get_quantity || !set_quantity) {
		return false;
	}

	auto quantity = get_quantity();

	if (quantity != ITEM_QTY_UNLIMITED) {
		quantity += qty;

		if (quantity <= quantity_max) {
			set_quantity(quantity);

			return true;
		}
	}

	return false;
}

bool ItemQuantity::decrement(int32_t qty) const {
	if (!get_quantity || !set_quantity) {
		return false;
	}

	auto quantity = get_quantity();

	if (quantity != ITEM_QTY_UNLIMITED) {
		quantity -= qty;

		if (quantity >= quantity_min
			&& quantity >= 0) {
			set_quantity(quantity);

			return true;
		}
	}

	return false;
}

bool ItemQuantity::set(int32_t qty) const
{
	if (qty >= quantity_min && qty <= quantity_max) {
		set_quantity(qty);

		return true;
	}

	return false;
}

bool ItemQuantity::unlimited() const {
	if (!set_quantity) {
		return false;
	}

	if (quantity_min <= ITEM_QTY_UNLIMITED
		&& quantity_max >= ITEM_QTY_UNLIMITED) {
		set_quantity(ITEM_QTY_UNLIMITED);

		return true;
	}

	return false;
}

bool ItemQuantity::zero() const {
	if (!set_quantity) {
		return false;
	}

	set_quantity(0);

	return true;
}

int32_t tr4_slot_to_item_id(uint32_t tr4_slot)
{
	if (tr4_slot == UINT32_MAX) {
		return ItemId::NONE;
	}

	return tr4_invobj_to_item_id(convert_obj_to_invobj(tr4_slot));
}

int32_t tr4_invobj_to_item_id(uint32_t tr4_invobj)
{
	if (tr4_invobj > 118) {
		return ItemId::NONE;
	}

	return tr4_invobj - abs(MIN_INVENTORY_ITEM_ID);
}

uint32_t item_id_to_item_index(int32_t item_id)
{
	const uint32_t item_id_count = MAX_INVENTORY_ITEM_ID - MIN_INVENTORY_ITEM_ID;

	return (item_id - MAX_INVENTORY_ITEM_ID) + item_id_count;
}

uint32_t item_id_to_tr4_slot(int32_t item_id)
{
	const auto tr4_invobj = item_id_to_tr4_invobj(item_id);
	if (tr4_invobj > 118) {
		return UINT32_MAX;
	}

	return Trng.pGlobTomb4->pAdr->pVetStructInventoryItems[tr4_invobj].Slot;
}

uint32_t item_id_to_tr4_invobj(int32_t item_id)
{
	if (item_id >= ItemId::NONE) {
		return UINT32_MAX;
	}

	return item_id + abs(MIN_INVENTORY_ITEM_ID);
}

ecs::Entity* get_item_by_item_id(int32_t item_id, ecs::EntityManager &entity_manager)
{
	return entity_manager.find_entity_with_component<item::ItemData>([&](const item::ItemData &item_data) -> bool {
		return item_data.item_id == item_id;
	});
}

item::ItemModelConfig* get_item_model_config(
	ecs::Entity &item,
	item::ItemModelType::Enum type,
	item::ItemModelType::Enum type_default
)
{
	auto item_model = item.get_component<item::ItemModelConfig>([&](item::ItemModelConfig &config) -> bool {
		return config.type == type;
	});
	if (!item_model) {
		item_model = item.get_component<item::ItemModelConfig>([&](item::ItemModelConfig &config) -> bool {
			return config.type == type_default;
		});
	}

	return item_model;
}

item::ItemDisplayConfig* get_item_display_config(
	ecs::Entity &item,
	item::ItemDisplayType::Enum type,
	item::ItemDisplayType::Enum type_default
)
{
	auto item_display = item.get_component<item::ItemDisplayConfig>([&](item::ItemDisplayConfig &config) -> bool {
		return config.type == type;
	});
	if (!item_display) {
		item_display = item.get_component<item::ItemDisplayConfig>([&](item::ItemDisplayConfig &config) -> bool {
			return config.type == type_default;
		});
	}

	return item_display;
}

void change_item_model(
	ecs::Entity &item,
	item::ItemModelType::Enum type
)
{
	const auto item_model = item.get_component<item::ItemModel>();
	if (!item_model) {
		return;
	}

	const auto model_config = get_item_model_config(item, type);
	if (!model_config) {
		return;
	}

	item_model->config = model_config;
}

void change_item_display(
	ecs::Entity &item,
	item::ItemDisplayType::Enum type,
	bool keep_pos,
	bool keep_orient,
	bool keep_rot,
	bool keep_tilt,
	bool keep_scale,
	bool keep_motion,
	bool keep_alpha
)
{
	const auto item_display = item.get_component<item::ItemDisplay>();
	if (!item_display) {
		return;
	}

	const auto display_config = item::get_item_display_config(item, type);
	if (!display_config) {
		return;
	}

	item_display->config = display_config;

	if (!keep_pos) {
		item_display->pos = display_config->pos;
	}
	if (!keep_orient) {
		item_display->orient = display_config->orient;
	}
	if (!keep_rot) {
		item_display->rot = core::Vector3D();
	}
	if (!keep_tilt) {
		item_display->tilt = display_config->tilt;
	}
	if (!keep_scale) {
		item_display->scale = display_config->scale;
	}
	if (!keep_motion) {
		remove_item_motion(item);
	}
	if (!keep_alpha) {
		item_display->alpha_enabled = true;
	}
}

void remove_item_motion(ecs::Entity &item, bool keep_background)
{
	item.remove_components<motion::Motion>([&](const motion::Motion &motion) -> bool {
		if (keep_background) {
			return !motion.background;
		}
		return true;
	});
}

void deactivate_item_actions(ecs::Entity &item)
{
	auto item_actions = item.get_components<item::ItemAction>();

	std::for_each(item_actions.begin(), item_actions.end(), [](item::ItemAction *action) -> void {
		action->active = false;
	});
}

}
}