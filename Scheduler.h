#pragma once

#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>
#include <fstream>
#include <string>
#include <chrono>
#include "Semaphore.h"
class Scheduler //rate monotonic scheduler
{
public:
	Scheduler();
	Scheduler(int start, int theRepeat);
	~Scheduler();
	string getOutput();
	int* getExe();
	int* getRun();
	string toString();
private:
	int t;
	int time;
	int** work;
	int repeat;
	const static int SIZE = 10;
	const static int SLEEP = 10;
	const static int CYCLE = 16;
	const static int TASK1 = 1;
	const static int TASK2 = 2;
	const static int TASK3 = 4;
	const static int TASK4 = 16;

	string output;

	Semaphore* sem1;
	Semaphore* sem2;
	Semaphore* sem3;
	Semaphore* sem4;
	thread thrT[SIZE];
	thread thr1[SIZE];
	thread thr2[SIZE];
	thread thr3[SIZE];
	thread thr4[SIZE];
	bool bool1;
	bool bool2;
	bool bool3;
	bool bool4;

	int exe[4];
	int run[4];


	void init();
	void begin();

	//thread
	void execute(int unit, Semaphore*& semThis, Semaphore*& semNext, bool boolThis, string task, int& run);
	bool doWork(int unit, string task, int& run);
	void timer(int period);
	
	
};

