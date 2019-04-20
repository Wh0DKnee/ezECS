#pragma once
#include <cassert>
#include "SparseSet.h"
#include "CommonTypes.h"

// We want to have a vector of pools of different component types,
// so we need this base class.
class PoolBase
{
public:
	//virtual void resize(size_t new_size) = 0;

	//virtual size_t size() = 0;
};

template<typename TComponent>
class ComponentPool : public PoolBase
{
private:
	std::vector<TComponent> data_;
	SparseSet<EntityID> sparse_set_;

public:

	void reserve(size_t new_capacity)
	{
		data_.reserve(new_capacity);
		sparse_set_.reserve(new_capacity);
	}

	void push_back(EntityID id, TComponent component)
	{
		data_.push_back(component);
		sparse_set_.add(id);
		assert(sparse_set_.size() == data_.size());
	}

	TComponent& getComponent(EntityID entity_id) { return data_[sparse_set_.sparse_[entity_id]]; }
};

