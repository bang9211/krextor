#ifndef __TCP_CLICKTOCALL_REQ_H__
#define __TCP_CLICKTOCALL_REQ_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_clicktocall_start_req_s tcp_clicktocall_start_req_t; 
struct tcp_clicktocall_start_req_s {
	tcp_msg_t base[1];
	char *sessionID;
	char *gwSessionID;
	char *subscriberName;
	int recordingType;
	char *callingNumber;
	char *calledNumber;
	int serviceCode;
	int ringBackToneType;
	int waitingMentID;
	int scenarioType;
	int callMentID;
	char *callingCID;
	char *calledCID;
	char *recordingFileName;
	int isAllRecording;
	int endIfRecordingFailed;
	int endIfRecordingEnded;
	int hostingCode;
	int wirelessTimeout;
	int wiredTimeout;
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

int tcp_clicktocall_start_req_init( tcp_clicktocall_start_req_t *clicktocall_start_req);
void tcp_clicktocall_start_req_final( tcp_clicktocall_start_req_t *clicktocall_start_req);

int tcp_clicktocall_start_req_decode_msg( tcp_clicktocall_start_req_t *clicktocall_start_req, tcp_msg_t *msg);


int tcp_clicktocall_stop_req_init( tcp_clicktocall_stop_req_t *clicktocall_stop_req);
void tcp_clicktocall_stop_req_final( tcp_clicktocall_stop_req_t *clicktocall_stop_req);

int tcp_clicktocall_stop_req_decode_msg( tcp_clicktocall_stop_req_t *clicktocall_stop_req, tcp_msg_t *msg);


int tcp_clicktocall_startrecording_req_init( tcp_clicktocall_startrecording_req_t *clicktocall_startrecording_req);
void tcp_clicktocall_startrecording_req_final( tcp_clicktocall_startrecording_req_t *clicktocall_startrecording_req);

int tcp_clicktocall_startrecording_req_decode_msg( tcp_clicktocall_startrecording_req_t *clicktocall_startrecording_req, tcp_msg_t *msg);


int tcp_clicktocall_stoprecording_req_init( tcp_clicktocall_stoprecording_req_t *clicktocall_stoprecording_req);
void tcp_clicktocall_stoprecording_req_final( tcp_clicktocall_stoprecording_req_t *clicktocall_stoprecording_req);

int tcp_clicktocall_stoprecording_req_decode_msg( tcp_clicktocall_stoprecording_req_t *clicktocall_stoprecording_req, tcp_msg_t *msg);

#endif // #ifndef __TCP_CLICKTOCALL_REQ_H__