// Implementation of a sparse set: https://programmingpraxis.com/2012/03/09/sparse-sets/
// Inspired by Sam Griffiths' implementation here: https://gist.github.com/sjgriffiths/06732c6076b9db8a7cf4dfe3a7aed43a

#pragma once

#include <vector>
#include <type_traits>

template<typename T>
class SparseSet
{
	static_assert(std::is_unsigned<T>::value, "SparseSet only works for unsigned integers.");

private:

	std::vector<T> dense;
	std::vector<T> sparse;

	size_t size = 0;		// current number of elements
	size_t capacity = 0;

public:

	size_t size() const { return size; }

	size_t capacity() const { return capacity; }

	bool empty() const { return size == 0; }

	bool clear() { size = 0; }

	void reserve(size_t new_capacity)
	{
		if (new_capacity > capacity)
		{
			dense.resize(new_capacity);
			sparse.resize(new_capacity);
			capacity = new_capacity;
		}
	}

	bool contains(const T& val) const
	{
		return val < capacity && sparse[val] < size && dense[sparse[val]] == val;
	}

	void add(const T& val)
	{
		if (contains(val))
		{
			return;
		}

		if (val > capacity)
		{
			reserve(val + 1);
		}

		sparse[val] = size;
		dense[size] = val;
		++size;
	}

	void erase(const T& val)
	{
		if (contains(val))
		{
			dense[sparse[val]] = dense[size - 1];	// overwrite to-be-deleted element with current last element
			sparse[dense[size - 1]] = sparse[val];  // update corresponding index in sparse vector
			--size;
		}
	}
};