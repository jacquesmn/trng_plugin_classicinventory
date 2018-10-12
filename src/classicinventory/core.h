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
	} else {
		return (value & bit) == bit;
	}
}

float degrees_to_radians(float degrees);
int16_t degrees_to_tr4_angle(float degrees);
float tr4_angle_to_degrees(int16_t angle);
void wrap_angle(float &degrees);
float angle_diff_smallest(float angle_a_degrees, float angle_b_degrees);
int32_t round(float x);
void jmn_PopMatrix();

}
}

#pragma pack(pop)