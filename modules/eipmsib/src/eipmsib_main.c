#include "tcp_client.h"
#include "tcp_server.h"
#include <upa/upa_tcp.h>

/**
 * @brief GW 로부터 DBIF 요청 메시지를 받아 eIPMS 의 TCP Server 에 요청을 전송한다.
 */
int dbif_handle_gwreq(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	uxc_ipcmsg_t *ipcmsg;
	dbif_server_t *client;

	client = tcp_client_instance();
	ipcmsg = (uxc_ipcmsg_t*)msg->data;

	ux_log( UXL_INFO, "1. Received DBIF request "); 
	return tcp_client_forward_gwreq( client, worker,ipcmsg);
}

/**
 * @brief eIPMS 의 TCP Server 로부터 응답을 받아 GW 의 DBIF 에 전송한다. 
 */
int tcp_client_handle_eipmsrsp(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
{
	int rv;
	dbif_server_t *client;
	upa_tcpmsg_t *tcpmsg;

	client = tcp_client_instance();
	tcpmsg = (upa_tcpmsg_t *)uxcmsg->data;

	if ( tcpmsg->evtype == UPA_TCP_EVENT_RECV) {
		ux_log( UXL_INFO, "4. Received TCP message");
		rv = dbif_forward_eipmsrsp( client, worker,tcpmsg);
	} else {
		ux_log( UXL_INFO, "invalid event");
		return -1;
	}

	return 0;
}

/**
 * @brief eIPMS 의 TCP Client 로부터 요청을 받아 decode/encode 후 ipcmsg 를 GW 로 전송한다. 
 * @param action action
 * @param worker worker
 * @param msg uxcutor message
 * @return 실행 결과
 */
int tcp_handle_eipmsreq( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
{
	upa_tcpmsg_t *tcpmsg;
	tcpmsg = (upa_tcpmsg_t *) uxcmsg->data;

	if ( tcpmsg->evtype == UPA_TCP_EVENT_RECV) {
		ux_log( UXL_INFO, "Receive event! (len=%d, worker=%d, chnl_idx=%d, peer_key=%d)",
				tcpmsg->netmsg->buflen, uxc_worker_get_thread_id(worker),
				tcpmsg->peerkey.chnl_idx, tcpmsg->peerkey.peer_key);
		return tcp_server_handle_eipmsreq( tcp_server_instance(), worker, tcpmsg);
	} else {
		ux_log( UXL_INFO, "invalid event");
		return -1;
	}

	return 0;
}

/**
 * @brief GW 로부터 DBIF 응답 메시지를 받아 eIPMS 의 TCP Client 에 전송한다.
 */
int dbif_handle_gwrsp(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	uxc_ipcmsg_t *ipcmsg;
	dbif_server_t *client;

	client = tcp_client_instance();
	ipcmsg = (uxc_ipcmsg_t*)msg->data;

	ux_log( UXL_INFO, "1. Received DBIF request "); 
	return tcp_client_forward_gwreq( client, worker,ipcmsg);
}
