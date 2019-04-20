#pragma once
#include "Component.h"
#include "Entity.h"
#include <vector>

class EntityManager;

// The base system class. Your systems should inherit from this.
class System
{
public:
	virtual void update(EntityManager& entity_manager, float delta_seconds) = 0;
	template<typename T>
	void requireComponent();

protected:
	std::vector<Entity> getMatchingEntities(EntityManager& entity_manager);

private:
	ComponentMask component_mask_;
};

template<typename T>
void System::requireComponent()
{
	component_mask_ |= ComponentMaskGetter<T>::getComponentMask();
}
