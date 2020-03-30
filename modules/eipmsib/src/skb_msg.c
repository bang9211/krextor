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
        clicktocall_start_req->filler2 = htons(clicktocall_start_req->filler2);
        break;
    case DBIF_CALL_STOP_REQUEST:				//처리 필요 없음
        break;
    case DBIF_CALL_START_RECORDING_REQUEST:		//처리 필요 없음
        break;
    case DBIF_CALL_STOP_RECORDING_REQUEST:		//처리 필요 없음
        break;
	case DBIF_CALL_RECORDING_START_REQUEST:
		break;
	case DBIF_CALL_RECORDING_STOP_REQUEST:
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
	clicktocall_service_status_rsp_tcp_t clicktocall_service_status_rsp[1];

	clicktocall_stop_rpt_tcp_t clicktocall_stop_rpt[1];
	clicktocall_startrecording_rpt_tcp_t clicktocall_startrecording_rpt[1];
	clicktocall_stoprecording_rpt_tcp_t clicktocall_stoprecording_rpt[1];
    clicktocall_service_status_rpt_tcp_t clicktocall_service_status_rpt[1];

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
					clicktocall_start_req->filler2 = ntohs(clicktocall_start_req->filler2);
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
				case START_RECORDING_RESPONSE:
					memcpy(clicktocall_startrecording_rsp, msg->body, sizeof(clicktocall_startrecording_rsp_tcp_t));
					clicktocall_startrecording_rsp->resultCode = ntohl(clicktocall_startrecording_rsp->resultCode);
					break;
				case STOP_RECORDING_RESPONSE:
					memcpy(clicktocall_stoprecording_rsp, msg->body, sizeof(clicktocall_stoprecording_rsp_tcp_t));
					clicktocall_stoprecording_rsp->resultCode = ntohl(clicktocall_stoprecording_rsp->resultCode);
					break;
				case SERVICE_STATUS_RESPONSE:
					memcpy(clicktocall_service_status_rsp, msg->body, sizeof(clicktocall_service_status_rsp_tcp_t));
					clicktocall_service_status_rsp->resultCode = ntohl(clicktocall_service_status_rsp->resultCode);
					clicktocall_service_status_rsp->status = ntohl(clicktocall_service_status_rsp->status);
					break;
				case STOP_REPORT:
					memcpy(clicktocall_stop_rpt, msg->body, sizeof(clicktocall_stop_rpt_tcp_t));
					clicktocall_stop_rpt->returnCode = ntohl(clicktocall_stop_rpt->returnCode);
					clicktocall_stop_rpt->isRecorded = ntohl(clicktocall_stop_rpt->isRecorded);
					break;
				case START_RECORDING_REPORT:
					memcpy(clicktocall_startrecording_rpt, msg->body, sizeof(clicktocall_startrecording_rpt_tcp_t));
					clicktocall_startrecording_rpt->resultCode = ntohl(clicktocall_startrecording_rpt->resultCode);
					break;
				case STOP_RECORDING_REPORT:
					memcpy(clicktocall_stoprecording_rpt, msg->body, sizeof(clicktocall_stoprecording_rpt_tcp_t));
					clicktocall_stoprecording_rpt->resultCode = ntohl(clicktocall_stoprecording_rpt->resultCode);
					break;
				case SERVICE_STATUS_REPORT:
					memcpy(clicktocall_service_status_rpt, msg->body, sizeof(clicktocall_service_status_rpt_tcp_t));
					clicktocall_service_status_rpt->status = ntohl(clicktocall_service_status_rpt->status);
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

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_clicktocall_heartbeat_req(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, HEARTBEAT_RESPONSE, 0, &skbmsg->header.requestID);
	skb_msg_display_header(&skbmsg->header);
}

void skb_msg_process_clicktocall_binding_req(skb_msg_t *skbmsg, char *userID, char *password) {
	clicktocall_binding_req_tcp_t clicktocall_binding_req;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BINDING_REQUEST, 0, NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	strncpy(clicktocall_binding_req.userID, userID, BINDING_USER_ID_LEN);
	strncpy(clicktocall_binding_req.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &clicktocall_binding_req, sizeof(clicktocall_binding_req));
	clicktocall_binding_req_tcp_display(&clicktocall_binding_req);
}

int skb_msg_process_clicktocall_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID) {
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

void skb_msg_process_clicktocall_heartbeat_rsp(skb_msg_t *skbmsg) {
	skb_msg_display_header(&skbmsg->header);
}

int skb_msg_process_clicktocall_binding_rsp(skb_msg_t *skbmsg) {
	clicktocall_binding_rsp_tcp_t clicktocall_binding_rsp[1];

	memcpy(clicktocall_binding_rsp, skbmsg->body, sizeof(clicktocall_binding_rsp_tcp_t));
	clicktocall_binding_rsp_tcp_display(clicktocall_binding_rsp);
	
	if (clicktocall_binding_rsp->resultCode < 2) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind : %d", clicktocall_binding_rsp->resultCode);
	return -1;
}

int skb_msg_process_clicktocall_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	memcpy(clicktocall_start_rsp, skbmsg->body, sizeof(clicktocall_start_rsp_tcp_t));
	clicktocall_start_rsp_tcp_display(clicktocall_start_rsp);
	//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(reqID_SID_Map, skbmsg->header.requestID), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "There is no sessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		// uh_int_del(reqID_SID_Map, skbmsg->header.requestID);
	}
	strncpy(gwSessionID, uh_int_get(reqID_GWSID_Map, skbmsg->header.requestID), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "There is no gwSessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		// uh_int_del(reqID_GWSID_Map, skbmsg->header.requestID);
	}
	rv = clicktocall_start_rsp_encode_to_dbif_msg(clicktocall_start_rsp, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_start_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];

	memcpy(clicktocall_stop_rsp, skbmsg->body, sizeof(clicktocall_stop_rsp_tcp_t));
	clicktocall_stop_rsp_tcp_display(clicktocall_stop_rsp);
	rv = clicktocall_stop_rsp_encode_to_dbif_msg(clicktocall_stop_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_stop_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_startrecording_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];

	memcpy(clicktocall_startrecording_rsp, skbmsg->body, sizeof(clicktocall_startrecording_rsp_tcp_t));
	clicktocall_startrecording_rsp_tcp_display(clicktocall_startrecording_rsp);
	rv = clicktocall_startrecording_rsp_encode_to_dbif_msg(clicktocall_startrecording_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_startrecording_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_stoprecording_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];

	memcpy(clicktocall_stoprecording_rsp, skbmsg->body, sizeof(clicktocall_stoprecording_rsp_tcp_t));
	clicktocall_stoprecording_rsp_tcp_display(clicktocall_stoprecording_rsp);
	rv = clicktocall_stoprecording_rsp_encode_to_dbif_msg(clicktocall_stoprecording_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_stoprecording_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_service_status_rsp( skb_msg_t *skbmsg) {
	clicktocall_service_status_rsp_tcp_t clicktocall_service_status_rsp[1];

	memcpy(clicktocall_service_status_rsp, skbmsg->body, sizeof(clicktocall_service_status_rsp_tcp_t));
	clicktocall_service_status_rsp_tcp_display(clicktocall_service_status_rsp);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocall_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stop_rpt_tcp_t clicktocall_stop_rpt[1];

	memcpy(clicktocall_stop_rpt, skbmsg->body, sizeof(clicktocall_stop_rpt_tcp_t));
	clicktocall_stop_rpt_tcp_display(clicktocall_stop_rpt);
	rv = clicktocall_stop_rpt_encode_to_dbif_msg(clicktocall_stop_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_stop_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_startrecording_rpt( skb_msg_t *skbmsg) {
	clicktocall_startrecording_rpt_tcp_t clicktocall_startrecording_rpt[1];

	memcpy(clicktocall_startrecording_rpt, skbmsg->body, sizeof(clicktocall_startrecording_rpt_tcp_t));
	clicktocall_startrecording_rpt_tcp_display(clicktocall_startrecording_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocall_stoprecording_rpt( skb_msg_t *skbmsg) {
	clicktocall_stoprecording_rpt_tcp_t clicktocall_stoprecording_rpt[1];

	memcpy(clicktocall_stoprecording_rpt, skbmsg->body, sizeof(clicktocall_stoprecording_rpt_tcp_t));
	clicktocall_stoprecording_rpt_tcp_display(clicktocall_stoprecording_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocall_service_status_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_service_status_rpt_tcp_t clicktocall_service_status_rpt[1];

	memcpy(clicktocall_service_status_rpt, skbmsg->body, sizeof(clicktocall_service_status_rpt_tcp_t));
	clicktocall_service_status_rpt_tcp_display(clicktocall_service_status_rpt);
	rv = clicktocall_service_status_rpt_encode_to_dbif_msg(clicktocall_service_status_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_service_status_rpt_dbif_display(dbif);

	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_clicktocallrecording_heartbeat_req(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, HEARTBEAT_RESPONSE, 0, &skbmsg->header.requestID);
	skb_msg_display_header(&skbmsg->header);
}

void skb_msg_process_clicktocallrecording_binding_req(skb_msg_t *skbmsg, char *userID, char *password) {
	clicktocallrecording_binding_req_tcp_t clicktocallrecording_binding_req;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BINDING_REQUEST, 0, NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	strncpy(clicktocallrecording_binding_req.userID, userID, BINDING_USER_ID_LEN);
	strncpy(clicktocallrecording_binding_req.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &clicktocallrecording_binding_req, sizeof(clicktocallrecording_binding_req));
	clicktocallrecording_binding_req_tcp_display(&clicktocallrecording_binding_req);
}

int skb_msg_process_clicktocallrecording_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID) {
	int rv;
	clicktocallrecording_start_req_tcp_t clicktocallrecording_start_req;

	// DBIF를 body로 변환
	clicktocallrecording_start_req_dbif_display(dbif);
	rv = clicktocallrecording_start_req_decode_dbif_msg(&clicktocallrecording_start_req, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_REQUEST, sizeof(clicktocallrecording_start_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocallrecording_start_req, sizeof(clicktocallrecording_start_req));
	clicktocallrecording_start_req_tcp_display(&clicktocallrecording_start_req);

	return rv;
}

int skb_msg_process_clicktocallrecording_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_stop_req_tcp_t clicktocallrecording_stop_req;

	// DBIF를 body로 변환
	clicktocallrecording_stop_req_dbif_display(dbif);
	rv = clicktocallrecording_stop_req_decode_dbif_msg(&clicktocallrecording_stop_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktocallrecording_stop_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocallrecording_stop_req, sizeof(clicktocallrecording_stop_req));
	clicktocallrecording_stop_req_tcp_display(&clicktocallrecording_stop_req);

	return rv;
}

int skb_msg_process_clicktocallrecording_service_status_req( skb_msg_t *skbmsg) {
	clicktocallrecording_service_status_req_tcp_t clicktocallrecording_service_status_req;

	memcpy(skbmsg->body, &clicktocallrecording_service_status_req, sizeof(clicktocallrecording_service_status_req));
	clicktocallrecording_service_status_req_tcp_display(&clicktocallrecording_service_status_req);

	return eUXC_SUCCESS;
}

void skb_msg_process_clicktocallrecording_heartbeat_rsp(skb_msg_t *skbmsg) {
	skb_msg_display_header(&skbmsg->header);
}

int skb_msg_process_clicktocallrecording_binding_rsp(skb_msg_t *skbmsg) {
	clicktocallrecording_binding_rsp_tcp_t clicktocallrecording_binding_rsp[1];

	memcpy(clicktocallrecording_binding_rsp, skbmsg->body, sizeof(clicktocallrecording_binding_rsp_tcp_t));
	clicktocallrecording_binding_rsp_tcp_display(clicktocallrecording_binding_rsp);
	
	if (clicktocallrecording_binding_rsp->resultCode < 2) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind : %d", clicktocallrecording_binding_rsp->resultCode);
	return -1;
}

int skb_msg_process_clicktocallrecording_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_start_rsp_tcp_t clicktocallrecording_start_rsp[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	memcpy(clicktocallrecording_start_rsp, skbmsg->body, sizeof(clicktocallrecording_start_rsp_tcp_t));
	clicktocallrecording_start_rsp_tcp_display(clicktocallrecording_start_rsp);
	//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(reqID_SID_Map, skbmsg->header.requestID), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "There is no sessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		// uh_int_del(reqID_SID_Map, skbmsg->header.requestID);
	}
	strncpy(gwSessionID, uh_int_get(reqID_GWSID_Map, skbmsg->header.requestID), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "There is no gwSessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		// uh_int_del(reqID_GWSID_Map, skbmsg->header.requestID);
	}
	rv = clicktocallrecording_start_rsp_encode_to_dbif_msg(clicktocallrecording_start_rsp, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_start_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocallrecording_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_stop_rsp_tcp_t clicktocallrecording_stop_rsp[1];

	memcpy(clicktocallrecording_stop_rsp, skbmsg->body, sizeof(clicktocallrecording_stop_rsp_tcp_t));
	clicktocallrecording_stop_rsp_tcp_display(clicktocallrecording_stop_rsp);
	rv = clicktocallrecording_stop_rsp_encode_to_dbif_msg(clicktocallrecording_stop_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_stop_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocallrecording_service_status_rsp( skb_msg_t *skbmsg) {
	clicktocall_service_status_rsp_tcp_t clicktocall_service_status_rsp[1];

	memcpy(clicktocall_service_status_rsp, skbmsg->body, sizeof(clicktocall_service_status_rsp_tcp_t));
	clicktocall_service_status_rsp_tcp_display(clicktocall_service_status_rsp);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocallrecording_start_rpt( skb_msg_t *skbmsg) {
	clicktocallrecording_start_rpt_tcp_t clicktocallrecording_start_rpt[1];

	memcpy(clicktocallrecording_start_rpt, skbmsg->body, sizeof(clicktocallrecording_start_rpt_tcp_t));
	clicktocallrecording_start_rpt_tcp_display(clicktocallrecording_start_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocallrecording_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_stop_rpt_tcp_t clicktocallrecording_stop_rpt[1];

	memcpy(clicktocallrecording_stop_rpt, skbmsg->body, sizeof(clicktocallrecording_stop_rpt_tcp_t));
	clicktocallrecording_stop_rpt_tcp_display(clicktocallrecording_stop_rpt);
	rv = clicktocallrecording_stop_rpt_encode_to_dbif_msg(clicktocallrecording_stop_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_stop_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocallrecording_service_status_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_service_status_rpt_tcp_t clicktocallrecording_service_status_rpt[1];

	memcpy(clicktocallrecording_service_status_rpt, skbmsg->body, sizeof(clicktocallrecording_service_status_rpt_tcp_t));
	clicktocallrecording_service_status_rpt_tcp_display(clicktocallrecording_service_status_rpt);
	rv = clicktocallrecording_service_status_rpt_encode_to_dbif_msg(clicktocallrecording_service_status_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_service_status_rpt_dbif_display(dbif);

	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_clicktoconference_heartbeat_req(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, HEARTBEAT_RESPONSE, 0, &skbmsg->header.requestID);
	skb_msg_display_header(&skbmsg->header);
}

void skb_msg_process_clicktoconference_binding_req(skb_msg_t *skbmsg, char *userID, char *password) {
	clicktoconference_binding_req_tcp_t clicktoconference_binding_req;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BINDING_REQUEST, 0, NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	strncpy(clicktoconference_binding_req.userID, userID, BINDING_USER_ID_LEN);
	strncpy(clicktoconference_binding_req.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &clicktoconference_binding_req, sizeof(clicktoconference_binding_req));
	clicktoconference_binding_req_tcp_display(&clicktoconference_binding_req);
}


int skb_msg_process_clicktoconference_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID) {
	int rv;
	clicktoconference_start_req_tcp_t clicktoconference_start_req;

	// DBIF를 body로 변환
	clicktoconference_start_req_dbif_display(dbif);
	rv = clicktoconference_start_req_decode_dbif_msg(&clicktoconference_start_req, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_REQUEST, sizeof(clicktoconference_start_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_start_req, sizeof(clicktoconference_start_req));
	clicktoconference_start_req_tcp_display(&clicktoconference_start_req);

	return rv;
}

int skb_msg_process_clicktoconference_add_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_add_party_req_tcp_t clicktoconference_add_party_req;

	// DBIF를 body로 변환
	clicktoconference_add_party_req_dbif_display(dbif);
	rv = clicktoconference_add_party_req_decode_dbif_msg(&clicktoconference_add_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_add_party_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_add_party_req, sizeof(clicktoconference_add_party_req));
	clicktoconference_add_party_req_tcp_display(&clicktoconference_add_party_req);

	return rv;
}

int skb_msg_process_clicktoconference_remove_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_remove_party_req_tcp_t clicktoconference_remove_party_req;

	// DBIF를 body로 변환
	clicktoconference_remove_party_req_dbif_display(dbif);
	rv = clicktoconference_remove_party_req_decode_dbif_msg(&clicktoconference_remove_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_remove_party_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_remove_party_req, sizeof(clicktoconference_remove_party_req));
	clicktoconference_remove_party_req_tcp_display(&clicktoconference_remove_party_req);

	return rv;
}

int skb_msg_process_clicktoconference_change_party_media_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_change_party_media_req_tcp_t clicktoconference_change_party_media_req;

	// DBIF를 body로 변환
	clicktoconference_change_party_media_req_dbif_display(dbif);
	rv = clicktoconference_change_party_media_req_decode_dbif_msg(&clicktoconference_change_party_media_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_change_party_media_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_change_party_media_req, sizeof(clicktoconference_change_party_media_req));
	clicktoconference_change_party_media_req_tcp_display(&clicktoconference_change_party_media_req);

	return rv;
}

int skb_msg_process_clicktoconference_change_option_req( skb_msg_t *skbmsg) {
	clicktoconference_change_option_req_tcp_t clicktoconference_change_option_req;

	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_change_option_req, sizeof(clicktoconference_change_option_req));
	clicktoconference_change_option_req_tcp_display(&clicktoconference_change_option_req);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktoconference_get_number_of_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_number_of_party_req_tcp_t clicktoconference_get_number_of_party_req;

	// DBIF를 body로 변환
	clicktoconference_get_number_of_party_req_dbif_display(dbif);
	rv = clicktoconference_get_number_of_party_req_decode_dbif_msg(&clicktoconference_get_number_of_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_get_number_of_party_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_get_number_of_party_req, sizeof(clicktoconference_get_number_of_party_req));
	clicktoconference_get_number_of_party_req_tcp_display(&clicktoconference_get_number_of_party_req);

	return rv;
}

int skb_msg_process_clicktoconference_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_stop_req_tcp_t clicktoconference_stop_req;

	// DBIF를 body로 변환
	clicktoconference_stop_req_dbif_display(dbif);
	rv = clicktoconference_stop_req_decode_dbif_msg(&clicktoconference_stop_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_stop_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_stop_req, sizeof(clicktoconference_stop_req));
	clicktoconference_stop_req_tcp_display(&clicktoconference_stop_req);

	return rv;
}

int skb_msg_process_clicktoconference_play_ment_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_play_ment_req_tcp_t clicktoconference_play_ment_req;

	// DBIF를 body로 변환
	clicktoconference_play_ment_req_dbif_display(dbif);
	rv = clicktoconference_play_ment_req_decode_dbif_msg(&clicktoconference_play_ment_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_play_ment_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_play_ment_req, sizeof(clicktoconference_play_ment_req));
	clicktoconference_play_ment_req_tcp_display(&clicktoconference_play_ment_req);

	return rv;
}

int skb_msg_process_clicktoconference_get_party_status_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_party_status_req_tcp_t clicktoconference_get_party_status_req;

	// DBIF를 body로 변환
	clicktoconference_get_party_status_req_dbif_display(dbif);
	rv = clicktoconference_get_party_status_req_decode_dbif_msg(&clicktoconference_get_party_status_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_get_party_status_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_get_party_status_req, sizeof(clicktoconference_get_party_status_req));
	clicktoconference_get_party_status_req_tcp_display(&clicktoconference_get_party_status_req);

	return rv;
}

int skb_msg_process_clicktoconference_cancel_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_cancel_party_req_tcp_t clicktoconference_cancel_party_req;

	// DBIF를 body로 변환
	clicktoconference_cancel_party_req_dbif_display(dbif);
	rv = clicktoconference_cancel_party_req_decode_dbif_msg(&clicktoconference_cancel_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktoconference_cancel_party_req), NULL);
	skb_msg_display_header(&skbmsg->header);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_cancel_party_req, sizeof(clicktoconference_cancel_party_req));
	clicktoconference_cancel_party_req_tcp_display(&clicktoconference_cancel_party_req);

	return rv;
}

void skb_msg_process_clicktoconference_heartbeat_rsp(skb_msg_t *skbmsg) {
	skb_msg_display_header(&skbmsg->header);
}

int skb_msg_process_clicktoconference_binding_rsp(skb_msg_t *skbmsg) {
	clicktoconference_binding_rsp_tcp_t clicktoconference_binding_rsp[1];

	memcpy(clicktoconference_binding_rsp, skbmsg->body, sizeof(clicktoconference_binding_rsp_tcp_t));
	clicktoconference_binding_rsp_tcp_display(clicktoconference_binding_rsp);
	
	if (clicktoconference_binding_rsp->resultCode < 2) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind : %d", clicktoconference_binding_rsp->resultCode);
	return -1;
}

int skb_msg_process_clicktoconference_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_start_rsp_tcp_t clicktoconference_start_rsp[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	memcpy(clicktoconference_start_rsp, skbmsg->body, sizeof(clicktoconference_start_rsp_tcp_t));
	clicktoconference_start_rsp_tcp_display(clicktoconference_start_rsp);
	//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(reqID_SID_Map, skbmsg->header.requestID), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "There is no sessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		// uh_int_del(reqID_SID_Map, skbmsg->header.requestID);
	}
	strncpy(gwSessionID, uh_int_get(reqID_GWSID_Map, skbmsg->header.requestID), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "There is no gwSessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		// uh_int_del(reqID_GWSID_Map, skbmsg->header.requestID);
	}
	rv = clicktoconference_start_rsp_encode_to_dbif_msg(clicktoconference_start_rsp, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_start_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_add_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_add_party_rsp_tcp_t clicktoconference_add_party_rsp[1];

	memcpy(clicktoconference_add_party_rsp, skbmsg->body, sizeof(clicktoconference_add_party_rsp_tcp_t));
	clicktoconference_add_party_rsp_tcp_display(clicktoconference_add_party_rsp);
	rv = clicktoconference_add_party_rsp_encode_to_dbif_msg(clicktoconference_add_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_add_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_remove_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_remove_party_rsp_tcp_t clicktoconference_remove_party_rsp[1];

	memcpy(clicktoconference_remove_party_rsp, skbmsg->body, sizeof(clicktoconference_remove_party_rsp_tcp_t));
	clicktoconference_remove_party_rsp_tcp_display(clicktoconference_remove_party_rsp);
	rv = clicktoconference_remove_party_rsp_encode_to_dbif_msg(clicktoconference_remove_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_remove_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_party_media_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_change_party_media_rsp_tcp_t clicktoconference_change_party_media_rsp[1];

	memcpy(clicktoconference_change_party_media_rsp, skbmsg->body, sizeof(clicktoconference_change_party_media_rsp_tcp_t));
	clicktoconference_change_party_media_rsp_tcp_display(clicktoconference_change_party_media_rsp);
	rv = clicktoconference_change_party_media_rsp_encode_to_dbif_msg(clicktoconference_change_party_media_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_change_party_media_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_option_rsp( skb_msg_t *skbmsg) {
	clicktoconference_change_option_rsp_tcp_t clicktoconference_change_option_rsp[1];

	memcpy(clicktoconference_change_option_rsp, skbmsg->body, sizeof(clicktoconference_change_option_rsp_tcp_t));
	clicktoconference_change_option_rsp_tcp_display(clicktoconference_change_option_rsp);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktoconference_get_number_of_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_number_of_party_rsp_tcp_t clicktoconference_get_number_of_party_rsp[1];

	memcpy(clicktoconference_get_number_of_party_rsp, skbmsg->body, sizeof(clicktoconference_get_number_of_party_rsp_tcp_t));
	clicktoconference_get_number_of_party_rsp_tcp_display(clicktoconference_get_number_of_party_rsp);
	rv = clicktoconference_get_number_of_party_rsp_encode_to_dbif_msg(clicktoconference_get_number_of_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_get_number_of_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_stop_rsp_tcp_t clicktoconference_stop_rsp[1];

	memcpy(clicktoconference_stop_rsp, skbmsg->body, sizeof(clicktoconference_stop_rsp_tcp_t));
	clicktoconference_stop_rsp_tcp_display(clicktoconference_stop_rsp);
	rv = clicktoconference_stop_rsp_encode_to_dbif_msg(clicktoconference_stop_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_stop_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_play_ment_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_play_ment_rsp_tcp_t clicktoconference_play_ment_rsp[1];

	memcpy(clicktoconference_play_ment_rsp, skbmsg->body, sizeof(clicktoconference_play_ment_rsp_tcp_t));
	clicktoconference_play_ment_rsp_tcp_display(clicktoconference_play_ment_rsp);
	rv = clicktoconference_play_ment_rsp_encode_to_dbif_msg(clicktoconference_play_ment_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_play_ment_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_get_party_status_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_party_status_rsp_tcp_t clicktoconference_get_party_status_rsp[1];

	memcpy(clicktoconference_get_party_status_rsp, skbmsg->body, sizeof(clicktoconference_get_party_status_rsp_tcp_t));
	clicktoconference_get_party_status_rsp_tcp_display(clicktoconference_get_party_status_rsp);
	rv = clicktoconference_get_party_status_rsp_encode_to_dbif_msg(clicktoconference_get_party_status_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_get_party_status_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_cancel_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_cancel_party_rsp_tcp_t clicktoconference_cancel_party_rsp[1];

	memcpy(clicktoconference_cancel_party_rsp, skbmsg->body, sizeof(clicktoconference_cancel_party_rsp_tcp_t));
	clicktoconference_cancel_party_rsp_tcp_display(clicktoconference_cancel_party_rsp);
	rv = clicktoconference_cancel_party_rsp_encode_to_dbif_msg(clicktoconference_cancel_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_cancel_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_add_party_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_add_party_rpt_tcp_t clicktoconference_add_party_rpt[1];

	memcpy(clicktoconference_add_party_rpt, skbmsg->body, sizeof(clicktoconference_add_party_rpt_tcp_t));
	clicktoconference_add_party_rpt_tcp_display(clicktoconference_add_party_rpt);
	rv = clicktoconference_add_party_rpt_encode_to_dbif_msg(clicktoconference_add_party_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_add_party_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_remove_party_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_remove_party_rpt_tcp_t clicktoconference_remove_party_rpt[1];

	memcpy(clicktoconference_remove_party_rpt, skbmsg->body, sizeof(clicktoconference_remove_party_rpt_tcp_t));
	clicktoconference_remove_party_rpt_tcp_display(clicktoconference_remove_party_rpt);
	rv = clicktoconference_remove_party_rpt_encode_to_dbif_msg(clicktoconference_remove_party_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_remove_party_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_party_media_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_change_party_media_rpt_tcp_t clicktoconference_change_party_media_rpt[1];

	memcpy(clicktoconference_change_party_media_rpt, skbmsg->body, sizeof(clicktoconference_change_party_media_rpt_tcp_t));
	clicktoconference_change_party_media_rpt_tcp_display(clicktoconference_change_party_media_rpt);
	rv = clicktoconference_change_party_media_rpt_encode_to_dbif_msg(clicktoconference_change_party_media_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_change_party_media_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_option_rpt( skb_msg_t *skbmsg) {
	clicktoconference_change_option_rpt_tcp_t clicktoconference_change_option_rpt[1];

	memcpy(clicktoconference_change_option_rpt, skbmsg->body, sizeof(clicktoconference_change_option_rpt_tcp_t));
	clicktoconference_change_option_rpt_tcp_display(clicktoconference_change_option_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktoconference_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_stop_rpt_tcp_t clicktoconference_stop_rpt[1];

	memcpy(clicktoconference_stop_rpt, skbmsg->body, sizeof(clicktoconference_stop_rpt_tcp_t));
	clicktoconference_stop_rpt_tcp_display(clicktoconference_stop_rpt);
	rv = clicktoconference_stop_rpt_encode_to_dbif_msg(clicktoconference_stop_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_stop_rpt_dbif_display(dbif);

	return rv;
}
