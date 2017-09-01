// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinWaker.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "WorkConfig.h"
#include "WinWakerDragDrop.h"
#include "Interface.h"
#include "WorkLog.h"

// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

extern PtrChangeWindowMessageFilterEx ptrChangeWindowMessageFilterEx;

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MOUSE_ACTIVE);
	m_pParent = pParent;
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_CONFIG_OK, m_ButtonOk);
	DDX_Control(pDX, ID_CONFIG_CANCEL, m_ButtonCancel);

	DDX_Control(pDX, IDC_EDIT_STEP, m_EditStep);
	DDX_Control(pDX, IDC_SPIN_STEP, m_SpinStep);

	DDX_Control(pDX, IDC_EDIT_INTER_TIME, m_EditInterTime);
	DDX_Control(pDX, IDC_SPIN_INTER_TIME, m_SpinInterTime);

	DDX_Control(pDX, IDC_CHECK_AUTORUN, m_CheckBoxAutoRun);
	DDX_Control(pDX, IDC_CHECK_FILTER_APPLICATION, m_CheckBoxFilterApplication);
	DDX_Control(pDX, IDC_LIST_APPLICATION, m_ListBoxApplication);
	DDX_Control(pDX, IDC_BUTTON_ADD_APPLICATION, m_ButtonAddApplication);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_APPLICATION, m_ButtonRemoveApplication);
	DDX_Control(pDX, IDC_BUTTON_SET_DEFAULT, m_ButtonSetDefault);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_ButtonClearApplication);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(ID_CONFIG_OK, &CConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CONFIG_CANCEL, &CConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_AUTORUN, &CConfigDlg::OnBnClickedCheckAutorun)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP, &CConfigDlg::OnDeltaposSpinStep)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INTER_TIME, &CConfigDlg::OnDeltaposSpinInterTime)
	ON_BN_CLICKED(IDC_CHECK_FILTER_APPLICATION, &CConfigDlg::OnBnClickedCheckFilterApplication)
	ON_LBN_SELCHANGE(IDC_LIST_APPLICATION, &CConfigDlg::OnLbnSelchangeListApplication)
	ON_BN_CLICKED(IDC_BUTTON_ADD_APPLICATION, &CConfigDlg::OnBnClickedButtonAddApplication)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_APPLICATION, &CConfigDlg::OnBnClickedButtonRemoveApplication)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT, &CConfigDlg::OnBnClickedButtonSetDefault)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CConfigDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CConfigDlg message handlers


BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO:  Add extra initialization here
	// Load configuration
	if (true)
	{
		m_ButtonOk.SetFlat(FALSE);
		m_ButtonOk.SetTooltipText("Save configuration");
		m_ButtonCancel.SetFlat(FALSE);
		m_ButtonCancel.SetTooltipText("Discard configuration");
		m_ButtonCancel.SetFocus();
		m_ButtonSetDefault.SetFlat(FALSE);
		m_ButtonSetDefault.SetTooltipText("Reset default configuration");

		m_ButtonAddApplication.SetFlat(FALSE);
		m_ButtonAddApplication.SetTooltipText("Open file dialog to add application");
		m_ButtonRemoveApplication.SetFlat(FALSE);
		m_ButtonRemoveApplication.SetTooltipText("Remove current selected application");
		m_ButtonRemoveApplication.EnableWindow(FALSE);
		m_ButtonClearApplication.SetFlat(FALSE);
		m_ButtonClearApplication.SetTooltipText("Remove all applications");

		int value = CWorkConfig::Instance()->GetAutoRun();
		if (value)
		{
			m_CheckBoxAutoRun.SetCheck(TRUE);
		}
		else
		{
			m_CheckBoxAutoRun.SetCheck(FALSE);
		}

		value = CWorkConfig::Instance()->GetStep();
		char szBuffer[MAX_PATH] = {0};
		sprintf(szBuffer, "%d", value);
		m_EditStep.SetWindowText(szBuffer);

		value = CWorkConfig::Instance()->GetInterTime();
		char szBuffer2[MAX_PATH] = {0};
		sprintf(szBuffer2, "%d", value);
		m_EditInterTime.SetWindowText(szBuffer2);

		value = CWorkConfig::Instance()->GetFilterApplication();
		if (value)
		{
			m_CheckBoxFilterApplication.SetCheck(TRUE);
		}
		else
		{
			m_CheckBoxFilterApplication.SetCheck(FALSE);
		}

		m_ListBoxApplication.ResetContent();
		std::list<std::string> list;
		CWorkConfig::Instance()->GetApplicationList(list);
		for (std::list<std::string>::iterator it = list.begin(); it != list.end(); it++)
		{
			m_ListBoxApplication.AddString(it->c_str());
		}
	}

	// Enable drag and drop
	EnableDragDrop(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	char szBuffer[MAX_PATH] = {0};
	int i;

	// Auto run
	if (m_CheckBoxAutoRun.GetCheck())
	{
		CWorkConfig::Instance()->SetAutoRun(TRUE);
	}
	else
	{
		CWorkConfig::Instance()->SetAutoRun(FALSE);
	}
	
	// Step
	memset(szBuffer, 0, sizeof szBuffer);
	m_EditStep.GetWindowText(szBuffer, sizeof szBuffer);
	i = atoi(szBuffer);
	CWorkConfig::Instance()->SetStep(i);

	// Inter Time
	memset(szBuffer, 0, sizeof szBuffer);
	m_EditInterTime.GetWindowText(szBuffer, sizeof szBuffer);
	i = atoi(szBuffer);
	CWorkConfig::Instance()->SetInterTime(i);

	// Filter Application
	if (m_CheckBoxFilterApplication.GetCheck())
	{
		CWorkConfig::Instance()->SetFilterApplication(TRUE);
	}
	else
	{
		CWorkConfig::Instance()->SetFilterApplication(FALSE);
	}

	// Application List
	CWorkConfig::Instance()->ResetApplicationList();
	int iCount = m_ListBoxApplication.GetCount();
	for (int i = 0; i < iCount; i++)
	{
		memset(szBuffer, 0, sizeof szBuffer);
		m_ListBoxApplication.GetText(i, szBuffer);
		CWorkConfig::Instance()->AddApplication(szBuffer);
	}

	// Save
	CWorkConfig::Instance()->Save();

	if (m_pParent)
	{
		m_pParent->PostMessage(WM_CONFIG_NOTIFY);
	}

	OnOK();
}


void CConfigDlg::OnBnClickedCancel()
{
	if (m_pParent)
	{
		m_pParent->PostMessage(WM_CONFIG_NOTIFY);
	}

	OnCancel();
}


void CConfigDlg::OnBnClickedButtonSetDefault()
{
	// Auto run
	m_CheckBoxAutoRun.SetCheck(TRUE);
	
	// Step
	m_EditStep.SetWindowText("5");

	// Inter Time
	m_EditInterTime.SetWindowText("10");

	// Filter Application
	m_CheckBoxFilterApplication.SetCheck(FALSE);

	// Application List
	m_ListBoxApplication.ResetContent();
	m_ListBoxApplication.AddString("POWERPNT.EXE");
	m_ListBoxApplication.AddString("EXCEL.EXE");
	m_ListBoxApplication.AddString("WINWORD.EXE");
	m_ListBoxApplication.AddString("putty.exe");
	m_ListBoxApplication.AddString("psftp.exe");
	m_ListBoxApplication.AddString("eclipse.exe");
	m_ListBoxApplication.AddString("devenv.exe");
}


void CConfigDlg::OnBnClickedCheckAutorun()
{
}


void CConfigDlg::OnDeltaposSpinStep(NMHDR *pNMHDR, LRESULT *pResult)
{
	char szBuffer[MAX_PATH] = {0};
	int i;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	// Get
	memset(szBuffer, 0, sizeof szBuffer);
	m_EditStep.GetWindowText(szBuffer, sizeof szBuffer);
	i = atoi(szBuffer);
	
	// ++--
	if (pNMUpDown->iDelta == 1)
	{
		i--;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		i++;
	}
	
	// Constraint
	if (i < CWorkConfig::MIN_STEP) i = CWorkConfig::MIN_STEP;
	if (i > CWorkConfig::MAX_STEP) i = CWorkConfig::MAX_STEP;

	// Set
	memset(szBuffer, 0, sizeof szBuffer);
	sprintf(szBuffer, "%d", i);
	m_EditStep.SetWindowText(szBuffer);

	*pResult = 0;
}


void CConfigDlg::OnDeltaposSpinInterTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	char szBuffer[MAX_PATH] = {0};
	int i;

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	// Get
	memset(szBuffer, 0, sizeof szBuffer);
	m_EditInterTime.GetWindowText(szBuffer, sizeof szBuffer);
	i = atoi(szBuffer);
	
	// ++--
	if (pNMUpDown->iDelta == 1)
	{
		i--;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		i++;
	}
	
	// Constraint
	if (i < CWorkConfig::MIN_INTER_TIME) i = CWorkConfig::MIN_INTER_TIME;
	if (i > CWorkConfig::MAX_INTER_TIME) i = CWorkConfig::MAX_INTER_TIME;

	// Set
	memset(szBuffer, 0, sizeof szBuffer);
	sprintf(szBuffer, "%d", i);
	m_EditInterTime.SetWindowText(szBuffer);

	*pResult = 0;
}


void CConfigDlg::OnBnClickedCheckFilterApplication()
{
}


void CConfigDlg::OnLbnSelchangeListApplication()
{
	int nIndex = m_ListBoxApplication.GetCurSel();
	int nCount = m_ListBoxApplication.GetCount();
	if (nIndex != LB_ERR && nCount > 0 && nIndex < nCount && nIndex >= 0)
	{
		m_ButtonRemoveApplication.EnableWindow(TRUE);
	}
	else
	{
		m_ButtonRemoveApplication.EnableWindow(FALSE);
	}
}


void CConfigDlg::OnBnClickedButtonAddApplication()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	TCHAR szFilters[]= _T("EXE Files (*.exe)|*.exe|Short Cut Files (*.lnk)|*.lnk||");

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(
		TRUE,
		_T("EXE Files"),
		_T("*.exe"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		szFilters);
	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if(fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();

		std::string szDroppedPath = pathName;

		std::string szExeFileName;
		BOOL rc = GetExeFileNameFromDropFiles(szDroppedPath, szExeFileName);
		if (rc)
		{
			AddApplicationToListBox(szExeFileName);
		}
	}
}


void CConfigDlg::OnBnClickedButtonRemoveApplication()
{
	int nIndex = m_ListBoxApplication.GetCurSel();
	int nCount = m_ListBoxApplication.GetCount();
	if (nIndex != LB_ERR && nCount > 0 && nIndex < nCount && nIndex >= 0)
	{
		m_ListBoxApplication.DeleteString(nIndex);

		if (m_ListBoxApplication.GetCount() == 0)
		{
			m_ButtonRemoveApplication.EnableWindow(FALSE);
		}
		else
		{
			if (nIndex >= m_ListBoxApplication.GetCount())
			{
				nIndex = m_ListBoxApplication.GetCount() - 1;
			}

			m_ListBoxApplication.SetCurSel(nIndex);
		}
	}
}


void CConfigDlg::OnBnClickedButtonClear()
{
	m_ListBoxApplication.ResetContent();
	m_ButtonRemoveApplication.EnableWindow(FALSE);
}


void CConfigDlg::OnDropFiles(HDROP hDropInfo)
{
	char szDroppedFile[MAX_PATH] = {0};	// drop file path
	UINT rc = DragQueryFile(
		hDropInfo,		// Struture Identifier
        0,				// -1 to Drop more than one file or ( integer 0 to max ) to drop selected No of files
        szDroppedFile,	// Droped File Name
        MAX_PATH		// Max char
		);
	if (rc)
	{
		std::string szDroppedPath = szDroppedFile;

		std::string szExeFileName;
		BOOL rc = GetExeFileNameFromDropFiles(szDroppedPath, szExeFileName);
		if (rc)
		{
			AddApplicationToListBox(szExeFileName);
		}
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

VOID CConfigDlg::AddApplicationToListBox(std::string szExeFileName)
{
	if (m_ListBoxApplication.GetCount() < CWorkConfig::MAX_APPLICATION_LIST_SIZE)
	{
		// Check if duplicated
		BOOL bFound = FALSE;
		CString szNew = szExeFileName.c_str();
		for (int i = 0; i < m_ListBoxApplication.GetCount(); i++)
		{
			CString szValue;
			m_ListBoxApplication.GetText(i, szValue);
			if (szValue.CompareNoCase(szNew) == 0)
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound)
		{
			m_ListBoxApplication.AddString(szExeFileName.c_str());
		}
		else
		{
			WorkLog::Format("application %s found, ignored\n", szExeFileName.c_str());
		}
	}
}
