#include "stdafx.h"
#include "WinWakerDragDrop.h"
#include <Windows.h>
#include <string>
#include "afxwin.h"
#include "WorkLog.h"

#pragma comment(lib, "Psapi.lib")

BOOL EnableDragDrop(CWnd* pWnd)
{
	PtrChangeWindowMessageFilterEx ptrChangeWindowMessageFilterEx = NULL;

	// CWnd
	pWnd->DragAcceptFiles(TRUE);

	// Message Filter
	HMODULE hModule = LoadLibrary("USER32.dll");
	if (hModule != NULL)
	{
		ptrChangeWindowMessageFilterEx =
			(PtrChangeWindowMessageFilterEx) GetProcAddress(hModule, "ChangeWindowMessageFilterEx");

		if (ptrChangeWindowMessageFilterEx != NULL)
		{
			CHANGEFILTERSTRUCT cfs;
			cfs.cbSize = sizeof CHANGEFILTERSTRUCT;
			cfs.ExtStatus = MSGFLTINFO_NONE;
			ptrChangeWindowMessageFilterEx(pWnd->GetSafeHwnd(), WM_DROPFILES, MSGFLT_ALLOW, &cfs);
			ptrChangeWindowMessageFilterEx(pWnd->GetSafeHwnd(), WM_COPYDATA , MSGFLT_ALLOW, &cfs);
			ptrChangeWindowMessageFilterEx(pWnd->GetSafeHwnd(), 0x0049, MSGFLT_ALLOW, &cfs);
		}

		FreeLibrary(hModule);
	}

	return TRUE;
}

BOOL GetExeFilePathFromLink(const char* szDroppedFile, std::string& szExeFilePath)
{
	BOOL bResult = FALSE;
	HRESULT hres;
	IShellLink* psl = NULL;
	IPersistFile* ppf = NULL;
	char szGotPath[MAX_PATH] = {0};
	WIN32_FIND_DATA wfd;

	hres = CoInitialize(NULL);
	if (hres != S_OK)
	{
		WorkLog::Format("CoInitialize fails\n");
		return FALSE;
	}

	// Get a pointer to the IShellLink interface.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl);
	if (SUCCEEDED(hres))
	{
		// Get a pointer to the IPersistFile interface.
		hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
		if (SUCCEEDED(hres))
		{
			WORD wsz[MAX_PATH];

			// Ensure string is Unicode.
			MultiByteToWideChar(CP_ACP, 0, szDroppedFile, -1, (LPWSTR) wsz, MAX_PATH);

			// Load the shell link.
			hres = ppf->Load((LPWSTR) wsz, STGM_READ);
			if (SUCCEEDED(hres))
			{
				// Resolve the link.
				hres = psl->Resolve(NULL, SLR_ANY_MATCH);
				if (SUCCEEDED(hres))
				{
					strcpy(szGotPath, szDroppedFile);
						
					// Get the path to the link target.
					hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA *)&wfd, SLGP_SHORTPATH);
					if (SUCCEEDED(hres))
					{
						szExeFilePath = szGotPath;
						WorkLog::Format("GetExeFilePathFromLink ok, %s\n", szExeFilePath.c_str());
						bResult = TRUE;
					}
					else
					{
						WorkLog::Format("Get path fails\n");
					}
				}
				else
				{
					WorkLog::Format("Resolve shell link fails\n");
				}
			}
			else
			{
				WorkLog::Format("Load shell link fails\n");
			}
				
			// Release pointer to IPersistFile interface.
			ppf->Release();
		}
		else
		{
			WorkLog::Format("QueryInterface fails\n");
		}

		// Release pointer to IShellLink interface.
		psl->Release();
	}
	else
	{
		WorkLog::Format("CoCreateInstance fails\n");
	}

	CoUninitialize();

	return bResult;
}

VOID ConvertOfficeFileNameToExeFileName(std::string szOfficePath, std::string& szExeFileName)
{
	if (szOfficePath == "outicon.exe")
	{
		szExeFileName = "OUTLOOK.EXE";
	}
	else if (szOfficePath == "xlicons.exe")
	{
		szExeFileName = "EXCEL.EXE";
	}
	else if (szOfficePath == "wordicon.exe")
	{
		szExeFileName = "WINWORD.EXE";
	}
	else if (szOfficePath == "pptico.exe")
	{
		szExeFileName = "POWERPNT.EXE";
	}
	else if (szOfficePath == "accicons.exe")
	{
		szExeFileName = "MSACCESS.EXE";
	}
	else if (szOfficePath == "visicon.exe")
	{
		szExeFileName = "VISIO.EXE";
	}
	else
	{
		szExeFileName = szOfficePath;
	}
}

BOOL GetExeFileNameFromDropFiles(std::string szDroppedPath, std::string& szExeFileName)
{
	BOOL bResult = FALSE;
	std::string szShortExeFilePath;			// short exe file path include "~"
	std::string szLongExeFilePath;			// long exe file path, without "~"

	// Get suffix
	std::string suffix = szDroppedPath.substr(szDroppedPath.length() - 3, 3);
	if (suffix == "exe")
	{
		szShortExeFilePath = szDroppedPath;
	}
	else if (suffix == "lnk")
	{
		BOOL bResult = GetExeFilePathFromLink(szDroppedPath.c_str(), szShortExeFilePath);
		if (!bResult)
		{
			WorkLog::Format("GetExeFilePathFromLink %s fails\n", szDroppedPath.c_str());
			return FALSE;
		}
	}
	else
	{
		WorkLog::Format("GetExeFileNameFromDropFiles fails, not lnk or exe file\n");
		return FALSE;
	}

	// Get szLongExeFilePath from szShortExeFilePath
	if (szShortExeFilePath.find("~") == std::string::npos)
	{
		szLongExeFilePath = szShortExeFilePath;
	}
	else
	{
		char szPath[MAX_PATH] = {0};
		DWORD dwRet = dwRet = GetLongPathName(szShortExeFilePath.c_str(), szPath, sizeof szPath);
		if(dwRet > 0 && dwRet < MAX_PATH)
		{
			szLongExeFilePath = szPath;
		}
		else
		{
			WorkLog::Format("GetLongPathName %s fails\n", szShortExeFilePath.c_str());
			return FALSE;
		}
	}

	// Get szOfficePath from szLongExeFilePath
	std::string szOfficePath;
	size_t pos = szLongExeFilePath.rfind("\\");
	szOfficePath = szLongExeFilePath.substr(pos + 1, szLongExeFilePath.size() - pos);

	// Convert szOfficePath to szExeFileName
	ConvertOfficeFileNameToExeFileName(szOfficePath, szExeFileName);

	return TRUE;
}