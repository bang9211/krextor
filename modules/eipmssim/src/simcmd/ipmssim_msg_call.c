#include "ipmssim_msg_call.h"

//=========================================
// REQUEST
//=========================================

int ipmssim_proc_clicktocall_binding_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocall_binding_req_tcp_t clicktocall_binding_req[1];
	clicktocall_binding_rsp_tcp_t clicktocall_binding_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocall_binding_req, rcvmsg->body, sizeof(clicktocall_binding_req_tcp_t));
	clicktocall_binding_req_tcp_display(log, clicktocall_binding_req);

	//make response
	clicktocall_binding_rsp_tcp_init(clicktocall_binding_rsp);
	clicktocall_binding_rsp->resultCode = 0;
	strncpy(clicktocall_binding_rsp->userID, "PGW1", BINDING_USER_ID_LEN);
	strncpy(clicktocall_binding_rsp->password, "PGW11", BINDING_PASSWORD_LEN);

	skb_msg_make_header(&sndmsg->header, BINDING_RESPONSE, sizeof(clicktocall_binding_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocall_binding_rsp_tcp_display(log, clicktocall_binding_rsp);
	memcpy(sndmsg->body, clicktocall_binding_rsp, sizeof(clicktocall_binding_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocall_start_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocall_start_req_tcp_t clicktocall_start_req[1];
	clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocall_start_req, rcvmsg->body, sizeof(clicktocall_start_req_tcp_t));
	clicktocall_start_req_tcp_display(log, clicktocall_start_req);

	//make response
	clicktocall_start_rsp_tcp_init(clicktocall_start_rsp);
	clicktocall_start_rsp->resultCode = 0;
	strncpy(clicktocall_start_rsp->serviceID, "service0001", SERVICE_ID_LEN);
	clicktocall_start_rsp->isRecording = 0;
	clicktocall_start_rsp->filler1 = 0;
	clicktocall_start_rsp->filler2= 0;
	strncpy(clicktocall_start_rsp->recordingFileURL, "uangel.com/test/test", RECORDING_FILE_URL_LEN);
	strncpy(clicktocall_start_rsp->recordingFileName, "testFileName", RECORDING_FILE_NAME_LEN);

	skb_msg_make_header(&sndmsg->header, START_RESPONSE, sizeof(clicktocall_start_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocall_start_rsp_tcp_display(log, clicktocall_start_rsp);
	memcpy(sndmsg->body, clicktocall_start_rsp, sizeof(clicktocall_start_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocall_stop_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocall_stop_req_tcp_t clicktocall_stop_req[1];
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocall_stop_req, rcvmsg->body, sizeof(clicktocall_stop_req_tcp_t));
	clicktocall_stop_req_tcp_display(log, clicktocall_stop_req);

	//make response
	clicktocall_stop_rsp_tcp_init(clicktocall_stop_rsp);
	clicktocall_stop_rsp->resultCode = 0;
	strncpy(clicktocall_stop_rsp->serviceID, "service0001", SERVICE_ID_LEN);

	skb_msg_make_header(&sndmsg->header, STOP_RESPONSE, sizeof(clicktocall_stop_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocall_stop_rsp_tcp_display(log, clicktocall_stop_rsp);
	memcpy(sndmsg->body, clicktocall_stop_rsp, sizeof(clicktocall_stop_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocall_startrecording_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req[1];
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocall_startrecording_req, rcvmsg->body, sizeof(clicktocall_startrecording_req_tcp_t));
	clicktocall_startrecording_req_tcp_display(log, clicktocall_startrecording_req);

	//make response
	clicktocall_startrecording_rsp_tcp_init(clicktocall_startrecording_rsp);
	clicktocall_startrecording_rsp->resultCode = 0;
	strncpy(clicktocall_startrecording_rsp->serviceID, "service0001", SERVICE_ID_LEN);
	strncpy(clicktocall_startrecording_rsp->recordingFileURL, "uangel.com/test/test", RECORDING_FILE_URL_LEN);
	strncpy(clicktocall_startrecording_rsp->recordingFileName, "testFileName", RECORDING_FILE_NAME_LEN);
	
	skb_msg_make_header(&sndmsg->header, START_RECORDING_RESPONSE, sizeof(clicktocall_startrecording_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocall_startrecording_rsp_tcp_display(log, clicktocall_startrecording_rsp);
	memcpy(sndmsg->body, clicktocall_startrecording_rsp, sizeof(clicktocall_startrecording_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocall_stoprecording_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req[1];
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocall_stoprecording_req, rcvmsg->body, sizeof(clicktocall_stoprecording_req_tcp_t));
	clicktocall_stoprecording_req_tcp_display(log, clicktocall_stoprecording_req);

	//make response
	clicktocall_stoprecording_rsp_tcp_init(clicktocall_stoprecording_rsp);
	clicktocall_stoprecording_rsp->resultCode = 0;
	strncpy(clicktocall_stoprecording_rsp->serviceID, "service0001", SERVICE_ID_LEN);
	
	skb_msg_make_header(&sndmsg->header, STOP_RECORDING_RESPONSE, sizeof(clicktocall_stoprecording_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocall_stoprecording_rsp_tcp_display(log, clicktocall_stoprecording_rsp);
	memcpy(sndmsg->body, clicktocall_stoprecording_rsp, sizeof(clicktocall_stoprecording_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktocall_service_status_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktocall_service_status_req_tcp_t clicktocall_service_status_req[1];
	clicktocall_service_status_rsp_tcp_t clicktocall_service_status_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktocall_service_status_req, rcvmsg->body, sizeof(clicktocall_service_status_req_tcp_t));
	clicktocall_service_status_req_tcp_display(log, clicktocall_service_status_req);

	//make response
	clicktocall_service_status_rsp_tcp_init(clicktocall_service_status_rsp);
	clicktocall_service_status_rsp->resultCode = 0;
	strncpy(clicktocall_service_status_rsp->serviceID, "service0001", SERVICE_ID_LEN);
	clicktocall_service_status_rsp->status = 0;
	
	skb_msg_make_header(&sndmsg->header, SERVICE_STATUS_RESPONSE, sizeof(clicktocall_service_status_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktocall_service_status_rsp_tcp_display(log, clicktocall_service_status_rsp);
	memcpy(sndmsg->body, clicktocall_service_status_rsp, sizeof(clicktocall_service_status_rsp_tcp_t));

	return 0;
}

