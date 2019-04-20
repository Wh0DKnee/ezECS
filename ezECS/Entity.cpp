#include "Entity.h"
#include "Config.h"
#include <algorithm>

EntityManager::EntityManager()
{
	next_free_id_ = NUM_INITIAL_FREE_ID;
	for (size_t i = 0; i < NUM_INITIAL_FREE_ID; ++i)
	{
		free_ids_.push_back(i);
	}
	component_pools_.resize(MAX_COMPONENTS);
}

Entity& EntityManager::createEntity()
{
	if (free_ids_.empty())
	{
		// Initial pool is full, so we need to generate new IDs and resize the pools.
		generateFreeIds();
	}
	entities_.emplace_back(free_ids_.front());
	free_ids_.pop_front();
	return entities_.back();
}

void EntityManager::deleteEntity(const Entity& e)
{
	free_ids_.push_front(e.id);
	auto it = std::find(entities_.begin(), entities_.end(), e);
	if (it != entities_.end())
	{
		entities_.erase(it);
	}
}

void EntityManager::generateFreeIds()
{
	for (size_t i = 0; i < NUM_FREE_ID_INCREMENT; ++i, ++next_free_id_)
	{
		free_ids_.push_back(next_free_id_);
	}
}

bool Entity::hasComponents(ComponentMask required_components_mask) const
{
	return (component_mask & required_components_mask).count() == required_components_mask.count();
}
