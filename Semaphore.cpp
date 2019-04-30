#include "pch.h"

Semaphore::Semaphore()
{
	count = 0;
}
Semaphore::Semaphore(int theCount)
{
	count = theCount;
}

void Semaphore::signal()
{
	unique_lock<mutex> lock(mu);
	count++;
	con.notify_one();
}
void Semaphore::wait()
{
	unique_lock<mutex> lock(mu);
	while (count == 0)
	{
		con.wait(lock);
	}
	count--;
}
int Semaphore::getCount()
{
	return count;
}
