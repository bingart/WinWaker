#ifndef WORKLOG_HPP
#define WORKLOG_HPP

#include <cstdio>
#include <stdarg.h>

class WorkLog
{
public:
	static void Format(const char* format, ...);
	static void SetSingleMode(const char* filename);
	static void Close();
	static void SetFlush(bool flush) { flush_ = flush; }
	static void SetEnabled(bool enabled) { enabled_ = enabled; }
private:
	static FILE* fp_;
	static bool flush_;
	static bool enabled_;
};

#endif

