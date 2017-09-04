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
/// Base64��MIME�ʼ��г��õı��뷽ʽ֮һ��
/// ������Ҫ˼���ǽ�������ַ��������ݱ����ֻ����{'A'-'Z', 'a'-'z', '0'-'9', '+', '/'}
/// ��64���ɴ�ӡ�ַ��Ĵ����ʳ�Ϊ��Base64���� <br>
/// Base64����ķ����ǣ�������������ÿ��ȡ6 bit���ô�6 bit��ֵ(0-63)��Ϊ����ȥ��������Ӧ�ַ���
/// ������ÿ3���ֽڽ�����Ϊ4���ַ�(3��8 �� 4��6)������4���ַ�����'='��䡣 
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

