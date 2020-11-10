#pragma once

class Page
{
private:
	int pageNum;
	int sequenceNum;

public:
	Page() : pageNum(-1), sequenceNum(-1) {}
	void setPage(int _pageNum, int _sequenceNum)
	{
		pageNum = _pageNum;
		sequenceNum = _sequenceNum;
	}

	void setSequenceNum(int _sequenceNum) {
		sequenceNum = _sequenceNum;
	}

	int getPageNum() 
	{
		return pageNum;
	}

	int getSequenceNum()
	{
		return sequenceNum;
	}
};