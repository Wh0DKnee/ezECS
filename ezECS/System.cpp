#include "System.h"
#include <iostream>

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
