#include <windows.h>
#include <iostream>
#include <vector>
#include "scrambler.h"
#include "easylog.hpp"

BOOL GetArguments(std::string line, char delimeter, std::vector<std::string>& arguments)
{
	arguments.clear();
	std::string arg;

	for (size_t pos = 0; pos < line.size(); pos++)
	{
		char ch = line.data()[pos];

		if (ch == delimeter)
		{
			if (arg.size() == 0)
			{
				// ignore
			}
			else
			{
				// new argument
				arguments.push_back(arg);

				// reset arg
				arg.clear();
			}
		}
		else
		{
			arg += ch;
		}
	}

	// the last argument
	if (arg.size() > 0)
	{
		arguments.push_back(arg);
	}

	return arguments.size() > 0;
}
