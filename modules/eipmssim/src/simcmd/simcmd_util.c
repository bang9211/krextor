#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <simcmd_errno.h>
#include <simcmd_termios.h>
#include <simcmd_util.h>


#define _RATE_TOK_STR	"-\\|/-\\|/-*"
#define _RATE_SCALE		10
#define _RATE_RANGE		100
#define _RATE_MAX		(_RATE_RANGE * _RATE_SCALE)


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief ����� ȭ�� ǥ�� ��� - �ʱ�ȭ �Լ�
 * @param rate process rate instance pointer
 * @param total total process count
 * @param column process rate column size
 * @param mode process rate display mode 
 *             SIMCMD_RATE_MODE_PERCENTAGE | 
 *             SIMCMD_RATE_MODE_PROCRATE | 
 *             SIMCMD_RATE_MODE_REMAINSTIME
 * @return result code
 */
int simcmd_rate_init(simcmd_rate_t *rate, unsigned long total,
		unsigned short column, unsigned int mode )
{
	if( !rate || total == 0 ) return eSIMCMD_INVALID_PARAM;

	memset(rate, 0, sizeof(simcmd_rate_t));
	rate->total = total;
	rate->column = !column ? _RATE_RANGE : column > _RATE_RANGE ? _RATE_RANGE : column;
	rate->mode = !(mode & SIMCMD_RATE_MODE_ALL) ? SIMCMD_RATE_MODE_PROCRATE : mode;
	rate->stime = time(0);
	rate->ctime = time(0);
	rate->rtime = time(0);

	return eSIMCMD_SUCCESS;
}

/**
 * @brief ���۷� ȭ�� ǥ�� ��� - ��ü ���� ������ ���� ��Ű�� �Լ�
 * @param rate process rate instance pointer
 * @param total total process count
 */ 
void simcmd_rate_set_total(simcmd_rate_t *rate, unsigned long total)
{
	if ( !rate || !total ) return ;
	rate->total = total;
	if( rate->total < rate->curr_cnt ) rate->total = rate->curr_cnt;
}

/**
 * @brief ����� ȭ�� ǥ�� ��� - ���� �Ϸ� ������ ���� ��Ű�� �Լ�
 * @param rate process rate instance pointer
 * @param val current process count
 * @see simcmd_rate_inc_curr()
 */
void simcmd_rate_set_curr(simcmd_rate_t *rate, unsigned long curr)
{
	if( !rate ) return ;
	rate->curr_cnt = curr;
	if( rate->curr_cnt > rate->total ) rate->curr_cnt = rate->total;
	rate->curr_rate = (rate->curr_cnt * _RATE_MAX) / rate->total;
	if( rate->curr_rate >= _RATE_MAX ) rate->curr_rate = _RATE_MAX;
	if( rate->total <= rate->curr_cnt ) rate->stop_flag = 1;
	rate->ctime = time(0);
}

/**
 * @brief ����� ȭ�� ǥ�� ��� - ���� �Ϸ� ������ ���� ��Ű�� �Լ�
 * @param rate process rate instance pointer
 * @see simcmd_rate_set_curr()
 */
void simcmd_rate_inc_curr(simcmd_rate_t *rate)
{
	if( !rate ) return ;
	simcmd_rate_set_curr(rate, rate->curr_cnt+1);
}

/**
 * @brief ����� ȭ�� ǥ�� ��� - ȭ�鿡 ���� ������� ����ϴ� �Լ�
 * @param rate process rate instance pointer
 */
void simcmd_rate_display(simcmd_rate_t *rate)
{
	int i, len = 0, column, col, row;
	char proc_rate[_RATE_RANGE+2];

	if( !rate ) return ;

	// get window size
	simcmd_termios_get_window_size(&col, &row);

	// percentabe - "50.1%"
	if( rate->mode & SIMCMD_RATE_MODE_PERCENTAGE ) {
		len += fprintf(stderr, "%3d.%0*d%% ", (rate->curr_rate / _RATE_SCALE), 
				(_RATE_SCALE / 10), (rate->curr_rate % _RATE_SCALE));
	}
	// process rate - "|*****/    |"
	if( rate->mode & SIMCMD_RATE_MODE_PROCRATE ) {
		column = col - 28/*percentage + remains time + delimiter*/;
		if( column < 10 ) proc_rate[0] = '\0';
		else {
			if( column > rate->column ) column = rate->column;
			column = (column / 10) * 10; 

			for( i = 0 ; i < ((rate->curr_rate * column) / _RATE_MAX) ; i ++ ) {
				proc_rate[i] = _RATE_TOK_STR[9];
			}
			if( !rate->stop_flag && rate->total > (column * 5) ) {
				// 10�� ����� ����Ǹ� 1% ����
				proc_rate[i++] = 
					_RATE_TOK_STR[((rate->curr_rate * column) / _RATE_RANGE) % _RATE_SCALE];
			}
			for( ; i < column ; i ++ ) proc_rate[i] = ' ';
			proc_rate[column] = '\0';
		}
		len += fprintf(stderr, "|%s| ", proc_rate);
	}
	// remains time - "X�ð� Y��" / "X�� Y��" / "X��"
	if( rate->mode & SIMCMD_RATE_MODE_REMAINSTIME ) {
		if( ((rate->curr_cnt - rate->remains_cnt) > 0 && 
				rate->ctime >= (rate->rtime + 3)) || rate->stop_flag ) {
			if( rate->curr_cnt - rate->remains_cnt > 0 ) {
				rate->remains = ((rate->ctime - rate->rtime) * 
					(rate->total - rate->curr_cnt)) / (rate->curr_cnt - rate->remains_cnt);
			} else rate->remains = 0; 
			rate->rtime = rate->ctime;
			rate->remains_cnt = rate->curr_cnt;
		}
		if( rate->remains > 3600 ) {
			len += fprintf(stderr, "%lu�ð� %lu�� ", rate->remains/3600, (rate->remains/60)%60);
		} else if( rate->remains > 60 ) {
			len += fprintf(stderr, "%lu�� %lu�� ", rate->remains / 60, rate->remains % 60);
		} else {
			len += fprintf(stderr, "%lu�� ", rate->remains);
		}
	}

	fprintf(stderr, "%*s\r%s", col - len - 2, "", rate->stop_flag ? "\n" : "");
}


/**
 * @brief stdin ���� ��� - getc ȭ�� ��� ���� �Լ� (��ü char ���, non-canonical mode)
 * @param repchr replace character
 * @return get character 
 * @see simcmd_getc_blind_stdout(), simcmd_gets_replace_stdout()
 */
char simcmd_getc_replace_stdout(char repchr)
{
	char c;
	simcmd_termios_t tios_new, tios_old;
	
	simcmd_termios_get( &tios_new );
	simcmd_termios_get( &tios_old );

	simcmd_termios_set_echo_off(&tios_new);			// echo mode off
	simcmd_termios_set_canonical_off(&tios_new);	// non-canonical mode
	simcmd_termios_set_vmin(&tios_new, 1);			// 1byte �Է¹��������� ����Ѵ�.
	simcmd_termios_set_vtime(&tios_new, 0);			// �ð��� üũ���� �ʴ´�.
	simcmd_termios_set(&tios_new);				

	c = getchar();

	if( c == '\n' ) printf("\n");
	else if( repchr >= ' ' ) printf("%c", repchr);

	simcmd_termios_set(&tios_old);

	return c; 
}

/**
 * @brief stdin ���� ��� - getc ȭ�� ��� ���� �Լ� (ȭ�鿡 ������� ����) 
 * @param repchr replace character
 * @see simcmd_getc_replace_stdout(), simcmd_gets_blind_stdout()
 */
char simcmd_getc_blind_stdout()
{
	char c;
	simcmd_termios_t tios_new, tios_old;
	
	simcmd_termios_get( &tios_new );
	simcmd_termios_get( &tios_old );

	simcmd_termios_set_echo_off(&tios_new);			// echo mode off
	simcmd_termios_set(&tios_new);				

	c = getchar();

	if( c == '\n' ) printf("\n");

	simcmd_termios_set(&tios_old);

	return c; 
}

/**
 * @brief stdin ���� ��� - gets ȭ�� ��� ���� �Լ� (��ü ���� ���, non-canonical mode)
 * @param buffer input buffer
 * @param length input buffer length
 * @param repstr replace string (�Է��� ��ŭ ��ü ������ �ݺ������� ��µ�)
 * @return buffer 
 * @see simcmd_gets_blind_stdout(), simcmd_getc_replace_stdout()
 */
char* simcmd_gets_replace_stdout(char *buffer, unsigned int length, char *repstr)
{
	char c;
	int replen = 0, idx = 0;
	
	if( !buffer|| !length ) return NULL;
	if( repstr ) replen = strlen(repstr);

	while( (c = simcmd_getc_replace_stdout(repstr[idx % replen])) ) {
		if( idx + 1 >= length ) break;
		if( c == '\n' ) break;
		buffer[idx++] = c;
	}
	buffer[idx] = '\0';

	return buffer;
}

/**
 * @brief stdin ���� ��� - gets ȭ�� ��� ���� �Լ� (ȭ�� ������� ����)
 * @param buffer input buffer
 * @param length input buffer length
 * @return buffer 
 * @see simcmd_gets_replace_stdout(), simcmd_gets_blind_stdout()
 */
char* simcmd_gets_blind_stdout(char *buffer, unsigned int length)
{
	char c;
	int idx = 0;
	
	if( !buffer|| !length ) return NULL;

	while( (c = simcmd_getc_blind_stdout()) ) {
		if( idx + 1 >= length ) break;
		if( c == '\n' ) break;
		buffer[idx++] = c;
	}
	buffer[idx] = '\0';

	return buffer;
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////
