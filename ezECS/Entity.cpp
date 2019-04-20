#include "Entity.h"
#include "Config.h"
#include <algorithm>

EntityManager::EntityManager()
{
	current_pool_size_ = INITIAL_POOL_SIZE;
	next_free_id_ = current_pool_size_;
	for (size_t i = 0; i < current_pool_size_; ++i)
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
		resizePoolsAndGenerateFreeIds();
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

void EntityManager::resizePoolsAndGenerateFreeIds()
{
	current_pool_size_ += POOL_INCREMENT_SIZE;
	for (size_t i = 0; i < POOL_INCREMENT_SIZE; ++i, ++next_free_id_)
	{
		free_ids_.push_back(next_free_id_);
	}
	/*for (std::shared_ptr<PoolBase> pool : component_pools_)
	{
		if (pool != nullptr)
		{
			pool->resize(pool->size() + POOL_INCREMENT_SIZE);
		}
	}*/
}

bool Entity::hasComponents(ComponentMask required_components_mask) const
{
	return (component_mask & required_components_mask).count() == required_components_mask.count();
}
