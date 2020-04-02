#ifndef __SKB_MSG_H__
#define __SKB_MSG_H__

#include "tcp_conf.h"
#include "uhash.h"
#include "skb_msg_info.h"
#include "clicktocall_req.h"
#include "clicktocall_rsp.h"
#include "clicktocall_rpt.h"
#include "clicktocallrecording_req.h"
#include "clicktocallrecording_rsp.h"
#include "clicktocallrecording_rpt.h"
#include "clicktoconference_req.h"
#include "clicktoconference_rsp.h"
#include "clicktoconference_rpt.h"
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

// #define UNSIGNED_INT32_MAX 4294967295	//2147483647 - (-2147483648), min: 0 max: 4294967295
// typedef enum {false, true} bool;

/** @brief SKB Request ID Pool */
uhash_int_t *reqID_SID_Map;
uhash_int_t *reqID_GWSID_Map;
uhash_ipc_t *reqID_IPC_Map;

typedef struct skb_header_s skb_header_t; 
struct skb_header_s {
	int8_t frameStart0;		//프레임 시작 (고정) : 0xfe
	int8_t frameStart1;		//프레임 시작 (고정) : 0xfe
	int16_t length;			//Header 포함 총 길이
	int32_t messageID;		//Message ID
	int32_t requestID;		//Request ID
	int8_t version0;		//현재 개방형 인터페이스 Version은 "0.1"로 정의한다. : 0
	int8_t version1;		//현재 개방형 인터페이스 Version은 "0.1"로 정의한다. : 1
	int8_t userID;			//IPMS와 개방형 GW 사이의 연동에서사용자 ID는 1값을 갖는다. : 1
	int8_t filler;
};

typedef struct skb_msg_s skb_msg_t;
struct skb_msg_s {
	skb_header_t header;
	char body[SKB_MSG_MAX_LEN - sizeof(skb_header_t)];
};

void create_skb_map();
void destroy_skb_map();

int skb_msg_cvt_order_hton(skb_msg_t *msg, int dbif_msgId);
int skb_msg_cvt_order_hton2(skb_msg_t *msg);
int skb_msg_cvt_order_hton3(skb_msg_t *msg, int chnl_idx);
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int chnIdx, int *msgId);
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey, int dbif_msgId);

int32_t getRandomInt32();
void skb_msg_make_header(skb_header_t* header, int32_t messageID, int16_t bodySize, int32_t *requestID);
int skb_msg_make_bind_request(skb_msg_t *skbmsg, int chnl_idx);
void skb_msg_get_header_display(skb_header_t* header, char *log);
void skb_msg_get_send_header_display(skb_header_t* header, char *log);
void skb_msg_get_recv_header_display(skb_header_t* header, char *log);
void skb_msg_display_send_header(skb_header_t* header);
void skb_msg_display_recv_header(skb_header_t* header);
int32_t skb_msg_generate_messasgeID();
int32_t skb_msg_generate_requestID();

//clicktocall
void skb_msg_process_clicktocall_heartbeat_req(skb_msg_t *skbmsg);				//heartbeat
void skb_msg_process_clicktocall_binding_req(skb_msg_t *skbmsg, char *userID, char *password);					//binding
int skb_msg_process_clicktocall_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID);
int skb_msg_process_clicktocall_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_startrecording_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_stoprecording_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

void skb_msg_process_clicktocall_heartbeat_rsp(skb_msg_t *skbmsg);				//heartbeat
int skb_msg_process_clicktocall_binding_rsp(skb_msg_t *skbmsg);					//binding
int skb_msg_process_clicktocall_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_startrecording_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_stoprecording_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_service_status_rsp( skb_msg_t *skbmsg);

int skb_msg_process_clicktocall_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocall_startrecording_rpt( skb_msg_t *skbmsg);
int skb_msg_process_clicktocall_stoprecording_rpt( skb_msg_t *skbmsg);
int skb_msg_process_clicktocall_service_status_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

//clicktocallrecording
void skb_msg_process_clicktocallrecording_heartbeat_req(skb_msg_t *skbmsg);		//heartbeat
void skb_msg_process_clicktocallrecording_binding_req(skb_msg_t *skbmsg, char *userID, char *password);		//binding
int skb_msg_process_clicktocallrecording_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID);
int skb_msg_process_clicktocallrecording_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocallrecording_service_status_req( skb_msg_t *skbmsg);

void skb_msg_process_clicktocallrecording_heartbeat_rsp(skb_msg_t *skbmsg);		//heartbeat
int skb_msg_process_clicktocallrecording_binding_rsp(skb_msg_t *skbmsg);		//binding
int skb_msg_process_clicktocallrecording_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocallrecording_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocallrecording_service_status_rsp( skb_msg_t *skbmsg);

int skb_msg_process_clicktocallrecording_start_rpt( skb_msg_t *skbmsg);
int skb_msg_process_clicktocallrecording_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktocallrecording_service_status_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

//clicktoconference
void skb_msg_process_clicktoconference_heartbeat_req(skb_msg_t *skbmsg);	//heartbeat
void skb_msg_process_clicktoconference_binding_req(skb_msg_t *skbmsg, char *userID, char *password);			//binding
int skb_msg_process_clicktoconference_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID);
int skb_msg_process_clicktoconference_add_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_remove_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_change_party_media_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_change_option_req( skb_msg_t *skbmsg);
int skb_msg_process_clicktoconference_get_number_of_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_play_ment_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_get_party_status_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_cancel_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

void skb_msg_process_clicktoconference_heartbeat_rsp(skb_msg_t *skbmsg);	//heartbeat
int skb_msg_process_clicktoconference_binding_rsp(skb_msg_t *skbmsg);			//binding
int skb_msg_process_clicktoconference_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_add_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_remove_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_change_party_media_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_change_option_rsp( skb_msg_t *skbmsg);
int skb_msg_process_clicktoconference_get_number_of_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_play_ment_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_get_party_status_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_cancel_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

int skb_msg_process_clicktoconference_add_party_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_remove_party_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_change_party_media_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);
int skb_msg_process_clicktoconference_change_option_rpt( skb_msg_t *skbmsg);
int skb_msg_process_clicktoconference_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif);

#endif // #ifndef __SKB_MSG_H__
