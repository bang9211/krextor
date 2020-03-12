#include "tcp_client.h"
#include "tcp_server.h"
#include <upa/upa_tcp.h>

/**
 * @brief Test TCP Server ���� IB TCP Client �κ��� TCP DBIF msg �� �޾� decode/encode �� ipcmsg �� uxcutor �� �����Ѵ�. 
 * @param action action
 * @param worker worker
 * @param msg uxcutor message
 * @return ���� ���
 */
int tcp_handle_svrreq( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
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
