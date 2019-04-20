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

	std::vector<T> dense_;
	std::vector<T> sparse_;

	size_t size_ = 0;		// current number of elements
	size_t capacity_ = 0;

public:

	size_t size() const { return size_; }

	size_t capacity() const { return capacity_; }

	bool empty() const { return size_ == 0; }

	bool clear() { size_ = 0; }

	void reserve(size_t new_capacity)
	{
		if (new_capacity > capacity_)
		{
			dense_.resize(new_capacity);
			sparse_.resize(new_capacity);
			capacity_ = new_capacity;
		}
	}

	bool contains(const T& val) const
	{
		return val < capacity_ && sparse_[val] < size_ && dense_[sparse_[val]] == val;
	}

	void add(const T& val)
	{
		if (contains(val))
		{
			return;
		}

		if (val >= capacity_)
		{
			reserve(val + 1);
		}

		sparse_[val] = size_;
		dense_[size_] = val;
		++size_;
	}

	void erase(const T& val)
	{
		if (contains(val))
		{
			dense_[sparse_[val]] = dense_[size_ - 1];	// overwrite to-be-deleted element with current last element
			sparse_[dense_[size_ - 1]] = sparse_[val];  // update corresponding index in sparse vector
			--size_;
		}
	}

	template<typename> friend class ComponentPool;
};