#include "tcp_server.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_server_t 
///////////////////////////////////////////////////////////////////////////////////

static tcp_server_t *_g_server = NULL;

static void _tcp_server_destroy( uxc_plugin_t *pi);
static int tcp_server_handle_clicktocall_start_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *msg );
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

int tcp_server_handle_svrreq( tcp_server_t *server, uxc_worker_t* worker, upa_tcpmsg_t *tcpmsg)
{
	skb_msg_t *skbmsg;
	int msgID,rv;
	tcp_clicktocall_start_req_t *tcp_clicktocall_start_req;

	// 1. receive msg 
	// msg = (tcp_msg_t *)tcpmsg->netmsg->buffer;
	skbmsg = (skb_msg_t *)tcpmsg->netmsg->buffer;
	tcp_clicktocall_start_req = (tcp_clicktocall_start_req_t *)skbmsg->body;
	
	rv = skb_msg_cvt_order_ntoh(skbmsg, CALL_START_REQUEST);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	msgID = skbmsg->header.messageID;
	ux_log(UXL_INFO, "received : %d", msgID);
	ux_log(UXL_INFO, "received : %s", tcp_clicktocall_start_req->subscriberName);

	// 2. display msg
	// rv = tcp_msg_display(msg);

	// 3. response to uxcutor
	switch(msgID)
	{
		case START_REQUEST:
			return tcp_server_handle_clicktocall_start_req(server, worker, tcpmsg, skbmsg);
		default:
			break;	
	}			
	return -1;
}

static int tcp_server_handle_clicktocall_start_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *msg )
{
	int rv;
	tcp_clicktocall_start_rsp_t clicktocall_start_rsp[1];
	tcp_clicktocall_start_req_t *clicktocall_start_req;
	skb_msg_t rspMsg;

	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_start_req ");
	// tcp_clicktocall_start_req_init(clicktocall_start_req);
	tcp_clicktocall_start_rsp_init(clicktocall_start_rsp);

	// rv = tcp_clicktocall_start_req_decode_msg(clicktocall_start_req, msg);
	// if (rv <eUXC_SUCCESS) return rv;

	clicktocall_start_req = msg->body;

	/* To do.. */	
	ux_log(UXL_CRT, "  [subscriberName] %s", clicktocall_start_req->subscriberName);
	ux_log(UXL_CRT, "  [recordingType] %d", clicktocall_start_req->recordingType);
	ux_log(UXL_CRT, "  [callingNumber] %s", clicktocall_start_req->callingNumber);
	ux_log(UXL_CRT, "  [calledNumber] %s", clicktocall_start_req->calledNumber);
	ux_log(UXL_CRT, "  [chargningNumber] %s", clicktocall_start_req->chargingNumber);
	ux_log(UXL_CRT, "  [serviceCode] %d", clicktocall_start_req->serviceCode);
	ux_log(UXL_CRT, "  [ringBackToneType] %d", clicktocall_start_req->ringBackToneType);
	ux_log(UXL_CRT, "  [waitingMentID] %d", clicktocall_start_req->waitingMentID);
	ux_log(UXL_CRT, "  [fillerInt8] %d", clicktocall_start_req->fillerInt8);
	ux_log(UXL_CRT, "  [scenarioType] %d", clicktocall_start_req->scenarioType);
	ux_log(UXL_CRT, "  [callMentID] %d", clicktocall_start_req->callMentID);
	ux_log(UXL_CRT, "  [callingCID] %s", clicktocall_start_req->callingCID);
	ux_log(UXL_CRT, "  [calledCID] %s", clicktocall_start_req->calledCID);
	ux_log(UXL_CRT, "  [recordingFileName] %s", clicktocall_start_req->recordingFileName);
	ux_log(UXL_CRT, "  [isAllRecording] %d", clicktocall_start_req->isAllRecording);
	ux_log(UXL_CRT, "  [endIfRecordingFailed] %d", clicktocall_start_req->endIfRecordingFailed);
	ux_log(UXL_CRT, "  [endIfRecordingEnded] %d", clicktocall_start_req->endIfRecordingEnded);
	ux_log(UXL_CRT, "  [hostingCode] %d", clicktocall_start_req->hostingCode);
	ux_log(UXL_CRT, "  [wirelessTimeout] %d", clicktocall_start_req->wirelessTimeout);
	ux_log(UXL_CRT, "  [wiredTimeout] %d", clicktocall_start_req->wiredTimeout);
	ux_log(UXL_CRT, "  [fillerInt16] %d", clicktocall_start_req->fillerInt16);
	ux_log(UXL_CRT, "  [filler] %s", clicktocall_start_req->filler);

	clicktocall_start_rsp->resultCode = 0;
	strcpy(clicktocall_start_rsp->serviceID, "service0001");
	clicktocall_start_rsp->isRecording = 0;
	clicktocall_start_rsp->filler1 = 0;
	clicktocall_start_rsp->filler2= 0;
	strcpy(clicktocall_start_rsp->recordingFileURL, "/test/test");
	strcpy(clicktocall_start_rsp->recordingFileName, "testFileName");

	rspMsg.header = *skb_msg_make_header(START_RESPONSE, sizeof(clicktocall_start_rsp), msg->header.requestID);
	rspMsg.body = clicktocall_start_rsp;

	rv = skb_msg_send(&rspMsg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

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

