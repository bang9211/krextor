#include "tcp_client.h"
#include <upa/upa_tcp.h>

/**
 * @brief GW �κ��� DBIF ��û �޽����� �޾� eIPMS �� TCP Server �� ��û�� �����Ѵ�.
 */
int dbif_handle_gwreq(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	uxc_ipcmsg_t *ipcmsg;
	tcp_client_t *client;
	int msg_size;

	client = tcp_client_instance();
	ipcmsg = (uxc_ipcmsg_t*)msg->data;
	msg_size = sizeof(uxc_ixpc_t) + ipcmsg->header.length + sizeof(long) ;

	// ux_log( UXL_INFO, "1. Received DBIF request "); 
	// ux_log(UXL_INFO, "1. Received ipcmsg from %d to %d, size=%d, mtype=%lu + header=%lu + dbif=%d",
	ux_log(UXL_INFO, " - Received ipcmsg from %d to %d, size=%d, mtype=%lu + header=%lu + dbif=%d",
		ipcmsg->header.srcQid, ipcmsg->header.dstQid, msg_size, sizeof(long), sizeof(uxc_ixpc_t), ipcmsg->header.length); 

	return tcp_client_forward_gwreq( client, worker,ipcmsg);
}

/**
 * @brief eIPMS �� TCP Server �κ��� ������ �޾� GW �� DBIF �� �����Ѵ�. 
 */
int tcp_client_handle_eipmsrsp(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
{
	int rv;
	tcp_client_t *client;
	upa_tcpmsg_t *tcpmsg;

	client = tcp_client_instance();
	tcpmsg = (upa_tcpmsg_t *)uxcmsg->data;

	if ( tcpmsg->evtype == UPA_TCP_EVENT_RECV) {
		// ux_log( UXL_INFO, "4. Received TCP message");
		rv = dbif_forward_eipmsrsp( client, worker,tcpmsg);
	} else {
		ux_log( UXL_INFO, "invalid event");
		return -1;
	}

	return rv;
}

/**
 * @brief eIPMS �� TCP Client �κ��� ��û�� �޾� decode/encode �� ipcmsg �� GW �� �����Ѵ�. 
 * @param action action
 * @param worker worker
 * @param msg uxcutor message
 * @return ���� ���
 */
int tcp_server_handle_smscreq( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *uxcmsg)
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
 * @brief GW �κ��� DBIF ���� �޽����� �޾� eIPMS �� TCP Client �� �����Ѵ�.
 */
int dbif_handle_gwrsp(uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	uxc_ipcmsg_t *ipcmsg;
	tcp_client_t *client;

	client = tcp_client_instance();
	ipcmsg = (uxc_ipcmsg_t*)msg->data;

	ux_log( UXL_INFO, "1. Received DBIF request "); 
	return tcp_client_forward_gwreq( client, worker,ipcmsg);
}
