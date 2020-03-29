#ifndef __CLICKTOCALLRECORDING_REQ_H__
#define __CLICKTOCALLRECORDING_REQ_H__

#include <string.h>
#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocallrecording_start_req_tcp_s clicktocallrecording_start_req_tcp_t; 
struct clicktocallrecording_start_req_tcp_s {
	char subscriberName[SUBSCRIBER_NAME_LEN];
	char callingNumber[CALL_NUMBER_LEN];
	char calledNumber[CALL_NUMBER_LEN];
    char callerInfo[CALLER_INFO_LEN];
    char calleeInfo[CALLEE_INFO_LEN];
	char chargingNumber[CHARGING_NUMBER_LEN];
	int8_t serviceCode;
    int8_t filler1;
    int16_t filler2;
	char recordingFileName[RECORDING_FILE_NAME_LEN];
    char number050[CALL_NUMBER_LEN];
	char filler3[8];
};

typedef struct clicktocallrecording_stop_req_tcp_s clicktocallrecording_stop_req_tcp_t; 
struct clicktocallrecording_stop_req_tcp_s {
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocallrecording_service_status_req_tcp_s clicktocallrecording_service_status_req_tcp_t; 
struct clicktocallrecording_service_status_req_tcp_s {
	char serviceID[SERVICE_ID_LEN];
};



///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_start_req_tcp_init( clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req);
void clicktocallrecording_start_req_tcp_final( clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req);
int clicktocallrecording_start_req_decode_dbif_msg( clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void clicktocallrecording_start_req_tcp_display(clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req);
void clicktocallrecording_start_req_dbif_display(uxc_dbif_t *dbif);

int clicktocallrecording_stop_req_tcp_init( clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req);
void clicktocallrecording_stop_req_tcp_final( clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req);
int clicktocallrecording_stop_req_decode_dbif_msg( clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req, uxc_dbif_t *dbif);
void clicktocallrecording_stop_req_tcp_display(clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req);
void clicktocallrecording_stop_req_dbif_display(uxc_dbif_t *dbif);

int clicktocallrecording_service_status_req_tcp_init( clicktocallrecording_service_status_req_tcp_t *clicktocallrecording_service_status_req);
void clicktocallrecording_service_status_req_tcp_final( clicktocallrecording_service_status_req_tcp_t *clicktocallrecording_service_status_req);
void clicktocallrecording_service_status_req_tcp_display(clicktocallrecording_service_status_req_tcp_t *clicktocallrecording_service_status_req);

#endif // #ifndef __CLICKTOCALLRECORDING_REQ_H__
