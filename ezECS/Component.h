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

// Assigns each type that this is templated with a unique id.
// This is used for components only.
template<typename T>
struct ComponentMaskGetter : BaseComponentMaskGetter
{
	static Id getId()
	{
		static Id id = next_id++;
		assert(id < MAX_COMPONENTS);
		return id;
	}

	static ComponentMask getComponentMask()
	{
		static auto mask = ComponentMask().set(getId());
		return mask;
	}
};
