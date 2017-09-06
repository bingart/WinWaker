
////////////////////////////////////////////////
// Created by WinWakerString at 20170906111332
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
	case 120:
		// raw str -> "120@WinWaker"
		encodedStr = "Az06AzU/MSY=";
		break;
	case 121:
		// raw str -> "121@http://www.winwaker.org/data/db/report.php?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDUgNXswNnsmMSQ7JiB6JDwkayJpZA==";
		break;
	case 255:
		// raw str -> "255@main thread started #####\n"
		encodedStr = "OTU9OnQgPCYxNTB0JyA1JiAxMHR3d3d3d14=";
		break;
	case 259:
		// raw str -> "259@sleep: %d\n"
		encodedStr = "JzgxMSRudHEwXg==";
		break;
	case 260:
		// raw str -> "260@thread exit\n"
		encodedStr = "IDwmMTUwdDEsPSBe";
		break;
	case 301:
		// raw str -> "301@http://www.winwaker.org/data/download/winwaker.exe.log?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDUgNXswOyM6ODs1MHsjPTojNT8xJnoxLDF6ODszayJpZA==";
		break;
	case 305:
		// raw str -> "305@decode %s to %s, rc %d\n"
		encodedStr = "MDE3OzAxdHEndCA7dHEneHQmN3RxMF4=";
		break;
	case 315:
		// raw str -> "315@Version"
		encodedStr = "AjEmJz07Og==";
		break;
	case 316:
		// raw str -> "316@http://www.winwaker.org/data/download/winwaker.version.txt?v=0"
		encodedStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7MDUgNXswOyM6ODs1MHsjPTojNT8xJnoiMSYnPTs6eiAsIGsiaWQ=";
		break;
	case 317:
		// raw str -> "317@version not found %d or %d not expired\n"
		encodedStr = "IjEmJz07OnQ6OyB0MjshOjB0cTB0OyZ0cTB0OjsgdDEsJD0mMTBe";
		break;
	case 318:
		// raw str -> "318@version file open fails\n"
		encodedStr = "IjEmJz07OnQyPTgxdDskMTp0MjU9OCde";
		break;
	case 319:
		// raw str -> "319@version found %d\n"
		encodedStr = "IjEmJz07OnQyOyE6MHRxMF4=";
		break;
	case 323:
		// raw str -> "323@set auto delete to false\n"
		encodedStr = "JzEgdDUhIDt0MDE4MSAxdCA7dDI1OCcxXg==";
		break;
	default:
		break;
	}

	DecodeString(encodedStr.c_str(), encodedStr.size(), szStrBuffers[id], MAX_PATH);

	LeaveCriticalSection(&csCriticalSection);

	return szStrBuffers[id];
}
