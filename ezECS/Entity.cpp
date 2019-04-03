#include "Entity.h"
#include "Config.h"

EntityManager::EntityManager()
{
	for (size_t i = 0; i < INITIAL_POOL_SIZE; ++i)
	{
		free_IDs.push_back(i);
	}
}

Entity EntityManager::createEntity()
{
	Entity e = Entity(free_IDs.front());
	free_IDs.pop_front();
	return e;
}

void EntityManager::deleteEntity(Entity e)
{
	free_IDs.push_front(e.id);
}
