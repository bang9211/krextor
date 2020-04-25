#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <simcmd_errno.h>
#include <simcmd_termios.h>


static tcflag_t _simcmd_termios_get_lflag( simcmd_termios_t *t, tcflag_t lflag );
static void _simcmd_termios_set_lflag( simcmd_termios_t *t, tcflag_t lflag, char is_set );
static cc_t _simcmd_termios_get_cc( simcmd_termios_t *t, unsigned char cc );
static void _simcmd_termios_set_cc( simcmd_termios_t *t, unsigned char cc, cc_t val );


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief ���� �͹̳� ���� ����(termios)�� ��ȸ�ϴ� �Լ�
 * @param t termios instance pointer
 * @return termios instance pointer
 * @see simcmd_termios_set()
 */
simcmd_termios_t* simcmd_termios_get( simcmd_termios_t *t )
{
	if( !t ) return NULL;
	tcgetattr(0, t);
	return t;
}

/**
 * @brief �͹̳� ���� ����(termios)�� �����ϴ� �Լ�
 * @param t update termios instance pointer
 * @see simcmd_termios_get()
 */
void simcmd_termios_set( simcmd_termios_t *t )
{
	tcsetattr(0, TCSANOW, t );
}


/**
 * @brief echo ���� ��ȸ �Լ�. 
 * @param t termios instance pointer
 * @return echo status (on : ECHO, off : 0)
 * @see simcmd_termios_set_echo_off(), simcmd_termsio_set_echo_on()
 */
int simcmd_termios_get_echo_status( simcmd_termios_t *t)
{
	return _simcmd_termios_get_lflag( t, ECHO );
}

/**
 * @brief termios�� echo mode off �����ϴ� �Լ�
 * @param t termios instance pointer
 * @see simcmd_termios_get_echo_status(), simcmd_termios_set_echo_on()
 */
void simcmd_termios_set_echo_off( simcmd_termios_t *t )
{
	_simcmd_termios_set_lflag( t, ECHO, 0 );
}

/**
 * @brief termios�� echo mode on �����ϴ� �Լ�
 * @param t termios instance pointer
 * @see simcmd_termios_get_echo_status(), simcmd_termios_set_echo_off()
 */
void simcmd_termios_set_echo_on( simcmd_termios_t *t )
{
	_simcmd_termios_set_lflag( t, ECHO, 1 );
}


/**
 * @brief canonical mode ���� ��ȸ �Լ�. 
 * @param t termios instance pointer
 * @return canonical status (canonical mode : ICNON, non-canonical mode : 0) 
 * @see simcmd_termios_set_canonical_on(), simcmd_termios_set_canonical_off()
 */
int simcmd_termios_get_canonical_status( simcmd_termios_t *t )
{
	return _simcmd_termios_get_lflag( t, ICANON );
}

/**
 * @brief termios�� canonical mode �����ϴ� �Լ�
 * @param t termios instance pointer
 * @see simcmd_termios_get_canonical_status(), simcmd_termios_set_canonical_off()
 */
void simcmd_termios_set_canonical_on( simcmd_termios_t *t )
{
	_simcmd_termios_set_lflag( t, ICANON, 1 );
}

/**
 * @brief termios�� non-canonical mode �����ϴ� �Լ�
 * @param t termios instance pointer
 * @see simcmd_termios_get_canonical_status(), simcmd_termios_set_canonical_on()
 */
void simcmd_termios_set_canonical_off( simcmd_termios_t *t )
{
	_simcmd_termios_set_lflag( t, ICANON, 0 );
}


/**
 * @brief minimum input byte ���� ��ȸ �Լ�
 * @param t termios instance pointer
 * @return VMIN (byte ����, ������ ����ŭ input�� ������ ���� wait ���� ����)
 * @see simcmd_termios_set_vmin()
 */
int simcmd_termios_get_vmin( simcmd_termios_t *t )
{
	return _simcmd_termios_get_cc( t, VMIN );
}

/** 
 * @brief termios�� minimum input byte ���� ���� �Լ�
 * @param t termios instance pointer
 * @param vmin minimum input byte
 * @see simcmd_termios_get_vmin()
 */
void simcmd_termios_set_vmin( simcmd_termios_t *t, unsigned char vmin )
{
	_simcmd_termios_set_cc( t, VMIN, vmin );
}


/**
 * @brief wait input time ���� ��ȸ �Լ�
 * @param t termios instance pointer
 * @return VTIME (0.1�� ����, ������ ����ŭ wait ���� ����)
 * @see simcmd_termios_set_vtime()
 */
int simcmd_termios_get_vtime( simcmd_termios_t *t )
{
	return _simcmd_termios_get_cc( t, VTIME );
}

/**
 * @brief termios�� wait input time ���� ���� �Լ�
 * @param t termios instance pointer
 * @param vtime wait input time
 * @see simcmd_termsio_get_vtime()
 */
void simcmd_termios_set_vtime( simcmd_termios_t *t, unsigned char vtime )
{
	_simcmd_termios_set_cc( t, VTIME, vtime );
}


/**
 * @brief �͹̳��� column, row size�� ��ȸ�ϴ� �Լ�
 * @param col column value pointer (output column size)
 * @param row row value pointer (output row size)
 * @return result code 
 */
int simcmd_termios_get_window_size( int *col, int *row )
{
	struct winsize ws;

	if( !col || !row ) return eSIMCMD_INVALID_PARAM;
	if( !isatty(STDIN_FILENO) ) return eSIMCMD_OPEN_FAILED;

	ioctl(STDIN_FILENO, TIOCGWINSZ, (char*)&ws);
	*col = ws.ws_col;
	*row = ws.ws_row;
	return eSIMCMD_SUCCESS;
}


/**
 * @brief ȭ���� �ʱ�ȭ�ϴ� �Լ� (clear ���ɾ�� ������ ��� ����)
 */
void simcmd_termios_clear_screen()
{
//	initscr();
//	clear();
//	refresh();
//	endwin();
	printf("\x1B[2J\x1B[H");
}


// TODO class �߰��ؼ� termios ����� �����ϵ���~~ 


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

/** @internal termios�� lflag �� �����Ǿ��ִ��� Ȯ���ϴ� �Լ� */
static tcflag_t _simcmd_termios_get_lflag( simcmd_termios_t *t, tcflag_t lflag )
{
	if( !t ) return 0;
	return t->c_lflag & lflag;
}

/** @internal termios�� lflag�� ����(is_set true) Ȥ�� ����(is_set false)�ϴ� �Լ� */
static void _simcmd_termios_set_lflag( simcmd_termios_t *t,  tcflag_t lflag, char is_set )
{
	if( !t ) return ;
	if( is_set ) t->c_lflag |= lflag;
	else t->c_lflag &= ~lflag;
}


/** @internal termios�� cc index�� ��ġ�ϴ� ���� ��ȸ�ϴ� �Լ� */ 
static cc_t _simcmd_termios_get_cc( simcmd_termios_t *t, unsigned char cc )
{
	if( !t ) return 0;
	if( cc < NCCS ) return t->c_cc[cc];
	else return 0;
}

/** @internal termios�� cc index�� ��ġ�ϴ� ���� val�� �����ϴ� �Լ� */ 
static void _simcmd_termios_set_cc( simcmd_termios_t *t, unsigned char cc, cc_t val )
{
	if( !t ) return ;
	if( cc < NCCS ) t->c_cc[cc] = val;
}
