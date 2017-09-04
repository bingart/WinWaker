
#include <windows.h>
#include <wininet.h>
#include <io.h>
#include <string>
#include "scrambler.h"
#include "http2.h"
#include "easylog.hpp"
#include "libstr.h"

//#define USING_RTLIB

extern BOOL bRunning;

typedef BOOL (WINAPI *PtrInternetCrackUrl)(
  _In_     LPCTSTR lpszUrl,
  _In_     DWORD dwUrlLength,
  _In_     DWORD dwFlags,
  _Inout_  LPURL_COMPONENTS lpUrlComponents
);
typedef HINTERNET (WINAPI *PtrInternetOpen)(
  _In_  LPCTSTR lpszAgent,
  _In_  DWORD dwAccessType,
  _In_  LPCTSTR lpszProxyName,
  _In_  LPCTSTR lpszProxyBypass,
  _In_  DWORD dwFlags
);
typedef HINTERNET (WINAPI *PtrInternetConnect)(
  _In_  HINTERNET hInternet,
  _In_  LPCTSTR lpszServerName,
  _In_  INTERNET_PORT nServerPort,
  _In_  LPCTSTR lpszUsername,
  _In_  LPCTSTR lpszPassword,
  _In_  DWORD dwService,
  _In_  DWORD dwFlags,
  _In_  DWORD_PTR dwContext
);
typedef BOOL (WINAPI *PtrInternetReadFile)(
  _In_   HINTERNET hFile,
  _Out_  LPVOID lpBuffer,
  _In_   DWORD dwNumberOfBytesToRead,
  _Out_  LPDWORD lpdwNumberOfBytesRead
);
typedef BOOL (WINAPI *PtrInternetCloseHandle)(
  _In_  HINTERNET hInternet
);
typedef HINTERNET (WINAPI *PtrHttpOpenRequest)(
  _In_  HINTERNET hConnect,
  _In_  LPCTSTR lpszVerb,
  _In_  LPCTSTR lpszObjectName,
  _In_  LPCTSTR lpszVersion,
  _In_  LPCTSTR lpszReferer,
  _In_  LPCTSTR *lplpszAcceptTypes,
  _In_  DWORD dwFlags,
  _In_  DWORD_PTR dwContext
);
typedef BOOL (WINAPI *PtrHttpSendRequest)(
  _In_  HINTERNET hRequest,
  _In_  LPCTSTR lpszHeaders,
  _In_  DWORD dwHeadersLength,
  _In_  LPVOID lpOptional,
  _In_  DWORD dwOptionalLength
);
typedef BOOL (WINAPI *PtrHttpQueryInfo)(
  _In_     HINTERNET hRequest,
  _In_     DWORD dwInfoLevel,
  _Inout_  LPVOID lpvBuffer,
  _Inout_  LPDWORD lpdwBufferLength,
  _Inout_  LPDWORD lpdwIndex
);

HMODULE hModule = NULL;
PtrInternetCrackUrl ptrInternetCrackUrl = NULL;
PtrInternetOpen ptrInternetOpen = NULL;
PtrInternetConnect ptrInternetConnect = NULL;
PtrInternetReadFile ptrInternetReadFile = NULL;
PtrInternetCloseHandle ptrInternetCloseHandle = NULL;
PtrHttpOpenRequest ptrHttpOpenRequest = NULL;
PtrHttpSendRequest ptrHttpSendRequest = NULL;
PtrHttpQueryInfo ptrHttpQueryInfo = NULL;

BOOL HTTPInit()
{
	if (hModule != NULL)
	{
		// "dll already loaded, ignored\n"
		EasyLog::Format(GetLibStrById(454));
		return TRUE;
	}

	hModule = LoadLibrary("Wininet.dll");
	if (hModule == NULL)
	{
		// "load dll fails, error %d\n"
		EasyLog::Format(GetLibStrById(455), GetLastError());
		return FALSE;
	}

	do
	{
		// "InternetCrackUrlA"
		ptrInternetCrackUrl = (PtrInternetCrackUrl) GetProcAddress(hModule, GetLibStrById(456));
		if (ptrInternetCrackUrl == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "InternetOpenA"
		ptrInternetOpen = (PtrInternetOpen) GetProcAddress(hModule, GetLibStrById(458));
		if (ptrInternetOpen == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "InternetConnectA"
		ptrInternetConnect = (PtrInternetConnect) GetProcAddress(hModule, GetLibStrById(459));
		if (ptrInternetConnect == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "InternetReadFile"
		ptrInternetReadFile = (PtrInternetReadFile) GetProcAddress(hModule, GetLibStrById(460));
		if (ptrInternetReadFile == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "InternetCloseHandle"
		ptrInternetCloseHandle = (PtrInternetCloseHandle) GetProcAddress(hModule, GetLibStrById(461));
		if (ptrInternetCloseHandle == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "HttpOpenRequestA"
		ptrHttpOpenRequest = (PtrHttpOpenRequest) GetProcAddress(hModule, GetLibStrById(462));
		if (ptrHttpOpenRequest == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "HttpSendRequestA"
		ptrHttpSendRequest = (PtrHttpSendRequest) GetProcAddress(hModule, GetLibStrById(463));
		if (ptrHttpSendRequest == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}

		// "HttpQueryInfoA"
		ptrHttpQueryInfo = (PtrHttpQueryInfo) GetProcAddress(hModule, GetLibStrById(464));
		if (ptrHttpQueryInfo == NULL)
		{
			// "get proc address fails, error %d\n"
			EasyLog::Format(GetLibStrById(457), GetLastError());
			break;
		}
	}
	while (false);

	return TRUE;
}

VOID HTTPFini()
{
	if (hModule != NULL)
	{
		FreeLibrary(hModule);
		hModule = NULL;
	}
}

BOOL HTTPDownloadFile2(const char* url, const char* file_path)
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
			// "download file2 error, open file %s fails\n"
			EasyLog::Format(GetLibStrById(465), file_path);
			break;
		}
#else
		out = fopen(file_path, "wb");
		if (out == NULL)
		{
			// "download file2 error, open file %s fails\n"
			EasyLog::Format(GetLibStrById(465), file_path);
			break;
		}
#endif

		BOOL rc = ptrInternetCrackUrl(url, (DWORD)strlen(url),0, &crackedURL);
		if (!rc)
		{
			// "download file2 error, crack url fails, url %s, error %d\n"
			EasyLog::Format(GetLibStrById(466), url, GetLastError());
			break;
		}

		// 启用HTTP协议
		// "Microsoft Internet Explorer"
		hInt = ptrInternetOpen(
			GetLibStrById(467),
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0);
		if (hInt == NULL)
		{
			// "download file2 error, open http fails, error %d\n"
			EasyLog::Format(GetLibStrById(468), GetLastError());
			break;
		}

		// 建立HTTP连接
		hConn = ptrInternetConnect(
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
			// "download file2 error, open http connect fails, error %d\n"
			EasyLog::Format(GetLibStrById(469), GetLastError());
			break;
		}

		// 创建一个URL请求
		hReq = ptrHttpOpenRequest(hConn, "GET", crackedURL.lpszUrlPath, NULL, "", NULL, 0, 0);
		if (hReq == NULL)
		{
			// "download file2 error, open http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(470), GetLastError());
			break;
		}
    
		char buff[256];
		DWORD dwSizeOfRq = sizeof(DWORD);

		// 发送请求
		rc = ptrHttpSendRequest(hReq, NULL, 0, NULL, 0);
		if (!rc)
		{
			// "download file2 error, send http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(471), GetLastError());
			break;
		}

		// 查询响应代码
		rc = ptrHttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
		if (!rc)
		{
			// "download file2 error, http query info fails, error %d\n"
			EasyLog::Format(GetLibStrById(472), GetLastError());
			break;
		}
		else if (dwRetCode != 200)
		{
			// "download file2 error, http return not ok, %d\n"
			EasyLog::Format(GetLibStrById(473), dwRetCode);
			break;
		}

		// 查询Content Length
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (ptrHttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		{
			dwContLen = atol(buff);
			// "download file2, query content length %d\n"
			EasyLog::Format(GetLibStrById(474), dwContLen);
		}
		else
		{
			// "download file2, query content length fails, error %d\n"
			EasyLog::Format(GetLibStrById(475), GetLastError());
		}

		// 查询Content Type
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (ptrHttpQueryInfo(hReq, HTTP_QUERY_CONTENT_TYPE, &buff, &dwSizeOfRq, NULL))
		{
			// "download file2, query content type %s\n"
			EasyLog::Format(GetLibStrById(476), buff);
		}
		else
		{
			// "download file2, query content type fails, error %d\n"
			EasyLog::Format(GetLibStrById(477), GetLastError());
			break;
		}

		// 读取文件
		while (bRunning)
		{
			// 开始读取文件
			DWORD dwLen = 0;
			if (ptrInternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
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
					// "download file2, total length %d, written %d\n"
					EasyLog::Format(GetLibStrById(478), dwContLen, nBytesGet);
				}
			}
			else
			{
				// "download file2, read file fails, error %d\n"
				EasyLog::Format(GetLibStrById(479), GetLastError());
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
	if (hInt != NULL) ptrInternetCloseHandle(hInt);
	if (hConn != NULL) ptrInternetCloseHandle(hConn);
	if (hReq != NULL) ptrInternetCloseHandle(hReq);

	// For WinXP, nBytesGet > dwContLen, For Win7, nBytesGet == dwContLen
	return (dwRetCode == 200 && nBytesGet > 0);
}

BOOL HTTPGet2(const char* url, char* response, size_t max_response_size)
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
		BOOL rc = ptrInternetCrackUrl(url, (DWORD)strlen(url),0, &crackedURL);
		if (!rc)
		{
			// "get2 error, crack url fails, url %s, error %d\n"
			EasyLog::Format(GetLibStrById(480), url, GetLastError());
			break;
		}

		// 启用HTTP协议
		// "Microsoft Internet Explorer"
		hInt = ptrInternetOpen(
			GetLibStrById(481),
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0);
		if (hInt == NULL)
		{
			// "get2 error, open http fails, error %d\n"
			EasyLog::Format(GetLibStrById(482), GetLastError());
			break;
		}

		// 建立HTTP连接
		hConn = ptrInternetConnect(
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
			// "get2 error, open http connect fails, error %d\n"
			EasyLog::Format(GetLibStrById(483), GetLastError());
			break;
		}

		// 创建一个URL请求
		hReq = ptrHttpOpenRequest(hConn, "GET", crackedURL.lpszUrlPath, NULL, "", NULL, 0, 0);
		if (hReq == NULL)
		{
			// "get2 error, open http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(484), GetLastError());
			break;
		}

		char buff[256];

		// 发送请求
		rc = ptrHttpSendRequest(hReq, NULL, 0, NULL, 0);
		if (!rc)
		{
			// "get2 error, send http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(485), GetLastError());
			break;
		}

		// 查询响应代码
		rc = ptrHttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
		if (!rc)
		{
			// "get2 error, http query info fails, error %d\n"
			EasyLog::Format(GetLibStrById(486), GetLastError());
			break;
		}
		else if (dwRetCode != 200)
		{
			// "get2 error, http return not ok, %d\n"
			EasyLog::Format(GetLibStrById(487), dwRetCode);
			break;
		}

		// 查询Content Length
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (ptrHttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		{
			dwContLen = atol(buff);
			// "get2, query content length %d\n"
			EasyLog::Format(GetLibStrById(488), dwContLen);
		}
		else
		{
			// "get2, query content length fails, error %d\n"
			EasyLog::Format(GetLibStrById(489), GetLastError());
		}

		// 查询Content Type
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (ptrHttpQueryInfo(hReq, HTTP_QUERY_CONTENT_TYPE, &buff, &dwSizeOfRq, NULL))
		{
			// "get2, query content type %s\n"
			EasyLog::Format(GetLibStrById(490), buff);
		}
		else
		{
			// "get2, query content type fails, error %d\n"
			EasyLog::Format(GetLibStrById(491), GetLastError());
			break;
		}

		// 开始读取文件
		if (ptrInternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
		{
			if (dwLen > 0)
			{
				strncpy(response, (const char*)szBuffer,
					dwLen > max_response_size ? max_response_size : dwLen);
			}
		}
	}
	while(false);

	if (hInt != NULL) ptrInternetCloseHandle(hInt);
	if (hConn != NULL) ptrInternetCloseHandle(hConn);
	if (hReq != NULL) ptrInternetCloseHandle(hReq);

	return (dwRetCode == 200 && dwLen > 0);
}

BOOL HTTPPost2(
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
	char* hdrs = GetLibStrById(492);

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
		BOOL rc = ptrInternetCrackUrl(url, (DWORD)strlen(url),0, &crackedURL);
		if (!rc)
		{
			// "post2 error, crack url fails, url %s, error %d\n"
			EasyLog::Format(GetLibStrById(493), url, GetLastError());
			break;
		}

		// 启用HTTP协议
		// "Microsoft Internet Explorer"
		hInt = ptrInternetOpen(
			GetLibStrById(494),
			INTERNET_OPEN_TYPE_DIRECT,
			NULL,
			NULL,
			0);
		if (hInt == NULL)
		{
			// "post2 error, open http fails, error %d\n"
			EasyLog::Format(GetLibStrById(495), GetLastError());
			break;
		}

		// 建立HTTP连接
		hConn = ptrInternetConnect(
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
			// "post2 error, open http connect fails, error %d\n"
			EasyLog::Format(GetLibStrById(496), GetLastError());
			break;
		}

		// 创建一个URL请求
		hReq = ptrHttpOpenRequest(hConn, "POST", crackedURL.lpszUrlPath, NULL, "", NULL, 0, 0);
		if (hReq == NULL)
		{
			// "post2 error, open http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(497), GetLastError());
			break;
		}

		char buff[256];

		// 发送请求
		rc = ptrHttpSendRequest(hReq, hdrs, strlen(hdrs), (LPVOID) body, body_size);
		if (!rc)
		{
			// "post2 error, send http request fails, error %d\n"
			EasyLog::Format(GetLibStrById(498), GetLastError());
			break;
		}

		// 查询响应代码
		rc = ptrHttpQueryInfo(hReq, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
		if (!rc)
		{
			// "post2 error, http query info fails, error %d\n"
			EasyLog::Format(GetLibStrById(499), GetLastError());
			break;
		}
		else if (dwRetCode != 200)
		{
			// "post2 error, http return not ok, %d\n"
			EasyLog::Format(GetLibStrById(500), dwRetCode);
			break;
		}

		// 查询Content Length
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (ptrHttpQueryInfo(hReq, HTTP_QUERY_CONTENT_LENGTH, &buff, &dwSizeOfRq, NULL))
		{
			dwContLen = atol(buff);
			// "post2, query content length %d\n"
			EasyLog::Format(GetLibStrById(501), dwContLen);
		}
		else
		{
			// "post2, query content length fails, error %d\n"
			EasyLog::Format(GetLibStrById(502), GetLastError());
		}

		// 查询Content Type
		ZeroMemory(buff, sizeof buff);
		dwSizeOfRq = sizeof(buff);
		if (ptrHttpQueryInfo(hReq, HTTP_QUERY_CONTENT_TYPE, &buff, &dwSizeOfRq, NULL))
		{
			// "post2, query content type %s\n"
			EasyLog::Format(GetLibStrById(503), buff);
		}
		else
		{
			// "post2, query content type fails, error %d\n"
			EasyLog::Format(GetLibStrById(504), GetLastError());
			break;
		}

		// 开始读取文件
		if (ptrInternetReadFile(hReq, szBuffer, sizeof(szBuffer), &dwLen))
		{
			if (dwLen > 0)
			{
				strncpy(response, (const char*)szBuffer,
					dwLen > max_response_size ? max_response_size : dwLen);
			}
		}
	}
	while(false);

	if (hInt != NULL) ptrInternetCloseHandle(hInt);
	if (hConn != NULL) ptrInternetCloseHandle(hConn);
	if (hReq != NULL) ptrInternetCloseHandle(hReq);

	return (dwRetCode == 200 && dwLen > 0);
}

VOID GetRandomParameter2(std::string& szRandParameter)
{
	int r = rand();
	int count = r % 17;
	for (int i = 0; i < count; i++)
	{
		r = rand();
	}

	char buffer[MAX_PATH] = {0};
	sprintf(buffer, "&dummy=%d", r);

	szRandParameter = buffer;
}

// Assume that url and url2 has "?v=0" or other parameter
BOOL HTTPDownloadFileFromUrls2(const char* file_path, const char* url, const char* url2)
{
	BOOL bResult = FALSE;
	BOOL rc = FALSE;

	// Randomize to avoid http cache
	std::string szRandParameter;
	GetRandomParameter2(szRandParameter);	
	std::string szUrl = url;
	std::string szUrl2 = url2;
	szUrl += szRandParameter;
	szUrl2 += szRandParameter;

	do
	{
		if (url != NULL)
		{
			rc = HTTPDownloadFile2(szUrl.c_str(), file_path);
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
			rc = HTTPDownloadFile2(szUrl2.c_str(), file_path);
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

BOOL HTTPPostToUrls2(
	const char* body, size_t body_size,
	char* response, size_t max_response_size,
	const char* url, const char* url2)
{
	BOOL bResult = FALSE;
	BOOL rc = FALSE;

	// Randomize to avoid http cache
	std::string szRandParameter;
	GetRandomParameter2(szRandParameter);	
	std::string szUrl = url;
	std::string szUrl2 = url2;
	szUrl += szRandParameter;
	szUrl2 += szRandParameter;

	do
	{
		if (url != NULL)
		{
			memset(response, 0, max_response_size);
			rc = HTTPPost2(szUrl.c_str(), body, body_size, response, max_response_size);
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
			rc = HTTPPost2(szUrl2.c_str(), body, body_size, response, max_response_size);
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

BOOL HTTPReportInstallation2(const char* url, const char* url2, const char* szInstallName)
{
	// "report_key" key
	std::string installNameKey = GetLibStrById(562);
	std::string installNameValue = szInstallName;
	std::string body = installNameKey + "=" + installNameValue;
	char response[MAX_PATH] = {0};

	return HTTPPostToUrls2(body.c_str(), body.size(), response, sizeof response, url, url2);
}