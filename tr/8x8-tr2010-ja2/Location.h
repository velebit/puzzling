#ifndef LOCATION__H
#define LOCATION__H

#include <assert.h>
#include <ostream>

class Location
{
private:
	typedef unsigned char value_type;

public:
	// Create an empty location so we can do containers etc.
	Location() : m_row(-1), m_col(-1) {}

	Location(int r, int c) : m_row(r), m_col(c) {}

	// Copy constructor
	Location(const Location& other)
	{
		m_row = other.m_row;
		m_col = other.m_col;
	}

	// Assignment
	Location& operator= (const Location& other)
	{
		m_row = other.m_row;
		m_col = other.m_col;
		return *this;
	}

	int row() const    { return m_row; }

	int column() const { return m_col; }

public:
	bool operator== (const Location& other) const
	{
		return ( (m_row == other.m_row) && (m_col == other.m_col) );
	}

private:
	// Elements of the set, stored as bits
	value_type m_row;
	value_type m_col;
};

inline std::ostream& operator<< (std::ostream& stream, const Location& loc)
{
	stream << "(" << loc.row() << "," << loc.column() << ")";
	return stream;
}


#endif // LOCATION__H
