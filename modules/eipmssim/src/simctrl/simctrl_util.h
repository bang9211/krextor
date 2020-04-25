#ifndef __SIMCTRL_UTIL_H__
#define __SIMCTRL_UTIL_H__


#include <sys/types.h>


#ifdef __cplusplus 
extern "C" {
#endif 


/////////////////////////////////////////////////////////////////////////////////
// struct & define for simulator controller - utilities 
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - utilities 
/////////////////////////////////////////////////////////////////////////////////

/** @brief 문자열을 복사하는데, 전달된 크기 내에서 복사된다. */
size_t simctrl_strlcpy(char* dst, const char* src, size_t dstsize);
/** @brief 문자열에 포함된 특정 문자를 변경해준다. */
char* simctrl_replace_str( char *src, char *search, char *changed, char *dst, int len );
/** brief time_t 의 값을 format에 맞게 string으로 변환해준다. */
char* simctrl_cvt_time_to_str( time_t time, char *fmt, char *buf, int buflen);
/** @brief 전달된 파라미터에 environment 값이 있을 때 절대 패스로 변경해 반환한다. */
int simctrl_get_conf_path( const char *path, char *cpath, int size);

/** @brief 전달된 파라미터에 environment를 절대경로로 변환한 후 file open한다. */
FILE *simctrl_fopen( char *fname, char *mode );
/** @brief 전달된 파라미터에 environment를 절대경로로 변환한 후 file을 열어 내용을 dump한다. */
char* simctrl_fdump( char *fname );
/** @brief 전달된 파라미터에 environment를 절대경로로 변환한 후 file을 remove한다. */
int simctrl_remove( char *fname );

/** @brief 전달된 파라미터에 environment를 절대경로로 변환한 후 directory를 생성한다. */
int simctrl_mkdir( char *fname );
/** @brief file name을 포함한 경로에 대해 env 변환 후 directory를 생성한다. */
int simctrl_mkdir_with_file( char *fname );

/** @brief 전달된 chracter를 일정 길이만큼 버퍼에 기록하고 '\n'을 입력한다. */
int simctrl_sprintf_line( char *buf, int buflen, char c);
/** @brief 전달된 chracter를 일정 길이만큼 화면에 출력하고 '\n'을 출력한다. */
void simctrl_display_line( char c);

/** @brief 전달된 파라미터에 특수기호를 변환한다. skip 또는 '.'으로 대체해준다. */
char* simctrl_decode_ascii( char *buf );

/** @brief process에게 kill signal을 보내 종료시킨다. */
void simctrl_kill( int pid, int kill_child_flag );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_UTIL_H__
