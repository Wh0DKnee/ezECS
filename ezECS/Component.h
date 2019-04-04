#pragma once
#include "Config.h"
#include <bitset>
#include <cassert>


using ComponentMask = std::bitset<MAX_COMPONENTS>;

struct BaseComponentMaskGetter
{
	using Id = uint8_t;

protected:
	static Id next_id;
};

template<typename T>
struct ComponentMaskGetter : BaseComponentMaskGetter
{
	static Id getId()
	{
		static Id id = next_id;
		++next_id;
		assert(id < MAX_COMPONENTS);
		return id;
	}

	static ComponentMask getComponentMask()
	{
		static ComponentMask mask;
		mask.set(getId());
		return mask;
	}
};

struct Position
{
	Position() = default;
	Position(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};

struct Velocity
{
	Velocity() = default;
	Velocity(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};

