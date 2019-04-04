#include "Entity.h"
#include "Config.h"

EntityManager::EntityManager()
{
	for (size_t i = 0; i < INITIAL_POOL_SIZE; ++i)
	{
		free_IDs.push_back(i);
	}

	components.resize(MAX_COMPONENTS);
	auto position_pool = std::make_shared<Pool<Position>>();
	position_pool->data.resize(INITIAL_POOL_SIZE);
	components[ComponentMaskGetter<Position>::getId()] = position_pool;

	auto velocity_pool = std::make_shared<Pool<Velocity>>();
	velocity_pool->data.resize(INITIAL_POOL_SIZE);
	components[ComponentMaskGetter<Velocity>::getId()] = velocity_pool;
}

Entity EntityManager::createEntity()
{
	//TODO: Resize pools when free_IDs is empty and generate new free_IDs.
	Entity e = Entity(free_IDs.front(), *this);
	free_IDs.pop_front();
	return e;
}

void EntityManager::deleteEntity(Entity e)
{
	free_IDs.push_front(e.id);
}
