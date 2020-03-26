#ifndef __CLICKTOCALLRECORDING_RPT_TCP_H__
#define __CLICKTOCALLRECORDING_RPT_TCP_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Report Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocallrecording_start_rpt_tcp_s clicktocallrecording_start_rpt_tcp_t; 
struct clicktocallrecording_start_rpt_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
	char recordingFileURL[RECORDING_FILE_URL_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
};

// CallEndReport
typedef struct clicktocallrecording_stop_rpt_tcp_s clicktocallrecording_stop_rpt_tcp_t; 
struct clicktocallrecording_stop_rpt_tcp_s {
	int32_t returnCode;
	char serviceID[SERVICE_ID_LEN];
    char startTime[TIME_LEN];    //YYYYMMDDHH24MISS
    char endTime[TIME_LEN];      //YYYYMMDDHH24MISS
	char callingNumber[CALL_NUMBER_LEN];
	char calledNumber[CALL_NUMBER_LEN];
	int32_t isRecorded;
	char recordingFileURL[RECORDING_FILE_URL_LEN];
	char recordingFileName[RECORDING_FILE_NAME_LEN];
};

//서비스 상태 조회 보고
typedef struct clicktocallrecording_service_status_rpt_tcp_s clicktocallrecording_service_status_rpt_tcp_t; 
struct clicktocallrecording_service_status_rpt_tcp_s {
	char serviceID[SERVICE_ID_LEN];
	int32_t status;
};


///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Report Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_start_rpt_tcp_init( clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt);
void clicktocallrecording_start_rpt_tcp_final( clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt);
int clicktocallrecording_start_rpt_encode_to_dbif_msg( clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt, uxc_dbif_t *dbif);
void clicktocallrecording_start_rpt_tcp_display(clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt);
void clicktocallrecording_start_rpt_dbif_display(uxc_dbif_t *dbif);

int clicktocallrecording_stop_rpt_tcp_init( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt);
void clicktocallrecording_stop_rpt_tcp_final( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt);
int clicktocallrecording_stop_rpt_encode_to_dbif_msg( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt, uxc_dbif_t *dbif);
void clicktocallrecording_stop_rpt_tcp_display(clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt);
void clicktocallrecording_stop_rpt_dbif_display(uxc_dbif_t *dbif);

int clicktocallrecording_service_status_rpt_tcp_init( clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt);
void clicktocallrecording_service_status_rpt_tcp_final( clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt);
int clicktocallrecording_service_status_rpt_encode_to_dbif_msg( clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt, uxc_dbif_t *dbif);
void clicktocallrecording_service_status_rpt_tcp_display(clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt);
void clicktocallrecording_service_status_rpt_dbif_display(uxc_dbif_t *dbif);

#endif // #ifndef __CLICKTOCALLRECORDING_RPT_TCP_H__
