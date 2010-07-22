#ifndef GRID_SOLVER__H
#define GRID_SOLVER__H

#include "Grid.h"
#include "Location.h"
#include "LocationSequenceBuilder.h"

#include <iostream>
#include <vector>

class GridSolver
{
private:
	std::vector<Location> m_locationSequence;
	std::vector<Grid>     m_gridStack;
	int                   m_bestBlanksSoFar;

public:
	GridSolver(int maxBlanks = 9999)
		: m_locationSequence(LocationSequenceBuilder().getLocations()),
		  m_bestBlanksSoFar(maxBlanks)
	{
		buildInitialGrid();
		std::cout << m_gridStack[0] << std::endl;
	}

	void solve()
	{
		trySomeNumbers(0, 0);
	}

private:
	void buildInitialGrid()
	{
		m_gridStack.resize(m_locationSequence.size() + 1);

		Grid& g = m_gridStack[0];

		g = Grid();

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

	void trySomeNumbers(int depth, int blanks)
	{
		if (depth == (int) m_locationSequence.size())
		{
			if (blanks > m_bestBlanksSoFar)
			{
				return;
			}
			else if (blanks == m_bestBlanksSoFar)
			{
				std::cout << "*** SOLUTION (tied = " << blanks << ") ***" << std::endl;
			}
			else
			{
				std::cout << "*** SOLUTION (new best = " << blanks << ") ***" << std::endl;
				m_bestBlanksSoFar = blanks;
			}
			std::cout << m_gridStack[depth] << std::endl;
			return;
		}

		assert(depth < (int) m_locationSequence.size());
		if (depth <= 6) // || depth >= 40)
		{
			std::cout.width(depth+2);
			std::cout << depth << std::endl;
		}

		const int r = m_locationSequence[depth].row();
		const int c = m_locationSequence[depth].column();

		if (m_gridStack[depth].isBlank(r, c))
		{
			m_gridStack[depth+1] = m_gridStack[depth];
			// this blank was already accounted for-- the budget stays the same
			trySomeNumbers(depth+1, blanks);
			return;
		}
		assert(m_gridStack[depth].isEmpty(r, c));

		Grid& gOld = m_gridStack[depth];
		Grid& gNew = m_gridStack[depth+1];

		for (int i = 1;  i <= 8;  ++i)
		{
			if (gOld.canBeSetTo(r, c, i))
			{
				gNew = gOld;
				int updatedBlanks = blanks + gNew.set(r, c, i);
				//assert(countBlanks(gNew) == updatedBlanks);
				if (updatedBlanks > m_bestBlanksSoFar)
					continue;
				trySomeNumbers(depth+1, updatedBlanks);
			}
		}

		if (blanks < m_bestBlanksSoFar)
		{
			gNew = gOld;
			gNew.setToBlank(r, c);
			//assert(countBlanks(gNew) == blanks+1);
			trySomeNumbers(depth+1, blanks+1);
		}
	}
};

#endif // GRID_SOLVER__H
