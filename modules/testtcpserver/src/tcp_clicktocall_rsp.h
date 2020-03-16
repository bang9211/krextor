#ifndef __TCP_CLICKTOCALL_RSP_H__
#define __TCP_CLICKTOCALL_RSP_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_clicktocall_start_rsp_s tcp_clicktocall_start_rsp_t; 
struct tcp_clicktocall_start_rsp_s {
	int result;
	int callTime;
	int finalUnitAction;
	int lang;
	char annStr[256];
	int brandId;
	char remainsStr[64]; 
};


///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_clicktocall_start_rsp_encode_ipcmsg( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp, tcp_msg_t *msg);

int tcp_clicktocall_start_rsp_init( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp);
void tcp_clicktocall_start_rsp_final( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp);

#endif // #ifndef __TCP_ CLICKTOCALL_RSP_H__
