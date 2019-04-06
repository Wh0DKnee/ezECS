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

	template<typename T>
	T& getComponent(Entity& entity);

	std::vector<Entity>& getEntities() { return entities; }

	template<typename T>
	std::shared_ptr<Pool<T>> getComponentPool();

	template<typename T>
	void registerComponent();

private:
	// A given component pool can be found in the vector at the component's id.
	// For example, the Pool<PositionComponent> will be at ComponentMaskGetter<PositionComponent>::getId()
	// in the component_pools.
	std::vector<std::shared_ptr<PoolBase>> component_pools;

	std::deque<EntityID> free_IDs;

	std::vector<Entity> entities;
};

template<typename T>
bool Entity::hasComponent() const
{
	return (ComponentMaskGetter<T>::getComponentMask() & component_mask).any();
}

template<typename T>
inline T& EntityManager::getComponent(Entity& entity)
{
	// TODO: don't assert but handle this case.
	assert(entity.component_mask.test(ComponentMaskGetter<T>::getId()));
	return getComponentPool<T>()->data[entity.id];
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
	auto pool = std::make_shared<Pool<T>>();
	pool->data.resize(INITIAL_POOL_SIZE);
	BaseComponentMaskGetter::Id component_id = ComponentMaskGetter<T>::getId();
	component_pools[component_id] = pool;
}
