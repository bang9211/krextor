#include "tcp_server.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_server_t 
///////////////////////////////////////////////////////////////////////////////////

static tcp_server_t *_g_server = NULL;

static void _tcp_server_destroy( uxc_plugin_t *pi);
static int tcp_server_handle_idpreq( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg );
static int tcp_server_handle_clicktocall_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg );
static int tcp_server_handle_acreq( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg );
static int tcp_server_handle_cirreq( tcp_server_t *server, uxc_worker_t *worker,
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


int tcp_server_handle_svrreq( tcp_server_t *server, uxc_worker_t* worker, upa_tcpmsg_t *tcpmsg)
{
	tcp_msg_t *msg;
	int msgID,rv;

	// 1. receive msg 
	msg = (tcp_msg_t *)tcpmsg->netmsg->buffer;
	
	rv = tcp_msg_cvt_order_ntoh(msg);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	msgID = msg->header.msgId;

	// 2. display msg
	rv = tcp_msg_display(msg);

	// 3. response to uxcutor
	switch(msgID)
	{
		case TCP_MSGID_CLICK_TO_CALL:
			return tcp_server_handle_clicktocall_req(server, worker, tcpmsg, msg);
		case TCP_MSGID_IDP :
			return tcp_server_handle_idpreq(server, worker, tcpmsg, msg);
		case TCP_MSGID_AC:
			return tcp_server_handle_acreq( server, worker, tcpmsg, msg);
		case TCP_MSGID_CIR:
			return tcp_server_handle_cirreq(server, worker, tcpmsg, msg);
		default:
			break;	
	}			
	return -1;
}

static int tcp_server_handle_clicktocall_req( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg )
{
	int rv;
	tcp_clicktocall_start_rsp_t clicktocall_rsp[1];
	tcp_clicktocall_start_req_t clicktocall_req[1];

	ux_log( UXL_INFO, "* tcp_server_handle_clicktocall_req ");
	tcp_clicktocall_req_init(clicktocall_req);
	tcp_clicktocall_rsp_init(clicktocall_rsp);

	rv = tcp_clicktocall_start_req_decode_msg(clicktocall_req, msg);
	if (rv <eUXC_SUCCESS) return rv;

	/* To do.. */	
	ux_log(UXL_CRT, "  [sessionID] %d", clicktocall_req->sessionID);
	ux_log(UXL_CRT, "  [gwSessionID] %s", clicktocall_req->gwSessionID);
	ux_log(UXL_CRT, "  [subscriberName] %s", clicktocall_req->subscriberName);
	ux_log(UXL_CRT, "  [recordingType] %s", clicktocall_req->recordingType);
	ux_log(UXL_CRT, "  [callingNumber] %s", clicktocall_req->callingNumber);
	ux_log(UXL_CRT, "  [calledNumber] %s", clicktocall_req->calledNumber);
	ux_log(UXL_CRT, "  [serviceCode] %s", clicktocall_req->serviceCode);
	ux_log(UXL_CRT, "  [ringBackToneType] %s", clicktocall_req->ringBackToneType);
	ux_log(UXL_CRT, "  [waitingMentID] %s", clicktocall_req->waitingMentID);
	ux_log(UXL_CRT, "  [scenarioType] %s", clicktocall_req->scenarioType);
	ux_log(UXL_CRT, "  [callMentID] %s", clicktocall_req->callMentID);
	ux_log(UXL_CRT, "  [callingCID] %s", clicktocall_req->callingCID);
	ux_log(UXL_CRT, "  [calledCID] %s", clicktocall_req->subscriberNcalledCIDame);
	ux_log(UXL_CRT, "  [recordingFileName] %s", clicktocall_req->recordingFileName);
	ux_log(UXL_CRT, "  [isAllRecording] %s", clicktocall_req->isAllRecording);
	ux_log(UXL_CRT, "  [endIfRecordingFailed] %s", clicktocall_req->endIfRecordingFailed);
	ux_log(UXL_CRT, "  [endIfRecordingEnded] %s", clicktocall_req->endIfRecordingEnded);
	ux_log(UXL_CRT, "  [hostingCode] %s", clicktocall_req->hostingCode);
	ux_log(UXL_CRT, "  [wirelessTimeout] %s", clicktocall_req->wirelessTimeout);
	ux_log(UXL_CRT, "  [wiredTimeout] %s", clicktocall_req->wiredTimeout);

	clicktocall_rsp->result = 0;
	clicktocall_rsp->callTime = 0;
	clicktocall_rsp->finalUnitAction = 100;
	clicktocall_rsp->brandId = 0;
	clicktocall_rsp->lang= 0;

	rv = tcp_clicktocall_rsp_encode_ipcmsg(clicktocall_rsp, msg);
	if (rv <eUXC_SUCCESS) return rv;
	rv = tcp_msg_send(msg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

	return 0;
}

static int tcp_server_handle_idpreq( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg )
{
	int rv;
	tcp_idprsp_t idprsp[1];
	tcp_idpreq_t idpreq[1];

	ux_log( UXL_INFO, "* tcp_server_handle_idpreq ");
	tcp_idpreq_init(idpreq);
	tcp_idprsp_init(idprsp);

	rv = tcp_idpreq_decode_msg(idpreq, msg);
	if (rv <eUXC_SUCCESS) return rv;

	/* To do.. */	
	ux_log(UXL_CRT, "  [ccrCount] %d", idpreq->ccrCount);
	ux_log(UXL_CRT, "  [called] %s", idpreq->called);
	ux_log(UXL_CRT, "  [dialed] %s", idpreq->dialed);

	idprsp->result = 0;
	idprsp->callTime = 0;
	idprsp->finalUnitAction = idpreq->ccrCount * 100;
	idprsp->brandId = 0;
	idprsp->lang= 0;

	rv = tcp_idprsp_encode_ipcmsg(idprsp, msg);
	if (rv <eUXC_SUCCESS) return rv;
	rv = tcp_msg_send(msg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

	return 0;
}

static int tcp_server_handle_acreq( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg )
{
	int rv;
	tcp_acrsp_t acrsp[1];
	tcp_acreq_t acreq[1];
	
	ux_log( UXL_INFO, "* tcp_server_handle_acreq");
	tcp_acreq_init(acreq);
	tcp_acrsp_init(acrsp);

	rv = tcp_acreq_decode_msg(acreq, msg);
	if (rv <eUXC_SUCCESS) return rv;

	/* To do .. */
	ux_log(UXL_CRT, "  [ccrCount] %d", acreq->ccrCount);
	ux_log(UXL_CRT, "  [called] %s", acreq->called);
	ux_log(UXL_CRT, "  [dialed] %s", acreq->dialed);

	acrsp->result = 0;
	acrsp->callTime = 0;
	acrsp->finalUnitAction = acreq->ccrCount * 100;
	acrsp->lang = 0;

	rv = tcp_acrsp_encode_ipcmsg(acrsp, msg);
	if (rv <eUXC_SUCCESS) return rv;
	rv = tcp_msg_send(msg, server->patcp, &tcpmsg->peerkey);	
	if (rv <eUXC_SUCCESS) return rv;

	return 0;
}

static int tcp_server_handle_cirreq( tcp_server_t *server, uxc_worker_t *worker,
					upa_tcpmsg_t *tcpmsg, tcp_msg_t *msg )
{
	int rv;
	tcp_cirrsp_t cirrsp[1];
	tcp_cirreq_t cirreq[1];

	ux_log( UXL_INFO, "* tcp_server_handle_cirreq",
			tcpmsg->peerkey);
	tcp_cirreq_init(cirreq);
	tcp_cirrsp_init(cirrsp);

	rv = tcp_cirreq_decode_msg(cirreq, msg);
	if (rv <eUXC_SUCCESS) return rv;

	/* To do .. */
	ux_log(UXL_CRT, "  [ccrCount] %d", cirreq->ccrCount);
	ux_log(UXL_CRT, "  [called] %s", cirreq->called);
	ux_log(UXL_CRT, "  [dialed] %s", cirreq->dialed);
	ux_log(UXL_CRT, "  [callTime] %d", cirreq->callTime);

	cirrsp->result = 0;
	cirrsp->callTime = 0;
	cirrsp->finalUnitAction = cirreq->ccrCount * 100;
	cirrsp->brandId = 0;
	cirrsp->lang = 0;

	rv = tcp_cirrsp_encode_ipcmsg(cirrsp, msg);
	if (rv <eUXC_SUCCESS) return rv;
	rv = tcp_msg_send(msg, server->patcp, &tcpmsg->peerkey);	
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

