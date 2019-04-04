#pragma once
#include <vector>

class PoolBase{};

template<typename T>
class Pool : public PoolBase
{
public:
	std::vector<T> data;
};
