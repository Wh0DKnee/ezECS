#include "Entity.h"
#include "Config.h"

EntityManager::EntityManager()
{
	for (size_t i = 0; i < INITIAL_POOL_SIZE; ++i)
	{
		freeIDs.push_back(i);
	}
}

Entity EntityManager::CreateEntity()
{
	Entity e = Entity(freeIDs.front());
	freeIDs.pop_front();
	return e;
}
