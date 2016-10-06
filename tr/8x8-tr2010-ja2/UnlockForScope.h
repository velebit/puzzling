#ifndef UNLOCK_FOR_SCOPE__H
#define UNLOCK_FOR_SCOPE__H

#include "Mutex.h"

class UnlockForScope
{
public:
	UnlockForScope(Mutex& mutex)
		: m_mutex(mutex)
	{
		m_mutex.unlock();
	}


    ~UnlockForScope()
	{
		m_mutex.lock();
	}

private:
	// No copy construction or assignment!
    UnlockForScope(const UnlockForScope&);
    UnlockForScope& operator=(const UnlockForScope&);

private:
    Mutex& m_mutex;
};

#endif // UNLOCK_FOR_SCOPE__H
