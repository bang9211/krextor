#include <ua_log.h>
#include <ua_socklib.h>
#include "ipmssim_tcpip.h"
#include "ipmssim_util.h"


#define _MAX_TCPIP_PEER 100

#define _PORT_LEN 3


static int _g_start_flag = 0;
static int _g_server_flag = -1;
static char _g_ipaddr[64];
static int _g_port[3] = {-1, -1, -1};
static char _g_peer_ip[_MAX_TCPIP_PEER][64];
static int _g_peer_sockfd[_MAX_TCPIP_PEER];
static time_t _g_peer_sndtime[_MAX_TCPIP_PEER];
static time_t _g_peer_rcvtime[_MAX_TCPIP_PEER];
static char _g_peer_tid[_MAX_TCPIP_PEER][IPMSSIM_TRCID_SIZE+8];

static int _ipmssim_accept_callback( int idx, char *ip, int sockfd );
static int _ipmssim_connect_callback( int idx, char *ip, int sockfd );
static int _ipmssim_close_callback( int idx, char *ip, int sockfd );

char *portName[_PORT_LEN] = {"CALL", "RECORDING", "CONFERENCE"};

//////////////////////////////////////////////////////////////////////////////
// public functions
//////////////////////////////////////////////////////////////////////////////

int ipmssim_tcpip_start( const char *ipaddr, unsigned short port[] )
{
	int rv, i;

	if( _g_start_flag ) {
		printf( "Already tcpip server start.\n" );
		return 0;
	}

	tcpip_initChnlInfo();
	tcpip_reset( sizeof(skb_header_t), TYPE_SHORT, 2, INCLUDE_HEADER );
	tcpip_changeSelSec( 0, 10 );
	tcpip_acceptCallback( _ipmssim_accept_callback );
	tcpip_connectCallback( _ipmssim_connect_callback );
	tcpip_cutSocketCallback( _ipmssim_close_callback );
	skb_msg_init(ipmssim_get_heartbeat_display());
	
	_g_server_flag = 1;
	strcpy(_g_ipaddr, ipaddr);
	for (i = 0; i < _PORT_LEN; i++) {
		tcpip_addChnl(portName[i]);
		rv = tcpip_start( MODE_SERVER, "0.0.0.0", "0.0.0.0", port[i], portName[i] );
		if( rv < 0 ) {
			printf( "tcpip_start() failed. err=%d, port=%d %s\n", rv, port[i], portName[i]);
			return -1;
		}
		_g_port[i] = port[i];
	}
	_g_start_flag = 1;
	
	printf( "tcpip_start() success.\n" );
	return 0;
}

int ipmssim_tcpip_stop()
{
	int rv, i;

	if( !_g_start_flag ) {
		printf( "Already tcpip server stop.\n" );
		return 0;
	}

	for( i = 0 ; i < _MAX_TCPIP_PEER ; i ++ ) {
		if( _g_peer_ip[i][0] && _g_peer_sockfd[i] >= 0 ) {
			tcpip_cutSocket( _g_peer_sockfd[i] );
		}
	}
	if( _g_server_flag ) { // server mode
		for (i = 0; i < _PORT_LEN; i++) {
			rv = tcpip_close( portName[i] );
			if( rv < 0 ) {
				printf( "tcpip_close() failed. err=%d\n", rv );
				return -1;
			}
		}
	}

	skb_msg_end();

	_g_start_flag = 0;
	_g_server_flag = -1;
	_g_ipaddr[0] = '\0';
	for (i = 0; i < _PORT_LEN; i++) {
		_g_port[i] = -1;
	}
	for( i = 0 ; i < _MAX_TCPIP_PEER ; i ++ ) {
		_g_peer_ip[i][0] = '\0';
		_g_peer_sockfd[i] = -1;
		_g_peer_sndtime[i] = 0;
		_g_peer_rcvtime[i] = 0;
	}

	printf( "tcpip_close() success.\n" );
	return 0;
}

void ipmssim_tcpip_display()
{
	int i, cnt;
	char sndtime[64], rcvtime[64];
	char fmt[] = { "%Y/%m/%d %H:%M:%S." };

	printf( "===== TCP/IP ================================\n");
	printf( "tcpip start    : %s\n", _g_start_flag ? "Yes" : "No" );
	if( _g_server_flag > -1 ) {
		printf( "tcpip mode     : %s\n", _g_server_flag ? "Server" : "Client" );
		printf( "tcpip ipaddr   : %s\n", _g_ipaddr );
		for (i = 0; i < _PORT_LEN; i++) {
			printf( " - port     : %d\n", _g_port[i] );
		}
	}
	if( _g_start_flag ) {
		printf( "tcpip peer list.\n" );
		for( cnt = i = 0 ; i < _MAX_TCPIP_PEER ; i ++ ) {
			if( _g_peer_ip[i][0] != '\0' ) {
				ipmssim_cvt_time_to_str( _g_peer_sndtime[i], 1000, fmt, sndtime, sizeof(sndtime) );
				ipmssim_cvt_time_to_str( _g_peer_rcvtime[i], 1000, fmt, rcvtime, sizeof(rcvtime) );
				printf( " - #%02d : sockfd[%d] ip[%s] tid[%s]\n"
						"         sndtime[%s] rcvtime[%s]\n",
						cnt+1, _g_peer_sockfd[i], _g_peer_ip[i], _g_peer_tid[i], sndtime, rcvtime );
				cnt += 1;
			}
		}
	}
	printf( "=============================================\n" );
}

int ipmssim_tcpip_read( skb_msg_t *msg )
{
	int rv, idx;
	int msgID;

	if( !msg ) return -1;
	if( !_g_start_flag ) return -1;

	idx = tcpip_receive();
	if( idx < 0 ) return idx;

	rv = tcpip_read( idx, (char*)msg );
	if( rv < 0 ) {
		printf( "tcpip_read() failed. index[%d] sockfd[%d] ipaddr[%s]",
				idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );
		return rv;
	}
	// uaLog( L_INFO, "Receive message from index[%d] sockfd[%d] ipaddr[%s]",
	// 		idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );

	rv = skb_msg_cvt_order_ntoh( msg, idx, &msgID );
	if( rv < 0 ) {
		printf( "skb_msg_cvt_order_ntoh() failed. index[%d] sockfd[%d] ipaddr[%s]",
				idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );
		return rv;
	}

	// skb_msg_display_recv_header(&msg->header);

	// ipmssim_msg_cvt_order_ntoh( msg );
	// ipmssim_msg_display( msg );

	// _g_peer_rcvtime[idx] = ipmssim_get_now_time_msec();
	// ipmssim_msg_dec_header_trcid( msg, _g_peer_tid[idx], sizeof(_g_peer_tid[idx]) );
	return idx;
}

int ipmssim_tcpip_write( int idx, skb_msg_t *msg )
{
	int rv, len;
	int dbif_msgID;
	clicktocall_binding_rsp_tcp_t *call_binding_rsp;
	clicktocallrecording_binding_rsp_tcp_t *recording_binding_rsp;
	clicktoconference_binding_rsp_tcp_t *conference_binding_rsp;

	if( idx < 0 || idx >= _MAX_TCPIP_PEER || !msg ) return -1;
	if( !_g_start_flag ) return -1;

	len = msg->header.length;

	dbif_msgID = skb_msg_cvt_to_dbif_msg_id(idx, msg->header.messageID);
	if (dbif_msgID != NONE_DBIF_MESSAGE) {
		skb_msg_cvt_order_hton(msg, dbif_msgID);
	} else {
		skb_msg_cvt_order_hton2(msg);
		if (msg->header.messageID == BINDING_RESPONSE) {
			switch (idx) {
				case TCP_CHANNEL_CALL:
					call_binding_rsp = (clicktocall_binding_rsp_tcp_t *) msg->body;
					call_binding_rsp->resultCode = htonl(call_binding_rsp->resultCode);
					break;
				case TCP_CHANNEL_RECORDING:
					recording_binding_rsp = (clicktocallrecording_binding_rsp_tcp_t *) msg->body;
					recording_binding_rsp->resultCode = htonl(recording_binding_rsp->resultCode);
					break;
				case TCP_CHANNEL_CONFERENCE:
					conference_binding_rsp = (clicktoconference_binding_rsp_tcp_t *) msg->body;
					conference_binding_rsp->resultCode = htonl(conference_binding_rsp->resultCode);
					break;
			}
		}
	}
	rv = tcpip_write( idx, (char*)msg, len - sizeof(skb_header_t) );	//** body의 크기만 들어가야함!!!
	if( rv < 0 ) {
		printf( "tcpip_write() failed. index[%d] sockfd[%d] ipaddr[%s]",
				idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );
		return rv;
	}
	// uaLog( L_INFO, "%d byte(s) write. index[%d] sockfd[%d] ipaddr[%s]",
	// 		len, idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );

	// ipmssim_msg_cvt_order_ntoh( msg ); // 혹占쏙옙 占쏜르댐옙 占쌕쏙옙 ordering 占쏙옙占쏙옙

	_g_peer_sndtime[idx] = ipmssim_get_now_time_msec();
	return 0;
}

int ipmssim_tcpip_write2( int idx, char *msg, int body_len )
{
	int rv;

	if( idx < 0 || idx >= _MAX_TCPIP_PEER || !msg ) return -1;
	if( !_g_start_flag ) return -1;

	rv = tcpip_write( idx, msg, body_len );	//** body의 크기만 들어가야함!!!
	if( rv < 0 ) {
		printf( "tcpip_write() failed. index[%d] sockfd[%d] ipaddr[%s]",
				idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );
		return rv;
	}
	// uaLog( L_INFO, "%d byte(s) write. index[%d] sockfd[%d] ipaddr[%s]",
	// 		len, idx, _g_peer_sockfd[idx], _g_peer_ip[idx] );

	// ipmssim_msg_cvt_order_ntoh( msg ); // 혹占쏙옙 占쏜르댐옙 占쌕쏙옙 ordering 占쏙옙占쏙옙

	_g_peer_sndtime[idx] = ipmssim_get_now_time_msec();
	return 0;
}


int ipmssim_tcpip_write_all( skb_msg_t *msg )
{
	int rv, i, err_flag = 0;

	if( !msg ) return -1;

	for( i = 0 ; i < _MAX_TCPIP_PEER ; i ++ ) {
		if( _g_peer_ip[i][0] && _g_peer_sockfd[i] >= 0 ) {
			rv = ipmssim_tcpip_write( i, msg );
			if( rv < 0 ) err_flag = 1;
		}
	}
	
	return err_flag ? -1 : 0;
}

void ipmssim_tcpip_keepalive( int interval, int (func)(int idx) )
{
	int i;
	time_t now = ipmssim_get_now_time_msec();

	if( interval <= 0 ) return ;
	if( !_g_start_flag ) return ;

	for( i = 0 ; i < _MAX_TCPIP_PEER ; i ++ ) {
		if( !_g_peer_ip[i][0] || _g_peer_sockfd[i] < 0 ) continue; 
		if( _g_server_flag ) { // server mode
			// interval占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占� 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
			if( _g_peer_rcvtime[i] && now >= _g_peer_rcvtime[i] + interval * 1000 ) {
				printf( "tcpip keepalive. %d seconds timeout cut socket.\n", interval );
				tcpip_cutSocket( _g_peer_sockfd[i] );
			}
		} else { // client mode
			// interval * 1.5占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占� 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
			// interval占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占� status inquery 占쌨쏙옙占쏙옙 占쏙옙占쏙옙
			if( _g_peer_rcvtime[i] && now >= _g_peer_rcvtime[i] + interval * 1500 ) {
				printf( "tcpip keepalive. %d.%03d seconds timeout cut socket.\n", 
						((interval * 1500) % 1000), ((interval * 1500) / 1000) );
				tcpip_cutSocket( _g_peer_sockfd[i] );
			} else if( now >= _g_peer_sndtime[i] + interval * 1000 ) {
				if( func ) func( i );
			}
		}
	}
}

int timer_idx[CHANNEL_SIZE] = {0, 1, 2};

void *t_function(void *chnl_idx)
{
	int rv, requestID;
	int timeout = ipmssim_get_timeout_time();		//7
	int interval = ipmssim_get_heartbeat_time();	//2
	int sent_time = 0;
	int cidx;
	skb_msg_t skbmsg[1];

	memcpy(&cidx, chnl_idx, sizeof(int));
	while(timer_switch[cidx] > 0) {
		sleep(1);
		seconds[cidx]++;
		if (is_heartbeat_sent[cidx] == 0) {
			if (seconds[cidx] - last_recv_seconds[cidx] >= interval) {
				// uaLog( L_INFO, "send heartbeat(chnl_idx : %d)", cidx);
				skb_msg_make_header(&skbmsg->header, HEARTBEAT_REQUEST, 0, NULL);
				// skb_msg_display_send_header(&skbmsg->header);
				skb_msg_display_send_heartbeat_req(&skbmsg->header, cidx);
				strcpy(skbmsg[0].body,  "");

				requestID = skbmsg->header.requestID;

				//메시지를 Network byte ordering으로 변경
				// rv = skb_msg_cvt_order_hton2(skbmsg);
				// if( rv< UX_SUCCESS) {
				// 	uaLog(L_INFO, "failed to skb_msg_cvt_order_hton2 : %d", requestID);
				// 	return NULL;
				// }

				// uaLog(L_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
				// uaLog(L_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
				rv = ipmssim_tcpip_write( cidx, skbmsg );
				if( rv < UX_SUCCESS) {
					uaLog( L_CRT, "failed to upa_tcp_send2 : %d", requestID);
					//세션 연결 종료
					tcpip_cutSocket( _g_peer_sockfd[tcpip_get_fd(cidx)] );
					return NULL;
				}
				is_heartbeat_sent[cidx] = 1;
				sent_time = seconds[cidx];
			}
		} else if (seconds[cidx] - sent_time >= timeout) {	//TIMEOUT
			ux_log( UXL_CRT, "heartbeat timeout : %d", cidx);
			//세션 연결 종료
			tcpip_cutSocket( _g_peer_sockfd[tcpip_get_fd(cidx)] );
			return NULL;
		}
	}

	return NULL;
}

void turn_heartbeat_timer_on(int chnl_idx) {
	if ((ipmssim_get_heartbeat_timer() == 1) && (timer_switch[chnl_idx] == 0)) {
		timer_switch[chnl_idx] = 1;
		uaLog( L_INFO, "heartbeat(chnl_idx : %d) timer on", chnl_idx);
		if (chnl_idx < 0 || chnl_idx >= CHANNEL_SIZE) {
			uaLog(L_CRT, "failed to tcp_client_open, unsupported chnl_idx : %d", chnl_idx);
			return;
		}
		pthread_create(&timer_thread[chnl_idx], NULL, t_function, &timer_idx[chnl_idx]);
	}
}

void turn_heartbeat_timer_off(int chnl_idx) {
	if (timer_switch[chnl_idx] > 0) {
		uaLog( L_INFO, "heartbeat(chnl_idx : %d) timer off", chnl_idx);
		timer_switch[chnl_idx] = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////
// private functions
//////////////////////////////////////////////////////////////////////////////

static int _ipmssim_accept_callback( int idx, char *ip, int sockfd )
{
	printf( "tcpip connect %s. index[%d] sockfd[%d] ipaddr[%s]\n", 
			_g_server_flag ? "client" : "server", idx, sockfd, ip );

	if( idx < 0 || idx >= _MAX_TCPIP_PEER ) {
		printf( "tcpip channel info buffer full....\n" );
		return -1;
	}

	timer_switch[idx] = 0;
	is_heartbeat_sent[idx] = 0;
	seconds[idx] = 0;
	last_recv_seconds[idx] = 0;

	strcpy( _g_peer_ip[idx], ip );
	_g_peer_sockfd[idx] = sockfd;
	return 0;
}

static int _ipmssim_connect_callback( int idx, char *ip, int sockfd )
{
	return _ipmssim_accept_callback( idx, ip, sockfd );
}

static int _ipmssim_close_callback(int idx, char *ip, int sockfd)
{
	printf( "tcpip close %s. index[%d] sockfd[%d] ipaddr[%s]\n", 
			_g_server_flag ? "client" : "server", idx, sockfd, ip );

	turn_heartbeat_timer_off(idx);

	if( idx < 0 || idx >= _MAX_TCPIP_PEER ) {
		printf("invalid tcpip channel index....\n");
		return -1;
	}

	_g_peer_ip[idx][0] = '\0';
	_g_peer_sockfd[idx] = -1;
	_g_peer_sndtime[idx] = 0;
	_g_peer_rcvtime[idx] = 0;
	_g_peer_tid[idx][0] = '\0';
	return 0;
}
