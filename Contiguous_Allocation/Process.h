#pragma once

class Process
{
private:
	int num;
	int size;
	int address;

public:
	Process() : num(-1), size(-1), address(0) {}

	void setProcess(int _num, int _size, int _address) {
		num = _num;
		size = _size;
		address = _address;
	}

	void setSize(int _size) {
		size = _size;
	}

	void setAddress(int _address) {
		address = _address;
	}

	int getSize() {
		return size;
	}

	int getNum() {
		return num;
	}

	int getAddress() {
		return address;
	}

};