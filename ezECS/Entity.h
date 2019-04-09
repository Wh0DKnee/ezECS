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

using EntityID = unsigned int;

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

	std::vector<Entity>& getEntities() { return entities; }

	template<typename T>
	std::shared_ptr<Pool<T>> getComponentPool();

	// Allocates a component pool for the specified component type. 
	template<typename T>
	void registerComponent();

private:
	// A given component pool can be found in the vector at the component's id.
	// For example, the Pool<PositionComponent> will be at ComponentMaskGetter<PositionComponent>::getId()
	// in the component_pools.
	std::vector<std::shared_ptr<PoolBase>> component_pools;

	std::deque<EntityID> free_ids;

	std::vector<Entity> entities;

	// grows as needed
	int current_pool_size;

	int next_free_id;

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
	return getComponentPool<T>()->data[entity.id];
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
	std::shared_ptr<Pool<T>> component_pool = getComponentPool<T>();
	if (component_pool == nullptr)
	{
		registerComponent<T>();
		component_pool = getComponentPool<T>();
	}
	assert(component_pool != nullptr);
	component_pool->data[entity.id] = component;
}

template<typename T>
inline std::shared_ptr<Pool<T>> EntityManager::getComponentPool()
{
	return std::static_pointer_cast<Pool<T>>(component_pools[ComponentMaskGetter<T>::getId()]);
}

template<typename T>
inline void EntityManager::registerComponent()
{
	/*
		Currently, a component pool is allocated even if only one instance of this component exists.
		This means that for components that are not frequently used, there will be quite a lot of
		wasted memory in its component pool. The entity ID is used as in index into the pool to find
		that entity's component, if it has it.

		This approach means that systems that require only a single component will be able to iterate
		over all of these components very fast, as they are contiguous in memory.

		Storing components in an ECS is not a trivial problem, and usually needs optimization on a case-by-case basis.
		For example: If we find that system X that requires components A, B and C (that are conceptually related in a way)
		occupies a large portion of our frame time, it is worth considering to join A, B and C into one component, 
		so that system X can iterate over them significantly faster, as they would then be contiguous in memory and we 
		would be very cache-friendly.

		I will try different pooling strategies in the future, but for now this simple approach will suffice for very basic
		games and for study purposes.
	*/
	auto pool = std::make_shared<Pool<T>>();
	// TODO: Once we've done the TODO in createEntity, this will also need to change to cope with
	// an increase in entities beyond INITIAL_POOL_SIZE.
	pool->data.resize(current_pool_size); 
	BaseComponentMaskGetter::Id component_id = ComponentMaskGetter<T>::getId();
	component_pools[component_id] = pool;
}
