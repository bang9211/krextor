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

/** @brief ���ڿ��� �����ϴµ�, ���޵� ũ�� ������ ����ȴ�. */
size_t simctrl_strlcpy(char* dst, const char* src, size_t dstsize);
/** @brief ���ڿ��� ���Ե� Ư�� ���ڸ� �������ش�. */
char* simctrl_replace_str( char *src, char *search, char *changed, char *dst, int len );
/** brief time_t �� ���� format�� �°� string���� ��ȯ���ش�. */
char* simctrl_cvt_time_to_str( time_t time, char *fmt, char *buf, int buflen);
/** @brief ���޵� �Ķ���Ϳ� environment ���� ���� �� ���� �н��� ������ ��ȯ�Ѵ�. */
int simctrl_get_conf_path( const char *path, char *cpath, int size);

/** @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� file open�Ѵ�. */
FILE *simctrl_fopen( char *fname, char *mode );
/** @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� file�� ���� ������ dump�Ѵ�. */
char* simctrl_fdump( char *fname );
/** @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� file�� remove�Ѵ�. */
int simctrl_remove( char *fname );

/** @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� directory�� �����Ѵ�. */
int simctrl_mkdir( char *fname );
/** @brief file name�� ������ ��ο� ���� env ��ȯ �� directory�� �����Ѵ�. */
int simctrl_mkdir_with_file( char *fname );

/** @brief ���޵� chracter�� ���� ���̸�ŭ ���ۿ� ����ϰ� '\n'�� �Է��Ѵ�. */
int simctrl_sprintf_line( char *buf, int buflen, char c);
/** @brief ���޵� chracter�� ���� ���̸�ŭ ȭ�鿡 ����ϰ� '\n'�� ����Ѵ�. */
void simctrl_display_line( char c);

/** @brief ���޵� �Ķ���Ϳ� Ư����ȣ�� ��ȯ�Ѵ�. skip �Ǵ� '.'���� ��ü���ش�. */
char* simctrl_decode_ascii( char *buf );

/** @brief process���� kill signal�� ���� �����Ų��. */
void simctrl_kill( int pid, int kill_child_flag );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_UTIL_H__
