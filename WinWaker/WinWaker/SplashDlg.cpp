// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWaker.h"
#include "SplashDlg.h"
#include "afxdialogex.h"


// CSplashDlg dialog

IMPLEMENT_DYNAMIC(CSplashDlg, CDialogEx)

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplashDlg::IDD, pParent)
{
	m_iCount = 0;
}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GIF, m_PictureGIF);
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplashDlg message handlers


BOOL CSplashDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetBackgroundColor(RGB(212, 208, 200));
	SetBackgroundImage(IDB_BITMAP1);

	RECT rect;
	GetClientRect(&rect);
	int y = (rect.bottom - 87)/2;
	int x = y;
	m_PictureGIF.SetWindowPos(NULL, x, y, 99, 87, SW_SHOW);
	if (m_PictureGIF.Load(MAKEINTRESOURCE(IDR_GIF1), "GIF"))
	{
		// The background color must be set after picture loaded !!!
		// Otherwise it will be overwritten by System Color
		m_PictureGIF.SetBkColor(RGB(212, 208, 200));
		m_PictureGIF.Draw();
	}
	
	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSplashDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_iCount++;
	if (m_iCount >= 3)
	{
		PostMessage(WM_CLOSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}
