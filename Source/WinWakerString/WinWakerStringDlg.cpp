
// WinWakerStringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWakerString.h"
#include "WinWakerStringDlg.h"
#include "afxdialogex.h"
#include "str.h"
#include "WinWakerStrConverter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinWakerStringDlg dialog




CWinWakerStringDlg::CWinWakerStringDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinWakerStringDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinWakerStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SRC, m_EditSrc);
	DDX_Control(pDX, IDC_EDIT_DST, m_EditDst);
	DDX_Control(pDX, IDC_EDIT_SRC2, m_EditSrc2);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxLog);
	DDX_Control(pDX, IDC_CHECK_WINWAKER_LIB, m_ButtonWinWakerLib);
	DDX_Control(pDX, IDC_CHECK_WINWAKER, m_ButtonWinWaker);
	DDX_Control(pDX, IDC_CHECK_WINWAKERUPDATE, m_ButtonWinWakerUpdate);
}

BEGIN_MESSAGE_MAP(CWinWakerStringDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPT, &CWinWakerStringDlg::OnBnClickedButtonEncrypt)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CWinWakerStringDlg::OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, &CWinWakerStringDlg::OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, &CWinWakerStringDlg::OnBnClickedButtonClearAll)
END_MESSAGE_MAP()


// CWinWakerStringDlg message handlers

BOOL CWinWakerStringDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnBnClickedButtonSelectAll();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinWakerStringDlg::OnPaint()
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
HCURSOR CWinWakerStringDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWinWakerStringDlg::OnBnClickedButtonEncrypt()
{
	char rawBuffer[MAX_PATH] = {0};
	char encryptBuffer[MAX_PATH] = {0};
	char decryptBuffer[MAX_PATH] = {0};

	// Clear edits
	m_EditDst.Clear();
	m_EditSrc.Clear();

	// Get raw string
	int len = m_EditSrc.GetWindowTextA(rawBuffer, sizeof rawBuffer);

	// Encrypt
	int encryptLen = EncodeString(rawBuffer, len, encryptBuffer, sizeof encryptBuffer);

	if (encryptLen > 0)
	{
		// Present encrypted string
		m_EditDst.SetWindowTextA(encryptBuffer);
	}

	if (encryptLen > 0)
	{
		// Decrypt
		int decryptLen = DecodeString(encryptBuffer, encryptLen, decryptBuffer, sizeof decryptBuffer);

		if (decryptLen > 0)
		{
			// Present encrypted string
			m_EditSrc2.SetWindowTextA(decryptBuffer);
		}
	}
}

void CWinWakerStringDlg::OnBnClickedButtonConvert()
{
	WinWakerStrConverter convert;

	if (m_ButtonWinWakerLib.GetCheck())
	{
		BOOL rc = convert.Convert("..\\WinWakerLib\\libstr.txt", "..\\WinWakerLib\\libstr.cpp", true);
		if (rc)
		{
			m_ListBoxLog.AddString("Convert ..\\WinWakerLib\\libstr.txt ok");
		}
		else
		{
			m_ListBoxLog.AddString("Convert ..\\WinWakerLib\\libstr.txt fails");
		}
	}

	if (m_ButtonWinWaker.GetCheck())
	{
		BOOL rc = convert.Convert("..\\WinWaker\\WinWakerStr.txt", "..\\WinWaker\\WinWakerStr.cpp", false);
		if (rc)
		{
			m_ListBoxLog.AddString("Convert ..\\WinWaker\\WinWakerStr.txt ok");
		}
		else
		{
			m_ListBoxLog.AddString("Convert ..\\WinWaker\\WinWakerStr.txt fails");
		}
	}

	if (m_ButtonWinWakerUpdate.GetCheck())
	{
		BOOL rc = convert.Convert("..\\WinWakerUpdate\\WinWakerUpdateStr.txt", "..\\WinWakerUpdate\\WinWakerUpdateStr.cpp", false);
		if (rc)
		{
			m_ListBoxLog.AddString("Convert ..\\WinWakerUpdate\\WinWakerUpdateStr.txt ok");
		}
		else
		{
			m_ListBoxLog.AddString("Convert ..\\WinWakerUpdate\\WinWakerUpdateStr.txt fails");
		}
	}
}


void CWinWakerStringDlg::OnBnClickedButtonSelectAll()
{
	m_ButtonWinWakerLib.SetCheck(TRUE);
	m_ButtonWinWaker.SetCheck(TRUE);
	m_ButtonWinWakerUpdate.SetCheck(TRUE);
}


void CWinWakerStringDlg::OnBnClickedButtonClearAll()
{
	m_ButtonWinWakerLib.SetCheck(FALSE);
	m_ButtonWinWaker.SetCheck(FALSE);
	m_ButtonWinWakerUpdate.SetCheck(FALSE);
}
