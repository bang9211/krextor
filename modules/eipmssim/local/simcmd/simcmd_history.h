#ifndef __SIMCMD_HISTORY_H__
#define __SIMCMD_HISTORY_H__


#include <stdio.h>
#include <readline/history.h>


/** @brief 입력한 명령어를 완성시킨다. result code : 0 성공, -1 : 실패 혹은 just print */
int simcmd_history_expand( char **line );

/** @brief 명령어를 history 리스트에 추가한다. */
void simcmd_history_add( char *line );

/** @brief 입력한 개수만큼의 history를 제외하고 삭제한다. */
void simcmd_history_truncate( int max );

/** @brief history이력을 화면에 출력한다. */
void simcmd_history_print();


#endif //#ifndef __SIMCMD_HISTORY_H__
