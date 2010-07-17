#ifndef CELL_VALUE_SET__H
#define CELL_VALUE_SET__H

#include <assert.h>
#include <ostream>

class CellValueSet
{
public:
	static const int MIN_VALUE = 1;
	static const int MAX_VALUE = 8;

private:
	typedef unsigned char value_type;

public:
	// Create an empty set
	CellValueSet() : m_mask(0) {}

	// Copy constructor
	CellValueSet(const CellValueSet& other)
	{
		m_mask = other.m_mask;
	}

	// Assignment
	CellValueSet& operator= (const CellValueSet& other)
	{
		m_mask = other.m_mask;
		return *this;
	}

public:
	bool contains(int n) const
	{
		assert((n >= MIN_VALUE) && (n <= MAX_VALUE));
		return (m_mask & (1 << (n - MIN_VALUE)));
	}

	void add(int n)
	{
		assert((n >= MIN_VALUE) && (n <= MAX_VALUE));
		m_mask = m_mask | (1 << (n - MIN_VALUE));
	}

	void remove(int n)
	{
		assert((n >= MIN_VALUE) && (n <= MAX_VALUE));
		m_mask = m_mask & ~(1 << (n - MIN_VALUE));
	}

    void clear()
	{
		m_mask = EMPTY;
	}

    void fill()
	{
		m_mask = FULL;
	}

    bool isEmpty() const
	{
		return m_mask == EMPTY;
	}

    bool isFull() const
	{
		return m_mask == FULL;
	}

public:
	bool operator== (const CellValueSet& other) const
	{
		return (m_mask == other.m_mask);
	}


private:
	static const value_type EMPTY = 0;
	static const value_type FULL  = ~0;

private:
	// Elements of the set, stored as bits
	value_type m_mask;
};

inline std::ostream& operator<< (std::ostream& stream, const CellValueSet& s)
{
	bool shown = false;
	stream << "{";
	for (int i = CellValueSet::MIN_VALUE;  i <= CellValueSet::MAX_VALUE;  ++i)
	{
		if (s.contains(i))
		{
			if (shown)
				stream << ",";
			stream << i;
			shown = true;
		}
	}
	stream << "}";
	return stream;
}


#endif // CELL_VALUE_SET__H
