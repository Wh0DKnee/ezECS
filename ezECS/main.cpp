#include <iostream>
#include "Entity.h"
#include "Component.h"
#include "System.h"

class MovementSystem : public System
{
public:
	MovementSystem()
	{
		requireComponent<Position>();
		//requireComponent<Velocity>();
	}

	virtual void update(EntityManager& entity_manager, float delta_seconds) override
	{
		std::shared_ptr<Pool<Position>> position_pool = entity_manager.getComponentPool<Position>();
		for (auto& entity : getMatchingEntities(entity_manager))
		{
			position_pool->data[entity.id].x += 1.f * delta_seconds;
			position_pool->data[entity.id].y += 2.f * delta_seconds;
		}
	}
};

int main()
{
	EntityManager entity_manager;
	Entity& entity = entity_manager.createEntity();
	entity_manager.addComponent(entity, Position(0, 0));
	entity_manager.addComponent(entity, Position(2, 1));
	MovementSystem movement_system;
	for (size_t i = 0; i < 10; ++i)
	{
		movement_system.update(entity_manager, 1.f);
		Position p = entity_manager.getComponent<Position>(entity);
		std::cout << p.x << " " << p.y << std::endl;
	}

	std::cout << ComponentMaskGetter<Position>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Velocity>::getComponentMask() << std::endl;
	
}