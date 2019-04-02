#pragma once
#include <stdint.h>
#include <deque>

using EntityID = int64_t;

struct Entity
{
	Entity(EntityID i) : id(i) {}
	EntityID id;
};

class EntityManager
{
public:
	EntityManager();

	Entity CreateEntity();

private:
	std::deque<EntityID> freeIDs;
};
