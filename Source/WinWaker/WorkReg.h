
#ifndef WORKREG_H
#define WORKREG_H

#include <string>

//BOOL ReadRegKeyValue(HKEY hKey, const char* subKey,  const char* valueName, DWORD* dwType, char* cBuffer, DWORD szBuffer);

class CWorkReg
{
private:
	HKEY m_Key;
	HKEY m_SubKey;
	std::string m_SubKeyName;
	BOOL m_Opened;

public:
	CWorkReg(HKEY key, const char* subKeyName, BOOL bRead = TRUE);
	~CWorkReg();

public:
	BOOL IsOpened() { return m_Opened; }

public:
	BOOL ReadStringValue(const char* szName, std::string& szValue, std::string szDefaultValue = "");
	BOOL ReadIntegerValue(const char* szName, int& iValue, int iDefaultValue = 0);

public:
	BOOL WriteStringValue(const char* szName, const char* szValue);
	BOOL WriteIntegerValue(const char* szName, int iValue);

public:
	BOOL RemoveValue(const char* szName);
};

#endif // REG_H