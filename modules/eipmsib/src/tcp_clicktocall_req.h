#ifndef __TCP_CLICKTOCALL_REQ_H__
#define __TCP_CLICKTOCALL_REQ_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_clicktocall_start_req_s tcp_clicktocall_start_req_t; 
struct tcp_clicktocall_start_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
	char *callingNumber;
	char *calledNumber;
	int recording;
	char *subscirberName;
	char *chargingNumber;
	int ringBackToneType;
	int waitingMentID;
	int callMentID;
	char *callingCID;
	char *calledCID;
	int hostingCode;
	int scenarioType;
	char *recordingFileName;
};

typedef struct tcp_clicktocall_stop_req_s tcp_clicktocall_stop_req_t; 
struct tcp_clicktocall_stop_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
	char *recordingFileName;
};

typedef struct tcp_clicktocall_startrecording_req_s tcp_clicktocall_startrecording_req_t; 
struct tcp_clicktocall_startrecording_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
	char *recordingFileName;
};

typedef struct tcp_clicktocall_stoprecording_req_s tcp_clicktocall_stoprecording_req_t; 
struct tcp_clicktocall_stoprecording_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
};

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_clicktocall_start_init( tcp_clicktocall_start_req_t *clicktocall_start_req);
void tcp_clicktocall_start_final( tcp_clicktocall_start_req_t *clicktocall_start_req);

int tcp_clicktocall_start_decode_msg( tcp_clicktocall_start_req_t *clicktocall_start_req, tcp_msg_t *msg);


int tcp_clicktocall_stop_init( tcp_clicktocall_stop_req_t *clicktocall_stop_req);
void tcp_clicktocall_stop_final( tcp_clicktocall_stop_req_t *clicktocall_stop_req);

int tcp_clicktocall_stop_decode_msg( tcp_clicktocall_stop_req_t *clicktocall_stop_req, tcp_msg_t *msg);


int tcp_clicktocall_startrecording_init( tcp_clicktocall_startrecording_req_t *clicktocall_startrecording_req);
void tcp_clicktocall_startrecording_final( tcp_clicktocall_startrecording_req_t *clicktocall_startrecording_req);

int tcp_clicktocall_startrecording_decode_msg( tcp_clicktocall_startrecording_req_t *clicktocall_startrecording_req, tcp_msg_t *msg);


int tcp_clicktocall_stoprecording_init( tcp_clicktocall_stoprecording_req_t *clicktocall_stoprecording_req);
void tcp_clicktocall_stoprecording_final( tcp_clicktocall_stoprecording_req_t *clicktocall_stoprecording_req);

int tcp_clicktocall_stoprecording_decode_msg( tcp_clicktocall_stoprecording_req_t *clicktocall_stoprecording_req, tcp_msg_t *msg);

#endif // #ifndef __TCP_CLICKTOCALL_REQ_H__
