#include "tcp_client.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_client_t 
///////////////////////////////////////////////////////////////////////////////////

int request_timeout;
pthread_t timeout_thread;
int timeout_switch = 0;

/** @brief SKB Request ID */
int binding_reqIDs[CHANNEL_SIZE] = {0};
int seconds[CHANNEL_SIZE] = {0};
int last_recv_seconds[CHANNEL_SIZE] = {0};
static tcp_client_t *_g_client = NULL;
pthread_t timer_thread[CHANNEL_SIZE];
int timer_idx[CHANNEL_SIZE] = {0, 1, 2};
ux_channel_t* channel_arr[CHANNEL_SIZE];
upa_peerkey_t peerkey_arr[CHANNEL_SIZE];
int timer_switch[CHANNEL_SIZE] = {0};
int is_heartbeat_sent[CHANNEL_SIZE];

void turn_heartbeat_timer_on(int chnl_idx);
void turn_heartbeat_timer_off(int chnl_idx);

static void _tcp_client_destroy( uxc_plugin_t *plugin);
static int _tcp_client_on_accept(upa_tcp_t *tcp, ux_channel_t *channel, ux_accptor_t *accptor,
				ux_cnector_t *cnector, upa_peerkey_t *peerkey);
static int _tcp_client_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector,
				upa_peerkey_t *peerkey);
static int _tcp_client_on_open(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector,
				upa_peerkey_t *peerkey);

uxc_plugin_t *tcp_client_create( void *xcutor, const char* cfile)
{
	int rv;
	tcp_client_t *client;

	client = (tcp_client_t*)ux_zalloc( ux_mem_default(), sizeof(tcp_client_t));
	rv = uxc_plugin_init( client->base, xcutor, cfile);
	if( rv < eUXC_SUCCESS) {
		ux_free( ux_mem_default(), client);
		return NULL;
	}

	client->base->destroy = _tcp_client_destroy;
	client->base->reconf = NULL;
	client->base->heartbeat = NULL;
	client->base->reload = NULL;

	rv = tcp_client_init( client, (uxc_xcutor_t*)xcutor, client->base->cfile);
	if( rv < eUXC_SUCCESS) {
		ux_free( ux_mem_default(), client);
		return NULL;
	}

	client->patcp = (upa_tcp_t*)uxc_xcutor_get_paif((uxc_xcutor_t*)xcutor, "PA_TCP");
	if( client->patcp == NULL ) { 
		ux_log(UXL_CRT, "Can't find TCP Protocol_Adapter");
		return NULL;
	}

	upa_tcp_set_accept_cb( client->patcp, _tcp_client_on_accept );
	upa_tcp_set_open_cb( client->patcp, _tcp_client_on_open );
	upa_tcp_set_close_cb( client->patcp, _tcp_client_on_close );

	_g_client = client;

	srand(time(NULL));
	create_skb_map();
	request_timeout = client->conf->request_timeout;
	turn_timeout_timer_on();

	return (uxc_plugin_t*)client;
}

static void _tcp_client_destroy( uxc_plugin_t *plugin)
{
	tcp_client_t *client;
	client = (tcp_client_t*) plugin;
	uxc_plugin_final( client->base);
	tcp_client_final( client);
	ux_free( ux_mem_default(), client);

	turn_timeout_timer_off();
	destroy_skb_map();
}

tcp_client_t* tcp_client_instance()
{
	return _g_client; 
}

int tcp_client_init( tcp_client_t *client, uxc_xcutor_t *xcutor, const char* cfile)
{
	int rv;

	client->xcutor = xcutor;

	rv = eipmsib_conf_init( client->conf, client->xcutor, cfile);
	if ( rv < eUXC_SUCCESS) return rv;

	client->patcp = (upa_tcp_t*) uxc_xcutor_get_paif( client->xcutor, "PA_TCP");
	if(client->patcp == NULL) {
		ux_log (UXL_CRT, "Can't find TCP Protocol_Adapter");
		return eUXC_INIT_FAIL;
	}

	client->padbif = (upa_dbif_t*)uxc_xcutor_get_paif( client->xcutor, "PA_DBIF");
	if( client->padbif == NULL ) {
		ux_log(UXL_CRT, "Can't find DBIF Protocol_Adapter");
		return eUXC_INIT_FAIL; 
	}
	ux_log (UXL_CRT, "Protocol Adapter created");
	return eUXC_SUCCESS;
}

void tcp_client_final( tcp_client_t *client)
{
	//nothing
}

int tcp_client_forward_gwreq( tcp_client_t *client, uxc_worker_t *worker, uxc_ipcmsg_t *ipcmsg )
{
	int rv = 0;
	int msg_size, msgId, requestID;
	upa_peerkey_t peerkey;

	skb_msg_t skbmsg;
	uxc_dbif_t *dbif;
	uxc_ixpc_t ipc_header;

	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];
	time_t rawtime;

	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length;	//dbif header + body
	msgId = ipcmsg->header.msgId;

	ux_log( UXL_INFO, "2. CALL tcp_client_forward_gwreq (len:%d, msgId:%d) ", msg_size, msgId);

	// IPC에서 DBIF 추출
	dbif = uxc_ipcmsg_get_dbif(ipcmsg);

	//수신한 DBIF 메시지를 msgID에 따라 TCP로 보낼 eIPMS의 메시지로 변경
	switch( msgId / 100) {
	case 1:
		ux_log(UXL_INFO, "2.1. Channel clicktocall");
		peerkey.chnl_idx = 0; // configuration 첫번째 채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 

		switch(msgId) {
		case DBIF_CALL_START_REQUEST:
			rv = skb_msg_process_clicktocall_start_req(&skbmsg, dbif, sessionID, gwSessionID);
			break;
		case DBIF_CALL_STOP_REQUEST:
			rv = skb_msg_process_clicktocall_stop_req(&skbmsg, dbif);
			break;
		case DBIF_CALL_START_RECORDING_REQUEST:
			rv = skb_msg_process_clicktocall_startrecording_req(&skbmsg, dbif);
			break;
		case DBIF_CALL_STOP_RECORDING_REQUEST:
			rv = skb_msg_process_clicktocall_stoprecording_req(&skbmsg, dbif);
			break;
		default:
			ux_log(UXL_CRT, "unsupported msgId : %d", msgId);
			return -1;
		}
		break;
	case 2:
		ux_log(UXL_INFO, "2.2. Channel clicktocallrecording");
		peerkey.chnl_idx = 1; //  채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 

		switch(msgId) {
		case DBIF_CALL_RECORDING_START_REQUEST:
			rv = skb_msg_process_clicktocallrecording_start_req(&skbmsg, dbif, sessionID, gwSessionID);
			break;
		case DBIF_CALL_RECORDING_STOP_REQUEST:
			rv = skb_msg_process_clicktocallrecording_stop_req(&skbmsg, dbif);
			break;
		default:
			ux_log(UXL_CRT, "unsupported msgId : %d", msgId);
			return -1;
		}
		break;
	case 3:
		ux_log(UXL_INFO, "2.3. Channel clicktoconference");
		peerkey.chnl_idx = 2; //  채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 

		switch(msgId) {
		case DBIF_CONFERENCE_START_REQUEST:
			rv = skb_msg_process_clicktoconference_start_req(&skbmsg, dbif, sessionID, gwSessionID);
			break;
		case DBIF_CONFERENCE_STOP_REQUEST:
			rv = skb_msg_process_clicktoconference_stop_req(&skbmsg, dbif);
			break;
		case DBIF_ADD_PARTY_REQUEST:
			rv = skb_msg_process_clicktoconference_add_party_req(&skbmsg, dbif);
			break;
		case DBIF_REMOVE_PARTY_REQUEST:
			rv = skb_msg_process_clicktoconference_remove_party_req(&skbmsg, dbif);
			break;
		case DBIF_CANCEL_PARTY_REQUEST:
			rv = skb_msg_process_clicktoconference_cancel_party_req(&skbmsg, dbif);
			break;
		case DBIF_CHANGE_PARTY_MEDIA_REQUEST:
			rv = skb_msg_process_clicktoconference_change_party_media_req(&skbmsg, dbif);
			break;
		case DBIF_PLAY_MENT_REQUEST:
			rv = skb_msg_process_clicktoconference_play_ment_req(&skbmsg, dbif);
			break;
		case DBIF_GET_NUMBER_OF_PARTY_REQUEST:
			rv = skb_msg_process_clicktoconference_get_number_of_party_req(&skbmsg, dbif);
			break;
		case DBIF_GET_PARTY_STATUS_REQUEST:
			rv = skb_msg_process_clicktoconference_get_party_status_req(&skbmsg, dbif);
			break;
		default:
			ux_log(UXL_CRT, "unsupported msgId : %d", msgId);
			return -1;
		}
		break;
	default:
		ux_log(UXL_CRT, "unsupported msgId : %d", msgId);
		return -1;
	}
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data in process req.");
		return rv;
	}

	msg_size = skbmsg.header.length;
	requestID = skbmsg.header.requestID;
	// ux_log(UXL_CRT, "sending tcp header size : %lu", sizeof(skbmsg.header));
	// ux_log(UXL_CRT, "sending tcp body size : %lu", msg_size - sizeof(skbmsg.header));

	//requestID와 sessionID Bind
	if(!uh_int_put(reqID_SID_Map, requestID, sessionID)) {
		ux_log(UXL_CRT, "failed to put to reqID_SID_Map : (%d - %s)", requestID, sessionID);
	}
	//requestID와 gwSessionID Bind
	if(!uh_int_put(reqID_GWSID_Map, requestID, gwSessionID)) {
		ux_log(UXL_CRT, "failed to put to reqID_GWSID_Map : (%d - %s)", requestID, gwSessionID);
	}
	//requestID와 ipc header Bind
	memcpy(&ipc_header, &ipcmsg->header, sizeof(uxc_ixpc_t));
	if(!uh_ipc_put(reqID_IPC_Map, requestID, &ipc_header)) {
		ux_log(UXL_CRT, "failed to put to reqID_IPC_Map : (%d)", requestID);
	}
	//requestID와 curreunt time(ms) Bind
	time ( &rawtime );
	if(!uh_tmt_put(reqID_timestamp_Map, requestID, rawtime)) {
		ux_log(UXL_CRT, "failed to put to reqID_timestamp_Map : (%d)", requestID);
	}
	
	//메시지를 Network byte ordering으로 변경
	rv = skb_msg_cvt_order_hton(&skbmsg, msgId);
	if( rv< UX_SUCCESS) {
		ux_log(UXL_INFO, "failed to skb_msg_cvt_order_hton : %d", requestID);
		return rv;
	}

	//TCP Send
	rv = upa_tcp_send2(_g_client->patcp, &peerkey, &skbmsg, msg_size, 1);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", requestID);
		return -1;
	}
	ux_log( UXL_INFO, "3. Forwarded DBIF to TCP msg. from gw to eIPMS, size=%d, header=%lu + body=%lu", msg_size, sizeof(skb_header_t), msg_size - sizeof(skb_header_t));
	return UX_SUCCESS;
}

int dbif_forward_eipmsrsp( tcp_client_t *client, uxc_worker_t *worker, upa_tcpmsg_t *tcpmsg)
{
	int rv, msgID;
	// skb_msg_t *skbmsg;
	skb_msg_t skbmsg[1];
	uxc_dbif_t dbif;
	uxc_ixpc_t *dbif_header;
	uxc_ipcmsg_t ipcmsg;
	int msg_size, requestID, status;

	// 1. receive skbmsg 
	// skbmsg = (skb_msg_t *) tcpmsg->netmsg->buffer;
	memcpy(skbmsg, tcpmsg->netmsg->buffer, sizeof(skb_msg_t));

	//endian 복구
	rv = skb_msg_cvt_order_ntoh(skbmsg, tcpmsg->peerkey.chnl_idx, &msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "failed to skb_msg_cvt_order_ntoh");
		return rv;
	}

	//수신한 메시지가 HEARTBEAT_RESPONSE를 제외하고, response인 경우에만 requestID와 일치하는 requestID가 있는지 확인
	if (skbmsg->header.messageID != HEARTBEAT_RESPONSE && skbmsg->header.messageID / 0x10000000 == 1) {
		if (uh_tmt_get(reqID_timestamp_Map, skbmsg->header.requestID) == -1) {	//not found
			ux_log(UXL_CRT, "failed to get reqID_timestamp_Map : %d", skbmsg->header.requestID);
			return -1;
		}
		uh_tmt_del(reqID_timestamp_Map, skbmsg->header.requestID);	//확인되면, timeout 명단에서 제거
	}

	// 2. process and response to uxcutor
	//채널 index를 먼저 구분하여 clicktocall, clicktocallrecording, clicktoconference 구분
	switch(tcpmsg->peerkey.chnl_idx) {
	case TCP_CHANNEL_CALL:
		switch(skbmsg->header.messageID) {
		//HEARTBEAT
		case HEARTBEAT_RESPONSE:
			skb_msg_display_recv_header(&skbmsg->header);
			is_heartbeat_sent[TCP_CHANNEL_CALL] = 0;
			last_recv_seconds[TCP_CHANNEL_CALL] = seconds[TCP_CHANNEL_CALL];		
			return UX_SUCCESS;
		case HEARTBEAT_REQUEST:
			skb_msg_display_recv_header(&skbmsg->header);
			skb_msg_process_clicktocall_heartbeat_req(skbmsg);
			msg_size = skbmsg->header.length;
			requestID = skbmsg->header.requestID;
			// 메시지를 Network byte ordering으로 변경
			rv = skb_msg_cvt_order_hton2(skbmsg);
			if( rv< UX_SUCCESS) {
				ux_log(UXL_CRT, "failed to skb_msg_cvt_order_hton2 : %d", requestID);
				return -1;
			}
			// ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
			// ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
			rv = upa_tcp_send2(_g_client->patcp, &tcpmsg->peerkey, skbmsg, msg_size, 1);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", requestID);
				return -1;
			}
			return rv;
		//BINDING
		case BINDING_RESPONSE:
			rv = skb_msg_process_clicktocall_binding_rsp(skbmsg);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "channel TCP_CHANNEL_CALL will be stopped");
				//세션 연결 종료
				ux_channel_stop2(channel_arr[TCP_CHANNEL_CALL], UX_TRUE);
			}
			turn_heartbeat_timer_on(TCP_CHANNEL_CALL);
			break;
		//RESPONSE
		case START_RESPONSE:
			rv = skb_msg_process_clicktocall_start_rsp(skbmsg, &dbif);
			break;
		case STOP_RESPONSE:
			rv = skb_msg_process_clicktocall_stop_rsp(skbmsg, &dbif);
			break;
		case START_RECORDING_RESPONSE:
			rv = skb_msg_process_clicktocall_startrecording_rsp(skbmsg, &dbif);
			break;
		case STOP_RECORDING_RESPONSE:
			rv = skb_msg_process_clicktocall_stoprecording_rsp(skbmsg, &dbif);
			break;
		case SERVICE_STATUS_RESPONSE:
			rv = skb_msg_process_clicktocall_service_status_rsp(skbmsg);	//no send
			break;
		//REPORT
		case STOP_REPORT:
			rv = skb_msg_process_clicktocall_stop_rpt(skbmsg, &dbif);
			break;
		case START_RECORDING_REPORT:
			rv = skb_msg_process_clicktocall_startrecording_rpt(skbmsg);	//no send
			break;
		case STOP_RECORDING_REPORT:
			rv = skb_msg_process_clicktocall_stoprecording_rpt(skbmsg);		//no send
			break;
		case SERVICE_STATUS_REPORT:
			rv = skb_msg_process_clicktocall_service_status_rpt(skbmsg, &dbif, &status);
			//착신자 호출 중(2), 정상 서비스 중(0)일 경우에만 gw로 포워딩
			if (status != 2 && status != 0) {
				return UX_SUCCESS;
			}
			break;
		default:
			ux_log(UXL_CRT, "unsupported messageID : %#010x", skbmsg->header.messageID)
			return -1;
		}
		break;		
	case TCP_CHANNEL_RECORDING:
		switch(skbmsg->header.messageID) {
		//HEARTBEAT
		case HEARTBEAT_RESPONSE:
			skb_msg_display_recv_header(&skbmsg->header);
			is_heartbeat_sent[TCP_CHANNEL_RECORDING] = 0;
			last_recv_seconds[TCP_CHANNEL_RECORDING] = seconds[TCP_CHANNEL_RECORDING];		
			return UX_SUCCESS;
		case HEARTBEAT_REQUEST:
			skb_msg_display_recv_header(&skbmsg->header);
			skb_msg_process_clicktocallrecording_heartbeat_req(skbmsg);
			msg_size = skbmsg->header.length;
			requestID = skbmsg->header.requestID;
			// 메시지를 Network byte ordering으로 변경
			rv = skb_msg_cvt_order_hton(skbmsg, 0);
			if( rv< UX_SUCCESS) {
				ux_log(UXL_CRT, "failed to skb_msg_cvt_order_hton : %d", requestID);
				break;
			}
			// ux_log(UXL_CRT, "sending tcp header size : %lu", sizeof(skbmsg->header));
			// ux_log(UXL_CRT, "sending tcp body size : %lu", msg_size - sizeof(skbmsg->header));
			rv = upa_tcp_send2(_g_client->patcp, &tcpmsg->peerkey, skbmsg, msg_size, 1);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", requestID);
				return -1;
			}
			return rv;
		//BINDING
		case BINDING_RESPONSE:
			rv = skb_msg_process_clicktocallrecording_binding_rsp(skbmsg);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "channel TCP_CHANNEL_RECORDING will be stopped");
				//세션 연결 종료
				ux_channel_stop2(channel_arr[TCP_CHANNEL_RECORDING], UX_TRUE);
			}
			turn_heartbeat_timer_on(TCP_CHANNEL_RECORDING);
			break;
		//RESPONSE
		case START_RECORDING_RESPONSE:
			rv = skb_msg_process_clicktocallrecording_start_rsp(skbmsg, &dbif);
			break;
		case STOP_RECORDING_RESPONSE:
			rv = skb_msg_process_clicktocallrecording_stop_rsp(skbmsg, &dbif);
			break;
		case SERVICE_STATUS_RESPONSE:
			rv = skb_msg_process_clicktocallrecording_service_status_rsp(skbmsg);	//no send
			break;
		//REPORT
		case START_REPORT:
			rv = skb_msg_process_clicktocallrecording_start_rpt(skbmsg);	//no send
			break;
		case STOP_REPORT:
			rv = skb_msg_process_clicktocallrecording_stop_rpt(skbmsg, &dbif);
			break;
		case SERVICE_STATUS_REPORT:
			// 수신녹취에서 규격상 이 메시지는 안올것 같아 보임
			rv = skb_msg_process_clicktocallrecording_service_status_rpt(skbmsg, &dbif);
			break;
		default:
			ux_log(UXL_CRT, "unsupported messageID : %#010x", skbmsg->header.messageID)
			break;	
		}
		break;
	case TCP_CHANNEL_CONFERENCE:
		switch(skbmsg->header.messageID) {
		//HEARTBEAT
		case HEARTBEAT_RESPONSE:
			skb_msg_display_recv_header(&skbmsg->header);
			is_heartbeat_sent[TCP_CHANNEL_CONFERENCE] = 0;
			last_recv_seconds[TCP_CHANNEL_CONFERENCE] = seconds[TCP_CHANNEL_CONFERENCE];		
			return UX_SUCCESS;
		case HEARTBEAT_REQUEST:
			skb_msg_display_recv_header(&skbmsg->header);
			skb_msg_process_clicktoconference_heartbeat_req(skbmsg);
			msg_size = skbmsg->header.length;
			requestID = skbmsg->header.requestID;
			// 메시지를 Network byte ordering으로 변경
			rv = skb_msg_cvt_order_hton(skbmsg, 0);
			if( rv< UX_SUCCESS) {
				ux_log(UXL_INFO, "failed to skb_msg_cvt_order_hton : %d", requestID);
				break;
			}
			// ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
			// ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
			rv = upa_tcp_send2(_g_client->patcp, &tcpmsg->peerkey, skbmsg, msg_size, 1);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", requestID);
				return -1;
			}
			return rv;
		//BINDING
		case BINDING_RESPONSE:
			rv = skb_msg_process_clicktoconference_binding_rsp(skbmsg);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "channel TCP_CHANNEL_CONFERENCE will be stopped");
				//세션 연결 종료
				ux_channel_stop2(channel_arr[TCP_CHANNEL_CONFERENCE], UX_TRUE);
			}
			turn_heartbeat_timer_on(TCP_CHANNEL_CONFERENCE);
			break;
		//RESPONSE
		case START_CONFERENCE_RESPONSE:
			rv = skb_msg_process_clicktoconference_start_rsp(skbmsg, &dbif);
			break;
		case ADD_PARTY_RESPONSE:
			rv = skb_msg_process_clicktoconference_add_party_rsp(skbmsg, &dbif);
			break;
		case REMOVE_PARTY_RESPONSE:
			rv = skb_msg_process_clicktoconference_remove_party_rsp(skbmsg, &dbif);
			break;
		case CHANGE_PARTY_MEDIA_RESPONSE:
			rv = skb_msg_process_clicktoconference_change_party_media_rsp(skbmsg, &dbif);
			break;
		case CHANGE_OPTION_RESPONSE:
			rv = skb_msg_process_clicktoconference_change_option_rsp(skbmsg);	//no send
			break;
		case GET_NUMBER_OF_PARTY_RESPONSE:
			rv = skb_msg_process_clicktoconference_get_number_of_party_rsp(skbmsg, &dbif);
			break;
		case STOP_CONFERENCE_RESPONSE:
			rv = skb_msg_process_clicktoconference_stop_rsp(skbmsg, &dbif);
			break;
		case PLAY_MENT_RESPONSE:
			rv = skb_msg_process_clicktoconference_play_ment_rsp(skbmsg, &dbif);
			break;
		case GET_PARTY_STATUS_RESPONSE:
			rv = skb_msg_process_clicktoconference_get_party_status_rsp(skbmsg, &dbif);
			break;
		case CANCEL_PARTY_RESPONSE:
			rv = skb_msg_process_clicktoconference_cancel_party_rsp(skbmsg, &dbif);
			break;
		//REPORT
		case ADD_PARTY_REPORT:
			rv = skb_msg_process_clicktoconference_add_party_rpt(skbmsg, &dbif);
			break;
		case REMOVE_PARTY_REPORT:
			rv = skb_msg_process_clicktoconference_remove_party_rpt(skbmsg, &dbif);
			break;
		case CHANGE_PARTY_MEDIA_REPORT:
			rv = skb_msg_process_clicktoconference_change_party_media_rpt(skbmsg, &dbif);
			break;
		case CHANGE_OPTION_REPORT:
			rv = skb_msg_process_clicktoconference_change_option_rpt(skbmsg);	//no send
			break;
		case STOP_CONFERENCE_REPORT:
			rv = skb_msg_process_clicktoconference_stop_rpt(skbmsg, &dbif);
			break;
		default:
			ux_log(UXL_CRT, "unsupported messageID : %#010x", skbmsg->header.messageID)
			break;	
		}
		break;
	default:
		ux_log(UXL_CRT, "unsupported Channel Index : %d", tcpmsg->peerkey.chnl_idx)
		return -1;
	}
	if (skbmsg->header.messageID != HEARTBEAT_REQUEST) {
		last_recv_seconds[tcpmsg->peerkey.chnl_idx] = seconds[tcpmsg->peerkey.chnl_idx] + 1;
	}

	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data in process rsp.");
		return rv;
	}

	requestID = skbmsg->header.requestID;
	if (msgID != 0) {
		dbif_header = uh_ipc_get(reqID_IPC_Map, requestID);
		if (dbif_header == NULL) {
			ux_log(UXL_CRT, "there is no ipc_header of reqID(%d)", requestID);
			return -1;
		}
		ipcmsg.header = *dbif_header;
		ipcmsg.header.msgId = msgID;
		ipcmsg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
		memcpy(ipcmsg.data, &dbif, sizeof(dbif));

		rv = tcp_client_send_ipcmsg(client, &ipcmsg, 0); 
		if( rv< UX_SUCCESS) {
			ux_log( UXL_INFO, "failed to tcp_client_send_ipcmsg : %d", msgID);
		} else {
			//성공 시 후처리 수행
			uh_ipc_del(reqID_IPC_Map, requestID);
			//stop report인 경우, reqID 제거
			if (skbmsg->header.messageID == STOP_REPORT || 
				skbmsg->header.messageID == STOP_RECORDING_REPORT ||
				skbmsg->header.messageID == STOP_CONFERENCE_REPORT) {
				uh_rid_del(reqID_Set, requestID);
			}
		}
		return rv;
	}
	
	return -1;
}	


int tcp_client_send_ipcmsg( tcp_client_t *client, uxc_ipcmsg_t* ipcmsg, int rv)
{
	int msg_size;

	//DBIF 헤더 설정
	// ux_log(UXL_CRT, "sending dbif size : %d ",ipcmsg->header.length);
	// ux_log(UXL_CRT, "sending dbif msgId : %d ", ipcmsg->header.msgId);
	
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length + sizeof(long) ;

	ipcmsg->mtype = UXC_MTYPE_DB;
	ipcmsg->header.dstQid = ipcmsg->header.srcQid;
	ipcmsg->header.srcQid = client->conf->mqid;
	ipcmsg->header.result = rv;
	
	ux_log(UXL_INFO, "5. Send ipcmsg to %d from %d, size=%d, mtype=%lu + header=%lu + dbif=%d",
		ipcmsg->header.dstQid, ipcmsg->header.srcQid, msg_size, sizeof(long), sizeof(uxc_ixpc_t), ipcmsg->header.length); 

	rv = msgsnd(ipcmsg->header.dstQid, &ipcmsg, msg_size, IPC_NOWAIT);
	if( rv < 0) {
		ux_log( UXL_MAJ, "msgsnd: E=%d, q=%d, len=%lu", errno, ipcmsg->header.dstQid,
					sizeof(uxc_ixpc_t) + ipcmsg->header.length);
		return rv;
	}

	return eUXC_SUCCESS;
}

static int _tcp_client_on_accept(upa_tcp_t *tcp, ux_channel_t *channel, ux_accptor_t *accptor,
				ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Accept TCP connection");
	return eUXC_SUCCESS;
}

void *timeout_function()
{
	int k, i;
	time_t tval;
	time_t rawtime;

	while(timeout_switch > 0) {
		sleep(1);
		time ( &rawtime );

		//hash traverse
		// ux_log(UXL_INFO, "Waiting response=========");
		for (k = kh_begin(reqID_timestamp_Map->h); k != kh_end(reqID_timestamp_Map->h); ++k) {
			if (kh_exist(reqID_timestamp_Map->h, k)) {
				const int key = kh_key(reqID_timestamp_Map->h,k);
				tval = kh_value(reqID_timestamp_Map->h, k);
				// ux_log(UXL_INFO, "  before|- k:%d  s:%d  e:%d  requestID=%d  timestamp=%lu", k, kh_begin(reqID_timestamp_Map->h), kh_end(reqID_timestamp_Map->h), key, tval);
				if ((rawtime - tval) >= request_timeout) {	//timeout
					ux_log(UXL_CRT, "request timeout, requestID : %d", key);
					uh_tmt_del(reqID_timestamp_Map, key);
					// printf("1\n");
					uh_int_del(reqID_SID_Map, key);
					// printf("2\n");
					uh_int_del(reqID_GWSID_Map, key);
					// printf("3\n");
					uh_ipc_del(reqID_IPC_Map, key);
					// printf("4\n");
					// ux_log(UXL_INFO, "  after |- k:%d  s:%d  e:%d  requestID=%d  timestamp=%lu", k, kh_begin(reqID_timestamp_Map->h), kh_end(reqID_timestamp_Map->h), key, tval);

					//bind request인 경우, 연결을 끊어주기
					for (i = 0; i < CHANNEL_SIZE; i++) {
						if (key == binding_reqIDs[i]) {
							ux_log(UXL_CRT, "failed to bind : %d", key);
							ux_channel_stop2(channel_arr[i], UX_TRUE);
						}
					}

					//TODO : timeout 발생 시, 추가로 처리해야할 것 고려하기

					if (k == kh_end(reqID_timestamp_Map->h)) break;
				}
			}
			// ux_log(UXL_INFO, "k:%d  s:%d  e:%d", k, kh_begin(reqID_timestamp_Map->h), kh_end(reqID_timestamp_Map->h));
		}
		// ux_log(UXL_INFO, "=========================");
	}

	return NULL;
}

void turn_timeout_timer_on() {
	if (timeout_switch == 0) {
		timeout_switch = 1;
		ux_log( UXL_INFO, "timeout timer on");
		pthread_create(&timeout_thread, NULL, timeout_function, NULL);
	}
}

void turn_timeout_timer_off() {
	if (timeout_switch > 0) {
		ux_log( UXL_INFO, "timeout timer off");
		timeout_switch = 0;
	}
}

void *t_function(void *chnl_idx)
{
	int rv, requestID;
	int timeout = _g_client->conf->heartbeat_timeout;
	int interval = _g_client->conf->heartbeat_interval;
	int sent_time = 0;
	int cidx;
	skb_msg_t skbmsg[1];

	memcpy(&cidx, chnl_idx, sizeof(int));
	while(timer_switch[cidx] > 0) {
		sleep(1);
		seconds[cidx]++;
		if (is_heartbeat_sent[cidx] == 0) {
			if (seconds[cidx] - last_recv_seconds[cidx] >= interval) {
				ux_log( UXL_INFO, "send heartbeat(chnl_idx : %d)", cidx);
				skb_msg_make_header(&skbmsg->header, HEARTBEAT_REQUEST, 0, NULL);
				skb_msg_display_send_header(&skbmsg->header);
				strcpy(skbmsg[0].body,  "");

				requestID = skbmsg->header.requestID;

				//메시지를 Network byte ordering으로 변경
				rv = skb_msg_cvt_order_hton2(skbmsg);
				if( rv< UX_SUCCESS) {
					ux_log(UXL_INFO, "failed to skb_msg_cvt_order_hton2 : %d", requestID);
					return NULL;
				}

				// ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
				// ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
				rv = upa_tcp_send2(_g_client->patcp, &peerkey_arr[cidx], skbmsg, sizeof(skb_header_t), 1);
				if( rv < UX_SUCCESS) {
					ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", requestID);
					//세션 연결 종료
					ux_channel_stop2(channel_arr[cidx], UX_TRUE);
					return NULL;
				}
				is_heartbeat_sent[cidx] = 1;
				sent_time = seconds[cidx];
			}
		} else if (seconds[cidx] - sent_time >= timeout) {	//TIMEOUT
			//세션 연결 종료
			ux_channel_stop2(channel_arr[cidx], UX_TRUE);
			return NULL;
		}
	}

	return NULL;
}

void turn_heartbeat_timer_on(int chnl_idx) {
	if (timer_switch[chnl_idx] == 0) {
		timer_switch[chnl_idx] = 1;
		ux_log( UXL_INFO, "heartbeat(chnl_idx : %d) timer on", chnl_idx);
		if (chnl_idx < 0 || chnl_idx >= CHANNEL_SIZE) {
			ux_log(UXL_CRT, "failed to tcp_client_open, unsupported chnl_idx : %d", chnl_idx);
			return;
		}
		pthread_create(&timer_thread[chnl_idx], NULL, t_function, &timer_idx[chnl_idx]);
	}
}

void turn_heartbeat_timer_off(int chnl_idx) {
	if (timer_switch[chnl_idx] > 0) {
		ux_log( UXL_INFO, "heartbeat(chnl_idx : %d) timer off", chnl_idx);
		timer_switch[chnl_idx] = 0;
	}
}

static int _tcp_client_on_open(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{

	skb_msg_t skbmsg[1];
	int msg_size, requestID, rv;
	int chnl_idx = peerkey->chnl_idx;
	time_t rawtime;
	char *binding_user_id;
	char *binding_password;

	ux_log( UXL_INFO, "Open TCP connection");

	//heartbeat 초기화
	// 1) IP녹취 시스템은 메시지의 교환이 없는 주기에는 세션의 유효성 검사를 위해 개방형GW로 Heartbeat 요청 메시지를 전달한다. 
	//    이 같은 경우 개방형 GW는 반드시 Heartbeat 응답 메시지를 IP녹취시스템으로 전달하여야 한다.
	// 2) 필요에 의해 개방형GW도 IP녹취시스템으로 Heartbeat 요청 메시지를 전송할 수 있으며, 
	//    이에 대해서 IP녹취 시스템은 바로 Heartbeat 응답 메시지를 개방형 GW로 전달하여야 한다.
	// 3) 세션의 유효성 검사는 2초(주기) 이상 상대 시스템으로부터 수신하는 메시지가 없을 때 요청 메시지를 전송하며, 
	//    7초(Timeout) 이상 Heartbeat응답 메시지를 포함하여 어떠한 메시지의 수신도 없을 때는 연결된 세션을 종료한다.(단, 협의에 의해 변경이 가능하다)
	channel_arr[chnl_idx] = channel;
	peerkey_arr[chnl_idx] = *peerkey;
	timer_switch[chnl_idx] = 0;
	is_heartbeat_sent[chnl_idx] = 0;
	seconds[chnl_idx] = 0;
	last_recv_seconds[chnl_idx] = 0;
	//heartbeat는 bind response 받은 이후 수행

	//binding request 전송
	ux_log( UXL_INFO, "send binding request(chnl_idx : %d)", chnl_idx);
	switch(chnl_idx) {
	case TCP_CHANNEL_CALL:
		binding_user_id = _g_client->conf->call_binding_user_id;
		binding_password = _g_client->conf->call_binding_password;
		break;
	case TCP_CHANNEL_RECORDING:
		binding_user_id = _g_client->conf->recording_binding_user_id;
		binding_password = _g_client->conf->recording_binding_password;
		break;
	case TCP_CHANNEL_CONFERENCE:
		binding_user_id = _g_client->conf->conference_binding_user_id;
		binding_password = _g_client->conf->conference_binding_password;
		break;
	default:
		ux_log(UXL_CRT, "failed to skb_msg_make_bind_request : wrong channel index");
		return -1;
	}
	rv = skb_msg_make_bind_request(skbmsg, chnl_idx, binding_user_id, binding_password);
	if( rv< UX_SUCCESS) {
		ux_log(UXL_CRT, "failed to skb_msg_make_bind_request");
		return -1;
	}
	msg_size = skbmsg->header.length;
	requestID = skbmsg->header.requestID;

	//Bind request의 requestID와 current time(ms) Bind
	time ( &rawtime );
	if(!uh_tmt_put(reqID_timestamp_Map, requestID, rawtime)) {
		ux_log(UXL_CRT, "failed to put to reqID_timestamp_Map : (%d)", requestID);
	}

	//Bind requestID 저장
	binding_reqIDs[chnl_idx] = requestID;

	//메시지를 Network byte ordering으로 변경
	rv = skb_msg_cvt_order_hton3(skbmsg, chnl_idx);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_CRT, "failed to skb_msg_cvt_order_hton3 : %d", requestID);
		return -1;
	}

	// ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
	// ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
	rv = upa_tcp_send2(_g_client->patcp, &peerkey_arr[chnl_idx], skbmsg, msg_size, 1);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", requestID);
		//세션 연결 종료
		ux_channel_stop2(channel_arr[chnl_idx], UX_TRUE);
		return -1;
	}


	return eUXC_SUCCESS;
}


static int _tcp_client_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	turn_heartbeat_timer_off(peerkey->chnl_idx);
	ux_log( UXL_INFO, "Close TCP connection");
	return eUXC_SUCCESS;
}

