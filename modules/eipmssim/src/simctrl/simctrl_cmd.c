#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <simcmd.h>
#include "simctrl_cmd.h"
#include "simctrl_rmi.h"
#include "simctrl_scen.h"
#include "simctrl_util.h"


static char *g_prompt;
static char g_stop_flag;


static int _simctrl_cmd_exit( int argc, char *argv[] );

static int _simctrl_cmd_rmi_connect( int argc, char *argv[] );
static int _simctrl_cmd_rmi_close( int argc, char *argv[] );
static int _simctrl_cmd_rmi_command( int argc, char *argv[] );
static int _simctrl_cmd_rmi_status( int argc, char *argv[] );

static int _simctrl_cmd_scen_load( int argc, char *argv[] );
static int _simctrl_cmd_scen_unload( int argc, char *argv[] );
static int _simctrl_cmd_scen_reload( int argc, char *argv[] );
static int _simctrl_cmd_scen_run( int argc, char *argv[] );
static int _simctrl_cmd_scen_display( int argc, char *argv[] );
static int _simctrl_cmd_scen_display_detail( int argc, char *argv[] );
static int _simctrl_cmd_scen_get_conf( int argc, char *argv[] );
static int _simctrl_cmd_scen_set_conf( int argc, char *argv[] );

static void _exit_program(int sig);


simcmd_reg_t _simctrl_cmd_common[] = {
	{	"exit",		_simctrl_cmd_exit,		"exit process." },
	{	"quit",		_simctrl_cmd_exit,		"quit process." },
	{ 	NULL,		NULL,					NULL } // sentinel
};

simcmd_reg_t _simctrl_cmd_rmi[] = {
	{	"rmi-cnect",
		_simctrl_cmd_rmi_connect,
		"connect rmi channel\nex) rmi-cnect \"host\" \"port\""
	},
	{	"rmi-close",
		_simctrl_cmd_rmi_close,
		"close rmi channel"
	},
	{	"rmi-cmd",		
		_simctrl_cmd_rmi_command,		
		"run rmi command.\nex) rmi-cmd \"mmc command\"" 
	},
	{	"rmi-status",
		_simctrl_cmd_rmi_status,
		"display rmi instance & connect status"
	},
	{ 	NULL, NULL, NULL } // sentinel
};

simcmd_reg_t _simctrl_cmd_scen[] = {
	{ 	"scen-display",
		_simctrl_cmd_scen_display,
		"display loaded scenario."
	},
	{	"l", 	
		_simctrl_cmd_scen_display, 
		"scen-display" 	
	},
	{	"scen-display-detail",
		_simctrl_cmd_scen_display_detail,
		"display loaded scenario detailed."
	},
	{ 	"scen-load",		
		_simctrl_cmd_scen_load,		
		"load scenario.\nex) scen-load \"xml file name\"" 
	},
	{ 	"scen-unload",	
		_simctrl_cmd_scen_unload,	
		"unload scenario.\nex) scen-unload \"scenario class name\" \"scenario name\""
		"ex) scen-unload \"scenario class name\"\nex) scen-unload \"all"
	},
	{
		"scen-reload",
		_simctrl_cmd_scen_reload,
		"reload(unload&load) scenario.\nex) scen-reload \"xml file name\""
	},
	{ 	"scen-run",		
		_simctrl_cmd_scen_run,		
		"run scenario.\nex) scen-run \"scenario index\"\n"
		"index format) \"0\", \"0-10\", \"all\" or \"0 1 5 12-19\""
	},
	{ 	"scen-get-conf",
		_simctrl_cmd_scen_get_conf,
		"get scenario configurations.\n"
		"  - display-trace-log : ȭ�鿡 �ó����� ���� ���� ö�� ���� (on/off)\n"
		"  - realtime-display  : ȭ�鿡 �ó����� ���� ���� �ǽð� ��� ���� (on/off)\n"
		"  - file-report       : File�� �ó����� ���� ��� ��� ���� (on/off)\n"
		"  - scenario-interval : �ó����� ���� �� ���� �ó����� ���� �� ���� �ð�(��)\n"
		"  - scenario-type     : �ó����� Ÿ�� (seagull/sipp/none)\n" 
		"  - rmi-host          : RMI ���� ���� (hostname or ip address)\n"
		"  - rmi-port          : RMI ���� ���� (port number)\n"
		"  - report-plugin     : �ó����� ��� �м� �÷�����\n" 
		"  - ignore-keyword    : �ó����� ���� �� ��� �� �� ������ keyword ����\n"
		"  - replace-keyword   : �ó����� ���� �� ��� �� �� ������ keyword ����\n"
		"  - define keywords   : ����� ���� (command�� ���ǵ� ��� value�� ����)\n" 
	},
	{	"scen-set-conf",
		_simctrl_cmd_scen_set_conf,
		"set scenario configurations.\n"
		"  - display-trace-log : ȭ�鿡 �ó����� ���� ���� ö�� ���� (on/off)\n"
		"  - realtime-display  : ȭ�鿡 �ó����� ���� ���� �ǽð� ��� ���� (on/off)\n"
		"  - file-report       : File�� �ó����� ���� ��� ��� ���� (on/off)\n"
		"  - scenario-interval : �ó����� ���� �� ���� �ó����� ���� �� ���� �ð�(��)\n"
		"  - scenario-type     : �ó����� Ÿ�� (seagull/sipp/none)\n" 
		"ex) scen-set-conf \"item_name\" \"item_value\""
	},
	{ 	NULL, NULL, NULL } // sentinel
};


//////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - command
//////////////////////////////////////////////////////////////////////////////

/*
 * @brief simulator controller command �ʱ�ȭ �Լ�
 * @param prompt prompt string
 * @param sleep_time stdin select timeout(milliseconds)
 * @return result code
 */
int simctrl_cmd_init( char *prompt, int sleep_time )
{
	int rv;

	g_prompt = prompt ? prompt : "";

	/* simcmd library�� �ʱ�ȭ �մϴ�. 
	   prompt�� ���ɾ� read�� wait time(msec)�� �����մϴ�.
	   wait time�� 0������ ��� default(100msec)�� �����մϴ�. */
	rv = simcmd_init( g_prompt, sleep_time );
	if( rv != eSIMCMD_SUCCESS ) {
		printf("simcmd_init() failed. err=%d(%s)\n", rv, simcmd_strerror(rv));
		return rv;
	}

	/* �ù������Ͱ� ó���� ���ɾ���� ����մϴ�.
	   �������� group, ���ɾ� ����Ʈ�� �����մϴ�.
	   �������� group�� help ó�� �� group ���� ���ɾ �����Ͽ� 
	   ȭ�鿡 ����ϱ� ���� �����̸� ������� �ʰ� NULL�� �־ �˴ϴ�. */
	rv = simcmd_register( SIMCMD_TYPE_RL, "common", _simctrl_cmd_common );
	if( rv == eSIMCMD_SUCCESS ) {
		rv = simcmd_register( SIMCMD_TYPE_RL, "rmi", _simctrl_cmd_rmi );
	}
	if( rv == eSIMCMD_SUCCESS ) {
		rv = simcmd_register( SIMCMD_TYPE_RL, "scenario", _simctrl_cmd_scen );
	}
	if( rv != eSIMCMD_SUCCESS ) {
		printf("simcmd_register() failed. err=%d(%s)\n", rv, simcmd_strerror(rv));
		return rv;
	}

	return 0;
}

/**
 * @brief simulator controller command ���� �Լ�  
          stop ȣ�� ������ ���ɾ� �Է��� ��ٸ��� �Է��� ���ɾ ����.
 * @return result code
 * @see simctrl_cmd_stop()
 */
int simctrl_cmd_start()
{
	int rv;

	while( !g_stop_flag ) {
		/* ���ɾ� �Է��� ��ٸ��� ���ɾ� �Է½� �ڵ� �����մϴ�.
		   non block mode�� ���ߵǾ� �����ð� stdin�� ������ �������ɴϴ�. */
		simcmd_readline();

		/* RMI�� ���� MMC ���ɾ� ���� �� ������ �޾ƾ� �Ѵ�.
		   non block mode�� ���ߵǾ� �����ð� ������ ������ �������ɴϴ�. */ 
		simctrl_rmi_t *rmi = simctrl_rmi_instance();
		if( rmi->init_flag && rmi->sd >= 0 && !rmi->proc_flag ) {
			do { 
				rv = simctrl_rmi_recv_result(rmi, 1);
				//if( rv == eSIMCTRL_SUCCESS ) {
				//	fprintf(stderr, "%s", g_prompt);
				//}
			} while( rv == eSIMCTRL_SUCC_CONTINUE ); 
		}

		fflush( stdout );
		fflush( stderr );
	}
	return 0;
}

/**
 * @brief simulator controller command ���� �Լ�
 * @see simctrl_cmd_start()
 */
void simctrl_cmd_stop()
{
	g_stop_flag = 1;
}

/**
 * @brief simulator controller command ���� �Լ�
 * @param argc argument count
 * @param argv argument values
 * @return result code
 */
int simctrl_cmd_run( int argc, char *argv[] )
{
	int i;

	if( argc < 1 ) return eSIMCMD_INVALID_PARAM;

	for( i = 0 ; _simctrl_cmd_scen[i].name && _simctrl_cmd_scen[i].name[0] ; i ++ ) {
		if( !strcasecmp(argv[0], _simctrl_cmd_scen[i].name) ) {
			return _simctrl_cmd_scen[i].func( argc, argv );		
		}
	}

	return eSIMCMD_CMD_NOT_FOUND;
}


//////////////////////////////////////////////////////////////////////////////
// private functions - commands
//////////////////////////////////////////////////////////////////////////////

/** @internal ���� ���� ó�� �Լ� */
static int _simctrl_cmd_exit( int argc, char *argv[] )
{
	printf("Exit simulator.\n");
	simctrl_cmd_stop();
	simcmd_init( NULL, 0);
	return 0;
}


/** @internal MMCD�� ���� �õ��Ѵ�. */
static int _simctrl_cmd_rmi_connect( int argc, char *argv[] )
{
	int rv;
	char *host, *port;
	simctrl_rmi_t *rmi;

	if( argc < 3 ) return eSIMCMD_INVALID_PARAM;
	host = argv[1];
	port = argv[2];

	rmi = simctrl_rmi_instance();
	if( rmi->init_flag && rmi->sd >= 0 ) {
		printf("already exist rmi channel...\n");
		return eSIMCMD_SUCCESS;
	}

	rv = simctrl_rmi_init( rmi, host, strtol(port, 0, 0), SIMCTRL_SELECT_TIMEOUT);
	if( rv < 0 ) {
		printf("init rmi failed. err=%d(%s)\n", rv, simctrl_strerror(rv));
		return eSIMCMD_SUCCESS;
	} else {
		printf("init rmi success.\n");
	}

	rv = simctrl_rmi_connect( rmi );
	if( rv < 0 ) {
		printf("connect rmi failed. err=%d(%s)\n", rv, simctrl_strerror(rv));
	} else {
		printf("connect rmi success.\n");
	}
	return eSIMCMD_SUCCESS;
}

/** @internal MMCD�� �������� �õ��Ѵ�. */
static int _simctrl_cmd_rmi_close( int argc, char *argv[] )
{
	int rv;
	simctrl_rmi_t *rmi;

	rmi = simctrl_rmi_instance();
	if( !rmi->init_flag || rmi->sd < 0 ) {
		printf("not connect rmi channel...\n");
		return eSIMCMD_SUCCESS;
	}

	rv = simctrl_rmi_close( rmi );
	if( rv < 0 ) {
		printf("close rmi failed. err=%d(%s)\n", rv, simctrl_strerror(rv));
	} else {
		printf("close rmi success.\n");
	}
	return eSIMCMD_SUCCESS;
}

/** @internal RMI instance ���� ��ȸ �Լ� */
static int _simctrl_cmd_rmi_status( int argc, char *argv[] )
{
	simctrl_rmi_t *rmi = simctrl_rmi_instance();
	simctrl_rmi_display( rmi );
	return eSIMCMD_SUCCESS;
}

/** @internal RMI ���ɾ� ���� �Լ� */
static int _simctrl_cmd_rmi_command( int argc, char *argv[] )
{
	int i, wlen, size;
	char buf[10240] = {0, };

	wlen = 0;
	size = sizeof(buf);
	for( i = 1 ; i < argc ; i ++ ) {
		wlen += snprintf(buf+wlen, (size > wlen) ? size-wlen : 0,
				"%s%s", i > 1 ? " " : "", argv[i]);
		if( wlen >= size - 1 ) {
			printf("Too long rmi commands.");
			return 0;
		}
	}
	simctrl_rmi_send_request(simctrl_rmi_instance(), buf);
	return 0;
}

/** @internal �ó����� �ε� ���� ó�� �Լ� */
static int _simctrl_cmd_scen_load( int argc, char *argv[] )
{
	int rv;
	char *xml;
	simctrl_scenmgr_t *mgr;

	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	xml = argv[1];

	mgr = simctrl_scenmgr_instance();

	rv = simctrl_scenmgr_load( mgr, xml );
	if( rv < 0 ) {
		printf("load scenario failed. error=%d(%s)\n", rv, simctrl_strerror(rv));
	} else {
		printf("load scenario success.\n");
	}

	// mmc ���������� �����Ǿ��ִٸ� rmi connect �����Ѵ�.
	if( rv == 0 && mgr->conf.rmi_host[0] && mgr->conf.rmi_port[0] ) {
		char *tmp[3];
		tmp[0] = "rmi-cnect";
		tmp[1] = mgr->conf.rmi_host;
		tmp[2] = mgr->conf.rmi_port;
		_simctrl_cmd_rmi_connect(3, tmp);
	}

	return eSIMCMD_SUCCESS;
}

/** @internal �ε��� �ó����� �ε� ��� ���� ó�� �Լ� */
static int _simctrl_cmd_scen_unload( int argc, char *argv[] )
{ 
	int rv;
	char *gname = NULL, *name = NULL;
	simctrl_scenmgr_t *mgr;

	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	gname = argv[1];

	mgr = simctrl_scenmgr_instance();

	// check class name, scenario name
	if( !strcasecmp(gname, "all") ) gname = NULL; 
	if( gname ) {
		if( !simctrl_scenmgr_get_grp(mgr, gname) ) {
			printf("Unknown scenario class name. name=%s\n", gname);
			return eSIMCMD_SUCCESS;
		}
		if( argc > 2 ) { // scenario class�� Ư�� scenario�� ����
			name = argv[2];
			if( !simctrl_scenmgr_get_scen(mgr, gname, name) ) {
				printf("Unknown scenario name. name=%s->%s\n", gname, name);
				return eSIMCMD_SUCCESS;
			}
		}
	}

	// unload scenario
	rv = simctrl_scenmgr_unload( mgr, gname, name);
	if( rv < 0 ) {
		printf("unload scenario failed. error=%d(%s)\n", rv, simctrl_strerror(rv));
	} else {
		printf("unload scenario success.\n");
	}

	// close rmi - ��ü �ó������� unload�Ǿ��ٸ� rmi ���ᵵ �������ش�.
	if( rv == 0 && mgr->load_flag == 0 ) {
		char *tmp[1];
		tmp[0] = "rmi-close";
		_simctrl_cmd_rmi_close( 1, tmp );
	}

	return eSIMCMD_SUCCESS;
}

/** @internal �ó����� ���ε�(unload & load) ���� ó�� �Լ� */
static int _simctrl_cmd_scen_reload( int argc, char *argv[] )
{
	int rv;
	char *tmp[3];

	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

	// unload scenario
	tmp[0] = "scen-unload";
	tmp[1] = "all";
	tmp[2] = NULL;
	rv = _simctrl_cmd_scen_unload( 2, tmp );
	if( rv != eSIMCMD_SUCCESS ) return rv;

	// load scenario
	tmp[0] = "scen-load";
	tmp[1] = argv[1];
	return _simctrl_cmd_scen_load( 2, tmp );
}

/** @internal �ó����� ���� ���� ó�� �Լ�. 
              �ó����� index�� �������� �ó������� ã�� �����ϰ� ��� ������ */
static int _simctrl_cmd_scen_run( int argc, char *argv[] )
{
	int rv, i, j, cnt, sidx, eidx;
	char *tok, *pos;
	char check_scen[MAX_SIMCTRL_GROUP * MAX_SIMCTRL_SCENARIO];
	simctrl_scenmgr_t *mgr;
	simctrl_scen_t *scen;
	struct timeval st;

	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

	// check scenario manager
	mgr = simctrl_scenmgr_instance();
	if( !mgr->load_flag ) {
		printf("do not load scenario...\n");
		return eSIMCMD_SUCCESS;
	}
			
	// init signal
	signal( SIGTERM, _exit_program);
	signal( SIGINT,  _exit_program);
	signal( SIGQUIT, _exit_program);
	signal( SIGPIPE, SIG_IGN);

	// init variables
	gettimeofday(&st, 0);
	memset(&check_scen, 0, sizeof(check_scen));
	cnt = 0;

	// ������ �ó������� Ȯ���Ѵ�.
	for( i = 1 ; i < argc ; i ++ ) {
		// ��ü �ó������� ������ ���
		if( !strcasecmp(argv[i], "all") ) { 
			memset(&check_scen, 1, sizeof(char)*mgr->nscen); // ��ϵ� scenario index�� ����
			cnt = mgr->nscen;
			break;
		}
		// Ư�� �ó������� ������ ���
		tok = argv[i];
		pos = strtok_r(NULL, "- ", &tok);
		sidx = strtol(pos, 0, 0);
		pos = strtok_r(NULL, "- ", &tok);
		eidx = pos ? strtol(pos, 0, 0) : sidx;
		if( sidx < 0 || eidx < sidx ) continue; 
		for( j = sidx ; j <= eidx ; j ++ ) {
			if( j >= mgr->nscen ) break; // scenario ��ϵ� index �������� ���� ����
			check_scen[j] = 1;
			cnt += 1;
		}
	}
	if( cnt == 0 ) {
		printf("Select wrong scenario index. scenario index range - %d ~ %d.\n", 0, mgr->nscen-1); 
		return eSIMCMD_INVALID_PARAM;
	}

	for( cnt = i = 0 ; i < mgr->nscen ; i ++ ) {
		if( check_scen[i] == 0 ) continue;
		if( cnt > 0 && mgr->conf.scen_interval > 0 ) sleep(mgr->conf.scen_interval);

		scen = simctrl_scenmgr_get_scen_i(mgr, i); 
		if( !scen ) {
			printf("Unknown scenario index(%d).\n", i);
			continue;
		}

		rv = simctrl_scen_proc(scen);
		if( rv ) check_scen[i] = 2; // ����� ������ ���

		cnt += 1;

		if( g_stop_flag ) {
			memset(&check_scen[i], 0, mgr->nscen - i); // ������� ���õ��� ���� �ɷ� ����
			break; // simctrl command stop ȣ��� ��� �� �������� �ʰ� �����Ѵ�.
		}
	}

	simctrl_scenmgr_display_result(mgr, check_scen, st);
	return eSIMCMD_SUCCESS;
}

/** @internal �ε��� �ó����� ����Ʈ ��� ���� ó�� �Լ� */
static int _simctrl_cmd_scen_display( int argc, char *argv[] )
{
	simctrl_scenmgr_t *mgr;
	simctrl_scengrp_t *grp;
	simctrl_scen_t *scen;

	// check scenario manager
	mgr = simctrl_scenmgr_instance();
	if( !mgr->load_flag ) {
		printf("do not load scenario...\n");
		return eSIMCMD_SUCCESS;
	}	

	simctrl_display_line('-');
	if( argc < 2 ) { // display all
		simctrl_scenmgr_display(mgr, SIMCTRL_DISPLAY_DEFAULT);
	} else if( argc < 3 ) { // display scenario class
		grp = simctrl_scenmgr_get_grp(mgr, argv[1]);
		if( grp ) simctrl_scengrp_display(grp, SIMCTRL_DISPLAY_DEFAULT);
	} else { // display scenario
		scen = simctrl_scenmgr_get_scen(mgr, argv[1], argv[2]);
		if( scen ) simctrl_scen_display(scen, SIMCTRL_DISPLAY_DEFAULT);
	}
	simctrl_display_line('-');
	return eSIMCMD_SUCCESS;
}

/** @internal �ε��� �ó����� ����Ʈ ��� ���� ó�� �Լ�. �ó����� �� �������� ��� */
static int _simctrl_cmd_scen_display_detail( int argc, char *argv[] )
{
	simctrl_scenmgr_t *mgr;
	simctrl_scengrp_t *grp;
	simctrl_scen_t *scen;

	// check scenario manager
	mgr = simctrl_scenmgr_instance();
	if( !mgr->load_flag ) {
		printf("do not load scenario...\n");
		return eSIMCMD_SUCCESS;
	}	
	
	simctrl_display_line('-');
	if( argc < 2 ) { // display all
		simctrl_scenmgr_display(mgr, SIMCTRL_DISPLAY_ALL); 
	} else if( argc < 3 ) { // display scenario class
		grp = simctrl_scenmgr_get_grp(mgr, argv[1]);
		if( grp ) simctrl_scengrp_display(grp, SIMCTRL_DISPLAY_ALL);
	} else { // display scenario
		scen = simctrl_scenmgr_get_scen(mgr, argv[1], argv[2]);
		if( scen ) simctrl_scen_display(scen, SIMCTRL_DISPLAY_ALL);
	}
	simctrl_display_line('-');
	return eSIMCMD_SUCCESS;
}

/** @internal �ε��� �ó������� �������� ��� ���� ó���Լ� */
static int _simctrl_cmd_scen_get_conf( int argc, char *argv[] )
{
	int i;
	simctrl_scenmgr_t *mgr; 

	mgr = simctrl_scenmgr_instance();
	if( !mgr->load_flag ) {
		printf("do not load scenario...\n");
		return eSIMCMD_SUCCESS;
	}	
	
	printf("Scenario configurations\n");
	printf("  - display-trace-log : %s\n", mgr->conf.display_flag ? "on" : "off");
	printf("  - realtime-display  : %s\n", mgr->conf.realtime_display ? "on" : "off");
	printf("  - file-report       : %s\n", mgr->conf.file_report_flag ? "on" : "off");
	printf("  - scenario-timeout  : %d sec\n", mgr->conf.scen_timeout);
	printf("  - scenario-interval : %d sec\n", mgr->conf.scen_interval);
	printf("  - scenario-type     : %s\n", simctrl_scen_type_str(mgr->conf.scen_type));
	printf("  - rmi-host          : %s\n", mgr->conf.rmi_host);
	printf("  - rmi-port          : %s\n", mgr->conf.rmi_port);
	printf("  - report-plugin     : %s\n", mgr->conf.report_plugin);
	if( mgr->conf.ignore_keyword_cnt == 0 ) {
		printf("  - ignore-keyword    : not defined.\n");
	} else {
		printf("  - ignore-keyword\n");
		for( i = 0 ; i < mgr->conf.ignore_keyword_cnt ; i ++ ) {
			printf("    + \"%s\"\n", mgr->conf.ignore_keyword[i]);
		}
	}
	if( mgr->conf.replace_keyword_cnt == 0 ) {
		printf("  - replace-keyword   : not defined.\n");
	} else {
		printf("  - replace-keyword\n");
		for( i = 0 ; i < mgr->conf.replace_keyword_cnt ; i ++ ) {
			printf("    + \"%s\"\n", mgr->conf.replace_keyword[i]);
		}
	}
	if( mgr->conf.define_cnt == 0 ) {
		printf("  - define keywords   : not defined.\n");
	} else {
		printf("  - define keywords\n");
		for( i = 0 ; i < mgr->conf.define_cnt ; i ++ ) {
			printf("    + %-*s = \"%s\"\n", 16, mgr->conf.defines[i].name, 
					mgr->conf.defines[i].value);
		}
	}
	return eSIMCMD_SUCCESS;
}

/** @internal �ε��� �ó������� �������� ���� ���� ó���Լ� */
static int _simctrl_cmd_scen_set_conf( int argc, char *argv[] )
{
	int val_i;
	char *name, *val;
	simctrl_scenmgr_t *mgr; 

	mgr = simctrl_scenmgr_instance();
	if( !mgr->load_flag ) {
		printf("do not load scenario...\n");
		return eSIMCMD_SUCCESS;
	}	

	if( argc < 3 ) return eSIMCMD_INVALID_PARAM;
	name = argv[1];
	val = argv[2];

	if( !strcasecmp(name, "display-trace-log") ) {
		if( !strcasecmp(val, "on") ) mgr->conf.display_flag = 1;
		else if( !strcasecmp(val, "off") ) mgr->conf.display_flag = 0;
		else {
			printf("Wrong %s value(%s) expected(on/off)\n", name, val);
			return eSIMCMD_INVALID_PARAM;
		}
	} else if( !strcasecmp(name, "realtime-display") ) {
		if( !strcasecmp(val, "on") ) mgr->conf.realtime_display = 1;
		else if( !strcasecmp(val, "off") ) mgr->conf.realtime_display = 0;
		else {
			printf("Wrong %s value(%s) expected(on/off)\n", name, val);
			return eSIMCMD_INVALID_PARAM;
		}
	} else if( !strcasecmp(name, "file-report") ) {
		if( !strcasecmp(val, "on") ) mgr->conf.file_report_flag = 1;
		else if( !strcasecmp(val, "off") ) mgr->conf.file_report_flag = 0;
		else {
			printf("Wrong %s value(%s) expected(on/off)\n", name, val);
			return eSIMCMD_INVALID_PARAM;
		}
	} else if( !strcasecmp(name, "scenario-timeout") ) {
		val_i = strtol(val, 0, 0);
		if( val_i < 0 || val_i >= 3600 ) {
			printf("Wrong %s value(%s) expedted(0 ~ 3600)\n", name, val);
			return eSIMCMD_INVALID_PARAM;
		}
		mgr->conf.scen_timeout = val_i;
	} else if( !strcasecmp(name, "scenario-interval") ) {
		val_i = strtol(val, 0, 0);
		if( val_i < 0 || val_i >= 30 ) {
			printf("Wrong %s value(%s) expected(0 ~ 30)\n", name, val);
			return eSIMCMD_INVALID_PARAM;
		}
		mgr->conf.scen_interval = val_i;
	} else if( !strcasecmp(name, "scenario-type") ) {
		if( !strcasecmp(val, "seagull") ) mgr->conf.scen_type = SIMCTRL_SCEN_TYPE_SEAGULL;
		else if( !strcasecmp(val, "sipp") ) mgr->conf.scen_type = SIMCTRL_SCEN_TYPE_SIPP;
		else if( !strcasecmp(val, "none") ) mgr->conf.scen_type = SIMCTRL_SCEN_TYPE_DEFAULT;
		else {
			printf("Wrong %s value(%s) expected(seagull/sipp/none)\n", name, val);
			return eSIMCMD_INVALID_PARAM;
		}
	} else {
		printf("Wrong scenario configuration item(%s)\n", name);
		return eSIMCMD_INVALID_PARAM;
	}

	printf("set configuration success. %s %s\n", name, val);
	return eSIMCMD_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// private functions - etc 
//////////////////////////////////////////////////////////////////////////////

static void _exit_program(int sig)
{
	simctrl_cmd_stop();
	signal( sig, _exit_program);
}

