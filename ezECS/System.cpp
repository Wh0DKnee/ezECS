#include "System.h"
#include <iostream>

MovementSystem::MovementSystem()
{
	requireComponent<Position>();
	//requireComponent<Velocity>();
}

void MovementSystem::update(EntityManager& entity_manager, float delta_seconds)
{
	std::shared_ptr<Pool<Position>> position_pool = entity_manager.getComponentPool<Position>();
	for (auto& entity : getMatchingEntities(entity_manager))
	{
		position_pool->data[entity.id].x += 1.f * delta_seconds;
		position_pool->data[entity.id].y += 2.f * delta_seconds; 
	}
}

std::vector<Entity> System::getMatchingEntities(EntityManager & entity_manager)
{
	std::vector<Entity> entities;
	for (Entity& e : entity_manager.getEntities())
	{
		if (e.hasComponents(component_mask))
		{
			entities.push_back(e);
		}
	}
	return entities;
}
