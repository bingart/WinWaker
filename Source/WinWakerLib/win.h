#ifndef WIN_H
#define WIN_H

#include <Windows.h>
#include <list>

HANDLE CreateProcess2(LPCSTR lpImage);
BOOL WaitProcess(HANDLE hProcess, int nWaitTime);

// Process ID is not good condition to search window, due to multiple process browser.
HWND FindWindowByProcessHandleAndClassName(HANDLE hProcess, LPCSTR lpClassName);
HWND FindWindowByTitleAndClassName(LPCSTR lpTitle, LPCSTR lpClassName);

VOID GetAllChildWindowHandle(HWND hParentWnd, std::list<HWND>* list);
HWND GetChildWindowHandleByClassName(HWND hParentWnd, const char* szChildClassName);

VOID SendMOS(HWND hWnd, int x, int y);
VOID SendMOSToDescendants(HWND hParentWnd, int x, int y);

VOID SendMessageToDescendants(HWND hParentWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif