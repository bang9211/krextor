#ifndef __CLICKTOCALL_RSP_TCP_H__
#define __CLICKTOCALL_RSP_TCP_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktocall_start_rsp_tcp_s clicktocall_start_rsp_tcp_t; 
struct clicktocall_start_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[32];
	int8_t isRecording;
	int8_t filler1;
	int16_t filler2;
	char recordingFileURL[64];
	char recordingFileName[64];
};

typedef struct clicktocall_stop_rsp_tcp_s clicktocall_stop_rsp_tcp_t; 
struct clicktocall_stop_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[32];
};

typedef struct clicktocall_startrecording_rsp_tcp_s clicktocall_startrecording_rsp_tcp_t; 
struct clicktocall_startrecording_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[32];
	char recordingFileURL[64];
	char recordingFileName[64];
};

typedef struct clicktocall_stoprecording_rsp_tcp_s clicktocall_stoprecording_rsp_tcp_t; 
struct clicktocall_stoprecording_rsp_tcp_s {
	int32_t resultCode;
	char serviceID[32];
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

#endif // #ifndef __CLICKTOCALL_RSP_TCP_H__
