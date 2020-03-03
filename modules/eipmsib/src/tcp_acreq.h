#ifndef __TCP_ACREQ_H__
#define __TCP_ACREQ_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// ApplyCharing Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_acreq_s tcp_acreq_t; 
struct tcp_acreq_s {
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
// ApplyCharging Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_acreq_init( tcp_acreq_t *acreq);
void tcp_acreq_final( tcp_acreq_t *acreq);

int tcp_acreq_decode_msg( tcp_acreq_t *acreq, tcp_msg_t *msg);

#endif // #ifndef __TCP_ACREQ_H__
