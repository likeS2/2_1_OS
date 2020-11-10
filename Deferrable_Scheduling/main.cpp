#include "Tasks.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

double Utilization(PeriodicTask parr[], AperiodicTask aparr[], int leng); // 주기적 task들의 utilization을 구하기 위함.
bool Condition(PeriodicTask parr[], AperiodicTask aparr[], int leng); // 주기적 task들의 utilization이 마감시간을 지키기 위한 조건을 만족하는지 확인하기 위함.

int main() {
	PeriodicTask parr[41]; //polling까지 41개의 periodic tasks. parr[40]이 polling.
	AperiodicTask aparr[10]; //10개의 aperiodic tasks.
	int t = 0; //시간
	int totalWaitingTime = 0;
	int averWaitingTime = 0;
	int index = 0; // i번째 aperiodic task에 접근할 때 사용.
	double u = 0; // utilization
	int myTemp = 0; // t보다 큰 가장 가까운 BP 주기를 찾을 때 사용.
	vector<int> vecTemp;
	vector<int> vec = { 8, 10, 12, 15, 16, 20, 25 };
	srand((unsigned int)time(NULL));

	while (1) {
		for (int i = 0; i <= 40; i++) { // periodic tasks의 주기, 수행시간 set. BP의 주기는 건너 뛰고 BP의 capacity는 1. parr[40]이 BP.
			if (i == 40)
				parr[i].exeTime = 1;
			else {
				parr[i].period = vec[rand() % 6 + 1]; // periodic tasks의 주기는 8, 10, 12, 15, 16, 20, 25 중 하나로 랜덤 결정.
				parr[i].exeTime = rand() % 9 + 1; // periodic tasks의 수행 시간은 1~9까지의 int로 랜덤 결정.
			}
		}
		for (int i = 0; i <= 9; i++) { // aperiodic tasks의 도착 시간은 작은 순으로 set, 수행시간 set.
			vecTemp.push_back((rand() % 991) + 1); // 도착 시간을 결정하기 위해 1~991까지의 수 중 하나를 랜덤으로 vecTemp에 push_back.
			aparr[i].exeTime = rand() % 4 + 1; // aperiodic tasks의 수행 시간은 1~4 중 랜덤으로 결정.
		}
		sort(vecTemp.begin(), vecTemp.end(), less<int>()); // vecTemp를 오름차순으로 정렬하여 aperiodic tasks의 배열 순서대로 도착하게 한다.
		for (int i = 0; i <= 9; i++) {
			aparr[i].arrTime = vecTemp[i];
		}

		if (Condition(parr, aparr, 40)) { // BP를 제외한 모든 주기적 tasks들이 마감 시간을 지킨다는 조건을 만족한다면
			cout << "success\n";
			double temp = (double)parr[40].exeTime / (41 * (pow(2, (double)1 / 41) - 1) - Utilization(parr, aparr, 39)); // BP 주기를 x라고 했을 때 조건에서 x의 범위를 구한 후 최대값을 x(주기)로 set.
			parr[40].period = (int)temp;
			break; // 랜덤으로 지정한 periodic tasks의 주기와 수행시간이 utilization을 만족한다면 break, 그렇지 않다면 만족할 때까지 다시 set.
		}
		cout << "repeat\n";
	}

	int sum = 0;
	for (int i = 0; i <= 40; i++)
		sum += parr[i].exeTime;
	for (int i = 0; i <= 9; i++)
		sum += aparr[i].exeTime; // 모든 tasks들의 수행 시간을 더한다

	int BPPeriod = parr[40].period;
	while (t <= sum) { // 모든 tasks들의 수행 시간까지 반복
		if (t % BPPeriod == 0) { // BP 주기가 됐을 때
			parr[40].exeTime = 1; //주기마다 BP capacity를 다시 1로 설정
			if (aparr[index].arrTime == t) { // aperiodic task의 도착 시간이 t이면 (capacity가 채워지는 시점 == task의 도착 시간)
				if (aparr[index].done == true) // aperiodic task을 수행 완료 했을 때 다음 aperiodic task로 넘어간다
					index++;
				else { // aperiodic task가 수행 완료되지 않았을 때
					aparr[index].tempTime += parr[40].exeTime; // aperiodic task의 현재 수행 시간에 BP의 수행 시간(capacity)만큼 더해준다
					if (aparr[index].tempTime == aparr[index].exeTime) // tempTime과 exeTime이 같으면 해당 task는 수행 완료된 것이므로 done = true로 바꿔준다.
						aparr[index].done = true;
					else
						aparr[index].waitingTime += BPPeriod; //수행했으나 task가 수행 완료되지 않았을 때 capacity가 채워질 때(다음 주기)까지 기다려야하므로 waitingTime에 BP의 주기만큼 더해준다.
				}
			}
			else if (aparr[index].arrTime < t) { // aperiodic task의 도착 시간 < t
				if (aparr[index].done == true) // aperiodic task을 수행 완료 했을 때 다음 aperiodic task로 넘어간다
					index++;
				else { // aperiodic task가 수행 완료되지 않았을 때
					if (parr[40].exeTime != 0) { //BP capacity가 0이 아니기 때문에 수행 가능할 때
						aparr[index].tempTime += parr[40].exeTime; // aperiodic task의 현재 수행 시간에 BP의 수행 시간만큼 더해준다
						if (aparr[index].tempTime == aparr[index].exeTime) // tempTime과 exeTime이 같으면 해당 task는 수행 완료된 것이므로 done = true로 바꿔준다.
							aparr[index].done = true;
						else
							aparr[index].waitingTime += BPPeriod; //수행했으나 task가 수행 완료되지 않았을 때 다음 BP 주기까지 기다려야하므로(capacity가 채워지길 기다리는 것) waitingTime에 BP 주기만큼 더해준다.
						parr[40].exeTime--; // capacity만큼 수행했으므로 capacity를 0으로 만들어준다.
					}
					else { // capacity가 0이기 때문에 수행 불가능하므로 (다음 주기-도착시간)만큼 기다려야 된다
						while (1) {
							int myTemp = 0;
							if (BPPeriod * myTemp < t && t <= BPPeriod * (myTemp + 1)) { //t보다 큰 가장 가까운 BP 주기를 찾는 것
								aparr[index].waitingTime += BPPeriod * (myTemp + 1) - aparr[index].arrTime;
								break;
							}
							else
								myTemp++;
						}
					}
				}
			}
			t++;
		}
		else
			t++;
	}
	for (int i = 0; i <= 9; i++)
		totalWaitingTime += aparr[i].waitingTime;
	averWaitingTime = totalWaitingTime / 10;
	cout << "Average waiting time of aperiodic tasks : " << averWaitingTime << "\n";
	return 0;
}


double Utilization(PeriodicTask parr[], AperiodicTask aparr[], int leng)
{
	double u;
	for (int i = 0; i <= leng; i++) { // periodic tasks의 utilization.
		u = (double)parr[i].exeTime / parr[i].period;
	}
	return u;
}

bool Condition(PeriodicTask parr[], AperiodicTask aparr[], int leng)
{
	double u = Utilization(parr, aparr, leng - 1);
	double condition = leng * (pow(2, (double)1 / leng) - 1);
	return (u <= condition);
}