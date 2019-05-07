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
		Scheduler* sch = new Scheduler(0,r);
		output += sch->toString() + "\n\n";
		out << output << endl;
		
		out.close();
	}

	cout << "done" << endl;
	
}

//Scheduler class
Scheduler::Scheduler()
{
	//timer begins at 0
	t = 0;
	repeat = 1;
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
	

	//initialize counters
	for (int m = 0; m < 4; m++)
	{
		exe[m] = 0;
		run[m] = 0;
	}
	//initialize output
	output = "";

	
	
	//begin();
}
void Scheduler::begin()
{
	bool1 = true;
	bool2 = true;
	bool3 = true;
	bool4 = true;
	//initialize semaphores
	sem1 = new Semaphore();
	sem2 = new Semaphore();
	sem3 = new Semaphore();
	sem4 = new Semaphore();
	for (int i = 0; i < 10; i++)
	{
		thrT[i] = thread([&](Scheduler * sch) {
			
			output += "\n\nTask 1: Release=" + to_string(t);
			output += "\nTask 2: Release=" + to_string(t);
			output += "\nTask 3: Release=" + to_string(t);
			output += "\nTask 4: Release=" + to_string(t);
			output += "\nTask 1: Deadline=" + to_string(TASK1 + t);
			output += "\nTask 2: Deadline=" + to_string(TASK2 + t);
			output += "\nTask 3: Deadline=" + to_string(TASK3 + t);
			output += "\nTask 4: Deadline=" + to_string(TASK4 + t);
			output += "\n";
			sch->timer(i);
		}, this);
		thrT[i].join();
	}
	
}

void Scheduler::execute(int unit, Semaphore*& semThis, Semaphore*& semNext, bool boolThis, string task, int &run)
{
	//set processor affinity
	HANDLE hProcess = GetCurrentProcess();
	SetProcessAffinityMask(hProcess, 1);
	//semThis->wait();
	bool overrun = false;
	output += "\nT: " + to_string(t) + " " + task + " begins";
	//repeat doWork() as many times as there are units
	
	if (((repeat == 2) && (unit == TASK2)) || ((repeat == 3) && (unit == TASK3)))
	{
		while (true)
		{
			overrun = doWork(unit, task, run);
			if (overrun == true)
			{
				boolThis = false;
				semNext->signal();
				semThis->wait();
				output += "\nT: " + to_string(t) + " " + task + " continues";
			}
		}
	}
	else
	{
		for (int i = 0; i < unit; i++)
		{
			overrun = doWork(unit, task, run);
			if (overrun == true)
			{
				boolThis = false;
				semNext->signal();
				semThis->wait();
				output += "\nT: " + to_string(t) + " " + task + " continues";
			}
		}
	}
	
	output += "\nT: " + to_string(t) + " " + task + " finished";

}
bool Scheduler::doWork(int unit, string task, int& run)
{
	bool overrun = false;
	int m, n, temp;
	for (int i=0; i<10200; i++)
	{
		//exit function if time deadline is hit
		if ((time >= unit) && (overrun == false))
		{
			output += "\nT: " + to_string(t) + " " + task + " missed deadline";
			//return;
			run++;
			overrun = true;
			return overrun;
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

void Scheduler::timer(int period)
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
			thr1[period] = thread([&](Scheduler * sch) {
				sem1->wait();
				exe[0]++;
				execute(TASK1, sem1, sem2, bool1, "Task 1", run[0]);
				
				bool1 = true;
				sem2->signal();
				
			}, this);
			if (bool1 == false)
				thr1[period].swap(thr1[period - 1]);
			thr2[period] = thread([&](Scheduler * sch) {
				sem2->wait();
				exe[1]++;
				execute(TASK2, sem2, sem3, bool2, "Task 2", run[1]);
				
				bool2 = true;
				sem3->signal();
				
			}, this);
			if (bool2 == false)
				thr2[period].swap(thr2[period - 1]);
			thr3[period] = thread([&](Scheduler * sch) {
				sem3->wait();
				exe[2]++;
				execute(TASK3, sem3, sem4, bool3, "Task 3", run[2]);
				
				bool3 = true;
				
				sem4->signal();
				
			}, this);
			if (bool3 == false)
				thr3[period].swap(thr3[period - 1]);
			thr4[period] = thread([&](Scheduler * sch) {
				sem4->wait();
				exe[3]++;
				execute(TASK4, sem4, sem1, bool4, "Task 4", run[3]);
				
				bool4 = true;
				
			}, this);
			if (bool1 == false)
				thr4[period].swap(thr4[period - 1]);
			sem1->signal();
		}
		chrono::milliseconds dura(SLEEP);
		this_thread::sleep_for(dura);
		t++;
		time++;
		if (time == 0)
		{
			thr1[period].join();
			thr2[period].join();
			thr3[period].join();
			thr4[period].join();
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
		counts += "\n\nTask " + to_string(j + 1) + " ran " + to_string(exe[j]) + " times.";
		counts += "\nTask " + to_string(j + 1) + " overran " + to_string(run[j]) + " times.";
	}
	return output + counts;
}