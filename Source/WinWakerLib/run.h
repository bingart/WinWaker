
#ifndef PROCESS_H
#define PROCESS_H

#include <Windows.h>
#include <string>

BOOL IsProcessExists(LPSTR lpName);
BOOL GetTokenByName(HANDLE &hToken, LPSTR lpName);
BOOL ExecProcess(LPCSTR lpImage, const char* szArguments = NULL);
BOOL RunProcess(LPCSTR lpImage, int nWaitTime, const char* szArguments = NULL);
BOOL RunProcessAsUser(LPCSTR lpImage, int nWaitTime, const char* szArguments = NULL);

// Terminate process with process name
BOOL MyTerminateProcess(LPCSTR lpName);
// Termimate process with process name prefix
BOOL MyTerminateProcess2(LPSTR lpNamePrefix);

BOOL IsValidCommandLine(const char* szCommandLine, std::string& szHost);

// For any application or service that invoke Update
VOID GetProtectionCode(std::string& szProtectionCode, const char* szHost);

#endif // PROCESS_H