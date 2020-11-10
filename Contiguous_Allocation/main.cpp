#include "Process.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cstdlib>
using namespace std;


int main() {
	ifstream file;
	vector<int> inputVec;
	int temp, memorySize;
	Process process, hole;

	file.open("input.txt");
	file >> memorySize; //256
	while (!file.eof()) {
		file >> temp;
		inputVec.push_back(temp);
	}
	file.close();

	vector<Process> processVec;
	vector<Process> memoryVec;
	
	for (int i = 0; i < inputVec.size(); i += 2) {
		process.setProcess(inputVec[i], inputVec[i + 1], 0);
		processVec.push_back(process);
	}
	
	hole.setProcess(-1, memorySize, 0);
	memoryVec.push_back(hole); // 초기 상태: 256K 모두 비었음
	int blockNum = 1, minDifference = memorySize, minIdx = 0, freeSize = memorySize, blockCnt = 0;

	for (auto proc : processVec) {
		if (proc.getSize() != 0) { // allocation
			cout << "REQUEST " << proc.getNum() << ": " << proc.getSize() << "K\n";
			for (int i = 0; i < memoryVec.size(); i++) {
				if (memoryVec[i].getNum() == -1) { // hole
					if (proc.getSize() == memoryVec[i].getSize()) { // process size == hole size
						memoryVec[i].setProcess(proc.getNum(), proc.getSize(), memoryVec[i].getAddress());
						blockNum--;
						freeSize -= memoryVec[i].getSize();
						cout << "Best Fit: Allocated at address ";
						if (memoryVec[i].getAddress() == 0)
							cout << memoryVec[i].getAddress() << "\n";
						else
							cout << memoryVec[i].getAddress() << "K\n";
						cout << "          " << freeSize << "K free, " << blockNum << " block(s), average size = " << int(ceil((double)freeSize / blockNum)) << "K\n\n";
						break;
					}
					else if (proc.getSize() < memoryVec[i].getSize()) { // process size < hole size
						if (blockNum > 1) {
							if (minDifference > 0 && memoryVec[i].getSize() - proc.getSize() <= minDifference) {
								minIdx = i;
								minDifference = memoryVec[i].getSize() - proc.getSize();
							}
							if (i == memoryVec.size() - 1) {//마지막 원소일 때
								int beforeSize = memoryVec[minIdx].getSize(), sizeSum = 0;
								for (int j = 0; j < i; j++)
									sizeSum += memoryVec[j].getSize();
								memoryVec[minIdx].setProcess(proc.getNum(), proc.getSize(), sizeSum);
								freeSize -= proc.getSize();
								hole.setProcess(-1, beforeSize - memoryVec[minIdx].getSize(), sizeSum + proc.getSize());
								memoryVec.push_back(hole);
								//blockNum++;
								cout << "Best Fit: Allocated at address ";
								if (memoryVec[i].getAddress() == 0)
									cout << memoryVec[i].getAddress() << "\n";
								else
									cout << memoryVec[i].getAddress() << "K\n";
								cout << "          " << freeSize << "K free, " << blockNum << " block(s), average size = " << int(ceil((double)freeSize / blockNum)) << "K\n\n";
								break; 
							}
						}
						else { // block 1개 (하나의 빈 공간)
							int sizeSum = 0;
							for (int j = 0; j < i; j++)
								sizeSum += memoryVec[j].getSize();
							hole.setProcess(-1, memoryVec[i].getSize() - proc.getSize(), sizeSum + proc.getSize());
							memoryVec[i].setProcess(proc.getNum(), proc.getSize(), sizeSum);
							memoryVec.push_back(hole);
							freeSize -= proc.getSize();
							cout << "Best Fit: Allocated at address ";
							if (memoryVec[i].getAddress() == 0)
								cout << memoryVec[i].getAddress() << "\n";
							else
								cout << memoryVec[i].getAddress() << "K\n";
							cout << "          " << freeSize << "K free, " << blockNum << " block(s), average size = " << int(ceil((double)freeSize / blockNum)) << "K\n\n";
							break;
						}
					}
					else { // process size > hole size.
						blockCnt++;
						if (blockCnt == blockNum) { // 모든 hole의 크기가 process size보다 작을 때
							cout << "The size of process is bigger than the sizes of holes.\n";
							if (freeSize >= proc.getSize()) {
								cout << "Compaction is available. ";
								int availableIdx = -1, firstIdx = -1, holeCnt = 1, tempSize = 0;
								for (int j = 0; j < memoryVec.size(); j++) {
									if (memoryVec[j].getNum() == -1) {
										if (holeCnt == 1)
											firstIdx = j;
										tempSize += memoryVec[j].getSize();
										holeCnt++;
										if (tempSize >= proc.getSize()) {
											availableIdx = j;
											break;
										}
									}
								}
								int tempI = i;
								i = firstIdx;
								if (memoryVec[i + 1].getNum() != -1 && memoryVec[i + 1].getSize() == memoryVec[availableIdx].getSize()) {
									memoryVec[availableIdx].setProcess(memoryVec[i + 1].getNum(), memoryVec[i + 1].getSize(), memoryVec[availableIdx].getAddress());
									memoryVec[i].setProcess(-1, memoryVec[i].getSize() + memoryVec[i + 1].getSize(), memoryVec[i].getAddress());
									blockNum--;
									for (int j = i + 1; j < memoryVec.size(); j++) {
										if (j != memoryVec.size() - 1) { //인덱스 하나씩 당겨줌
											memoryVec[j] = memoryVec[j + 1];
										}
										else
											memoryVec.pop_back(); // 마지막 원소 지워줌 
									}
									memoryVec[i].setProcess(proc.getNum(), proc.getSize(), memoryVec[i].getAddress());
									blockNum--;
									cout << "(Compaction by changing location with a process which has same size.)\n";
								}
								else {
									hole = memoryVec[availableIdx];
									for (int j = availableIdx - 1; j >= firstIdx + 1; j--) { //하나씩 옮기기
										if (memoryVec[j].getNum() != -1)
											memoryVec[j + 1] = memoryVec[j];
									}
									memoryVec[i + 1] = hole;
									memoryVec[i].setProcess(-1, memoryVec[i].getSize() + memoryVec[i + 1].getSize(), memoryVec[i].getAddress());
									blockNum--;
									for (int j = i + 1; j < memoryVec.size(); j++) {
										if (j != memoryVec.size() - 1) { //인덱스 하나씩 당겨줌
											memoryVec[j] = memoryVec[j + 1];
										}
										else
											memoryVec.pop_back(); // 마지막 원소 지워줌 
									}
									memoryVec[i].setProcess(proc.getNum(), proc.getSize(), memoryVec[i].getAddress());
									blockNum--;
									cout << "(Compaction by moving each process one by one.)\n";
								}
								freeSize -= tempSize;
								cout << "Best Fit: Allocated at address ";
								if (memoryVec[i].getAddress() == 0)
									cout << memoryVec[i].getAddress() << "\n";
								else
									cout << memoryVec[i].getAddress() << "K\n";
								cout << "          " << freeSize << "K free, " << blockNum << " block(s), average size = ";
								if (blockNum == 0)
									cout << "X\n\n";
								else
									cout << int(ceil((double)freeSize / blockNum)) << "K\n\n";
								blockCnt = 0;
								i = tempI;
								break;
							}
							else {
								cout << "Compaction is not available. There is no way to allocate this process.\n\n";
								blockCnt = 0;
								break;
							}
						}
						else
							continue;
					}
				}
				else // process일 때
					continue;
			}
			if (freeSize == 0)
				cout << "0K is free. There is no space to allocate the process.\n\n";
		}
		else { // release
			cout << "FREE REQUEST " << proc.getNum();
			for (int i = 0; i < memoryVec.size(); i++) {
				if (memoryVec[i].getNum() == proc.getNum()) { // release 대상일 때
					int tempSize = memoryVec[i].getSize();
					memoryVec[i].setProcess(-1, memoryVec[i].getSize(), memoryVec[i].getAddress()); // hole로 바꿔줌
					cout << "   (" << memoryVec[i].getSize() << "K)\n" << "Best Fit: Freed at address";
					if (memoryVec[i].getAddress() == 0)
						cout << setw(4) << memoryVec[i].getAddress() << "\n";
					else
						cout << setw(4) << memoryVec[i].getAddress() << "K\n";

					if (i != memoryVec.size() - 1 && memoryVec[i + 1].getNum() == -1) { //hole과 hole이 붙어있다면 coalescing
						cout << "          Coalescing blocks at addresses " << memoryVec[i].getAddress() << "K and " << memoryVec[i + 1].getAddress() << "K\n";
						memoryVec[i].setProcess(-1, memoryVec[i].getSize() + memoryVec[i + 1].getSize(), memoryVec[i].getAddress());
						for (int j = i + 1; j < memoryVec.size(); j++) { 
							if (j != memoryVec.size() - 1) { //인덱스 하나씩 당겨줌
								memoryVec[j] = memoryVec[j + 1];
							}
							else
								memoryVec.pop_back(); // 마지막 원소 지워줌 
						}
					}
					else if (i > 0 && memoryVec[i - 1].getNum() == -1) { //hole과 hole이 붙어있다면 coalescing
						cout << "          Coalescing blocks at addresses " << memoryVec[i - 1].getAddress() << "K and " << memoryVec[i].getAddress() << "K\n";
						memoryVec[i - 1].setProcess(-1, memoryVec[i - 1].getSize() + memoryVec[i].getSize(), memoryVec[i - 1].getAddress());
						for (int j = i; j < memoryVec.size(); j++) {
							if (j != memoryVec.size() - 1) { //인덱스 하나씩 당겨줌
								memoryVec[j] = memoryVec[j + 1];
							}
							else
								memoryVec.pop_back(); // 마지막 원소 지워줌 
						}
					}
					else { //붙어있지 않을 때
						blockNum++;
					}
					freeSize += tempSize;
				}
				else //release 대상 아닐 때
					continue;

				cout << "          " << freeSize << "K free, " << blockNum << " block(s), average size = " << int(ceil((double)freeSize / blockNum)) << "K\n\n";
				break;
			}
		}
	}
	system("pause");
	cout << "\n";
	return 0;
}
