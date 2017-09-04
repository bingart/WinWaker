
// WinWakerDlg.h : header file
//

#pragma once

#include "btnst.h"
#include "MySystemTray.h"
#include "WorkConfig.h"
#include "ConfigDlg.h"
#include "AboutDlg.h"
#include "DaemonDlg.h"
#include "TipDlg.h"
#include "Interface.h"

#define MOUSE_TITLE ("Windows Waker")
#define ACTIVATING_TITLE_NO_CONDITION ("Waking up Windows unconditionally")
#define ACTIVATING_TITLE_APPLICATION_FILTERING ("Waking up Windows with application filtering")
#define ACTIVATING_TIP ("Waking up, click to sleep, right click for more information")
#define ACTIVATING_TIP2 ("Waking up, double click to sleep, right click for more information")
#define DEACTIVATING_TITLE ("Sleeping")
#define DEACTIVATING_TIP ("Sleeping, click to wake up, right click for more information")
#define DEACTIVATING_TIP2 ("Sleeping, double click to wake up, right click for more information")

#define MENU_DEACTIVATING_TITLE ("Sleep")
#define MENU_ACTIVATING_TITLE ("Wake up")

// CWinWakerDlg dialog
class CWinWakerDlg : public CDialogEx, public IWorkConfigListener
{
// Construction
public:
	CWinWakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ADMOUSE_DIALOG };

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

	CMySystemTray m_TrayIcon;

	bool m_bFirst;
	int m_iFirst;
	bool m_bExit;
	int m_iSendPos;
	int m_iSendCount;
	int m_iTipTime;
	int m_iCursorX;
	int m_iCursorY;

	CButtonST m_ButtonMouse;

	CConfigDlg* m_ConfigDlg;
	CAboutDlg* m_AboutDlg;
	CDaemonDlg* m_DaemonDlg;
	CTipDlg* m_TipDlg;

public:
	void SendMouseEvent();
	BOOL IsFilterApplication();
	BOOL GetProcessList(std::list<std::string>& list);
	void Cleanup();
	void Notify() {};
	void UpdateTip();
	
	afx_msg LRESULT OnMouseNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConfigNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAboutNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTipNotify(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnShutdown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReopen(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnMenuActive();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuHide();
	afx_msg void OnMenuConfig();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuExit();

	afx_msg void OnClose();

	afx_msg void OnBnClickedButtonMouse();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
