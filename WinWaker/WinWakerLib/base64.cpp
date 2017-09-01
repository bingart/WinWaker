
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "scrambler.h"
#include "base64.h"

static const char *const base64_encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64Enc(char *buf, const unsigned char* text, int size) 
{ 
	int buflen = 0; 
	
	while (size > 0)
	{
		*buf++ = base64_encoding[ (text[0] >> 2 ) & 0x3f];
		if(size>2)
		{
			*buf++ = base64_encoding[((text[0] & 0x03) << 4) | (text[1] >> 4)];
			*buf++ = base64_encoding[((text[1] & 0x0F) << 2) | (text[2] >> 6)];
			*buf++ = base64_encoding[text[2] & 0x3F];
		}
		else
		{
			switch(size)
			{
			case 1:
				*buf++ = base64_encoding[(text[0] & 3) << 4 ];
				*buf++ = '=';
				*buf++ = '=';
				break;
			case 2: 
				*buf++ = base64_encoding[((text[0] & 0x03) << 4) | (text[1] >> 4)]; 
				*buf++ = base64_encoding[((text[1] & 0x0F) << 2) | (text[2] >> 6)]; 
				*buf++ = '='; 
				break; 
			} 
		} 
		
		text += 3; 
		size -= 3; 
		buflen += 4; 
	} 
	
	*buf = '\0'; 
	return buflen; 
} 

char GetBase64Value(char ch)
{
	if ((ch >= 'A') && (ch <= 'Z')) 
		return ch - 'A'; 
	if ((ch >= 'a') && (ch <= 'z')) 
		return ch - 'a' + 26; 
	if ((ch >= '0') && (ch <= '9')) 
		return ch - '0' + 52; 
	switch (ch) 
	{ 
	case '+': 
		return 62; 
	case '/': 
		return 63; 
	case '=': /* base64 padding */ 
		return 0; 
	default: 
		return 0; 
	} 
}

//进行base64解码输入应该是4的倍数(根据mime标准)
//如果不是4倍数返回错误
//注意 如果是最后一个字符 那么长度不准备 可能会多1 
//返回buf长度
// 2006.04.13 check '=' '==' padding
int Base64Dec(unsigned char *buf, const char *text, int size) 
{
	unsigned char chunk[4];
	int parsenum = 0;

	if (size == 0)
	{
		return 0;
	}

	if (size < 0)
	{
		return -1;
	}
	
	if (size % 4 != 0)
	{
		return -1;
	}

	if (text[size-1] == '=')
	{
		--parsenum;
	}

	if (text[size-2] == '=')
	{
		--parsenum;
	}

	//printf("go here, parsenum=%d\n", parsenum);

	while (size > 0)
	{
		chunk[0] = GetBase64Value(text[0]); 
		chunk[1] = GetBase64Value(text[1]); 
		chunk[2] = GetBase64Value(text[2]); 
		chunk[3] = GetBase64Value(text[3]); 
		
		*buf++ = (chunk[0] << 2) | (chunk[1] >> 4); 
		*buf++ = (chunk[1] << 4) | (chunk[2] >> 2); 
		*buf++ = (chunk[2] << 6) | (chunk[3]);
		
		text += 4;
		size -= 4;
		parsenum += 3;
	}

	*buf = 0;
	
	return parsenum;
} 

int encode64(char* dst, size_t max, const void* src, size_t len)
{
	if (max <= (len + 2) / 3 * 4)
	{
		return -1;
	}
	return Base64Enc(dst, (const unsigned char*) src, (int) len); 
}

int decode64(void* dst, size_t max, const char* src, size_t len)
{
	if (max <= (len + 3) / 4 * 3)
	{
		return -1;
	}
	return Base64Dec((unsigned char*) dst, src, (int) len);
}
