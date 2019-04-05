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

	// TODO: Find a way to automate this, so we don't have to jump into here
	// every time we add a new component type.
	auto position_pool = std::make_shared<Pool<Position>>();
	position_pool->data.resize(INITIAL_POOL_SIZE);
	component_pools[ComponentMaskGetter<Position>::getId()] = position_pool;

	auto velocity_pool = std::make_shared<Pool<Velocity>>();
	velocity_pool->data.resize(INITIAL_POOL_SIZE);
	component_pools[ComponentMaskGetter<Velocity>::getId()] = velocity_pool;

	auto circle_pool = std::make_shared<Pool<Circle>>();
	circle_pool->data.resize(INITIAL_POOL_SIZE);
	component_pools[ComponentMaskGetter<Circle>::getId()] = circle_pool;
}

Entity& EntityManager::createEntity()
{
	//TODO: Resize pools when free_IDs is empty and generate new free_IDs.
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
