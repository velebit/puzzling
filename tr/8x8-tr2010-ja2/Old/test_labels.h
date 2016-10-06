#ifndef TEST_LABELS__H
#define TEST_LABELS__H

#include <iostream>
#include <string>
#include <stdlib.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

class TestRun
{
public:
	TestRun(const string& functionName)
		: m_functionName(functionName)
	{
		cout << ">>> " << m_functionName << " >>> starting" << endl;
	}

	~TestRun()
	{
		cout << "<<< " << m_functionName << " <<< ending" << endl;
	}

private:
	string m_functionName;
};

#define TEST TestRun testRun(__FUNCTION__)

#endif // TEST_LABELS__H
