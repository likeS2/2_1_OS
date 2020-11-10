#include "Tasks.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

double Utilization(PeriodicTask parr[], AperiodicTask aparr[], int leng); // �ֱ��� task���� utilization�� ���ϱ� ����.
bool Condition(PeriodicTask parr[], AperiodicTask aparr[], int leng); // �ֱ��� task���� utilization�� �����ð��� ��Ű�� ���� ������ �����ϴ��� Ȯ���ϱ� ����.

int main() {
	PeriodicTask parr[41]; //polling���� 41���� periodic tasks. parr[40]�� polling.
	AperiodicTask aparr[10]; //10���� aperiodic tasks.
	int t = 0; //�ð�
	int totalWaitingTime = 0;
	int averWaitingTime = 0;
	int index = 0; // i��° aperiodic task�� ������ �� ���.
	double u = 0; // utilization
	int myTemp = 0; // t���� ū ���� ����� BP �ֱ⸦ ã�� �� ���.
	vector<int> vecTemp;
	vector<int> vec = { 8, 10, 12, 15, 16, 20, 25 };
	srand((unsigned int)time(NULL));

	while (1) {
		for (int i = 0; i <= 40; i++) { // periodic tasks�� �ֱ�, ����ð� set. BP�� �ֱ�� �ǳ� �ٰ� BP�� capacity�� 1. parr[40]�� BP.
			if (i == 40)
				parr[i].exeTime = 1;
			else {
				parr[i].period = vec[rand() % 6 + 1]; // periodic tasks�� �ֱ�� 8, 10, 12, 15, 16, 20, 25 �� �ϳ��� ���� ����.
				parr[i].exeTime = rand() % 9 + 1; // periodic tasks�� ���� �ð��� 1~9������ int�� ���� ����.
			}
		}
		for (int i = 0; i <= 9; i++) { // aperiodic tasks�� ���� �ð��� ���� ������ set, ����ð� set.
			vecTemp.push_back((rand() % 991) + 1); // ���� �ð��� �����ϱ� ���� 1~991������ �� �� �ϳ��� �������� vecTemp�� push_back.
			aparr[i].exeTime = rand() % 4 + 1; // aperiodic tasks�� ���� �ð��� 1~4 �� �������� ����.
		}
		sort(vecTemp.begin(), vecTemp.end(), less<int>()); // vecTemp�� ������������ �����Ͽ� aperiodic tasks�� �迭 ������� �����ϰ� �Ѵ�.
		for (int i = 0; i <= 9; i++) {
			aparr[i].arrTime = vecTemp[i];
		}

		if (Condition(parr, aparr, 40)) { // BP�� ������ ��� �ֱ��� tasks���� ���� �ð��� ��Ų�ٴ� ������ �����Ѵٸ�
			cout << "success\n";
			double temp = (double)parr[40].exeTime / (41 * (pow(2, (double)1 / 41) - 1) - Utilization(parr, aparr, 39)); // BP �ֱ⸦ x��� ���� �� ���ǿ��� x�� ������ ���� �� �ִ밪�� x(�ֱ�)�� set.
			parr[40].period = (int)temp;
			break; // �������� ������ periodic tasks�� �ֱ�� ����ð��� utilization�� �����Ѵٸ� break, �׷��� �ʴٸ� ������ ������ �ٽ� set.
		}
		cout << "repeat\n";
	}

	int sum = 0;
	for (int i = 0; i <= 40; i++)
		sum += parr[i].exeTime;
	for (int i = 0; i <= 9; i++)
		sum += aparr[i].exeTime; // ��� tasks���� ���� �ð��� ���Ѵ�

	int BPPeriod = parr[40].period;
	while (t <= sum) { // ��� tasks���� ���� �ð����� �ݺ�
		if (t % BPPeriod == 0) { // BP �ֱⰡ ���� ��
			parr[40].exeTime = 1; //�ֱ⸶�� BP capacity�� �ٽ� 1�� ����
			if (aparr[index].arrTime == t) { // aperiodic task�� ���� �ð��� t�̸� (capacity�� ä������ ���� == task�� ���� �ð�)
				if (aparr[index].done == true) // aperiodic task�� ���� �Ϸ� ���� �� ���� aperiodic task�� �Ѿ��
					index++;
				else { // aperiodic task�� ���� �Ϸ���� �ʾ��� ��
					aparr[index].tempTime += parr[40].exeTime; // aperiodic task�� ���� ���� �ð��� BP�� ���� �ð�(capacity)��ŭ �����ش�
					if (aparr[index].tempTime == aparr[index].exeTime) // tempTime�� exeTime�� ������ �ش� task�� ���� �Ϸ�� ���̹Ƿ� done = true�� �ٲ��ش�.
						aparr[index].done = true;
					else
						aparr[index].waitingTime += BPPeriod; //���������� task�� ���� �Ϸ���� �ʾ��� �� capacity�� ä���� ��(���� �ֱ�)���� ��ٷ����ϹǷ� waitingTime�� BP�� �ֱ⸸ŭ �����ش�.
				}
			}
			else if (aparr[index].arrTime < t) { // aperiodic task�� ���� �ð� < t
				if (aparr[index].done == true) // aperiodic task�� ���� �Ϸ� ���� �� ���� aperiodic task�� �Ѿ��
					index++;
				else { // aperiodic task�� ���� �Ϸ���� �ʾ��� ��
					if (parr[40].exeTime != 0) { //BP capacity�� 0�� �ƴϱ� ������ ���� ������ ��
						aparr[index].tempTime += parr[40].exeTime; // aperiodic task�� ���� ���� �ð��� BP�� ���� �ð���ŭ �����ش�
						if (aparr[index].tempTime == aparr[index].exeTime) // tempTime�� exeTime�� ������ �ش� task�� ���� �Ϸ�� ���̹Ƿ� done = true�� �ٲ��ش�.
							aparr[index].done = true;
						else
							aparr[index].waitingTime += BPPeriod; //���������� task�� ���� �Ϸ���� �ʾ��� �� ���� BP �ֱ���� ��ٷ����ϹǷ�(capacity�� ä������ ��ٸ��� ��) waitingTime�� BP �ֱ⸸ŭ �����ش�.
						parr[40].exeTime--; // capacity��ŭ ���������Ƿ� capacity�� 0���� ������ش�.
					}
					else { // capacity�� 0�̱� ������ ���� �Ұ����ϹǷ� (���� �ֱ�-�����ð�)��ŭ ��ٷ��� �ȴ�
						while (1) {
							int myTemp = 0;
							if (BPPeriod * myTemp < t && t <= BPPeriod * (myTemp + 1)) { //t���� ū ���� ����� BP �ֱ⸦ ã�� ��
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
	for (int i = 0; i <= leng; i++) { // periodic tasks�� utilization.
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