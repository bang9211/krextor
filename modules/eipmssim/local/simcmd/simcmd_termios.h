#ifndef __SIMCMD_TERM_H__
#define __SIMCMD_TERM_H__


#include <term.h>
#include <termios.h>


/** @brief termios type */
typedef struct termios simcmd_termios_t;


/** @brief 현재 터미널 상태 정보(termios)를 조회하는 함수 */
simcmd_termios_t* simcmd_termios_get(simcmd_termios_t *t);
/** @brief 터미널 상태 정보(termios)를 변경하는 함수 */
void simcmd_termios_set(simcmd_termios_t *t);

/** @brief echo 상태 조회 함수. */
int simcmd_termios_get_echo_status( simcmd_termios_t *t );
/** @brief termios에 echo mode off 설정하는 함수 */
void simcmd_termios_set_echo_off( simcmd_termios_t *t );
/** @brief termios에 echo mode on 설정하는 함수 */
void simcmd_termios_set_echo_on( simcmd_termios_t *t );

/** @brief canonical mode 상태 조회 함수. */
int simcmd_termios_get_canonical_status( simcmd_termios_t *t );
/** @brief termios에 canonical mode 설정하는 함수 */
void simcmd_termios_set_canonical_on( simcmd_termios_t *t );
/** @brief termios에 non-canonical mode 설정하는 함수 */
void simcmd_termios_set_canonical_off( simcmd_termios_t *t );

/** @brief minimum input byte 정보 조회 함수 */
int simcmd_termios_get_vmin( simcmd_termios_t *t );
/** @brief termios에 minimum input byte 정보 설정 함수 */
void simcmd_termios_set_vmin( simcmd_termios_t *t, unsigned char vmin );
/** @brief wait input time 정보 조회 함수 */
int simcmd_termios_get_vtime( simcmd_termios_t *t );
/** @brief termios에 wait input time 정보 설정 함수 */
void simcmd_termios_set_vtime( simcmd_termios_t *t, unsigned char vtime );

/** @brief 터미널의 column, row size를 조회하는 함수 */
int simcmd_termios_get_window_size( int *col, int *row );

/** @brief 화면을 초기화하는 함수 (clear 명령어와 동일한 기능 제공) */
void simcmd_termios_clear_screen();


#endif //#ifndef __SIMCMD_TERM_H__
