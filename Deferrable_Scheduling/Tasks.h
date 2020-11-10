#pragma once

struct PeriodicTask
{
	PeriodicTask();
	int period;
	int exeTime;
};

struct AperiodicTask
{
	AperiodicTask();
	int arrTime;
	int exeTime;
	int waitingTime;
	int tempTime; // ������� �ش� task�� ����� �ð�
	bool done;
};

PeriodicTask::PeriodicTask()
{
	period = 0;
	exeTime = 0;
}


AperiodicTask::AperiodicTask()
{
	arrTime = 0;
	exeTime = 0;
	waitingTime = 0;
	tempTime = 0;
	done = false;
}

