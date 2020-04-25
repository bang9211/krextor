#include "tcp_client.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_client_t 
///////////////////////////////////////////////////////////////////////////////////

int request_timeout;
pthread_t timeout_thread;
int timeout_switch = 0;

/** @brief SKB Request ID */
int binding_reqIDs[CHANNEL_SIZE] = {0};
int seconds[CHANNEL_SIZE] = {0};
int last_recv_seconds[CHANNEL_SIZE] = {0};
static tcp_client_t *_g_client = NULL;
pthread_t timer_thread[CHANNEL_SIZE];
int timer_idx[CHANNEL_SIZE] = {0, 1, 2};
ux_channel_t* channel_arr[CHANNEL_SIZE];
upa_peerkey_t peerkey_arr[CHANNEL_SIZE];
int timer_switch[CHANNEL_SIZE] = {0};
int is_heartbeat_sent[CHANNEL_SIZE];

void turn_heartbeat_timer_on(int chnl_idx);
void turn_heartbeat_timer_off(int chnl_idx);

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

	skb_msg_init(client->conf->heartbeat_display);
	request_timeout = client->conf->request_timeout;
	turn_timeout_timer_on();

	return (uxc_plugin_t*)client;
}

static void _tcp_client_destroy( uxc_plugin_t *plugin)
{
	tcp_client_t *client;
	client = (tcp_client_t*) plugin;
	uxc_plugin_final( client->base);
	tcp_client_final( client);
	ux_free( ux_mem_default(), client);

	turn_timeout_timer_off();
	skb_msg_end();
}

tcp_client_t* tcp_client_instance()
{
	return _g_client; 
}

int tcp_client_init( tcp_client_t *client, uxc_xcutor_t *xcutor, const char* cfile)
{
	int rv;

	client->xcutor = xcutor;

	rv = smscib_conf_init( client->conf, client->xcutor, cfile);
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
	int msg_size, msgId, serialNumber;
	upa_peerkey_t peerkey;

	skb_msg_t skbmsg;
	uxc_dbif_t *dbif;
	uxc_ixpc_t ipc_header;

	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];
	time_t rawtime;

	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length;	//dbif header + body
	msgId = ipcmsg->header.msgId;

	// ux_log( UXL_INFO, "2. CALL tcp_client_forward_gwreq (len:%d, msgId:%d) ", msg_size, msgId);
	ux_log( UXL_INFO, " - CALL tcp_client_forward_gwreq (len:%d, msgId:%d) ", msg_size, msgId);

	// IPC에서 DBIF 추출
	dbif = uxc_ipcmsg_get_dbif(ipcmsg);

	//수신한 DBIF 메시지를 msgID에 따라 TCP로 보낼 SMSC의 메시지로 변경
	peerkey.chnl_idx = 0; // configuration 첫번째 채널
	peerkey.peer_key = 0; // 채널의 첫번째 PEER 

	switch(msgId) {
		case DBIF_GW_DELIVER:
			rv = skb_msg_process_deliver_msg(&skbmsg, dbif, sessionID, gwSessionID, &serialNumber);
			//serialNumber와 sessionID Bind
			if(!uh_int_put(sn_SID_Map, serialNumber, sessionID)) {
				ux_log(UXL_CRT, "failed to put to sn_SID_Map : (%d - %s)", serialNumber, sessionID);
			}
			//serialNumber와 gwSessionID Bind
			if(!uh_int_put(sn_GWSID_Map, serialNumber, gwSessionID)) {
				ux_log(UXL_CRT, "failed to put to sn_GWSID_Map : (%d - %s)", serialNumber, gwSessionID);
			}
			//serialNumber와 ipc header Bind
			memcpy(&ipc_header, &ipcmsg->header, sizeof(uxc_ixpc_t));
			if(!uh_ipc_put(sn_IPC_Map, serialNumber, &ipc_header)) {
				ux_log(UXL_CRT, "failed to put to sn_IPC_Map : (%d)", serialNumber);
			}
			//serialNumber와 curreunt time(ms) Bind
			time ( &rawtime );
			if(!uh_tmt_put(sn_timestamp_Map, serialNumber, rawtime)) {
				ux_log(UXL_CRT, "failed to put to sn_timestamp_Map : (%d)", serialNumber);
			}
			break;
		case DBIF_GW_REPORT_ACK:
			rv = skb_msg_process_report_ack_msg(&skbmsg, dbif);
			break;
		default:
			ux_log(UXL_CRT, "unsupported msgId : %d", msgId);
			return -1;
	}

	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data in process req.");
		return rv;
	}

	msg_size = skbmsg.header.length + sizeof(skb_header_t);
	// ux_log(UXL_CRT, "sending tcp header size : %lu", sizeof(skbmsg.header));
	// ux_log(UXL_CRT, "sending tcp body size : %lu", msg_size - sizeof(skbmsg.header));
	
	//메시지를 Network byte ordering으로 변경
	rv = skb_msg_cvt_order_hton(&skbmsg, msgId);
	if( rv< UX_SUCCESS) {
		ux_log(UXL_INFO, "failed to skb_msg_cvt_order_hton : %d", msgId);
		return rv;
	}

	//TCP Send
	rv = upa_tcp_send2(_g_client->patcp, &peerkey, &skbmsg, msg_size, 1);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", msgId);
		return -1;
	}
	// ux_log( UXL_INFO, "3. Forwarded DBIF to TCP msg. from gw to SMSC, size=%d, header=%lu + body=%lu", msg_size, sizeof(skb_header_t), msg_size - sizeof(skb_header_t));
	ux_log( UXL_INFO, " - Forwarded DBIF to TCP msg. from gw to SMSC, size=%d, header=%lu + body=%lu", msg_size, sizeof(skb_header_t), msg_size - sizeof(skb_header_t));
	return UX_SUCCESS;
}

int dbif_forward_smscrsp( tcp_client_t *client, uxc_worker_t *worker, upa_tcpmsg_t *tcpmsg)
{
	int rv, msgID, mqkey, dstQid = -1;
	// skb_msg_t *skbmsg;
	skb_msg_t skbmsg[1];
	uxc_dbif_t dbif;
	uxc_ixpc_t *dbif_header;
	uxc_ipcmsg_t ipcmsg;
	int msg_size, serialNumber, status;
	deliver_ack_msg_tcp_t deliver_ack_msg[1];
	char headerLog[SKB_HEADER_DISPLAY_SIZE];

	// 1. receive skbmsg 
	// skbmsg = (skb_msg_t *) tcpmsg->netmsg->buffer;
	memcpy(skbmsg, tcpmsg->netmsg->buffer, sizeof(skb_msg_t));

	//endian 복구
	rv = skb_msg_cvt_order_ntoh(skbmsg, tcpmsg->peerkey.chnl_idx, &msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "failed to skb_msg_cvt_order_ntoh");
		return rv;
	}

	//header 검증하여 정상적인 수신인지 확인
	if (check_header(&skbmsg->header) < 0) {
		skb_msg_get_header_display(&skbmsg->header, headerLog);
		ux_log(UXL_CRT, "failed to check header\n%s", headerLog);

		//세션 연결 종료
		ux_channel_stop2(channel_arr[tcpmsg->peerkey.chnl_idx], UX_TRUE);
		return -1;
	}

	// 2. process and response to uxcutor
	switch(tcpmsg->peerkey.chnl_idx) {
		case TCP_CHANNEL_SMS_CLI:
			switch(skbmsg->header.msgType) {
				case ALIVE_ACK:
					skb_msg_display_recv_alive_ack_msg(&skbmsg->header);
					is_heartbeat_sent[TCP_CHANNEL_SMS_CLI] = 0;
					last_recv_seconds[TCP_CHANNEL_SMS_CLI] = seconds[TCP_CHANNEL_SMS_CLI];		
					return UX_SUCCESS;
				case BIND_ACK:
					rv = skb_msg_process_bind_ack_msg(skbmsg);
					if( rv < UX_SUCCESS) {
						ux_log( UXL_CRT, "channel TCP_CHANNEL_SMS_CLI will be stopped");
						//세션 연결 종료
						ux_channel_stop2(channel_arr[TCP_CHANNEL_SMS_CLI], UX_TRUE);
					}
					turn_heartbeat_timer_on(TCP_CHANNEL_SMS_CLI);
					break;
				case DELIVER_ACK:
					//수신한 메시지가 DELIVER_ACK인 경우에만 serialNumber와 일치하는 serialNumber가 있는지 확인
					memcpy(deliver_ack_msg, skbmsg->body, sizeof(deliver_ack_msg_tcp_t));
					if (uh_tmt_get(sn_timestamp_Map, deliver_ack_msg->serialNumber) == -1) {	//not found
						ux_log(UXL_CRT, "failed to get sn_timestamp_Map : %d", deliver_ack_msg->serialNumber);
						return -1;
					}
					uh_tmt_del(sn_timestamp_Map, deliver_ack_msg->serialNumber);	//확인되면, timeout 명단에서 제거
					rv = skb_msg_process_deliver_ack_msg(skbmsg, &dbif, &serialNumber);
					break;
				case REPORT:
					rv = skb_msg_process_report_msg(skbmsg, &dbif, &serialNumber);
					break;
				default:
					ux_log(UXL_CRT, "unsupported msgType : %#010x", skbmsg->header.msgType)
					return -1;
			}
			break;
		default:
			ux_log(UXL_CRT, "unsupported Channel Index : %d", tcpmsg->peerkey.chnl_idx)
			return -1;
	}
	last_recv_seconds[tcpmsg->peerkey.chnl_idx] = seconds[tcpmsg->peerkey.chnl_idx] + 1;

	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data in process rsp.");
		return rv;
	}

	if (msgID != NONE_DBIF_MESSAGE) {
		dbif_header = uh_ipc_get(sn_IPC_Map, serialNumber);
		if (dbif_header == NULL) {
			ux_log(UXL_CRT, "there is no ipc_header of reqID(%d)", serialNumber);
			mqkey = uxc_get_conf_process_mqkey( _g_client->conf->dbif_gw_process_name, &rv);
			if( rv < eUXC_SUCCESS ) {
				printf("Don't exist gw block information for '%s' in proc.conf\n", _g_client->conf->dbif_gw_process_name);
				return -1;
			}
			dstQid = msgget(mqkey, 0666 | IPC_CREAT);
		}

		if (dstQid == -1) {
			ipcmsg.header = *dbif_header;
			ipcmsg.header.msgId = msgID;
			ipcmsg.header.length = sizeof(uxc_dbif_t) - UXC_DBIF_MAX_DATA + dbif.dataLen;
			memcpy(ipcmsg.data, &dbif, sizeof(dbif));
		} else {
			ipcmsg.mtype = UXC_MTYPE_SLEE;
			ipcmsg.header.srcProcId = getpid();
			ipcmsg.header.srcSubSysId = 0;
			ipcmsg.header.dstProcId = 0;
			ipcmsg.header.dstSubSysId = 0;
			ipcmsg.header.msgId = 0;
			ipcmsg.header.cmdId = (int)random();
			ipcmsg.header.userData = 0;
			ipcmsg.header.srcQid = dstQid;
			// ipcmsg.header.dstQid = dstQid;
			ipcmsg.header.result = 0;
			ipcmsg.header.fdIdx = 0;
		}

		rv = tcp_client_send_ipcmsg(client, &ipcmsg, 0);
		if( rv< UX_SUCCESS) {
			ux_log( UXL_INFO, "failed to tcp_client_send_ipcmsg : %d", msgID);
		} else {
			//성공 시 후처리 수행
			//report인 경우, reqID 제거
			if (skbmsg->header.msgType == REPORT) {
				uh_ipc_del(sn_IPC_Map, serialNumber);
				uh_rid_del(sn_Set, serialNumber);
			}
		}
		return rv;
	}
	
	return eUXC_SUCCESS;
}	


int tcp_client_send_ipcmsg( tcp_client_t *client, uxc_ipcmsg_t* ipcmsg, int rv)
{
	int msg_size;

	//DBIF 헤더 설정
	// ux_log(UXL_CRT, "sending dbif size : %d ",ipcmsg->header.length);
	// ux_log(UXL_CRT, "sending dbif msgId : %d ", ipcmsg->header.msgId);
	
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length + sizeof(long) ;

	ipcmsg->mtype = UXC_MTYPE_DB;
	ipcmsg->header.dstQid = ipcmsg->header.srcQid;
	ipcmsg->header.srcQid = client->conf->mqid;
	ipcmsg->header.result = rv;
	
	// ux_log(UXL_INFO, "5. Send ipcmsg to %d from %d, size=%d, mtype=%lu + header=%lu + dbif=%d",
	// 	ipcmsg->header.dstQid, ipcmsg->header.srcQid, msg_size, sizeof(long), sizeof(uxc_ixpc_t), ipcmsg->header.length); 
	ux_log(UXL_INFO, " - Send ipcmsg to %d from %d, size=%d, mtype=%lu + header=%lu + dbif=%d",
		ipcmsg->header.dstQid, ipcmsg->header.srcQid, msg_size, sizeof(long), sizeof(uxc_ixpc_t), ipcmsg->header.length); 

	rv = msgsnd(ipcmsg->header.dstQid, &ipcmsg, msg_size, IPC_NOWAIT);
	if( rv < 0) {
		ux_log( UXL_MAJ, "msgsnd: E=%d, q=%d, len=%lu", errno, ipcmsg->header.dstQid,
					sizeof(uxc_ixpc_t) + ipcmsg->header.length);
		return rv;
	}
	ux_log(UXL_INFO, "Success to send : %d", rv);

	return eUXC_SUCCESS;
}

static int _tcp_client_on_accept(upa_tcp_t *tcp, ux_channel_t *channel, ux_accptor_t *accptor,
				ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "Accept TCP connection");
	return eUXC_SUCCESS;
}

void *timeout_function()
{
	int k, i;
	time_t tval;
	time_t rawtime;

	while(timeout_switch > 0) {
		sleep(1);
		time ( &rawtime );

		//hash traverse
		// ux_log(UXL_INFO, "Waiting response=========");
		for (k = kh_begin(sn_timestamp_Map->h); k != kh_end(sn_timestamp_Map->h); ++k) {
			if (kh_exist(sn_timestamp_Map->h, k)) {
				const int key = kh_key(sn_timestamp_Map->h,k);
				tval = kh_value(sn_timestamp_Map->h, k);
				// ux_log(UXL_INFO, "  before|- k:%d  s:%d  e:%d  serialNumber=%d  timestamp=%lu", k, kh_begin(sn_timestamp_Map->h), kh_end(sn_timestamp_Map->h), key, tval);
				if ((rawtime - tval) >= request_timeout) {	//timeout
					ux_log(UXL_CRT, "request timeout, serialNumber : %d", key);
					uh_tmt_del(sn_timestamp_Map, key);
					// printf("1\n");
					uh_int_del(sn_SID_Map, key);
					// printf("2\n");
					uh_int_del(sn_GWSID_Map, key);
					// printf("3\n");
					uh_ipc_del(sn_IPC_Map, key);
					// printf("4\n");
					// ux_log(UXL_INFO, "  after |- k:%d  s:%d  e:%d  serialNumber=%d  timestamp=%lu", k, kh_begin(sn_timestamp_Map->h), kh_end(sn_timestamp_Map->h), key, tval);

					//bind request인 경우, 연결을 끊어주기
					for (i = 0; i < CHANNEL_SIZE; i++) {
						if (key == binding_reqIDs[i]) {
							ux_log(UXL_CRT, "failed to bind : %d", key);
							ux_channel_stop2(channel_arr[i], UX_TRUE);
						}
					}

					//TODO : timeout 발생 시, 추가로 처리해야할 것 고려하기

					if (k == kh_end(sn_timestamp_Map->h)) break;
				}
			}
			// ux_log(UXL_INFO, "k:%d  s:%d  e:%d", k, kh_begin(sn_timestamp_Map->h), kh_end(sn_timestamp_Map->h));
		}
		// ux_log(UXL_INFO, "=========================");
	}

	return NULL;
}

void turn_timeout_timer_on() {
	if (timeout_switch == 0) {
		timeout_switch = 1;
		ux_log( UXL_INFO, "timeout timer on");
		pthread_create(&timeout_thread, NULL, timeout_function, NULL);
	}
}

void turn_timeout_timer_off() {
	if (timeout_switch > 0) {
		ux_log( UXL_INFO, "timeout timer off");
		timeout_switch = 0;
	}
}

void *t_function(void *chnl_idx)
{
	int rv;
	int timeout = _g_client->conf->heartbeat_timeout;
	int interval = _g_client->conf->heartbeat_interval;
	int sent_time = 0;
	int cidx;
	skb_msg_t skbmsg[1];

	memcpy(&cidx, chnl_idx, sizeof(int));
	while(timer_switch[cidx] > 0) {
		sleep(1);
		seconds[cidx]++;
		if (is_heartbeat_sent[cidx] == 0) {
			if (seconds[cidx] - last_recv_seconds[cidx] >= interval) {
				skb_msg_make_header(&skbmsg->header, ALIVE, 0);
				skb_msg_display_send_alive_msg(&skbmsg->header);
				strcpy(skbmsg[0].body,  "");

				//메시지를 Network byte ordering으로 변경
				rv = skb_msg_cvt_order_hton2(skbmsg);
				if( rv< UX_SUCCESS) {
					ux_log(UXL_INFO, "failed to skb_msg_cvt_order_hton2");
					return NULL;
				}

				// ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
				// ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
				rv = upa_tcp_send2(_g_client->patcp, &peerkey_arr[cidx], skbmsg, sizeof(skb_header_t), 1);
				if( rv < UX_SUCCESS) {
					ux_log( UXL_CRT, "failed to upa_tcp_send2");
					//세션 연결 종료
					ux_channel_stop2(channel_arr[cidx], UX_TRUE);
					return NULL;
				}
				is_heartbeat_sent[cidx] = 1;
				sent_time = seconds[cidx];
			}
		} else if (seconds[cidx] - sent_time >= timeout) {	//TIMEOUT
			ux_log( UXL_CRT, "heartbeat timeout : %d", cidx);
			//세션 연결 종료
			ux_channel_stop2(channel_arr[cidx], UX_TRUE);
			return NULL;
		}
	}

	return NULL;
}

void turn_heartbeat_timer_on(int chnl_idx) {
	if (timer_switch[chnl_idx] == 0) {
		timer_switch[chnl_idx] = 1;
		ux_log( UXL_INFO, "heartbeat(chnl_idx : %d) timer on", chnl_idx);
		if (chnl_idx < 0 || chnl_idx >= CHANNEL_SIZE) {
			ux_log(UXL_CRT, "failed to tcp_client_open, unsupported chnl_idx : %d", chnl_idx);
			return;
		}
		pthread_create(&timer_thread[chnl_idx], NULL, t_function, &timer_idx[chnl_idx]);
	}
}

void turn_heartbeat_timer_off(int chnl_idx) {
	if (timer_switch[chnl_idx] > 0) {
		ux_log( UXL_INFO, "heartbeat(chnl_idx : %d) timer off", chnl_idx);
		timer_switch[chnl_idx] = 0;
	}
}

static int _tcp_client_on_open(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{

	skb_msg_t skbmsg[1];
	int msg_size, rv;
	int chnl_idx = peerkey->chnl_idx;
	time_t rawtime;
	char *binding_user_id;
	char *binding_password;

	ux_log( UXL_INFO, "Open TCP connection");

	//heartbeat 초기화
	// 1) IP녹취 시스템은 메시지의 교환이 없는 주기에는 세션의 유효성 검사를 위해 개방형GW로 Heartbeat 요청 메시지를 전달한다. 
	//    이 같은 경우 개방형 GW는 반드시 Heartbeat 응답 메시지를 IP녹취시스템으로 전달하여야 한다.
	// 2) 필요에 의해 개방형GW도 IP녹취시스템으로 Heartbeat 요청 메시지를 전송할 수 있으며, 
	//    이에 대해서 IP녹취 시스템은 바로 Heartbeat 응답 메시지를 개방형 GW로 전달하여야 한다.
	// 3) 세션의 유효성 검사는 2초(주기) 이상 상대 시스템으로부터 수신하는 메시지가 없을 때 요청 메시지를 전송하며, 
	//    7초(Timeout) 이상 Heartbeat응답 메시지를 포함하여 어떠한 메시지의 수신도 없을 때는 연결된 세션을 종료한다.(단, 협의에 의해 변경이 가능하다)
	channel_arr[chnl_idx] = channel;
	peerkey_arr[chnl_idx] = *peerkey;
	timer_switch[chnl_idx] = 0;
	is_heartbeat_sent[chnl_idx] = 0;
	seconds[chnl_idx] = 0;
	last_recv_seconds[chnl_idx] = 0;
	//heartbeat는 bind response 받은 이후 수행

	//binding request 전송
	ux_log( UXL_INFO, "send binding request(chnl_idx : %d)", chnl_idx);
	switch(chnl_idx) {
		case TCP_CHANNEL_SMS_CLI:
			binding_user_id = _g_client->conf->call_binding_user_id;
			binding_password = _g_client->conf->call_binding_password;
			break;
		default:
			ux_log(UXL_CRT, "failed to skb_msg_make_bind_request : wrong channel index");
			return -1;
	}
	rv = skb_msg_make_bind_request(skbmsg, binding_user_id, binding_password);
	if( rv< UX_SUCCESS) {
		ux_log(UXL_CRT, "failed to skb_msg_make_bind_request");
		return -1;
	}
	msg_size = skbmsg->header.length + sizeof(skb_header_t);

	//Bind request의 serialNumber와 current time(ms) Bind
	time ( &rawtime );
	if(!uh_tmt_put(sn_timestamp_Map, serialNumber, rawtime)) {
		ux_log(UXL_CRT, "failed to put to sn_timestamp_Map : (%d)", serialNumber);
	}

	//Bind serialNumber 저장
	binding_reqIDs[chnl_idx] = serialNumber;

	//메시지를 Network byte ordering으로 변경
	rv = skb_msg_cvt_order_hton2(skbmsg);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_CRT, "failed to skb_msg_cvt_order_hton3 : %d", serialNumber);
		return -1;
	}

	// ux_log(UXL_CRT, "seding tcp header size : %lu", sizeof(skbmsg->header));
	// ux_log(UXL_CRT, "seding tcp body size : %lu", msg_size - sizeof(skbmsg->header));
	rv = upa_tcp_send2(_g_client->patcp, &peerkey_arr[chnl_idx], skbmsg, msg_size, 1);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "failed to upa_tcp_send2 : %d", serialNumber);
		//세션 연결 종료
		ux_channel_stop2(channel_arr[chnl_idx], UX_TRUE);
		return -1;
	}


	return eUXC_SUCCESS;
}


static int _tcp_client_on_close(upa_tcp_t *tcp, ux_channel_t *channel, ux_cnector_t *cnector, upa_peerkey_t *peerkey)
{
	turn_heartbeat_timer_off(peerkey->chnl_idx);
	ux_log( UXL_INFO, "Close TCP connection");
	return eUXC_SUCCESS;
}

