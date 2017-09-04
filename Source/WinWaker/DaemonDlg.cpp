// DaemonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWaker.h"
#include "DaemonDlg.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "WorkLog.h"

// CDaemonDlg dialog

IMPLEMENT_DYNAMIC(CDaemonDlg, CDialogEx)

CDaemonDlg::CDaemonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDaemonDlg::IDD, pParent)
{
	m_pParent = pParent;
}

CDaemonDlg::~CDaemonDlg()
{
}

void CDaemonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDaemonDlg, CDialogEx)
	ON_MESSAGE(WM_SHUTDOWN, OnShutdown)
	ON_MESSAGE(WM_REOPEN, OnReopen)
END_MESSAGE_MAP()


// CDaemonDlg message handlers


BOOL CDaemonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowText(DAEMON_WINDOW_NAME);
	SetWindowPos(NULL, -200, -200, 2, 2, SWP_SHOWWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDaemonDlg::OnShutdown(WPARAM wParam, LPARAM lParam)
{
	WorkLog::Format("CDaemonDlg::OnShutdown\n");

	if (m_pParent)
	{
		m_pParent->PostMessage(WM_SHUTDOWN);
		WorkLog::Format("CDaemonDlg::OnShutdown send WM_SHUTDOWN to parent\n");
	}

	OnOK();

	return 1;
}

LRESULT CDaemonDlg::OnReopen(WPARAM wParam, LPARAM lParam)
{
	WorkLog::Format("CDaemonDlg::OnReopen\n");

	if (m_pParent)
	{
		m_pParent->PostMessage(WM_REOPEN);
		WorkLog::Format("CDaemonDlg::OnReopen send WM_REOPEN to parent\n");
	}

	OnOK();

	return 1;
}