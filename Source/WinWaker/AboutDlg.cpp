// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWaker.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "BtnST.h"
#include <string>
#include "Interface.h"

// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MOUSE_ACTIVE);
	m_pParent = pParent;
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ABOUT, m_ButtonAbout);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CAboutDlg::OnBnClickedButtonAbout)
END_MESSAGE_MAP()


// CAboutDlg message handlers


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Add extra initialization here
	m_ButtonAbout.SetFlat();
	m_ButtonAbout.SetIcon(IDI_MOUSE_ACTIVE);
	std::string title;
	title  = "Windows Waker Version 1.0.5    \n";
	title += "Copyright 2015 www.winwaker.org\n";
	title += "All Right Reservered           ";
	m_ButtonAbout.SetWindowText(title.c_str());
	m_ButtonAbout.SetTooltipText("Click to close");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAboutDlg::OnBnClickedButtonAbout()
{
	if (m_pParent)
	{
		m_pParent->PostMessage(WM_ABOUT_NOTIFY);
	}

	OnOK();
}
