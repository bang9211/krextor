#ifndef __SKB_MSG_H__
#define __SKB_MSG_H__

#include "tcp_conf.h"
#include "tcp_clicktocall_req.h"
#include "tcp_clicktocall_rsp.h"
#include <upa/upa_tcp.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>

///////////////////////////////////////////////////////////////////////////////////
// SKB Common Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

/** @brief define DBIF(GW) message ID */
#define CALL_START_REQUEST									100
#define CALL_STOP_REQUEST									101
#define CALL_START_RECORDING_REQUEST						102
#define CALL_STOP_RECORDING_REQUEST							103
#define CALL_SERVICE_STATUS_REPORT							200
#define CALL_END_REPORT										201
#define CALL_RECORDING_START_REQUEST						300
#define CALL_RECORDING_STOP_REQUEST							301
#define RECORDING_CALL_SERVICE_STATUS_REPORT				400
#define RECORDING_CALL_END_REPORT							401
#define CONFERENCE_START_REQUEST							500
#define CONFERENCE_STOP_REQUEST								501
#define ADD_PARTY_REQUEST									502
#define REMOVE_PARTY_REQUEST								503
#define CANCEL_PARTY_REQUEST								504
#define CHANGE_PARTY_MEDIA_REQUEST							505
#define PLAY_MENT_REQUEST									506
#define GET_NUMBER_OF_PARTY_REQUEST							507
#define GET_PARTY_STATUS_REQUEST							508
#define ADD_PARTY_REPORT									600
#define REMOVE_PARTY_REPORT									601
#define CHANGE_PARTY_MEDIA_REPORT							602
#define CLOSE_CONF_REPORT									603


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
	void *body;
};

int skb_msg_cvt_order_hton(skb_msg_t *msg, int msgId);
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int msgId);
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey);

int32_t skb_msg_generate_messasgeID();
skb_header_t* skb_msg_make_header(int32_t messageID, int16_t bodySize, int32_t *requestID);
int32_t skb_msg_generate_requestID();


#endif // #ifndef __SKB_MSG_H__
