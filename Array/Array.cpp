#include "Array.h"
#include <string>

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
		if (argc != 3)
			throw "ERROR! Wrong number of arguments.";

		std::ifstream file_in;
		file_in.open(argv[1]);
		if (!file_in.is_open())
			throw "ERROR! Wrong name of input file.";

		std::ofstream file_out;
		file_out.open(argv[2]);
		if (!file_out.is_open())
			throw "ERROR! Wrong name of output file.";

		TArray arr(10);
		std::string line;
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
		file_out.close();
		file_in.close();
	}
	catch (const char * str)
	{
		std::cout << str << std::endl;
	}
	return 0;
}