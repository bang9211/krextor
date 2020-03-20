#ifndef __TCP_CLICKTOCALL_REQ_H__
#define __TCP_CLICKTOCALL_REQ_H__

#include <string.h>
#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_clicktocall_start_req_s tcp_clicktocall_start_req_t; 
// #pragma pack(push, 1)
struct tcp_clicktocall_start_req_s {
	// tcp_msg_t base[1];
	char subscriberName[32];
	int8_t recordingType;
	char callingNumber[24];
	char calledNumber[24];
	char chargingNumber[24];
	int8_t serviceCode;
	int8_t ringBackToneType;
	int16_t waitingMentID;
	int8_t fillerInt8;
	int8_t scenarioType;
	int16_t callMentID;
	char callingCID[16];
	char calledCID[16];
	char recordingFileName[64];
	int8_t isAllRecording;
	int8_t endIfRecordingFailed;
	int8_t endIfRecordingEnded;
	int8_t hostingCode;
	int8_t wirelessTimeout;
	int8_t wiredTimeout;
	int16_t fillerInt16;
	char filler[32];
};
// #pragma pack(pop)

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
// void tcp_clicktocall_start_req_display_tcpmsg(tcp_msg_t *msg);


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
