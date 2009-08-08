#include "combinatorics.h"
#include "UnorderedRoll.h"

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace combinatorics;
using namespace dice;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

template<typename T>
static inline void checkEq(const string& label, const T& have, const T& want)
{
	if (have != want)
	{
		cerr << "Mismatch for " << label << ": have '" << have << "', "
			 << "wanted '" << want << "'." << endl;
		abort();
	}
}

template<typename T>
static inline void checkEq(const string& label,
						   const T have, const T want, const T fuzz)
{
	if ((want > (have + fuzz)) || (have > (want + fuzz)))
	{
		cerr << "Mismatch for " << label << ":" << endl
			 << "    have   " << have << "," << endl
			 << "    wanted " << want << "," << endl
			 << "    off by " << (have-want)
			 << " (limit = " << fuzz << ")." << endl;
		abort();
	}
}

static void test05_limits()
{
	for (int i = 0;  i <= 23;  ++i)
	{
		UnorderedRoll roll(i/10,     (i+1)/10, (i+2)/10, (i+3)/10, (i+4)/10,
						   (i+5)/10, (i+6)/10, (i+7)/10, (i+8)/10, (i+9)/10);
		cout << "  weight for " << roll.getNumDice() << " dice: "
			 << roll.getWeight() << endl;
	}
	cout << "test05_limits: OK" << endl;
}

static void test10_iterate_step(unsigned int dice)
{
	cout << "  (iterate, " << dice << " dice)" << endl;
	num_t elem = 0;
	typedef UnorderedRoll::weight_t weight_t;
	weight_t weight = 0;
	UnorderedRoll r = UnorderedRoll::begin(dice);
	bool keepGoing = true;
	while (keepGoing)
	{
		++elem;
		checkEq("dice of iterator", r.getNumDice(), dice);
		weight += r.getWeight();
		keepGoing = r.increment();
	}
	weight_t expectedFinal = rpower(0.1, r.getNumDice());
	checkEq("final weight", r.getWeight(), expectedFinal, expectedFinal*1e-12);
	checkEq("size of iterator", elem,
			nchoose(UnorderedRoll::NUM_SIDES - 1 + dice, dice));
	checkEq<weight_t>("weight of iterator", weight, 1, 1e-12);
}

static void test10_iterate()
{
	for (int i = 0;  i <= 10;  ++i)
	{
		test10_iterate_step(i);
	}
	test10_iterate_step(18);
	//test10_iterate_step(20);
	//test10_iterate_step(23);
	//test10_iterate_step(28);
	cout << "test10_iterate: OK" << endl;
}

static void test20_toString()
{
	UnorderedRoll obj0(1,1,1,1,1,1,1,1,1,1);
	UnorderedRoll obj1;
	UnorderedRoll obj2(0,1,2,3,4,5,0,0,0,0);

    checkEq("obj0.toString()", obj0.toString(),
			std::string("{ 10x1 9x1 8x1 7x1 6x1 5x1 4x1 3x1 2x1 1x1 }"));
    checkEq("obj1.toString()", obj1.toString(),
			std::string("{ 10x0 9x0 8x0 7x0 6x0 5x0 4x0 3x0 2x0 1x0 }"));
    checkEq("obj2.toString()", obj2.toString(),
			std::string("{ 10x0 9x0 8x0 7x0 6x5 5x4 4x3 3x2 2x1 1x0 }"));
	cout << "test20_toString: OK" << endl;
}

int main(int argc, char** argv)
{
	test05_limits();
	test10_iterate();
	test20_toString();
	return 0;
}
