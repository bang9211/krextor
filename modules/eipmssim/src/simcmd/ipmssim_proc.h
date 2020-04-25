#ifndef __IPMSSIM_PROC_H__
#define __IPMSSIM_PROC_H__

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ua_log.h>
#include "cJSON.h"
#include "ipmssim_tcpip.h"
#include "ipmssim_util.h"
#include "ipmssim_msg.h"
#include "ipmssim_msg_call.h"
#include "ipmssim_msg_record.h"
#include "ipmssim_msg_conf.h"
#include "../../../SKBGW/modules/eipmsib/src/skb_msg.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktocall_req.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktocall_rsp.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktocall_rpt.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktocallrecording_req.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktocallrecording_rsp.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktocallrecording_rpt.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktoconference_req.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktoconference_rsp.h"
#include "../../../SKBGW/modules/eipmsib/src/clicktoconference_rpt.h"


int ipmssim_get_send_msg_flag();
int ipmssim_get_send_succ_flag();
int ipmssim_get_heartbeat_time();
int ipmssim_get_heartbeat_timer();
int ipmssim_get_heartbeat_display();
int ipmssim_get_timeout_time();
int ipmssim_get_stsinqack_seq_chg_flag();
int ipmssim_get_sequence();
char *ipmssim_get_date();
int ipmssim_get_usgrep_timeout_flag();
void ipmssim_set_send_msg_flag( int flag );
void ipmssim_set_send_succ_flag( int flag );
void ipmssim_set_heartbeat_time( int hbtime );
void ipmssim_set_heartbeat_timer_on();
void ipmssim_set_heartbeat_timer_off();
void ipmssim_set_timeout_time( int hbtime );
void ipmssim_set_heartbeat_display( int hbd );
void ipmssim_set_stsinqack_seq_chg_flag( int flag );
void ipmssim_set_sequence( int sequence );
void ipmssim_set_date( char *date );
void ipmssim_set_usgrep_timeout_flag( int flag );

void ipmssim_proc_heartbeat();
int ipmssim_proc_msg( skb_msg_t *rcvmsg, int idx );
// int ipmssim_proc_status_inquery( int idx );
int ipmssim_proc_balance_report( const char *fname, int idx );
int ipmssim_proc_balance_report_body( const char *fname );

int ipmssim_proc_make_call_service_status_rpt( const char * fname);
int ipmssim_proc_send_call_service_status_rpt( const char * fname);
int ipmssim_proc_make_call_call_end_rpt( const char * fname);
int ipmssim_proc_send_call_call_end_rpt( const char * fname);
int ipmssim_proc_make_call_recording_start_rpt( const char * fname);
int ipmssim_proc_send_call_recording_start_rpt( const char * fname);
int ipmssim_proc_make_call_recording_end_rpt( const char * fname);
int ipmssim_proc_send_call_recording_end_rpt( const char * fname);

int ipmssim_proc_make_record_service_status_rpt( const char * fname);
int ipmssim_proc_send_record_service_status_rpt( const char * fname);
int ipmssim_proc_make_record_call_start_rpt( const char * fname);
int ipmssim_proc_send_record_call_start_rpt( const char * fname);
int ipmssim_proc_make_record_call_end_rpt( const char * fname);
int ipmssim_proc_send_record_call_end_rpt( const char * fname);

int ipmssim_proc_make_conf_add_party_rpt( const char * fname);
int ipmssim_proc_send_conf_add_party_rpt( const char * fname);
int ipmssim_proc_make_conf_remove_party_rpt( const char * fname);
int ipmssim_proc_send_conf_remove_party_rpt( const char * fname);
int ipmssim_proc_make_conf_change_party_media_rpt( const char * fname);
int ipmssim_proc_send_conf_change_party_media_rpt( const char * fname);
int ipmssim_proc_make_conf_change_option_rpt( const char * fname);
int ipmssim_proc_send_conf_change_option_rpt( const char * fname);
int ipmssim_proc_make_conf_close_conf_rpt( const char * fname);
int ipmssim_proc_send_conf_close_conf_rpt( const char * fname);

int ipmssim_proc_get_json_from_file( const char * fname, cJSON **json);

int ipmssim_proc_send_test_rpt( const char * fname, int chnl_idx);


#endif // #ifndef __IPMSSIM_PROC_H__
