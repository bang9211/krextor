#ifndef __IPMSSIM_UTIL_H__
#define __IPMSSIM_UTIL_H__


size_t strlcpy(char* dst, const char* src, size_t dstsize);

char *ipmssim_get_hostname();
char* ipmssim_trim( char *buffer );

unsigned long ntohll( unsigned long val );
unsigned long htonll( unsigned long val );

long ipmssim_get_now_time_msec();
char *ipmssim_cvt_time_to_str( time_t time, int unit, const char *fmt, char *buf, int buflen );
char *ipmssim_cvt_number_to_str( long number, int unit, char *buf, int buflen );
int64_t ipmssim_cvt_str_to_int( const char *str, int unit );

char* ipmssim_make_line_str( const char *title, char c, char *buf, int buflen );

int (ipmssim_encode_base64)(const char * s_string, size_t s_length, char *dest, int destlen);
char * (ipmssim_decode_base64)(const char * s_string, int *slen);


#endif // #ifndef __IPMSSIM_UTIL_H__
