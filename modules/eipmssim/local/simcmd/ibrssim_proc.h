#ifndef __IPMSSIM_PROC_H__
#define __IPMSSIM_PROC_H__


#include "ipmssim_msg.h"


int ipmssim_get_send_msg_flag();
int ipmssim_get_send_succ_flag();
int ipmssim_get_heartbeat_time();
int ipmssim_get_stsinqack_seq_chg_flag();
int ipmssim_get_sequence();
char *ipmssim_get_date();
int ipmssim_get_usgrep_timeout_flag();
void ipmssim_set_send_msg_flag( int flag );
void ipmssim_set_send_succ_flag( int flag );
void ipmssim_set_heartbeat_time( int hbtime );
void ipmssim_set_stsinqack_seq_chg_flag( int flag );
void ipmssim_set_sequence( int sequence );
void ipmssim_set_date( char *date );
void ipmssim_set_usgrep_timeout_flag( int flag );

void ipmssim_proc_heartbeat();
int ipmssim_proc_msg( ipmssim_msg_t *rcvmsg, int idx );
// int ipmssim_proc_status_inquery( int idx );
// int ipmssim_proc_balance_report( const char *fname, int idx );
// int ipmssim_proc_balance_report_body( const char *fname );


#endif // #ifndef __IPMSSIM_PROC_H__
