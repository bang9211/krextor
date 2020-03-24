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
	int rv, msg_size, msgId;
	upa_peerkey_t peerkey;

	skb_msg_t skbmsg;
	tcp_msg_t *msg;
	uxc_dbif_t *dbif;
	uxc_ixpc_t *ipc_header;

	char *sessionID = NULL;
	char *gwSessionID = NULL;
	clicktocall_start_req_tcp_t clicktocall_start_req;
	clicktocall_stop_req_tcp_t clicktocall_stop_req;
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req;
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req;

	msg = (tcp_msg_t *)&ipcmsg->header;
	msg_size = sizeof(uxc_ixpc_t) + msg->header.length;	//dbif header + body
	msgId = msg->header.msgId;

	ux_log( UXL_INFO, "2. CALL tcp_client_forward_gwreq (len:%d, msgId:%d) ", msg_size, msgId);

	// IPC에서 DBIF 추출
	dbif = uxc_ipcmsg_get_dbif(ipcmsg);

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

	//수신한 DBIF 메시지를 msgID에 따라 TCP로 보낼 eIPMS의 메시지로 변경
	switch( msgId / 100) {
	case 1:
		ux_log(UXL_INFO, "2.1. Set Channel clicktocall");
		peerkey.chnl_idx = 0; // configuration 첫번째 채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 

		switch(msgId) {
		case DBIF_CALL_START_REQUEST:
			// DBIF를 body로 변환
			clicktocall_start_req_dbif_display(dbif);
			clicktocall_start_req_decode_dbif_msg(&clicktocall_start_req, &sessionID, &gwSessionID, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, START_REQUEST, sizeof(clicktocall_start_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_start_req, sizeof(clicktocall_start_req));
			clicktocall_start_req_tcp_display(&clicktocall_start_req);
			msg_size = skbmsg.header.length;
			break;
		case DBIF_CALL_STOP_REQUEST:
			// DBIF를 body로 변환
			clicktocall_stop_req_dbif_display(dbif);
			clicktocall_stop_req_decode_dbif_msg(&clicktocall_stop_req, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, STOP_REQUEST, sizeof(clicktocall_stop_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_stop_req, sizeof(clicktocall_stop_req));
			clicktocall_stop_req_tcp_display(&clicktocall_stop_req);
			msg_size = skbmsg.header.length;
			break;
		case DBIF_CALL_START_RECORDING_REQUEST:
			// DBIF를 body로 변환
			clicktocall_startrecording_req_dbif_display(dbif);
			clicktocall_startrecording_req_decode_dbif_msg(&clicktocall_startrecording_req, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, START_RECORDING_REQUEST, sizeof(clicktocall_startrecording_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_startrecording_req, sizeof(clicktocall_startrecording_req));
			clicktocall_startrecording_req_tcp_display(&clicktocall_startrecording_req);
			msg_size = skbmsg.header.length;
			break;
		case DBIF_CALL_STOP_RECORDING_REQUEST:
			// DBIF를 body로 변환
			clicktocall_stoprecording_req_dbif_display(dbif);
			clicktocall_stoprecording_req_decode_dbif_msg(&clicktocall_stoprecording_req, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, STOP_RECORDING_REQUEST, sizeof(clicktocall_stoprecording_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_stoprecording_req, sizeof(clicktocall_stoprecording_req));
			clicktocall_stoprecording_req_tcp_display(&clicktocall_stoprecording_req);
			msg_size = skbmsg.header.length;
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
		break;
	case 3:
		ux_log(UXL_INFO, "2.3. Set Channel clicktoconference");
		peerkey.chnl_idx = 2; //  채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 
		break;
	default:
		ux_log(UXL_CRT, "Unsupported msgId : %d", msgId);
		return -1;
	}

	ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg.header));
	ux_log(UXL_CRT, "seding tcp body size : %lu", sizeof(clicktocall_start_req));

	// TODO 1 : DBIF에서 받은 msg에서 sessionID, gwSessionID 제외하여 저장하고 있다가 response에 사용
	// TODO 2 : DBIF에서 받은 msg에서 header의 dstqid, srcqid 저장하고 있다가 response에 사용
	//requestID와 sessionID Bind
	if(!uh_int_put(reqID_SID_Map, skbmsg.header.requestID, sessionID)) {
		ux_log(UXL_CRT, "failed to put to reqID_SID_Map : (%d - %s)", skbmsg.header.requestID, sessionID);
	}
	//requestID와 gwSessionID Bind
	if(!uh_int_put(reqID_GWSID_Map, skbmsg.header.requestID, gwSessionID)) {
		ux_log(UXL_CRT, "failed to put to reqID_GWSID_Map : (%d - %s)", skbmsg.header.requestID, gwSessionID);
	}
	//requestID와 ipc header Bind
    ipc_header = malloc(sizeof(uxc_ixpc_t));
	memcpy(ipc_header, &ipcmsg->header, sizeof(uxc_ixpc_t));
	if(!uh_ipc_put(reqID_IPC_Map, skbmsg.header.requestID, ipc_header)) {
		ux_log(UXL_CRT, "failed to put to reqID_IPC_Map : (%d)", skbmsg.header.requestID);
	}

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
	skb_msg_t *msg;
	uxc_dbif_t dbif;
	char *sessionID;
	char *gwSessionID;
	uxc_ixpc_t *dbif_header;
	tcp_msg_t dbif_msg;
	
	clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];

	// ux_log( UXL_INFO, "5. Recieved TCP response =");

	// 1. receive msg 
	msg = (skb_msg_t *) tcpmsg->netmsg->buffer;

	//endian 복구
	rv = skb_msg_cvt_order_ntoh(msg, tcpmsg->peerkey.chnl_idx, &msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	skb_msg_display_header(&msg->header);
	// 2. process and response to uxcutor
	//채널 index를 먼저 구분하여 clicktocall, clicktocallrecording, clicktoconference 구분
	switch(tcpmsg->peerkey.chnl_idx) {
	case TCP_CHANNEL_CALL:
		switch(msg->header.messageID) {
		case START_RESPONSE:
			memcpy(clicktocall_start_rsp, msg->body, sizeof(clicktocall_start_rsp_tcp_t));
			clicktocall_start_rsp_tcp_display(clicktocall_start_rsp);
			//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
			sessionID = uh_int_get(reqID_SID_Map, msg->header.requestID);
			if (sessionID == NULL) {
				ux_log(UXL_CRT, "There is no sessionID of reqID(%d)", msg->header.requestID);
				return -1;
			}
			gwSessionID = uh_int_get(reqID_GWSID_Map, msg->header.requestID);
			if (gwSessionID == NULL) {
				ux_log(UXL_CRT, "There is no gwSessionID of reqID(%d)", msg->header.requestID);
				return -1;
			}
			dbif_header = uh_ipc_get(reqID_IPC_Map, msg->header.requestID);
			if (dbif_header == NULL) {
				ux_log(UXL_CRT, "There is no ipc_header of reqID(%d)", msg->header.requestID);
				return -1;
			}
			rv = clicktocall_start_rsp_encode_to_dbif_msg(clicktocall_start_rsp, sessionID, gwSessionID, &dbif);
			if (rv <eUXC_SUCCESS) return rv;
			memcpy(dbif_msg.data, &dbif, sizeof(dbif));
			dbif_msg.header = *dbif_header;
			dbif_msg.header.msgId = msgID;
			dbif_msg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
			clicktocall_start_rsp_dbif_display(&dbif);
			break;
		case STOP_RESPONSE:
			memcpy(clicktocall_stop_rsp, msg->body, sizeof(clicktocall_stop_rsp_tcp_t));
			clicktocall_stop_rsp_tcp_display(clicktocall_stop_rsp);
			dbif_header = uh_ipc_get(reqID_IPC_Map, msg->header.requestID);
			if (dbif_header == NULL) {
				ux_log(UXL_CRT, "There is no ipc_header of reqID(%d)", msg->header.requestID);
				return -1;
			}
			rv = clicktocall_stop_rsp_encode_to_dbif_msg(clicktocall_stop_rsp, &dbif);
			if (rv <eUXC_SUCCESS) return rv;
			memcpy(dbif_msg.data, &dbif, sizeof(dbif));
			dbif_msg.header = *dbif_header;
			dbif_msg.header.msgId = msgID;
			dbif_msg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
			clicktocall_stop_rsp_dbif_display(&dbif);
			break;
		case STOP_REPORT:
			break;
		case START_RECORDING_RESPONSE:
			memcpy(clicktocall_startrecording_rsp, msg->body, sizeof(clicktocall_startrecording_rsp_tcp_t));
			clicktocall_startrecording_rsp_tcp_display(clicktocall_startrecording_rsp);
			dbif_header = uh_ipc_get(reqID_IPC_Map, msg->header.requestID);
			if (dbif_header == NULL) {
				ux_log(UXL_CRT, "There is no ipc_header of reqID(%d)", msg->header.requestID);
				return -1;
			}
			rv = clicktocall_startrecording_rsp_encode_to_dbif_msg(clicktocall_startrecording_rsp, &dbif);
			if (rv <eUXC_SUCCESS) return rv;
			memcpy(dbif_msg.data, &dbif, sizeof(dbif));
			dbif_msg.header = *dbif_header;
			dbif_msg.header.msgId = msgID;
			dbif_msg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
			clicktocall_startrecording_rsp_dbif_display(&dbif);
			break;
		case START_RECORDING_REPORT:
			break;
		case STOP_RECORDING_RESPONSE:
			memcpy(clicktocall_stoprecording_rsp, msg->body, sizeof(clicktocall_stoprecording_rsp_tcp_t));
			clicktocall_stoprecording_rsp_tcp_display(clicktocall_stoprecording_rsp);
			dbif_header = uh_ipc_get(reqID_IPC_Map, msg->header.requestID);
			if (dbif_header == NULL) {
				ux_log(UXL_CRT, "There is no ipc_header of reqID(%d)", msg->header.requestID);
				return -1;
			}
			rv = clicktocall_stoprecording_rsp_encode_to_dbif_msg(clicktocall_stoprecording_rsp, &dbif);
			if (rv <eUXC_SUCCESS) return rv;
			memcpy(dbif_msg.data, &dbif, sizeof(dbif));
			dbif_msg.header = *dbif_header;
			dbif_msg.header.msgId = msgID;
			dbif_msg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
			clicktocall_stoprecording_rsp_dbif_display(&dbif);
			break;
		case STOP_RECORDING_REPORT:
			break;
		case SERVICE_STATUS_RESPONSE:
			break;
		case SERVICE_STATUS_REPORT:
			break;
		default:
			ux_log(UXL_CRT, "Unsupported messageID : %#010x", msg->header.messageID)
			break;	
		}
		break;		
	case TCP_CHANNEL_RECORDING:
		break;
	case TCP_CHANNEL_CONFERENCE:
		break;
	default:
		ux_log(UXL_CRT, "Unsupported Channel Index : %d", tcpmsg->peerkey.chnl_idx)
		break;
	}		

	if (dbif_msg.header.msgId != 0) {
		rv = tcp_client_send_ipcmsg(client, &dbif_msg, 0); 
		if( rv< UX_SUCCESS) {
			ux_log( UXL_INFO, "can't send msg");
		}
		return rv;
	}
	
	return -1;
}	


int tcp_client_send_ipcmsg( tcp_client_t *client, tcp_msg_t* msg, int rv)
{
	int msg_size;
	uxc_ipcmsg_t ipcmsg;

	//DBIF 헤더 설정
	//TODO 4 : TCP Client에서 받은 DBIF 요청에 저장했던 dstQid, srcQid 헤더에 추가하기
	//TODO 7 : serviceID 같은지 확인하기
	ux_log(UXL_CRT, "sending dbif size : %d ",msg->header.length);
	ux_log(UXL_CRT, "sending dbif msgId : %d ", msg->header.msgId);
	
	ipcmsg.header = msg->header;
	memcpy(ipcmsg.data, msg->data, ipcmsg.header.length);
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg.header.length + sizeof(long) ;

	ipcmsg.mtype = UXC_MTYPE_DB;
	ipcmsg.header.dstQid = ipcmsg.header.srcQid;
	ipcmsg.header.srcQid = client->conf->mqid;
	ipcmsg.header.result = rv;
	
	ux_log(UXL_INFO, "5. Send ipcmsg to %d from %d, size=%d, header=%lu + dbif=%d\n",
		ipcmsg.header.dstQid, ipcmsg.header.srcQid, msg_size, sizeof(uxc_ixpc_t),ipcmsg.header.length); 

	rv = msgsnd(ipcmsg.header.dstQid, &ipcmsg, msg_size, IPC_NOWAIT);
	if( rv < 0) {
		ux_log( UXL_MAJ, "msgsnd: E=%d, q=%d, len=%lu", errno, ipcmsg.header.dstQid,
					sizeof(uxc_ixpc_t) + ipcmsg.header.length);
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
	return eUXC_SUCCESS;
}


static int _tcp_client_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Close TCP connection");
	return eUXC_SUCCESS;
}

