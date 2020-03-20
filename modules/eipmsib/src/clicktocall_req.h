#ifndef __CLICKTOCALL_REQ_H__
#define __CLICKTOCALL_REQ_H__

#include <string.h>
#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocall_start_req_tcp_s clicktocall_start_req_tcp_t; 
// #pragma pack(push, 1)
struct clicktocall_start_req_tcp_s {
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

typedef struct tcp_clicktocall_stop_req_s clicktocall_stop_req_tcp_t; 
struct tcp_clicktocall_stop_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
	char *recordingFileName;
};

typedef struct tcp_clicktocall_startrecording_req_s clicktocall_startrecording_req_tcp_t; 
struct tcp_clicktocall_startrecording_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
	char *recordingFileName;
};

typedef struct tcp_clicktocall_stoprecording_req_s clicktocall_stoprecording_req_tcp_t; 
struct tcp_clicktocall_stoprecording_req_s {
	tcp_msg_t base[1];
	int networkType;
	char *sessionID;
	char *gwSessionID;
};

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_start_req_tcp_init( clicktocall_start_req_tcp_t *clicktocall_start_req);
void clicktocall_start_req_tcp_final( clicktocall_start_req_tcp_t *clicktocall_start_req);
int clicktocall_start_req_decode_dbif_msg( clicktocall_start_req_tcp_t *clicktocall_start_req, char* sessionID, char* gwSessionID, uxc_dbif_t *dbif);
void clicktocall_start_req_tcp_display(clicktocall_start_req_tcp_t *clicktocall_start_req);
void clicktocall_start_req_dbif_display(uxc_dbif_t *dbif);


int clicktocall_stop_req_tcp_init( clicktocall_stop_req_tcp_t *clicktocall_stop_req);
void clicktocall_stop_req_tcp_final( clicktocall_stop_req_tcp_t *clicktocall_stop_req);
int clicktocall_stop_req_decode_dbif_msg( clicktocall_stop_req_tcp_t *clicktocall_stop_req, tcp_msg_t *msg);


int clicktocall_startrecording_req_tcp_init( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req);
void clicktocall_startrecording_req_tcp_final( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req);
int clicktocall_startrecording_req_decode_dbif_msg( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req, tcp_msg_t *msg);


int clicktocall_stoprecording_req_tcp_init( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req);
void clicktocall_stoprecording_req_tcp_final( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req);
int clicktocall_stoprecording_req_decode_dbif_msg( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req, tcp_msg_t *msg);

#endif // #ifndef __CLICKTOCALL_REQ_H__
