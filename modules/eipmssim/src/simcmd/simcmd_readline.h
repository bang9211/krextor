#ifndef __SIMCMD_READLINE_H__
#define __SIMCMD_READLINE_H__


#include <readline/readline.h>


/** @brief readline manager �ʱ�ȭ �Լ�. ���� ���� �� callback function �ʱ�ȭ */
int simcmd_rl_init( char* prompt, int timeout_msec );
/** @breif simulator command ���� �� command ���� �Լ� */
int simcmd_rl_readline();


#endif //#ifndef __SIMCMD_READLINE_H__
