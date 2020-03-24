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
	clicktocall_start_req_tcp_t clicktocall_start_req[1];

	// 1. receive msg 
	skbmsg = (skb_msg_t *)tcpmsg->netmsg->buffer;
	ux_log(UXL_INFO, "header size : %lu", sizeof(skbmsg->header));

	memcpy(clicktocall_start_req, skbmsg->body, sizeof(clicktocall_start_req_tcp_t));
	
	rv = skb_msg_cvt_order_ntoh(skbmsg, tcpmsg->peerkey.chnl_idx, &msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	msgID = skbmsg->header.messageID;

	// 2. display msg
	// rv = tcp_msg_display(msg);

	// 3. response to uxcutor
	switch(tcpmsg->peerkey.chnl_idx) {
		case TCP_CHANNEL_CALL:
			switch(msgID)
			{
				case START_REQUEST:
					ux_log(UXL_INFO, "received : chn(%d) - msgID(%d)", tcpmsg->peerkey.chnl_idx, msgID);
					return tcp_server_handle_clicktocall_start_req(server, worker, tcpmsg, skbmsg);
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

	ux_log(UXL_INFO, "* tcp_server_handle_clicktocall_start_req ");
	// clicktocall_start_req_tcp_init(clicktocall_start_req);
	clicktocall_start_rsp_tcp_init(clicktocall_start_rsp);

	// rv = clicktocall_start_req_decode_dbif_msg(clicktocall_start_req, skbmsg);
	// if (rv <eUXC_SUCCESS) return rv;

	// clicktocall_start_req = skbmsg->body;
	memcpy(clicktocall_start_req, skbmsg->body, sizeof(clicktocall_start_req_tcp_t));

	//header
	skb_msg_display_header(&skbmsg->header);
	clicktocall_start_req_tcp_display(clicktocall_start_req);

	clicktocall_start_rsp->resultCode = 0;
	strcpy(clicktocall_start_rsp->serviceID, "service0001");
	clicktocall_start_rsp->isRecording = 0;
	clicktocall_start_rsp->filler1 = 0;
	clicktocall_start_rsp->filler2= 0;
	strcpy(clicktocall_start_rsp->recordingFileURL, "/test/test");
	strcpy(clicktocall_start_rsp->recordingFileName, "testFileName");

	skb_msg_make_header(&rspMsg.header, START_RESPONSE, sizeof(clicktocall_start_rsp), &skbmsg->header.requestID);
	// rspMsg.body = clicktocall_start_rsp;
	memcpy(rspMsg.body, clicktocall_start_rsp, sizeof(clicktocall_start_rsp_tcp_t));

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

