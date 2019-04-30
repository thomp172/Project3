#pragma once

#include <mutex>
#include <condition_variable>
using namespace std;

class Semaphore
{
public:
	Semaphore();
	Semaphore(int theCount);
	void signal();
	void wait();
	int getCount();
private:
	mutex mu;
	condition_variable con;
	unsigned int count;

};