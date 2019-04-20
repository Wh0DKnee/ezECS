#pragma once
#include <vector>

// We want to have a vector of pools of different component types,
// so we need this base class.
//class PoolBase
//{
//public:
//	virtual void resize(size_t new_size) = 0;
//
//	virtual size_t size() = 0;
//};
//
//template<typename T>
//class Pool : public PoolBase
//{
//public:
//	std::vector<T> data;
//
//	virtual void resize(size_t new_size) override
//	{
//		data.resize(new_size);
//	}
//
//	virtual size_t size() override
//	{
//		return data.size();
//	}
//};
