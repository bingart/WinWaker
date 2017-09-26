
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
		WorkLog::Format("reg open ok, subKeyName=%s\n", subKeyName);
	}
	else
	{
		WorkLog::Format("reg open fails, subKeyName=%s, lRet=%d, try to create\n", subKeyName, lRet);

		DWORD dwDisposition = REG_CREATED_NEW_KEY;
		lRet = RegCreateKeyEx(
			m_Key,
			subKeyName,
			0,
			REG_NONE,
			REG_OPTION_NON_VOLATILE,
			KEY_QUERY_VALUE | KEY_SET_VALUE,
			NULL,
			&m_SubKey,
			&dwDisposition);
		if (lRet == ERROR_SUCCESS)
		{
			WorkLog::Format("reg create ok\n");
			m_Opened = TRUE;
		}
		else
		{
			WorkLog::Format("reg create fails, subKeyName=%s, ec=%d\n", subKeyName, lRet);
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
		LONG lRet = RegQueryValueEx(
			m_SubKey,
			szName,
			0,
			&dwType,
			(BYTE *)cBuffer,
			&dwBuffer);
		if (lRet == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ)
			{
				szValue = cBuffer;
				bResult = TRUE;
			}
			else
			{
				WorkLog::Format("read fails, szName=%s, dwType=%d mismatch\n", szName, dwType);
			}
		}
		else
		{
			WorkLog::Format("read fails, szName=%s, ec=%d\n", szName, lRet);
		}
	}
	else
	{
		WorkLog::Format("read fails, szName=%s, reg key is null\n", szName);
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
		LONG lRet = RegQueryValueEx(
			m_SubKey,
			szName,
			0,
			&dwType,
			(BYTE *)cBuffer,
			&dwBuffer);
		if (lRet == ERROR_SUCCESS)
		{
			if (dwType == REG_DWORD && dwBuffer == 4)
			{
				iValue = *((int *)cBuffer);
				bResult = TRUE;
			}
			else
			{
				WorkLog::Format("read fails, szName=%s, dwType %d mismatch\n", szName, dwType);
			}
		}
		else
		{
			WorkLog::Format("read fails, szName=%s, ec=%d\n", szName, lRet);
		}
	}
	else
	{
		WorkLog::Format("read fails, szName=%s, reg key is null\n", szName);
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
		long lRet = RegSetValueEx(
			m_SubKey,
			szName,
			0,
			dwType,
			(BYTE *)cBuffer,
			dwBuffer);
		if (lRet == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			WorkLog::Format("write fails, szName=%s, ec=%d\n", szName, lRet);
		}
	}
	else
	{
		WorkLog::Format("write fails, szName=%s, reg key is null\n", szName);
	}

	return FALSE;
}

BOOL CWorkReg::WriteIntegerValue(const char* szName, int iValue)
{
	DWORD dwType = REG_DWORD;
	DWORD dwBuffer = 4;

	if (m_SubKey != NULL)
	{
		long lRet = RegSetValueEx(
			m_SubKey,
			szName,
			0,
			dwType,
			(BYTE *)&iValue,
			dwBuffer);
		if (lRet == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			WorkLog::Format("write fails, szName=%s, ec=%d\n", szName, lRet);
		}
	}
	else
	{
		WorkLog::Format("write fails, szName=%s, reg key is null\n", szName);
	}

	return FALSE;
}

BOOL CWorkReg::RemoveValue(const char* szName)
{
	if (m_SubKey != NULL)
	{
		long lRet = RegDeleteValue(
			m_SubKey,
			szName);
		if (lRet == ERROR_SUCCESS)
		{
			return TRUE;
		}
		else
		{
			WorkLog::Format("delete fails, szName=%s, ec=%d\n", szName, lRet);
		}
	}
	else
	{
		WorkLog::Format("delete fails, szName=%s, reg key is null\n", szName);
	}

	return FALSE;
}
