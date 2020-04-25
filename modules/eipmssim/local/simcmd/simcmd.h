#ifndef __SIMCMD_H__
#define __SIMCMD_H__


#include "simcmd_errno.h"
#include "simcmd_history.h"
#include "simcmd_register.h"
#include "simcmd_readchar.h"
#include "simcmd_readline.h"
#include "simcmd_termios.h"
#include "simcmd_util.h"


/** @brief simlator command ���� ����� �����Ѵ�. Readline ��带 �������� 
           �����ϰ� ������ simcmd_set_type() �Լ��� �̿��Ͽ� ������ �� �ִ�. */
#define SIMCMD_TYPE_RL	0	// readline mode
#define SIMCMD_TYPE_RC	1	// readchar mode


/** @brief simuator command manager �ʱ�ȭ �Լ� */
int simcmd_init( char *prompt, int timeout_msec );
/** @brief simulator command type ���� �Լ� */ 
int simcmd_set_type( int type );
/** @brief simulator command type ��ȸ �Լ� */
int simcmd_get_type( void );
/** @brief simulator command ��� �Լ� */
int simcmd_register( int type, char *name, simcmd_reg_t *reg );
/** @breif simulator command ���� �� command ���� �Լ� */
int simcmd_readline();

#endif //#ifndef __SIMCMD_H__
