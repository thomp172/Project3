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
	semT = new Semaphore(1);
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

	thread thrT = thread(&Scheduler::timer, this);
	//thread thrR = thread(&Scheduler::rms, this);
	thrT.join();
	//thrR.join();

}

//thread to be called
void Scheduler::execute(int unit, Semaphore* sem)
{
	/*if (unit == 2)
		sem->wait();*/
	/*if (unit == 2)
		sem2->wait();*/ //stop in middle of execution at each unit
	//output += "T: " + to_string(t) + " U: " + to_string(unit) + "\n";
	for (int i = 0; i < unit; i++)
	{
		
		doWork(unit);
		
	}
	if (unit == 1)
	{
		//sem->signal();
		sem2->signal();
	}
}
void Scheduler::doWork(int unit)
{
	/*if (unit == 1)
	{
		sem1->wait();
	}
	else if (unit == 2)
		sem2->wait();*/
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
	Semaphore* semTemp = new Semaphore();
	if (t == 1)//(t % 1 == 0)
	{
		output += "T: " + to_string(t) + " U: " + to_string(1) + "\n";
		thread thr1 = thread(&Scheduler::execute, this, 1, semTemp);
		thr1.join();
		
		sem2->wait();
		output += "T: " + to_string(t) + " U: " + to_string(2) + "\n";
		thread thr2 = thread(&Scheduler::execute, this, 2, semTemp);
		thr2.join();
	}
}
void Scheduler::timer()
{
	while (t < CYCLE) //16 time units each cycle
	{
		//semT->wait();
		cout << "T: " << t << endl;
		//schedule();
		thread thrR = thread(&Scheduler::schedule, this);
		Sleep(SLEEP);
		t++;
		thrR.join();
		//semT->signal();
	}
	cout << output << endl;
}