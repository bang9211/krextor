#ifndef __TCP_CLICKTOCALL_RSP_H__
#define __TCP_CLICKTOCALL_RSP_H__

#include "tcp_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_clicktocall_start_rsp_s tcp_clicktocall_start_rsp_t; 
struct tcp_clicktocall_start_rsp_s {
	int32_t resultCode;
	char serviceID[32];
	int8_t isRecording;
	int8_t filler1;
	int16_t filler2;
	char recordingFileURL[64];
	char recordingFileName[64];
};

typedef struct tcp_clicktocall_stop_rsp_s tcp_clicktocall_stop_rsp_t; 
struct tcp_clicktocall_stop_rsp_s {
	int32_t resultCode;
	char serviceID[32];
};

typedef struct tcp_clicktocall_startrecording_rsp_s tcp_clicktocall_startrecording_rsp_t; 
struct tcp_clicktocall_startrecording_rsp_s {
	int32_t resultCode;
	char serviceID[32];
	char recordingFileURL[64];
	char recordingFileName[64];
};


typedef struct tcp_clicktocall_stoprecording_rsp_s tcp_clicktocall_stoprecording_rsp_t; 
struct tcp_clicktocall_stoprecording_rsp_s {
	int32_t resultCode;
	char serviceID[32];
};


///////////////////////////////////////////////////////////////////////////////////
// tcp_clicktocall Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_clicktocall_start_rsp_encode_ipcmsg( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp, tcp_msg_t *msg);
int tcp_clicktocall_start_rsp_init( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp);
void tcp_clicktocall_start_rsp_final( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp);

// int tcp_clicktocall_stop_rsp_encode_ipcmsg( tcp_clicktocall_stop_rsp_t *clicktocall_stop_rsp, tcp_msg_t *msg);
// int tcp_clicktocall_stop_rsp_init( tcp_clicktocall_stop_rsp_t *clicktocall_stop_rsp);
// void tcp_clicktocall_stop_rsp_final( tcp_clicktocall_stop_rsp_t *clicktocall_stop_rsp);

// int tcp_clicktocall_startrecording_rsp_encode_ipcmsg( tcp_clicktocall_startrecording_rsp_t *clicktocall_startrecording_rsp, tcp_msg_t *msg);
// int tcp_clicktocall_startrecording_rsp_init( tcp_clicktocall_startrecording_rsp_t *clicktocall_startrecording_rsp);
// void tcp_clicktocall_startrecording_rsp_final( tcp_clicktocall_startrecording_rsp_t *clicktocall_startrecording_rsp);

// int tcp_clicktocall_stoprecording_rsp_encode_ipcmsg( tcp_clicktocall_stoprecording_rsp_t *clicktocall_stoprecording_rsp, tcp_msg_t *msg);
// int tcp_clicktocall_stoprecording_rsp_init( tcp_clicktocall_stoprecording_rsp_t *clicktocall_stoprecording_rsp);
// void tcp_clicktocall_stoprecording_rsp_final( tcp_clicktocall_stoprecording_rsp_t *clicktocall_stoprecording_rsp);

#endif // #ifndef __TCP_ CLICKTOCALL_RSP_H__
