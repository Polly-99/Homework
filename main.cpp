#include <string>
#include <ctime>
#include "Array/Array.h"
#include "AVL_Tree/TTree.h"
#include "Hash_Table/Hash_Table.h"

int value(std::string line, int first, int last)
{
	int data = 0;
	int k = 1;
	if (line[first] == 45) {
		k = -1;
		first++;
	}
	try {
		for (int i = first; i < last; i++)
		{
			if (47 > line[i] || line[i] > 58)
				throw "Wrong command";
			data = data * 10 + (line[i] - 48);
		}
		data = k * data;
	}
	catch (const char * str) {
		std::cout << str << std::endl;
	}
	return data;
}

int value_first(std::string line)
{
	int first = line.find_first_of(" ") + 1;
	int last = line.find_last_of(" ");
	int data = value(line, first, last);
	return data;
}

int value_last(std::string line)
{
	int first = line.find_last_of(" ") + 1;
	int last = line.find_last_of("\n");
	if (last == -1)
		last = line.length();
	int data = value(line, first, last);
	return data;
}


int main(int argc, char *argv[])
{
	try {
		if (argc != 2)
			throw "ERROR! Wrong number of arguments.";

		std::ofstream file_out;
		file_out.open("results.ans");
		if (!file_out.is_open())
			throw "ERROR! Wrong name of output file.";

		std::ifstream file_in;
		file_in.open(argv[1]);
		if (!file_in.is_open())
			throw "ERROR! Wrong name of input file.";

		std::string line;
		clock_t start = clock();
		for (int i = 0; i < 50000; i++) {
			file_in.seekg(0);
				TArray arr;
			while (!file_in.eof())
			{
				std::getline(file_in, line);
				if (line.find("add ") == 0)
					arr.add(value_first(line), value_last(line));
				if (line.find("delete ") == 0)
					arr.remove(value_last(line));
				if (line.find("search ") == 0) {
					if (arr.search(value_last(line)))
						file_out << "\nfound: (" << arr.search(value_last(line))->first << ", " << arr.search(value_last(line))->second << ") \n";
					else
						file_out << "\n" << value_last(line) << " is not found \n";
				}
				if (line == "min")
					file_out << "\nmin: " << arr.min().first << "\n";
				if (line == "max")
					file_out << "\nmax: " << arr.max().first << "\n";
				if (line == "print") {
					file_out << "\nprint:\n";
					arr.print(file_out);
				}
			}
		}
		clock_t end = clock();
		float arr = (float)(end - start) / CLOCKS_PER_SEC;
		std::cout << "array time: " << arr << std::endl;

		start = clock();
		for (int i = 0; i < 50000; i++) {
			file_in.seekg(0);

			TTree avl;
			while (!file_in.eof())
			{
				std::getline(file_in, line);
				if (line.find("add ") == 0)
					avl.add(value_first(line), value_last(line));
				if (line == "max")
					file_out << "\nmax: " << avl.max() << "\n";
				if (line == "min")
					file_out << "\nmin: " << avl.min() << "\n";
				if (line.find("search ") == 0) {
					if (avl.search(value_last(line)) != nullptr)
						file_out << "\nfound: (" << avl.search(value_last(line))->Key << ", " << avl.search(value_last(line))->Data << ") \n";
					else
						file_out << "\n" << value_last(line) << " is not found \n";
				}
				if (line.find("delete ") == 0)
					avl.remove(value_last(line));
				if (line == "print") {
					file_out << "\nprint:\n";
					avl.print(file_out);
				}
			}  
		}
		end = clock();
		float avl = (float)(end - start) / CLOCKS_PER_SEC;
		std::cout << "AVL-tree time: " << avl << std::endl;

		start = clock();
		for (int i = 0; i < 50000; i++) {
			THash_Table <13> table;
			file_in.seekg(0);
			while (!file_in.eof())
			{
				std::getline(file_in, line);
				if (line.find("add ") == 0)
					table.add(value_first(line), value_last(line));
				if (line.find("delete ") == 0)
					table.remove(value_last(line));
				if (line.find("search ") == 0) {
					if (table.search(value_last(line)) != nullptr)
						file_out << "\nfound: (" << table.search(value_last(line))->Key << ", " << table.search(value_last(line))->Data << ") \n";
					else
						file_out << "\n" << value_last(line) << " is not found \n";
				}
				if (line == "min")
					file_out << "\nmin: " << table.min() << "\n";
				if (line == "max")
					file_out << "\nmax: " << table.max() << "\n";
				if (line == "print") {
					file_out << "\nprint:\n";
					table.print(file_out);
				}
			}
		}
		end = clock();
		float hash = (float)(end - start) / CLOCKS_PER_SEC;
		std::cout << "hash-table time: " << hash << std::endl;
		file_out.close();
		file_in.close();
		if (arr < hash) {
			if (arr < avl)
				std::cout << "array is the best";
			else
				std::cout << "avl is the best";
		}
		else {
			if (hash < avl)
				std::cout << "hash is the best";
			else
				std::cout << "avl is the best";
		}
	}
	catch (const char * str)
	{
		std::cout << str << std::endl;
	}
	return 0;
}