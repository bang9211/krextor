#ifndef __CLICKTOCALL_RPT_TCP_H__
#define __CLICKTOCALL_RPT_TCP_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Report Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocall_stop_rpt_tcp_s clicktocall_stop_rpt_tcp_t; 
struct clicktocall_stop_rpt_tcp_s {
	int32_t resultCode;
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
	int32_t resultCode;
	char serviceID[SERVICE_ID_LEN];
};

#endif // #ifndef __CLICKTOCALL_RPT_TCP_H__
