#pragma once
#include <vector>

// We want to have a vector of pools of different component types,
// so we need this base class.
class PoolBase{};

template<typename T>
class Pool : public PoolBase
{
public:
	std::vector<T> data;
};
