// AdMouseUpdate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <list>
#include "easylog.hpp"
#include "reg.h"
#include "file.h"
#include "libstr.h"
#include "scrambler.h"
#include "WinWakerThread.h"

#pragma comment(lib, "advapi32.lib")

BOOL bRunning = TRUE;

////
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void __cdecl _tmain(int argc, TCHAR *argv[]) 
{
	// Get temp path
	GetUserTempPath();
	// Get exe file path
	GetExeFilePath();
	// Get time string
	GetTimeString();

	// Init log
	// "WinWakerUpdate"
	std::string szHost = GetStrById(120);
	// "Log.log"
	std::string szLogFilePath = userTempPath + "\\" + szHost + "." + timeString + ".log";
	EasyLog::SetSingleMode(szLogFilePath.c_str());

	// "#### start\n"
	EasyLog::Format("%s", GetStrById(206));

	// Run
	WinWakerThreadFunction(NULL);
}
