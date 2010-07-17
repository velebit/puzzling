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
	if (! g.isSet(loc.row(), loc.column()))
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
		g.setRaw(0,i, 1);
	}
	for (int i = 0;  i < numCols;  ++i)
	{
		maybeAddToLocationSequence(g, Location(numRows-1, i), 5);
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
	cout << g_locationSequence.size() << endl;
}

static void buildInitialGrid()
{
	g_gridStack.resize(g_locationSequence.size() + 1);

	Grid& g = g_gridStack[0];
	g = Grid();
	for (int i = 0;  i < Grid::NUM_COLUMNS;  ++i)
	{
		g.set(0,i, i+1);
	}
}

static bool trySomeNumbers(int depth)
{
	assert(depth < (int) g_locationSequence.size());
	if (depth < 4 || depth >= 35)
	{
		cout.width(depth+2);
		cout << depth << endl;
	}

	const int r = g_locationSequence[depth].row();
	const int c = g_locationSequence[depth].column();
	assert(! g_gridStack[depth].isSet(r, c));

	Grid& gOld = g_gridStack[depth];
	Grid& gNew = g_gridStack[depth+1];

	for (int i = 1;  i <= 8;  ++i)
	{
		if (gOld.canBeSetTo(r, c, i))
		{
			gNew = gOld;
			if (! gNew.set(r, c, i))
				continue;
			if (depth >= 50)
				cout << gNew << endl;
			trySomeNumbers(depth+1);
		}
	}

	//g = Grid();  // wipe the old value!
	return false;
}

int main(int argc, char** argv)
{
	buildLocationSequence();
	buildInitialGrid();

	trySomeNumbers(0);
}
