#ifndef __CLICKTOCALLRECORDING_RSP_TCP_H__
#define __CLICKTOCALLRECORDING_RSP_TCP_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocallrecording_start_rsp_tcp_s clicktocallrecording_start_rsp_tcp_t; 
struct clicktocallrecording_start_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
	char recordingFileURL[RECORDING_FILE_URL_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
};

typedef struct clicktocallrecording_stop_rsp_tcp_s clicktocallrecording_stop_rsp_tcp_t; 
struct clicktocallrecording_stop_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocallrecording_service_status_rsp_tcp_s clicktocallrecording_service_status_rsp_tcp_t; 
struct clicktocallrecording_service_status_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
	int32_t status;
};


///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_start_rsp_tcp_init( clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp);
void clicktocallrecording_start_rsp_tcp_final( clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp);
int clicktocallrecording_start_rsp_encode_to_dbif_msg( clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void clicktocallrecording_start_rsp_tcp_display(clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp);
void clicktocallrecording_start_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktocallrecording_stop_rsp_tcp_init( clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp);
void clicktocallrecording_stop_rsp_tcp_final( clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp);
int clicktocallrecording_stop_rsp_encode_to_dbif_msg( clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp, uxc_dbif_t *dbif);
void clicktocallrecording_stop_rsp_tcp_display(clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp);
void clicktocallrecording_stop_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktocallrecording_service_status_rsp_tcp_init( clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp);
void clicktocallrecording_service_status_rsp_tcp_final( clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp);
void clicktocallrecording_service_status_rsp_tcp_display(clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp);

#endif // #ifndef __CLICKTOCALLRECORDING_RSP_TCP_H__
