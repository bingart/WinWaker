#pragma once
#include "btnst.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <string>


// CConfigDlg dialog

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	HICON m_hIcon;
	CWnd* m_pParent;

	CButtonST m_ButtonOk;
	CButtonST m_ButtonCancel;
	CButtonST m_ButtonSetDefault;

	CEdit m_EditStep;
	CSpinButtonCtrl m_SpinStep;
	
	CEdit m_EditInterTime;
	CSpinButtonCtrl m_SpinInterTime;
	
	CButton m_CheckBoxAutoRun;

	CButton m_CheckBoxFilterApplication;
	CListBox m_ListBoxApplication;
	CButtonST m_ButtonAddApplication;
	CButtonST m_ButtonRemoveApplication;
	CButtonST m_ButtonClearApplication;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSetDefault();

	afx_msg void OnBnClickedCheckAutorun();
	afx_msg void OnDeltaposSpinStep(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinInterTime(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedCheckFilterApplication();
	afx_msg void OnLbnSelchangeListApplication();
	afx_msg void OnBnClickedButtonAddApplication();
	afx_msg void OnBnClickedButtonRemoveApplication();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	VOID AddApplicationToListBox(std::string szExeFileName);
};
