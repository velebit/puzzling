#include "CellValueSet.h"
#include "checking.h"

void test00_empty_full()
{
	checkTrue("min value", CellValueSet::MIN_VALUE == 1);
	checkTrue("max value", CellValueSet::MAX_VALUE == 8);

	CellValueSet s;
	s.clear();
	checkTrue("is empty", s.isEmpty());
	checkTrue("isn't full", ! s.isFull());
	for (int i = 1;  i <= 8;  ++i)
	{
		checkTrue("all are empty", ! s.contains(i));
	}

	s.fill();
	checkTrue("is full", s.isFull());
	checkTrue("isn't empty", ! s.isEmpty());
	for (int i = 1;  i <= 8;  ++i)
	{
		checkTrue("all are full", s.contains(i));
	}

	s.clear();
	checkTrue("is empty", s.isEmpty());
	checkTrue("isn't full", ! s.isFull());
	for (int i = 1;  i <= 8;  ++i)
	{
		checkTrue("all are empty", ! s.contains(i));
	}
}

void test01_construct_empty()
{
	CellValueSet s;
	checkTrue("is empty", s.isEmpty());
	checkTrue("isn't full", ! s.isFull());
	for (int i = 1;  i <= 8;  ++i)
	{
		checkTrue("all are empty", ! s.contains(i));
	}
}

void test02_only_one()
{
	CellValueSet s;
	for (int i = 1;  i <= 8;  ++i)
	{
		s.clear();
		s.add(i);
		checkTrue("isn't empty", ! s.isEmpty());
		checkTrue("isn't full", ! s.isFull());
		for (int j = 1;  j <= 8;  ++j)
		{
			if (j == i)
				checkTrue("current is full", s.contains(i));
			else
				checkTrue("others are empty", ! s.contains(j));
		}
	}
}

void test03_all_but_one()
{
	CellValueSet s;
	for (int i = 1;  i <= 8;  ++i)
	{
		s.fill();
		s.remove(i);
		checkTrue("isn't empty", ! s.isEmpty());
		checkTrue("isn't full", ! s.isFull());
		for (int j = 1;  j <= 8;  ++j)
		{
			if (j == i)
				checkTrue("current is empty", ! s.contains(i));
			else
				checkTrue("others are full", s.contains(j));
		}
	}
}

void test04_manual_fill()
{
	CellValueSet s;
	s.clear();
	for (int i = 1;  i <= 8;  ++i)
	{
		checkTrue("isn't full", ! s.isFull());
		s.add(i);
	}
	checkTrue("is full", s.isFull());
	checkTrue("isn't empty", ! s.isEmpty());
}

void test05_manual_clear()
{
	CellValueSet s;
	s.fill();
	for (int i = 1;  i <= 8;  ++i)
	{
		checkTrue("isn't empty", ! s.isEmpty());
		s.remove(i);
	}
	checkTrue("is empty", s.isEmpty());
	checkTrue("isn't full", ! s.isFull());
}

int main(int argc, char** argv)
{
	test00_empty_full();
	test01_construct_empty();
	test02_only_one();
	test03_all_but_one();
	test04_manual_fill();
	test05_manual_clear();
	return 0;
}
