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

	int TASK1 = 1;
	int TASK2 = 2;
	int TASK3 = 4;
	int TASK4 = 16;
	string current;
	bool bool1;
	bool bool2;
	bool bool3;
	bool bool4;
	string output;
	Semaphore* sem1;
	Semaphore* sem2;
	Semaphore* sem3;
	Semaphore* sem4;
	Semaphore* semT;
	Semaphore* semR;
	void init();

	//thread
	void doWork(int unit);
	void timer();
	void execute(int unit);

	
};

