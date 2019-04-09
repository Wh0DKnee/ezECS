#include "Entity.h"
#include "Config.h"
#include <algorithm>

EntityManager::EntityManager()
{
	for (size_t i = 0; i < INITIAL_POOL_SIZE; ++i)
	{
		free_IDs.push_back(i);
	}

	component_pools.resize(MAX_COMPONENTS);
}

Entity& EntityManager::createEntity()
{
	//TODO: Resize pools when free_IDs is empty and generate new free_IDs.
	assert(!free_IDs.empty());
	entities.emplace_back(free_IDs.front());
	free_IDs.pop_front();
	return entities.back();
}

void EntityManager::deleteEntity(const Entity& e)
{
	free_IDs.push_front(e.id);
	auto it = std::find(entities.begin(), entities.end(), e);
	if (it != entities.end())
	{
		entities.erase(it);
	}
}

bool Entity::hasComponents(ComponentMask required_components_mask) const
{
	return (component_mask & required_components_mask).count() == required_components_mask.count();
}
