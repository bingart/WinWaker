#ifndef BASE64_H
#define BASE64_H

#include <stddef.h>

///
/// @file base64.h
/// @brief Operation for Base64.
/// Base 64 or quadrosexagesimal is a positional notation using a base of 64.
/// It is the largest power-of-two base that can be represented using only printable ASCII characters.
/// This has led to its use as a transfer encoding for e-mail among other things.
/// All well-known variants that are known by the name Base64 use the characters A-Z, a-z, and 0-9
/// in that order for the first 62 digits but the symbols chosen for the last two digits 
/// vary considerably between different systems. Several other encoding methods 
/// such as uuencode and later versions of binhex use a different set of 64 characters 
/// to represent 6 binary digits, but these are never called by the name Base64.<br>
/// Base64是MIME邮件中常用的编码方式之一。
/// 它的主要思想是将输入的字符串或数据编码成只含有{'A'-'Z', 'a'-'z', '0'-'9', '+', '/'}
/// 这64个可打印字符的串，故称为“Base64”。 <br>
/// Base64编码的方法是，将输入数据流每次取6 bit，用此6 bit的值(0-63)作为索引去查表，输出相应字符。
/// 这样，每3个字节将编码为4个字符(3×8 → 4×6)；不满4个字符的以'='填充。 
///

/// @brief encode a string to base64 string.
/// @param dst - pointer of destination buffer
/// @param max - max size of destination buffer
/// @param src - pointer of source data
/// @param len - size of source data
/// @retval -1 failure
/// @retval >=0 the length after encoded
int encode64(char* dst, size_t max, const void* src, size_t len);

/// @brief decode a string from base64 string.
/// @param dst - pointer of destination buffer
/// @param max - max size of destination buffer
/// @param src - pointer of source data
/// @param len - size of source data
/// @retval -1 failure
/// @retval >=0 the length after decoded
int decode64(void* dst, size_t max, const char* src, size_t len);

#endif

