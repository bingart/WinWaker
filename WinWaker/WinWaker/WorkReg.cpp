
#include <windows.h>
#include <iostream>
#include "WorkLog.h"
#include "WorkReg.h"

CWorkReg::CWorkReg(HKEY key, const char* subKeyName, BOOL bRead)
{
	m_Key = key;
	m_SubKey = NULL;
	m_SubKeyName = subKeyName;
	m_Opened = FALSE;
	
	REGSAM sam;
	if (bRead)
	{
		sam = KEY_READ;
	}
	else
	{
		sam = KEY_WRITE;
	}

	LONG lRet = RegOpenKeyEx(m_Key, subKeyName, 0, sam, &m_SubKey);
    if(lRet == ERROR_SUCCESS) 
    {
		WorkLog::Format("reg open ok\n");
	}
	else
	{
		WorkLog::Format("reg open fails, try to create\n");

		DWORD dwDisposition = REG_CREATED_NEW_KEY;
		if (RegCreateKeyEx(
			m_Key,
			subKeyName,
			0,
			REG_NONE,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&m_SubKey,
			&dwDisposition) == ERROR_SUCCESS)
		{
			WorkLog::Format("reg create ok\n");
			m_Opened = TRUE;
		}
		else
		{
			WorkLog::Format("reg create fails\n");
		}
	}
}

CWorkReg::~CWorkReg()
{
	if (m_SubKey != NULL)
	{
		RegCloseKey(m_SubKey);
	}
}

BOOL CWorkReg::ReadStringValue(const char* szName, std::string& szValue, std::string szDefaultValue)
{
	DWORD dwType;
	char cBuffer[1024] = {0};
	DWORD dwBuffer = sizeof cBuffer;
	BOOL bResult = FALSE;

	if (m_SubKey != NULL)
	{
		if(RegQueryValueEx(
			m_SubKey,
			szName,
			0,
			&dwType,
			(BYTE *)cBuffer,
			&dwBuffer) == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ)
			{
				szValue = cBuffer;
				bResult = TRUE;
			}
			else
			{
				WorkLog::Format("read fails, dwType %d mismatch\n", dwType);
			}
		}
		else
		{
			WorkLog::Format("read fails for %s, query error %d\n", szName, GetLastError());
		}
	}
	else
	{
		WorkLog::Format("read fails for %s, reg key is null\n", szName);
	}

	if (!bResult)
	{
		szValue = szDefaultValue;
	}

	return bResult;
}

BOOL CWorkReg::ReadIntegerValue(const char* szName, int& iValue, int iDefaultValue)
{
	DWORD dwType;
	char cBuffer[MAX_PATH] = {0};
	DWORD dwBuffer = sizeof cBuffer;
	BOOL bResult = FALSE;

	if (m_SubKey != NULL)
	{
		if(RegQueryValueEx(
			m_SubKey,
			szName,
			0,
			&dwType,
			(BYTE *)cBuffer,
			&dwBuffer) == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwBuffer == 4)
			{
				iValue = *((int *)cBuffer);
				bResult = TRUE;
			}
			else
			{
				WorkLog::Format("read fails, dwType %d mismatch\n", dwType);
			}
		}
		else
		{
			WorkLog::Format("read fails for %s, query error %d\n", szName, GetLastError());
		}
	}
	else
	{
		WorkLog::Format("read fails for %s, reg key is null\n", szName);
	}

	if (!bResult)
	{
		iValue = iDefaultValue;
	}

	return bResult;
}

BOOL CWorkReg::WriteStringValue(const char* szName, const char* szValue)
{
	DWORD dwType = REG_SZ;
	char cBuffer[MAX_PATH] = {0};
	DWORD dwBuffer;

	strncpy(cBuffer, szValue, MAX_PATH);
	dwBuffer = strnlen(cBuffer, MAX_PATH) + 1;

	if (m_SubKey != NULL)
	{
		if(RegSetValueEx(
			m_SubKey,
			szName,
			0,
			dwType,
			(BYTE *)cBuffer,
			dwBuffer) == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			WorkLog::Format("write fails for %s, set error %d\n", szName, GetLastError());
		}
	}
	else
	{
		WorkLog::Format("write fails for %s, reg key is null\n", szName);
	}

	return FALSE;
}

BOOL CWorkReg::WriteIntegerValue(const char* szName, int iValue)
{
	DWORD dwType = REG_DWORD;
	DWORD dwBuffer = 4;

	if (m_SubKey != NULL)
	{
		if(RegSetValueEx(
			m_SubKey,
			szName,
			0,
			dwType,
			(BYTE *)&iValue,
			dwBuffer) == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			WorkLog::Format("write fails for %s, set error %d\n", szName, GetLastError());
		}
	}
	else
	{
		WorkLog::Format("write fails for %s, reg key is null\n", szName);
	}

	return FALSE;
}

BOOL CWorkReg::RemoveValue(const char* szName)
{
	if (m_SubKey != NULL)
	{
		if (RegDeleteValue(
			m_SubKey,
			szName) == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			WorkLog::Format("delete fails for %s, delete error %d\n", szName, GetLastError());
		}
	}
	else
	{
		WorkLog::Format("delete fails for %s, reg key is null\n", szName);
	}

	return FALSE;
}
