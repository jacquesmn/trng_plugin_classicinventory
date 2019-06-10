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

#include <cstdint>

namespace classicinventory {
namespace core {

namespace Axis {
enum Enum {
	X,
	Y,
	Z
};
}

struct Vector2D {
	float x;
	float y;

	Vector2D(
		float x = 0,
		float y = 0
	)
		:
		x(x),
		y(y)
	{}
};

struct Vector3D {
	float x;
	float y;
	float z;

	Vector3D(
		float x = 0,
		float y = 0,
		float z = 0
	)
		:
		x(x),
		y(y),
		z(z)
	{}
};

template <class T>
void set_bit(T &value, uint32_t bit, bool set = true)
{
	if (set) {
		value |= bit;
	}
	else {
		value &= ~bit;
	}
}

template <class T>
bool bit_set(T &value, uint32_t bit, bool partial = false)
{
	if (partial) {
		return (value & bit) != 0;
	}
	else {
		return (value & bit) == bit;
	}
}

template <class T>
void fill_array(T *arr, uint32_t arr_size, T value)
{
	for (uint32_t i = 0; i < arr_size; ++i) {
		arr[i] = value;
	}
}

float degrees_to_radians(float degrees);
float radians_to_degrees(float radians);
uint16_t degrees_to_tr4_angle(float degrees);
float tr4_angle_to_degrees(uint16_t angle);
void wrap_angle(float &degrees);
float angle_diff_smallest(float angle_a_degrees, float angle_b_degrees);
void point_on_circle(Vector2D origin, float radius, float angle_degrees, Vector2D &result);
int32_t round(float x);
float random(float range_min, float range_max);
int random_sign();
void jmn_PopMatrix(uint32_t num=1);

}
}

#pragma pack(pop)