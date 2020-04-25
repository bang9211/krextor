#include "ipmssim_msg_conf.h"

int ipmssim_proc_clicktoconference_binding_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_binding_req_tcp_t clicktoconference_binding_req[1];
	clicktoconference_binding_rsp_tcp_t clicktoconference_binding_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_binding_req, rcvmsg->body, sizeof(clicktoconference_binding_req_tcp_t));
	clicktoconference_binding_req_tcp_display(log, clicktoconference_binding_req);

	//make response
	clicktoconference_binding_rsp_tcp_init(clicktoconference_binding_rsp);
	clicktoconference_binding_rsp->resultCode = 0;
	strncpy(clicktoconference_binding_rsp->userID, "PGW3", BINDING_USER_ID_LEN);
	strncpy(clicktoconference_binding_rsp->password, "PGW33", BINDING_PASSWORD_LEN);

	skb_msg_make_header(&sndmsg->header, BINDING_RESPONSE, sizeof(clicktoconference_binding_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_binding_rsp_tcp_display(log, clicktoconference_binding_rsp);
	memcpy(sndmsg->body, clicktoconference_binding_rsp, sizeof(clicktoconference_binding_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_start_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_start_req_tcp_t clicktoconference_start_req[1];
	clicktoconference_start_rsp_tcp_t clicktoconference_start_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_start_req, rcvmsg->body, sizeof(clicktoconference_start_req_tcp_t));
	clicktoconference_start_req_tcp_display(log, clicktoconference_start_req);

	//make response
	clicktoconference_start_rsp_tcp_init(clicktoconference_start_rsp);
	clicktoconference_start_rsp->resultCode = 0;
	strncpy(clicktoconference_start_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_start_rsp->chairmanName, "ythan", CHAIRMAN_NAME_LEN);
	strncpy(clicktoconference_start_rsp->chairmanNumber, "01012345678", CALL_NUMBER_LEN);
	strncpy(clicktoconference_start_rsp->chargingNumber, "CH01", CHARGING_NUMBER_LEN);
	clicktoconference_start_rsp->confType = 0;
	clicktoconference_start_rsp->recordingFilePath = 0;
	clicktoconference_start_rsp->filler1 = 0;
	strncpy(clicktoconference_start_rsp->startTime, "YYYYMMDDHH24MISS", TIME_LEN);
	clicktoconference_start_rsp->smsNoti = 0;
	clicktoconference_start_rsp->voiceNoti = 0;
	clicktoconference_start_rsp->filler2 = 0;
	clicktoconference_start_rsp->recordingFileID = 0;
	strncpy(clicktoconference_start_rsp->recordingFileURL, "uangel.com/test/test", RECORDING_FILE_URL_LEN);
	strncpy(clicktoconference_start_rsp->recordingFileName, "testFileName", RECORDING_FILE_NAME_LEN);
	strncpy(clicktoconference_start_rsp->password, "testPW", PASSWORD_LEN);
	clicktoconference_start_rsp->serviceMentID = 0;
	clicktoconference_start_rsp->callingCategoryType = 0;
	strncpy(clicktoconference_start_rsp->msp_id, "", 4);
	strncpy(clicktoconference_start_rsp->sp_id, "", 4);
	strncpy(clicktoconference_start_rsp->service_id, "", 4);

	skb_msg_make_header(&sndmsg->header, START_CONFERENCE_RESPONSE, sizeof(clicktoconference_start_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_start_rsp_tcp_display(log, clicktoconference_start_rsp);
	memcpy(sndmsg->body, clicktoconference_start_rsp, sizeof(clicktoconference_start_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_add_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_add_party_req_tcp_t clicktoconference_add_party_req[1];
	clicktoconference_add_party_rsp_tcp_t clicktoconference_add_party_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_add_party_req, rcvmsg->body, sizeof(clicktoconference_add_party_req_tcp_t));
	clicktoconference_add_party_req_tcp_display(log, clicktoconference_add_party_req);

	//make response
	clicktoconference_add_party_rsp_tcp_init(clicktoconference_add_party_rsp);
	clicktoconference_add_party_rsp->resultCode = 0;
	strncpy(clicktoconference_add_party_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_add_party_rsp->participantList, "hong^01022223333|gil^01044445555|dong^01066667777", MAX_PARTICIPANT_LIST_LEN);

	skb_msg_make_header(&sndmsg->header, ADD_PARTY_RESPONSE, sizeof(clicktoconference_add_party_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_add_party_rsp_tcp_display(log, clicktoconference_add_party_rsp);
	memcpy(sndmsg->body, clicktoconference_add_party_rsp, sizeof(clicktoconference_add_party_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_remove_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_remove_party_req_tcp_t clicktoconference_remove_party_req[1];
	clicktoconference_remove_party_rsp_tcp_t clicktoconference_remove_party_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_remove_party_req, rcvmsg->body, sizeof(clicktoconference_remove_party_req_tcp_t));
	clicktoconference_remove_party_req_tcp_display(log, clicktoconference_remove_party_req);

	//make response
	clicktoconference_remove_party_rsp_tcp_init(clicktoconference_remove_party_rsp);
	clicktoconference_remove_party_rsp->resultCode = 0;
	strncpy(clicktoconference_remove_party_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_remove_party_rsp->participantNumber, "01022223333", CALL_NUMBER_LEN);

	skb_msg_make_header(&sndmsg->header, REMOVE_PARTY_RESPONSE, sizeof(clicktoconference_remove_party_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_remove_party_rsp_tcp_display(log, clicktoconference_remove_party_rsp);
	memcpy(sndmsg->body, clicktoconference_remove_party_rsp, sizeof(clicktoconference_remove_party_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_change_party_media_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_change_party_media_req_tcp_t clicktoconference_change_party_media_req[1];
	clicktoconference_change_party_media_rsp_tcp_t clicktoconference_change_party_media_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_change_party_media_req, rcvmsg->body, sizeof(clicktoconference_change_party_media_req_tcp_t));
	clicktoconference_change_party_media_req_tcp_display(log, clicktoconference_change_party_media_req);

	//make response
	clicktoconference_change_party_media_rsp_tcp_init(clicktoconference_change_party_media_rsp);
	clicktoconference_change_party_media_rsp->resultCode = 0;
	strncpy(clicktoconference_change_party_media_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_change_party_media_rsp->participantNumber, "01088889999", CALL_NUMBER_LEN);
	clicktoconference_change_party_media_rsp->mediaType = 0;

	skb_msg_make_header(&sndmsg->header, CHANGE_PARTY_MEDIA_RESPONSE, sizeof(clicktoconference_change_party_media_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_change_party_media_rsp_tcp_display(log, clicktoconference_change_party_media_rsp);
	memcpy(sndmsg->body, clicktoconference_change_party_media_rsp, sizeof(clicktoconference_change_party_media_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_change_option_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_change_option_req_tcp_t clicktoconference_change_option_req[1];
	clicktoconference_change_option_rsp_tcp_t clicktoconference_change_option_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_change_option_req, rcvmsg->body, sizeof(clicktoconference_change_option_req_tcp_t));
	clicktoconference_change_option_req_tcp_display(log, clicktoconference_change_option_req);

	//make response
	clicktoconference_change_option_rsp_tcp_init(clicktoconference_change_option_rsp);
	clicktoconference_change_option_rsp->resultCode = 0;
	strncpy(clicktoconference_change_option_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_change_option_rsp->password, "password", PASSWORD_LEN);

	skb_msg_make_header(&sndmsg->header, CHANGE_OPTION_RESPONSE, sizeof(clicktoconference_change_option_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_change_option_rsp_tcp_display(log, clicktoconference_change_option_rsp);
	memcpy(sndmsg->body, clicktoconference_change_option_rsp, sizeof(clicktoconference_change_option_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_get_number_of_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_get_number_of_party_req_tcp_t clicktoconference_get_number_of_party_req[1];
	clicktoconference_get_number_of_party_rsp_tcp_t clicktoconference_get_number_of_party_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_get_number_of_party_req, rcvmsg->body, sizeof(clicktoconference_get_number_of_party_req_tcp_t));
	clicktoconference_get_number_of_party_req_tcp_display(log, clicktoconference_get_number_of_party_req);

	//make response
	clicktoconference_get_number_of_party_rsp_tcp_init(clicktoconference_get_number_of_party_rsp);
	clicktoconference_get_number_of_party_rsp->resultCode = 0;
	strncpy(clicktoconference_get_number_of_party_rsp->confID, "C0001", CONF_ID_LEN);
	clicktoconference_get_number_of_party_rsp->number = 0;

	skb_msg_make_header(&sndmsg->header, GET_NUMBER_OF_PARTY_RESPONSE, sizeof(clicktoconference_get_number_of_party_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_get_number_of_party_rsp_tcp_display(log, clicktoconference_get_number_of_party_rsp);
	memcpy(sndmsg->body, clicktoconference_get_number_of_party_rsp, sizeof(clicktoconference_get_number_of_party_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_stop_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_stop_req_tcp_t clicktoconference_stop_req[1];
	clicktoconference_stop_rsp_tcp_t clicktoconference_stop_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_stop_req, rcvmsg->body, sizeof(clicktoconference_stop_req_tcp_t));
	clicktoconference_stop_req_tcp_display(log, clicktoconference_stop_req);

	//make response
	clicktoconference_stop_rsp_tcp_init(clicktoconference_stop_rsp);
	clicktoconference_stop_rsp->resultCode = 0;
	strncpy(clicktoconference_stop_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_stop_rsp->startTime, "YYYYMMDDHH24MISS", TIME_LEN);
	strncpy(clicktoconference_stop_rsp->endTime, "YYYYMMDDHH24MISS", TIME_LEN);
	clicktoconference_stop_rsp->recordingFileID = 0;

	skb_msg_make_header(&sndmsg->header, STOP_CONFERENCE_RESPONSE, sizeof(clicktoconference_stop_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_stop_rsp_tcp_display(log, clicktoconference_stop_rsp);
	memcpy(sndmsg->body, clicktoconference_stop_rsp, sizeof(clicktoconference_stop_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_play_ment_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_play_ment_req_tcp_t clicktoconference_play_ment_req[1];
	clicktoconference_play_ment_rsp_tcp_t clicktoconference_play_ment_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_play_ment_req, rcvmsg->body, sizeof(clicktoconference_play_ment_req_tcp_t));
	clicktoconference_play_ment_req_tcp_display(log, clicktoconference_play_ment_req);

	//make response
	clicktoconference_play_ment_rsp_tcp_init(clicktoconference_play_ment_rsp);
	clicktoconference_play_ment_rsp->resultCode = 0;
	strncpy(clicktoconference_play_ment_rsp->confID, "C0001", CONF_ID_LEN);

	skb_msg_make_header(&sndmsg->header, PLAY_MENT_RESPONSE, sizeof(clicktoconference_play_ment_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_play_ment_rsp_tcp_display(log, clicktoconference_play_ment_rsp);
	memcpy(sndmsg->body, clicktoconference_play_ment_rsp, sizeof(clicktoconference_play_ment_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_get_party_status_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_get_party_status_req_tcp_t clicktoconference_get_party_status_req[1];
	clicktoconference_get_party_status_rsp_tcp_t clicktoconference_get_party_status_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_get_party_status_req, rcvmsg->body, sizeof(clicktoconference_get_party_status_req_tcp_t));
	clicktoconference_get_party_status_req_tcp_display(log, clicktoconference_get_party_status_req);

	//make response
	clicktoconference_get_party_status_rsp_tcp_init(clicktoconference_get_party_status_rsp);
	clicktoconference_get_party_status_rsp->resultCode = 0;
	strncpy(clicktoconference_get_party_status_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_get_party_status_rsp->participantName, "gil", PARTICIPANT_NAME_LEN);
	strncpy(clicktoconference_get_party_status_rsp->participantNumber, "01044445555", PARTICIPANT_NUMBER_LEN);

	skb_msg_make_header(&sndmsg->header, GET_PARTY_STATUS_RESPONSE, sizeof(clicktoconference_get_party_status_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_get_party_status_rsp_tcp_display(log, clicktoconference_get_party_status_rsp);
	memcpy(sndmsg->body, clicktoconference_get_party_status_rsp, sizeof(clicktoconference_get_party_status_rsp_tcp_t));

	return 0;
}

int ipmssim_proc_clicktoconference_cancel_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg) {
	clicktoconference_cancel_party_req_tcp_t clicktoconference_cancel_party_req[1];
	clicktoconference_cancel_party_rsp_tcp_t clicktoconference_cancel_party_rsp[1];
	char log[SKB_HEADER_DISPLAY_SIZE];

	//request display
	skb_msg_get_recv_header_display(&rcvmsg->header, log);
	memcpy(clicktoconference_cancel_party_req, rcvmsg->body, sizeof(clicktoconference_cancel_party_req_tcp_t));
	clicktoconference_cancel_party_req_tcp_display(log, clicktoconference_cancel_party_req);

	//make response
	clicktoconference_cancel_party_rsp_tcp_init(clicktoconference_cancel_party_rsp);
	clicktoconference_cancel_party_rsp->resultCode = 0;
	strncpy(clicktoconference_cancel_party_rsp->confID, "C0001", CONF_ID_LEN);
	strncpy(clicktoconference_cancel_party_rsp->participantNumber, "01066667777", PARTICIPANT_NUMBER_LEN);

	skb_msg_make_header(&sndmsg->header, CANCEL_PARTY_RESPONSE, sizeof(clicktoconference_cancel_party_rsp), &rcvmsg->header.requestID);
	skb_msg_get_send_header_display(&sndmsg->header, log);
	clicktoconference_cancel_party_rsp_tcp_display(log, clicktoconference_cancel_party_rsp);
	memcpy(sndmsg->body, clicktoconference_cancel_party_rsp, sizeof(clicktoconference_cancel_party_rsp_tcp_t));

	return 0;
}