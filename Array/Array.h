#pragma once
#include <iostream>
#include <fstream>

class TArray {
	std::pair<int, int> * Ptr;
	int Size;
	int Capacity;

	int find(int key) {
		for (int i = 0; i < Size; i++) {
			if (Ptr[i].first == key)
				return i;
		}
		return -1;
	}

	void resize()
	{
		TArray copy(*this);
		Capacity = Capacity*2 + 1;
		Ptr = new std::pair<int, int>[Capacity];
		for (int i = 0; i < Size; i++) {
			Ptr[i] = copy.Ptr[i];
		}
	}

public:
	TArray() 
	: Ptr(nullptr)
	, Size(0)
	, Capacity(0)
	{ }

	TArray(int capacity)
		: Size(0)
		, Capacity(capacity)
	{
		Ptr = new std::pair<int, int>[Capacity];
	}

	~TArray(){
		delete[] Ptr;
	}

	std::pair<int, int> & operator[](int index)
	{
		return Ptr[index];
	}

	void add(int key, int data) {
		if (Size + 1 > Capacity)
			resize();
		std::pair<int, int> value(key, data);
		Ptr[Size] = value;
		int i = Size;
		Size++;
		while  (i > 0 && Ptr[i] < Ptr[i - 1]) {
			std::swap(Ptr[i], Ptr[i - 1]);
				i--;
		}
	}

	std::pair<int, int>  * search(int key) {
		int i = find(key);
		if (i == -1)
			return nullptr;
		return &Ptr[i];
	}

	void remove(int key) {
		if (find(key) == -1)
			return;
		for (int i = find(key); i < Size - 1; i++)
		{
			Ptr[i] = Ptr[i + 1];
		}
		Size--;
	}

	std::pair<int, int> min(){
		return Ptr[0];
	}

	std::pair<int, int> max() {
		return Ptr[Size - 1];
	}

	void print(std::ofstream& file_out)
	{
		for (int i = 0; i < Size; i++) {
			file_out << "(" << Ptr[i].first << ", " << Ptr[i].second << ") ";
		}
	}
};
