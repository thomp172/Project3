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
	output = "";
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
	timer();
	thrR.join();
	//thread thrT = thread(&Scheduler::timer, this);
	//thread thrR = thread(&Scheduler::rms, this);
	//thrT.join();
	//thrR.join();

}

//thread to be called
void Scheduler::execute(int unit)
{
	for (int i = 0; i < unit; i++)
	{
		
		doWork(unit);
		
	}
	if (unit == 1)
	{
		//sem->signal();
		sem2->signal();
	}
	else if (unit == 2)
		sem3->signal();
	else if (unit == 4)
		sem4->signal();
}
void Scheduler::doWork(int unit)
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
	Sleep(SLEEP); //1 period * unit
	//call unit # of times
}
void Scheduler::schedule()
{

	while (t<CYCLE)
	{
		semT->wait();
		cout << " U: 1" << endl;
		thread thr1 = thread(&Scheduler::execute, this, 1);
		thr1.join();

		sem2->wait();
		cout << " U: 2" << endl;
		thread thr2 = thread(&Scheduler::execute, this, 2);
		thr2.join();

		sem3->wait();
		cout << " U: 4" << endl;
		thread thr3 = thread(&Scheduler::execute, this, 4);
		thr3.join();

		sem4->wait();
		cout << " U: 16" << endl;
		thread thr4 = thread(&Scheduler::execute, this, 16);
		thr4.join();
	}
}
void Scheduler::timer()
{
	thread thrR;// = thread(&Scheduler::schedule, this);
	for (int i = 1; i <= 10; i++)
	{
		while (t < CYCLE) //16 time units each cycle
		{
			cout << "\nT: " << t*i;
			if (t == 0)
			{
				semT->signal();
			}
			Sleep(SLEEP);

			t++;
		}
	}
	t++;
	//thrR.join();
	cout << output << endl;
}