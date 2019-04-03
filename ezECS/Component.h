#pragma once
#include <vector>
#include <algorithm>
#include "Entity.h"
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
	float x;
	float y;
};

struct Velocity
{
	float x;
	float y;
};

class ComponentManager
{
public:
	ComponentManager();

	std::vector<Position> positions;
	std::vector<Velocity> velocities;
};

