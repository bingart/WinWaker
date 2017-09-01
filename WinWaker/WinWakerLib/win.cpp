
#include "win.h"
#include "easylog.hpp"
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <map>
#include "libstr.h"

#pragma comment(lib, "User32.lib")

extern BOOL bRunning;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

HANDLE CreateProcess2(LPCSTR lpImage)
{
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
		(LPSTR) lpImage, // Command line. 
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

		return hProcess;
	}
	else
	{
		return NULL;
	}
}

BOOL WaitProcess(HANDLE hProcess, int nWaitTime)
{
	int count = 0;
	BOOL bExit = FALSE;
	while (bRunning && count < nWaitTime)
	{
		DWORD dwResult = WaitForSingleObject(hProcess, 1000);
		if (dwResult == WAIT_OBJECT_0)
		{
			// "wait process 0x%x to exit, get signal\n"
			EasyLog::Format(GetLibStrById(554), hProcess);
			bExit = TRUE;
			break;
		}
		else if (dwResult == WAIT_TIMEOUT)
		{
			// "wait process 0x%x to exit, time out %d\n"
			EasyLog::Format(GetLibStrById(555), hProcess, count);
		}
		else
		{
			// "wait process 0x%x to exit, failed, break\n"
			EasyLog::Format(GetLibStrById(556), hProcess);
			break;
		}

		count++;
	}

	if (!bExit)
	{
		// terminate process
		::TerminateProcess(hProcess, 0);
	}

	if (hProcess != NULL)
	{
		CloseHandle(hProcess);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

BOOL WINAPI GetPidMap(std::map<DWORD, DWORD>& pidMap)
{
	PROCESSENTRY32 procEntry;

	pidMap.clear();

	// Get a handle to a Toolhelp snapshot of the systems processes.
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		return FALSE ;
	}

	// Get the first process' information.
	memset((LPVOID)&procEntry, 0 ,sizeof(PROCESSENTRY32));
	procEntry.dwSize = sizeof(PROCESSENTRY32) ;
	
	if (Process32First(hSnapShot, &procEntry))
	{
		do
		{
			DWORD pid = procEntry.th32ProcessID;
			DWORD ppid =  procEntry.th32ParentProcessID;
		
			pidMap[pid] = ppid;

			memset((LPVOID)&procEntry, 0 ,sizeof(PROCESSENTRY32));
			procEntry.dwSize = sizeof(PROCESSENTRY32);
					;
		} while (Process32Next(hSnapShot, &procEntry));
	}

	return TRUE;
}

// Structure used to communicate data from and to enumeration procedure
struct EnumData
{
    DWORD dwProcessId;
	std::string szWndClassName;
	std::map<DWORD, DWORD> pidMap;
    HWND hWnd;
};

// Application-defined callback for EnumWindows
BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
    // Retrieve storage location for communication data
    EnumData& ed = *(EnumData*)lParam;
    DWORD dwProcessId = 0x0;
    DWORD dwParentProcessId = 0x0;
    
	char buffer[MAX_PATH] = {0};
	std::string className = "";
	memset(buffer, 0, sizeof buffer);
	int nNumber = GetClassName(hWnd, buffer, sizeof buffer);
	if (nNumber > 0)
	{
		className = buffer;
		if (className == ed.szWndClassName)
		{
			memset(buffer, 0, sizeof buffer);
		}
	}

	// Apply filter - if you want to implement additional restrictions,
    // this is the place to do so.
	if (className == ed.szWndClassName)
	{
		// Query process ID for hWnd
		GetWindowThreadProcessId(hWnd, &dwProcessId);
    
		// Query parent process id
		if (ed.pidMap.find(dwProcessId) != ed.pidMap.end())
		{
			dwParentProcessId = ed.pidMap[dwProcessId];
		}

		if (dwParentProcessId == ed.dwProcessId || dwProcessId == ed.dwProcessId)
		{
			// Found a window matching the process ID
			ed.hWnd = hWnd;
        
			// Report success
			SetLastError(ERROR_SUCCESS);

			// Stop enumeration
			return FALSE;
		}
		else
		{
			return TRUE;
		}
    }
	else
	{
		// Continue enumeration
		return TRUE;
	}
}

// Main entry
HWND FindWindowByProcessIdAndClassName(DWORD dwProcessId, LPCSTR lpClassName)
{
    EnumData ed;
	ed.dwProcessId = dwProcessId;
	ed.szWndClassName = lpClassName;
	GetPidMap(ed.pidMap);
	ed.hWnd = NULL;

    if (!EnumWindows( EnumProc, (LPARAM)&ed) &&
         (GetLastError() == ERROR_SUCCESS))
	{
        return ed.hWnd;
    }
	else
	{
	    return NULL;
	}
}

// Helper method for convenience
HWND FindWindowByProcessHandleAndClassName(HANDLE hProcess, LPCSTR lpClassName)
{
    return FindWindowByProcessIdAndClassName(GetProcessId(hProcess), lpClassName);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// Structure used to communicate data from and to enumeration procedure
struct EnumData2
{
    std::string szWndTitle;
	std::string szWndClassName;
    HWND hWnd;
};

// Application-defined callback for EnumWindows
BOOL CALLBACK EnumProc2( HWND hWnd, LPARAM lParam )
{
    // Retrieve storage location for communication data
    EnumData2& ed = *(EnumData2*)lParam;
	char buffer[MAX_PATH] = {0};
	memset(buffer, 0, sizeof buffer);

	// Query title for hWnd
    GetWindowText(hWnd, buffer, sizeof buffer);

	// Apply filter - if you want to implement additional restrictions,
    // this is the place to do so.
	std::string title = buffer;
	// EasyLog::Format("enum window title %s\n", title.c_str());
	std::string::size_type s = title.find(ed.szWndTitle);

	bool isClassNameMatched = true;
	if (ed.szWndClassName.size() > 0)
	{
		memset(buffer, 0, sizeof buffer);
		int nNumber = GetClassName(hWnd, buffer, sizeof buffer);
		if (nNumber > 0)
		{
			std::string className = buffer;
			isClassNameMatched = (className == ed.szWndClassName);
		}
	}

	if (s != std::string::npos && isClassNameMatched)
	{
        // Found a window matching the process ID
        ed.hWnd = hWnd;
        
		// Report success
        SetLastError( ERROR_SUCCESS );

		// Stop enumeration
        return FALSE;
    }
	else
	{
		// Continue enumeration
		return TRUE;
	}
}

HWND FindWindowByTitleAndClassName(LPCSTR lpTitle, LPCSTR lpClassName)
{
    EnumData2 ed;
	ed.szWndTitle = lpTitle;
	ed.szWndClassName = lpClassName;
	ed.hWnd = NULL;

    if ( !EnumWindows( EnumProc2, (LPARAM)&ed ) &&
         ( GetLastError() == ERROR_SUCCESS ) )
	{
        return ed.hWnd;
    }
	else
	{
	    return NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	std::list<HWND>* lpList = (std::list<HWND> *) lParam;
	lpList->push_back(hwnd);

	// Continue to search
	return TRUE;
}

VOID GetAllChildWindowHandle(HWND hParentWnd, std::list<HWND>* list)
{
	list->clear();

	EnumChildWindows(hParentWnd, EnumChildProc, (LPARAM)list);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// Structure used to communicate data from and to enumeration procedure
struct EnumData3
{
	std::string szWndClassName;
    HWND hWnd;
};

BOOL CALLBACK EnumChildProc2(HWND hWnd, LPARAM lParam)
{
    // Retrieve storage location for communication data
    EnumData3& ed = *(EnumData3 *)lParam;

	bool isClassNameMatched = false;
	if (ed.szWndClassName.size() > 0)
	{
		char buffer[MAX_PATH] = {0};
		memset(buffer, 0, sizeof buffer);
		int nNumber = GetClassName(hWnd, buffer, sizeof buffer);
		if (nNumber > 0)
		{
			std::string className = buffer;
			isClassNameMatched = (className == ed.szWndClassName);
		}
	}

	if (isClassNameMatched)
	{
		// Found, save handle and stop enum
		ed.hWnd = hWnd;
		return FALSE;
	}
	else
	{
		// Continue to search
		return TRUE;
	}
}

HWND GetChildWindowHandleByClassName(HWND hParentWnd, const char* szChildClassName)
{
    EnumData3 ed;
	ed.szWndClassName = szChildClassName;
	ed.hWnd = NULL;

    EnumChildWindows(hParentWnd, EnumChildProc2, (LPARAM)&ed);
    return ed.hWnd;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

VOID SendMOS(HWND hWnd, int x, int y)
{
	PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(x, y));
	PostMessage(hWnd, WM_LBUTTONUP, 0L, MAKELONG(x, y));
}

VOID SendMOSToDescendants(HWND hParentWnd, int x, int y)
{
	std::list<HWND> l;
	GetAllChildWindowHandle(hParentWnd, &l);

	SendMOS(hParentWnd, x, y);

	for (std::list<HWND>::iterator it = l.begin(); it != l.end(); it++)
	{
		SendMOS(*it, x, y);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

VOID SendMessageToDescendants(HWND hParentWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	std::list<HWND> l;
	GetAllChildWindowHandle(hParentWnd, &l);

	PostMessage(hParentWnd, Msg, wParam, lParam);

	for (std::list<HWND>::iterator it = l.begin(); it != l.end(); it++)
	{
		PostMessage(*it, Msg, wParam, lParam);
	}
}
