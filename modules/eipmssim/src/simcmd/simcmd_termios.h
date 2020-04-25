#ifndef __SIMCMD_TERM_H__
#define __SIMCMD_TERM_H__


#include <term.h>
#include <termios.h>


/** @brief termios type */
typedef struct termios simcmd_termios_t;


/** @brief ���� �͹̳� ���� ����(termios)�� ��ȸ�ϴ� �Լ� */
simcmd_termios_t* simcmd_termios_get(simcmd_termios_t *t);
/** @brief �͹̳� ���� ����(termios)�� �����ϴ� �Լ� */
void simcmd_termios_set(simcmd_termios_t *t);

/** @brief echo ���� ��ȸ �Լ�. */
int simcmd_termios_get_echo_status( simcmd_termios_t *t );
/** @brief termios�� echo mode off �����ϴ� �Լ� */
void simcmd_termios_set_echo_off( simcmd_termios_t *t );
/** @brief termios�� echo mode on �����ϴ� �Լ� */
void simcmd_termios_set_echo_on( simcmd_termios_t *t );

/** @brief canonical mode ���� ��ȸ �Լ�. */
int simcmd_termios_get_canonical_status( simcmd_termios_t *t );
/** @brief termios�� canonical mode �����ϴ� �Լ� */
void simcmd_termios_set_canonical_on( simcmd_termios_t *t );
/** @brief termios�� non-canonical mode �����ϴ� �Լ� */
void simcmd_termios_set_canonical_off( simcmd_termios_t *t );

/** @brief minimum input byte ���� ��ȸ �Լ� */
int simcmd_termios_get_vmin( simcmd_termios_t *t );
/** @brief termios�� minimum input byte ���� ���� �Լ� */
void simcmd_termios_set_vmin( simcmd_termios_t *t, unsigned char vmin );
/** @brief wait input time ���� ��ȸ �Լ� */
int simcmd_termios_get_vtime( simcmd_termios_t *t );
/** @brief termios�� wait input time ���� ���� �Լ� */
void simcmd_termios_set_vtime( simcmd_termios_t *t, unsigned char vtime );

/** @brief �͹̳��� column, row size�� ��ȸ�ϴ� �Լ� */
int simcmd_termios_get_window_size( int *col, int *row );

/** @brief ȭ���� �ʱ�ȭ�ϴ� �Լ� (clear ��ɾ�� ������ ��� ����) */
void simcmd_termios_clear_screen();


#endif //#ifndef __SIMCMD_TERM_H__
