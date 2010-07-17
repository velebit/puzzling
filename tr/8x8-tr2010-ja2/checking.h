#ifndef CHECKING__H
#define CHECKING__H

#include <iostream>
#include <string>
#include <stdlib.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

static inline void failed()
{
	//abort();
}

template<typename T>
static inline void checkEq(const string& label, const T& have, const T& want)
{
	if (have != want)
	{
		cerr << "Mismatch for `" << label << "': have '" << have << "', "
			 << "wanted '" << want << "'." << endl;
		failed();
	}
}

template<typename T>
static inline void checkEq(const string& label,
						   const T have, const T want, const T fuzz)
{
	if ((want > (have + fuzz)) || (have > (want + fuzz)))
	{
		cerr << "Mismatch for `" << label << "':" << endl
			 << "    have   " << have << "," << endl
			 << "    wanted " << want << "," << endl
			 << "    off by " << (have-want)
			 << " (limit = " << fuzz << ")." << endl;
		failed();
	}
}

static inline void checkTrue(const string& label, bool have)
{
	if (! have)
	{
		cerr << "Failure for `" << label << "': returned false,"
			" expected true." << endl;
		failed();
	}
}

#endif // CHECKING__H
