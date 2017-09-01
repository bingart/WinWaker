
#include <windows.h>
#include <iostream>
#include "scrambler.h"
#include "reg.h"
#include "easylog.hpp"
#include "libstr.h"

BOOL ReadRegKeyValue(HKEY hKey, const char* subKey, const char* valueName, DWORD* dwType, char* cBuffer, DWORD szBuffer)
{
    // 打开键
	HKEY hKey2;
    LONG lRet = RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hKey2);
    if(lRet == ERROR_SUCCESS) 
    {
        //读取键值
        if(RegQueryValueEx(
			hKey2,
			valueName,
			0,
			dwType,
			(BYTE*)cBuffer,
			&szBuffer) == ERROR_SUCCESS)
		{
			// 关闭键
			RegCloseKey(hKey2);
			return TRUE;
		}
		else
		{
			// 关闭键
			RegCloseKey(hKey2);
			// "read reg key value, query key value fails, error %d\n"
			EasyLog::Format(GetLibStrById(508), GetLastError());
			return FALSE;
		}
	}
	else
	{
		// "read reg key value, open key fails, error %d\n"
		EasyLog::Format(GetLibStrById(509), GetLastError());
		return FALSE;
	}
}

BOOL CreateServiceWithReg(const char* szSvcName, const char* szSvcDisplayName, const char* szSvcPath)
{
	SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;

    // 打开键
	HKEY hKey = NULL;
	HKEY hServiceKey = NULL;
	DWORD dw;
	// "SYSTEM\\CurrentControlSet\\Services"
	LPCTSTR lpSubKey = GetLibStrById(510);
	DWORD dwValue;
	char szValue[MAX_PATH] = {0};
	WCHAR szWideValue[MAX_PATH] = {0};
	BOOL bResult = FALSE;

	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ|KEY_WRITE|KEY_CREATE_SUB_KEY, &hKey);
    if(lRet == ERROR_SUCCESS) 
    {
		if(RegCreateKeyEx(
			hKey,
			szSvcName,
			0,
			REG_NONE,
			REG_OPTION_NON_VOLATILE,
			KEY_WRITE|KEY_READ,
			NULL,
			&hServiceKey,
			&dw) == ERROR_SUCCESS)
		{
			do
			{
				std::string key;

				// Type
				dwValue = SERVICE_WIN32_OWN_PROCESS;
				// "Type"
				key = GetLibStrById(511);
				if (RegSetValueEx(hServiceKey, key.c_str(), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD)))
				{
					// "write value %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(512), key.c_str(), GetLastError());
					break;
				}

				// Start
				dwValue = SERVICE_AUTO_START;
				// "Start"
				key = GetLibStrById(513);
				if (RegSetValueEx(hServiceKey, key.c_str(), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD)))
				{
					// "write value %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(512), key.c_str(), GetLastError());
					break;
				}
				
				// Local System
				// "LocalSystem"
				strcpy(szValue, GetLibStrById(514));
				// "ObjectName"
				key = GetLibStrById(515);
				if (RegSetValueEx(hServiceKey, key.c_str(), 0, REG_SZ, (CONST BYTE*)szValue, strlen(szValue)))
				{
					// "write value %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(512), key.c_str(), GetLastError());
					break;
				}

				// Image Path
				strcpy(szValue, szSvcPath);
				// "ImagePath"
				key = GetLibStrById(516);
				if (RegSetValueEx(hServiceKey, key.c_str(), 0, REG_EXPAND_SZ, (CONST BYTE*)szValue, strlen(szValue)))
				{
					// "write value %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(512), key.c_str(), GetLastError());
					break;
				}
				
				// Error Control
				dwValue = SERVICE_ERROR_NORMAL;
				// "ErrorControl"
				key = GetLibStrById(517);
				if (RegSetValueEx(hServiceKey, key.c_str(), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD)))
				{
					// "write value %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(512), key.c_str(), GetLastError());
					break;
				}
				
				// Display Name
				strcpy(szValue, szSvcDisplayName);
				// "DisplayName"
				key = GetLibStrById(518);
				if (RegSetValueEx(hServiceKey, key.c_str(), 0, REG_SZ, (CONST BYTE*)szValue, strlen(szValue)))
				{
					// "write value %s fails, error %d\n"
					EasyLog::Format(GetLibStrById(512), key.c_str(), GetLastError());
					break;
				}

				// "create service registration ok\n"
				EasyLog::Format(GetLibStrById(519));
				bResult = TRUE;
			}
			while (false);
		}
		else
		{
			// "create key %s fails, error %d\n"
			EasyLog::Format(GetLibStrById(520), szSvcName, GetLastError());
		}
	}
	else
	{
		// "open key %s fails, error %d\n"
		EasyLog::Format(GetLibStrById(521), lpSubKey, GetLastError());
	}

	if (hKey != NULL) RegCloseKey(hKey);
	if (hServiceKey != NULL) RegCloseKey(hServiceKey);

	return bResult;
}

BOOL CreateServiceWithAPI(const char* szSvcName, const char* szSvcDisplayName, const char* szSvcPath)
{
	SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;
	BOOL bResult = FALSE;

	// Get a handle to the SCM database. 
	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS    // full access rights 
		);

	if (schSCManager == NULL) 
	{
		// "OpenSCManager failed (%d)\n"
		EasyLog::Format(GetLibStrById(566), GetLastError());
	}
	else
	{
		// Create the service
		schService = CreateService( 
			schSCManager,              // SCM database
			szSvcName,                 // name of service
			szSvcDisplayName,	       // service name to display
			SERVICE_ALL_ACCESS,        // desired access
			SERVICE_WIN32_OWN_PROCESS, // service type
			SERVICE_AUTO_START,        // start type
			SERVICE_ERROR_NORMAL,      // error control type
			szSvcPath,                 // path to service's binary
			NULL,                      // no load ordering group
			NULL,                      // no tag identifier
			NULL,                      // no dependencies
			NULL,                      // LocalSystem account
			NULL                       // no password
			);
		if (schService == NULL)
		{
			// "CreateService failed (%d)\n"
			EasyLog::Format(GetLibStrById(567), GetLastError());
		}
		else
		{
			bResult = TRUE;
			// "CreateService successfully\n
			EasyLog::Format(GetLibStrById(568));
		}
	}
    
	if (schService != NULL)
	{
		CloseServiceHandle(schService);
	}
	
	if (schSCManager != NULL)
	{
		CloseServiceHandle(schSCManager);
	}

	return bResult;
}

BOOL IsServiceExists(const char* svcName)
{
    // 打开键
	HKEY hKey = NULL;
	HKEY hServiceKey = NULL;
	BOOL bResult = FALSE;
	// "SYSTEM\\CurrentControlSet\\Services"
	LPCTSTR lpSubKey = GetLibStrById(510);
	std::string lpServiceSubKey = lpSubKey;
	lpServiceSubKey += "\\";
	lpServiceSubKey += svcName;

	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpServiceSubKey.c_str(), 0, KEY_READ, &hKey);
    if(lRet == ERROR_SUCCESS) 
    {
		bResult = TRUE;
	}

	if (hKey != NULL) RegCloseKey(hKey);

	return bResult;
}
