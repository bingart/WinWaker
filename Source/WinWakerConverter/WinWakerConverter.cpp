// WinWakerConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "codec.h"
#include "scrambler.h"

#define BLOCK_SIZE 64

void Usage()
{
	printf("WinWakerConverter e exe_file\r\n");
	printf("WinWakerConverter d txt_file\r\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 3)
	{
		Usage();
		return 0;
	}

	if (strcmp(argv[1], "e") == 0)
	{
		std::string exe_file_name = argv[2];
		std::string txt_file_name = exe_file_name + ".txt";
		Encode(exe_file_name.c_str(), txt_file_name.c_str());
	}
	else if (strcmp(argv[1], "d") == 0)
	{
		std::string txt_file_name = argv[2];
		std::string exe_file_name = txt_file_name + ".exe";
		Decode(txt_file_name.c_str(), exe_file_name.c_str());
	}
	else
	{
		Usage();
	}

	return 0;
}

