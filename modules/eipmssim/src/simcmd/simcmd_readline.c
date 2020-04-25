#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "simcmd_errno.h"
#include "simcmd_history.h"
#include "simcmd_register.h"
#include "simcmd_readline.h"


static struct timeval _g_timeout;

static char** _rl_attempted_completion_function(const char *text, int start, int end);
static char* _rl_generator_function(const char *text, int state);
static void _rl_process_function( char *line );


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief readline manager �ʱ�ȭ �Լ�. ���� ���� �� callback function �ʱ�ȭ 
          Tell the GNU Readline library how to complete.  We want to try to complete
          on command names if this is the first word in the line, or on filenames if not. 
 * @param prompt prompt string
 * @param timeout_msec stdin select timeout(milliseconds)
 * @return result code
 */
int simcmd_rl_init( char *prompt, int msec ) 
{
	/* The name of the calling program.  You should initialize this to
	   whatever was in argv[0].  It is used when parsing conditionals. */
	rl_readline_name = prompt; 

	/* Pointer to alternative function to create matches.
	   Function is called with TEXT, START, and END.
	   START and END are indices in RL_LINE_BUFFER saying what the boundaries
	   of TEXT are.
	   If this function exists and returns NULL then call the value of
	   rl_completion_entry_function to try to match, otherwise use the
	   array of strings returned. */
	rl_attempted_completion_function = _rl_attempted_completion_function;

	rl_callback_handler_install( prompt, _rl_process_function);

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
int simcmd_rl_readline()
{
	int fd, max_fds; 
	struct timeval timeout;
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	max_fds = STDIN_FILENO + 1;

	timeout.tv_sec = _g_timeout.tv_sec;
	timeout.tv_usec= _g_timeout.tv_usec;

	fd = select( max_fds, &readfds, 0, 0, &timeout );
	if( fd < 0 ) {
		if( errno == EINTR ) return eSIMCMD_SUCCESS;
		else {
			printf("select() failed2. err = %d(%s)\n", errno, strerror(errno) );
			return eSIMCMD_ERROR;
		}
	}

	if( (FD_ISSET(STDIN_FILENO, &readfds)) ) {
		rl_callback_read_char();
	}
	
	return eSIMCMD_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
static char** _rl_attempted_completion_function(const char *text, int start, int end)
{
	char **matches = (char **)NULL;

	/* If this word is at the start of the line, then it is a command
	   to complete.  Otherwise it is the name of a file in the current
	   directory. */
	if( start == 0 ) matches = rl_completion_matches(text, _rl_generator_function);
	return matches;
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
static char* _rl_generator_function(const char *text, int state)
{
	static int list_index, len;
	char *name;
	int i;

	/* If this is a new word to complete, initialize now.  This includes
	   saving the length of TEXT for efficiency, and initializing the index
	   variable to 0. */
	if( !state ) {
		list_index = 0;
	    len = strlen(text);
	}

	/* Return the next name which partially matches from the command list. */
	for( i = list_index ; i < simcmd_reg_count() ; i ++ ) {
		list_index++;
		name = simcmd_reg_get_name(i);
		if( name && strncasecmp(name, text, len) == 0 ) {
			return strdup(name);
		}
	}

	/* If no names matched, then return NULL. */
	return ((char *)NULL);
}

/** @internal readline callback handler function. 
              command �ڵ� �ϼ� �� command ó�� function ȣ�� */
static void _rl_process_function( char *line )
{
	int rv, argc = 0;
	char *buffer = NULL;
	char *tok, *arg, *argv[100] = { NULL, };
	simcmd_proc_f func;

	if( !line ) return ;

	// ���ɾ �ϼ���Ų��.
	rv = simcmd_history_expand( &line );
	if( rv != eSIMCMD_SUCCESS ) return ;

	// �Է��� ���ɾ �м��Ѵ�. 
	buffer = strdup(line);
	tok = buffer;
	if( tok[0] == '!' ) { // !�� ��� ���� ���� command�� ����� �� �ִ�. 
		argv[0] = "!";
		argc = 1;
		tok = buffer+1;
	}
	while( (arg = strtok_r(NULL, " \t\n", &tok)) ) {
		if( argc == 100 ) break;
		argv[argc] = arg;
		argc += 1;
	}
	if( argc == 0 ) goto final; 

	// command ? �� ��쿡�� ? command�� ������ش�.
	if( argc == 2 && !strcmp(argv[1], "?") ) {
		tok = argv[0];
		argv[0] = argv[1];
		argv[1] = tok;
	}

	// ���ɾ history�� ����Ѵ�.
	simcmd_history_add( line );
	simcmd_history_truncate( 50 );
	
	// ���ɾ� ó���� �Լ��� ã�� �����Ѵ�.
	func = simcmd_reg_get_func( argv[0] );
	if( func ) rv = func(argc, argv);
	else rv = eSIMCMD_CMD_NOT_FOUND;
	if( rv < 0 ) {
		printf("%s cmd=\"%s\"\n", simcmd_strerror(rv), line);
	}

final:
	// buffer alloc�Ѱ��� ��ȯ���ش�.
	if( buffer ) {
		free(buffer);
		buffer = NULL;
	}
	return ;
}

