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
	Scheduler(int start);
	~Scheduler();
	string getOutput();
	int* getExe();
	int* getRun();
private:
	int t;
	int time;
	int** work;
	const static int SIZE = 10;
	const static int SLEEP = 10;
	const static int CYCLE = 16;
	const static int TASK1 = 1;
	const static int TASK2 = 2;
	const static int TASK3 = 4;
	const static int TASK4 = 16;

	string current;
	string output;

	Semaphore* sem1;
	Semaphore* sem2;
	Semaphore* sem3;
	Semaphore* sem4;
	thread thrT;
	thread thr1;
	thread thr2;
	thread thr3;
	thread thr4;

	int exe[4];
	int run[4];
	int start1;
	int start2;
	int start3;
	int start4;
	int run1;
	int run2;
	int run3;
	int run4;


	void init();
	void begin();

	//thread
	void execute(int unit, Semaphore* semThis, string task, int& run);
	bool doWork(int unit, string task, bool overrun, int& run);
	void timer();
	
	
};

