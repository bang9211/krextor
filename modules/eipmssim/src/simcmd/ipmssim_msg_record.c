#include "ipmssim_msg_record.h"

int ipmssim_proc_clicktocallrecording_binding_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocallrecording_binding_req_tcp_t clicktocallrecording_binding_req[1];
	clicktocallrecording_binding_rsp_tcp_t clicktocallrecording_binding_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocallrecording_binding_req, rcvmsg->body, sizeof(clicktocallrecording_binding_req_tcp_t));
	clicktocallrecording_binding_req_tcp_display(log, clicktocallrecording_binding_req);

	//make response
	clicktocallrecording_binding_rsp_tcp_init(clicktocallrecording_binding_rsp);
	clicktocallrecording_binding_rsp->resultCode = 0;
	strncpy(clicktocallrecording_binding_rsp->userID, "PGW2", BINDING_USER_ID_LEN);

	skb_msg_make_header(&sndmsg->header, BINDING_RESPONSE, sizeof(clicktocallrecording_binding_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocallrecording_binding_rsp_tcp_display(log, clicktocallrecording_binding_rsp);
	memcpy(sndmsg->body, clicktocallrecording_binding_rsp, sizeof(clicktocallrecording_binding_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocallrecording_start_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocallrecording_start_req_tcp_t clicktocallrecording_start_req[1];
	clicktocallrecording_start_rsp_tcp_t clicktocallrecording_start_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocallrecording_start_req, rcvmsg->body, sizeof(clicktocallrecording_start_req_tcp_t));
	clicktocallrecording_start_req_tcp_display(log, clicktocallrecording_start_req);

	//make response
	clicktocallrecording_start_rsp_tcp_init(clicktocallrecording_start_rsp);
	clicktocallrecording_start_rsp->resultCode = 0;
	strncpy(clicktocallrecording_start_rsp->serviceID, "service0001", SERVICE_ID_LEN);
	strncpy(clicktocallrecording_start_rsp->recordingFileURL, "uangel.com/test/test", RECORDING_FILE_URL_LEN);
	strncpy(clicktocallrecording_start_rsp->recordingFileName, "testFileName", RECORDING_FILE_NAME_LEN);

	skb_msg_make_header(&sndmsg->header, START_RESPONSE, sizeof(clicktocallrecording_start_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocallrecording_start_rsp_tcp_display(log, clicktocallrecording_start_rsp);
	memcpy(sndmsg->body, clicktocallrecording_start_rsp, sizeof(clicktocallrecording_start_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocallrecording_stop_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocallrecording_stop_req_tcp_t clicktocallrecording_stop_req[1];
	clicktocallrecording_stop_rsp_tcp_t clicktocallrecording_stop_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocallrecording_stop_req, rcvmsg->body, sizeof(clicktocallrecording_stop_req_tcp_t));
	clicktocallrecording_stop_req_tcp_display(log, clicktocallrecording_stop_req);

	//make response
	clicktocallrecording_stop_rsp_tcp_init(clicktocallrecording_stop_rsp);
	clicktocallrecording_stop_rsp->resultCode = 0;
	strncpy(clicktocallrecording_stop_rsp->serviceID, "service0001", SERVICE_ID_LEN);

	skb_msg_make_header(&sndmsg->header, STOP_RESPONSE, sizeof(clicktocallrecording_stop_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocallrecording_stop_rsp_tcp_display(log, clicktocallrecording_stop_rsp);
	memcpy(sndmsg->body, clicktocallrecording_stop_rsp, sizeof(clicktocallrecording_stop_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocallrecording_service_status_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocallrecording_service_status_req_tcp_t clicktocallrecording_service_status_req[1];
	clicktocallrecording_service_status_rsp_tcp_t clicktocallrecording_service_status_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocallrecording_service_status_req, rcvmsg->body, sizeof(clicktocallrecording_service_status_req_tcp_t));
	clicktocallrecording_service_status_req_tcp_display(log, clicktocallrecording_service_status_req);

	//make response
	clicktocallrecording_service_status_rsp_tcp_init(clicktocallrecording_service_status_rsp);
	clicktocallrecording_service_status_rsp->resultCode = 0;
	strncpy(clicktocallrecording_service_status_rsp->serviceID, "service0001", SERVICE_ID_LEN);
	clicktocallrecording_service_status_rsp->status = 0;

	skb_msg_make_header(&sndmsg->header, SERVICE_STATUS_RESPONSE, sizeof(clicktocallrecording_service_status_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocallrecording_service_status_rsp_tcp_display(log, clicktocallrecording_service_status_rsp);
	memcpy(sndmsg->body, clicktocallrecording_service_status_rsp, sizeof(clicktocallrecording_service_status_rsp_tcp_t));

	return 0;
}
