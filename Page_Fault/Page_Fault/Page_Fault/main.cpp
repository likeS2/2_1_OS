#include "Page.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <cstdlib>
#define FRAME_SIZE 100

using namespace std;

int FIFO(vector<Page> inputV, vector<Page>& pageV);
int LRU(vector<Page> inputV, vector<Page>& pageV);
int Optimal(vector<Page> inputV, vector<Page>& pageV);
void application(ofstream& out, ifstream& in, vector<Page> inputV, vector<Page>& pageV);
void fileIO(ofstream& out, ifstream& in, vector<Page>& inputV);

int main() 
{
	vector<Page> pageVec(FRAME_SIZE);
	vector<Page> inputVec;
	ofstream fout;
	ifstream fin;

	application(fout, fin, inputVec, pageVec);

	system("pause");
	return 0;
}


int FIFO(vector<Page> inputV, vector<Page>& pageV)
{
	int pageFaultNum = 0, seq = 1, currentSize = 0; // seq = 들어온 순서. 제일 먼저 들어온 seq가 제일 작음.
	bool IsFull = false, IsAllocated = false;
	Page tempPage;

	for (int i = 0; i < inputV.size(); i++) 
	{
		for (int j = 0; j < pageV.size(); j++) 
		{
			if (pageV[j].getPageNum() == inputV[i].getPageNum())
			{
				IsAllocated = true;
				break;
			}
		}

		if (currentSize == FRAME_SIZE)
			IsFull = true;

		if (IsAllocated == false)
		{
			if (IsFull = false) // full이 아니고 할당되어있지 않은 page라면 순서대로 할당
			{
				tempPage.setPage(inputV[i].getPageNum(), seq);
				pageV.push_back(tempPage);
			}
			else // full인데 page fault 발생
			{
				int minSeq = pageV[0].getSequenceNum(), firstComeIdx = 0;
				for (int j = 0; j < pageV.size(); j++)
				{
					if (pageV[j].getSequenceNum() <= minSeq)
					{
						minSeq = pageV[j].getSequenceNum();
						firstComeIdx = j;
					}
				}
				pageV[firstComeIdx].setPage(inputV[i].getPageNum(), seq);
				pageFaultNum++;

			}
			currentSize++;
			seq++;
		}
		else //이미 할당되어있는 page라면 continue
			continue;
	}

	return pageFaultNum;
}

int LRU(vector<Page> inputV, vector<Page>& pageV)
{
	int pageFaultNum = 0, seq = 1, currentSize = 0; // seq = 사용된 시점. 최근 사용할수록 seq 증가.
	bool IsFull = false, IsAllocated = false;
	Page tempPage;

	for (int i = 0; i < inputV.size(); i++)
	{
		for (int j = 0; j < pageV.size(); j++)
		{
			if (pageV[j].getPageNum() == inputV[i].getPageNum())
			{
				IsAllocated = true;
				pageV[j].setSequenceNum(seq); // seq를 증가시켜 최근 사용한 것 표시
				seq++;
				break;
			}
		}

		if (currentSize == FRAME_SIZE)
			IsFull = true;

		if (IsAllocated == false)
		{
			if (IsFull = false) // full이 아니고 할당되어있지 않은 page라면 순서대로 할당
			{
				tempPage.setPage(inputV[i].getPageNum(), seq);
				pageV.push_back(tempPage);
			}
			else // full인데 page fault 발생
			{
				int minSeq = pageV[0].getSequenceNum(), leastRecentlyUsedIdx = 0;
				for (int j = 0; j < pageV.size(); j++)
				{
					if (pageV[j].getSequenceNum() <= minSeq)
					{
						minSeq = pageV[j].getSequenceNum();
						leastRecentlyUsedIdx = j;
					}
				}
				pageV[leastRecentlyUsedIdx].setPage(inputV[i].getPageNum(), seq);
				pageFaultNum++;

			}
			currentSize++;
			seq++;
		}
		else //이미 할당되어있는 page라면 continue
			continue;
	}

	return pageFaultNum;
}

int Optimal(vector<Page> inputV, vector<Page>& pageV)
{
	int pageFaultNum = 0, seq = 1, currentSize = 0; // seq = 미래에 사용될 시점. 나중에 사용될 예정일수록 seq 작음.
	bool IsFull = false, IsAllocated = false;
	Page tempPage;

	for (int i = 0; i < inputV.size(); i++)
	{
		for (int j = 0; j < pageV.size(); j++)
		{
			if (pageV[j].getPageNum() == inputV[i].getPageNum())
			{
				IsAllocated = true;
				break;
			}
		}

		if (currentSize == FRAME_SIZE)
			IsFull = true;

		if (IsAllocated == false)
		{
			if (IsFull = false) // full이 아니고 할당되어있지 않은 page라면 순서대로 할당
			{
				tempPage.setPage(inputV[i].getPageNum(), seq);
				pageV.push_back(tempPage);
			}
			else // full인데 page fault 발생
			{
				int minSeq = pageV[pageV.size() - 1].getSequenceNum(), optimalIdx = 0;
				for (int j = pageV.size() - 1; j >= 0; j--) // 뒤에서부터 돌면서 가장 나중에 사용될 page의 seq를 가장 작게 지정
				{
					if (pageV[j].getSequenceNum() <= minSeq)
					{
						minSeq = pageV[j].getSequenceNum();
						optimalIdx = j;
					}
				}
				pageV[optimalIdx].setPage(inputV[i].getPageNum(), seq);
				pageFaultNum++;

			}
			currentSize++;
			seq++;
		}
		else //이미 할당되어있는 page라면 continue
			continue;
	}

	return pageFaultNum;
}

void application(ofstream& out, ifstream& in, vector<Page> inputV, vector<Page>& pageV)
{
	int select;

	fileIO(out, in, inputV);

	while (true)
	{
		cout << "=========Simulation of Page Fault=========\n";
		cout << " 1. FIFO page replacement\n";
		cout << " 2. LRU page replacement\n";
		cout << " 3. Optimal page replacement\n";
		cout << "------------------------------------------\n";
		cout << "Please enter a number: ";
		cin >> select;
		cout << "------------------------------------------\n";

		if (0 < select && select < 4)
			break;
		else
			cout << "Please enter a number in range of 1~3\n\n";
	}

	switch (select)
	{
	case 1:
		cout << "1. FIFO page replacement\n";
		cout << "Number of page fault occurrences: " << FIFO(inputV, pageV) << "\n";
		cout << "==========================================\n\n";
		break;
	case 2:
		cout << "2. LRU page replacement\n";
		cout << "Number of page fault occurrences: " << LRU(inputV, pageV) << "\n";
		cout << "==========================================\n\n";
		break;
	case 3:
		cout << "3. Optimal page replacement\n";
		cout << "Number of page fault occurrences: " << Optimal(inputV, pageV) << "\n";
		cout << "==========================================\n\n";
		break;
	default:
		break;
	}
}

void fileIO(ofstream & out, ifstream & in, vector<Page>& inputV)
{
	srand((unsigned)time(NULL));

	out.open("input.txt");
	for (int i = 0; i < 500; i++) // 난수(0~100) 생성 후 file에 내보내기
	{
		if (i != 499)
			out << rand() % 101 << " ";
		else
			out << rand() % 101;
	}
	out.close();


	Page tempPage;
	int temp;
	in.open("input.txt");
	while (!in.eof()) // file로 내보낸 난수 읽어와서 inputVec에 저장
	{
		in >> temp;

		tempPage.setPage(temp, -1);
		inputV.push_back(tempPage);
	}
	in.close();
}
