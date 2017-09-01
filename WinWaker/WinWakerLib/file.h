
#ifndef FILE_H
#define FILE_H

#include <string>
#include <list>

extern std::string userTempPath;
extern std::string windowsTempPath;
extern std::string timeString;

BOOL IsFileExists(const char* file_name);

BOOL IsFileExists(const char* file_name, int file_size);

VOID GetUserTempPath();

VOID GetWindowsTempPath();
VOID GetTimeString();

// Check if file creation time expired, i.e. now - file's creation time > expired_time 
BOOL IsFileExpired(const char* file_path, int expired_time);

BOOL RemoveTempFiles(std::list<std::string> deleteFilePathList);

#endif // FILE_H