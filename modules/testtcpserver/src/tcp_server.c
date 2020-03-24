#include "tcp_server.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_server_t 
///////////////////////////////////////////////////////////////////////////////////

static tcp_server_t *_g_server = NULL;

static void _tcp_server_destroy( uxc_plugin_t *pi);
static int tcp_server_handle_clicktocall_start_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *msg );
static int tcp_server_handle_clicktocall_stop_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg );
static int tcp_server_handle_clicktocall_startrecording_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg );
static int tcp_server_handle_clicktocall_stoprecording_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg );
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
	clicktocall_start_req_tcp_t clicktocall_start_req[1];
	clicktocall_stop_req_tcp_t clicktocall_stop_req[1];
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req[1];
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req[1];

	// 1. receive msg 
	skbmsg = (skb_msg_t *)tcpmsg->netmsg->buffer;
	ux_log(UXL_INFO, "header size : %lu", sizeof(skbmsg->header));

	rv = skb_msg_cvt_order_ntoh(skbmsg, tcpmsg->peerkey.chnl_idx, &msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	msgID = skbmsg->header.messageID;

	// 2. display msg
	// rv = tcp_msg_display(msg);
	ux_log(UXL_INFO, "received : chn(%d) - msgID(%d)", tcpmsg->peerkey.chnl_idx, msgID);

	// 3. response to uxcutor
	switch(tcpmsg->peerkey.chnl_idx) {
		case TCP_CHANNEL_CALL:
			switch(msgID)
			{
				case START_REQUEST:
					memcpy(clicktocall_start_req, skbmsg->body, sizeof(clicktocall_start_req_tcp_t));
					return tcp_server_handle_clicktocall_start_req(server, worker, tcpmsg, skbmsg);
				case STOP_REQUEST:
					memcpy(clicktocall_stop_req, skbmsg->body, sizeof(clicktocall_stop_req_tcp_t));
					return tcp_server_handle_clicktocall_stop_req(server, worker, tcpmsg, skbmsg);
				case START_RECORDING_REQUEST:
					memcpy(clicktocall_startrecording_req, skbmsg->body, sizeof(clicktocall_startrecording_req_tcp_t));
					return tcp_server_handle_clicktocall_startrecording_req(server, worker, tcpmsg, skbmsg);
				case STOP_RECORDING_REQUEST:
					memcpy(clicktocall_stoprecording_req, skbmsg->body, sizeof(clicktocall_stoprecording_req_tcp_t));
					return tcp_server_handle_clicktocall_stoprecording_req(server, worker, tcpmsg, skbmsg);
				default:
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

static int tcp_server_handle_clicktocall_start_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg )
{
	int rv;
	clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	clicktocall_start_req_tcp_t clicktocall_start_req[1];
	skb_msg_t rspMsg;

	//request display
	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_start_req ");
	memcpy(clicktocall_start_req, skbmsg->body, sizeof(clicktocall_start_req_tcp_t));
	skb_msg_display_header(&skbmsg->header);
	clicktocall_start_req_tcp_display(clicktocall_start_req);

	//make response
	clicktocall_start_rsp_tcp_init(clicktocall_start_rsp);
	clicktocall_start_rsp->resultCode = 0;
	strcpy(clicktocall_start_rsp->serviceID, "service0001");
	clicktocall_start_rsp->isRecording = 0;
	clicktocall_start_rsp->filler1 = 0;
	clicktocall_start_rsp->filler2= 0;
	strcpy(clicktocall_start_rsp->recordingFileURL, "/test/test");
	strcpy(clicktocall_start_rsp->recordingFileName, "testFileName");

	skb_msg_make_header(&rspMsg.header, START_RESPONSE, sizeof(clicktocall_start_rsp), &skbmsg->header.requestID);
	skb_msg_display_header(&rspMsg.header);
	clicktocall_start_rsp_tcp_display(clicktocall_start_rsp);
	memcpy(rspMsg.body, clicktocall_start_rsp, sizeof(clicktocall_start_rsp_tcp_t));

	rv = skb_msg_send(&rspMsg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

	return 0;
}


static int tcp_server_handle_clicktocall_stop_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg )
{
	int rv;
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];
	clicktocall_stop_req_tcp_t clicktocall_stop_req[1];
	skb_msg_t rspMsg;

	//request display
	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_stop_req ");
	memcpy(clicktocall_stop_req, skbmsg->body, sizeof(clicktocall_stop_req_tcp_t));
	skb_msg_display_header(&skbmsg->header);
	clicktocall_stop_req_tcp_display(clicktocall_stop_req);

	//make response
	clicktocall_stop_rsp_tcp_init(clicktocall_stop_rsp);
	clicktocall_stop_rsp->resultCode = 0;
	strcpy(clicktocall_stop_rsp->serviceID, "service0001");

	skb_msg_make_header(&rspMsg.header, STOP_RESPONSE, sizeof(clicktocall_stop_rsp), &skbmsg->header.requestID);
	skb_msg_display_header(&rspMsg.header);
	clicktocall_stop_rsp_tcp_display(clicktocall_stop_rsp);
	memcpy(rspMsg.body, clicktocall_stop_rsp, sizeof(clicktocall_stop_rsp_tcp_t));

	rv = skb_msg_send(&rspMsg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

	return 0;
}

static int tcp_server_handle_clicktocall_startrecording_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg ) {
	int rv;
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req[1];
	skb_msg_t rspMsg;

	//request display
	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_startrecording_req ");
	memcpy(clicktocall_startrecording_req, skbmsg->body, sizeof(clicktocall_startrecording_req_tcp_t));
	skb_msg_display_header(&skbmsg->header);
	clicktocall_startrecording_req_tcp_display(clicktocall_startrecording_req);

	//make response
	clicktocall_startrecording_rsp_tcp_init(clicktocall_startrecording_rsp);
	clicktocall_startrecording_rsp->resultCode = 0;
	strcpy(clicktocall_startrecording_rsp->serviceID, "service0001");
	strcpy(clicktocall_startrecording_rsp->recordingFileURL, "/test/test");
	strcpy(clicktocall_startrecording_rsp->recordingFileName, "testFileName");

	skb_msg_make_header(&rspMsg.header, START_RECORDING_RESPONSE, sizeof(clicktocall_startrecording_rsp), &skbmsg->header.requestID);
	skb_msg_display_header(&rspMsg.header);
	clicktocall_startrecording_rsp_tcp_display(clicktocall_startrecording_rsp);
	memcpy(rspMsg.body, clicktocall_startrecording_rsp, sizeof(clicktocall_startrecording_rsp_tcp_t));

	rv = skb_msg_send(&rspMsg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

	return 0;
}

static int tcp_server_handle_clicktocall_stoprecording_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, skb_msg_t *skbmsg ) {
	int rv;
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req[1];
	skb_msg_t rspMsg;

	//request display
	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_clicktocall_stoprecording_req ");
	memcpy(clicktocall_stoprecording_req, skbmsg->body, sizeof(clicktocall_stoprecording_req_tcp_t));
	skb_msg_display_header(&skbmsg->header);
	clicktocall_stoprecording_req_tcp_display(clicktocall_stoprecording_req);

	//make response
	clicktocall_stoprecording_rsp_tcp_init(clicktocall_stoprecording_rsp);
	clicktocall_stoprecording_rsp->resultCode = 0;
	strcpy(clicktocall_stoprecording_rsp->serviceID, "service0001");

	skb_msg_make_header(&rspMsg.header, STOP_RECORDING_RESPONSE, sizeof(clicktocall_stoprecording_rsp), &skbmsg->header.requestID);
	skb_msg_display_header(&rspMsg.header);
	clicktocall_stoprecording_rsp_tcp_display(clicktocall_stoprecording_rsp);
	memcpy(rspMsg.body, clicktocall_stoprecording_rsp, sizeof(clicktocall_stoprecording_rsp_tcp_t));

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

