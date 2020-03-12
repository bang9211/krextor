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

	return (uxc_plugin_t*)client;
}

static void _tcp_client_destroy( uxc_plugin_t *plugin)
{
	tcp_client_t *client;
	client = (tcp_client_t*) plugin;
	uxc_plugin_final( client->base);
	tcp_client_final( client);
	ux_free( ux_mem_default(), client);
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

int tcp_client_handle_dbifreq( tcp_client_t *client, uxc_worker_t *worker, uxc_ipcmsg_t *ipcmsg )
{
	int rv, msg_size, msgId;
	upa_peerkey_t peerkey;
	tcp_msg_t *msg;

	msg = (tcp_msg_t *)&ipcmsg->header;
	msg_size = sizeof(uxc_ixpc_t) + msg->header.length;
	msgId = msg->header.msgId;

	rv = tcp_msg_cvt_order_hton(msg);
	if( rv< UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	ux_log( UXL_INFO, "2. CALL tcp_client_handle_dbifreq (len:%d, msgId:%d) ", msg_size, msgId);

	if( msgId / 100 == 1) {
		ux_log(UXL_INFO, "2.1. Set Channel clicktocall");
		peerkey.chnl_idx = 0; // configuration 첫번째 채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 
	} else if( msgId / 100 == 2) {
		ux_log(UXL_INFO, "2.2. Set Channel clicktocallrecording");
		peerkey.chnl_idx = 1; //  채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 
	} else if( msgId / 100 == 3) {
		ux_log(UXL_INFO, "2.3. Set Channel clicktoconference");
		peerkey.chnl_idx = 2; //  채널
		peerkey.peer_key = 0; // 채널의 첫번째 PEER 
	}

	rv = upa_tcp_send2(_g_client->patcp, &peerkey, msg, msg_size, 1);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data.");
		return -1;
	} else {
		ux_log( UXL_INFO, "3. Relay dbif msg. from uxcutor to tcp server (len:%d)", msg_size);
		return UX_SUCCESS;
	}
}

int tcp_client_handle_dbifrsp( tcp_client_t *client, uxc_worker_t *worker, upa_tcpmsg_t *tcpmsg)
{
	int rv;
	tcp_msg_t *msg;

	msg = (tcp_msg_t *) tcpmsg->netmsg->buffer;

	ux_log( UXL_INFO, "5. CALL tcp_client_handle_dbifrsp =");

	rv = tcp_msg_cvt_order_ntoh(msg);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}	

	rv = tcp_client_send_ipcmsg(client, msg, 0); 
	if( rv< UX_SUCCESS) {
		ux_log( UXL_INFO, "can't send msg");
	}

	return rv;
}	

int tcp_client_send_ipcmsg( tcp_client_t *client,  tcp_msg_t* msg, int rv)
{
	int msg_size;
	uxc_ipcmsg_t ipcmsg;

	ux_log(UXL_CRT, "	[length] %d ",msg->header.length);
	ux_log(UXL_CRT, "	[msgId] %d ", msg->header.msgId);

	ipcmsg.header = msg->header;
	memcpy(ipcmsg.data, msg->data, ipcmsg.header.length);
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg.header.length + sizeof(long) ;

	ipcmsg.mtype = UXC_MTYPE_DB;
	ipcmsg.header.dstQid = ipcmsg.header.srcQid;
	ipcmsg.header.srcQid = client->conf->mqid;
	ipcmsg.header.result = rv;
	
	ux_log(UXL_INFO, "6. Send ipcmsg to %d from %d, size=%d, header=%d + dbif=%d\n",ipcmsg.header.dstQid,
					client->conf->mqid, msg_size, sizeof(uxc_ixpc_t),ipcmsg.header.length); 

	rv = msgsnd(ipcmsg.header.dstQid, &ipcmsg, msg_size, IPC_NOWAIT);
	if( rv < 0) {
		ux_log( UXL_MAJ, "msgsnd: E=%d, q=%d, len=%d", errno, ipcmsg.header.dstQid,
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

