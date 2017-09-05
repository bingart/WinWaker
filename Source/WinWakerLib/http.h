
#ifndef HTTP_H
#define HTTP_H

BOOL HTTPDownloadFile(const char* url, const char* file_path);
BOOL HTTPGet(const char* url, char* response, size_t response_size);
BOOL HTTPPost(
	const char* url,
	const char* body, size_t body_size,
	char* response, size_t max_response_size);

BOOL HTTPDownloadFileFromUrls(const char* file_path, const char* url, const char* url2);
BOOL HTTPPostToUrls(
	const char* body, size_t body_size,
	char* response, size_t max_response_size,
	const char* url, const char* url2);

BOOL HTTPReport(const char* url, const char* url2, const char* szInstallName);

#endif // HTTP_H