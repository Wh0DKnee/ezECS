#include "Entity.h"
#include "Config.h"
#include <algorithm>

EntityManager::EntityManager()
{
	current_pool_size = INITIAL_POOL_SIZE;
	next_free_id = current_pool_size;
	for (size_t i = 0; i < current_pool_size; ++i)
	{
		free_ids.push_back(i);
	}
	component_pools.resize(MAX_COMPONENTS);
}

Entity& EntityManager::createEntity()
{
	if (free_ids.empty())
	{
		// Initial pool is full, so we need to generate new IDs and resize the pools.
		resizePoolsAndGenerateFreeIds();
	}
	entities.emplace_back(free_ids.front());
	free_ids.pop_front();
	return entities.back();
}

void EntityManager::deleteEntity(const Entity& e)
{
	free_ids.push_front(e.id);
	auto it = std::find(entities.begin(), entities.end(), e);
	if (it != entities.end())
	{
		entities.erase(it);
	}
}

void EntityManager::resizePoolsAndGenerateFreeIds()
{
	current_pool_size += POOL_INCREMENT_SIZE;
	for (size_t i = 0; i < POOL_INCREMENT_SIZE; ++i, ++next_free_id)
	{
		free_ids.push_back(next_free_id);
	}
	for (std::shared_ptr<PoolBase> pool : component_pools)
	{
		if (pool != nullptr)
		{
			pool->resize(pool->size() + POOL_INCREMENT_SIZE);
		}
	}
}

bool Entity::hasComponents(ComponentMask required_components_mask) const
{
	return (component_mask & required_components_mask).count() == required_components_mask.count();
}
