#ifndef ERRNO_EXCEPTION__H
#define ERRNO_EXCEPTION__H

#include <exception>
#include "safeErrorString.h"

class ErrnoException : public std::exception
{
public:
	ErrnoException(int err_num) : m_errNum(err_num) {};
	virtual ~ErrnoException() throw () {};

	int errorNumber() const { return m_errNum; }

	const std::string description() const throw ()
	{
		return safeErrorString(m_errNum);
	}

private:
	int m_errNum;
};

#endif // ERRNO_EXCEPTION__H
