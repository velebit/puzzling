#include "factorial.h"
#include "factorial_Cache.h"

namespace combinatorics
{
	// An internal cache of all factorials representable as 64-bit integers.
	static const factorial::Cache<num_t> FACTORIAL_CACHE_N(MAX_FACTORIAL+1);

	// An internal cache of the first 1024 factorials represented as doubles.
	static const factorial::Cache<real_t> FACTORIAL_CACHE_R(1024);

	// Internal factorial implementations: fetch results from the cache.
	static inline num_t _nfact(int n)
	{
		return FACTORIAL_CACHE_N.get(n);
	}
	static inline real_t _rfact(int n)
	{
		return FACTORIAL_CACHE_R.get(n);
	}

	// factorials!
	num_t nfact(int n)
	{
		return _nfact(n);
	}

	real_t rfact(int n)
	{
		return _rfact(n);
	}

	// number of combinations (N choose K)
	num_t nchoose(int n, int k)
	{
		if (n <= MAX_FACTORIAL)
		{
			// If the set size is small enough (so that n! is representable
			// as a num_t), make use of the cache to calculate the result in
			// 1 multiplication and 1 division.
			return ( _nfact(n) / (_nfact(n-k) * _nfact(k)) );
		}

		// If n! is too big, we can still calculate the result less
		// efficiently by eliminating redundant multiplications and
		// interspersing the divisions with the multiplications.
		// Eventually we still overrun a num_t, but not till n == 63
		// (assuming num_t is a 64-bit unsigned integer).
		num_t result = 1;
		for (int i = 0;  i < k;  ++i)
		{
			result = result * (n-i) / (1+i);
		}
		return result;
	}

	real_t rchoose(int n, int k)
	{
		return ( _rfact(n) / (_rfact(n-k) * _rfact(k)) );
	}

	// Internal exponentiation implementation.
	template <typename T>
	inline T _power(T base, unsigned int exponent)
	{
		T result = static_cast<T>(1);
		T pow2   = base;
		while (1)
		{
			if (exponent & 0x1)
			{
				result *= pow2;
			}
			exponent = exponent >> 1;
			if (! exponent)
			{
				break;
			}
			pow2 = pow2 * pow2;
		}
		return result;
	}

	// exponentiation
	num_t  npower(num_t base, unsigned int exponent)
	{
		return _power(base, exponent);
	}

	real_t rpower(real_t base, unsigned int exponent)
	{
		return _power(base, exponent);
	}
};
