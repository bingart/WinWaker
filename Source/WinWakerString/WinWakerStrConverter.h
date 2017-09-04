
#ifndef WINWAKER_STR_CONVERTER_H
#define WINWAKER_STR_CONVERTER_H

#include <Windows.h>

class WinWakerStrConverter  
{
public:
	BOOL Convert(const char* txtFilePath, const char* cppFilePath, bool isLib);
};

#endif