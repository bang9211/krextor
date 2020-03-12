#ifndef __TCP_IDPRSP_H__
#define __TCP_IDPRSP_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_idp Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_idprsp_s tcp_idprsp_t; 
struct tcp_idprsp_s {
	int result;
	int callTime;
	int finalUnitAction;
	int lang;
	char annStr[256];
	int brandId;
	char remainsStr[64]; 
};


///////////////////////////////////////////////////////////////////////////////////
// tcp_idp Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_idprsp_encode_ipcmsg( tcp_idprsp_t *idprsp, tcp_msg_t *msg);

int tcp_idprsp_init( tcp_idprsp_t *idprsp);
void tcp_idprsp_final( tcp_idprsp_t *idprsp);

#endif // #ifndef __TCP_ IDPRSP_H__
