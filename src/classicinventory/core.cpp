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

#include "core.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#include <trng_core.h>

namespace classicinventory {
namespace core {

float degrees_to_radians(float degrees)
{
	return degrees * float(M_PI / 180.f);
}

float radians_to_degrees(float radians)
{
	return radians * float(180.f / M_PI);
}

uint16_t degrees_to_tr4_angle(float degrees)
{
	return round(degrees * 65536.f / 360.f) & 0xffff;
}

float tr4_angle_to_degrees(uint16_t angle)
{
	return ((angle & 0xffff) / 65536.f) * 360.f;
}

void wrap_angle(float &degrees)
{
	degrees = fmod(degrees, 360);

	if (degrees < 0) {
		degrees += 360;
	}
}

float angle_diff_smallest(float angle_a_degrees, float angle_b_degrees)
{
	wrap_angle(angle_a_degrees);
	wrap_angle(angle_b_degrees);

	return fmod(((angle_b_degrees - angle_a_degrees) + 540), 360) - 180;
}

int32_t round(float x)
{
	return int32_t(x > 0.0 ? x + 0.5 : x - 0.5);
}

float random(float range_min, float range_max)
{
	std::random_device device;
	std::mt19937 engine(device());

	const std::uniform_real_distribution<> distribution(range_min, range_max);

	return float(distribution(engine));
}

int random_sign()
{
	const auto sign = round(random(-1, 1));

	return sign == 0 ? 1 : sign;
}

void jmn_PopMatrix()
{
	*Trng.pGlobTomb4->pAdr->pZonaRecord30 -= 48;
}

}
}