#include "tcp_server.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_server_t 
///////////////////////////////////////////////////////////////////////////////////

static tcp_server_t *_g_server = NULL;

static void _tcp_server_destroy( uxc_plugin_t *pi);
static int tcp_server_handle_clicktocall_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg );
static int _tcp_server_get_thrid( uxc_paif_t *paif, uxc_msg_t *msg);
static int _tcp_server_on_accept(upa_tcp_t *tcp, ux_channel_t *channel, ux_accptor_t *accptor,
				ux_cnector_t *cnector, upa_peerkey_t *peerkey);
static int _tcp_server_on_open(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector,
				upa_peerkey_t *peerkey);
static int _tcp_server_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector,
				upa_peerkey_t *peerkey);


uxc_plugin_t* tcp_server_create( void *xcutor, const char *cfile)
{
	int rv;
	tcp_server_t *plugin;

	plugin = (tcp_server_t*)ux_zalloc( ux_mem_default(), sizeof(tcp_server_t));
	rv = uxc_plugin_init( plugin->base, xcutor, cfile);
	if( rv < eUXC_SUCCESS) {
		ux_free( ux_mem_default(), plugin);
		return NULL;
	}

	plugin->base->destroy = _tcp_server_destroy;
	plugin->base->reconf = NULL;
	plugin->base->heartbeat = NULL;
	plugin->base->reload = NULL;
	plugin->xcutor = (uxc_xcutor_t*) xcutor;

	plugin->patcp = (upa_tcp_t*) uxc_xcutor_get_paif( uxc_xcutor_instance(), "PA_TCP");
	if( plugin->patcp == NULL ) { 
		ux_log(UXL_CRT, "Can't find TCP Protocol_Adapter");
		return NULL;
	}

	uxc_paif_set_thrid_func((uxc_paif_t*)plugin->patcp, _tcp_server_get_thrid);
	upa_tcp_set_accept_cb( plugin->patcp, _tcp_server_on_accept );
	upa_tcp_set_open_cb( plugin->patcp, _tcp_server_on_open );
	upa_tcp_set_close_cb( plugin->patcp, _tcp_server_on_close );

	ux_log (UXL_CRT, "Protocol Adapter created");
	_g_server = plugin;
	return (uxc_plugin_t*)plugin;
}

static void _tcp_server_destroy( uxc_plugin_t *pi)
{
	tcp_server_t *plugin;

	ux_log( UXL_INFO, "destroy server_final()");
	plugin = (tcp_server_t*)pi;
	uxc_plugin_final( plugin->base);
	ux_free( ux_mem_default(), plugin);
}

tcp_server_t* tcp_server_instance()
{
	return _g_server; 
}

static int _tcp_server_get_thrid( uxc_paif_t *paif, uxc_msg_t *msg)
{
	static int count = 0;
    int nworker = upa_tcp_get_worker_count( (upa_tcp_t*)paif);
    int idx = nworker ? count++ % nworker : 0;
	return idx;
}


int tcp_server_handle_eipmsreq( tcp_server_t *server, uxc_worker_t* worker, upa_tcpmsg_t *tcpmsg)
{
	skb_msg_t *skbmsg;
	tcp_msg_t *msg;
	int msgID,rv;

	//response : 받기는 하지만 gw에 전달해줄 필요는 없음. 향후 처리가 필요할 수 있으므로 데이터 파싱까지는 수행
	clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];

	//TODO : request(report) 처리
	//CallServiceStatusReport
	//CallEndReport

	// 1. receive msg 
	msg = (skb_msg_t *)tcpmsg->netmsg->buffer;
	ux_log(UXL_INFO, "received header size : %lu", sizeof(skbmsg->header));
	

	//Header만 endian 복구 
	// => TODO : header endian 복구하고, messageID와 채널 인덱스를 조합하여 msgId를 생성하고 
	// 그걸 받아서 전체 endian 복구하게 하자
	rv = skb_header_cvt_order_ntoh(&skbmsg->header);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	// 2. process and response to uxcutor
	//채널 index를 먼저 구분하여 clicktocall, clicktocallrecording, clicktoconference 구분
	switch(tcpmsg->peerkey.chnl_idx) {
		case TCP_CHANNEL_CALL:
			switch(skbmsg->header.messageID)
			{
				case START_RESPONSE:
					//TODO : body도 endian 복구하여 clicktocall_start_rsp 만들기
					//TODO : requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 만들기
					//TODO : 기존에 저장한 qid 파악해 dbif 보내기
					return tcp_server_handle_clicktocall_req(server, worker, tcpmsg, msg);
				case STOP_RESPONSE:
				case STOP_REPORT:
				case START_RECORDING_RESPONSE:
				case START_RECORDING_REPORT:
				case STOP_RECORDING_RESPONSE:
				case STOP_RECORDING_REPORT:
				case SERVICE_STATUS_RESPONSE:
				case SERVICE_STATUS_REPORT:
				default:
					ux_log(UXL_CRT, "Unsupported messageID : %#010x", skbmsg->header.messageID)
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
	return -1;
}

static int tcp_server_handle_clicktocall_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg )
{
	// int rv;
	// uxc_dbif_t *dbif;
	clicktocall_start_rsp_tcp_t clicktocall_rsp[1];
	clicktocall_start_req_tcp_t clicktocall_req[1];

	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_req ");
	clicktocall_start_req_tcp_init(clicktocall_req);
	clicktocall_start_rsp_tcp_init(clicktocall_rsp);

	// dbif = (uxc_dbif_t*)msg->data;
	// rv = clicktocall_start_req_decode_dbif_msg(clicktocall_req, NULL, NULL, dbif);
	// if (rv <eUXC_SUCCESS) return rv;

	// //TODO 5 : 받은 Notification 처리
	// // ux_log(UXL_CRT, "  [sessionID] %s", clicktocall_req->sessionID);
	// // ux_log(UXL_CRT, "  [gwSessionID] %s", clicktocall_req->gwSessionID);
	// ux_log(UXL_CRT, "  [subscriberName] %s", clicktocall_req->subscriberName);
	// ux_log(UXL_CRT, "  [recordingType] %d", clicktocall_req->recordingType);
	// ux_log(UXL_CRT, "  [callingNumber] %s", clicktocall_req->callingNumber);
	// ux_log(UXL_CRT, "  [calledNumber] %s", clicktocall_req->calledNumber);
	// ux_log(UXL_CRT, "  [serviceCode] %d", clicktocall_req->serviceCode);
	// ux_log(UXL_CRT, "  [ringBackToneType] %d", clicktocall_req->ringBackToneType);
	// ux_log(UXL_CRT, "  [waitingMentID] %d", clicktocall_req->waitingMentID);
	// ux_log(UXL_CRT, "  [scenarioType] %d", clicktocall_req->scenarioType);
	// ux_log(UXL_CRT, "  [callMentID] %d", clicktocall_req->callMentID);
	// ux_log(UXL_CRT, "  [callingCID] %s", clicktocall_req->callingCID);
	// ux_log(UXL_CRT, "  [calledCID] %s", clicktocall_req->calledCID);
	// ux_log(UXL_CRT, "  [recordingFileName] %s", clicktocall_req->recordingFileName);
	// ux_log(UXL_CRT, "  [isAllRecording] %d", clicktocall_req->isAllRecording);
	// ux_log(UXL_CRT, "  [endIfRecordingFailed] %d", clicktocall_req->endIfRecordingFailed);
	// ux_log(UXL_CRT, "  [endIfRecordingEnded] %d", clicktocall_req->endIfRecordingEnded);
	// ux_log(UXL_CRT, "  [hostingCode] %d", clicktocall_req->hostingCode);
	// ux_log(UXL_CRT, "  [wirelessTimeout] %d", clicktocall_req->wirelessTimeout);
	// ux_log(UXL_CRT, "  [wiredTimeout] %d", clicktocall_req->wi₩redTimeout);

	// clicktocall_rsp->result = 0;
	// clicktocall_rsp->callTime = 0;
	// clicktocall_rsp->finalUnitAction = 100;
	// clicktocall_rsp->brandId = 0;
	// clicktocall_rsp->lang= 0;

	// rv = clicktocall_start_rsp_encode_to_dbif_msg(clicktocall_rsp, msg);
	// if (rv <eUXC_SUCCESS) return rv;
	// rv = tcp_msg_send(msg, server->patcp, &tcpmsg->peerkey);	
	// if (rv <eUXC_SUCCESS) return rv;

	return 0;
}

static int _tcp_server_on_accept(upa_tcp_t *tcp, ux_channel_t *channel, ux_accptor_t *accptor,
				ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Accept TCP connection");
	return eUXC_SUCCESS;
}


static int _tcp_server_on_open(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Open TCP connection");
	return eUXC_SUCCESS;
}


static int _tcp_server_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Close TCP connection");
	return eUXC_SUCCESS;
}

