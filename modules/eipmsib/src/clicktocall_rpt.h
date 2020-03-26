#ifndef __CLICKTOCALL_RPT_TCP_H__
#define __CLICKTOCALL_RPT_TCP_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Report Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

// CallEndReport
typedef struct clicktocall_stop_rpt_tcp_s clicktocall_stop_rpt_tcp_t; 
struct clicktocall_stop_rpt_tcp_s {
	int32_t returnCode;
	char serviceID[SERVICE_ID_LEN];
    char serviceStartTime[SERVICE_TIME_LEN];    //YYYYMMDDHH24MISS
    char serviceEndTime[SERVICE_TIME_LEN];      //YYYYMMDDHH24MISS
	int32_t isRecorded;
};

typedef struct clicktocall_startrecording_rpt_tcp_s clicktocall_startrecording_rpt_tcp_t; 
struct clicktocall_startrecording_rpt_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
};

typedef struct clicktocall_stoprecording_rpt_tcp_s clicktocall_stoprecording_rpt_tcp_t; 
struct clicktocall_stoprecording_rpt_tcp_s {
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
};

//서비스 상태 조회 보고
typedef struct clicktocall_service_status_rpt_tcp_s clicktocall_service_status_rpt_tcp_t; 
struct clicktocall_service_status_rpt_tcp_s {
	char serviceID[SERVICE_ID_LEN];
	int32_t status;
};


///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Report Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stop_rpt_tcp_init( clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt);
void clicktocall_stop_rpt_tcp_final( clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt);
int clicktocall_stop_rpt_encode_to_dbif_msg( clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt, uxc_dbif_t *dbif);
void clicktocall_stop_rpt_tcp_display(clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt);
void clicktocall_stop_rpt_dbif_display(uxc_dbif_t *dbif);

int clicktocall_startrecording_rpt_tcp_init( clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt);
void clicktocall_startrecording_rpt_tcp_final( clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt);
void clicktocall_startrecording_rpt_tcp_display(clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt);

int clicktocall_stoprecording_rpt_tcp_init( clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt);
void clicktocall_stoprecording_rpt_tcp_final( clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt);
void clicktocall_stoprecording_rpt_tcp_display(clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt);

int clicktocall_service_status_rpt_tcp_init( clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt);
void clicktocall_service_status_rpt_tcp_final( clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt);
int clicktocall_service_status_rpt_encode_to_dbif_msg( clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt, uxc_dbif_t *dbif);
void clicktocall_service_status_rpt_tcp_display(clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt);
void clicktocall_service_status_rpt_dbif_display(uxc_dbif_t *dbif);

#endif // #ifndef __CLICKTOCALL_RPT_TCP_H__
