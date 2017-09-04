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
BOOL gbAutoDelete = TRUE;
FILE* gpLogFile = NULL;

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
	GetWindowsTempPath();

	// Init log
	// "WinWakerUpdate"
	std::string szHost = GetStrById(120);
	// "Log.log"
	std::string szLogFilePath = windowsTempPath + "\\" + szHost + GetStrById(205);
	gpLogFile = fopen(szLogFilePath.c_str(), "a");

	// "#### start\n"
	fprintf(gpLogFile, GetStrById(206));


}
