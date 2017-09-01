#pragma once

#include <map>
#include "SystemTray.h"

class CMySystemTray : public CSystemTray
{
public:
	CMySystemTray(void);
	~CMySystemTray(void);

public:
	virtual void CustomizeMenu(CMenu* menu);
};

