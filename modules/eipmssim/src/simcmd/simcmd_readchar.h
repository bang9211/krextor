#ifndef __SIMCMD_READCHAR_H__
#define __SIMCMD_READCHAR_H__


/** @brief readline manager �ʱ�ȭ �Լ�. ���� ���� �� callback function �ʱ�ȭ */
int simcmd_rc_init( char* prompt, int timeout_msec );
/** @breif simulator command ���� �� command ���� �Լ� */
int simcmd_rc_readchar();


#endif //#ifndef __SIMCMD_READCHAR_H__
