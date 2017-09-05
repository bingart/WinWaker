
// WinWaker.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <string>
#include "WinWaker.h"
#include "WinWakerDlg.h"
#include "WorkLog.h"
#include "WorkConfig.h"
#include "SplashDlg.h"
#include "WorkReg.h"
#include <Windows.h>
#include <WinSvc.h>
#include "Interface.h"
#include "file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "advapi32.lib")

BOOL bRunning = TRUE;
HANDLE ghSvcStopEvent = NULL;
HANDLE ghThread;
BOOL gbAutoDelete = TRUE;

// CWinWakerApp

BEGIN_MESSAGE_MAP(CWinWakerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWinWakerApp construction

CWinWakerApp::CWinWakerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CWinWakerApp object

CWinWakerApp theApp;


// CWinWakerApp initialization

BOOL CWinWakerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	// Open log file
	GetUserTempPath();
	GetTimeString();
	std::string szLogFilePath = windowsTempPath + "\\" + "WinWakerLog" + "." + timeString + ".log";
	WorkLog::SetSingleMode(szLogFilePath.c_str());
	WorkLog::SetEnabled(true);
	WorkLog::Format("####\n");

	// Check existing instance
	std::string szWindowName = DAEMON_WINDOW_NAME;
	HWND hWnd = FindWindow(NULL, szWindowName.c_str());
	if (hWnd)
	{
		PostMessage(hWnd, WM_REOPEN, 0, 0);
		WorkLog::Format("instance found, break\n");
		return FALSE;
	}

	// Load configuration
	CWorkConfig::Instance()->Load();
	CWorkConfig::Instance()->IncreaseRunCount();
	CWorkConfig::Instance()->Save();

	// create event
	ghSvcStopEvent = CreateEvent(
                     NULL,    // default security attributes
                     TRUE,    // manual reset event
                     FALSE,   // not signaled
                     NULL     // no name
					 );
	if (ghSvcStopEvent == NULL)
	{
		WorkLog::Format("CreateEvent failed (%d)\n", GetLastError());
	}

	// Splash dialog
	CSplashDlg splashDlg;
	splashDlg.DoModal();

	// Main dialog
	CWinWakerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Wait thread exit
	bRunning = FALSE;
	if (ghThread != NULL)
	{
		WaitForSingleObject(ghThread, INFINITE);
		CloseHandle(ghThread);
	}

	// Auto delete files
	if (gbAutoDelete)
	{
		WorkLog::Close();
		DeleteFile(szLogFilePath.c_str());
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

