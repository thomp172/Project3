// Scheduler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
int main()
{

	system("PAUSE");
	string output = "";
	for (int i = 0; i < 10; i++)
	{
		Scheduler* sch = new Scheduler(i * 16);
		output +=  sch->getOutput() + "\n\n";
	}
	cout << output << endl;
	system("PAUSE");
	
}

Scheduler::Scheduler()
{
	//timer begins at 0
	t = 0;
	init();
	begin();
}

Scheduler::Scheduler(int start)
{
	//timer begins at start
	t = start;
	//begin
	init();
	begin();
}

Scheduler::~Scheduler()
{
	delete work;
	delete sem1;
	delete sem2;
	delete sem3;
	delete sem4;
}

string Scheduler::getOutput()
{
	return output;
}
//functions
void Scheduler::init()
{
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
	//initialize output
	output = "";
	//initialize booleans
	bool1 = false;
	bool2 = false;
	bool3 = false;
	bool4 = false;

	output += "Task 1: Release=" + to_string(t);
	output += "\nTask 2: Release=" + to_string(t);
	output += "\nTask 3: Release=" + to_string(t);
	output += "\nTask 4: Release=" + to_string(t);
	output += "\nTask 1: Deadline=" + to_string(TASK1+t);
	output += "\nTask 2: Deadline=" + to_string(TASK2+t);
	output += "\nTask 3: Deadline=" + to_string(TASK3+t);
	output += "\nTask 4: Deadline=" + to_string(TASK4+t);
	output += "\n";
}
void Scheduler::begin()
{
	thread thrT = thread([&](Scheduler * sch) {
		sch->timer();
	}, this);
	thread thr1 = thread([&] (Scheduler * sch) {
		sem1->wait();
		output += "\nT: " + to_string(t) + " Task 1 begins";
		//repeat doWork() as many times as there are units
		for (int i = 0; i < TASK1; i++)
		{
			doWork(TASK1);
			if (time >= TASK1)
			{
				output += "\nT: " + to_string(t) + " Task 1 missed deadline";
				bool1 = true;
				sem2->signal();
				return;
			}
		}
		if (bool1 == false)
		{
			output += "\nT: " + to_string(t) + " Task 1 finished";
			bool1 = true;
		}
		sem2->signal();
	}, this);
	thread thr2 = thread([&](Scheduler * sch) {
		sem2->wait();
		output += "\nT: " + to_string(t) + " Task 2 begins";
		//repeat doWork() as many times as there are units
		for (int i = 0; i < TASK2; i++)
		{
			doWork(TASK2);
			if (time >= TASK2)
			{
				output += "\nT: " + to_string(t) + " Task 2 missed deadline";
				bool2 = true;
				sem3->signal();
				return; 
			}
		}
		if (bool2 == false)
		{
			output += "\nT: " + to_string(t) + " Task 2 finished";
			bool2 = true;
		}
		sem3->signal();
	}, this);
	thread thr3 = thread([&](Scheduler * sch) {
		sem3->wait();
		output += "\nT: " + to_string(t) + " Task 3 begins";
		//repeat doWork() as many times as there are units
		for (int i = 0; i < TASK3; i++)
		{
			doWork(TASK3);
			if (time >= TASK3)
			{
				output += "\nT: " + to_string(t) + " Task 3 missed deadline";
				bool3 = true;
				sem4->signal();
				return;
			}

		}
		if (bool3 == false)
		{
			output += "\nT: " + to_string(t) + " Task 3 finished";
			bool3 = true;
		}
		sem4->signal();
	}, this);
	thread thr4 = thread([&](Scheduler * sch) {
		sem4->wait();
		output += "\nT: " + to_string(t) + " Task 4 begins";
		//repeat doWork() as many times as there are units
		for (int i = 0; i < TASK4; i++)
		{
			doWork(TASK4);
			if (time >= TASK4)
			{
				output += "\nT: " + to_string(t) + " Task 4 missed deadline";
				bool4 = true;
				return;
			}
		}
		if (bool4 == false)
		{
			output += "\nT: " + to_string(t) + " Task 4 finished";
			bool4 = true;
		}
	}, this);
	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();

	thrT.join();

}


void Scheduler::doWork(int unit)
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
	for (int i=0; i<18000; i++)
	{
		if (time >= unit)
			return;
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
	time = 0;
	int arr = 0;
	string temp;
	string compare = "begin";
	while (time <= CYCLE) //16 time units each cycle
	{
		//must fix if statement time comparisons.... need threads 
		//to start early if previous threads already finished
		if ((time < TASK1) && (bool1 == false))
		{
			sem1->signal();
		}
		//else if ((time < TASK2) && (bool2 == false))
		//{
		//	if (bool1 == false)
		//	{
		//		output += "\nT: " + to_string(t) + " Task 1 missed deadline";
		//		bool1 = true;
		//	}
		//	//sem2->signal();
		//}
		//else if ((time < TASK3) && (bool3 == false))
		//{
		//	if (bool2 == false)
		//	{
		//		output += "\nT: " + to_string(t) + " Task 2 missed deadline";
		//		bool2 = true;
		//	}
		//		
		//	//sem3->signal();
		//}
		//else if ((time < TASK4) && (bool4 == false))
		//{
		//	if (bool3 == false)
		//	{
		//		output += "\nT: " + to_string(t) + " Task 3 missed deadline";
		//		bool3 = true;
		//	}
		//	//sem4->signal();
		//}
		//else if ((time >= TASK4) && (bool4 == false))
		//{
		//	output += "\nT: " + to_string(t) + " Task 4 missed deadline";
		//	bool4 = true;
		//}
		Sleep(SLEEP);
		t++;
		time++;
	}
	
	//cout << output << endl;
	//exit(0);
}