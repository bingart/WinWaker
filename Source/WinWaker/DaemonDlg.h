#pragma once


// CDaemonDlg dialog

class CDaemonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDaemonDlg)

public:
	CDaemonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDaemonDlg();

// Dialog Data
	enum { IDD = IDD_DAEMON_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CWnd* m_pParent;

	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnShutdown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReopen(WPARAM wParam, LPARAM lParam);

};
