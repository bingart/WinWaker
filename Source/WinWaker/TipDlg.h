#pragma once
#include "btnst.h"
#include <string>
#include "afxwin.h"

// CTipDlg dialog

class CTipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTipDlg)

public:
	CTipDlg(int iX, int iY, int iDelayTime, const char* szExeFileName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTipDlg();

// Dialog Data
	enum { IDD = IDD_TIP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CWnd* m_pParent;
	std::string m_szExeFileName;
	int m_iX;
	int m_iY;
	int m_iDelayTime;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButtonST m_ButtonTip;
};
