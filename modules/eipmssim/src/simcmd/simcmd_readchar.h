#ifndef __SIMCMD_READCHAR_H__
#define __SIMCMD_READCHAR_H__


/** @brief readline manager 초기화 함수. 설정 정보 및 callback function 초기화 */
int simcmd_rc_init( char* prompt, int timeout_msec );
/** @breif simulator command 수집 및 command 싱행 함수 */
int simcmd_rc_readchar();


#endif //#ifndef __SIMCMD_READCHAR_H__
