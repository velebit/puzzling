#include "combinatorics.h"

#include <stdio.h>

using namespace combinatorics;

int main(int argc, char** argv)
{
	if (0)
	{
		printf("start...\n");
		for (int i = 0;  i < (100*1000000);  ++i)
		{
			nchoose(20,10);
		}
		printf("...end\n");
		return 0;
	}

	if (1)
	{
		printf("%4d %25llu\n", 0, nfact(0));
		for (int i = 1;  i <= MAX_FACTORIAL;  ++i)
		{
			printf("%4d %25llu  (ratio: %6.2f)\n", i, nfact(i),
				   (static_cast<double>(nfact(i)) /
					static_cast<double>(nfact(i-1)) ));
		}
	}

	if (1)
	{
		printf("%4d %4d %20llu\n", 0, 0, nchoose(0, 0));
		for (int i = 1;  i <= 62;  ++i)
		{
			printf("%4d %4d %20llu  (ratio: %6.2f)\n", i, i/2, nchoose(i, i/2),
				   ( static_cast<double>(nchoose(i, i/2))
					 / static_cast<double>(nchoose(i-1, (i-1)/2)) ));
		}
	}

	if (0)
	{
		num_t power = 1;
		printf("%4d %25llu\n", 0, power);
		for (int i = 1;  i <= 19;  ++i)
		{
			num_t last = power;
			power *= 10;
			printf("%4d %25llu  (ratio: %6.2f)\n", i, power,
				   static_cast<double>(power)/static_cast<double>(last));
		}
	}

	return 0;
}
