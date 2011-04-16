#ifndef MUTEX__H
#define MUTEX__H

#include <pthread.h>
#include <string>
#include "ErrnoException.h"

class Mutex
{
public:
    Mutex()
	{
		int retval;
		pthread_mutexattr_t mutex_attr;
		retval = pthread_mutexattr_init(&mutex_attr);
		if (retval != 0)
		{
			throw InitException("attr_init", retval);
		}
		retval = pthread_mutexattr_settype(&mutex_attr,
										   PTHREAD_MUTEX_ERRORCHECK);
		if (retval != 0)
		{
			throw InitException("attr_settype(ERRORCHECK)", retval);
		}

		retval = pthread_mutex_init(&m_mutex, &mutex_attr);
		if (retval != 0)
		{
			throw InitException("mutex_init", retval);
		}

		retval = pthread_mutexattr_destroy(&mutex_attr);
		if (retval != 0)
		{
			//throw InitException("attr_destroy", retval);
		}
	}


    ~Mutex()
	{
		/*int retval =*/
		pthread_mutex_destroy(&m_mutex);
	}

public:
	class Exception : public ErrnoException
	{
	public:
		Exception(const char* label, int err_num)
			: ErrnoException(err_num)
		{
			snprintf(m_prefix, sizeof(m_prefix), "%s: ", label);
		}

		Exception(const char* label, const char* stage, int err_num)
			: ErrnoException(err_num)
		{
			snprintf(m_prefix, sizeof(m_prefix), "%s [%s]: ", label, stage);
		}

		virtual ~Exception() throw () {};

		const std::string description() const
		{
			return std::string(m_prefix) + ErrnoException::description();
		}

	private:
		char m_prefix[256];
	};

	class InitException : public Exception
	{
	public:
	InitException(const char* stage, int err_num)
			: Exception("failed to create mutex", stage, err_num) {};
	};

	class LockException : public Exception
	{
	public:
		LockException(int err_num)
			: Exception("failed to lock mutex", err_num) {};
	};

	class UnlockException : public Exception
	{
	public:
		UnlockException(int err_num)
			: Exception("failed to unlock mutex", err_num) {};
	};

private:
	// No copy construction or assignment!
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);

protected:
	// Lock/unlock operations should only be performed via the scoped operators
	friend class LockForScope;
	friend class UnlockForScope;

	void lock()
	{
		int retval = pthread_mutex_lock(&m_mutex);
		if (retval != 0)
		{
			throw LockException(retval);
		}
	}

    void unlock()
	{
		int retval = pthread_mutex_unlock(&m_mutex);
		if (retval != 0)
		{
			throw UnlockException(retval);
		}
	}

private:
    pthread_mutex_t m_mutex;
};

#endif // MUTEX__H
