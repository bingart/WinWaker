
////////////////////////////////////////////////
// Created by WinWakerString at 20170906113802
////////////////////////////////////////////////

#include "libstr.h"
#include "str.h"
#include <string>

char* szStrBuffers[1024] = {0};
CRITICAL_SECTION csCriticalSection;
BOOL bInitCriticalSection = FALSE;

char* GetStrById(int id)
{
	if (!bInitCriticalSection)
	{
		InitializeCriticalSection(&csCriticalSection);
		for (int i = 0; i < 1024; i++)
		{
			szStrBuffers[i] = NULL;
		}
		bInitCriticalSection = TRUE;
	}

	EnterCriticalSection(&csCriticalSection);

	if (true)
	{
		if (id < 0 || id >= 1024)
		{
			LeaveCriticalSection(&csCriticalSection);
			return "";
		}

		if (szStrBuffers[id] == NULL)
		{
			szStrBuffers[id] = new char[MAX_PATH];
			memset(szStrBuffers[id], 0, MAX_PATH);
			if (szStrBuffers[id] == NULL)
			{
				LeaveCriticalSection(&csCriticalSection);
				return "";
			}
		}
		else
		{
			LeaveCriticalSection(&csCriticalSection);
			return szStrBuffers[id];
		}
	}

	std::string encodedStr;

	switch(id)
	{
	case 101:
		// raw str -> "101@http://www.winwaker.org/download/winwaker.version.txt?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDsjOjg7NTB7Iz06IzU/MSZ6IjEmJz07OnogLCBrImlk";
		break;
	case 102:
		// raw str -> "102@http://www.winwaker.org/download/winwakerupdate.version.txt?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDsjOjg7NTB7Iz06IzU/MSYhJDA1IDF6IjEmJz07OnogLCBrImlk";
		break;
	case 103:
		// raw str -> "103@http://www.winwaker.org/download/winwakerupdate.cab?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDsjOjg7NTB7Iz06IzU/MSYhJDA1IDF6NzU2ayJpZA==";
		break;
	case 104:
		// raw str -> "104@http://www.winwaker.org/download/winwakerupdate.cfg?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDsjOjg7NTB7Iz06IzU/MSYhJDA1IDF6NzIzayJpZA==";
		break;
	case 120:
		// raw str -> "120@WinWakerUpdate"
		encodedStr = "Az06AzU/MSYBJDA1IDE=";
		break;
	case 121:
		// raw str -> "121@--cookies-file=%s/pcookies.txt %s"
		encodedStr = "eXk3Ozs/PTEneTI9ODFpcSd7JDc7Oz89MSd6ICwgdHEn";
		break;
	default:
		break;
	}

	DecodeString(encodedStr.c_str(), encodedStr.size(), szStrBuffers[id], MAX_PATH);

	LeaveCriticalSection(&csCriticalSection);

	return szStrBuffers[id];
}
