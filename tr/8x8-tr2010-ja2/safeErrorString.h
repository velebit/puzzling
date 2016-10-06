#ifndef SAFE_ERROR_STRING__H
#define SAFE_ERROR_STRING__H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>

inline const char* safeErrorString(int err_num, size_t buf_size, char* buf)
{
	buf[0] = '\0';

	// Don't bother with the return value of strerror_r; it could
	// be either a char* or an int. =(
	strerror_r(err_num, buf, buf_size);

	if (buf[0])
	{
		buf[buf_size-1] = '\0';  // paranoia =)
	}
	else
	{
		snprintf(buf, buf_size, "[unknown errno %d]", err_num);
	}

	return buf;
}

inline std::string safeErrorString(int err_num)
{
	const int BUF_SIZE = 256;
	char buf[BUF_SIZE];
	return std::string(safeErrorString(err_num, BUF_SIZE, buf));
}

#endif // SAFE_ERROR_STRING__H
