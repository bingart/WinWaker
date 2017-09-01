#pragma once
#include "btnst.h"


// CAboutDlg dialog

class CAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	HICON m_hIcon;
	CWnd* m_pParent;

	CButtonST m_ButtonAbout;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAbout();
};
