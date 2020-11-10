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
	int pageFaultNum = 0, seq = 1, currentSize = 0; // seq = ���� ����. ���� ���� ���� seq�� ���� ����.
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
			if (IsFull = false) // full�� �ƴϰ� �Ҵ�Ǿ����� ���� page��� ������� �Ҵ�
			{
				tempPage.setPage(inputV[i].getPageNum(), seq);
				pageV.push_back(tempPage);
			}
			else // full�ε� page fault �߻�
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
		else //�̹� �Ҵ�Ǿ��ִ� page��� continue
			continue;
	}

	return pageFaultNum;
}

int LRU(vector<Page> inputV, vector<Page>& pageV)
{
	int pageFaultNum = 0, seq = 1, currentSize = 0; // seq = ���� ����. �ֱ� ����Ҽ��� seq ����.
	bool IsFull = false, IsAllocated = false;
	Page tempPage;

	for (int i = 0; i < inputV.size(); i++)
	{
		for (int j = 0; j < pageV.size(); j++)
		{
			if (pageV[j].getPageNum() == inputV[i].getPageNum())
			{
				IsAllocated = true;
				pageV[j].setSequenceNum(seq); // seq�� �������� �ֱ� ����� �� ǥ��
				seq++;
				break;
			}
		}

		if (currentSize == FRAME_SIZE)
			IsFull = true;

		if (IsAllocated == false)
		{
			if (IsFull = false) // full�� �ƴϰ� �Ҵ�Ǿ����� ���� page��� ������� �Ҵ�
			{
				tempPage.setPage(inputV[i].getPageNum(), seq);
				pageV.push_back(tempPage);
			}
			else // full�ε� page fault �߻�
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
		else //�̹� �Ҵ�Ǿ��ִ� page��� continue
			continue;
	}

	return pageFaultNum;
}

int Optimal(vector<Page> inputV, vector<Page>& pageV)
{
	int pageFaultNum = 0, seq = 1, currentSize = 0; // seq = �̷��� ���� ����. ���߿� ���� �����ϼ��� seq ����.
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
			if (IsFull = false) // full�� �ƴϰ� �Ҵ�Ǿ����� ���� page��� ������� �Ҵ�
			{
				tempPage.setPage(inputV[i].getPageNum(), seq);
				pageV.push_back(tempPage);
			}
			else // full�ε� page fault �߻�
			{
				int minSeq = pageV[pageV.size() - 1].getSequenceNum(), optimalIdx = 0;
				for (int j = pageV.size() - 1; j >= 0; j--) // �ڿ������� ���鼭 ���� ���߿� ���� page�� seq�� ���� �۰� ����
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
		else //�̹� �Ҵ�Ǿ��ִ� page��� continue
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
	for (int i = 0; i < 500; i++) // ����(0~100) ���� �� file�� ��������
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
	while (!in.eof()) // file�� ������ ���� �о�ͼ� inputVec�� ����
	{
		in >> temp;

		tempPage.setPage(temp, -1);
		inputV.push_back(tempPage);
	}
	in.close();
}
