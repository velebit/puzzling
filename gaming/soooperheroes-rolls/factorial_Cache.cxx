#include "factorial_Cache.h"

using namespace combinatorics;

template <typename T>
factorial::Cache<T>::Cache(int size)
	: m_size(size), m_cache(buildCache(size))
{
}

template <typename T>
factorial::Cache<T>::~Cache()
{
	delete[] m_cache;
}

template <typename T>
T* factorial::Cache<T>::buildCache(int size)
{
	assert(size > 1);
	T* cache = new T[size];
	cache[0] = 1;
	T current = 1;
	for (int i = 1;  i < size;  ++i)
	{
		current *= i;
		cache[i] = current;
	}
	return cache;
}

template class factorial::Cache<num_t>;
template class factorial::Cache<real_t>;
