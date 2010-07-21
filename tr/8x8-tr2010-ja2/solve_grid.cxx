#include "Grid.h"
#include "GridLines.h"
#include "Location.h"

#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;

std::vector<Location> g_locationSequence;
std::vector<Grid>     g_gridStack;

static void maybeAddToLocationSequence(Grid& g, Location loc, int value)
{
	if (g.isEmpty(loc.row(), loc.column()))
	{
		g.setRaw(loc.row(), loc.column(), value);
		g_locationSequence.push_back(loc);
	}
}

static void buildLocationSequence()
{
	const int numRows = Grid::NUM_ROWS;
	const int numCols = Grid::NUM_COLUMNS;
	const int numBoth = std::min(numRows, numCols);

	Grid g;
	for (int i = 0;  i < numCols;  ++i)
	{
		g.setRaw(numRows-1,i, 1);
	}
	for (int i = 0;  i < numCols;  ++i)
	{
		maybeAddToLocationSequence(g, Location(0, i), 5);
	}
	for (int i = 0;  i < numBoth;  ++i)
	{
		maybeAddToLocationSequence(g, Location(i, i), 2);
	}
	for (int i = 0;  i < numBoth;  ++i)
	{
		maybeAddToLocationSequence(g, Location(i, numCols-1-i), 3);
	}
	for (int i = 0;  i < numRows;  ++i)
	{
		maybeAddToLocationSequence(g, Location(i, 0), 4);
	}
	for (int i = 0;  i < numRows;  ++i)
	{
		maybeAddToLocationSequence(g, Location(i, numCols-1), 4);
	}
	for (int i = 0;  i < numRows;  ++i)
	{
		for (int j = 0;  j < numCols;  ++j)
		{
			if (i == 2 || i == 5 || j == 2 || j == 5)
				maybeAddToLocationSequence(g, Location(i, j), 6);
		}
	}
	for (int i = 0;  i < numRows;  ++i)
	{
		for (int j = 0;  j < numCols;  ++j)
		{
			maybeAddToLocationSequence(g, Location(i, j), 8);
		}
	}
	cout << g << endl;
	cout << g_locationSequence.size() << endl << endl;
}

static void buildInitialGrid()
{
	g_gridStack.resize(g_locationSequence.size() + 1);

	Grid& g = g_gridStack[0];

	g = Grid();
	//if (blankIndex1 >= 0)
	//	g.setToBlank(blankIndex1 / Grid::NUM_COLUMNS,
	//				 blankIndex1 % Grid::NUM_COLUMNS);
	//g.setToBlank(2,3);
	//g.setToBlank(2,4);
	//g.setToBlank(3,2);
	//g.setToBlank(3,5);
	//g.setToBlank(4,2);
	//g.setToBlank(4,5);

	for (int i = 0;  i < Grid::NUM_COLUMNS;  ++i)
	{
		if (g.isEmpty(Grid::NUM_ROWS-1, i))
			g.set(Grid::NUM_ROWS-1, i, i+1);
	}
}

static int countBlanks(const Grid& g)
{
	int count = 0;

	for (int i = 0;  i < Grid::NUM_ROWS;  ++i)
	{
		for (int j = 0;  j < Grid::NUM_COLUMNS;  ++j)
		{
			if (g.isBlank(i, j))
				++count;
		}
	}

	return count;
}

static void trySomeNumbers(int depth, int blankBudget)
{
	if (depth == (int) g_locationSequence.size())
	{
		cout << "*** SOLUTION ***" << endl;
		cout << g_gridStack[depth] << endl;
		return;
	}

	assert(depth < (int) g_locationSequence.size());
	if (depth <= 6) // || depth >= 40)
	{
		cout.width(depth+2);
		cout << depth << endl;
	}

	const int r = g_locationSequence[depth].row();
	const int c = g_locationSequence[depth].column();

	if (g_gridStack[depth].isBlank(r, c))
	{
		g_gridStack[depth+1] = g_gridStack[depth];
		// this blank was already accounted for-- the budget stays the same
		trySomeNumbers(depth+1, blankBudget);
		return;
	}
	assert(g_gridStack[depth].isEmpty(r, c));

	Grid& gOld = g_gridStack[depth];
	Grid& gNew = g_gridStack[depth+1];

	for (int i = 1;  i <= 8;  ++i)
	{
		if (gOld.canBeSetTo(r, c, i))
		{
			gNew = gOld;
			int extraBlanks = gNew.set(r, c, i);
			assert(countBlanks(gNew) == (extraBlanks + (3 - blankBudget)));
			if (extraBlanks > blankBudget)
				continue;
			//if (depth >= 50)
			//	cout << gNew << endl;
			trySomeNumbers(depth+1, blankBudget - extraBlanks);
		}
	}

	if (blankBudget > 0)
	{
		gNew = gOld;
		gNew.setToBlank(r, c);
		trySomeNumbers(depth+1, blankBudget-1);
	}
}

int main(int argc, char** argv)
{
	buildLocationSequence();
	buildInitialGrid();
	cout << g_gridStack[0] << endl;
	trySomeNumbers(0, 3);
}
