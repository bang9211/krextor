#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simcmd.h"


static int _simcmd_cmd_system( int argc, char *argv[] );
static int _simcmd_cmd_cmdlist( int argc, char *argv[] );
static int _simcmd_cmd_help( int argc, char *argv[] );
static int _simcmd_cmd_history( int argc, char *argv[] );
static int _simcmd_cmd_exit( int argc, char *argv[] );

static int _get_cmd_maxlen( char *grp );
static int _help_grp( char *grp, int maxlen );
static int _help_cmd( char *cmd, int maxlen );
static void _help_print( char *help, char *cmd, int maxlen );


simcmd_reg_t simcmd_reg_common[] = {
	{ "!",			_simcmd_cmd_system,		"run system command" },	
	{ "?",			_simcmd_cmd_help,		"display help" },
	{ "command",	_simcmd_cmd_cmdlist,	"display total command list" },
	{ "c",			_simcmd_cmd_cmdlist,	"command" },
	{ "help",		_simcmd_cmd_help,		"display help" },
	{ "history",	_simcmd_cmd_history, 	"display history of previous commans" },
	{ "h",			_simcmd_cmd_history,	"history" },	
	{ "exit",		_simcmd_cmd_exit,		"simulator exit" },
	{ "quit",		_simcmd_cmd_exit,		"simulator quit" },
	{ NULL, NULL, NULL } // sentinel
};

simcmd_reg_t simcmd_reg_common_char[] = {
	{ "?",			_simcmd_cmd_help,		"display help" },
	{ "c",			_simcmd_cmd_cmdlist,	"display total command list" },
	{ "h",			_simcmd_cmd_help,		"display help" },
	{ "e",			_simcmd_cmd_exit,		"simulator exit" },
	{ "q",			_simcmd_cmd_exit,		"simulator quit" },
	{ NULL, NULL, NULL } // sentinel
};


//////////////////////////////////////////////////////////////////////////////
// base command functions 
//////////////////////////////////////////////////////////////////////////////

static int _simcmd_cmd_system( int argc, char *argv[] )
{
	int i, wlen, size;
	char buf[10240] = {0, };

	wlen = 0;
	size = sizeof(buf);
	for( i = 1 ; i < argc ; i ++ ) {
		wlen += snprintf(buf+wlen, (size > wlen) ? size-wlen : 0,
					"%s%s", i > 1 ? " " : "", argv[i]);
		if( wlen >= size - 1 ) {
			printf("Too long system commands.");
			return 0;
		}
	}
	system(buf);
	return 0;
}


static int _simcmd_cmd_cmdlist( int argc, char *argv[] )
{
	int i, maxlen;
	char *name;

	maxlen = _get_cmd_maxlen(NULL);

	for( i = 0 ; i < simcmd_reg_count() ; i ++ ) {
		name = simcmd_reg_get_name(i);
		if( !name ) break;
		printf("  %-*s", maxlen, name);
		if( (i+1) % 5 == 0 ) printf("\n");
	}
	if( i % 5 != 0 ) printf("\n");

	return eSIMCMD_SUCCESS;
}


static int _simcmd_cmd_help( int argc, char *argv[] )
{
	int i, find_flag = 0; 
	char *arg = NULL, *grpname;

	if( argc >= 2 ) arg = argv[1];

	if( !arg || !strcasecmp(arg, "all") ) {
		for( i = 0 ; i < simcmd_reg_grp_count() ; i ++ ) {
			grpname = simcmd_reg_get_grpname_i(i);
			if( grpname ) _help_grp( grpname, _get_cmd_maxlen(NULL) );
		}
		_help_grp( NULL, _get_cmd_maxlen(NULL) );
	} else {
		// grpname�� ã�� ������ �ش� grpname�� command help�� ã�� ����Ѵ�.
		find_flag = _help_grp( arg, _get_cmd_maxlen(arg) );
		if( !find_flag ) {
			// groupname�� �ƴϸ�, command help�̴�. command�� help�� ã�� ����Ѵ�. 
			find_flag = _help_cmd( arg, strlen(arg) );
		}
		if( !find_flag ) { // ������ ã�Ҵµ� �ϳ��� ��ã�� ��� ����ó��
			return eSIMCMD_INVALID_PARAM;
		}
	}
	return eSIMCMD_SUCCESS; 
}


static int _simcmd_cmd_history( int argc, char *argv[] )
{
	simcmd_history_print();
    return eSIMCMD_SUCCESS;
}


static int _simcmd_cmd_exit( int argc, char *argv[] )
{
	printf("Exit simulator.\n");
	exit(0);
}



//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

static int _get_cmd_maxlen( char *grp )
{
	int i, maxlen = 0;
	char *name, *grpname;

	for( i = 0 ; i < simcmd_reg_count() ; i ++ ) {
		name = simcmd_reg_get_name(i);
		if( !name ) continue;
		if( grp ) {
			grpname = simcmd_reg_get_grpname( name );
			if( !grpname || strcasecmp(grp, grpname) ) continue;
		}
		if( maxlen < strlen(name) ) {
			maxlen = strlen(name);
		}
	}
	return maxlen;
}


static int _help_grp( char *grp, int maxlen )
{
	int i, find_flag = 0;
	char *name, *grpname, *help;

	if( maxlen <= 0 ) maxlen = _get_cmd_maxlen(grp);

	for( i = 0 ; i < simcmd_reg_count() ; i ++ ) {
		name = simcmd_reg_get_name(i);
		if( !name ) continue; 
		grpname = simcmd_reg_get_grpname(name);
		if( (grpname && grp && !strcasecmp(grpname, grp)) || 
				(!grpname && !grp) ) { // group name�� ������ ���� �͸�, ������ ��ġ�ϴ� �͸�
			if( !find_flag ) {
				find_flag = 1;
				if( grpname ) printf(" -- %s commands --\n", grpname );
				else printf(" -- commands --\n");
			}
			help = simcmd_reg_get_help( name );
			if( help ) _help_print( help, name, maxlen );
		}
	}
	if( find_flag ) printf("\n");
	return find_flag;
}


static int _help_cmd( char *cmd, int maxlen )
{
	int i;
	char *name, *help;

	if( !cmd ) return 0;
	if( maxlen <= 0 ) maxlen = strlen(cmd);

	for( i = 0 ; i < simcmd_reg_count() ; i ++ ) {
		name = simcmd_reg_get_name(i);
		if( name && !strcasecmp(name, cmd) ) {
			help = simcmd_reg_get_help( name );
			if( help ) _help_print( help, name, maxlen );
			printf("\n");
			return 1;
		}
	}
	return 0;
}


static void _help_print( char *help, char *cmd, int maxlen )
{
	int i;

	printf("    %-*s    ", maxlen, cmd);
	for( i = 0 ; help[i] ; i ++ ) {
		printf("%c", help[i]);
		if( help[i] == '\n' && help[i+1] && help[i+1] != '\n' ) { // �ٳѱ��� ��� ������ �ش�.
			printf("    %-*s    ", maxlen, "");
		}
	}
	if( help[i-1] != '\n' ) printf("\n");
}
