
#include <windows.h>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include "scrambler.h"
#include "process.h"
#include "argument.h"
#include "easylog.hpp"
#include "libstr.h"

extern BOOL bRunning;

std::string toupper(const std::string & s)
{
    std::string ret(s.size(), char());
    for(unsigned int i = 0; i < s.size(); ++i)
        ret[i] = (s[i] <= 'z' && s[i] >= 'a') ? s[i]-('a'-'A') : s[i];
    return ret;
}

BOOL IsProcessExists(LPSTR lpName)
{
	if(!lpName)
	{
		return FALSE;
	}

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		// "CreateToolhelp32Snapshot fails, error %d\n"
		EasyLog::Format(GetLibStrById(522), GetLastError());
		return FALSE;
	}
	
	BOOL found = FALSE;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			std::string s = pe32.szExeFile;
			std::string s2 = lpName;
			s = toupper(s);
			s2 = toupper(s2);
			if(s.compare(s2) == 0)
			{
				found = TRUE;
				break;
			}
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	else
	{
		// "Process32First fails, error %d\n"
		EasyLog::Format(GetLibStrById(523), GetLastError());
    }
	
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		CloseHandle (hProcessSnap);
	}
	return found;
}

BOOL GetTokenByName(HANDLE &hToken, LPSTR lpName)
{
	if(!lpName)
	{
		return FALSE;
	}

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		// "CreateToolhelp32Snapshot fails, error %d\n"
		EasyLog::Format(GetLibStrById(524), GetLastError());
		return (FALSE);
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if(!strcmp(_strupr(pe32.szExeFile),_strupr(lpName)))
			{
				HANDLE hProcess =
					OpenProcess(
						PROCESS_QUERY_INFORMATION,
						FALSE,
						pe32.th32ProcessID);
				BOOL bRet = OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
				if (bRet)
				{
					CloseHandle (hProcessSnap);
					return TRUE;
				}
				else
				{
					// "OpenProcessToken fails, error %d\n"
					EasyLog::Format(GetLibStrById(525), GetLastError());
				}
			}
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	else
	{
		// "Process32First fails, error %d\n"
		EasyLog::Format(GetLibStrById(526), GetLastError());
    }
	
	CloseHandle (hProcessSnap);
	return FALSE;
}

BOOL ExecProcess(LPCSTR lpImage, const char* szArguments)
{
	if(!lpImage)
	{
		return FALSE;
	}

	std::string szCmdLine = lpImage;
	if (szArguments != NULL)
	{
		szCmdLine += " ";
		szCmdLine += szArguments;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	BOOL bResult = CreateProcess(
		NULL,   // No module name (use command line). 
		(LPSTR) szCmdLine.c_str(), // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi);             // Pointer to PROCESS_INFORMATION structure.

	if (bResult)
	{
		// "run ok\n"
		EasyLog::Format(GetLibStrById(560));
	}
	else
	{
		// "run fails\n"
		EasyLog::Format(GetLibStrById(561));
	}

	return bResult;
}

BOOL RunProcess(LPCSTR lpImage, int nWaitTime, const char* szArguments = NULL)
{
	if(!lpImage)
	{
		return FALSE;
	}

	std::string szCmdLine = lpImage;
	if (szArguments != NULL)
	{
		szCmdLine += " ";
		szCmdLine += szArguments;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	BOOL bResult = CreateProcess(
		NULL,   // No module name (use command line). 
		(LPSTR) szCmdLine.c_str(), // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi);             // Pointer to PROCESS_INFORMATION structure.

	if (bResult)
	{
		hProcess = pi.hProcess;
		hThread = pi.hThread;

		int count = 0;
		BOOL bExit = FALSE;
		while (bRunning && count < nWaitTime)
		{
			DWORD dwResult = WaitForSingleObject(hProcess, 1000);
			if (dwResult == WAIT_OBJECT_0)
			{
				// "wait process %s to exit, get signal\n"
				EasyLog::Format(GetLibStrById(527), lpImage);
				bExit = TRUE;
				break;
			}
			else if (dwResult == WAIT_TIMEOUT)
			{
				// "wait process %s to exit, time out %d\n"
				EasyLog::Format(GetLibStrById(528), lpImage, count);
			}
			else
			{
				// "wait process %s to exit, failed, break\n"
				EasyLog::Format(GetLibStrById(529), lpImage);
				break;
			}

			count++;
		}

		// "wait process %s to exit, finish, bRunning %d, bExit %d\n"
		EasyLog::Format(GetLibStrById(530), lpImage, bRunning, bExit);

		if (!bExit)
		{
			// terminate process
			::TerminateProcess(hProcess, 0);
			// "terminate process %s\n"
			EasyLog::Format(GetLibStrById(531), lpImage);
		}

		CloseHandle(hProcess);
		CloseHandle(hThread);
	}

	return bResult;
}

BOOL RunProcessAsUser(LPCSTR lpImage, int nWaitTime, const char* szArguments = NULL)
{
	if(!lpImage)
	{
		return FALSE;
	}

	std::string szCmdLine = lpImage;
	if (szArguments != NULL)
	{
		szCmdLine += " ";
		szCmdLine += szArguments;
	}

	HANDLE hToken = NULL;
	HANDLE hUserToken = NULL;
	BOOL bResult = false;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	do
	{
		// Get token of explorer.exe
		// "EXPLORER.EXE"
		if(!GetTokenByName(hToken, GetLibStrById(532)))
		{
			// "GetTokenByName fails, error %d\n"
			EasyLog::Format(GetLibStrById(533), GetLastError());
			break;
		}

		// Duplicate token
		if(!DuplicateTokenEx(
			hToken,
			MAXIMUM_ALLOWED,
			NULL,
			SecurityIdentification,
			TokenPrimary,
			&hUserToken))
		{
			// "DuplicateTokenEx fails, error %d\n"
			EasyLog::Format(GetLibStrById(534), GetLastError());
			break;
		}

		// Create process as user
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		// "winsta0\\default"
		si.lpDesktop = GetLibStrById(550);
		bResult = CreateProcessAsUser(
				hUserToken,
				(LPSTR) szCmdLine.c_str(),
				NULL,
				NULL,
				NULL,
				FALSE,	// BOOL inheritHandles
				NORMAL_PRIORITY_CLASS,
				NULL,
				NULL,
				&si,
				&pi);
		if(bResult)
		{
			// "CreateProcessAsUser %s ok\n"
			EasyLog::Format(GetLibStrById(535), lpImage);
		}
		else
		{
			// "CreateProcessAsUser %s fails, error %d\n"
			EasyLog::Format(GetLibStrById(536), lpImage, GetLastError());
		}
	}
	while (false);

	if (bResult)
	{
		HANDLE hProcess = pi.hProcess;
		HANDLE hThread = pi.hThread;

		int count = 0;
		BOOL bExit = FALSE;
		while (bRunning && count < nWaitTime)
		{
			DWORD dwResult = WaitForSingleObject(hProcess, 1000);
			if (dwResult == WAIT_OBJECT_0)
			{
				// "wait process %s to exit, get signal\n"
				EasyLog::Format(GetLibStrById(537), lpImage);
				bExit = TRUE;
				break;
			}
			else if (dwResult == WAIT_TIMEOUT)
			{
				// "wait process %s to exit, time out %d\n"
				EasyLog::Format(GetLibStrById(538), lpImage, count);
			}
			else
			{
				// "wait process %s to exit, failed, break\n"
				EasyLog::Format(GetLibStrById(539), lpImage);
				break;
			}

			count++;
		}

		// "wait process %s to exit, finish, bRunning %d, bExit %d\n"
		EasyLog::Format(GetLibStrById(540), lpImage, bRunning, bExit);

		if (!bExit)
		{
			// terminate process
			::TerminateProcess(hProcess, 0);
			// "terminate process %s\n"
			EasyLog::Format(GetLibStrById(541), lpImage);
		}

		CloseHandle(hProcess);
		CloseHandle(hThread);
	}

	if (!hToken) CloseHandle(hToken);
	if (!hUserToken) CloseHandle(hUserToken);

	return bResult;
}

BOOL MyTerminateProcess(LPCSTR lpName)
{
	if(!lpName)
	{
		return FALSE;
	}

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};
	BOOL bResult = FALSE;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		// "CreateToolhelp32Snapshot fails, error %d\n"
		EasyLog::Format(GetLibStrById(542), GetLastError());
		return (FALSE);
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if(!strcmp(_strupr(pe32.szExeFile),_strupr((char *)lpName)))
			{
				HANDLE hProcess =
					OpenProcess(
						PROCESS_QUERY_INFORMATION,
						FALSE,
						pe32.th32ProcessID);
				BOOL rc = TerminateProcess(hProcess, 0);
				if (rc)
				{
					// "Terminate process %s ok\n"
					EasyLog::Format(GetLibStrById(543), pe32.szExeFile);
				}
				else
				{
					// "Terminate process %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(544), pe32.szExeFile, GetLastError());
				}

				bResult = TRUE;
				break;
			}
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	else
	{
		// "Process32First fails, error %d\n"
		EasyLog::Format(GetLibStrById(545), GetLastError());
    }
	
	CloseHandle (hProcessSnap);
	
	return bResult;
}

BOOL MyTerminateProcess2(LPCSTR lpNamePrefix)
{
	if(!lpNamePrefix)
	{
		return FALSE;
	}

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};
	BOOL bResult = FALSE;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		// "CreateToolhelp32Snapshot fails, error %d\n"
		EasyLog::Format(GetLibStrById(546), GetLastError());
		return (FALSE);
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if(!strncmp(_strupr(pe32.szExeFile),_strupr((char *)lpNamePrefix), strlen(lpNamePrefix)))
			{
				HANDLE hProcess =
					OpenProcess(
						PROCESS_QUERY_INFORMATION,
						FALSE,
						pe32.th32ProcessID);
				BOOL rc = TerminateProcess(hProcess, 0);
				if (rc)
				{
					// "Terminate process %s ok\n"
					EasyLog::Format(GetLibStrById(547), pe32.szExeFile);
				}
				else
				{
					// "Terminate process %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(548), pe32.szExeFile, GetLastError());
				}

				bResult = TRUE;
			}
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	else
	{
		// "Process32First fails, error %d\n"
		EasyLog::Format(GetLibStrById(549), GetLastError());
    }
	
	CloseHandle (hProcessSnap);
	
	return bResult;
}

// Only valid when:
// Two parameter, delimiter is "," AND
// first parameter is string, 6 char AND
// second parameter is int, AND and 0x80 is true
BOOL IsValidCommandLine(const char* szCommandLine, std::string& szHost)
{
	std::vector<std::string> arguments;
	BOOL rc = GetArguments(szCommandLine, ',', arguments);
	if (rc && arguments.size() == 3 && arguments.at(0).size() == 6)
	{
		int i = 0;
		int rc = sscanf(arguments.at(1).c_str(), "%d", &i);
		if (rc == 1 && (i & 0x80))
		{
			// Get Host Name
			szHost = arguments.at(2);

			return TRUE;
		}
	}

	return FALSE;
}

VOID GetProtectionCode(std::string& szProtectionCode, const char* szHost)
{
	// "fuchen,128,"
	szProtectionCode = GetLibStrById(559);
	szProtectionCode += szHost;
}