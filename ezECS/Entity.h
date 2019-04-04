#pragma once
#include <stdint.h>
#include <deque>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include "Pool.h"
#include "Component.h"

class EntityManager;

using EntityID = int64_t;

struct Entity
{
	Entity(EntityID i, EntityManager& em) : id(i), entity_manager(em) {}
	EntityID id;

	// bitset that keeps track of what components an entity has
	ComponentMask component_mask;

	EntityManager& entity_manager;

	template<typename T>
	bool hasComponent();

	template<typename T>
	void addComponent(T component);
};

class EntityManager
{
public:
	EntityManager();

	Entity createEntity();

	// Need to unregister components before calling this.
	void deleteEntity(Entity e);

	template<typename T>
	void addComponent(Entity entity, T component);

private:
	// A given component pool can be found in the vector at the component's id.
	std::vector<std::shared_ptr<PoolBase>> components;

	std::deque<EntityID> free_IDs;
};

template<typename T>
bool Entity::hasComponent()
{
	return (ComponentMaskGetter<T>::getComponentMask() & component_mask).any();
}

template<typename T>
void Entity::addComponent(T component)
{
	if (hasComponent<T>()) 
	{
		std::cout << "This entity already has this component. Having multiple instances of a component is not supported currently." << std::endl;
		return;
	}
	component_mask |= ComponentMaskGetter<T>::getComponentMask();
	entity_manager.addComponent<T>(*this, component);
}

template<typename T>
void EntityManager::addComponent(Entity entity, T component)
{
	assert(components[ComponentMaskGetter<T>::getId()] != nullptr);
	std::static_pointer_cast<Pool<T>>(components[ComponentMaskGetter<T>::getId()])->data[entity.id] = component;
}
