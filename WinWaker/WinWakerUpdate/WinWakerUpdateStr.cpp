
////////////////////////////////////////////////
// Created by WinWakerUpdateString at 20150119103202
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
	case 120:
		// raw str -> "120@WinWakerUpdate"
		encryptStr = "Az06AzU/MSYBJDA1IDE=";
		break;
	case 121:
		// raw str -> "121@http://www.winwaker.org/adweb/db/report_bug.php?v=0"
		encryptStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7NTAjMTZ7MDZ7JjEkOyYgCzYhM3okPCRrImlk";
		break;
	case 202:
		// raw str -> "202@WinWaker Update Service"
		encryptStr = "Az06AzU/MSZ0ASQwNSAxdAcxJiI9NzE=";
		break;
	case 205:
		// raw str -> "205@Log.log"
		encryptStr = "GDszejg7Mw==";
		break;
	case 206:
		// raw str -> "206@#### start\n"
		encryptStr = "d3d3d3QnIDUmIF4=";
		break;
	case 207:
		// raw str -> "207@install"
		encryptStr = "PTonIDU4OA==";
		break;
	case 208:
		// raw str -> "208@install end\n"
		encryptStr = "PTonIDU4OHQxOjBe";
		break;
	case 209:
		// raw str -> "209@Cannot install service, error %d\n"
		encryptStr = "FzU6OjsgdD06JyA1ODh0JzEmIj03MXh0MSYmOyZ0cTBe";
		break;
	case 210:
		// raw str -> "210@OpenSCManager failed (%d)\n"
		encryptStr = "GyQxOgcXGTU6NTMxJnQyNT04MTB0fHEwfV4=";
		break;
	case 211:
		// raw str -> "211@CreateService failed, error %d\n"
		encryptStr = "FyYxNSAxBzEmIj03MXQyNT04MTB4dDEmJjsmdHEwXg==";
		break;
	case 212:
		// raw str -> "212@CreateService successfully\n"
		encryptStr = "FyYxNSAxBzEmIj03MXQnITc3MScnMiE4OC1e";
		break;
	case 213:
		// raw str -> "213@SvcLog.log"
		encryptStr = "ByI3GDszejg7Mw==";
		break;
	case 215:
		// raw str -> "215@started #####\n"
		encryptStr = "JyA1JiAxMHR3d3d3d14=";
		break;
	case 216:
		// raw str -> "216@RegisterServiceCtrlHandler fails, error %d\n"
		encryptStr = "BjEzPScgMSYHMSYiPTcxFyAmOBw1OjA4MSZ0MjU9OCd4dDEmJjsmdHEwXg==";
		break;
	case 217:
		// raw str -> "217@RegisterServiceCtrlHandler ok\n"
		encryptStr = "BjEzPScgMSYHMSYiPTcxFyAmOBw1OjA4MSZ0Oz9e";
		break;
	case 218:
		// raw str -> "218@SvcMain return\n"
		encryptStr = "ByI3GTU9OnQmMSAhJjpe";
		break;
	case 219:
		// raw str -> "219@SvcInit\n"
		encryptStr = "ByI3HTo9IF4=";
		break;
	case 222:
		// raw str -> "222@SvcInit return\n"
		encryptStr = "ByI3HTo9IHQmMSAhJjpe";
		break;
	case 226:
		// raw str -> "226@SvcCtrlHandler stop\n"
		encryptStr = "ByI3FyAmOBw1OjA4MSZ0JyA7JF4=";
		break;
	case 227:
		// raw str -> "227@%s failed with %d"
		encryptStr = "cSd0MjU9ODEwdCM9IDx0cTA=";
		break;
	case 255:
		// raw str -> "255@main thread started #####\n"
		encryptStr = "OTU9OnQgPCYxNTB0JyA1JiAxMHR3d3d3d14=";
		break;
	case 256:
		// raw str -> "256@EXPLORER.EXE"
		encryptStr = "EQwEGBsGEQZ6EQwR";
		break;
	case 257:
		// raw str -> "257@task wait %s ok\n"
		encryptStr = "IDUnP3QjNT0gdHEndDs/Xg==";
		break;
	case 258:
		// raw str -> "258@task wait %s fails, continue ... %d\n"
		encryptStr = "IDUnP3QjNT0gdHEndDI1PTgneHQ3OzogPTohMXR6enp0cTBe";
		break;
	case 259:
		// raw str -> "259@sleep: %d\n"
		encryptStr = "JzgxMSRudHEwXg==";
		break;
	case 260:
		// raw str -> "260@thread exit\n"
		encryptStr = "IDwmMTUwdDEsPSBe";
		break;
	case 301:
		// raw str -> "301@http://www.winwaker.org/adweb/download/winwakerupdate.txt?v=0"
		encryptStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7NTAjMTZ7MDsjOjg7NTB7Iz06IzU/MSYhJDA1IDF6ICwgayJpZA==";
		break;
	case 305:
		// raw str -> "305@decode %s to %s, rc %d\n"
		encryptStr = "MDE3OzAxdHEndCA7dHEneHQmN3RxMF4=";
		break;
	case 315:
		// raw str -> "315@Version"
		encryptStr = "AjEmJz07Og==";
		break;
	case 316:
		// raw str -> "316@http://www.winwaker.org/adweb/download/version.txt?v=0"
		encryptStr = "PCAgJG57eyMjI3ojPTojNT8xJno7JjN7NTAjMTZ7MDsjOjg7NTB7IjEmJz07OnogLCBrImlk";
		break;
	case 317:
		// raw str -> "317@version not found %d or %d not expired\n"
		encryptStr = "IjEmJz07OnQ6OyB0MjshOjB0cTB0OyZ0cTB0OjsgdDEsJD0mMTBe";
		break;
	case 318:
		// raw str -> "318@version file open fails\n"
		encryptStr = "IjEmJz07OnQyPTgxdDskMTp0MjU9OCde";
		break;
	case 319:
		// raw str -> "319@version found %d\n"
		encryptStr = "IjEmJz07OnQyOyE6MHRxMF4=";
		break;
	case 323:
		// raw str -> "323@set auto delete to false\n"
		encryptStr = "JzEgdDUhIDt0MDE4MSAxdCA7dDI1OCcxXg==";
		break;
	default:
		break;
	}

	DecryptString(encryptStr.c_str(), encryptStr.size(), szStrBuffers[id], MAX_PATH);

	LeaveCriticalSection(&csCriticalSection);

	return szStrBuffers[id];
}
