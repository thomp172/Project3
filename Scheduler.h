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
private:
	int t;
	int time;
	int** work;
	const static int SIZE = 10;
	const static int SLEEP = 10;
	const static int CYCLE = 16;

	int TASK1 = 1;
	int TASK2 = 2;
	int TASK3 = 4;
	int TASK4 = 16;
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


	void init();
	void begin();

	//thread
	void execute(int unit, Semaphore* semThis, string task);
	void doWork(int unit);
	void timer();
	
	
};

