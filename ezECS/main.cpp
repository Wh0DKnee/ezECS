#include <iostream>
#include "Entity.h"
#include "Component.h"
#include "System.h"

int main()
{
	EntityManager entity_manager;
	Entity entity = entity_manager.createEntity();
	ComponentManager component_manager;
	MovementSystem movement_system;
	movement_system.update(component_manager, 1);

	std::cout << ComponentMaskGetter<Position>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Velocity>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Position>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Velocity>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Position>::getComponentMask() << std::endl;
}