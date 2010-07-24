#ifndef GRID_SOLVER__H
#define GRID_SOLVER__H

#include "Grid.h"
#include "Location.h"
#include "LocationSequence.h"

#include <iostream>
#include <vector>

class GridSolver
{
private:
	const std::vector<Location>& m_locationSequence;
	std::vector<Grid>            m_gridStack;
	int                          m_finalDepth;
	int                          m_maxAllowedBlanks;

public:
	GridSolver(int maxBlanks = 9999)
		: m_locationSequence(LocationSequence::get()),
		  m_finalDepth(0),
		  m_maxAllowedBlanks(maxBlanks)
	{
		m_finalDepth = static_cast<int>( m_locationSequence.size() );
		buildInitialGrid();
		std::cout << m_gridStack[0] << std::endl;
	}

	~GridSolver() {};

	void solve()
	{
		trySomeNumbers(0, 0, countBlanks(m_gridStack[0]));
	}

private:
	void buildInitialGrid()
	{
		m_gridStack.resize(m_finalDepth + 1);

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

	void foundResult(const Grid& grid, int blanks)
	{
		if (blanks > m_maxAllowedBlanks)
		{
			return;
		}
		else if (blanks == m_maxAllowedBlanks)
		{
			std::cout << "*** SOLUTION (tied = " << blanks << ") ***" << std::endl;
		}
		else
		{
			std::cout << "*** SOLUTION (new best = " << blanks << ") ***" << std::endl;
			m_maxAllowedBlanks = blanks;
		}
		std::cout << grid << std::endl;
	}

	void trySomeNumbers(int depth, int locationIndex, int blanks)
	{
		if (depth == m_finalDepth)
		{
			foundResult(m_gridStack[depth], blanks);
			return;
		}

		if (depth <= 6) // || depth >= 40)
		{
			std::cout.width(depth+3);
			std::cout << locationIndex << std::endl;
		}

		//assert(depth < m_finalDepth);
		assert(locationIndex < m_locationSequence.size());
		const int r = m_locationSequence[locationIndex].row();
		const int c = m_locationSequence[locationIndex].column();

		if (m_gridStack[depth].isBlank(r, c))
		{
			m_gridStack[depth+1] = m_gridStack[depth];
			// this blank was already accounted for-- the budget stays the same
			trySomeNumbers(depth+1, locationIndex+1, blanks);
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
				if (updatedBlanks > m_maxAllowedBlanks)
					continue;
				trySomeNumbers(depth+1, locationIndex+1, updatedBlanks);
			}
		}

		if (blanks < m_maxAllowedBlanks)
		{
			gNew = gOld;
			gNew.setToBlank(r, c);
			//assert(countBlanks(gNew) == blanks+1);
			trySomeNumbers(depth+1, locationIndex+1, blanks+1);
		}
	}
};

#endif // GRID_SOLVER__H
