#ifndef GRID_LINES__H
#define GRID_LINES__H

#include <assert.h>
#include <iostream>
#include <algorithm>

#include "Grid.h"

template <class Op>
inline void forOthersInRow(int r, int c, Op& op)
{
	for (int i = 0;  i < Grid::NUM_COLUMNS;  ++i)
	{
		if (i != c)
		{
			op(r, i);
		}
	}
}

template <class Op>
inline void forOthersInColumn(int r, int c, Op& op)
{
	for (int i = 0;  i < Grid::NUM_ROWS;  ++i)
	{
		if (i != r)
		{
			op(i, c);
		}
	}
}

template <class Op>
inline void forOthersInForeDiagonal(int r, int c, Op& op)
{
	// The "fore" diagonal is the one where (r,c) is followed by (r+1,c+1),
	// i.e. where r-c is constant.
	int d = r-c;
	int rA = std::max(d, 0);
	int rZ = std::min(Grid::NUM_ROWS + d, Grid::NUM_ROWS + 0);
	for (int i = rA;  i < rZ;  ++i)
	{
		if (i != r)
		{
			op(i, i-d);
		}
	}
}

template <class Op>
inline void forOthersInBackDiagonal(int r, int c, Op& op)
{
	// The "back" diagonal is the one where (r,c) is followed by (r+1,c-1),
	// i.e. where r+c is constant.
	int d = r+c;
	int rA = std::max(d - Grid::NUM_COLUMNS + 1, 0);
	int rZ = std::min(d + 1, Grid::NUM_ROWS + 0);
	for (int i = rA;  i < rZ;  ++i)
	{
		if (i != r)
		{
			op(i, d-i);
		}
	}
}

template <class Op>
inline void forOthersInLines(int r, int c, Op& op)
{
	forOthersInRow(r, c, op);
	forOthersInColumn(r, c, op);
	forOthersInForeDiagonal(r, c, op);
	forOthersInBackDiagonal(r, c, op);
}

#endif // GRID_LINES__H
