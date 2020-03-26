#ifndef __CLICKTOCALL_RSP_TCP_H__
#define __CLICKTOCALL_RSP_TCP_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocall_start_rsp_tcp_s clicktocall_start_rsp_tcp_t; 
struct clicktocall_start_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
	int8_t isRecording;
	int8_t filler1;
	int16_t filler2;
	char recordingFileURL[RECORDING_FILE_URL_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
};

typedef struct clicktocall_stop_rsp_tcp_s clicktocall_stop_rsp_tcp_t; 
struct clicktocall_stop_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocall_startrecording_rsp_tcp_s clicktocall_startrecording_rsp_tcp_t; 
struct clicktocall_startrecording_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
	char recordingFileURL[RECORDING_FILE_URL_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
};

typedef struct clicktocall_stoprecording_rsp_tcp_s clicktocall_stoprecording_rsp_tcp_t; 
struct clicktocall_stoprecording_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocall_service_status_rsp_tcp_s clicktocall_service_status_rsp_tcp_t; 
struct clicktocall_service_status_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
	int32_t status;
};


///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_start_rsp_tcp_init( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp);
void clicktocall_start_rsp_tcp_final( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp);
int clicktocall_start_rsp_encode_to_dbif_msg( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void clicktocall_start_rsp_tcp_display(clicktocall_start_rsp_tcp_t *clicktocall_start_rsp);
void clicktocall_start_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktocall_stop_rsp_tcp_init( clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp);
void clicktocall_stop_rsp_tcp_final( clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp);
int clicktocall_stop_rsp_encode_to_dbif_msg( clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp, uxc_dbif_t *dbif);
void clicktocall_stop_rsp_tcp_display(clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp);
void clicktocall_stop_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktocall_startrecording_rsp_tcp_init( clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp);
void clicktocall_startrecording_rsp_tcp_final( clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp);
int clicktocall_startrecording_rsp_encode_to_dbif_msg( clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp, uxc_dbif_t *dbif);
void clicktocall_startrecording_rsp_tcp_display(clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp);
void clicktocall_startrecording_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktocall_stoprecording_rsp_tcp_init( clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp);
void clicktocall_stoprecording_rsp_tcp_final( clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp);
int clicktocall_stoprecording_rsp_encode_to_dbif_msg( clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp, uxc_dbif_t *dbif);
void clicktocall_stoprecording_rsp_tcp_display(clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp);
void clicktocall_stoprecording_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktocall_service_status_rsp_tcp_init( clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp);
void clicktocall_service_status_rsp_tcp_final( clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp);
void clicktocall_service_status_rsp_tcp_display(clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp);

#endif // #ifndef __CLICKTOCALL_RSP_TCP_H__
