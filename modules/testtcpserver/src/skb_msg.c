#include "skb_msg.h"
 
////////////////////////////////////////////////////////////////////////////////////
// functions for tcp_msg_t
////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief network eIPMS 메시지의 값들을 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton(skb_msg_t *msg, int msgId)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;
    tcp_clicktocall_start_req_t *clicktocall_start_req;

    header = &msg->header;
	
    switch(msgId) {
    case CALL_START_REQUEST:
        clicktocall_start_req = (tcp_clicktocall_start_req_t *) msg->body;
        clicktocall_start_req->waitingMentID = htons(clicktocall_start_req->waitingMentID);
        clicktocall_start_req->callMentID = htons(clicktocall_start_req->callMentID);
        clicktocall_start_req->fillerInt16 = htons(clicktocall_start_req->fillerInt16);
        break;
    case CALL_STOP_REQUEST:
        break;
    case CALL_START_RECORDING_REQUEST:
        break;
    case CALL_STOP_RECORDING_REQUEST:
        break;
	default :
		ux_log( UXL_INFO, "Unknown Msg Id : [%d]\n", msgId);
		break;
    }

	/* skb header convert */
	header->length = htons(header->length);
	header->messageID = htonl(header->messageID);
	header->requestID = htonl(header->requestID);
#endif

	return UX_SUCCESS;
}


/**
 * @brief network eIPMS 메시지의 값들을 host byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과
 */
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int msgId)
{
#if !UX_BIGENDIAN
    skb_header_t *header;
    tcp_clicktocall_start_req_t *clicktocall_start_req;

	if (msg == NULL) return -1;

    header = &msg->header;
	
    switch(msgId) {
    case CALL_START_REQUEST:
        clicktocall_start_req = (tcp_clicktocall_start_req_t *) msg->body;
        clicktocall_start_req->waitingMentID = ntohs(clicktocall_start_req->waitingMentID);
        clicktocall_start_req->callMentID = ntohs(clicktocall_start_req->callMentID);
        clicktocall_start_req->fillerInt16 = ntohs(clicktocall_start_req->fillerInt16);
        break;
    case CALL_STOP_REQUEST:
        break;
    case CALL_START_RECORDING_REQUEST:
        break;
    case CALL_STOP_RECORDING_REQUEST:
        break;
	default :
		ux_log( UXL_INFO, "Unknown Msg Id : [%d]\n", msgId);
		break;
    }

	/* skb header convert */
	header->length = ntohs(header->length);
	header->messageID = ntohl(header->messageID);
	header->requestID = ntohl(header->requestID);
#endif

	return UX_SUCCESS;
}

/**
 * @brief encoding 한 ipcmsg 를 uxcutor 로 전송한다. 
 * @param msg ipcmsg, message
 * @param tcp tcp pa
 * @param peerkey peer key
 * @return 실행 결과 
 */
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "= msg_send_skbmsg =");
	int rv, msg_size;
		
	msg_size = msg->header.length; 

	//TODO
	rv = skb_msg_cvt_order_hton(msg, CALL_START_REQUEST);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	rv = upa_tcp_send2(tcp, peerkey, msg, msg_size, 1);
	if ( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data.");
		return -1;
	} else {
		ux_log( UXL_INFO, "= sent data. length = %d\n ", msg_size);
		return eUXC_SUCCESS;
	}
}

void skb_msg_make_header(skb_header_t* header, int32_t messageID, int16_t bodySize, int32_t *requestID) {
	int32_t temp;

	header.frameStart0 = 0Xfe;
	header.frameStart1 = 0Xfe;
	header.length = sizeof(skb_header_t) + bodySize;
	header.messageID = messageID;
	if (requestID == NULL) {
		temp = skb_msg_generate_requestID();
		requestID = &temp;
	}
	header.requestID = requestID;
	header.version0 = 0x00;
	header.version1 = 0x01;
	header.userID = 1;
	header.filler = 0;
}

int32_t skb_msg_generate_requestID() {
	return 0;
}