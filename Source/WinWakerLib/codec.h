
#ifndef CODEC_H
#define CODEC_H

#include <Windows.h>

#define BLOCK_SIZE 64

BOOL Encode(const char* exe_file_path, const char* txt_file_path);
BOOL Decode(const char* txt_file_path, const char* exe_file_path);

BOOL DecodePHP(std::string szSrcLine, std::string& szDstLine);

#endif // CODEC_H