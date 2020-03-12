#ifndef __TCP_CIRREQ_H__
#define __TCP_CIRREQ_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// CallInformationReport Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_cirreq_s tcp_cirreq_t; 
struct tcp_cirreq_s {
	int ccrCount;
	int continueFlag;
	char *msisdn;
	char *imsi;
	char *called;
	char *dialed;
	int callTime;
	int endTime;
	int usedTime;
	int timeAndTimeZoneLen;
	char *timeAndTimeZone;
	char *locationInfo;
	int locationInfoLen;
	int reportingReason;
	int callAttemptElapsedTime;
	char *callRefNumber;
	char *scpAddr;
	char *dest;
	char *vlrNumber;
	int redirInfo;
	int vpnFlag;
	char *accCode;
	int bUcb;
};

///////////////////////////////////////////////////////////////////////////////////
// CallInformationReport Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_cirreq_init( tcp_cirreq_t *cirreq);
void tcp_cirreq_final( tcp_cirreq_t *cirreq);

int tcp_cirreq_decode_msg( tcp_cirreq_t *cirreq, tcp_msg_t *msg);

#endif // #ifndef __TCP_CIRREQ_H__
