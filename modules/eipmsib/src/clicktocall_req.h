#ifndef __CLICKTOCALL_REQ_H__
#define __CLICKTOCALL_REQ_H__

#include <string.h>
#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocall_binding_req_tcp_s clicktocall_binding_req_tcp_t;
struct clicktocall_binding_req_tcp_s {
	char userID[BINDING_USER_ID_LEN];
	char password[BINDING_PASSWORD_LEN];
};

typedef struct clicktocall_start_req_tcp_s clicktocall_start_req_tcp_t; 
struct clicktocall_start_req_tcp_s {
	char subscriberName[SUBSCRIBER_NAME_LEN];
	int8_t recordingType;
	char callingNumber[CALL_NUMBER_LEN];
	char calledNumber[CALL_NUMBER_LEN];
	char chargingNumber[CHARGING_NUMBER_LEN];
	int8_t serviceCode;
	int8_t ringBackToneType;
	int16_t waitingMentID;
	int8_t filler1;
	int8_t scenarioType;
	int16_t callMentID;
	char callingCID[CID_LEN];
	char calledCID[CID_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
	int8_t isAllRecording;
	int8_t endIfRecordingFailed;
	int8_t endIfRecordingEnded;
	int8_t hostingCode;
	int8_t wirelessTimeout;
	int8_t wiredTimeout;
	int16_t filler2;
	char filler3[32];
};

typedef struct clicktocall_stop_req_tcp_s clicktocall_stop_req_tcp_t; 
struct clicktocall_stop_req_tcp_s {
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocall_startrecording_req_tcp_s clicktocall_startrecording_req_tcp_t; 
struct clicktocall_startrecording_req_tcp_s {
	char serviceID[SERVICE_ID_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
};

typedef struct clicktocall_stoprecording_req_tcp_s clicktocall_stoprecording_req_tcp_t; 
struct clicktocall_stoprecording_req_tcp_s {
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocall_service_status_req_tcp_s clicktocall_service_status_req_tcp_t; 
struct clicktocall_service_status_req_tcp_s {
	char serviceID[SERVICE_ID_LEN];
};



///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////
int clicktocall_binding_req_tcp_init( clicktocall_binding_req_tcp_t *clicktocall_binding_req);
void clicktocall_binding_req_tcp_final( clicktocall_binding_req_tcp_t *clicktocall_binding_req);
void clicktocall_binding_req_tcp_display(char *headerStr, clicktocall_binding_req_tcp_t *clicktocall_binding_req);

int clicktocall_start_req_tcp_init( clicktocall_start_req_tcp_t *clicktocall_start_req);
void clicktocall_start_req_tcp_final( clicktocall_start_req_tcp_t *clicktocall_start_req);
int clicktocall_start_req_decode_dbif_msg( clicktocall_start_req_tcp_t *clicktocall_start_req, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void clicktocall_start_req_tcp_display(char *headerStr, clicktocall_start_req_tcp_t *clicktocall_start_req);
void clicktocall_start_req_dbif_display(uxc_dbif_t *dbif);

int clicktocall_stop_req_tcp_init( clicktocall_stop_req_tcp_t *clicktocall_stop_req);
void clicktocall_stop_req_tcp_final( clicktocall_stop_req_tcp_t *clicktocall_stop_req);
int clicktocall_stop_req_decode_dbif_msg( clicktocall_stop_req_tcp_t *clicktocall_stop_req, uxc_dbif_t *dbif);
void clicktocall_stop_req_tcp_display(char *headerStr, clicktocall_stop_req_tcp_t *clicktocall_stop_req);
void clicktocall_stop_req_dbif_display(uxc_dbif_t *dbif);

int clicktocall_startrecording_req_tcp_init( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req);
void clicktocall_startrecording_req_tcp_final( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req);
int clicktocall_startrecording_req_decode_dbif_msg( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req, uxc_dbif_t *dbif);
void clicktocall_startrecording_req_tcp_display(char *headerStr, clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req);
void clicktocall_startrecording_req_dbif_display(uxc_dbif_t *dbif);

int clicktocall_stoprecording_req_tcp_init( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req);
void clicktocall_stoprecording_req_tcp_final( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req);
int clicktocall_stoprecording_req_decode_dbif_msg( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req, uxc_dbif_t *dbif);
void clicktocall_stoprecording_req_tcp_display(char *headerStr, clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req);
void clicktocall_stoprecording_req_dbif_display(uxc_dbif_t *dbif);

int clicktocall_service_status_req_tcp_init( clicktocall_service_status_req_tcp_t *clicktocall_service_status_req);
void clicktocall_service_status_req_tcp_final( clicktocall_service_status_req_tcp_t *clicktocall_service_status_req);
void clicktocall_service_status_req_tcp_display(char *headerStr, clicktocall_service_status_req_tcp_t *clicktocall_service_status_req);

#endif // #ifndef __CLICKTOCALL_REQ_H__
