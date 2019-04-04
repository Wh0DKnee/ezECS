#include <iostream>
#include "Entity.h"
#include "Component.h"
#include "System.h"

int main()
{
	EntityManager entity_manager;
	Entity& entity = entity_manager.createEntity();
	entity_manager.addComponent(entity, Position(0, 0));
	entity_manager.addComponent(entity, Position(2, 1));
	MovementSystem movement_system;
	for (size_t i = 0; i < 10; ++i)
	{
		movement_system.update(entity_manager, 1);
		Position p = entity_manager.getComponent<Position>(entity);
		std::cout << p.x << " " << p.y << std::endl;
	}

	std::cout << ComponentMaskGetter<Position>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Velocity>::getComponentMask() << std::endl;
	
}