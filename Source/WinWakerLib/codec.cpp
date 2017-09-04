
#include <windows.h>
#include <io.h>
#include <string>
#include "scrambler.h"
#include "base64.h"
#include "codec.h"
#include "easylog.hpp"
#include "libstr.h"
#include "key.h"

#define MAX_CODEC_LENGTH 20480000
#define MOD 13

BOOL Encode(const char* exe_file_path, const char* txt_file_path)
{
	FILE* in = NULL;
	FILE* out = NULL;
	int rc = -1;

	do
	{
		in = fopen(exe_file_path, "rb");
		if (in == NULL)
		{
			// "encode file fail, open exe file %s error\n"
			EasyLog::Format(GetLibStrById(401), exe_file_path);
			break;
		}

		out = fopen(txt_file_path, "w");
		if (out == NULL)
		{
			// "encode file fail, open txt file %s error\n"
			EasyLog::Format(GetLibStrById(402), txt_file_path);
			break;
		}

		// Write KEY at fist line
		// "%d\n"
		fprintf(out, GetLibStrById(552), KEY);

		char ibuffer[BLOCK_SIZE];
		char obuffer[2*BLOCK_SIZE];

		rc = 0;
		while (!feof(in) && rc >= 0)
		{
			memset(ibuffer, 0, sizeof ibuffer);
			memset(obuffer, 0, sizeof obuffer);

			size_t len = fread(ibuffer, 1, BLOCK_SIZE, in);
			if (len > 0)
			{
				// encrypt
				for (size_t i = 0; i < len; i++)
				{
					ibuffer[i] = ibuffer[i] ^ (i % MOD) ^ KEY;
				}

				// convert to base64
				rc = encode64(obuffer, sizeof obuffer, ibuffer, len);
				if (rc > 0)
				{
					// write into txt file
					// "%s\n"
					fprintf(out, GetLibStrById(403), obuffer);
				}
				else if (rc == 0)
				{
					// "encode file, encode64 rc %d\n"
					EasyLog::Format(GetLibStrById(404), rc);
				}
				else
				{
					// "encode file fails, encode64 rc %d < 0\n"
					EasyLog::Format(GetLibStrById(405), rc);
					break;
				}
			}
		}
	}
	while (FALSE);

	// close files
	if (in != NULL) fclose(in);
	if (out != NULL)
	{
		_commit(_fileno(out));
		fclose(out);
	}

	return rc >= 0;
}

BOOL Decode(const char* txt_file_path, const char* exe_file_path)
{
	FILE* in = NULL;
	FILE* out = NULL;
	char* cache = NULL;	// cache
	int total_length = 0;
	int key = 0;

	do
	{
		in = fopen(txt_file_path, "r");
		if (in == NULL)
		{
			// "decode file fail, open txt file %s error\n"
			EasyLog::Format(GetLibStrById(406), txt_file_path);
			break;
		}

		out = fopen(exe_file_path, "wb");
		if (out == NULL)
		{
			// "decode file fail, open exe file %s error\n"
			EasyLog::Format(GetLibStrById(407), exe_file_path);
			break;
		}

		char ibuffer[2*BLOCK_SIZE];
		char obuffer[2*BLOCK_SIZE];
		cache = new char [MAX_CODEC_LENGTH];

		// Read key from first line
		bool keyFound = false;
		if (!feof(in))
		{
			memset(ibuffer, 0, sizeof ibuffer);
			char* str = fgets(ibuffer, sizeof ibuffer, in);
			if (str != NULL)
			{
				size_t len2 = strlen(ibuffer);

				// remove the line feed
				ibuffer[len2 - 1] = '\0';

				key = atoi(ibuffer);
			}
		}

		if (key == 0)
		{
			// "decode fails, key not fould\n"
			EasyLog::Format(GetLibStrById(553));
		}

		while (!feof(in) && total_length < MAX_CODEC_LENGTH)
		{
			memset(ibuffer, 0, sizeof ibuffer);
			memset(obuffer, 0, sizeof obuffer);

			char* str = fgets(ibuffer, sizeof ibuffer, in);
			if (str != NULL)
			{
				size_t len2 = strlen(ibuffer);

				// remove the line feed
				ibuffer[len2 - 1] = '\0';

				// revert base64
				int rc = decode64(obuffer, sizeof obuffer, ibuffer, strlen(ibuffer));
				if (rc > 0)
				{
					memcpy(cache + total_length, obuffer, rc);
					total_length += rc;

					// Decrypt
					for (size_t i = 0; i < rc; i++)
					{
						obuffer[i] = obuffer[i] ^ (i % MOD) ^ (key & 0xff);
					}

					fwrite(obuffer, 1, rc, out);
				}
				else if (rc == 0)
				{
					// "decode file, decode64 rc %d\n"
					EasyLog::Format(GetLibStrById(408), rc);
				}
				else
				{
					// "decode file fails, decode64 rc %d < 0\n"
					EasyLog::Format(GetLibStrById(409), rc);
					break;
				}
			}
		}
	}
	while (FALSE);

	if (cache != NULL)
	{
		delete []cache;
	}

	// close files
	if (in != NULL) fclose(in);
	if (out != NULL)
	{
		fflush(out);
		_commit(_fileno(out));
		fclose(out);
	}

	return total_length > 0 && total_length < MAX_CODEC_LENGTH;
}

BOOL DecodePHP(std::string szSrcLine, std::string& szDstLine)
{
	char buffer[1024] = {0};
	char buffer2[1024] = {0};

	// Decode base64
	memset(buffer, 0, sizeof buffer);
	int rc = decode64(buffer, sizeof buffer, szSrcLine.c_str(), szSrcLine.size());
	if (rc <= 6)
	{
		// prefix + suffix
		return FALSE;
	}

	// Remove prefix and suffix
	memset(buffer2, 0, sizeof buffer2);
	memcpy(buffer2, buffer + 3, rc - 6);

	// Decode base64
	memset(buffer, 0, sizeof buffer);
	rc = decode64(buffer, sizeof buffer, buffer2, rc - 6);
	if (rc <= 6)
	{
		// prefix + suffix
		return FALSE;
	}

	// Remove prefix and suffix
	memset(buffer2, 0, sizeof buffer2);
	memcpy(buffer2, buffer + 3, rc - 6);

	// Decode base64
	memset(buffer, 0, sizeof buffer);
	rc = decode64(buffer, sizeof buffer, buffer2, rc - 6);
	if (rc <= 0)
	{
		// prefix + suffix
		return FALSE;
	}

	// Output
	szDstLine = buffer;

	return TRUE;
}