#include <cstdint>
#include <iostream>
#include <cassert>

#include "Fingers.h"
#include "Hands.h"
#include "Players.h"


static void TestAll()
{
	TestFingers();
	TestHands();
	TestPlayers();
}


int main()
{
	TestAll();
}
