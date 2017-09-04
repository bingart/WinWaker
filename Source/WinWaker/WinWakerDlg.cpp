
// WinWakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWaker.h"
#include "WinWakerDlg.h"
#include "afxdialogex.h"
#include "AboutDlg.h"
#include "ConfigDlg.h"
#include "WorkConfig.h"
#include "WorkLog.h"
#include "WinWakerDragDrop.h"
#include <TlHelp32.h>
#include "WinWakerTask.h"
#include "Interface.h"
#include "TipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinWakerDlg dialog

extern BOOL bRunning;
extern HANDLE ghSvcStopEvent;
extern PtrChangeWindowMessageFilterEx ptrChangeWindowMessageFilterEx;

CWinWakerDlg::CWinWakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinWakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MOUSE_ACTIVE);

	m_bFirst = true;
	m_iFirst = 0;
	m_bExit = false;
	m_iSendPos = 0;
	m_iSendCount = 0;
	m_iTipTime = 0;
	m_iCursorX = 0;
	m_iCursorY = 0;

	m_ConfigDlg = NULL;
	m_AboutDlg = NULL;
	m_DaemonDlg = NULL;
	m_TipDlg = NULL;
}

void CWinWakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MOUSE, m_ButtonMouse);
}

BEGIN_MESSAGE_MAP(CWinWakerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_ACTIVE, &CWinWakerDlg::OnMenuActive)
	ON_COMMAND(ID_MENU_OPEN, &CWinWakerDlg::OnMenuOpen)
	ON_COMMAND(ID_MENU_HIDE, &CWinWakerDlg::OnMenuHide)
	ON_COMMAND(ID_MENU_ABOUT, &CWinWakerDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_EXIT, &CWinWakerDlg::OnMenuExit)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MOUSE_NOTIFY, OnMouseNotify)
	ON_MESSAGE(WM_CONFIG_NOTIFY, OnConfigNotify)
	ON_MESSAGE(WM_ABOUT_NOTIFY, OnAboutNotify)
	ON_MESSAGE(WM_TIP_NOTIFY, OnTipNotify)
	ON_MESSAGE(WM_SHUTDOWN, OnShutdown)
	ON_MESSAGE(WM_REOPEN, OnReopen)
	ON_BN_CLICKED(IDC_BUTTON_MOUSE, &CWinWakerDlg::OnBnClickedButtonMouse)
	ON_COMMAND(ID_MENU_CONFIG, &CWinWakerDlg::OnMenuConfig)
	ON_WM_CONTEXTMENU()
	ON_WM_DROPFILES()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CWinWakerDlg message handlers

BOOL CWinWakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Add extra initialization here
	if (!m_TrayIcon.Create(
		                NULL,                          // Let icon deal with its own messages
                        WM_MOUSE_NOTIFY,               // Icon notify message to use
                        MOUSE_TITLE,                   // tooltip
                        m_hIcon,
                        IDR_MENU1,                     // ID of tray icon
                        FALSE,
                        _T(MOUSE_TITLE),               // balloon tip
                        _T(""),                        // balloon title
                        NIIF_WARNING,                  // balloon icon
                        20 ))                          // balloon timeout
    {
		return FALSE;
    }
	m_TrayIcon.SetMenuDefaultItem(0, TRUE);
	
	// GUI init
	m_ButtonMouse.SetFlat();
	UpdateTip();

	// Timer
	SetTimer(0, 1000, NULL);

	// Enable drag and drop
	EnableDragDrop(this);

	// Start deamon dialog for shutdown
	m_DaemonDlg = new CDaemonDlg(this);
	m_DaemonDlg->Create(CDaemonDlg::IDD, NULL);
	m_DaemonDlg->ShowWindow(SW_SHOW);

	// Start AdMouseTask thread
	WinWakerTask();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinWakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinWakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWinWakerDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu mnuPopupMain;

	mnuPopupMain.LoadMenu(IDR_MENU1);

	CMenu *mnuPopupMenu = mnuPopupMain.GetSubMenu(0);
	ASSERT(mnuPopupMenu);
	
	mnuPopupMenu->EnableMenuItem(ID_MENU_ACTIVE, MF_BYCOMMAND | MF_ENABLED);
	mnuPopupMenu->EnableMenuItem(ID_MENU_OPEN, MF_BYCOMMAND | MF_ENABLED);
	mnuPopupMenu->EnableMenuItem(ID_MENU_ABOUT, MF_BYCOMMAND | MF_ENABLED);
	mnuPopupMenu->EnableMenuItem(ID_MENU_EXIT, MF_BYCOMMAND | MF_ENABLED);

	if (CWorkConfig::Instance()->GetWorkingMode())
	{
		// current mode is waking
		mnuPopupMenu->ModifyMenu(0, MF_BYPOSITION, ID_MENU_ACTIVE, MENU_DEACTIVATING_TITLE);
	}
	else
	{
		// current mode is sleeping
		mnuPopupMenu->ModifyMenu(0, MF_BYPOSITION, ID_MENU_ACTIVE, MENU_ACTIVATING_TITLE);
	}

	mnuPopupMenu->ModifyMenu(2, MF_BYPOSITION, ID_MENU_HIDE, "Hide"); 

	mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CWinWakerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// Hide main window after 2 seconds
	if (m_bFirst)
	{
		m_iFirst++;
		if (m_iFirst >= 2)
		{
			m_bFirst = false;

			PostMessage(WM_CLOSE);
		}
	}

	// Waking up Windows
	if (CWorkConfig::Instance()->GetWorkingMode())
	{
		// Check if cursor move
		POINT point;
		GetCursorPos(&point);
		if (m_iCursorX == point.x && m_iCursorY == point.y)
		{
			// Cursor NOT moving
			m_iSendCount++;
		}
		else
		{
			// Save current cursor position
			m_iCursorX = point.x;
			m_iCursorY = point.y;

			// Cursor moving
			m_iSendCount = 0;
		}

		if (m_iSendCount >= CWorkConfig::Instance()->GetInterTime())
		{
			// Reset
			m_iSendCount = 0;

			// Check filter application
			if (IsFilterApplication())
			{
				SendMouseEvent();
			}
		}
	}

	// Recover status information from tip information
	if (m_iTipTime > 0)
	{
		m_iTipTime--;
		if (m_iTipTime <= 0)
		{
			UpdateTip();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CWinWakerDlg::SendMouseEvent()
{
	m_iSendPos++;

	int dx;
	int dy;
	int delta = CWorkConfig::Instance()->GetStep();
	if (m_iSendPos % 2 == 0)
	{
		dx = delta;
		dy = delta;
	}
	else
	{
		dx = (-1) * delta;
		dy = (-1) * delta;
	}

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.mouseData = 0;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.time = 0;
	input.mi.dwExtraInfo = GetMessageExtraInfo();

	SendInput(
		1,
		&input,
		sizeof INPUT);
}


void CWinWakerDlg::Cleanup()
{
	WorkLog::Format("clean up\n");
	if (m_ConfigDlg)
	{
		m_ConfigDlg->DestroyWindow();
		delete m_ConfigDlg;
		m_ConfigDlg = NULL;
		WorkLog::Format("exit, close config dlg\n");
	}

	if (m_AboutDlg)
	{
		m_AboutDlg->DestroyWindow();
		delete m_AboutDlg;
		m_AboutDlg = NULL;
		WorkLog::Format("exit, close about dlg\n");
	}

	if (m_TipDlg)
	{
		m_TipDlg->DestroyWindow();
		delete m_TipDlg;
		m_TipDlg = NULL;
		WorkLog::Format("exit, close tip dlg\n");
	}

	if (m_DaemonDlg)
	{
		m_DaemonDlg->DestroyWindow();
		delete m_DaemonDlg;
		m_DaemonDlg = NULL;
		WorkLog::Format("exit, close deamon dlg\n");
	}
}


void CWinWakerDlg::UpdateTip()
{
	if (CWorkConfig::Instance()->GetWorkingMode())
	{
		m_ButtonMouse.SetIcon(IDI_MOUSE_ACTIVE);
		if (m_iTipTime <= 0)
		{
			if (CWorkConfig::Instance()->GetFilterApplication())
			{
				m_ButtonMouse.SetWindowText(ACTIVATING_TITLE_APPLICATION_FILTERING);
			}
			else
			{
				m_ButtonMouse.SetWindowText(ACTIVATING_TITLE_NO_CONDITION);
			}
		}
		m_ButtonMouse.SetTooltipText(ACTIVATING_TIP);
		m_TrayIcon.SetTooltipText(ACTIVATING_TIP2);
		m_TrayIcon.SetIcon(IDI_MOUSE_ACTIVE);
	}
	else
	{
		m_ButtonMouse.SetIcon(IDI_MOUSE_DEACTIVE);
		if (m_iTipTime <= 0)
		{
			m_ButtonMouse.SetWindowText(DEACTIVATING_TITLE);
		}
		m_ButtonMouse.SetTooltipText(DEACTIVATING_TIP);
		m_TrayIcon.SetTooltipText(DEACTIVATING_TIP2);
		m_TrayIcon.SetIcon(IDI_MOUSE_DEACTIVE);
	}


}


LRESULT CWinWakerDlg::OnMouseNotify(WPARAM wParam, LPARAM lParam)
{
	// Revert working mode
	if (CWorkConfig::Instance()->GetWorkingMode())
	{
		CWorkConfig::Instance()->SetWorkingMode(FALSE);
	}
	else
	{
		CWorkConfig::Instance()->SetWorkingMode(TRUE);
	}

	// Save
	CWorkConfig::Instance()->Save();

	// Update GUI
	UpdateTip();

	// Reset tip
	m_iTipTime = 0;

	return 1;
}


LRESULT CWinWakerDlg::OnConfigNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_ConfigDlg)
	{
		delete m_ConfigDlg;
		m_ConfigDlg = NULL;
	}

	// Update GUI, for Application Filtering
	UpdateTip();

	return 1;
}


LRESULT CWinWakerDlg::OnAboutNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_AboutDlg)
	{
		delete m_AboutDlg;
		m_AboutDlg = NULL;
	}

	return 1;
}


LRESULT CWinWakerDlg::OnTipNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_TipDlg)
	{
		delete m_TipDlg;
		m_TipDlg = NULL;
	}

	return 1;
}


LRESULT CWinWakerDlg::OnShutdown(WPARAM wParam, LPARAM lParam)
{
	OnMenuExit();

	WorkLog::Format("CWinWakerDlg::OnShutdown\n");

	return 1;
}


LRESULT CWinWakerDlg::OnReopen(WPARAM wParam, LPARAM lParam)
{
	OnMenuOpen();

	WorkLog::Format("CWinWakerDlg::OnReopen\n");

	return 1;
}


void CWinWakerDlg::OnMenuActive()
{
	PostMessage(WM_MOUSE_NOTIFY);
}


void CWinWakerDlg::OnMenuOpen()
{
	CSystemTray::MaximiseFromTray(this);
}


void CWinWakerDlg::OnMenuHide()
{
	CSystemTray::MinimiseToTray(this);
}


void CWinWakerDlg::OnMenuConfig()
{
	if (!m_ConfigDlg)
	{
		m_ConfigDlg = new CConfigDlg(this);
		m_ConfigDlg->Create(CConfigDlg::IDD, NULL);
	}

	if (m_ConfigDlg)
	{
		m_ConfigDlg->ShowWindow(SW_SHOW);
		m_ConfigDlg->FlashWindow(TRUE);
		m_ConfigDlg->SetFocus();
		WorkLog::Format("config dlg opened\n");
	}
}


void CWinWakerDlg::OnMenuAbout()
{
	if (!m_AboutDlg)
	{
		m_AboutDlg = new CAboutDlg(this);
		m_AboutDlg->Create(CAboutDlg::IDD, NULL);
	}

	if (m_AboutDlg)
	{
		m_AboutDlg->ShowWindow(SW_SHOW);
		m_AboutDlg->FlashWindow(TRUE);
		m_AboutDlg->SetFocus();
		WorkLog::Format("about dlg opened\n");
	}
}


void CWinWakerDlg::OnMenuExit()
{
	m_bExit = true;
	PostMessage(WM_CLOSE);
}


void CWinWakerDlg::OnClose()
{
	if (!m_bExit)
	{
		CSystemTray::MinimiseToTray(this);
	}
	else
	{
		// Clean all child window
		Cleanup();

		// Notify exit
		bRunning = FALSE;
		SetEvent(ghSvcStopEvent);

		CDialogEx::OnClose();
	}
}


void CWinWakerDlg::OnBnClickedButtonMouse()
{
	PostMessage(WM_MOUSE_NOTIFY);
}


void CWinWakerDlg::OnDropFiles(HDROP hDropInfo)
{
	if (m_TipDlg == NULL)
	{
		char szDroppedFile[MAX_PATH] = {0};	// drop file path
		UINT rc = DragQueryFile(
			hDropInfo,		// Struture Identifier
			0,				// -1 to Drop more than one file or ( integer 0 to max ) to drop selected No of files
			szDroppedFile,	// Droped File Name
			MAX_PATH		// Max char
			);
		if (rc)
		{
			std::string szDroppedPath = szDroppedFile;

			std::string szExeFileName;
			BOOL rc = GetExeFileNameFromDropFiles(szDroppedPath, szExeFileName);
			if (rc)
			{
				if (CWorkConfig::Instance()->AddApplication(szExeFileName))
				{
					// Add application ok
					CWorkConfig::Instance()->SetFilterApplication(TRUE);
					CWorkConfig::Instance()->Save();
					WorkLog::Format("Add application %s and set filter application to true\n", szExeFileName.c_str());

					// Tip
					/*
					POINT point;
					rc = DragQueryPoint(
						hDropInfo,
						&point);
					RECT rect;
					GetWindowRect(&rect);
					int x = rect.left + point.x;
					int y = rect.top + point.y;
					m_TipDlg = new CTipDlg(x, y, 4, szExeFileName.c_str(), this);
					m_TipDlg->Create(CTipDlg::IDD, NULL);
					m_TipDlg->ShowWindow(SW_SHOW);
					*/
					std::string szTip = "The application " + szExeFileName + " is added";
					m_iTipTime = 2;
					m_ButtonMouse.SetWindowText(szTip.c_str());
				}
				else
				{
					// Add application fails
				}
			}
		}
	}
	else
	{

	}

	__super::OnDropFiles(hDropInfo);
}

BOOL CWinWakerDlg::IsFilterApplication()
{
	BOOL bFilterApplication = CWorkConfig::Instance()->GetFilterApplication();

	if (bFilterApplication)
	{
		// Get filtered application list
		std::list<std::string> applicationList;
		CWorkConfig::Instance()->GetApplicationList(applicationList);
		if (applicationList.size() > 0)
		{
			// Get process list
			std::list<std::string> processList;
			GetProcessList(processList);

			BOOL bFound = FALSE;
			for (std::list<std::string>::iterator it = processList.begin(); it != processList.end(); it++)
			{
				for (std::list<std::string>::iterator it2 = applicationList.begin(); it2 != applicationList.end(); it2++)
				{
					if (it->size() > 0 && it2->size() > 0)
					{
						CString first = it->c_str();
						CString second = it2->c_str();
						if (first.CompareNoCase(second) == 0)
						{
							bFound = TRUE;
							WorkLog::Format("application found, process %s, application %s\n", it->c_str(), it2->c_str());
							break;
						}
					}
				}

				if (bFound)
				{
					break;
				}
			}

			return bFound;
		}
		else
		{
			// Disable filter application when application list is empty
			return TRUE;
		}
	}
	else
	{
		// No filter
		return TRUE;
	}
}

BOOL CWinWakerDlg::GetProcessList(std::list<std::string>& list)
{
	list.clear();

	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		WorkLog::Format("CreateToolhelp32Snapshot fails, error %d\n", GetLastError());
		return FALSE;
	}
	else
	{
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcessSnap, &pe32))
		{
			do
			{
				std::string s = pe32.szExeFile;
				list.push_back(s);
			}
			while (Process32Next(hProcessSnap, &pe32));
		}
		else
		{
			WorkLog::Format("Process32First fails, error %d\n", GetLastError());
		}
	
		CloseHandle (hProcessSnap);

		return TRUE;
	}
}
