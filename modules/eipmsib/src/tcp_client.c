#include "tcp_client.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_client_t 
///////////////////////////////////////////////////////////////////////////////////

static tcp_client_t *_g_client = NULL;
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

	return (uxc_plugin_t*)client;
}

static void _tcp_client_destroy( uxc_plugin_t *plugin)
{
	tcp_client_t *client;
	client = (tcp_client_t*) plugin;
	uxc_plugin_final( client->base);
	tcp_client_final( client);
	ux_free( ux_mem_default(), client);

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

	rv = tcp_conf_init( client->conf, client->xcutor, cfile);
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
	int msg_size, msgId;
	upa_peerkey_t peerkey;

	skb_msg_t skbmsg;
	uxc_dbif_t *dbif;
	uxc_ixpc_t ipc_header;

	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length;	//dbif header + body
	msgId = ipcmsg->header.msgId;

	ux_log( UXL_INFO, "2. CALL tcp_client_forward_gwreq (len:%d, msgId:%d) ", msg_size, msgId);

	// IPC에서 DBIF 추출
	dbif = uxc_ipcmsg_get_dbif(ipcmsg);

	//수신한 DBIF 메시지를 msgID에 따라 TCP로 보낼 eIPMS의 메시지로 변경
	switch( msgId / 100) {
	case 1:
		ux_log(UXL_INFO, "2.1. Set Channel clicktocall");
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
			ux_log(UXL_CRT, "Unsupported msgId : %d", msgId);
			return -1;
		}
		break;
	case 2:
		ux_log(UXL_INFO, "2.2. Set Channel clicktocallrecording");
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
			ux_log(UXL_CRT, "Unsupported msgId : %d", msgId);
			return -1;
		}
		break;
	case 3:
		ux_log(UXL_INFO, "2.3. Set Channel clicktoconference");
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
			ux_log(UXL_CRT, "Unsupported msgId : %d", msgId);
			return -1;
		}
		break;
	default:
		ux_log(UXL_CRT, "Unsupported msgId : %d", msgId);
		return -1;
	}
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data in process req.");
		return rv;
	}

	msg_size = skbmsg.header.length;
	ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg.header));
	ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg.header));

	//requestID와 sessionID Bind
	if(!uh_int_put(reqID_SID_Map, skbmsg.header.requestID, sessionID)) {
		ux_log(UXL_CRT, "failed to put to reqID_SID_Map : (%d - %s)", skbmsg.header.requestID, sessionID);
	}
	//requestID와 gwSessionID Bind
	if(!uh_int_put(reqID_GWSID_Map, skbmsg.header.requestID, gwSessionID)) {
		ux_log(UXL_CRT, "failed to put to reqID_GWSID_Map : (%d - %s)", skbmsg.header.requestID, gwSessionID);
	}
	//requestID와 ipc header Bind
	memcpy(&ipc_header, &ipcmsg->header, sizeof(uxc_ixpc_t));
	if(!uh_ipc_put(reqID_IPC_Map, skbmsg.header.requestID, &ipc_header)) {
		ux_log(UXL_CRT, "failed to put to reqID_IPC_Map : (%d)", skbmsg.header.requestID);
	}
	/* ipc header */
	// struct uxc_ixpc_s {
	//     short	srcSubSysId;	/**< source subsystem ID */
	//     short	srcProcId;		/**< srouce process ID */
	//     short	dstSubSysId;	/**< destination subsystem ID*/
	//     short	dstProcId;		/**< destination process ID*/
	//     int      srcQid;			/**< source Q id */
	//     int      dstQid;			/**< destination Q id */
	//     int      msgId;			/**< message ID */
	//     int      cmdId;			/**< command ID */
	//     int		userData;		/**< user data */
	//     int		fdIdx;			/**< fd index */
	//     short    length;			/**< length */
	//     short 	result;			/**< result */
	// };

	//메시지를 Network byte ordering으로 변경
	rv = skb_msg_cvt_order_hton(&skbmsg, msgId);
	if( rv< UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	//TCP Send
	rv = upa_tcp_send2(_g_client->patcp, &peerkey, &skbmsg, msg_size, 1);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data.");
		return -1;
	}
	ux_log( UXL_INFO, "3. Forwarded msg. from gw to eipms (len:%d)", msg_size);
	return UX_SUCCESS;
}

int dbif_forward_eipmsrsp( tcp_client_t *client, uxc_worker_t *worker, upa_tcpmsg_t *tcpmsg)
{
	int rv, msgID;
	skb_msg_t *skbmsg;
	uxc_dbif_t dbif;
	uxc_ixpc_t *dbif_header;
	uxc_ipcmsg_t ipcmsg;

	// 1. receive skbmsg 
	skbmsg = (skb_msg_t *) tcpmsg->netmsg->buffer;

	//endian 복구
	rv = skb_msg_cvt_order_ntoh(skbmsg, tcpmsg->peerkey.chnl_idx, &msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "skbmsg data error");
		return rv;
	}

	skb_msg_display_header(&skbmsg->header);
	// 2. process and response to uxcutor
	//채널 index를 먼저 구분하여 clicktocall, clicktocallrecording, clicktoconference 구분
	switch(tcpmsg->peerkey.chnl_idx) {
	case TCP_CHANNEL_CALL:
		switch(skbmsg->header.messageID) {
		//HEARTBEAT
		case HEARTBEAT_RESPONSE:
			skb_msg_process_clicktocall_heartbeat_rsp(skbmsg);
			break;
		case HEARTBEAT_REQUEST:
			skb_msg_process_clicktocall_heartbeat_req(skbmsg);
			rv = upa_tcp_send2(_g_client->patcp, &tcpmsg->peerkey, &skbmsg, skbmsg->header.length, 1);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "can't send data.");
				return -1;
			}
			return rv;
		//BINDING
		case BINDING_RESPONSE:
			rv = skb_msg_process_clicktocall_binding_rsp(skbmsg);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "failed to bind.");
				//TODO : TCP 연결 끊기
				return -1;
			}
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
			rv = skb_msg_process_clicktocall_stoprecording_rpt(skbmsg);	//no send
			break;
		case SERVICE_STATUS_REPORT:
			rv = skb_msg_process_clicktocall_service_status_rpt(skbmsg, &dbif);
			break;
		default:
			ux_log(UXL_CRT, "Unsupported messageID : %#010x", skbmsg->header.messageID)
			break;	
		}
		break;		
	case TCP_CHANNEL_RECORDING:
		switch(skbmsg->header.messageID) {
		//HEARTBEAT
		case HEARTBEAT_RESPONSE:
			skb_msg_process_clicktocallrecording_heartbeat_rsp(skbmsg);
			break;
		case HEARTBEAT_REQUEST:
			skb_msg_process_clicktocallrecording_heartbeat_req(skbmsg);
			rv = upa_tcp_send2(_g_client->patcp, &tcpmsg->peerkey, &skbmsg, skbmsg->header.length, 1);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "can't send data.");
				return -1;
			}
			return rv;
		//BINDING
		case BINDING_RESPONSE:
			rv = skb_msg_process_clicktocallrecording_binding_rsp(skbmsg);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "failed to bind.");
				//TODO : TCP 연결 끊기
				return -1;
			}
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
			rv = skb_msg_process_clicktocallrecording_service_status_rpt(skbmsg, &dbif);
			break;
		default:
			ux_log(UXL_CRT, "Unsupported messageID : %#010x", skbmsg->header.messageID)
			break;	
		}
		break;
	case TCP_CHANNEL_CONFERENCE:
		switch(skbmsg->header.messageID) {
		//HEARTBEAT
		case HEARTBEAT_RESPONSE:
			skb_msg_process_clicktoconference_heartbeat_rsp(skbmsg);
			break;
		case HEARTBEAT_REQUEST:
			skb_msg_process_clicktoconference_heartbeat_req(skbmsg);
			rv = upa_tcp_send2(_g_client->patcp, &tcpmsg->peerkey, &skbmsg, skbmsg->header.length, 1);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "can't send data.");
				return -1;
			}
			return rv;
		//BINDING
		case BINDING_RESPONSE:
			rv = skb_msg_process_clicktoconference_binding_rsp(skbmsg);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_CRT, "failed to bind.");
				//TODO : TCP 연결 끊기
				return -1;
			}
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
			ux_log(UXL_CRT, "Unsupported messageID : %#010x", skbmsg->header.messageID)
			break;	
		}
		break;
	default:
		ux_log(UXL_CRT, "Unsupported Channel Index : %d", tcpmsg->peerkey.chnl_idx)
		break;
	}		
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data in process rsp.");
		return rv;
	}

	if (msgID != 0) {
		dbif_header = uh_ipc_get(reqID_IPC_Map, skbmsg->header.requestID);
		if (dbif_header == NULL) {
			ux_log(UXL_CRT, "There is no ipc_header of reqID(%d)", skbmsg->header.requestID);
			return -1;
		}
		ipcmsg.header = *dbif_header;
		ipcmsg.header.msgId = msgID;
		ipcmsg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
		memcpy(ipcmsg.data, &dbif, sizeof(dbif));

		rv = tcp_client_send_ipcmsg(client, &ipcmsg, 0); 
		if( rv< UX_SUCCESS) {
			ux_log( UXL_INFO, "can't send skbmsg");
		}
		return rv;
	}
	
	return -1;
}	


int tcp_client_send_ipcmsg( tcp_client_t *client, uxc_ipcmsg_t* ipcmsg, int rv)
{
	int msg_size;

	//DBIF 헤더 설정
	//TODO : serviceID 같은지 확인하기
	ux_log(UXL_CRT, "sending dbif size : %d ",ipcmsg->header.length);
	ux_log(UXL_CRT, "sending dbif msgId : %d ", ipcmsg->header.msgId);
	
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length + sizeof(long) ;

	ipcmsg->mtype = UXC_MTYPE_DB;
	ipcmsg->header.dstQid = ipcmsg->header.srcQid;
	ipcmsg->header.srcQid = client->conf->mqid;
	ipcmsg->header.result = rv;
	
	ux_log(UXL_INFO, "5. Send ipcmsg to %d from %d, size=%d, header=%lu + dbif=%d\n",
		ipcmsg->header.dstQid, ipcmsg->header.srcQid, msg_size, sizeof(uxc_ixpc_t),ipcmsg->header.length); 

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


static int _tcp_client_on_open(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Open TCP connection");
	//TODO : heartbeat 보내기
	//TODO : binding request 보내기
	return eUXC_SUCCESS;
}


static int _tcp_client_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Close TCP connection");
	return eUXC_SUCCESS;
}

