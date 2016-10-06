#include "Mutex.h"
#include "LockForScope.h"
#include "checking.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>

void test00_create_destroy()
{
	{
		Mutex oneMutex;
	}
	{
		Mutex anotherMutex;
	}
}

void test01_lock_unlock()
{
	Mutex mutex;
	{
		LockForScope scope(mutex);
	}
	{
		LockForScope scope(mutex);
	}
	{
		LockForScope scope(mutex);
	}
}

static void* lock_helper(void* arg)
{
	Mutex* mutex = static_cast<Mutex*>(arg);

	const bool debugSpew = false;
	if (debugSpew)      std::cout << "helper: locking" << std::endl;
	{
		LockForScope scope(*mutex);
		if (debugSpew)  std::cout << "helper: locked" << std::endl;
		usleep(1000*1000);
		if (debugSpew)  std::cout << "helper: unlocking" << std::endl;
	}
	if (debugSpew)      std::cout << "helper: unlocked" << std::endl;
	return 0;
}

void test10_concurrency()
{
	const bool debugSpew = false;

	Mutex mutex;
	pthread_t thread;
	int retval = pthread_create(&thread, 0, lock_helper, &mutex);
	if (retval != 0)
	{
		std::cout << "error creating thread: " << safeErrorString(retval)
				  << std::endl;
		exit(0);
	}

	struct timeval before, after;
	usleep(100*1000);
	gettimeofday(&before, 0);
	if (debugSpew)      std::cout << "main:   locking" << std::endl;
	{
		LockForScope scope(mutex);
		if (debugSpew)  std::cout << "main:   locked" << std::endl;
		if (debugSpew)  std::cout << "main:   unlocking" << std::endl;
	}
	if (debugSpew)      std::cout << "main:   unlocked" << std::endl;
	gettimeofday(&after, 0);
	double elapsed = ((after.tv_sec - before.tv_sec) +
					  1e-6 * (after.tv_usec - before.tv_usec));
	if (debugSpew)      std::cout << "elapsed: " << elapsed << std::endl;
	checkEq("main thread waited on mutex", elapsed, .900, .050);
	usleep(100*1000);
}

void test20_error_lock_twice()
{
	Mutex mutex;
	bool reachedScope2   = false;
	bool insideScope2    = false;
	bool exceptionCaught = false;
	try
	{
		LockForScope scope1(mutex);
		{
			reachedScope2 = true;
			LockForScope scope2(mutex);
			insideScope2  = true;
		}
	}
	catch (Mutex::LockException& mle)
	{
		exceptionCaught = true;
	}
	checkTrue("double locking threw an exception", exceptionCaught);
	checkTrue("double locking threw at the expected place",
			  reachedScope2 && !insideScope2);
}

int main(int argc, char** argv)
{
	try
	{
		test00_create_destroy();
		test01_lock_unlock();
		test10_concurrency();
		test20_error_lock_twice();
	}
	catch (Mutex::Exception& me)
	{
		std::cout << "Caught unexpected Mutex::Exception:" << std::endl;
		//std::cout << "what> " << me.what() << std::endl;
		std::cout << "desc> " << me.description() << std::endl;
	}
	catch (std::exception& se)
	{
		std::cout << "Caught unexpected std::exception:" << std::endl;
		std::cout << "what> " << se.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Caught unexpected non-standard exception" << std::endl;
	}
	return 0;
}
