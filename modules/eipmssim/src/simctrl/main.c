#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "simctrl_cmd.h"
#include "simctrl_rmi.h"
#include "simctrl_def.h"


static char *g_pname;


extern void __version_check__( int argc, char *argv[] );

static int _simctrl_rmi_run( int argc, char *argv[] );
static int _simctrl_cmd_run( int argc, char *argv[] );
static int _simctrl_scen_load( int argc, char *argv[] );
static void _simctrl_help();


int main( int argc, char *argv[] )
{
	int rv;
	char *buf;

	// check version / history. must call this function first.
	__version_check__( argc, argv );

	// get process name
	buf = strrchr(argv[0], '/');
	if( buf ) g_pname = buf+1;
	else g_pname = argv[0];

	// get opt - rmi / cmd / help
	if( argc >= 2 ) {
		if( !strcasecmp(argv[1], "rmi-run") ) return _simctrl_rmi_run( argc, argv );
		if( !strcasecmp(argv[1], "cmd-run") ) return _simctrl_cmd_run( argc, argv ); 
		if( !strcasecmp(argv[1], "scen-load") ) {
			rv = _simctrl_scen_load( argc, argv ); 
			if( rv != eSIMCTRL_SUCCESS ) return rv;
		}
		if( !strcasecmp(argv[1], "--help") || !strcasecmp(argv[1], "-h") ) {
			_simctrl_help();
			return eSIMCTRL_SUCCESS;
		}
	}

	// init simctrl command 
	rv = simctrl_cmd_init(SIMCTRL_PROMPT_STR, SIMCTRL_SELECT_TIMEOUT); 
	if( rv < 0 ) return rv;

	// start
	rv = simctrl_cmd_start();

	system( "stty echo" ); // 종료 이후에 prompt에 input 출력되지 않는 경우 발생하여 추가
	return 0;
}


/** @internal rmi command를 실행한다. */
static int _simctrl_rmi_run( int argc, char *argv[] )
{
	int rv, i, len = 0;
	char cmd[10240];

	if( argc < 5 ) {
		_simctrl_help();
		return eSIMCTRL_INVALID_PARAM;
	}

	simctrl_rmi_t *rmi = simctrl_rmi_instance();
	rv = simctrl_rmi_init( rmi, argv[2], strtol(argv[3], 0, 0), SIMCTRL_SELECT_TIMEOUT);
	if( rv < 0 ) return rv;

	for( i = 4 ; i < argc ; i ++ ) {
		len += snprintf(cmd+len, sizeof(cmd) > len ? sizeof(cmd)-len : 0, 
				"%s%s", i == 4 ? "" : " ", argv[i]);
	}
	return simctrl_rmi_run( rmi, cmd, 1 );
}

/** @internal scenario를 실행한다. */
static int _simctrl_cmd_run( int argc, char *argv[] )
{
	int rv;
	char *cmd_argv[3];

	if( argc < 6 ) {
		_simctrl_help();
		return eSIMCTRL_INVALID_PARAM;
	}

	cmd_argv[0] = "scen-load";
	cmd_argv[1] = argv[2];
	rv = simctrl_cmd_run( 2, cmd_argv );
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	if( !strcasecmp(argv[3], "ON") ) {
		cmd_argv[0] = "scen-display";
		rv = simctrl_cmd_run( 1, cmd_argv );
		if( rv != eSIMCTRL_SUCCESS ) return rv;
	}

	cmd_argv[0] = "scen-set-conf";
	cmd_argv[1] = "display-trace-log";
	cmd_argv[2] = argv[4];
	rv = simctrl_cmd_run( 3, cmd_argv );
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	cmd_argv[0] = "scen-set-conf";
	cmd_argv[1] = "file-report";
	cmd_argv[2] = argv[5];
	rv = simctrl_cmd_run( 3, cmd_argv );
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	cmd_argv[0] = "scen-run";
	cmd_argv[1] = "all";
	return simctrl_cmd_run( 2, cmd_argv );
}

/** @internal scenario load 수행한다. */
static int _simctrl_scen_load( int argc, char *argv[] )
{
	int rv;
	char *cmd_argv[2];

	if( argc < 3 ) {
		_simctrl_help();
		return eSIMCTRL_INVALID_PARAM;
	}

	cmd_argv[0] = "scen-load";
	cmd_argv[1] = argv[2];
	rv = simctrl_cmd_run( 2, cmd_argv );
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	cmd_argv[0] = "scen-display";
	return simctrl_cmd_run( 1, cmd_argv );
}

/** @internal option 기반 명령에 대한 설명을 출력한다. */
static void _simctrl_help()
{
	printf( "usage) %s [rmi-run|cmd-run|scen-load|--help|-h] arguments.\n"
			"       rmi-run    : run rmi command. rmi-run host port command\n"
			"       cmd-run    : run scenario command. cmd-run scenario_file flags\n"
			"                    flag 1 - scen-display [ON|OFF]\n"
			"                    flag 2 - display-trace-log [ON|OFF]\n"
			"                    flag 3 - file-report [ON|OFF]\n"
			"       scen-load  : load scenario command. scen-load scenario_file\n"
			"       --help, -h : display help.\n", 
			g_pname );
}
