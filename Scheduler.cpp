// Scheduler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
int main()
{
	Scheduler* sch = new Scheduler();
}

Scheduler::Scheduler()
{
	//timer begins at 0
	t = 0;
	//initialize work**
	work = new int*[SIZE];
	for (int k = 0; k < SIZE; k++)
	{
		work[k] = new int[SIZE];
	}
	int j;
	for (int i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			work[i][j] = 1;
		}
	}
	//initialize semaphores
	sem1 = new Semaphore();
	sem2 = new Semaphore();
	sem3 = new Semaphore();
	sem4 = new Semaphore();
	semT = new Semaphore();
	semR = new Semaphore(1);
	//initialize output
	output = "";
	//initialize booleans
	bool1 = false;
	bool2 = false;
	bool3 = false;
	bool4 = false;

	output += "Task 1: Deadline=" + to_string(TASK1);
	output += "\nTask 2: Deadline=" + to_string(TASK2);
	output += "\nTask 3: Deadline=" + to_string(TASK3);
	output += "\nTask 4: Deadline=" + to_string(TASK4);
	//begin
	init();
}


Scheduler::~Scheduler()
{
	delete work;
	delete sem1;
	delete sem2;
	delete sem3;
	delete sem4;
	delete semT;
	delete semR;
}

//functions
void Scheduler::init()
{
	//thread thrR = thread(&Scheduler::schedule, this);
	//timer();
	
	thread thrT = thread(&Scheduler::timer, this);
	//thread thrR = thread(&Scheduler::rms, this);
	//thrT.join();
	//thrR.join();

	thread thr1 = thread(&Scheduler::execute, this, TASK1);
	thread thr2 = thread(&Scheduler::execute, this, TASK2);
	thread thr3 = thread(&Scheduler::execute, this, TASK3);
	thread thr4 = thread(&Scheduler::execute, this, TASK4);
	//thrT.join();
	//thrR.join();
	//thrT.join();
	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
	thrT.join();



}

//thread to be called
void Scheduler::execute(int unit)
{
	string out;
	int time = t;
	//repeat doWork() as many times as there are units
	for (int i = 0; i < unit; i++)
	{
		if (time == t)
		{
			if (unit == TASK1)
			{
				sem1->wait();
				out = "Task 1";
			}
			else if (unit == TASK2)
			{
				sem2->wait();
				out = "Task 2";
			}
			else if (unit == TASK3)
			{
				sem3->wait();
				out = "Task 3";
			}
			else if (unit == TASK4)
			{
				sem4->wait();
				out = "Task 4";
			}
			if (current != out)
				output += "\nT: " + to_string(t) + " " + out + " begins";
		}
		doWork(unit);
	}
	if ((bool1 == false) && (unit == TASK1))
	{
		output += "\nT: " + to_string(t) + " Task 1 finished";
		bool1 = true;
	}
	else if ((bool2 == false) && (unit == TASK2))
	{
		output += "\nT: " + to_string(t) + " Task 2 finished";
		bool2 = true;
	}
	else if ((bool3 == false) && (unit == TASK3))
	{
		output += "\nT: " + to_string(t) + " Task 3 finished";
		bool3 = true;
	}
	else if ((bool4 == false) && (unit == TASK4))
	{
		output += "\nT: " + to_string(t) + " Task 4 finished";
		bool4 = true;
	}
}
void Scheduler::doWork(int unit)
{
	for (int i=0; i<2500; i++)
	{
		if (unit == TASK1)
			current = "Task 1";
		else if (unit == TASK2)
			current = "Task 2";
		else if (unit == TASK3)
			current = "Task 3";
		else if (unit == TASK4)
			current = "Task 4";
		int m, n, temp;
		//execute multiplication of array
		for (m = 0; m < SIZE; m++)
		{
			for (n = 0; n < SIZE; n++)
			{
				temp = SIZE - 1 - m;
				work[m][n] = work[m][n] * work[m][n];
				work[m][n] = work[m][n] * work[temp][n];
			}
		}
	}
}

void Scheduler::timer()
{
	t = 0;
	string temp;
	string compare = "begin";
	while (t < CYCLE) //16 time units each cycle
	{
		
		if ((t < TASK1) && (bool1 == false))
		{
			sem1->signal();
		}
		if ((t >= TASK1) && (t < TASK2) && (bool2 == false))
		{
			if (bool1 == false)
			{
				output += "\nT: " + to_string(t) + " Task 1 missed deadline";
				bool1 = true;
			}
			sem2->signal();
		}
		if ((t >= TASK2) && (t < TASK3) && (bool3 == false))
		{
			if (bool2 == false)
			{
				output += "\nT: " + to_string(t) + " Task 2 missed deadline";
				bool2 = true;
			}
				
			sem3->signal();
		}
		if ((t >= TASK3) && (t < TASK4) && (bool4 == false))
		{
			if (bool3 == false)
			{
				output += "\nT: " + to_string(t) + " Task 3 missed deadline";
				bool3 = true;
			}
			sem4->signal();
		}

		Sleep(SLEEP);
		t++;
	}
	if (bool4 == false)
		output += "\nT: " + to_string(t) + " Task 4 missed deadline";
	cout << output << endl;
	exit(0);
}