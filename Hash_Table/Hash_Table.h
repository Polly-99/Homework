#pragma once
#include <iostream>

struct TCell {
	int Key;
	int Data;
	TCell * Next;

	TCell()
		:Next(nullptr)
		, Key(NULL)
		, Data(NULL)
	{ }

	TCell(int key, int data)
		:Next(nullptr)
		, Key(key)
		, Data(data)
	{ }

	~TCell()
	{
		if (Next)
			delete Next;
	}
};

template<std::size_t N>
class THash_Table
{
	TCell * table[N];

	int hash(int key)
	{
		return key % N;
	}

public:

	THash_Table()
	{
		for (int i = 0; i < N; i++)
			table[i] = nullptr;
	}

	~THash_Table()
	{
		for (int i = 0; i < N; i++) {
			if (table[i] != nullptr)
				delete table[i];
		}
	}

	TCell * add(int key, int data)
	{
		if (table[hash(key)]) {
			TCell * cur = table[hash(key)];
			while (cur->Next) {
				if (key == cur->Key) {
					return cur;
				}
				cur = cur->Next;
			}
			if (key == cur->Key)
				return cur;
			cur->Next = new TCell(key, data);
			return cur->Next;
		}
		table[hash(key)] = new TCell(key, data);
		return table[hash(key)];
	}

	TCell * search(int key)
	{
		if (table[hash(key)]) {
			TCell * cur = table[hash(key)];
			while (cur && cur->Key != key)
				cur = cur->Next;
			return cur;
		}
		return nullptr;
	}

	TCell * remove(int key)
	{
		if (table[hash(key)]) {
			TCell * cur = table[hash(key)];
			if (cur->Key == key) {
				table[hash(key)] = cur->Next;
				cur->Next = nullptr;
				delete cur;
				return table[hash(key)];
			}
			while (cur->Next && cur->Next->Key != key)
				cur = cur->Next;
			if (cur->Next) {
				TCell * del = cur->Next;
				cur->Next = del->Next;
				del->Next = nullptr;
				delete del;
			}
			return cur;
		}
		return nullptr;
	}

	int min()
	{
		TCell * cur = nullptr;
		int i = 0;
		while (table[i] == nullptr)
			i++;
		int min = table[i]->Key;
		for (i; i < N; i++)
		{
			if (table[i]) {
				cur = table[i];
				while (cur) {
					if (cur->Key < min)
						min = cur->Key;
					cur = cur->Next;
				}
			}
		}
		return min;
	}

	int max()
	{
		TCell * cur = nullptr;
		int i = 0;
		while (table[i] == nullptr)
			i++;
		int max = table[i]->Key;
		for (i; i < N; i++)
		{
			if (table[i]) {
				cur = table[i];
				while (cur) {
					if (cur->Key > max)
						max = cur->Key;
					cur = cur->Next;
				}
			}
		}
		return max;
	}

	void print(const char * filename)
	{
		try {
			std::ofstream file_out(filename, std::ios_base::in | std::ios_base::app);
			if (!file_out.is_open())
				throw "ERROR! Wrong name of output file.";
			TCell * cur = nullptr;
			for (int i = 0; i < N; i++)
			{
				file_out << i << ": ";
				if (table[i]) {
					cur = table[i];
					while (cur) {
						file_out << "(" << cur->Key << "; " << cur->Data << "), ";
						cur = cur->Next;
					}
				}
				file_out << "\n";
			}
			file_out.close();
		}
		catch (const char * str) {
			std::cout << str << std::endl;
		}
	}
};
