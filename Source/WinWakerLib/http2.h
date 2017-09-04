#ifndef HTTP2_H
#define HTTP2_H

BOOL HTTPInit();
VOID HTTPFini();
BOOL HTTPDownloadFile2(const char* url, const char* file_path);
BOOL HTTPGet2(const char* url, char* response, size_t response_size);
BOOL HTTPPost2(
	const char* url,
	const char* body, size_t body_size,
	char* response, size_t max_response_size);

BOOL HTTPDownloadFileFromUrls2(const char* file_path, const char* url, const char* url2);
BOOL HTTPPostToUrls2(
	const char* body, size_t body_size,
	char* response, size_t max_response_size,
	const char* url, const char* url2);

BOOL HTTPReportInstallation2(const char* url, const char* url2, const char* szInstallName);

#endif // HTTP2_H