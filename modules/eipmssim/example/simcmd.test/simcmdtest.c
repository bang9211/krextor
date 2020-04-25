#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "simcmd.h"


static int g_stop_flag;
static char *g_pname;
static char g_prompt[] = { "[SIMCMDLIB TESTER] " };

static int _cmd_exit( int argc, char *argv[] );
static int _cmd_test1( int argc, char *argv[] );
static int _cmd_test2( int argc, char *argv[] );
static int _cmd_test3( int argc, char *argv[] );
static int _cmd_change( int argc, char *argv[] );
static int _cmd_plus( int argc, char *argv[] );
static int _cmd_minus( int argc, char *argv[] );
static int _cmd_clear( int argc, char *argv[] );
static int _cmd_window_size( int argc, char *argv[] );
static int _cmd_process_rate( int argc, char *argv[] );

// readline �� command list
simcmd_reg_t _cmd_common[] = {
	/* simcmd library���� �⺻���� �����ϴ� command�� ��ü�մϴ�. */
	{ "exit",			_cmd_exit,			"exit simcmdtest process." },
	{ "quit",			_cmd_exit,			"quit simcmdtest process." },
	{ NULL, NULL, NULL } // sentinel /* ���ɾ��� �������� �ǹ��մϴ�. */
};

simcmd_reg_t _cmd_test[] = {
	{ "test1",			_cmd_test1, 		"test1..." },
	{ "test2",			_cmd_test2, 		"test2...\ntest2...", },
	{ "test3",			_cmd_test3,			"test3...\ntest3...\ntest3..." },
	{ "change", 		_cmd_change,		"change read command type. readline -> readchar" },
	{ "clear", 			_cmd_clear,			"clear screen" },
	{ "window-size",	_cmd_window_size,	"get terminal window size." },
	{ "process-rate",	_cmd_process_rate, 	"test process rate.\nprocess-rate \"total count\" \"sleep msec\"" },
	{ NULL, NULL, NULL } // sentinel
};

// readchar �� command list
simcmd_reg_t _cmd_common_char[] = {
	{ "e",				_cmd_exit, 			"exit simcmdtest process." },
	{ "q",				_cmd_exit, 			"quit simcmdtest process." },
	{ NULL, NULL, NULL } // sentinel
};

simcmd_reg_t _cmd_test_char[] = {
	{ "+",				_cmd_plus,  		"increase character" },
	{ "-", 				_cmd_minus, 		"decrease character" },
	{ "c",				_cmd_change,		"change read command type. readchar -> readline" },
	{ NULL, NULL, NULL } // sentinel
};


int main( int argc, char *argv[])
{
	int rv;
	char *buf;

	// get process name
	buf = strrchr(argv[0], '/');
	if( buf ) g_pname = buf+1;
	else g_pname = argv[0];

	/* simcmd library�� �ʱ�ȭ �մϴ�. 
	   prompt�� ���ɾ� read�� wait time(msec)�� �����մϴ�.
	   wait time�� 0������ ��� default(100msec)�� �����մϴ�. */
	rv = simcmd_init( g_prompt, 3000 );
	if( rv != eSIMCMD_SUCCESS ) {
		printf("simcmd_init() failed. err=%d(%s)\n", rv, simcmd_strerror(rv));
		return rv;
	}
	/* �ù������Ͱ� ó���� ���ɾ���� ����մϴ�.
	   �������� group, ���ɾ� ����Ʈ�� �����մϴ�.
	   �������� group�� help ó�� �� group ���� ���ɾ �����Ͽ� 
	   ȭ�鿡 ����ϱ� ���� �����̸� ������� �ʰ� NULL�� �־ �˴ϴ�. */
	// readline mode���� ����� command ���
	rv = simcmd_register( SIMCMD_TYPE_RL, "common", _cmd_common );
	if( rv == eSIMCMD_SUCCESS ) rv = simcmd_register( SIMCMD_TYPE_RL, NULL, _cmd_test );
	if( rv != eSIMCMD_SUCCESS ) { 
		printf("simcmd_register() failed. err=%d(%s)\n", rv, simcmd_strerror(rv));
		return rv;
	}
	// readchar mode���� ����� command ���
	rv = simcmd_register( SIMCMD_TYPE_RC, "common", _cmd_common_char );
	if( rv == eSIMCMD_SUCCESS ) rv = simcmd_register( SIMCMD_TYPE_RC, NULL, _cmd_test_char );
	if( rv != eSIMCMD_SUCCESS ) { 
		printf("simcmd_register() failed. err=%d(%s)\n", rv, simcmd_strerror(rv));
		return rv;
	}

	while( !g_stop_flag ) {
		/* ���ɾ� �Է��� ��ٸ��� ���ɾ� �Է½� �ڵ� �����մϴ�.
		   non block mode�� ���ߵǾ� �����ð� stdin�� ������ �������ɴϴ�. */
		rv = simcmd_readline();
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// private functions - commands 
//////////////////////////////////////////////////////////////////////////////

static int _cmd_exit( int argc, char *argv[] )
{
	printf(" Exit simulator.\n");
	g_stop_flag = 1;
	return 0;
}


static int _cmd_quit( int argc, char *argv[] )
{
	return _cmd_exit( argc, argv );
}


static int _cmd_test1( int argc, char *argv[] )
{
	int i;

	/* argc�� argument ������ argv�� argment list�� ������ �ֽ��ϴ�.
	   argv[0]���� ���� �����Ǹ� �Է��� ���ɾ �������� �����Ѱ��Դϴ�. */

	/* ���� ����(����)�� ��� ó�� ��� ���� �޽����� ȭ�鿡 �ڵ����� ����մϴ�. */
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

	printf( " Command is \"%s\".\n", argv[0]);

	for( i = 1 ; i < argc ; i ++ ) {
		printf(" %d's argument is \"%s\".\n", i, argv[i]);
	}

	return eSIMCMD_SUCCESS;
}


static int _cmd_test2( int argc, char *argv[] )
{
	return _cmd_test1( argc, argv );
}


static int _cmd_test3( int argc, char *argv[] )
{
	return _cmd_test2( argc, argv );
}


static int _cmd_change( int argc, char *argv[] )
{
	int type = simcmd_get_type();
	/* ���ɾ� ���� ��带 �����մϴ�.
	   SIMCMD_TYPE_RL (readline mode), SIMCMD_TYPE_RC (readchar mode) */
	simcmd_set_type( type == SIMCMD_TYPE_RL ? SIMCMD_TYPE_RC : SIMCMD_TYPE_RL );
	printf(" change mode.\n");
	return eSIMCMD_SUCCESS;
}


static int _cmd_plus( int argc, char *argv[] )
{
	printf(" %s : increase...\n", argv[0]);
	return eSIMCMD_SUCCESS;
}


static int _cmd_minus( int argc, char *argv[] )
{
	printf(" %s : decrease...\n", argv[0]);
	return eSIMCMD_SUCCESS;
}


static int _cmd_clear( int argc, char *argv[] )
{
	simcmd_termios_clear_screen();
	printf(" %s : clear screen...\n", argv[0]);
	return eSIMCMD_SUCCESS;
}


static int _cmd_window_size( int argc, char *argv[] )
{
	int col, row;
	/* terminal window size�� ��ȸ�մϴ�. */
	simcmd_termios_get_window_size( &col, &row);
	printf(" %s : column = %d, row = %d\n", argv[0], col, row);
	return eSIMCMD_SUCCESS;
}


static int _cmd_process_rate( int argc, char *argv[] )
{
	int total, sleeptime;
	simcmd_rate_t rate;

	if( argc < 3 ) return eSIMCMD_INVALID_PARAM;
	printf(" %s : total=%s, sleep msec=%s\n", argv[0], argv[1], argv[2]);
	total = strtol(argv[1], 0, 0);
	if( total <= 0 ) total = 1000;
	sleeptime = strtol(argv[2], 0, 0);
	if( sleeptime <= 0 ) sleeptime = 10;

	simcmd_rate_init(&rate, total, 70, SIMCMD_RATE_MODE_ALL); 
	while( !rate.stop_flag ) {
		simcmd_rate_inc_curr(&rate);
		simcmd_rate_display(&rate);
		
		sleep(sleeptime / 1000);
		usleep((sleeptime % 1000) * 1000);
	}
	return eSIMCMD_SUCCESS;
}
