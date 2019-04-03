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

	Entity createEntity();

	// Need to unregister components before calling this.
	void deleteEntity(Entity e);

private:
	std::deque<EntityID> free_IDs;
};
