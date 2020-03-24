#ifndef __SKB_MSG_H__
#define __SKB_MSG_H__

#include "tcp_conf.h"
#include "uhash.h"
#include "clicktocall_req.h"
#include "clicktocall_rsp.h"
#include <time.h>
#include <stdlib.h>
#include <upa/upa_tcp.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>

///////////////////////////////////////////////////////////////////////////////////
// SKB Common Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

/** @brief define TCP channel info */
#define TCP_CHANNEL_CALL 										0
#define TCP_CHANNEL_RECORDING 									1
#define TCP_CHANNEL_CONFERENCE 									2

/** @brief define DBIF(GW) message ID */
#define DBIF_CALL_START_REQUEST									100
#define DBIF_CALL_STOP_REQUEST									101
#define DBIF_CALL_START_RECORDING_REQUEST						102
#define DBIF_CALL_STOP_RECORDING_REQUEST						103

#define DBIF_CALL_START_RESPONSE								200
#define DBIF_CALL_STOP_RESPONSE									201
#define DBIF_CALL_START_RECORDING_RESPONSE						202
#define DBIF_CALL_STOP_RECORDING_RESPONSE						203
#define DBIF_CALL_SERVICE_STATUS_REPORT							204
#define DBIF_CALL_END_REPORT									205

#define DBIF_CALL_RECORDING_START_REQUEST						300
#define DBIF_CALL_RECORDING_STOP_REQUEST						301

#define DBIF_CALL_RECORDING_START_RESPONSE						400
#define DBIF_CALL_RECORDING_STOP_RESPONSE						401
#define DBIF_RECORDING_CALL_SERVICE_STATUS_REPORT				402
#define DBIF_RECORDING_CALL_END_REPORT							403

#define DBIF_CONFERENCE_START_REQUEST							500
#define DBIF_CONFERENCE_STOP_REQUEST							501
#define DBIF_ADD_PARTY_REQUEST									502
#define DBIF_REMOVE_PARTY_REQUEST								503
#define DBIF_CANCEL_PARTY_REQUEST								504
#define DBIF_CHANGE_PARTY_MEDIA_REQUEST							505
#define DBIF_PLAY_MENT_REQUEST									506
#define DBIF_GET_NUMBER_OF_PARTY_REQUEST						507
#define DBIF_GET_PARTY_STATUS_REQUEST							508

#define DBIF_CONFERENCE_START_RESEPONSE							600
#define DBIF_CONFERENCE_STOP_RESEPONSE							601
#define DBIF_ADD_PARTY_RESEPONSE								602
#define DBIF_REMOVE_PARTY_RESEPONSE								603
#define DBIF_CANCEL_PARTY_RESEPONSE								604
#define DBIF_CHANGE_PARTY_MEDIA_RESEPONSE						605
#define DBIF_PLAY_MENT_RESEPONSE								606
#define DBIF_GET_NUMBER_OF_PARTY_RESEPONSE						607
#define DBIF_GET_PARTY_STATUS_RESEPONSE							608
#define DBIF_ADD_PARTY_REPORT									609
#define DBIF_REMOVE_PARTY_REPORT								610
#define DBIF_CHANGE_PARTY_MEDIA_REPORT							611
#define DBIF_CLOSE_CONF_REPORT									612


/** @brief define TCP(eIPMS) message ID */
#define HEARTBEAT_REQUEST									0x00000000
#define HEARTBEAT_RESEPONSE									0x10000000
#define BINDING_REQUEST										0x00000001
#define BINDING_RESPONSE									0x10000001
#define START_REQUEST										0x00000010
#define START_RESPONSE										0x10000010
#define STOP_REQUEST										0x00000011
#define STOP_RESPONSE										0x10000011
#define STOP_REPORT											0x20000011
#define START_RECORDING_REQUEST								0x00000020
#define START_RECORDING_RESPONSE							0x10000020
#define START_RECORDING_REPORT								0x20000020
#define STOP_RECORDING_REQUEST								0x00000021
#define STOP_RECORDING_RESPONSE								0x10000021
#define STOP_RECORDING_REPORT								0x20000021
#define SERVICE_STATUS_REQUEST								0x00000030
#define SERVICE_STATUS_RESPONSE								0x10000030
#define SERVICE_STATUS_REPORT								0x20000030

/** @brief SKB MAX Message Length */
#define SKB_MSG_MAX_LEN		64*1024

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

int skb_msg_cvt_order_hton(skb_msg_t *msg, int msgId);
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int chnIdx, int *msgId);
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey);

int32_t getRandomInt32();
void skb_msg_make_header(skb_header_t* header, int32_t messageID, int16_t bodySize, int32_t *requestID);
void skb_msg_display_header(skb_header_t* header);
int32_t skb_msg_generate_messasgeID();
int32_t skb_msg_generate_requestID();


#endif // #ifndef __SKB_MSG_H__
