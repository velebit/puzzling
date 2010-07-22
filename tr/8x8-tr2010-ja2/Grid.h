#ifndef GRID__H
#define GRID__H

#include <assert.h>
#include <ostream>

#include "CellValueSet.h"

class Grid
{
public:
	static const int NUM_ROWS    = 8;
	static const int NUM_COLUMNS = 8;
	typedef unsigned char value_type;

public:
	// Create an empty grid
	Grid()
	{
		for (int i = 0;  i < NUM_ROWS*NUM_COLUMNS;  ++i)
		{
			m_elements[i] = EMPTY;
		}
	}

	// Copy constructor
	Grid(const Grid& grid)
	{
		for (int i = 0;  i < NUM_ROWS*NUM_COLUMNS;  ++i)
		{
			m_elements[i] = grid.m_elements[i];
		}
		for (int i = 0;  i < NUM_ROWS*NUM_COLUMNS;  ++i)
		{
			m_conflicts[i] = grid.m_conflicts[i];
		}
	}

	// Assignment
	Grid& operator= (const Grid& grid)
	{
		for (int i = 0;  i < NUM_ROWS*NUM_COLUMNS;  ++i)
		{
			m_elements[i] = grid.m_elements[i];
		}
		for (int i = 0;  i < NUM_ROWS*NUM_COLUMNS;  ++i)
		{
			m_conflicts[i] = grid.m_conflicts[i];
		}
		return *this;
	}

protected:
	void checkCell(int r, int c) const
	{
		assert((r >= 0) && (r < NUM_ROWS));
		assert((c >= 0) && (c < NUM_COLUMNS));
	}

public:
	value_type get(int r, int c) const
	{
		checkCell(r, c);
		return m_elements[r*NUM_COLUMNS + c];
	}

	// Set without doing any sanity checks.
	// Useful if the grid is being used for something other than
	// trying to build a pseudo-magic square.
	void setRaw(int r, int c, value_type value)
	{
		checkCell(r, c);
		m_elements[r*NUM_COLUMNS + c] = value;
	}

	// Returns the number of blank cells that had to be added because
	// no other option was possible.
	int set(int r, int c, value_type value)
	{
		checkCell(r, c);
		assert(isEmpty(r, c));
		assert(canBeSetTo(r, c, value));
		m_elements[r*NUM_COLUMNS + c] = value;
		int extraBlanks = markConflictingElements(r, c, value);
		//sanityCheck();
		return extraBlanks;
	}

	bool hasValue(int r, int c) const
	{
		return get(r, c) <= CellValueSet::MAX_VALUE;
	}

	bool canBeSetTo(int r, int c, value_type value)
	{
		return ! m_conflicts[r*NUM_COLUMNS + c].contains(value);
	}

	// "Empty" means "hasn't been set yet, may be set in the future".
	bool isEmpty(int r, int c) const
	{
		return get(r, c) == EMPTY;
	}

	// "Blank" means "set to a non-numeric blank square".
	void setToBlank(int r, int c)
	{
		checkCell(r, c);
		assert(isEmpty(r, c));
		m_elements[r*NUM_COLUMNS + c] = BLANK;
	}

	// "Blank" means "set to a non-numeric blank square".
	bool isBlank(int r, int c) const
	{
		return get(r, c) == BLANK;
	}

	void sanityCheck(int r, int c) const
	{
		CellValueSet conflicts = recomputeConflictingElements(r, c);
		assert(m_conflicts[r*NUM_COLUMNS + c] == conflicts);
	}

	void sanityCheck() const
	{
		for (int r = 0;  r < Grid::NUM_ROWS;  ++r)
		{
			for (int c = 0;  c < Grid::NUM_COLUMNS;  ++c)
			{
				sanityCheck(r, c);
			}
		}
	}

protected:
	friend class AddConflictingElements;

	// Returns true if we've run out of options just now.
	// In other words, it will return true if there are still other,
	// or if the cell has been set to be blank *previously*.
	bool setOneConflictingElement(int r, int c, value_type value)
	{
		m_conflicts[r*NUM_COLUMNS + c].add(value);
		return ((! m_conflicts[r*NUM_COLUMNS + c].isFull()) || isBlank(r, c));
	}

	int markConflictingElements(int r, int c, int value);
	
	CellValueSet recomputeConflictingElements(int r, int c) const;

private:
	// "Empty" means "hasn't been set yet, may be set in the future".
	static const value_type EMPTY = static_cast<value_type>(-1);

	// "Blank" means "set to a non-numeric blank square".
	static const value_type BLANK = static_cast<value_type>(-2);

private:
	// Elements of the grid
	value_type   m_elements[NUM_ROWS*NUM_COLUMNS];
	CellValueSet m_conflicts[NUM_ROWS*NUM_COLUMNS];
};

std::ostream& operator<< (std::ostream& stream, const Grid& grid);

#endif // GRID__H
