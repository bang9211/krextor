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
    clicktocall_start_req_tcp_t clicktocall_start_req[1];

    header = &msg->header;
	
    switch(msgId) {
    case DBIF_CALL_START_REQUEST:
		memcpy(clicktocall_start_req, msg->body, sizeof(clicktocall_start_req_tcp_t));
        clicktocall_start_req->waitingMentID = htons(clicktocall_start_req->waitingMentID);
        clicktocall_start_req->callMentID = htons(clicktocall_start_req->callMentID);
        clicktocall_start_req->fillerInt16 = htons(clicktocall_start_req->fillerInt16);
        break;
    case DBIF_CALL_STOP_REQUEST:
        break;
    case DBIF_CALL_START_RECORDING_REQUEST:
        break;
    case DBIF_CALL_STOP_RECORDING_REQUEST:
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
 * @brief network eIPMS 메시지의 값들을 host byte ordering으로 바꾸고, 실행 결과와 DBIF msgId를 반환한다.
 * @param msg network eIPMS message
 * @return 실행 결과, msgId(response의 경우 NULL)
 */
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int chnIdx, int *msgId)
{
#if !UX_BIGENDIAN
    skb_header_t *header;
    clicktocall_start_req_tcp_t clicktocall_start_req[1];
	clicktocall_stop_req_tcp_t clicktocall_stop_req[1];
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req[1];
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req[1];

    clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];

	if (msg == NULL) return -1;

    header = &msg->header;

	/* skb header convert */
	header->length = ntohs(header->length);
	header->messageID = ntohl(header->messageID);
	header->requestID = ntohl(header->requestID);

	//body 복구
	switch(chnIdx) {
		case TCP_CHANNEL_CALL:
			switch(msg->header.messageID)
			{
				//받은 메시지가 요청인 경우(simulator case)
				case START_REQUEST:
					memcpy(clicktocall_start_req, msg->body, sizeof(clicktocall_start_req_tcp_t));
					clicktocall_start_req->waitingMentID = ntohs(clicktocall_start_req->waitingMentID);
					clicktocall_start_req->callMentID = ntohs(clicktocall_start_req->callMentID);
					clicktocall_start_req->fillerInt16 = ntohs(clicktocall_start_req->fillerInt16);
					break;
				case STOP_REQUEST:
					memcpy(clicktocall_stop_req, msg->body, sizeof(clicktocall_stop_req_tcp_t));
					break;
				case START_RECORDING_REQUEST:
					memcpy(clicktocall_startrecording_req, msg->body, sizeof(clicktocall_startrecording_req_tcp_t));
					break;
				case STOP_RECORDING_REQUEST:
					memcpy(clicktocall_stoprecording_req, msg->body, sizeof(clicktocall_stoprecording_req_tcp_t));
					break;
				//받은 메시지가 응답, 보고의 경우(normal case)
				case START_RESPONSE:
					memcpy(clicktocall_start_rsp, msg->body, sizeof(clicktocall_start_rsp_tcp_t));
					clicktocall_start_rsp->resultCode = ntohl(clicktocall_start_rsp->resultCode);
					clicktocall_start_rsp->filler2 = ntohs(clicktocall_start_rsp->filler2);
					break;
				case STOP_RESPONSE:
					memcpy(clicktocall_stop_rsp, msg->body, sizeof(clicktocall_stop_rsp_tcp_t));
					clicktocall_stop_rsp->resultCode = ntohl(clicktocall_stop_rsp->resultCode);
					break;
				case STOP_REPORT:
					break;
				case START_RECORDING_RESPONSE:
					memcpy(clicktocall_startrecording_rsp, msg->body, sizeof(clicktocall_startrecording_rsp_tcp_t));
					clicktocall_startrecording_rsp->resultCode = ntohl(clicktocall_startrecording_rsp->resultCode);
					break;
				case START_RECORDING_REPORT:
					break;
				case STOP_RECORDING_RESPONSE:
					memcpy(clicktocall_stoprecording_rsp, msg->body, sizeof(clicktocall_stoprecording_rsp_tcp_t));
					clicktocall_stoprecording_rsp->resultCode = ntohl(clicktocall_stoprecording_rsp->resultCode);
					break;
				case STOP_RECORDING_REPORT:
					break;
				case SERVICE_STATUS_RESPONSE:
					break;
				case SERVICE_STATUS_REPORT:
					break;
				default:
					ux_log(UXL_CRT, "Unsupported messageID : %#010x", msg->header.messageID)
					return -1;
			}
			break;		
		case TCP_CHANNEL_RECORDING:
			break;
		case TCP_CHANNEL_CONFERENCE:
			break;
		default:
			ux_log(UXL_CRT, "Unsupported Channel Index : %d", chnIdx)
			return -1;
	}
#endif
//DBIF로 보내져야 하는 메시지인 경우, DBIF msgId 설정
switch(chnIdx) {
		case TCP_CHANNEL_CALL:
			switch(msg->header.messageID)
			{
				//받은 메시지가 응답, 보고의 경우(normal case)
				case START_RESPONSE:
					*msgId = DBIF_CALL_START_RESPONSE;
					break;
				case STOP_RESPONSE:
					*msgId = DBIF_CALL_STOP_RESPONSE;
					break;
				case START_RECORDING_RESPONSE:
					*msgId = DBIF_CALL_START_RECORDING_RESPONSE;
					break;
				case STOP_RECORDING_RESPONSE:
					*msgId = DBIF_CALL_STOP_RECORDING_RESPONSE;
					break;
				case STOP_REPORT:				//CallEndReport
					*msgId = DBIF_CALL_END_REPORT;
					break;
				case SERVICE_STATUS_REPORT:		//CallServiceStatusReport
					*msgId = DBIF_CALL_SERVICE_STATUS_REPORT;
					break;
				default:
					msgId = NULL;
					break;
			}
			break;		
		case TCP_CHANNEL_RECORDING:
			break;
		case TCP_CHANNEL_CONFERENCE:
			break;
		default:
			ux_log(UXL_CRT, "Unsupported Channel Index : %d", chnIdx)
			return -1;
	}

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
	rv = skb_msg_cvt_order_hton(msg, DBIF_CALL_START_REQUEST);
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

	header->frameStart0 = 0Xfe;
	header->frameStart1 = 0Xfe;
	header->length = sizeof(skb_header_t) + bodySize;
	header->messageID = messageID;
	if (requestID == NULL) {
		temp = skb_msg_generate_requestID();
		requestID = &temp;
	}
	header->requestID = *requestID;
	header->version0 = 0x00;
	header->version1 = 0x01;
	header->userID = 1;
	header->filler = 0;
}

void skb_msg_display_header(skb_header_t* header) {
	ux_log(UXL_INFO, "TCP Header");
	ux_log(UXL_INFO, "  [frameStart1] 0x%hhX", header->frameStart1);
	ux_log(UXL_INFO, "  [length] %d", header->length);
	ux_log(UXL_INFO, "  [messageID] %#010x", header->messageID);
	ux_log(UXL_INFO, "  [requestID] %d", header->requestID);
	ux_log(UXL_INFO, "  [version0] 0x%hhX", header->version0);
	ux_log(UXL_INFO, "  [version1] 0x%hhX", header->version1);
	ux_log(UXL_INFO, "  [userID] %d", header->userID);
	ux_log(UXL_INFO, "  [filler] %d", header->filler);
}

//TODO : requestID 수명이 다하면 초기화해줘야함(고갈 가능성)
int32_t skb_msg_generate_requestID() {
	return getRandomInt32();
}

int32_t getRandomInt32() {
	int32_t x;
	x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	x |= (rand() & 0xff) << 16;
	x |= (rand() & 0xff) << 24;
	return x;
}

void create_skb_map() {
	reqID_SID_Map = uh_int_init();
	reqID_GWSID_Map = uh_int_init();
	reqID_IPC_Map = uh_ipc_init();
}

void destroy_skb_map() {
//    uxc_ixpc_t* tval = 0;
//    khiter_t k;
//    for (k = kh_begin(reqID_IPC_Map->h); k != kh_end(reqID_IPC_Map->h); ++k) {
//       if (kh_exist(reqID_IPC_Map->h, k)) {
//          const char *key = kh_key(reqID_IPC_Map->h, k);
//          tval = kh_value(reqID_IPC_Map->h, k);
// 		 free(tval);
//       }
//    }

	uh_int_destroy(reqID_SID_Map);
	uh_int_destroy(reqID_GWSID_Map);
	uh_ipc_destroy(reqID_IPC_Map);
}


int skb_msg_process_clicktocall_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID[], char *gwSessionID[]) {
	int rv;
	clicktocall_start_req_tcp_t clicktocall_start_req;

	// DBIF를 body로 변환
	clicktocall_start_req_dbif_display(dbif);
	rv = clicktocall_start_req_decode_dbif_msg(&clicktocall_start_req, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_REQUEST, sizeof(clicktocall_start_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_start_req, sizeof(clicktocall_start_req));
	clicktocall_start_req_tcp_display(&clicktocall_start_req);

	return rv;
}

int skb_msg_process_clicktocall_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stop_req_tcp_t clicktocall_stop_req;

	// DBIF를 body로 변환
	clicktocall_stop_req_dbif_display(dbif);
	rv = clicktocall_stop_req_decode_dbif_msg(&clicktocall_stop_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktocall_stop_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_stop_req, sizeof(clicktocall_stop_req));
	clicktocall_stop_req_tcp_display(&clicktocall_stop_req);

	return rv;
}

int skb_msg_process_clicktocall_startrecording_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req;

	// DBIF를 body로 변환
	clicktocall_startrecording_req_dbif_display(dbif);
	rv = clicktocall_startrecording_req_decode_dbif_msg(&clicktocall_startrecording_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_RECORDING_REQUEST, sizeof(clicktocall_startrecording_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_startrecording_req, sizeof(clicktocall_startrecording_req));
	clicktocall_startrecording_req_tcp_display(&clicktocall_startrecording_req);
	
	return rv;
}

int skb_msg_process_clicktocall_stoprecording_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req;

	// DBIF를 body로 변환
	clicktocall_stoprecording_req_dbif_display(dbif);
	rv = clicktocall_stoprecording_req_decode_dbif_msg(&clicktocall_stoprecording_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_RECORDING_REQUEST, sizeof(clicktocall_stoprecording_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_stoprecording_req, sizeof(clicktocall_stoprecording_req));
	clicktocall_stoprecording_req_tcp_display(&clicktocall_stoprecording_req);

	return rv;
}
