#pragma once
#include "Component.h"
#include "Entity.h"
#include <vector>

class EntityManager;

class System
{
public:
	virtual void update(EntityManager& entity_manager, float delta_seconds) = 0;
	template<typename T>
	void requireComponent();

protected:
	std::vector<Entity> getMatchingEntities(EntityManager& entity_manager);

private:
	ComponentMask component_mask;
};

class MovementSystem : public System
{
public:
	MovementSystem();

	virtual void update(EntityManager& entity_manager, float delta_seconds) override;
};

template<typename T>
void System::requireComponent()
{
	component_mask |= ComponentMaskGetter<T>::getComponentMask();
}
