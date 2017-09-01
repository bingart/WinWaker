#include <ctime>
#include <io.h>
#include <Windows.h>
#include <time.h>
#include "scrambler.h"
#include "easylog.hpp"
#include "libstr.h"

namespace
{
	char filename_[MAX_PATH] = {0};
}

FILE* EasyLog::fp_ = NULL;
bool EasyLog::flush_ = true;
bool EasyLog::enabled_ = false;

void EasyLog::SetSingleMode(const char* filename)
{
	if (fp_)
	{
		Close();
	}

	fp_ = fopen(filename, "a");
	if (fp_)
	{
		strcpy(filename_, filename);
	}
}

void EasyLog::Close()
{
	if (fp_)
	{
		fflush(fp_);
		_commit(_fileno(fp_));
		fclose( fp_ );
		fp_ = NULL;
	}
}

void EasyLog::Format(const char* format, ...)
{
	if (!enabled_)
	{
		return;
	}

	if (!fp_)
	{
		return;
	}

    time_t	now;
	time(&now);
	struct tm* tm;
	tm = localtime(&now);
	
	char header[64] = {0};
	char body[2048] = {0};

	// "S %d %d %02d:%02d:%02d :"
	sprintf_s(header, GetLibStrById(551),
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
