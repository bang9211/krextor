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

	//수신한 DBIF 메시지를 msgID에 따라 TCP로 보낼 eIPMS의 메시지로 변경
	switch( msgId / 100) {
	case 1:
		ux_log(UXL_INFO, "2.1. Set Channel clicktocall");
		peerkey.chnl_idx = 0; // configuration 첫번째 채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 

		switch(msgId) {
		case CALL_START_REQUEST:
			// DBIF를 body로 변환
			clicktocall_start_req_dbif_display(dbif);
			clicktocall_start_req_decode_dbif_msg(&clicktocall_start_req, sessionID, gwSessionID, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, START_REQUEST, sizeof(clicktocall_start_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_start_req, sizeof(clicktocall_start_req));
			clicktocall_start_req_tcp_display(&clicktocall_start_req);
			msg_size = skbmsg.header.length;
			break;
		case CALL_STOP_REQUEST:
			// DBIF를 body로 변환
			clicktocall_stop_req_dbif_display(dbif);
			clicktocall_stop_req_decode_dbif_msg(&clicktocall_stop_req, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, START_REQUEST, sizeof(clicktocall_stop_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_stop_req, sizeof(clicktocall_stop_req));
			clicktocall_stop_req_tcp_display(&clicktocall_stop_req);
			msg_size = skbmsg.header.length;
			break;
		case CALL_START_RECORDING_REQUEST:
			// DBIF를 body로 변환
			clicktocall_startrecording_req_dbif_display(dbif);
			clicktocall_startrecording_req_decode_dbif_msg(&clicktocall_startrecording_req, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, START_REQUEST, sizeof(clicktocall_startrecording_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_startrecording_req, sizeof(clicktocall_startrecording_req));
			clicktocall_startrecording_req_tcp_display(&clicktocall_startrecording_req);
			msg_size = skbmsg.header.length;
			break;
		case CALL_STOP_RECORDING_REQUEST:
			// DBIF를 body로 변환
			clicktocall_stoprecording_req_dbif_display(dbif);
			clicktocall_stoprecording_req_decode_dbif_msg(&clicktocall_stoprecording_req, dbif);
			// TCP Header 설정
			skb_msg_make_header(&skbmsg.header, START_REQUEST, sizeof(clicktocall_stoprecording_req), NULL);
			skb_msg_display_header(&skbmsg.header);
			// TCP Body 설정
			memcpy(skbmsg.body, &clicktocall_stoprecording_req, sizeof(clicktocall_stoprecording_req));
			clicktocall_stoprecording_req_tcp_display(&clicktocall_stoprecording_req);
			msg_size = skbmsg.header.length;
			break;
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

	ux_log(UXL_INFO, "seding tcp header size : %lu", sizeof(skbmsg.header));
	ux_log(UXL_INFO, "seding tcp body size : %lu", sizeof(clicktocall_start_req));

	// TODO 1 : DBIF에서 받은 msg에서 sessionID, gwSessionID 제외하여 저장하고 있다가 response에 사용
	// TODO 2 : DBIF에서 받은 msg에서 header의 dstqid, srcqid 저장하고 있다가 response에 사용
	//requestID와 sessionID Bind
	if(!uh_int_put(reqIDSIDMap, skbmsg.header.requestID, sessionID)) {
		ux_log(UXL_CRT, "failed to put to reqIDSIDMap : (%d - %s)", skbmsg.header.requestID, sessionID);
	}
	//requestID와 gwSessionID Bind
	if(!uh_int_put(reqIDGWSIDMap, skbmsg.header.requestID, gwSessionID)) {
		ux_log(UXL_CRT, "failed to put to reqIDGWSIDMap : (%d - %s)", skbmsg.header.requestID, gwSessionID);
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

	msg = (skb_msg_t *) tcpmsg->netmsg->buffer;

	ux_log( UXL_INFO, "5. CALL dbif_forward_eipmsrsp =");

	//TODO 0  : 어떻게 할지
	// rv = skb_msg_cvt_order_ntoh(msg, );
	// if( rv < UX_SUCCESS) {
	// 	ux_log(UXL_INFO, "msg data error");
	// 	return rv;
	// }	

	msgID = msg->header.messageID;
	ux_log(UXL_INFO, "received : %d", msgID);

	switch(msgID)
	{
		case START_RESPONSE:
			//TODO 1 : 아래에서 tcp response를 dbif 형식의 메시지로 변경시키기
			// rv = tcp_clicktocall_start_rsp_encode_ipcmsg(clicktocall_start_rsp, msg);
			// if (rv <eUXC_SUCCESS) return rv;

			// rv = tcp_client_send_ipcmsg(client, msg, 0); 
			// if( rv< UX_SUCCESS) {
			// 	ux_log( UXL_INFO, "can't send msg");
			// }
		default:
			break;	
	}			


	return rv;
}	

int tcp_client_send_ipcmsg( tcp_client_t *client,  tcp_msg_t* msg, int rv)
{
	int msg_size;
	uxc_ipcmsg_t ipcmsg;

	ux_log(UXL_CRT, "	[length] %d ",msg->header.length);
	ux_log(UXL_CRT, "	[msgId] %d ", msg->header.msgId);


	//TODO 3 : DBIF 헤더 설정
	//TODO 4 : TCP Client에서 받은 DBIF 요청에 저장했던 dstQid, srcQid 헤더에 추가하기
	//TODO 5 : TCP Client에서 받은 DBIF 요청에 저장했던 sessionID, gwSessionID 추가하기
	//TODO 6 : Filler 2개 제거하기
	//TODO 7 : serviceID 같은지 확인하기

	ipcmsg.header = msg->header;
	memcpy(ipcmsg.data, msg->data, ipcmsg.header.length);
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg.header.length + sizeof(long) ;

	ipcmsg.mtype = UXC_MTYPE_DB;
	ipcmsg.header.dstQid = ipcmsg.header.srcQid;
	ipcmsg.header.srcQid = client->conf->mqid;
	ipcmsg.header.result = rv;
	
	ux_log(UXL_INFO, "6. Send ipcmsg to %d from %d, size=%d, header=%lu + dbif=%d\n",ipcmsg.header.dstQid,
					client->conf->mqid, msg_size, sizeof(uxc_ixpc_t),ipcmsg.header.length); 

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

