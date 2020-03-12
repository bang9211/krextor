#include "tcp_client.h"
#include "tcp_server.h"
#include <upa/upa_tcp.h>

/**
 * @brief DBIF 메시지를 받아 eIPMS 의 TCP Server 에 전송한다.
 */
int tcp_handle_gwreq(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	uxc_ipcmsg_t *ipcmsg;
	tcp_client_t *client;

	client = tcp_client_instance();
	ipcmsg = (uxc_ipcmsg_t*)msg->data;

	ux_log( UXL_INFO, "1. Received DBIF request "); 
	return tcp_client_handle_dbifreq( client, worker,ipcmsg);
}

/**
 * @brief eIPMS 의 TCP Server 로부터 메시지를 받아 DBIF 에 전송한다. 
 */
int tcp_handle_eipmsrsp(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
{
	int rv;
	tcp_client_t *client;
	upa_tcpmsg_t *tcpmsg;

	client = tcp_client_instance();
	tcpmsg = (upa_tcpmsg_t *)uxcmsg->data;

	if ( tcpmsg->evtype == UPA_TCP_EVENT_RECV) {
		ux_log( UXL_INFO, "4. Received TCP message");
		rv = tcp_client_handle_dbifrsp( client, worker,tcpmsg);
	} else {
		ux_log( UXL_INFO, "invalid event");
		return -1;
	}

	return 0;
}

/**
 * @brief eIPMS 의 TCP Client 로부터 TCP DBIF msg 를 받아 decode/encode 후 ipcmsg 를 uxcutor 로 전송한다. 
 * @param action action
 * @param worker worker
 * @param msg uxcutor message
 * @return 실행 결과
 */
int tcp_handle_testsvrreq( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
{
	upa_tcpmsg_t *tcpmsg;
	tcpmsg = (upa_tcpmsg_t *) uxcmsg->data;

	if ( tcpmsg->evtype == UPA_TCP_EVENT_RECV) {
		ux_log( UXL_INFO, "Receive event! (len=%d, worker=%d, chnl_idx=%d, peer_key=%d)",
				tcpmsg->netmsg->buflen, uxc_worker_get_thread_id(worker),
				tcpmsg->peerkey.chnl_idx, tcpmsg->peerkey.peer_key);
		return tcp_server_handle_svrreq( tcp_server_instance(), worker, tcpmsg);
	} else {
		ux_log( UXL_INFO, "invalid event");
		return -1;
	}

	return 0;
}

/**
 * @brief Test TCP Server 에서 IB TCP Client 로부터 TCP DBIF msg 를 받아 decode/encode 후 ipcmsg 를 uxcutor 로 전송한다. 
 * @param action action
 * @param worker worker
 * @param msg uxcutor message
 * @return 실행 결과
 */
int tcp_handle_test_svrreq( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
{
	upa_tcpmsg_t *tcpmsg;
	tcpmsg = (upa_tcpmsg_t *) uxcmsg->data;

	if ( tcpmsg->evtype == UPA_TCP_EVENT_RECV) {
		ux_log( UXL_INFO, "Receive event! (len=%d, worker=%d, chnl_idx=%d, peer_key=%d)",
				tcpmsg->netmsg->buflen, uxc_worker_get_thread_id(worker),
				tcpmsg->peerkey.chnl_idx, tcpmsg->peerkey.peer_key);
		return tcp_server_handle_svrreq( tcp_server_instance(), worker, tcpmsg);
	} else {
		ux_log( UXL_INFO, "invalid event");
		return -1;
	}

	return 0;
}
