#ifndef __SIMCMD_HISTORY_H__
#define __SIMCMD_HISTORY_H__


#include <stdio.h>
#include <readline/history.h>


/** @brief �Է��� ��ɾ �ϼ���Ų��. result code : 0 ����, -1 : ���� Ȥ�� just print */
int simcmd_history_expand( char **line );

/** @brief ��ɾ history ����Ʈ�� �߰��Ѵ�. */
void simcmd_history_add( char *line );

/** @brief �Է��� ������ŭ�� history�� �����ϰ� �����Ѵ�. */
void simcmd_history_truncate( int max );

/** @brief history�̷��� ȭ�鿡 ����Ѵ�. */
void simcmd_history_print();


#endif //#ifndef __SIMCMD_HISTORY_H__
