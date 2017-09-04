
#include "str.h"
#include <string.h>
#include "scrambler.h"
#include "base64.h"
#include "key.h"

int EncryptString(char* rawStr, size_t rawLen, char* encryptStr, size_t maxEncryptSize)
{
	// Clear
	memset(encryptStr, 0, maxEncryptSize);

	// XOR
	for (size_t s = 0; s < rawLen; s++)
	{
		rawStr[s] = rawStr[s] ^ KEY;
	}

	// BASE64
	return encode64(encryptStr, maxEncryptSize, rawStr, rawLen);
}

int DecryptString(const char* encryptStr, size_t encryptLen, char* rawStr, size_t maxRawSize)
{
	// Clear
	memset(rawStr, 0, maxRawSize);

	// BASE64
	int rc = decode64(rawStr, maxRawSize ,encryptStr, encryptLen);
	if (rc <= 0)
	{
		return rc;
	}

	// XOR
	for (int i = 0; i < rc; i++)
	{
		rawStr[i] = rawStr[i] ^ KEY;
	}

	return rc;
}
