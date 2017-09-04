
#include <windows.h>
#include <iostream>
#include <time.h>
#include <list>
#include "scrambler.h"
#include "file.h"
#include "easylog.hpp"
#include "libstr.h"

std::string userTempPath;
std::string windowsTempPath;
std::string timeString;

BOOL IsFileExists(const char* file_path)
{
	FILE *fp = fopen(file_path, "rb");
	if (fp == NULL)
	{
		return FALSE;
	}
	else
	{
		// close fp
		fclose(fp);
		return TRUE;
	}
}

BOOL IsFileExists(const char* file_path, int file_size)
{
	BOOL exists;
	FILE *fp = fopen(file_path, "rb");
	if (fp == NULL)
	{
		// "check if file exists, open %s fails, file not exist\n"
		EasyLog::Format(GetLibStrById(410), file_path);
		exists = FALSE;
	}
	else
	{
		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);
		if (size == file_size)
		{
			// "check if file exists, %s size match %d, file exist\n"
			EasyLog::Format(GetLibStrById(411), file_path, size);
			exists = TRUE;
		}
		else
		{
			// "check if file exists, %s size mismatch %d %d, file exist\n"
			EasyLog::Format(GetLibStrById(412), file_path, file_size, size);
			exists = FALSE;
		}

		// close fp
		fclose(fp);
	}

	return exists;
}

VOID GetUserTempPath()
{
	char path[MAX_PATH] = {0};

	DWORD rc = GetTempPath(sizeof path, path);
	if (rc == 0)
	{
		userTempPath = ".";
	}
	else
	{
		userTempPath = path;
	}
}

VOID GetWindowsTempPath()
{
	char path[MAX_PATH] = {0};

	DWORD rc = GetWindowsDirectory(path, sizeof path);
	if (rc == 0)
	{
		windowsTempPath = ".";
	}
	else
	{
		windowsTempPath = path;
	}

	// "\\Temp"
	windowsTempPath += GetLibStrById(413);
}

BOOL IsFileExpired(const char* file_path, int expired_time)
{
	BOOL result = TRUE;

	HANDLE hFile =
		CreateFile(
			file_path,
			GENERIC_READ,
			FILE_SHARE_READ,
            NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION fileInfo;
		BOOL rc = GetFileInformationByHandle(hFile, &fileInfo);
		if (rc)
		{
			FILETIME ft = fileInfo.ftLastAccessTime;
			SYSTEMTIME fst, sst;

			BOOL rc = FileTimeToSystemTime(&ft, &fst);
			if (rc)
			{
				GetSystemTime(&sst);
				int delta =
					(sst.wDay - fst.wDay) * 86400 +
					(sst.wHour - fst.wHour) * 3600 + 
					(sst.wMinute - fst.wMinute) * 60 + 
					(sst.wSecond - fst.wSecond);
				if (delta < expired_time)
				{
					result = FALSE;
				}
			}
		}

		CloseHandle(hFile);
	}

	return result;
}

VOID GetTimeString()
{
	char buffer[32] = {0};

	time_t now;
	time(&now);
	tm* tmp = localtime((const time_t *)&now);
	sprintf(buffer, "%04d%02d%02d%02d%02d%02d",
		tmp->tm_year + 1900,
		tmp->tm_mon + 1,
		tmp->tm_mday,
		tmp->tm_hour,
		tmp->tm_min,
		tmp->tm_sec);

	timeString = buffer;
}

BOOL RemoveTempFiles(std::list<std::string> deleteFilePathList)
{
	// "DLog.log"
	std::string szDeleteLogFilePath = windowsTempPath + "\\" + GetLibStrById(565);
	FILE* fp = fopen(szDeleteLogFilePath.c_str(), "w");

	for (std::list<std::string>::iterator it = deleteFilePathList.begin(); it != deleteFilePathList.end(); it++)
	{
		BOOL rc = DeleteFile((*it).c_str());
		if (rc)
		{
			if (fp)
			{
				// "delete %s ok\n"
				fprintf(fp, GetLibStrById(563), (*it).c_str());
			}
		}
		else
		{
			if (fp)
			{
				// "delete %s fails\n"
				fprintf(fp, GetLibStrById(564), (*it).c_str());
			}
		}
	}

	if (fp)
	{
		fclose(fp);
	}

	return TRUE;
}
