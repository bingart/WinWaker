
////////////////////////////////////////////////
// Created by WinWakerString at 20170901153749
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

	std::string encryptStr;

	switch(id)
	{
	case 101:
		// raw str -> "101@http://www.winwaker.org/data/download/version.txt?v=0"
		encryptStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDUgNXswOyM6ODs1MHsiMSYnPTs6eiAsIGsiaWQ=";
		break;
	case 102:
		// raw str -> "102@http://www.winwaker.org/data/download/winwaker.txt?v=0"
		encryptStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDUgNXswOyM6ODs1MHsjPTojNT8xJnogLCBrImlk";
		break;
	case 120:
		// raw str -> "120@WinWaker"
		encryptStr = "Az06AzU/MSY=";
		break;
	case 121:
		// raw str -> "121@http://www.winwaker.org/data/db/report_bug.php?v=0"
		encryptStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDUgNXswNnsmMSQ7JiALNiEzeiQ8JGsiaWQ=";
		break;
	default:
		break;
	}

	DecryptString(encryptStr.c_str(), encryptStr.size(), szStrBuffers[id], MAX_PATH);

	LeaveCriticalSection(&csCriticalSection);

	return szStrBuffers[id];
}
