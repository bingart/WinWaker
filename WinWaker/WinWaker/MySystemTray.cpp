#include "stdafx.h"
#include "MySystemTray.h"
#include "resource.h"
#include "WorkConfig.h"
#include "WinWakerDlg.h"

CMySystemTray::CMySystemTray(void)
	: CSystemTray()
{
}


CMySystemTray::~CMySystemTray(void)
{
}

void CMySystemTray::CustomizeMenu(CMenu* menu)
{
	if (CWorkConfig::Instance()->GetWorkingMode())
	{
		menu->ModifyMenu(0, MF_BYPOSITION, ID_MENU_ACTIVE, MENU_DEACTIVATING_TITLE);
	}
	else
	{
		menu->ModifyMenu(0, MF_BYPOSITION, ID_MENU_ACTIVE, MENU_ACTIVATING_TITLE);
	}
}