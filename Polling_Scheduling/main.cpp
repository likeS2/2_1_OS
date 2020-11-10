#include "Tasks.h"
#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
using namespace std;

double Utilization(PeriodicTask parr[], AperiodicTask aparr[], int leng); // 주기적 tasks의 utilization을 구하기 위함
bool Condition(PeriodicTask parr[], AperiodicTask aparr[], int leng); // utilization이 n(2^1/n - 1)보다 작거나 같은지 확인하기 위함


int main() {
	PeriodicTask parr[41]; //polling까지 41개의 periodic tasks. parr[40]이 polling.
	AperiodicTask aparr[10]; //10개의 aperiodic tasks.
	int t = 0; //시간
	int totalWaitingTime = 0;
	int averWaitingTime = 0;
	int index = 0; // i번째 aperiodic task에 접근할 때 사용.
	double u = 0; // utilization
	int myTemp = 0; // t보다 큰 다음 polling 시점을 구할 때 사용. 
	vector<int> vecTemp;
	vector<int> vec = { 8, 10, 12, 15, 16, 20, 25 };
	srand((unsigned int)time(NULL));

	while (1) {
		for (int i = 0; i <= 40; i++) { // periodic tasks의 주기, 수행시간 set. 단, polling의 주기는 건너 뛰고 polling의 수행 시간은 1~4의 int 중 랜덤 결정.
			if (i == 40) {
				parr[i].exeTime = rand() % 3 + 1;
			}
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

		if (Condition(parr, aparr, 40)) { // polling을 제외한 모든 주기적 tasks들이 마감 시간을 지킨다는 조건을 만족한다면
			cout << "success\n";
			double temp = (double)parr[40].exeTime / (41 * (pow(2, (double)1 / 41) - 1) - Utilization(parr, aparr, 39)); // polling 주기를 x라고 했을 때 조건에서 x의 범위를 구한 후 최대값을 x(주기)로 set.
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

	int pollPeriod = parr[40].period;
	while (t <= sum) { // 모든 tasks들의 수행 시간까지 반복
		if (t % pollPeriod == 0) { // polling 주기가 됐을 때 (t가 polling 하는 시점일 때)
			if (aparr[index].arrTime == t) { // aperiodic task의 도착 시간이 t이면 (polling하는 시점 == task의 도착 시간)
				if (aparr[index].done == true) // aperiodic task을 수행 완료 했을 때 다음 aperiodic task로 넘어간다
					index++;
				else { // aperiodic task가 수행 완료되지 않았을 때
					aparr[index].tempTime += parr[40].exeTime; // aperiodic task의 현재 수행 시간에 polling의 수행 시간만큼 더해준다
					if (aparr[index].tempTime == aparr[index].exeTime) // tempTime과 exeTime이 같으면 해당 task는 수행 완료된 것이므로 done = true로 바꿔준다.
						aparr[index].done = true;
					else
						aparr[index].waitingTime += pollPeriod; //수행했으나 task가 수행 완료되지 않았을 때 다음 polling 주기까지 기다려야하므로 waitingTime에 polling 주기만큼 더해준다.
				}
			}
			else if (aparr[index].arrTime < t) { // aperiodic task의 도착 시간 < t이면 (task의 도착 시점 < polling하는 시점)
				while (1) {
					if (pollPeriod * myTemp < t && t <= pollPeriod * (myTemp + 1)) { // t보다 큰 다음 polling 시점을 구하는 것
						aparr[index].waitingTime += pollPeriod * (myTemp + 1) - aparr[index].arrTime; // polling 시점이 아닌 시각에 도착한 task의 waiting time은 다음 polling 시간 - task의 도착 시간
						break;
					}
					else
						myTemp++;
				}
				if (aparr[index].done == true) // aperiodic task을 수행 완료 했을 때 다음 aperiodic task로 넘어간다
					index++;
				else { // aperiodic task가 수행 완료되지 않았을 때
					aparr[index].tempTime += parr[40].exeTime; // aperiodic task의 현재 수행 시간에 polling의 수행 시간만큼 더해준다
					if (aparr[index].tempTime == aparr[index].exeTime) // tempTime과 exeTime이 같으면 해당 task는 수행 완료된 것이므로 done = true로 바꿔준다.
						aparr[index].done = true;
					else
						aparr[index].waitingTime += pollPeriod; //수행했으나 task가 수행 완료되지 않았을 때 다음 polling 주기까지 기다려야하므로 waitingTime에 polling 주기만큼 더해준다.
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

