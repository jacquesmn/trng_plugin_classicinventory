#include <StdAfx.h>

#include "core.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <trng_core.h>

namespace classicinventory {
namespace core {

float degrees_to_radians(float degrees)
{
	return float(degrees) * float(M_PI / 180.f);
}

int16_t degrees_to_tr4_angle(float degrees)
{
	return round(degrees * 65536.f / 360.f) & 0xffff;
}

float tr4_angle_to_degrees(int16_t angle)
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

void jmn_PopMatrix()
{
	*Trng.pGlobTomb4->pAdr->pZonaRecord30 -= 48;
}

}
}