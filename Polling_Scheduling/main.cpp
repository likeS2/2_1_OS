#include "Tasks.h"
#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
using namespace std;

double Utilization(PeriodicTask parr[], AperiodicTask aparr[], int leng); // �ֱ��� tasks�� utilization�� ���ϱ� ����
bool Condition(PeriodicTask parr[], AperiodicTask aparr[], int leng); // utilization�� n(2^1/n - 1)���� �۰ų� ������ Ȯ���ϱ� ����


int main() {
	PeriodicTask parr[41]; //polling���� 41���� periodic tasks. parr[40]�� polling.
	AperiodicTask aparr[10]; //10���� aperiodic tasks.
	int t = 0; //�ð�
	int totalWaitingTime = 0;
	int averWaitingTime = 0;
	int index = 0; // i��° aperiodic task�� ������ �� ���.
	double u = 0; // utilization
	int myTemp = 0; // t���� ū ���� polling ������ ���� �� ���. 
	vector<int> vecTemp;
	vector<int> vec = { 8, 10, 12, 15, 16, 20, 25 };
	srand((unsigned int)time(NULL));

	while (1) {
		for (int i = 0; i <= 40; i++) { // periodic tasks�� �ֱ�, ����ð� set. ��, polling�� �ֱ�� �ǳ� �ٰ� polling�� ���� �ð��� 1~4�� int �� ���� ����.
			if (i == 40) {
				parr[i].exeTime = rand() % 3 + 1;
			}
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

		if (Condition(parr, aparr, 40)) { // polling�� ������ ��� �ֱ��� tasks���� ���� �ð��� ��Ų�ٴ� ������ �����Ѵٸ�
			cout << "success\n";
			double temp = (double)parr[40].exeTime / (41 * (pow(2, (double)1 / 41) - 1) - Utilization(parr, aparr, 39)); // polling �ֱ⸦ x��� ���� �� ���ǿ��� x�� ������ ���� �� �ִ밪�� x(�ֱ�)�� set.
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

	int pollPeriod = parr[40].period;
	while (t <= sum) { // ��� tasks���� ���� �ð����� �ݺ�
		if (t % pollPeriod == 0) { // polling �ֱⰡ ���� �� (t�� polling �ϴ� ������ ��)
			if (aparr[index].arrTime == t) { // aperiodic task�� ���� �ð��� t�̸� (polling�ϴ� ���� == task�� ���� �ð�)
				if (aparr[index].done == true) // aperiodic task�� ���� �Ϸ� ���� �� ���� aperiodic task�� �Ѿ��
					index++;
				else { // aperiodic task�� ���� �Ϸ���� �ʾ��� ��
					aparr[index].tempTime += parr[40].exeTime; // aperiodic task�� ���� ���� �ð��� polling�� ���� �ð���ŭ �����ش�
					if (aparr[index].tempTime == aparr[index].exeTime) // tempTime�� exeTime�� ������ �ش� task�� ���� �Ϸ�� ���̹Ƿ� done = true�� �ٲ��ش�.
						aparr[index].done = true;
					else
						aparr[index].waitingTime += pollPeriod; //���������� task�� ���� �Ϸ���� �ʾ��� �� ���� polling �ֱ���� ��ٷ����ϹǷ� waitingTime�� polling �ֱ⸸ŭ �����ش�.
				}
			}
			else if (aparr[index].arrTime < t) { // aperiodic task�� ���� �ð� < t�̸� (task�� ���� ���� < polling�ϴ� ����)
				while (1) {
					if (pollPeriod * myTemp < t && t <= pollPeriod * (myTemp + 1)) { // t���� ū ���� polling ������ ���ϴ� ��
						aparr[index].waitingTime += pollPeriod * (myTemp + 1) - aparr[index].arrTime; // polling ������ �ƴ� �ð��� ������ task�� waiting time�� ���� polling �ð� - task�� ���� �ð�
						break;
					}
					else
						myTemp++;
				}
				if (aparr[index].done == true) // aperiodic task�� ���� �Ϸ� ���� �� ���� aperiodic task�� �Ѿ��
					index++;
				else { // aperiodic task�� ���� �Ϸ���� �ʾ��� ��
					aparr[index].tempTime += parr[40].exeTime; // aperiodic task�� ���� ���� �ð��� polling�� ���� �ð���ŭ �����ش�
					if (aparr[index].tempTime == aparr[index].exeTime) // tempTime�� exeTime�� ������ �ش� task�� ���� �Ϸ�� ���̹Ƿ� done = true�� �ٲ��ش�.
						aparr[index].done = true;
					else
						aparr[index].waitingTime += pollPeriod; //���������� task�� ���� �Ϸ���� �ʾ��� �� ���� polling �ֱ���� ��ٷ����ϹǷ� waitingTime�� polling �ֱ⸸ŭ �����ش�.
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

