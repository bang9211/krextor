#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <ua_log.h>
#include "ipmssim_util.h"


//////////////////////////////////////////////////////////////////////////////////////////////
// public functions for ipmssim utilities 
//////////////////////////////////////////////////////////////////////////////////////////////

size_t strlcpy(char* dst, const char* src, size_t dstsize)
{
	register char *d = dst;
	register const char *s = src;
	register size_t len = dstsize;

	if( len && --len) {
		do {
			if( (*d++ = *s++) == 0) break;
		} while( --len);
	}
	if( !len) {
		if( dstsize) *d = '\0';
		while( *s++);
	}

	return (s - src - 1);

}


char *ipmssim_get_hostname()
{
	static char host_name[128] = {0, };

	if( host_name[0] == '\0' ) {
		gethostname(host_name, sizeof(host_name));
	}
	return host_name;
}

char* ipmssim_trim( char *buffer )
{
	while( *buffer <= ' ' && *buffer ) buffer++;
	int end = strlen(buffer)-1;
	while( end >= 0 && buffer[end] <= ' ') buffer[end--] = '\0';
	return buffer;
}


unsigned long ntohll( unsigned long val )
{
#if defined(LINUX)
/** @brief 64BIT �����͸� SWAP�Ѵ�. */
#define _SWAP64(x) \
	((((x) & 0xff00000000000000ull) >> 56)                 \
	| (((x) & 0x00ff000000000000ull) >> 40)                \
	| (((x) & 0x0000ff0000000000ull) >> 24)                \
	| (((x) & 0x000000ff00000000ull) >> 8)                 \
	| (((x) & 0x00000000ff000000ull) << 8)                 \
	| (((x) & 0x0000000000ff0000ull) << 24)                \
	| (((x) & 0x000000000000ff00ull) << 40)                \
	| (((x) & 0x00000000000000ffull) << 56))
	return _SWAP64(val);
#else
	return val;
#endif
}

unsigned long htonll( unsigned long val )
{
	return ntohll( val );
}


long ipmssim_get_now_time_msec()
{
	struct timeval tv[1];
	gettimeofday(tv, 0);
	return ((tv->tv_sec * 1000) + (tv->tv_usec / 1000));
}

char *ipmssim_cvt_time_to_str( time_t time, int unit, const char *fmt, char *buf, int buflen )
{
	int len, i;
	time_t t;
	struct tm ltime;

	if( !buf || buflen < 0 ) return NULL;

	if( unit <= 0 ) unit = 1;
	t = time / unit;

	if( !fmt || fmt[0] == '\0' ) {
		len = strftime( buf, buflen, "%Y%m%d%H%M%S", localtime_r(&t, &ltime) );
	} else {
		len = strftime( buf, buflen, fmt, localtime_r(&t, &ltime) );
	}
	if( unit > 1 ) {
		t = unit; 
		for( i = 0 ; ; i ++ ) {
			t /= 10;
			if( t == 0 ) break;
		}
		len += snprintf( buf+len, buflen > len ? buflen-len : 0, "%0*d", i, (int)(time % unit) );
	}
	return buf;
}

char *ipmssim_cvt_number_to_str( long number, int unit, char *buf, int buflen )
{
	int len, i;
	long t;

	if( !buf || buflen < 0 ) return NULL;

	if( unit <= 0 ) unit = 1;
	t = number / unit;

	len = snprintf( buf, buflen, "%ld", t );
	if( unit > 1 ) {
		t = unit; 
		for( i = 0 ; ; i ++ ) {
			t /= 10;
			if( t == 0 ) break;
		}
		len += snprintf( buf+len, buflen > len ? buflen-len : 0, ".%0*d", i, (int)(number % unit) );
	}
	return buf;
}

int64_t ipmssim_cvt_str_to_int( const char *str, int unit )
{
	int idx = 0, dot_idx = -1;
	char val_s[256];
	int64_t val_i;
	
	if( !str ) return -1;

	if( unit <= 0 ) unit = 1;
	strlcpy( val_s, str, sizeof(val_s) );

	if( val_s[0] == '-' ) idx = 1;
	else if( val_s[0] == '+' ) idx = 1;

	// �Ҽ����� ���ڸ� Ȯ���Ѵ�.
	for( ; val_s[idx] ; idx ++ ) {
		if( val_s[idx] < '0' || val_s[idx] > '9' ) {
			if( val_s[idx] == '.' ) dot_idx = idx+1;
			val_s[idx] = '\0';
			break;
		}
	}
	// ���� �κ��� ���� ����Ѵ�.
	val_i = atoi(val_s) * unit;
	// �Ҽ��� ������ ���� ����Ѵ�. 
	if( dot_idx > 0 ) {
		for( idx = 0 ; pow(10,idx) < unit ; idx ++ ) {
			if( val_s[dot_idx+idx] < '0' || val_s[dot_idx+idx] > '9' ) {
				val_s[dot_idx+idx] = '0';
			}
		}
		val_s[dot_idx+idx] = '\0';
		val_i += atoi(val_s+dot_idx);
	}
	return val_i;
}


char* ipmssim_make_line_str( const char *title, char c, char *buf, int buflen )
{
	int len;

	if( buf ) buf[0] = '\0';
	if( !buf || buflen <= 0 ) return NULL;
	if( c <= ' ' ) c = '-';

	if( !title || title[0] == '\0' ) {
		memset(buf, c, buflen-1);
		buf[buflen-1] = '\0';
	} else {
		len = snprintf(buf, buflen, "%c%c%c%c%c << %s >> ", c, c, c, c, c, title);
		if( len < buflen ) memset(buf+len, c, buflen-len-1);
		buf[buflen-1] = '\0';
	}
	return buf;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// public functions for base64
//////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(__api_peek_vector__) 
# define __api_peek_vector__(m_cast,m_base,m_sign,m_offset) \
	((m_cast)(((unsigned char *)(m_base)) m_sign (size_t)(m_offset))) 
#endif 
#if !defined(api_peek_byte) 
# define api_peek_byte(m_base,m_offset) \
	(*__api_peek_vector__(unsigned char *,m_base,+,m_offset)) 
#endif 
#if !defined(api_poke_byte) 
# define api_poke_byte(m_base,m_offset,m_value) \
	(*__api_peek_vector__(unsigned char *,m_base,+,m_offset)) = (unsigned char)(m_value) 
#endif 

static unsigned long (__api_decode_base64__)(int s_character) 
{
	if((s_character) >= ((int)'a'))return((((unsigned long)(s_character)) - ((unsigned long)'a')) + 26lu); 
	else if((s_character) >= ((int)'A'))return(((unsigned long)(s_character)) - ((unsigned long)'A'));
	else if((s_character) >= ((int)'0'))return((((unsigned long)(s_character)) - ((unsigned long)'0')) + 52lu);
	else if((s_character) == ((int)'+'))return(62lu);
	else if((s_character) == ((int)'/'))return(63lu);
	return(0lu);
}

int (ipmssim_encode_base64)(const char * s_string, size_t s_length, char *dest, int destlen)
{
	static const unsigned char c_alpha_table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="};
	char * s_result = dest;
	size_t s_source_offset = (size_t)0, s_target_offset = (size_t)0;
	unsigned long s_value;
	int s_quad, s_trip, len;

	len = (((s_length + ((size_t)2)) / ((size_t)3)) << 2) + ((size_t)1);
	if ( destlen <= len ) {
		uaLog( L_INFO," buffer len(%d) <= base64 encoded len(%d)", destlen, len);
		return -1;
	}
	while(s_source_offset < s_length)
	{
		s_value = ((unsigned long)(api_peek_byte((void *)s_string, s_source_offset) & ((int)0xff))) << 8; 
		if((s_source_offset + ((size_t)1)) < s_length)
		{
			s_value |= (unsigned long)(api_peek_byte((void *)s_string, s_source_offset + ((size_t)1)) & ((int)0xff)); 
			s_trip = (int)1;
		} else s_trip = (int)0;

		s_value <<= 8;
		if((s_source_offset + ((size_t)2)) < s_length)
		{
			s_value |= (unsigned long)(api_peek_byte((void *)s_string, s_source_offset + ((size_t)2)) & ((int)0xff)); 
			s_quad = (int)1;
		} else s_quad = (int)0;

		api_poke_byte((void *)s_result, s_target_offset + ((size_t)3), (int)c_alpha_table[(s_quad == (int)1) ? (s_value & 0x3flu) : 64]);
		s_value >>= 6;
		api_poke_byte((void *)s_result, s_target_offset + ((size_t)2), (int)c_alpha_table[(s_trip == (int)1) ? (s_value & 0x3flu) : 64]);
		s_value >>= 6;
		api_poke_byte((void *)s_result, s_target_offset + ((size_t)1), (int)c_alpha_table[s_value & 0x3flu]);
		s_value >>= 6;
		api_poke_byte((void *)s_result, s_target_offset, (int)c_alpha_table[s_value & 0x3flu]);
		s_source_offset += (size_t)3, s_target_offset += (size_t)4;
	}
	api_poke_byte((void *)s_result, s_target_offset, (int)'\0');
	return 0;
}

char * (ipmssim_decode_base64)(const char * s_string, int *slen)
{
	char * s_result;
	size_t s_length = strlen(s_string), s_source_offset = (size_t)0, s_target_offset = (size_t)0;
	unsigned long s_value;

	s_result = (char *)malloc((((s_length + ((size_t)3)) >> 2) * ((size_t)3)) + ((size_t)1));
	if(s_result == ((char *)0))return((char *)0);
	while(s_source_offset < s_length)
	{
		s_value  = ((__api_decode_base64__(api_peek_byte((void *)s_string, s_source_offset)) & 0x3flu) << 18) |
		((__api_decode_base64__(api_peek_byte((void *)s_string, s_source_offset + ((size_t)1))) & 0x3flu) << 12) |
		((__api_decode_base64__(api_peek_byte((void *)s_string, s_source_offset + ((size_t)2))) & 0x3flu) << 6) |
		(__api_decode_base64__(api_peek_byte((void *)s_string, s_source_offset + ((size_t)3))) & 0x3flu);
		api_poke_byte((void *)s_result, s_target_offset++, (int)((s_value >> 16) & 0xfflu));
		if(api_peek_byte((void *)s_string, s_source_offset + ((size_t)2)) != ((int)'='))
		{
			api_poke_byte((void *)s_result, s_target_offset++, (int)((s_value >> 8) & 0xfflu));
			if(api_peek_byte((void *)s_string, s_source_offset + ((size_t)3)) != ((int)'='))api_poke_byte((void *)s_result, s_target_offset++, (int)(s_value & 0xfflu));
		} 
		s_source_offset += (size_t)4;
	}
	api_poke_byte((void *)s_result, s_target_offset, (int)'\0');
	*slen = s_target_offset;
	return(s_result);
}


