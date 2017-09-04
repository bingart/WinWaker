
#ifndef STR_H
#define STR_H

int EncryptString(char* rawStr, size_t rawLen, char* encryptStr, size_t maxEncryptSize);
int DecryptString(const char* encryptStr, size_t encryptLen, char* rawStr, size_t maxRawSize);

#endif