#ifndef __TCP_CIRRSP_H__
#define __TCP_CIRRSP_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_cir Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_cirrsp_s tcp_cirrsp_t; 
struct tcp_cirrsp_s {
	int result;
	int callTime;
	int finalUnitAction;
	int lang;
	char annStr[256];
	int brandId;
	char remainsStr[64]; 
};

///////////////////////////////////////////////////////////////////////////////////
// tcp_cir Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_cirrsp_encode_ipcmsg( tcp_cirrsp_t *cirrsp, tcp_msg_t *msg);

int tcp_cirrsp_init( tcp_cirrsp_t *cirrsp);
void tcp_cirrsp_final( tcp_cirrsp_t *cirrsp);

#endif // #ifndef __TCP_ CIRRSP_H__
