#ifndef __TCP_MSG_H__
#define __TCP_MSG_H__
#include "tcp_conf.h"
#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>

///////////////////////////////////////////////////////////////////////////////////
// TCP Common Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

#define TCP_MSGID_CLICKTOCALL_START 								100
#define TCP_MSGID_CLICKTOCALL_STOP 									101
#define TCP_MSGID_CLICKTOCALL_STARTRECORDING 						102
#define TCP_MSGID_CLICKTOCALL_STOPRECORDING 						103

////////////////////////////////////////////////////////////////////////////////////
// functions for tcp_msg_t
////////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_msg_s tcp_msg_t;
struct tcp_msg_s{
	uxc_ixpc_t header; 
	char data[UXC_IPCMSG_DATA_LEN];
};	

int tcp_msg_cvt_order_hton(tcp_msg_t *msg);
int tcp_msg_cvt_order_ntoh(tcp_msg_t *msg);
int tcp_msg_display( tcp_msg_t *msg);
int tcp_msg_send( tcp_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey);

#endif // #ifndef __TCP_MSG_H__
