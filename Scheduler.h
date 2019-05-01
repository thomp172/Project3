#pragma once

#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>
#include <string>
#include "Semaphore.h"
class Scheduler //rate monotonic scheduler
{
public:
	Scheduler();
	~Scheduler();
private:
	int t;
	double b;
	int** work;
	const static int SIZE = 10;
	const static int SLEEP = 10;
	const static int CYCLE = 16;
	int current;
	string output;
	Semaphore* sem1;
	Semaphore* sem2;
	Semaphore* sem3;
	Semaphore* sem4;
	Semaphore* semT;
	void init();
	//thread
	void doWork(int unit);
	void timer();
	void execute(int unit);
	void schedule();

	
};

