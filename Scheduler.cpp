// Scheduler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
int main()
{
	Scheduler* sch = new Scheduler();
}

Scheduler::Scheduler()
{
	t = 0;
	
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
	sem1 = new Semaphore();
	sem2 = new Semaphore();
	sem3 = new Semaphore();
	sem4 = new Semaphore();
	semT = new Semaphore();
	semR = new Semaphore(1);
	output = "";
	bool1 = false;
	bool2 = false;
	bool3 = false;
	bool4 = false;
	init();
}


Scheduler::~Scheduler()
{
	delete work;
	delete sem1;
	delete sem2;
}

//functions
void Scheduler::init()
{
	thread thrR = thread(&Scheduler::schedule, this);
	//timer();
	
	thread thrT = thread(&Scheduler::timer, this);
	//thread thrR = thread(&Scheduler::rms, this);
	//thrT.join();
	//thrR.join();

	thread thr1 = thread(&Scheduler::execute, this, 1);
	thread thr2 = thread(&Scheduler::execute, this, 2);
	thread thr3 = thread(&Scheduler::execute, this, 4);
	thread thr4 = thread(&Scheduler::execute, this, 16);
	//thrT.join();
	thrR.join();
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
	for (int i = 0; i < unit; i++)
	{
		if (unit == 1)
		{
			sem1->wait();
			//cout << " U: 1" << endl;
		}
		else if (unit == 2)
		{
			sem2->wait();
			//cout << " U: 2" << endl;
		}
		else if (unit == 4)
		{
			sem3->wait();
			//cout << " U: 4" << endl;
		}
		else if (unit == 16)
		{
			sem4->wait();
			//cout << " U: 16" << endl;
		}
		output += " U: " + to_string(unit);
		doWork(unit);
		//sem1->wait();
	}
	if (unit == 1)
	{
		//sem->signal();
		bool1 = true;
		sem2->signal();
		//sem1->signal();
	}
	else if (unit == 2)
	{
		bool2 = true;
		sem3->signal();
		//sem1->signal();
	}
	else if (unit == 4)
	{
		bool3 = true;
		sem4->signal();
		//sem1->signal();
	}
	else if (unit == 16)
		bool4 = true;
}
void Scheduler::doWork(int unit)
{
	int i = 0;
	while (i < 1)
	{
		current = unit;
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
		i++;
	}
	Sleep(SLEEP);
}
void Scheduler::schedule()
{
	while (t < CYCLE)
	{
		semT->wait();
		if (bool1 == false)
			sem1->signal();
		else if (bool2 == false)
			sem2->signal();
		else if (bool3 == false)
			sem3->signal();
		else if (bool4 == false)
			sem4->signal();
		semR->signal();
	}
}
void Scheduler::timer()
{
	thread thrR;// = thread(&Scheduler::schedule, this);
	for (int i = 0; i < 2; i++)
	{
		t = 0;
		while (t < CYCLE) //16 time units each cycle
		{
			semR->wait();
			output += "\nT: " + to_string(CYCLE * i + t);
			semT->signal();
			Sleep(SLEEP);

			t++;
		}
	}
	t++;
	//thrR.join();
	cout << output << endl;
	//exit(0);
}