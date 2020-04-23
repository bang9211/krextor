#ifndef __SKB_MSG_H__
#define __SKB_MSG_H__

#include "eipmsib_conf.h"
#include "uhash.h"
#include "skb_msg_info.h"
#include "clicktocall_req.h"
#include "clicktocall_rsp.h"
#include "clicktocall_rpt.h"
#include <time.h>
#include <stdlib.h>
#include <upa/upa_tcp.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////////
// SKB Common Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

/** @brief SKB Request ID Pool */
uhash_rid_t *reqID_Set;				//requestID
uhash_tmt_t *reqID_timestamp_Map;	//requestID - timestamp
uhash_int_t *reqID_SID_Map;			//requestID - sessionID
uhash_int_t *reqID_GWSID_Map;		//requestID - gwSessionID
uhash_ipc_t *reqID_IPC_Map;			//requestID - uxc_ixpc_t

typedef struct skb_header_s skb_header_t; 
struct skb_header_s {
	int32_t msgType;		//
	int32_t length;			//Body 길이
};

typedef struct skb_msg_s skb_msg_t;
struct skb_msg_s {
	skb_header_t header;
	char body[SKB_MSG_MAX_BODY_LEN];
};

int _heartbeat_display;

void skb_msg_init(int heartbeat_display);
void skb_msg_end();
void create_skb_map();
void destroy_skb_map();

int skb_msg_cvt_to_skb_msg_id(int dbif_msgID);
int skb_msg_cvt_to_dbif_msg_id(int chnl_idx, int msgID);

int skb_msg_cvt_order_hton(skb_msg_t *msg, int dbif_msgID);
int skb_msg_cvt_order_hton2(skb_msg_t *msg);
int skb_msg_cvt_order_hton3(skb_msg_t *msg, int chnl_idx);
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int chnIdx, int *msgId);
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey, int dbif_msgID);

int32_t getRandomInt32();
void skb_msg_make_header(skb_header_t* header, int32_t messageID, int16_t bodySize, int32_t *requestID);
int skb_msg_make_bind_request(skb_msg_t *skbmsg, int chnl_idx, char *id, char *pw);
void skb_msg_get_header_display(skb_header_t* header, char *log);
void skb_msg_get_send_header_display(skb_header_t* header, char *log);
void skb_msg_get_recv_header_display(skb_header_t* header, char *log);
void skb_msg_display_send_header(skb_header_t* header);
void skb_msg_display_recv_header(skb_header_t* header);
void skb_msg_display_send_heartbeat_req(skb_header_t* header, int chnl_idx);
void skb_msg_display_recv_heartbeat_req(skb_header_t* header, int chnl_idx);
void skb_msg_display_send_heartbeat_rsp(skb_header_t* header, int chnl_idx);
void skb_msg_display_recv_heartbeat_rsp(skb_header_t* header, int chnl_idx);
int32_t skb_msg_generate_messasgeID();
int32_t skb_msg_generate_requestID();

//clicktocall
void skb_msg_process_clicktocall_heartbeat_req(skb_msg_t *skbmsg);				//heartbeat
void skb_msg_process_clicktocall_binding_req(skb_msg_t *skbmsg, char *userID, char *password);					//binding
int skb_msg_process_clicktocall_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID);

void skb_msg_process_clicktocall_heartbeat_rsp(skb_msg_t *skbmsg);				//heartbeat
int skb_msg_process_clicktocall_binding_rsp(skb_msg_t *skbmsg);					//binding
int skb_msg_process_clicktocall_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

void *timeout_function();
void turn_timeout_timer_on();
void turn_timeout_timer_off();

#endif // #ifndef __SKB_MSG_H__
