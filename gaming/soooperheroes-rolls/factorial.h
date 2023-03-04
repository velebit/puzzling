#ifndef FACTORIAL__H
#define FACTORIAL__H

#include "combinatorics_common.h"

namespace combinatorics
{
	// Maximum value whose factorial fits in a 32-bit unsigned integer
	//static const int MAX_FACTORIAL = 12;

	// Maximum value whose factorial fits in a num_t (64-bit unsigned integer)
	static const int MAX_FACTORIAL = 20;

	// factorials!
	num_t  nfact(int n);
	real_t rfact(int n);

	// number of combinations (N choose K)
	num_t  nchoose(int n, int k);
	real_t rchoose(int n, int k);

	// exponentiation
	num_t  npower(num_t base, unsigned int exponent);
	real_t rpower(real_t base, unsigned int exponent);
};

#endif // FACTORIAL__H
