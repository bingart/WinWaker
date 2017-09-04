#include <ctime>
#include <io.h>
#include <Windows.h>
#include <time.h>
#include "WorkLog.h"

namespace
{
	char filename_[MAX_PATH] = {0};
}

FILE* WorkLog::fp_ = stdout;
bool WorkLog::flush_ = true;
bool WorkLog::enabled_ = false;

void WorkLog::SetSingleMode(const char* filename)
{
	if (fp_)
	{
		fclose(fp_);
	}

	fp_ = fopen(filename, "w");
	if (fp_)
	{
		strcpy(filename_, filename);
	}
}

void WorkLog::Close()
{
	if (fp_)
	{
		fflush(fp_);
		_commit(_fileno(fp_));
		fclose( fp_ );

		fp_ = NULL;
	}
}

void WorkLog::Format(const char* format, ...)
{
	if (!enabled_)
	{
		return;
	}

	if (fp_ == NULL)
	{
		return;
	}

    time_t	now;
	time(&now);
	struct tm* tm;
	tm = localtime(&now);
	
	char header[64] = {0};
	char body[2048] = {0};

	sprintf_s(header, "S %d %d %02d:%02d:%02d :",
		GetCurrentProcessId(), GetCurrentThreadId(), tm->tm_hour, tm->tm_min, tm->tm_sec );

	va_list ap;
	va_start(ap, format);
	vsprintf_s(body, format, ap);
	va_end(ap);

	fwrite(header, 1, strlen(header), fp_);
	fwrite(body, 1, strlen(body), fp_);

	if(flush_)
	{
		if (fp_ != NULL)
		{
			fflush(fp_);
			_commit(_fileno(fp_));
		}
	}
}
