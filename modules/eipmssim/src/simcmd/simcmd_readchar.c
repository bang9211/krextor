#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "simcmd_errno.h"
#include "simcmd_register.h"
#include "simcmd_termios.h"


static char *_g_prompt;
static struct timeval _g_timeout;


static void _simcmd_rc_readchar( void );
static void _simcmd_rc_process_function( int c );


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief readchar manager �ʱ�ȭ �Լ�. ���� ���� �ʱ�ȭ
 * @param prompt prompt string
 * @param timeout_msec stdin select timeout(milliseconds)
 * @return result code
 */
int simcmd_rc_init( char *prompt, int msec )
{
	_g_prompt = prompt;

	if( msec > 0 ) {
		_g_timeout.tv_sec = msec / 1000; 
		_g_timeout.tv_usec= (msec % 1000) * 1000;
	} else {
		_g_timeout.tv_sec = 0;
		_g_timeout.tv_usec= 100000; 
	}

	return eSIMCMD_SUCCESS;
}

/**
 * @breif simulator command ���� �� command ���� �Լ�
 * @return result code
 */
int simcmd_rc_readchar()
{
	int fd, max_fds; 
	fd_set readfds;
	struct timeval timeout;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	max_fds = STDIN_FILENO + 1;

	timeout.tv_sec = _g_timeout.tv_sec;
	timeout.tv_usec= _g_timeout.tv_usec;

	fd = select( max_fds, &readfds, 0, 0, &timeout );
	if( fd < 0 ) {
		if( errno == EINTR ) return eSIMCMD_SUCCESS;
		else {
			printf("select() failed1. err = %d(%s)\n", errno, strerror(errno) );
			return eSIMCMD_ERROR;
		}
	}

	if( FD_ISSET(STDIN_FILENO, &readfds) ) {
		_simcmd_rc_readchar();
	}
	
	return eSIMCMD_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

/** @internal read character & call process function. */
static void _simcmd_rc_readchar( void )
{
	simcmd_termios_t tios_new, tios_old;
	
	simcmd_termios_get( &tios_new );
	simcmd_termios_get( &tios_old );

	simcmd_termios_set_echo_off(&tios_new);			// echo mode off
	simcmd_termios_set_canonical_off(&tios_new);	// non-canonical mode
	simcmd_termios_set_vmin(&tios_new, 1);			// 1byte �Է¹��������� ����Ѵ�.
	simcmd_termios_set_vtime(&tios_new, 0);			// �ð��� üũ���� �ʴ´�.
	simcmd_termios_set(&tios_new);				

	_simcmd_rc_process_function( getchar() );

	simcmd_termios_set(&tios_old);
}


/** @internal readchar process function. search command function and call function */ 
static void _simcmd_rc_process_function( int c )
{
	int rv;
	char buf[128], *argv[1];
	simcmd_proc_f func;

	// input argument�� �����Ѵ�.
	buf[0] = (char)c;
	buf[1] = '\0';
	argv[0] = buf;

	// ���ɾ� ó���� �Լ��� ã�� �����Ѵ�.
	func = simcmd_reg_get_func( buf );
	if( func ) rv = func(1, argv);
	else rv = eSIMCMD_CMD_NOT_FOUND;
	if( rv < eSIMCMD_SUCCESS && rv != eSIMCMD_CMD_NOT_FOUND ) {
		printf("%s cmd=\"%s\"\n", simcmd_strerror(rv), buf);
	}
}


