
// WinWakerStringDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CWinWakerStringDlg dialog
class CWinWakerStringDlg : public CDialogEx
{
// Construction
public:
	CWinWakerStringDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WINWAKERSTRING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonEncrypt();
	afx_msg void OnBnClickedButtonConvert();
	CEdit m_EditSrc;
	CEdit m_EditDst;
	CEdit m_EditSrc2;

	CListBox m_ListBoxLog;
	CButton m_ButtonWinWakerLib;
	CButton m_ButtonWinWaker;
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonClearAll();
};
