#ifndef __SIMCMD_UTIL_H__
#define __SIMCMD_UTIL_H__


#include <time.h>


//////////////////////////////////////////////////////////////////////////////
// Utilities - Process Rate 
//////////////////////////////////////////////////////////////////////////////

/** @brief ����� ȭ�� ��� ��� - Percentage, Process Rate and Remains Time */
#define SIMCMD_RATE_MODE_PERCENTAGE		0x0001
#define SIMCMD_RATE_MODE_PROCRATE		0x0002
#define SIMCMD_RATE_MODE_REMAINSTIME	0x0004
#define SIMCMD_RATE_MODE_ALL			(SIMCMD_RATE_MODE_PERCENTAGE | \
		SIMCMD_RATE_MODE_PROCRATE | SIMCMD_RATE_MODE_REMAINSTIME)


/** @brief process rate manager type */
typedef struct simcmd_rate_s simcmd_rate_t;
/** @brief process rate manager struct */
struct simcmd_rate_s {
	char stop_flag; /**< process rate ���Ῡ�� */
	unsigned long total; /**< ��ü �����ؾ��� process ���� */
	unsigned short column; /**< process rate ���� ǥ�� column size */
	unsigned int mode; /**< display mode. percentage|process_rate|remains_time */
	time_t stime; /**< start time */
	unsigned long curr_cnt; /**< current process complete count */
	unsigned short curr_rate; /**< current process complete rate */
	time_t ctime; /**< current time */
	unsigned long remains; /**< rememains time - ���� �ð� ���� */
	unsigned long remains_cnt; /**< remains time - ���� üũ���� �� ��� process complete count */
	time_t rtime; /**< remains time - ���� üũ���� ��� �ð� */
};


/** @brief ����� ȭ�� ǥ�� ��� - �ʱ�ȭ �Լ� */
int simcmd_rate_init(simcmd_rate_t *rate, unsigned long total, 
		unsigned short column, unsigned int mode);
/** @brief ���۷� ȭ�� ǥ�� ��� - ��ü ���� ������ ���� ��Ű�� �Լ� */
void simcmd_rate_set_total(simcmd_rate_t *rate, unsigned long total);
/** @brief ����� ȭ�� ǥ�� ��� - ���� �Ϸ� ������ ���� ��Ű�� �Լ� */
void simcmd_rate_set_curr(simcmd_rate_t *rate, unsigned long curr);
/** @brief ����� ȭ�� ǥ�� ��� - ���� �Ϸ� ������ ���� ��Ű�� �Լ� */
void simcmd_rate_inc_curr(simcmd_rate_t *rate);
/** @brief ����� ȭ�� ǥ�� ��� - ȭ�鿡 ���� ������� ����ϴ� �Լ� */
void simcmd_rate_display(simcmd_rate_t *rate);

/** @brief stdin ���� ��� - getc ȭ�� ��� ���� �Լ� (��ü char ���, non-canonical mode) */
char simcmd_getc_replace_stdout(char repchr);
/** @brief stdin ���� ��� - getc ȭ�� ��� ���� �Լ� (ȭ�鿡 ������� ����) */
char simcmd_getc_blind_stdout();
/** @brief stdin ���� ��� - gets ȭ�� ��� ���� �Լ� (��ü ���� ���, non-canonical mode) */
char* simcmd_gets_replace_stdout(char *buffer, unsigned int length, char *repstr);
/** @brief stdin ���� ��� - gets ȭ�� ��� ���� �Լ� (ȭ�� ������� ����) */
char* simcmd_gets_blind_stdout(char *buffer, unsigned int length);


#endif //#ifndef __SIMCMD_UTIL_H__
