// Scheduler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
int main()
{

	string file;
	int repeat = 0;
	for (int r=1; r<=3; r++)
	{
		int start[4] = { 0,0,0,0 };
		int run[4] = { 0, 0, 0, 0 };
		string output = "";
		ofstream out;
		
		file = "Case" + to_string(r) + ".out";
		cout << "loading " << file << endl;
		out.open(file);
		int m;
		for (int i = 0; i < 10; i++)
		{//repeat program 10 times
			Scheduler* sch = new Scheduler(i * 16,r);
			output += sch->getOutput() + "\n\n";
			for (m = 0; m < 4; m++)
			{
				start[m] += sch->getExe()[m];
				run[m] += sch->getRun()[m];
			}

		}
		out << output << endl;
		for (int j = 0; j < 4; j++)
		{
			out << "Task " + to_string(j + 1) + " ran " + to_string(start[j]) + " times." << endl;
			out << "Task " + to_string(j + 1) + " overran " + to_string(run[j]) + " times." << endl;
		}
		out.close();
	}

	cout << "done" << endl;
	
}

//Scheduler class
Scheduler::Scheduler()
{
	//timer begins at 0
	t = 0;
	repeat = 10000;
	init();
	begin();
}

Scheduler::Scheduler(int start, int theRepeat)
{
	//timer begins at start
	t = start;
	repeat = theRepeat;
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

	//initialize counters
	for (int m = 0; m < 4; m++)
	{
		exe[m] = 0;
		run[m] = 0;
	}
	//initialize output
	output = "";

	output += "Task 1: Release=" + to_string(t);
	output += "\nTask 2: Release=" + to_string(t);
	output += "\nTask 3: Release=" + to_string(t);
	output += "\nTask 4: Release=" + to_string(t);
	output += "\nTask 1: Deadline=" + to_string(TASK1+t);
	output += "\nTask 2: Deadline=" + to_string(TASK2+t);
	output += "\nTask 3: Deadline=" + to_string(TASK3+t);
	output += "\nTask 4: Deadline=" + to_string(TASK4+t);
	output += "\n";
	
	//begin();
}
void Scheduler::begin()
{
	thrT = thread([&](Scheduler * sch) {
		sch->timer();
	}, this);
	thrT.join();

}

void Scheduler::execute(int unit, Semaphore* semThis, string task, int &run)
{
	semThis->wait();
	//set processor affinity
	HANDLE hProcess = GetCurrentProcess();
	SetProcessAffinityMask(hProcess, 1);
	bool overrun = false;
	output += "\nT: " + to_string(t) + " " + task + " begins";
	//repeat doWork() as many times as there are units
	
	if (((repeat == 2) && (unit == TASK2)) || ((repeat == 3) && (unit == TASK3)))
	{
		while (overrun == false)
			overrun = doWork(unit, task, run);
	}
	else
	{
		for (int i = 0; i < unit; i++)
		{
			overrun = doWork(unit, task, run);
			if ((overrun == true) && (unit == TASK4))
				return;
		}
	}
	output += "\nT: " + to_string(t) + " " + task + " finished";

}
bool Scheduler::doWork(int unit, string task, int& run)
{
	bool overrun = false;
	int m, n, temp;
	for (int i=0; i<10500; i++)
	{
		//exit function if time deadline is hit
		if ((time >= unit) && (overrun == false))
		{
			output += "\nT: " + to_string(t) + " " + task + " missed deadline";
			//return;
			run++;
			overrun = true;
			if ((overrun == true) && (unit == TASK4))
			{
				output += "\nT: " + to_string(t) + " " + task + " cannot complete";
				return overrun;
			}
		}
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
	return overrun;
}

void Scheduler::timer()
{
	//set processor affinity
	HANDLE hProcess = GetCurrentProcess();
	SetProcessAffinityMask(hProcess, 1);
	time = 0;
	while (time <= CYCLE) //16 time units each cycle
	{
		if (time == 0)
		{//initiate thread start
			//task threads
			thr1 = thread([&](Scheduler * sch) {
				execute(TASK1, sem1, "Task 1", run[0]);
				exe[0]++;
				sem2->signal();
			}, this);
			thr2 = thread([&](Scheduler * sch) {
				execute(TASK2, sem2, "Task 2", run[1]);
				exe[1]++;
				sem3->signal();
			}, this);
			thr3 = thread([&](Scheduler * sch) {
				execute(TASK3, sem3, "Task 3", run[2]);
				exe[2]++;
				sem4->signal();
			}, this);
			thr4 = thread([&](Scheduler * sch) {
				execute(TASK4, sem4, "Task 4", run[3]);
				exe[3]++;
			}, this);
			sem1->signal();
		}
		chrono::milliseconds dura(SLEEP);
		this_thread::sleep_for(dura);
		t++;
		time++;
		if (time == 0)
		{
			thr1.join();
			thr2.join();
			thr3.join();
			thr4.join();
		}
	}


}

string Scheduler::getOutput()
{
	return output;
}

int* Scheduler::getExe()
{
	return exe;
}
int* Scheduler::getRun()
{
	return run;
}
string Scheduler::toString()
{
	string counts = "";
	for (int j = 0; j < 4; j++)
	{
		counts += "\nTask " + to_string(j + 1) + " ran " + to_string(exe[j]) + " times.";
		counts += "\nTask " + to_string(j + 1) + " overran " + to_string(run[j]) + " times.";
	}
	return output + counts;
}