#ifndef __SIMCMD_UTIL_H__
#define __SIMCMD_UTIL_H__


#include <time.h>


//////////////////////////////////////////////////////////////////////////////
// Utilities - Process Rate 
//////////////////////////////////////////////////////////////////////////////

/** @brief 진행률 화면 출력 모드 - Percentage, Process Rate and Remains Time */
#define SIMCMD_RATE_MODE_PERCENTAGE		0x0001
#define SIMCMD_RATE_MODE_PROCRATE		0x0002
#define SIMCMD_RATE_MODE_REMAINSTIME	0x0004
#define SIMCMD_RATE_MODE_ALL			(SIMCMD_RATE_MODE_PERCENTAGE | \
		SIMCMD_RATE_MODE_PROCRATE | SIMCMD_RATE_MODE_REMAINSTIME)


/** @brief process rate manager type */
typedef struct simcmd_rate_s simcmd_rate_t;
/** @brief process rate manager struct */
struct simcmd_rate_s {
	char stop_flag; /**< process rate 종료여부 */
	unsigned long total; /**< 전체 진행해야할 process 개수 */
	unsigned short column; /**< process rate 진행 표시 column size */
	unsigned int mode; /**< display mode. percentage|process_rate|remains_time */
	time_t stime; /**< start time */
	unsigned long curr_cnt; /**< current process complete count */
	unsigned short curr_rate; /**< current process complete rate */
	time_t ctime; /**< current time */
	unsigned long remains; /**< rememains time - 남은 시간 정보 */
	unsigned long remains_cnt; /**< remains time - 이전 체크했을 때 당시 process complete count */
	time_t rtime; /**< remains time - 이전 체크했을 당시 시간 */
};


/** @brief 진행률 화면 표시 기능 - 초기화 함수 */
int simcmd_rate_init(simcmd_rate_t *rate, unsigned long total, 
		unsigned short column, unsigned int mode);
/** @brief 진핼률 화면 표시 기능 - 전체 진행 개수를 변경 시키는 함수 */
void simcmd_rate_set_total(simcmd_rate_t *rate, unsigned long total);
/** @brief 진행률 화면 표시 기능 - 진행 완료 개수를 변경 시키는 함수 */
void simcmd_rate_set_curr(simcmd_rate_t *rate, unsigned long curr);
/** @brief 진행률 화면 표시 기능 - 진행 완료 개수를 증가 시키는 함수 */
void simcmd_rate_inc_curr(simcmd_rate_t *rate);
/** @brief 진행률 화면 표시 기능 - 화면에 현재 진행률을 출력하는 함수 */
void simcmd_rate_display(simcmd_rate_t *rate);

/** @brief stdin 수집 기능 - getc 화면 출력 제어 함수 (대체 char 출력, non-canonical mode) */
char simcmd_getc_replace_stdout(char repchr);
/** @brief stdin 수집 기능 - getc 화면 출력 제어 함수 (화면에 출력하지 않음) */
char simcmd_getc_blind_stdout();
/** @brief stdin 수집 기능 - gets 화면 출력 제어 함수 (대체 문구 출력, non-canonical mode) */
char* simcmd_gets_replace_stdout(char *buffer, unsigned int length, char *repstr);
/** @brief stdin 수집 기능 - gets 화면 출력 제어 함수 (화면 출력하지 않음) */
char* simcmd_gets_blind_stdout(char *buffer, unsigned int length);


#endif //#ifndef __SIMCMD_UTIL_H__
