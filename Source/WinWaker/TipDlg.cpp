// TipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWaker.h"
#include "TipDlg.h"
#include "afxdialogex.h"
#include "Interface.h"


// CTipDlg dialog

IMPLEMENT_DYNAMIC(CTipDlg, CDialogEx)

CTipDlg::CTipDlg(int iX, int iY, int iDelayTime, const char* szExeFileName, CWnd* pParent /*=NULL*/)
	: CDialogEx(CTipDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_szExeFileName = szExeFileName;
	m_iX = iX;
	m_iY = iY;
	m_iDelayTime = iDelayTime;
}

CTipDlg::~CTipDlg()
{
}

void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TIP, m_ButtonTip);
}


BEGIN_MESSAGE_MAP(CTipDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTipDlg message handlers


BOOL CTipDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	RECT rect;
	GetWindowRect(&rect);
	SetWindowPos(&CWnd::wndTop, m_iX, m_iY, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	
	std::string str = "The application " + m_szExeFileName + " is added";
	m_ButtonTip.SetFlat(FALSE);
	m_ButtonTip.DrawTransparent();
	m_ButtonTip.SetWindowText(str.c_str());
	m_ButtonTip.SetWindowPos(&CWnd::wndTop, 1, 1, rect.right - rect.left - 1, rect.bottom - rect.top - 1, SWP_SHOWWINDOW);

	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTipDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_iDelayTime--;
	if (m_iDelayTime <= 0)
	{
		if (m_pParent)
		{
			m_pParent->PostMessage(WM_TIP_NOTIFY);
		}

		OnOK();
	}

	CDialogEx::OnTimer(nIDEvent);
}
