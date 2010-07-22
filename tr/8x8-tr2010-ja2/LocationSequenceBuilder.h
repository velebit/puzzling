#ifndef LOCATION_SEQUENCE_BUILDER__H
#define LOCATION_SEQUENCE_BUILDER__H

#include "Location.h"
#include "Grid.h"

#include <iostream>
#include <vector>
#include <algorithm>

class LocationSequenceBuilder
{
private:
	std::vector<Location> m_locationSequence;
	Grid                  m_bookkeepingGrid;

public:
	LocationSequenceBuilder()
	{
		buildLocationSequence();
	}

	std::vector<Location> getLocations() const { return m_locationSequence; }

private:
	void maybeAddToLocationSequence(Location loc, int dummyValue)
	{
		if (m_bookkeepingGrid.isEmpty(loc.row(), loc.column()))
		{
			m_bookkeepingGrid.setRaw(loc.row(), loc.column(), dummyValue);
			m_locationSequence.push_back(loc);
		}
	}

	void buildLocationSequence()
	{
		const int numRows = Grid::NUM_ROWS;
		const int numCols = Grid::NUM_COLUMNS;
		const int numBoth = std::min(numRows, numCols);

		for (int i = 0;  i < numCols;  ++i)
		{
			m_bookkeepingGrid.setRaw(numRows-1,i, 1);
		}
		for (int i = 0;  i < numCols;  ++i)
		{
			maybeAddToLocationSequence(Location(0, i), 2);
		}
		for (int i = 0;  i < numBoth;  ++i)
		{
			maybeAddToLocationSequence(Location(i, i), 3);
		}
		for (int i = 0;  i < numBoth;  ++i)
		{
			maybeAddToLocationSequence(Location(i, numCols-1-i), 4);
		}
		for (int i = 0;  i < numRows;  ++i)
		{
			maybeAddToLocationSequence(Location(i, 0), 5);
		}
		for (int i = 0;  i < numRows;  ++i)
		{
			maybeAddToLocationSequence(Location(i, numCols-1), 6);
		}
		for (int i = 0;  i < numRows;  ++i)
		{
			for (int j = 0;  j < numCols;  ++j)
			{
				if (i == 2 || i == 5 || j == 2 || j == 5)
					maybeAddToLocationSequence(Location(i, j), 7);
			}
		}
		for (int i = 0;  i < numRows;  ++i)
		{
			for (int j = 0;  j < numCols;  ++j)
			{
				maybeAddToLocationSequence(Location(i, j), 8);
			}
		}
		std::cout << m_bookkeepingGrid << std::endl;
		std::cout << m_locationSequence.size() << std::endl << std::endl;
	}
};

#endif // LOCATION_SEQUENCE_BUILDER__H
