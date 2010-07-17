#include "Grid.h"

#include "GridLines.h"
#include "CellValueSet.h"

// ============================================================

class AddConflictingElements
{
public:
	AddConflictingElements(Grid& grid, int value)
		: m_grid(grid), m_value(value), m_okay(true) {};

	void operator() (int r, int c)
	{
		if (! m_grid.setOneConflictingElement(r, c, m_value))
			m_okay = false;
	}

	bool isFullGridPossible() const { return m_okay; }

private:
	Grid& m_grid;
	int   m_value;
	bool  m_okay;
};

bool Grid::addConflictingElements(int r, int c, int value)
{
	AddConflictingElements addConflicts(*this, value);
	forOthersInLines(r, c, addConflicts);
	return addConflicts.isFullGridPossible();
}

// ============================================================

class CollectConflictingElements
{
public:
	CollectConflictingElements(const Grid& grid) : m_grid(grid) {};

	void operator() (int r, int c)
	{
		if (m_grid.isSet(r, c))
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
			if (! grid.isSet(r, c))
			{
				stream << ".";
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
