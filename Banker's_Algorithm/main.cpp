#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>

using namespace std;


int main() {
	int resourceNum, processNum;
	vector<int> instanceVec;

	srand((unsigned int)time(NULL));

	resourceNum = rand() % 3 + 3;
	processNum = rand() % 3 + 3;
	cout << "Number of resources: " << setw(2) << resourceNum << "\n";
	cout << "Number of processes: " << setw(2) << processNum << "\n";
	cout << "Number of instances: ";
	for (int i = 0; i < resourceNum; i++) {
		instanceVec.push_back(rand() % 6 + 15); //instance: 15-20
		cout << setw(3) << instanceVec[i];
	}
	cout << "\n";

	vector<bool> IsFinished(processNum, 0);
	vector<vector<int>> maxVec(processNum, vector<int>(resourceNum));
	vector<vector<int>> allocationVec(processNum, vector<int>(resourceNum));
	vector<int> allocSumVec(resourceNum);
	vector<vector<int>> needVec(processNum, vector<int>(resourceNum));
	vector<int> availableVec(resourceNum);

	cout << "---------MAX---------\n";
	for (int i = 0; i < processNum; i++) {
		cout << "P" << i << ": ";
		for (int j = 0; j < resourceNum; j++) {
			maxVec[i][j] = rand() % (instanceVec[j] - 6) + 3;
			cout << setw(3) << maxVec[i][j];
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "------Allocation------\n";
	for (int i = 0; i < processNum; i++) {
		cout << "P" << i << ": ";
		for (int j = 0; j < resourceNum; j++) {
			allocationVec[i][j] = rand() % 4;
			allocSumVec[j] += allocationVec[i][j];
			cout << setw(3) << allocationVec[i][j];
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "--------Need--------\n";
	for (int i = 0; i < processNum; i++) {
		cout << "P" << i << ": ";
		for (int j = 0; j < resourceNum; j++) {
			needVec[i][j] = maxVec[i][j] - allocationVec[i][j];
			cout << setw(3) << needVec[i][j];
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "------Available------\n    ";
	for (int i = 0; i < resourceNum; i++) {
		availableVec[i] = instanceVec[i] - allocSumVec[i];
		cout << setw(3) << availableVec[i];
	}
	cout << "\n\n";

	// request
	int proc;
	vector<int> requestVec(resourceNum);
	while (1) {
		cout << "Enter the number of process: ";
		cin >> proc;
		if (0 <= proc && proc <= processNum)
			break;
		else
			cout << "Enter an appropriate number of process.\n";
	}
	cout << "Enter the Request instances of process" << proc << ": ";
	for (int i = 0; i < resourceNum; i++) {
		cin >> requestVec[i];
	}

	for (int i = 0; i < resourceNum; i++) {
		if (i == resourceNum - 1) {
			if (requestVec[i] <= needVec[proc][i] && requestVec[i] <= availableVec[i]) {
				for (int j = 0; j < resourceNum; j++) {
					allocationVec[proc][j] += requestVec[j];
					needVec[proc][j] -= requestVec[j];
					availableVec[j] -= requestVec[j];
				}
			}
			else {
				cout << "Request is not accepted for the resources.\n";
				cout << "The program is ended.\n";
				exit(0);
			}
		}
		else {
			if (requestVec[i] <= needVec[proc][i] && requestVec[i] <= availableVec[i])
				continue;
			else {
				cout << "Request is not accepted for the resources.\n";
				cout << "The program is ended.\n";
				exit(0);
			}
		}
		cout << "Request is accepted for the resources.\n";
	}

	//safety sequence
	bool allFinished = false;
	bool noSequence = false;
	int finishedNum = 0;
	int nonavailProc = 0;
	vector<int> finishedVec;
	vector<int> workVec(resourceNum);
	for (int i = 0; i < availableVec.size(); i++) 
		workVec[i] = availableVec[i];

	cout << "\n======Safety Sequence======\n";
	while (!allFinished && !noSequence) { 
		for (int i = 0; i < processNum; i++) {
			for (int j = 0; j < resourceNum; j++) {
				if (j == resourceNum - 1) {
					if (needVec[i][j] <= workVec[j]) {
						if (IsFinished[i] == false) {
							for (int k = 0; k < processNum; k++) {
								for (int l = 0; l < resourceNum; l++) {
									needVec[k][l] -= requestVec[l];
									workVec[l] += requestVec[l];
									workVec[l] -= requestVec[l];
								}
							}
							IsFinished[i] = true;
							finishedNum++;
							finishedVec.push_back(i);
						}
						else {
							if (finishedNum == processNum)
								allFinished = true;
							else
								break;
						}
					}
					else {
						if (IsFinished[i] == false) {
							if (nonavailProc == processNum)
								noSequence = true;
							else {
								nonavailProc++;
								break;
							}
						}
						else
							break;
					}
				}
				else {
					if (needVec[i][j] <= workVec[j]) {
						if (IsFinished[i] == false)
							continue;
						else
							break;
					}
					else {
						if (nonavailProc == processNum) {
							noSequence = true;
						}
						else {
							nonavailProc++;
							break;
						}
					}
				}
				
			}
		}
	}
	for (int i = 0; i < finishedVec.size(); i++) {
		if (i == finishedVec.size() - 1)
			cout << finishedVec[i] << "\n";
		else
			cout << finishedVec[i] << " -> ";
	}

	if (finishedVec.size() != processNum)
		cout << "Not available for all of the processes. There is no safety sequence in this case.\n";

	system("pause");

	return 0;
}
