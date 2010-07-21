#include "Grid.h"

#include "GridLines.h"
#include "CellValueSet.h"

// ============================================================

class AddConflictingElements
{
public:
	AddConflictingElements(Grid& grid, int value)
		: m_grid(grid), m_value(value), m_numNewBlanks(0) {};

	void operator() (int r, int c)
	{
		if (! m_grid.setOneConflictingElement(r, c, m_value))
		{
			assert(! m_grid.isBlank(r, c));
			m_grid.setToBlank(r, c);
			++m_numNewBlanks;
		}
	}

	int getNumNewBlanks() const { return m_numNewBlanks; }

private:
	Grid&     m_grid;
	const int m_value;
	int       m_numNewBlanks;
};

int Grid::markConflictingElements(int r, int c, int value)
{
	AddConflictingElements addConflicts(*this, value);
	forOthersInLines(r, c, addConflicts);
	return addConflicts.getNumNewBlanks();
}

// ============================================================

class CollectConflictingElements
{
public:
	CollectConflictingElements(const Grid& grid) : m_grid(grid) {};

	void operator() (int r, int c)
	{
		if (m_grid.hasValue(r, c))
		{
			m_set.add(m_grid.get(r, c));
		}
	}

	CellValueSet getResult() const { return m_set; }

private:
	const Grid&  m_grid;
	CellValueSet m_set;
};

CellValueSet Grid::recomputeConflictingElements(int r, int c) const
{
	CollectConflictingElements collectConflicts(*this);
	forOthersInLines(r, c, collectConflicts);
	return collectConflicts.getResult();
}

// ============================================================

std::ostream& operator<< (std::ostream& stream, const Grid& grid)
{
	std::streamsize oldWidth = stream.width();
	for (int r = 0;  r < Grid::NUM_ROWS;  ++r)
	{
		for (int c = 0;  c < Grid::NUM_COLUMNS;  ++c)
		{
			stream.width(2);
			if (grid.isEmpty(r, c))
			{
				stream << ".";
			}
			else if (grid.isBlank(r, c))
			{
				stream << "~";
			}
			else
			{
				stream << static_cast<int>(grid.get(r, c));
			}
		}
		stream << std::endl;
	}
	stream.width(oldWidth);
	return stream;
}
