#include "Grid.h"
#include "GridLines.h"
#include "checking.h"
#include <iostream>

void test00_construct_empty()
{
	Grid g;
	for (int i = 0;  i < 8;  ++i)
	{
		for (int j = 0;  j < 8;  ++j)
		{
			checkTrue("all grid slots are empty", ! g.isSet(i, j));
		}
	}
}

void test10_single_element_grid()
{
	Grid g;
	g.set(0,0, 1);

	checkTrue("one grid slot is filled",   g.isSet(0, 0));
	checkTrue("one grid slot is set to 1", g.get(0, 0) == 1);

	for (int i = 0;  i < 8;  ++i)
	{
		for (int j = 0;  j < 8;  ++j)
		{
			if (! (i == 0 && j == 0))
			{
				checkTrue("all but one grid slots are empty", ! g.isSet(i, j));
				if (i == 0 || j == 0 || i == j)
				{
					checkTrue("these 3 lines can't get a 1",
							  ! g.canBeSetTo(i, j, 1));
				}
				else
				{
					checkTrue("outside the 3 lines you can get a 1",
							  g.canBeSetTo(i, j, 1));
				}
			}
		}
	}
}

class SetElement
{
public:
	SetElement(Grid& grid, int value) : m_grid(grid), m_value(value) {};

	void operator() (int r, int c) { m_grid.setRaw(r, c, m_value); }

private:
	Grid& m_grid;
	int m_value;
};

static void do_test50_grid_lines(int r, int c, int v)
{
	Grid g;
	SetElement setElement(g, v);
	forOthersInLines(r, c, setElement);
	//std::cout << g << std::endl;
	for (int i = 0;  i < 8;  ++i)
	{
		for (int j = 0;  j < 8;  ++j)
		{
			if ((i == r) && (j == c))
			{
				checkTrue("center is unset", ! g.isSet(i, j));
			}
			else if ( (i == r) || (j == c)
					  || ((i-j) == (r-c)) || ((i+j) == (r+c)) )
			{
				checkTrue("lines are set", g.isSet(i, j));
			}
			else
			{
				checkTrue("background is unset", ! g.isSet(i, j));
			}
		}
	}
}

void test50_grid_lines()
{
	do_test50_grid_lines(3, 4,  2);
	do_test50_grid_lines(1, 1,  5);
	do_test50_grid_lines(6, 6,  6);
	do_test50_grid_lines(1, 6,  7);
	do_test50_grid_lines(6, 1,  8);
}

int main(int argc, char** argv)
{
	test00_construct_empty();
	test10_single_element_grid();
	test50_grid_lines();
	return 0;
}
