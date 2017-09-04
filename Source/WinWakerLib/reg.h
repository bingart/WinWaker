
#ifndef REG_H
#define REG_H

#include <string>

BOOL ReadRegKeyValue(HKEY hKey, const char* subKey,  const char* valueName, DWORD* dwType, char* cBuffer, DWORD szBuffer);

BOOL CreateServiceWithReg(const char* szSvcName, const char* szSvcDisplayName, const char* szSvcPath);
BOOL CreateServiceWithAPI(const char* szSvcName, const char* szSvcDisplayName, const char* szSvcPath);
BOOL IsServiceExists(const char* svcName);

#endif // REG_H