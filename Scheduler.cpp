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
		//sem1->wait();
	}
	if (unit == 1)
	{
		//sem->signal();
		sem2->signal();
		//sem1->signal();
	}
	else if (unit == 2)
	{
		sem3->signal();
		//sem1->signal();
	}
	else if (unit == 4)
	{
		sem4->signal();
		//sem1->signal();
	}
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
	//list<thread> arr;
	thread arr[40];
	int i = 0;
	while (t<CYCLE)
	{
		semT->wait();
		cout << " U: 1" << endl;
		arr[i] = thread(&Scheduler::execute, this, 1);
		//thr1.join();
		i++;
		sem2->wait();
		cout << " U: 2" << endl;
		arr[i] = thread(&Scheduler::execute, this, 2);
		//thr2.join();
		i++;
		sem3->wait();
		cout << " U: 4" << endl;
		arr[i] = thread(&Scheduler::execute, this, 4);
		//thr3.join();
		i++;
		sem4->wait();
		cout << " U: 16" << endl;
		arr[i] = thread(&Scheduler::execute, this, 16);
		i++;
	}
	///thread thr = thread(&Scheduler::execute, this, 1);
	for (int j = 0; j < i; j++)
	{
		arr[j].join();
	}
}
void Scheduler::timer()
{
	thread thrR;// = thread(&Scheduler::schedule, this);
	for (int i = 0; i < 10; i++)
	{
		t = 0;
		while (t < CYCLE) //16 time units each cycle
		{
			cout << "\nT: " << CYCLE*i + t;
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