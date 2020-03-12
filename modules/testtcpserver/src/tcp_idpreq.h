#ifndef __TCP_IDPREQ_H__
#define __TCP_IDPREQ_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// InitialDP Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_idpreq_s tcp_idpreq_t; 
struct tcp_idpreq_s {
	tcp_msg_t base[1];
	int ccrCount;
	int continueFlag;
	char *msisdn;
	char *imsi;
	char *called;
	char *dialed;
	int locationInfoLen;
	char *locationInfo;
	int timeAndTimeZoneLen;
	char *timeAndTimeZone;
	int startTime;
	char *vlrNumber;
	int redirInfo;
	int vpnFlag;
	char *accCode;
	int usedTime;
	int bearerSvcLen;
	char *bearerSvc;
	int bUcb;
};

///////////////////////////////////////////////////////////////////////////////////
// InitialDP Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_idpreq_init( tcp_idpreq_t *idpreq);
void tcp_idpreq_final( tcp_idpreq_t *idpreq);

int tcp_idpreq_decode_msg( tcp_idpreq_t *idpreq, tcp_msg_t *msg);

#endif // #ifndef __TCP_IDPREQ_H__
