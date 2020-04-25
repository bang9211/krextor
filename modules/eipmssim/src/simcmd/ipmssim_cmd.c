#include <signal.h>
#include <unistd.h>
#include <ua_log.h>
#include "simcmd.h"
#include "ipmssim_proc.h"
#include "ipmssim_tcpip.h"
#include "ipmssim_util.h"
#include "../../../SKBGW/modules/eipmsib/src/skb_msg.h"


static int _g_stop_flag = 0;
static char *_g_prompt = "";

static int _g_log_id = 0;

static int _g_display_flag = 1;

// static int _g_tps_proc_flag = 0;
// static char _g_fname[256];
// static int _g_tps = 0;
// static int _g_max = 0;
// static int _g_cnt = 0;

static int _ipmssim_cmd_tcpip_start( int argc, char *argv[] );
static int _ipmssim_cmd_tcpip_stop( int argc, char *argv[] );
static int _ipmssim_cmd_get_status( int argc, char *argv[] );
static int _ipmssim_cmd_set_display_on( int argc, char *argv[] );
static int _ipmssim_cmd_set_display_off( int argc, char *argv[] );
static int _ipmssim_cmd_set_send_msg_on( int argc, char *argv[] );
static int _ipmssim_cmd_set_send_msg_off( int argc, char *argv[] );
static int _ipmssim_cmd_set_send_res_succ( int argc, char *argv[] );
static int _ipmssim_cmd_set_send_res_err( int argc, char *argv[] );
static int _ipmssim_cmd_set_heartbeat_time( int argc, char *argv[] );
static int _ipmssim_cmd_set_heartbeat_display_on( int argc, char *argv[] );
static int _ipmssim_cmd_set_heartbeat_display_off( int argc, char *argv[] );
static int _ipmssim_cmd_set_timeout_time( int argc, char *argv[] );

//call
static int _ipmssim_cmd_make_call_service_status_rpt( int argc, char *argv[] );		//204
static int _ipmssim_cmd_send_call_service_status_rpt( int argc, char *argv[] );		//204
static int _ipmssim_cmd_make_call_call_end_rpt( int argc, char *argv[] );			//205
static int _ipmssim_cmd_send_call_call_end_rpt( int argc, char *argv[] );			//205
static int _ipmssim_cmd_make_call_recording_start_rpt( int argc, char *argv[] );	//
static int _ipmssim_cmd_send_call_recording_start_rpt( int argc, char *argv[] );	//
static int _ipmssim_cmd_make_call_recording_end_rpt( int argc, char *argv[] );		//
static int _ipmssim_cmd_send_call_recording_end_rpt( int argc, char *argv[] );		//
//record
static int _ipmssim_cmd_make_record_service_status_rpt( int argc, char *argv[] );	//402
static int _ipmssim_cmd_send_record_service_status_rpt( int argc, char *argv[] );	//402
static int _ipmssim_cmd_make_record_call_start_rpt( int argc, char *argv[] );		//
static int _ipmssim_cmd_send_record_call_start_rpt( int argc, char *argv[] );		//
static int _ipmssim_cmd_make_record_call_end_rpt( int argc, char *argv[] );			//403
static int _ipmssim_cmd_send_record_call_end_rpt( int argc, char *argv[] );			//403
//conference
static int _ipmssim_cmd_make_conf_add_party_rpt( int argc, char *argv[] );			//609
static int _ipmssim_cmd_send_conf_add_party_rpt( int argc, char *argv[] );			//609
static int _ipmssim_cmd_make_conf_remove_party_rpt( int argc, char *argv[] );		//610
static int _ipmssim_cmd_send_conf_remove_party_rpt( int argc, char *argv[] );		//610
static int _ipmssim_cmd_make_conf_change_party_media_rpt( int argc, char *argv[] );	//611
static int _ipmssim_cmd_send_conf_change_party_media_rpt( int argc, char *argv[] );	//611
static int _ipmssim_cmd_make_conf_change_option_rpt( int argc, char *argv[] );		//
static int _ipmssim_cmd_send_conf_change_option_rpt( int argc, char *argv[] );		//
static int _ipmssim_cmd_make_conf_close_conf_rpt( int argc, char *argv[] );			//612
static int _ipmssim_cmd_send_conf_close_conf_rpt( int argc, char *argv[] );			//612

static int _ipmssim_cmd_send_test_rpt(int argc, char *argv[]);


// static int _ipmssim_cmd_set_trcid_date( int argc, char *argv[] );
// static int _ipmssim_cmd_set_trcid_sequence( int argc, char *argv[] );
// static int _ipmssim_cmd_send_status_inquery( int argc, char *argv[] );
static int _ipmssim_cmd_send_balance_report( int argc, char *argv[] );
static int _ipmssim_cmd_make_balance_report( int argc, char *argv[] );
// static int _ipmssim_cmd_performance_test( int argc, char *argv[] );


simcmd_reg_t _ipmssim_cmd[] = {
	{ "tcpip-start",			_ipmssim_cmd_tcpip_start,			"TCP/IP server start.\nex) tcpip-start \"IP address\" \"port\"" },
	{ "tcpip-stop",				_ipmssim_cmd_tcpip_stop,			"TCP/IP server stop." },
	{ "get-status",				_ipmssim_cmd_get_status,			"get connection / configure status." },
	{ "set-display-on",			_ipmssim_cmd_set_display_on,		"set display mode on." },
	{ "set-display-off",		_ipmssim_cmd_set_display_off,		"set display mode off." },
	{ "set-send-msg-on",		_ipmssim_cmd_set_send_msg_on,		"set send response message mode on." },
	{ "set-send-msg-off",		_ipmssim_cmd_set_send_msg_off,		"set send response message mode off." },
	{ "set-send-res-succ",		_ipmssim_cmd_set_send_res_succ,		"set send success response message." },
	{ "set-send-res-err",		_ipmssim_cmd_set_send_res_err,		"set send error response message." },
	{ "set-heartbeat-time",		_ipmssim_cmd_set_heartbeat_time,	"set heartbeat time.\nex) set-heartbeat-time \"time(sec)\"" },
	{ "set-heartbeat-display-on",		_ipmssim_cmd_set_heartbeat_display_on,		"set heartbeat display.\nex) set-heartbeat-display on" },
	{ "set-heartbeat-display-off",		_ipmssim_cmd_set_heartbeat_display_off,		"set heartbeat display.\nex) set-heartbeat-display off" },
	{ "set-timeout-time",		_ipmssim_cmd_set_timeout_time,		"set heartbeat time.\nex) set-heartbeat-time \"time(sec)\"" },

	{ "make-call-service_status-report",	_ipmssim_cmd_make_call_service_status_rpt,	"make call - call service status report message.\nex) make-call-service-status-report \"file name\"" },
	{ "send-call-service-status-report",	_ipmssim_cmd_send_call_service_status_rpt,	"send call - call service status report message.\nex) send-call-service_status-report \"file name\"" },
	{ "make-call-end-rpt",	_ipmssim_cmd_make_call_call_end_rpt,	"make call - call end report message.\nex) make-call-end-rpt \"file name\"" },
	{ "send-call-end-rpt",	_ipmssim_cmd_send_call_call_end_rpt,	"send call - call end report message.\nex) send-call-end-rpt \"file name\"" },
	{ "make-call-recording_start-rpt",	_ipmssim_cmd_make_call_recording_start_rpt,	"make call - call recording start report message.\nex) make-call-recording-start-rpt \"file name\"" },
	{ "send-call-recording-start-rpt",	_ipmssim_cmd_send_call_recording_start_rpt,	"send call - call recording start report message.\nex) send-call-recording-start-rpt \"file name\"" },
	{ "make-call-recording_end-rpt",	_ipmssim_cmd_make_call_recording_end_rpt,	"make call - call recording end report message.\nex) make-call-recording-end-rpt \"file name\"" },
	{ "send-call-recording-end-rpt",	_ipmssim_cmd_send_call_recording_end_rpt,	"send call - call recording end report message.\nex) send-call_recording-end-rpt \"file name\"" },

	{ "make-record-service-status-rpt",	_ipmssim_cmd_make_record_service_status_rpt,	"make record - service status report message.\nex) make-record-service-status-rpt \"file name\"" },
	{ "send-record-service-status-rpt",	_ipmssim_cmd_send_record_service_status_rpt,	"send record - service status report message.\nex) send-record-service-status-rpt \"file name\"" },
	{ "make-record-call-start-rpt",	_ipmssim_cmd_make_record_call_start_rpt,	"make record - call start report message.\nex) make-record-call-start-rpt \"file name\"" },
	{ "send-record-call-start-rpt",	_ipmssim_cmd_send_record_call_start_rpt,	"send record - call start report message.\nex) send-record-call-start-rpt \"file name\"" },
	{ "make-record-call-end-rpt",	_ipmssim_cmd_make_record_call_end_rpt,	"make record - call end report message.\nex) make-record-call-end-rpt \"file name\"" },
	{ "send-record-call-end-rpt",	_ipmssim_cmd_send_record_call_end_rpt,	"send record - call end report message.\nex) send-record-call-end-rpt \"file name\"" },

	{ "make-conf-add-party-rpt",	_ipmssim_cmd_make_conf_add_party_rpt,	"make conference - add party report message.\nex) make-conf-add-party-rpt \"file name\"" },
	{ "send-conf-add-party-rpt",	_ipmssim_cmd_send_conf_add_party_rpt,	"send conference - add party report message.\nex) send-conf-add-party-rpt \"file name\"" },
	{ "make-conf-remove-party-rpt",	_ipmssim_cmd_make_conf_remove_party_rpt,	"make conference - remove party report message.\nex) make-conf-remove-party-rpt \"file name\"" },
	{ "send-conf-remove-party-rpt",	_ipmssim_cmd_send_conf_remove_party_rpt,	"send conference - remove party report message.\nex) send-conf-remove-party-rpt \"file name\"" },
	{ "make-conf-change-party-media-rpt",	_ipmssim_cmd_make_conf_change_party_media_rpt,	"make conference - change party media report message.\nex) make-conf-change-party-media-rpt \"file name\"" },
	{ "send-conf-change-party-media-rpt",	_ipmssim_cmd_send_conf_change_party_media_rpt,	"send conference - change party media report message.\nex) send-conf-change-party-media-rpt \"file name\"" },
	{ "make-conf-change-option-rpt",	_ipmssim_cmd_make_conf_change_option_rpt,	"make conference - change option report message.\nex) make-conf-change-option-rpt \"file name\"" },
	{ "send-conf-change-option-rpt",	_ipmssim_cmd_send_conf_change_option_rpt,	"send conference - change option report message.\nex) send-conf-change-option-rpt \"file name\"" },
	{ "make-conf-close-conf-rpt",	_ipmssim_cmd_make_conf_close_conf_rpt,	"make conference - close conf report message.\nex) make-conf-close-conf-rpt \"file name\"" },
	{ "send-conf-close-conf-rpt",	_ipmssim_cmd_send_conf_close_conf_rpt,	"send conference - close conf report message.\nex) send-conf-close-conf-rpt \"file name\"" },

	{ "send-test-rpt",	_ipmssim_cmd_send_test_rpt,	"send - test report message.\nex) conf-close-conf-rpt \"file name\"" },

	// { "set-trcid-date",			_ipmssim_cmd_set_trcid_date,		"set date of transaction id.\nex) set-trcid-date \"Date\"" },
	// { "set-trcid-sequence",		_ipmssim_cmd_set_trcid_sequence,	"set sequence of transaction id.\nex) set-trcid-sequence \"Sequence\"" },
	{ "send-balance-report",	_ipmssim_cmd_send_balance_report,	"send balance report message.\nex) send-balance-report \"file name\"" },
	{ "make-balance-report",	_ipmssim_cmd_make_balance_report,	"make balance report body file.\nex) make-balance-report \"file name\"" },
	// { "performance-test",		_ipmssim_cmd_performance_test,		"send balance report message for performance test.\n"
	// 																"ex) performance-test \"file name\" \"tps\" \"max send count\"" },
	{ NULL, NULL, NULL } // sentinel
};


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

int ipmssim_cmd_init( char *prompt, int sleep_time )
{
	int rv;

	_g_prompt = prompt ? prompt : "";

	_g_log_id = LOG_open( NULL, LOG_MODE_STDOUT );
	LOG_setCriteriaMask( _g_log_id, _g_display_flag ? 0xff : 0x00 );

	rv = simcmd_register( SIMCMD_TYPE_RL, "ipmssim", _ipmssim_cmd );
	if( rv != eSIMCMD_SUCCESS ) {
		printf( "%ssimcmd_register() failed. err=%d(%s)\n", _g_prompt, rv, simcmd_strerror(rv));
		return rv;
	}

	return 0;
}

int ipmssim_cmd_start()
{
	int rv, i;
	// time_t now, hb_prev = 0, tps_prev = 0;
	skb_msg_t msg;

	while( !_g_stop_flag ) {
		rv = simcmd_readline();

		for( i = 0 ; i < 10 ; i ++ ) {
			rv = ipmssim_tcpip_read( &msg );
			if( rv >= 0 ) ipmssim_proc_msg( &msg, rv/*sock_idx*/ );
		}

		// now = ipmssim_get_now_time_msec();
		// if( now >= hb_prev + 100 ) {
		// 	ipmssim_proc_heartbeat();
		// 	hb_prev = now;
		// }
		// if( now >= tps_prev + 1000 ) {
		// 	if( _g_tps_proc_flag ) {
		// 		for( i = 0 ; i < _g_tps ; i ++ ) {
		// 			ipmssim_proc_balance_report( _g_fname, -1/*idx*/ );
		// 			if( i % 10 == 0 ) {
		// 				if( _g_tps <= 1000 ) usleep( 100 );
		// 				if( _g_tps <= 100 ) usleep( 1000 ); 
		// 			} else {
		// 				if( _g_tps <= 10 ) usleep( 10000 );
		// 			}
		// 			_g_cnt += 1;
		// 			if( _g_cnt >= _g_max ) break;
		// 		}
		// 		if( _g_cnt >= _g_max ) {
		// 			_g_tps_proc_flag = 0;
		// 			_g_fname[0] = '\0';
		// 			_g_tps = 0;
		// 			_g_max = 0;
		// 			_g_cnt = 0;
		// 		}
		// 	}
		// 	tps_prev = now;
		// }
	}

	return 0;
}

void ipmssim_cmd_stop()
{
	_g_stop_flag = 1;
}

int ipmssim_cmd_run( int argc, char *argv[] )
{
	int i;

	if( argc < 1 ) return eSIMCMD_INVALID_PARAM;
	if( !argv ) return eSIMCMD_INVALID_PARAM;

	for( i = 0 ; _ipmssim_cmd[i].name && _ipmssim_cmd[i].name[0] ; i ++ ) {
		if( !strcasecmp(argv[0], _ipmssim_cmd[i].name) ) {
			return _ipmssim_cmd[i].func( argc, argv );
		}
	}

	return eSIMCMD_CMD_NOT_FOUND;
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

// static void _ipmssim_handle_sigint( int sig )
// {
// 	_ipmssim_cmd_exit( 0, NULL );
// 	signal( sig, _ipmssim_handle_sigint );
// }


// static int _ipmssim_cmd_exit( int argc, char *argv[] )
// {
// 	printf( "%sExit simulator.\n", _g_prompt );
// 	ipmssim_cmd_stop();
// 	simcmd_init( NULL, 0 );
// 	system("stty echo");
// 	return eSIMCMD_SUCCESS;
// }


static int _ipmssim_cmd_tcpip_start( int argc, char *argv[] )
{
	unsigned short port[3];

	if( argc < 4 ) return eSIMCMD_INVALID_PARAM;

	port[0] = (unsigned short)strtol(argv[1], 0, 0);
	port[1] = (unsigned short)strtol(argv[2], 0, 0);
	port[2] = (unsigned short)strtol(argv[3], 0, 0);
	ipmssim_tcpip_start( "0.0.0.0", port );

	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_tcpip_stop( int argc, char *argv[] )
{
	ipmssim_tcpip_stop();
	return eSIMCMD_SUCCESS;
}


static int _ipmssim_cmd_get_status( int argc, char *argv[] )
{
	ipmssim_tcpip_display();

	printf( "===== CONFIG ================================\n" );
	printf( "display mode        : %s\n", _g_display_flag ? "ON" : "OFF" );
	printf( "send msg mode       : %s\n", ipmssim_get_send_msg_flag() ? "ON" : "OFF" );
	printf( "send response       : %s\n", ipmssim_get_send_succ_flag() ? "Success" : "Error" );
	printf( "heartbeat time      : %d\n", ipmssim_get_heartbeat_time() );
	printf( "heartbeat display   : %s\n", ipmssim_get_heartbeat_display() ? "ON" : "OFF" );
	printf( "timeout time        : %d\n", ipmssim_get_timeout_time() );
 

	// printf( "stsinqack seq mode  : %shange sequence to last received sequence\n", 
	// 		ipmssim_get_stsinqack_seq_chg_flag() ? "C" : "Don't c" );
	// printf( "trcid date          : %s\n", ipmssim_get_date() ); 
	// printf( "trcid sequence      : %d\n", ipmssim_get_sequence() ); 
	// printf( "usgrep timeout mode : %s\n", ipmssim_get_usgrep_timeout_flag() ? "ON" : "OFF" );
	printf( "=============================================\n" );

	// if( _g_tps_proc_flag ) {
	// 	printf( "===== PERFORMANCE TEST ======================\n" );
	// 	printf( "tps                 : %d\n", _g_tps );
	// 	printf( "max count           : %d\n", _g_max );
	// 	printf( "current count       : %d\n", _g_cnt );
	// 	printf( "=============================================\n" );
	// }

	return eSIMCMD_SUCCESS;
}


static int _ipmssim_cmd_set_display_on( int argc, char *argv[] )
{
	_g_display_flag = 1;
	LOG_setCriteriaMask( _g_log_id, _g_display_flag ? 0xff : 0x00 );
	printf( "set display mode on.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_set_display_off( int argc, char *argv[] )
{
	_g_display_flag = 0;
	LOG_setCriteriaMask( _g_log_id, _g_display_flag ? 0xff : 0x00 );
	printf( "set display mode off.\n" );
	return eSIMCMD_SUCCESS;
}


static int _ipmssim_cmd_set_send_msg_on( int argc, char *argv[] )
{
	ipmssim_set_send_msg_flag( 1 );
	printf( "set send msg mode on.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_set_send_msg_off( int argc, char *argv[] )
{
	ipmssim_set_send_msg_flag( 0 );
	printf( "set send msg mode off.\n" );
	return eSIMCMD_SUCCESS;
}


static int _ipmssim_cmd_set_send_res_succ( int argc, char *argv[] )
{
	ipmssim_set_send_succ_flag( 1 );
	printf( "set send success response.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_set_send_res_err( int argc, char *argv[] )
{
	ipmssim_set_send_succ_flag( 0 );
	printf( "set send error response.\n" );
	return eSIMCMD_SUCCESS;
}


static int _ipmssim_cmd_set_heartbeat_time( int argc, char *argv[] )
{
	int hbtime;

	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

	hbtime = strtol( argv[1], 0, 0 );
	ipmssim_set_heartbeat_time( hbtime );
	if( hbtime != ipmssim_get_heartbeat_time() ) {
		printf( "set heartbeat time failed.\n" );
	} else {
		printf( "set heartbeat time success.\n" );
	}
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_set_timeout_time( int argc, char *argv[] )
{
	int totime;

	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

	totime = strtol( argv[1], 0, 0 );
	ipmssim_set_timeout_time( totime );
	if( totime != ipmssim_get_timeout_time() ) {
		printf( "set heartbeat time failed.\n" );
	} else {
		printf( "set heartbeat time success.\n" );
	}
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_set_heartbeat_display_on( int argc, char *argv[] )
{
	ipmssim_set_heartbeat_display( 1 );
	if( 1 != ipmssim_get_heartbeat_display() ) {
		printf( "set heartbeat display failed.\n" );
	} else {
		printf( "set heartbeat display success.\n" );
	}
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_set_heartbeat_display_off( int argc, char *argv[] )
{
	ipmssim_set_heartbeat_display( 0 );
	if( 0 != ipmssim_get_heartbeat_display() ) {
		printf( "set heartbeat display failed.\n" );
	} else {
		printf( "set heartbeat display success.\n" );
	}
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_call_service_status_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_call_service_status_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_call_service_status_rpt( argv[1]);
	if( rv < 0 ) printf( "send call service status report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_call_call_end_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_call_call_end_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_call_call_end_rpt( argv[1]);
	if( rv < 0 ) printf( "send call end report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_call_recording_start_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_call_recording_start_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_call_recording_start_rpt( argv[1]);
	if( rv < 0 ) printf( "send call recording start report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_call_recording_end_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_call_recording_end_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_call_recording_end_rpt( argv[1]);
	if( rv < 0 ) printf( "send call recording end report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_record_service_status_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_record_service_status_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_record_service_status_rpt( argv[1]);
	if( rv < 0 ) printf( "send record serivce status report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_record_call_start_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_record_call_start_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_record_call_start_rpt( argv[1]);
	if( rv < 0 ) printf( "send record start report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_record_call_end_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_record_call_end_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_record_call_end_rpt( argv[1]);
	if( rv < 0 ) printf( "send record stop report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_conf_add_party_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_conf_add_party_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_conf_add_party_rpt( argv[1]);
	if( rv < 0 ) printf( "send conf add party report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_conf_remove_party_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_conf_remove_party_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_conf_remove_party_rpt( argv[1]);
	if( rv < 0 ) printf( "send conf remove party report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_conf_change_party_media_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_conf_change_party_media_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_conf_change_party_media_rpt( argv[1]);
	if( rv < 0 ) printf( "send conf change party media report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_conf_change_option_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_conf_change_option_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_conf_change_option_rpt( argv[1]);
	if( rv < 0 ) printf( "send conf change option report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_conf_close_conf_rpt( int argc, char *argv[] ) {
	printf( "unsupported.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_conf_close_conf_rpt( int argc, char *argv[] ) {
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_send_conf_close_conf_rpt( argv[1]);
	if( rv < 0 ) printf( "send conf close conf report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_send_test_rpt(int argc, char *argv[]) {
	int chnl_idx;
	if( argc < 3 ) return eSIMCMD_INVALID_PARAM;
	chnl_idx = atoi(argv[2]);
	int rv = ipmssim_proc_send_test_rpt( argv[1], chnl_idx);
	if( rv < 0 ) printf( "send conf close conf report message failed.\n" );
	return eSIMCMD_SUCCESS;
}


// static int _ipmssim_cmd_set_stsinqack_seq_on( int argc, char *argv[] )
// {
// 	ipmssim_set_stsinqack_seq_chg_flag( 1 );
// 	printf( "set change status inquery messsage sequence to last received sequence mode on.\n" );
// 	return eSIMCMD_SUCCESS;
// }

// static int _ipmssim_cmd_set_stsinqack_seq_off( int argc, char *argv[] )
// {
// 	ipmssim_set_stsinqack_seq_chg_flag( 0 );
// 	printf( "set change status inquery messsage sequence to last received sequence mode off.\n" );
// 	return eSIMCMD_SUCCESS;
// }


// static int _ipmssim_cmd_set_trcid_date( int argc, char *argv[] )
// {
// 	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

// 	ipmssim_set_date( argv[1] );
// 	if( strcmp(argv[1], ipmssim_get_date()) ) {
// 		printf( "set date of trcid failed.\n" );
// 	} else {
// 		printf( "set date of trcid success.\n" );
// 	}
// 	return eSIMCMD_SUCCESS;
// }

// static int _ipmssim_cmd_set_trcid_sequence( int argc, char *argv[] )
// {
// 	int sequence;

// 	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;

// 	sequence = strtol( argv[1], 0, 0 );
// 	ipmssim_set_sequence( sequence );
// 	if( sequence != ipmssim_get_sequence() ) {
// 		printf( "set sequence of trcid failed.\n" );
// 	} else {
// 		printf( "set sequence of trcid success.\n" );
// 	}
// 	return eSIMCMD_SUCCESS;
// }


// static int _ipmssim_cmd_send_status_inquery( int argc, char *argv[] )
// {
// 	// int rv = ipmssim_proc_status_inquery( -1/*idx*/ );
// 	// if( rv < 0 ) printf( "send status inquery message failed.\n" );
// 	return eSIMCMD_SUCCESS;
// }

static int _ipmssim_cmd_send_balance_report( int argc, char *argv[] )
{
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_balance_report( argv[1], -1/*idx*/ );
	if( rv < 0 ) printf( "send balance report message failed.\n" );
	return eSIMCMD_SUCCESS;
}

static int _ipmssim_cmd_make_balance_report( int argc, char *argv[] )
{
	if( argc < 2 ) return eSIMCMD_INVALID_PARAM;
	int rv = ipmssim_proc_balance_report_body( argv[1] );
	if( rv < 0 ) printf( "make balance report body file failed.\n" );
	return eSIMCMD_SUCCESS;
}

// static int _ipmssim_cmd_performance_test( int argc, char *argv[] )
// {
// 	if( _g_tps_proc_flag ) {
// 		printf( "Performance test not finished yet.\n");
// 		return eSIMCMD_SUCCESS;
// 	}

// 	if( argc < 4 ) return eSIMCMD_INVALID_PARAM;

// 	if( !argv[1][0] || strlen(argv[1]) >= sizeof(_g_fname) ) return eSIMCMD_INVALID_PARAM;
// 	strcpy( _g_fname, argv[1] );

// 	_g_tps = strtol( argv[2], 0, 0 );
// 	_g_max = strtol( argv[3], 0, 0 );
// 	if( _g_tps <= 0 || _g_max <= 0 ) return eSIMCMD_INVALID_PARAM;

// 	_g_cnt = 0;
// 	_g_tps_proc_flag = 1;
// 	return eSIMCMD_SUCCESS;
// }