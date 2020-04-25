#include "skb_msg.h"
 
////////////////////////////////////////////////////////////////////////////////////
// functions for skb_msg_t
////////////////////////////////////////////////////////////////////////////////////

void skb_msg_init(int heartbeat_display) {
	srand(time(NULL));
	create_skb_map();
	_heartbeat_display = heartbeat_display;
	serialNumber = 0;
}

void skb_msg_end() {
	destroy_skb_map();
}

/**
 * @brief DBIF msgType를 eIPMS msgType로 바꾼다.
 * @param dbif_msgID DBIF message Type
 * @return eIPMS message Type, except -1
 */
int skb_msg_cvt_to_skb_msg_id(int dbif_msgID) {
	switch (dbif_msgID) {
		case DBIF_GW_DELIVER:
			return DELIVER;
		case DBIF_GW_REPORT_ACK:
			return REPORT_ACK;
		case DBIF_SMSC_DELIVER_ACK:
			return DELIVER_ACK;
		case DBIF_SMSC_REPORT:
			return REPORT;
		case DBIF_SMSC_DELIVER:
			return DELIVER;
		case DBIF_SMSC_REPORT_ACK:
			return REPORT_ACK;
		case DBIF_GW_DELIVER_ACK:
			return DELIVER_ACK;
		case DBIF_GW_REPORT:
			return REPORT;
		default:
			ux_log(UXL_CRT, "unsupported dbif msg id : %d", dbif_msgID);
			return -1;
	}
}

/**
 * @brief eIPMS msgType를 DBIF msgType로 바꾼다.
 * @param msgID eIPMS message Type
 * @return DBIF message Type, except -1 
 */
int skb_msg_cvt_to_dbif_msg_id(int chnl_idx, int msgID) {
	switch (chnl_idx) {
		case TCP_CHANNEL_SMS_CLI:
			switch (msgID) {
				case DELIVER:
					return DBIF_GW_DELIVER;
				case REPORT_ACK:
					return DBIF_GW_REPORT_ACK;
				case DELIVER_ACK:
					return DBIF_SMSC_DELIVER_ACK;
				case REPORT:
					return DBIF_SMSC_REPORT;
				case BIND:
				case BIND_ACK:
				case ALIVE:
				case ALIVE_ACK:
					return NONE_DBIF_MESSAGE;
				default:
					ux_log(UXL_CRT, "unsupported msg type : %#010x", msgID);
					return -1;
			}
		case TCP_CHANNEL_SMS_SVR:
			switch (msgID) {
				case DELIVER:
					return DBIF_SMSC_DELIVER;
				case REPORT_ACK:
					return DBIF_SMSC_REPORT_ACK;
				case DELIVER_ACK:
					return DBIF_GW_DELIVER_ACK;
				case REPORT:
					return DBIF_GW_REPORT;
				case BIND:
				case BIND_ACK:
				case ALIVE:
				case ALIVE_ACK:
					return NONE_DBIF_MESSAGE;
				default:
					ux_log(UXL_CRT, "unsupported msg type : %#010x", msgID);
					return -1;
			}
		default:
			ux_log(UXL_CRT, "unsupported chnl_idx : %d", chnl_idx);
			return -1;
	}
}

/**
 * @brief network eIPMS 메시지의 값들을 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton(skb_msg_t *msg, int dbif_msgID)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;

    deliver_msg_tcp_t *deliver_msg;
    deliver_ack_msg_tcp_t *deliver_ack_msg;
    report_msg_tcp_t *report_msg;
    report_ack_msg_tcp_t *report_ack_msg;

    header = &msg->header;
	
    switch(dbif_msgID) {
	//client side hton
    case DBIF_GW_DELIVER:
	case DBIF_SMSC_DELIVER:
		deliver_msg = (deliver_msg_tcp_t *) msg->body;
        deliver_msg->teleServiceID = htonl(deliver_msg->teleServiceID);
        deliver_msg->serialNumber = htonl(deliver_msg->serialNumber);
        break;
    case DBIF_GW_REPORT_ACK:
	case DBIF_SMSC_REPORT_ACK:
		report_ack_msg = (report_ack_msg_tcp_t *) msg->body;
		report_ack_msg->result = htonl(report_ack_msg->result);
        break;
	//server side hton
	case DBIF_GW_DELIVER_ACK:
	case DBIF_SMSC_DELIVER_ACK:
		deliver_ack_msg = (deliver_ack_msg_tcp_t *) msg->body;
		deliver_ack_msg->result = htonl(deliver_ack_msg->result);
		deliver_ack_msg->serialNumber = htonl(deliver_ack_msg->serialNumber);
		break;
	case DBIF_GW_REPORT:
	case DBIF_SMSC_REPORT:
		report_msg = (report_msg_tcp_t *) msg->body;
		report_msg->result = htonl(report_msg->result);
		report_msg->serialNumber = htonl(report_msg->serialNumber);
		report_msg->deliverTime = htonl(report_msg->deliverTime);
		break;
	default:
		ux_log( UXL_INFO, "unknown DBIF Msg Id : [%d]\n", dbif_msgID);
		break;
    }

	/* skb header convert */
	header->length = htonl(header->length);
	header->msgType = htonl(header->msgType);
#endif

	return UX_SUCCESS;
}

/**
 * @brief network eIPMS 메시지의 header를 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton2(skb_msg_t *msg)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;
    header = &msg->header;

	/* skb header convert */
	header->length = htonl(header->length);
	header->msgType = htonl(header->msgType);
#endif

	return UX_SUCCESS;
}

/**
 * @brief network eIPMS binding response 메시지를 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton3(skb_msg_t *msg, int chnl_idx)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;
	bind_ack_msg_tcp_t *bind_ack_msg;

	switch(chnl_idx) {
	case TCP_CHANNEL_SMS_CLI:
		bind_ack_msg = (bind_ack_msg_tcp_t *) msg->body;
        bind_ack_msg->result = htonl(bind_ack_msg->result);
		break;
	default:
		break;
	}

    header = &msg->header;

	/* skb header convert */
	header->length = htonl(header->length);
	header->msgType = htonl(header->msgType);
#endif

	return UX_SUCCESS;
}


/**
 * @brief network 메시지의 값들을 host byte ordering으로 바꾸고, 실행 결과와 DBIF msgId를 반환한다.
 * @param msg network message
 * @return 실행 결과, msgId(response의 경우 NULL)
 */
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int chnIdx, int *msgId)
{
#if !UX_BIGENDIAN
    skb_header_t *header;

	bind_ack_msg_tcp_t *bind_ack_msg;
    deliver_msg_tcp_t *deliver_msg;
    deliver_ack_msg_tcp_t *deliver_ack_msg;
    report_msg_tcp_t *report_msg;
    report_ack_msg_tcp_t *report_ack_msg;

	if (msg == NULL) return -1;

    header = &msg->header;

	/* skb header convert */
	header->length = ntohl(header->length);
	header->msgType = ntohl(header->msgType);

	//body 복구
	switch(msg->header.msgType)
	{
		case ALIVE:				//처리 필요 없음
			break;
		case ALIVE_ACK:			//처리 필요 없음
			break;
		case BIND:				//처리 필요 없음
			break;
		case BIND_ACK:
			bind_ack_msg = (bind_ack_msg_tcp_t *) msg->body;
			bind_ack_msg->result = ntohl(bind_ack_msg->result);
			break;
		case DELIVER:
			deliver_msg = (deliver_msg_tcp_t *) msg->body;
			deliver_msg->teleServiceID = ntohl(deliver_msg->teleServiceID);
			deliver_msg->serialNumber = ntohl(deliver_msg->serialNumber);
			break;
		case DELIVER_ACK:
			deliver_ack_msg = (report_ack_msg_tcp_t *) msg->body;
			deliver_ack_msg->result = ntohl(deliver_ack_msg->result);
			deliver_ack_msg->serialNumber = ntohl(deliver_ack_msg->serialNumber);
			break;
		case REPORT:
			report_msg = (report_msg_tcp_t *) msg->body;
			report_msg->result = ntohl(report_msg->result);
			report_msg->serialNumber = ntohl(report_msg->serialNumber);
			report_msg->deliverTime = ntohl(report_msg->deliverTime);
			break;
		case REPORT_ACK:
			report_ack_msg = (report_ack_msg_tcp_t *) msg->body;
			report_ack_msg->result = ntohl(report_ack_msg->result);
			break;
		default:
			ux_log(UXL_CRT, "unsupported msgType : %#010x", msg->header.msgType);
			// skb_msg_get_header_display(&msg->header, headerLog);
			// ux_log(UXL_CRT, "unsupported msg type : %s", headerLog);
			return -1;
			// break;
	}
#endif
	//DBIF로 보내져야 하는 메시지인 경우, DBIF msgId 설정
	switch(msg->header.msgType) {
	//client
	case DELIVER_ACK:
		*msgId = DBIF_SMSC_DELIVER_ACK;
		break;
	case REPORT:
		*msgId = DBIF_SMSC_REPORT;
		break;
	//server
	case DELIVER:
		*msgId = DBIF_SMSC_DELIVER;
		break;
	case REPORT_ACK:
		*msgId = DBIF_SMSC_REPORT_ACK;
		break;
	default:
		*msgId = NONE_DBIF_MESSAGE;
		break;
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
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey, int dbif_msgID)
{
	ux_log( UXL_INFO, "= msg_send_skbmsg =");
	int rv, msg_size;
		
	msg_size = msg->header.length; 

	rv = skb_msg_cvt_order_hton(msg, dbif_msgID);
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

int32_t generateSerialNumber() {
	return serialNumber++;
}

int check_header(skb_header_t* header) {
	//TODO
	return 0;
}

void skb_msg_make_header(skb_header_t* header, int32_t msgType, int32_t bodySize) {
	int32_t temp;

	header->length = bodySize;
	header->msgType = msgType;
}

int skb_msg_make_bind_request(skb_msg_t *skbmsg, char *id, char *pw) {
	skb_msg_process_bind_msg(skbmsg, id, pw);
	return UX_SUCCESS;
}

void skb_msg_get_header_display(skb_header_t* header, char *log) {
	char temp[SKB_HEADER_DISPLAY_SIZE] = "";
	sprintf(temp,
	"  [length] %d\n"
	"  [msgType] %#010x",
		header->length, 
		header->msgType);
	strcat(log, temp);
}

void skb_msg_get_send_header_display(skb_header_t* header, char *log) {
	strcpy(log, "");
	strcat(log, "Sending TCP \nHeader\n");
	skb_msg_get_header_display(header, log);
}

void skb_msg_get_recv_header_display(skb_header_t* header, char *log) {
	strcpy(log, "");
	strcat(log, "Received TCP \nHeader\n");
	skb_msg_get_header_display(header, log);
}

void skb_msg_display_send_header(skb_header_t* header) {
	char log[SKB_HEADER_DISPLAY_SIZE] = "";
	skb_msg_get_send_header_display(header, log);
	ux_log(UXL_INFO, "%s", log);
}

void skb_msg_display_recv_header(skb_header_t* header) {
	char log[SKB_HEADER_DISPLAY_SIZE] = "";
	skb_msg_get_recv_header_display(header, log);
	ux_log(UXL_INFO, "%s", log);
}

void skb_msg_display_send_alive_msg(skb_header_t* header) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Sending TCP ALIVE msg in client");
}

void skb_msg_display_recv_alive_msg(skb_header_t* header) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Received TCP ALIVE msg in client");
}

void skb_msg_display_send_alive_ack_msg(skb_header_t* header) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Sending TCP ALIVE_ACK msg in client");
}

void skb_msg_display_recv_alive_ack_msg(skb_header_t* header) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Received TCP ALIVE_ACK msg in client");
}

void create_skb_map() {
	sn_Set = uh_rid_init();
	sn_timestamp_Map = uh_tmt_init();
	sn_SID_Map = uh_int_init();    
	sn_GWSID_Map = uh_int_init();
	sn_IPC_Map = uh_ipc_init();
}

void destroy_skb_map() {
//    uxc_ixpc_t* tval = 0;
//    khiter_t k;
//    for (k = kh_begin(sn_IPC_Map->h); k != kh_end(sn_IPC_Map->h); ++k) {
//       if (kh_exist(sn_IPC_Map->h, k)) {
//          const char *key = kh_key(sn_IPC_Map->h, k);
//          tval = kh_value(sn_IPC_Map->h, k);
// 		 free(tval);
//       }
//    }
	uh_rid_destroy(sn_Set);
	uh_tmt_destroy(sn_timestamp_Map);
	uh_int_destroy(sn_SID_Map);
	uh_int_destroy(sn_GWSID_Map);
	uh_ipc_destroy(sn_IPC_Map);
}

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_alive(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, ALIVE_ACK, 0);
}

void skb_msg_process_bind_msg(skb_msg_t *skbmsg, char *userID, char *password) {
	bind_msg_tcp_t bind_msg;
	int msg_size = sizeof(bind_msg);
	// TCP Body 설정
	strncpy(bind_msg.userID, userID, BINDING_USER_ID_LEN);
	strncpy(bind_msg.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &bind_msg, msg_size);
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BIND, msg_size);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	clicktocall_binding_req_tcp_display(headerLog, &bind_msg);
}

int skb_msg_process_deliver_msg( skb_msg_t *skbmsg, uxc_dbif_t *dbif, 
	char *sessionID, char *gwSessionID, int *serialNumber) {
	int rv;
	deliver_msg_tcp_t deliver_msg;

	// DBIF를 body로 변환
	deliver_msg_dbif_display(dbif);
	rv = deliver_msg_decode_dbif_msg(&deliver_msg, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, DELIVER, sizeof(deliver_msg));
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	*serialNumber = generateSerialNumber();
	deliver_msg.serialNumber = *serialNumber;
	memcpy(skbmsg->body, &deliver_msg, sizeof(deliver_msg));
	deliver_msg_tcp_display(headerLog, &deliver_msg);

	return rv;
}

int skb_msg_process_report_ack_msg( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	report_ack_msg_tcp_t report_ack_msg;

	// DBIF를 body로 변환
	report_ack_msg_dbif_display(dbif);
	rv = report_ack_msg_decode_dbif_msg(&report_ack_msg, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, REPORT_ACK, sizeof(report_ack_msg));
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &report_ack_msg, sizeof(report_ack_msg));
	report_ack_msg_tcp_display(headerLog, &report_ack_msg);

	return rv;
}

void skb_msg_process_alive_ack(skb_msg_t *skbmsg) {
	// skb_msg_get_header_display(&skbmsg->header, headerLog);
}

int skb_msg_process_bind_ack_msg(skb_msg_t *skbmsg) {
	bind_ack_msg_tcp_t bind_ack_msg[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(bind_ack_msg, skbmsg->body, sizeof(bind_ack_msg_tcp_t));
	bind_ack_msg_tcp_display(headerLog, bind_ack_msg);
	
	if (bind_ack_msg->result == 1) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind(resultCode : %d)", bind_ack_msg->result);
	return -1;
}

int skb_msg_process_deliver_ack_msg( skb_msg_t *skbmsg, uxc_dbif_t *dbif, int *serialNumber) {
	int rv;
	deliver_ack_msg_tcp_t deliver_ack_msg[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(deliver_ack_msg, skbmsg->body, sizeof(deliver_ack_msg_tcp_t));
	deliver_ack_msg_tcp_display(headerLog, deliver_ack_msg);
	//serialNumber에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(sn_SID_Map, deliver_ack_msg->serialNumber), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no sessionID of reqID(%d)", deliver_ack_msg->serialNumber);
		return -1;
	}
	strncpy(gwSessionID, uh_int_get(sn_GWSID_Map, deliver_ack_msg->serialNumber), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no gwSessionID of reqID(%d)", deliver_ack_msg->serialNumber);
		return -1;
	}
	rv = deliver_ack_msg_encode_to_dbif_msg(deliver_ack_msg, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	deliver_ack_msg_dbif_display(dbif);
	*serialNumber = deliver_ack_msg->serialNumber;

	return rv;
}

int skb_msg_process_report_msg( skb_msg_t *skbmsg, uxc_dbif_t *dbif, int *serialNumber) {
	int rv;
	report_msg_tcp_t report_msg[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(report_msg, skbmsg->body, sizeof(report_msg_tcp_t));
	report_msg_tcp_display(headerLog, report_msg);
	//serialNumber에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(sn_SID_Map, report_msg->serialNumber), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no sessionID of reqID(%d)", report_msg->serialNumber);
		return -1;
	} else {
		uh_int_del(sn_SID_Map, report_msg->serialNumber);
	}
	strncpy(gwSessionID, uh_int_get(sn_GWSID_Map, report_msg->serialNumber), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no gwSessionID of reqID(%d)", report_msg->serialNumber);
		return -1;
	} else {
		uh_int_del(sn_GWSID_Map, report_msg->serialNumber);
	}
	rv = report_msg_encode_to_dbif_msg(report_msg, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	report_msg_dbif_display(dbif);
	*serialNumber = report_msg->serialNumber;

	return rv;
}