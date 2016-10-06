#ifndef LOCK_FOR_SCOPE__H
#define LOCK_FOR_SCOPE__H

#include "Mutex.h"

class LockForScope
{
public:
	LockForScope(Mutex& mutex)
		: m_mutex(mutex)
	{
		m_mutex.lock();
	}


    ~LockForScope()
	{
		m_mutex.unlock();
	}

private:
	// No copy construction or assignment!
    LockForScope(const LockForScope&);
    LockForScope& operator=(const LockForScope&);

private:
    Mutex& m_mutex;
};

#endif // LOCK_FOR_SCOPE__H
