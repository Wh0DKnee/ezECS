#pragma once
#include <stdint.h>
#include <deque>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include "ComponentPool.h"
#include "Component.h"
#include "CommonTypes.h"

// An entity is an ID. We also keep track of what components an entity has with a bitmask.
struct Entity
{
	Entity(EntityID id) : id(id){}
	EntityID id;

	// bitset that keeps track of what components an entity has
	ComponentMask component_mask;

	template<typename T>
	bool hasComponent() const;

	bool hasComponents(ComponentMask required_components_mask) const;
};

inline bool operator ==(const Entity& lhs, const Entity& rhs)
{
	return lhs.id == rhs.id;
}

class EntityManager
{
public:
	EntityManager();

	Entity& createEntity();

	void deleteEntity(const Entity& e);

	template<typename T>
	void addComponent(Entity& entity, T component);

	// NOTE: Only call this if you know that the entity has this component.
	// If you do not know, call getComponentSafe() instead.
	template<typename T>
	T& getComponent(Entity& entity);

	template<typename T>
	T* getComponentSafe(Entity& entity);

	std::vector<Entity>& getEntities() { return entities_; }

	template<typename T>
	std::shared_ptr<ComponentPool<T>> getComponentPool();

	// Allocates a component pool for the specified component type. 
	template<typename T>
	std::shared_ptr<ComponentPool<T>> initializeComponentPool();

private:

	std::vector<std::shared_ptr<PoolBase>> component_pools_;

	std::deque<EntityID> free_ids_;

	std::vector<Entity> entities_;

	// grows as needed
	int current_pool_size_;

	int next_free_id_;

	void resizePoolsAndGenerateFreeIds();
};

template<typename T>
bool Entity::hasComponent() const
{
	return (ComponentMaskGetter<T>::getComponentMask() & component_mask).any();
}

template<typename T>
T& EntityManager::getComponent(Entity& entity)
{
	assert(entity.component_mask.test(ComponentMaskGetter<T>::getId()));
	return getComponentPool<T>()->getComponent(entity.id);
}

template<typename T>
T* EntityManager::getComponentSafe(Entity & entity)
{
	auto pool = getComponentPool<T>();
	if (pool == nullptr)
	{
		return nullptr;
	}
	if (entity.id >= pool->data.size())
	{
		return nullptr;
	}
	return pool->data[entity.id];
}

template<typename T>
void EntityManager::addComponent(Entity& entity, T component)
{
	if (entity.hasComponent<T>())
	{
		std::cout << "This entity already has this component. Having multiple instances of a component is not supported currently." << std::endl;
		return;
	}
	entity.component_mask |= ComponentMaskGetter<T>::getComponentMask();
	std::shared_ptr<ComponentPool<T>> component_pool = getComponentPool<T>();
	if (component_pool == nullptr)
	{
		component_pool = initializeComponentPool<T>();
	}
	assert(component_pool != nullptr);
	component_pool->push_back(entity.id, component);
}

template<typename T>
inline std::shared_ptr<ComponentPool<T>> EntityManager::getComponentPool()
{
	return std::static_pointer_cast<ComponentPool<T>>(component_pools_[ComponentMaskGetter<T>::getId()]);
}

template<typename T>
std::shared_ptr<ComponentPool<T>> EntityManager::initializeComponentPool()
{
	auto pool = std::make_shared<ComponentPool<T>>();
	BaseComponentMaskGetter::Id component_id = ComponentMaskGetter<T>::getId();
	component_pools_[component_id] = pool;
	return pool;
}
