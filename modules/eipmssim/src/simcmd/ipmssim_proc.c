
#include "ipmssim_proc.h"


static int _g_send_msg_flag = 1;
static int _g_send_succ_flag = 1;

static int _g_heartbeat_time = 2;
static int _g_heartbeat_timer = 1;
static int _g_timeout_time = 7;

static int _g_heartbeat_display = 0;

static int _g_stsinqack_seq_chg_flag = 1;
static int _g_sequence = -1;
static char _g_date[8];
static int _g_usgrep_timeout_flag = 0;
static int _g_bind_flag[3] = {0,};



//////////////////////////////////////////////////////////////////////////////
// public functions - control
//////////////////////////////////////////////////////////////////////////////

int ipmssim_get_send_msg_flag() { return _g_send_msg_flag; }
int ipmssim_get_send_succ_flag() { return _g_send_succ_flag; }

int ipmssim_get_heartbeat_time() { return _g_heartbeat_time; }
int ipmssim_get_heartbeat_timer() { return _g_heartbeat_timer; }
int ipmssim_get_timeout_time() { return _g_timeout_time; }
int ipmssim_get_heartbeat_display() { return _g_heartbeat_display; }

int ipmssim_get_bind_flag(int chnl_idx) { return _g_bind_flag[chnl_idx]; }
int ipmssim_get_stsinqack_seq_chg_flag() { return _g_stsinqack_seq_chg_flag; }
int ipmssim_get_sequence() { return _g_sequence; }
char *ipmssim_get_date() { return _g_date; }
int ipmssim_get_usgrep_timeout_flag() { return _g_usgrep_timeout_flag; }

void ipmssim_set_send_msg_flag( int flag ) { _g_send_msg_flag = !flag ? 0 : 1; }
void ipmssim_set_send_succ_flag( int flag ) { _g_send_succ_flag = !flag ? 0 : 1; }

void ipmssim_set_heartbeat_time( int hbtime ) { 
	if( hbtime >= 0 && hbtime <= 60 ) _g_heartbeat_time = hbtime; }
void ipmssim_set_timeout_time( int totime ) { 
	if( totime >= 0 && totime <= 60 ) _g_timeout_time = totime; }
void ipmssim_set_heartbeat_display( int hbd ) { _g_heartbeat_display = hbd; _heartbeat_display = hbd; }


void ipmssim_set_stsinqack_seq_chg_flag( int flag ) { _g_stsinqack_seq_chg_flag = !flag ? 0 : 1; }
void ipmssim_set_sequence( int sequence ) { 
	if( sequence >= IPMSSIM_SEQ_MIN && sequence <= IPMSSIM_SEQ_MAX ) _g_sequence = sequence; }
void ipmssim_set_usgrep_timeout_flag( int flag ) { _g_usgrep_timeout_flag = !flag ? 0 : 1; }
void ipmssim_set_date( char *date ) {
	if( !date || strlen(date) != 4 ) return ;
	int _month = (date[0]-'0')*10 + (date[1]-'0');
	if( _month < 1 || _month > 12 ) return ;
	int _day_max[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int _day = (date[2]-'0')*10 + (date[3]-'0');
	if( _day < 1 || _day > _day_max[_month-1] ) return ;
	// 2?? 28???? ???? ???????? ???? ??????.
	strcpy( _g_date, date ); 
}


//////////////////////////////////////////////////////////////////////////////
// public functions - ibrs proc 
//////////////////////////////////////////////////////////////////////////////

void ipmssim_proc_heartbeat()
{
	// ipmssim_tcpip_keepalive( _g_heartbeat_time, ipmssim_proc_status_inquery );
}

int ipmssim_proc_msg( skb_msg_t *rcvmsg, int idx )
{
	int rv = 0, snd_flag = 0;
	// int set_seq = 0;
	// int result;
	// char trcid[IPMSSIM_TRCID_SIZE+8], timestr[64], datestr[8];
	skb_msg_t sndmsg;
	skb_header_t *header;

	if( !rcvmsg|| idx < 0 ) return -1;

	header = &rcvmsg->header;

	switch(idx) {
		case TCP_CHANNEL_CALL:
			switch( header->messageID ) {
				case BINDING_REQUEST:
					if( !ipmssim_get_send_msg_flag() ) { // not send case
						uaLog( L_MIN, "Not send BINDING_RESPONSE." );
						break;
					}
					if( !ipmssim_get_send_succ_flag() ) { // error send case
						// result = eIPMSSIM_SYSTEM_FAIL;
					} else {
						// if( header->version0 != 0x00 && header->version1 != 0x01) result = eIPMSSIM_INVALID_VERSION;
						// else result = eIPMSSIM_NO_ERROR;
					}
					rv = ipmssim_proc_clicktocall_binding_req(idx, rcvmsg, &sndmsg);
					_g_bind_flag[TCP_CHANNEL_CALL] = 1;
					turn_heartbeat_timer_on(TCP_CHANNEL_CALL);
					snd_flag = 1;
					// set_seq = 1;
					break;
				case HEARTBEAT_REQUEST:
					skb_msg_display_recv_heartbeat_req(&rcvmsg->header, TCP_CHANNEL_CALL);
					skb_msg_make_header(&sndmsg.header, HEARTBEAT_RESPONSE, 0, &rcvmsg->header.requestID);
					skb_msg_display_send_heartbeat_rsp(&sndmsg.header, TCP_CHANNEL_CALL);
					snd_flag = 1;
					break;
				case HEARTBEAT_RESPONSE:
					is_heartbeat_sent[TCP_CHANNEL_CALL] = 0;
					last_recv_seconds[TCP_CHANNEL_CALL] = seconds[TCP_CHANNEL_CALL];	
					skb_msg_display_recv_heartbeat_rsp(&rcvmsg->header, TCP_CHANNEL_CALL);
					break;
				case START_REQUEST:
					rv = ipmssim_proc_clicktocall_start_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case STOP_REQUEST:
					rv = ipmssim_proc_clicktocall_stop_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case START_RECORDING_REQUEST:
					rv = ipmssim_proc_clicktocall_startrecording_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case STOP_RECORDING_REQUEST:
					rv = ipmssim_proc_clicktocall_stoprecording_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case SERVICE_STATUS_REQUEST:
					rv = ipmssim_proc_clicktocall_service_status_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				default:
					printf("unsupported message id : %#010x, %d\n", header->messageID, idx);
					// uaLog( L_MAJ, "Unknown message id. : msgid = %d (tid:%s, time:%s)\n",
					// 		header->messageID, 
					// 		ipmssim_msg_dec_header_trcid(rcvmsg, trcid, sizeof(trcid)), 
					// 		ipmssim_msg_dec_header_time(rcvmsg, "", timestr, sizeof(timestr)) );
					// rv = -1;
					return -1;
			}
			break;
		case TCP_CHANNEL_RECORDING:
			switch( header->messageID ) {
				case BINDING_REQUEST:
					rv = ipmssim_proc_clicktocallrecording_binding_req(idx, rcvmsg, &sndmsg);
					_g_bind_flag[TCP_CHANNEL_RECORDING] = 1;
					turn_heartbeat_timer_on(TCP_CHANNEL_RECORDING);
					snd_flag = 1;
					// set_seq = 1;
					break;
				case HEARTBEAT_REQUEST:
					skb_msg_display_recv_heartbeat_req(&rcvmsg->header, TCP_CHANNEL_RECORDING);
					skb_msg_make_header(&sndmsg.header, HEARTBEAT_RESPONSE, 0, &rcvmsg->header.requestID);
					skb_msg_display_send_heartbeat_rsp(&sndmsg.header, TCP_CHANNEL_RECORDING);
					snd_flag = 1;
					// set_seq = 1;
					break;
				case HEARTBEAT_RESPONSE:
					is_heartbeat_sent[TCP_CHANNEL_RECORDING] = 0;
					last_recv_seconds[TCP_CHANNEL_RECORDING] = seconds[TCP_CHANNEL_RECORDING];
					skb_msg_display_recv_heartbeat_rsp(&rcvmsg->header, TCP_CHANNEL_RECORDING);
					break;
				case START_REQUEST:
					rv = ipmssim_proc_clicktocallrecording_start_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case STOP_REQUEST:
					rv = ipmssim_proc_clicktocallrecording_stop_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case SERVICE_STATUS_REQUEST:
					rv = ipmssim_proc_clicktocallrecording_service_status_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				default:
					printf("unsupported message id : %#010x, %d\n", header->messageID, idx);
					return -1;
			}
			break;
		case TCP_CHANNEL_CONFERENCE:
			switch( header->messageID ) {
				case BINDING_REQUEST:
					rv = ipmssim_proc_clicktoconference_binding_req(idx, rcvmsg, &sndmsg);
					_g_bind_flag[TCP_CHANNEL_CONFERENCE] = 1;
					turn_heartbeat_timer_on(TCP_CHANNEL_CONFERENCE);
					snd_flag = 1;
					// set_seq = 1;
					break;
				case HEARTBEAT_REQUEST:
					skb_msg_display_recv_heartbeat_req(&rcvmsg->header, TCP_CHANNEL_CONFERENCE);
					skb_msg_make_header(&sndmsg.header, HEARTBEAT_RESPONSE, 0, &rcvmsg->header.requestID);
					skb_msg_display_send_heartbeat_rsp(&sndmsg.header, TCP_CHANNEL_CONFERENCE);
					snd_flag = 1;
					// set_seq = 1;
					break;
				case HEARTBEAT_RESPONSE:
					is_heartbeat_sent[TCP_CHANNEL_CONFERENCE] = 0;
					last_recv_seconds[TCP_CHANNEL_CONFERENCE] = seconds[TCP_CHANNEL_CONFERENCE];
					skb_msg_display_recv_heartbeat_rsp(&rcvmsg->header, TCP_CHANNEL_CONFERENCE);
					break;
				case START_CONFERENCE_REQUEST:
					rv = ipmssim_proc_clicktoconference_start_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case STOP_CONFERENCE_REQUEST:
					rv = ipmssim_proc_clicktoconference_stop_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case ADD_PARTY_REQUEST:
					rv = ipmssim_proc_clicktoconference_add_party_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case REMOVE_PARTY_REQUEST:
					rv = ipmssim_proc_clicktoconference_remove_party_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case CANCEL_PARTY_REQUEST:
					rv = ipmssim_proc_clicktoconference_cancel_party_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case CHANGE_PARTY_MEDIA_REQUEST:
					rv = ipmssim_proc_clicktoconference_change_party_media_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case PLAY_MENT_REQUEST:
					rv = ipmssim_proc_clicktoconference_play_ment_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case GET_NUMBER_OF_PARTY_REQUEST:
					rv = ipmssim_proc_clicktoconference_get_number_of_party_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				case GET_PARTY_STATUS_REQUEST:
					rv = ipmssim_proc_clicktoconference_get_party_status_req(idx, rcvmsg, &sndmsg);
					snd_flag = 1;
					break;
				default:
					printf("unsupported message id : %#010x, %d\n", header->messageID, idx);
					return -1;
			}
			break;
		default:
			printf("unsupported chnl_idx : %d\n", idx);
			return -1;
	}
	if( rv < 0 ) {
		printf("failed to process request : %#010x, %d\n", header->messageID, header->requestID);
	}

	if (rcvmsg->header.messageID != HEARTBEAT_REQUEST) {
		last_recv_seconds[idx] = seconds[idx] + 1;
	}

	// if( set_seq ) {
	// 	ipmssim_msg_dec_header_trcid( rcvmsg, trcid, sizeof(trcid) );
	// 	strncpy( _g_date, trcid, 4 );
	// 	_g_date[4] = '\0';
	// 	_g_sequence = strtol( trcid+4, 0, 10 );
	// }
	if( snd_flag ) { 
		rv = ipmssim_tcpip_write( idx, &sndmsg );
	}

	return rv;	
}

int ipmssim_proc_make_call_service_status_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_call_service_status_rpt( const char * fname) {
	
	cJSON *json = NULL;
	const cJSON *serviceID = NULL;
	const cJSON *status = NULL;

	skb_msg_t sndmsg;

	clicktocall_service_status_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (!cJSON_IsNumber(status)) {
		ux_log(UXL_MAJ, "Fail to get status JSON key");
		return -1;
	}
	rpt.status = status->valueint;

	skb_msg_make_header(&sndmsg.header, SERVICE_STATUS_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CALL, &sndmsg );
}

int ipmssim_proc_make_call_call_end_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_call_call_end_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *returnCode = NULL;
	const cJSON *serviceID = NULL;
	const cJSON *startTime = NULL;
	const cJSON *endTime = NULL;
	const cJSON *isRecorded = NULL;

	skb_msg_t sndmsg;

	clicktocall_stop_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	returnCode = cJSON_GetObjectItemCaseSensitive(json, "returnCode");
    if (!cJSON_IsNumber(returnCode)) {
		ux_log(UXL_MAJ, "Fail to get returnCode JSON key");
		return -1;
	}
	rpt.returnCode = returnCode->valueint;

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	startTime = cJSON_GetObjectItemCaseSensitive(json, "startTime");
    if (!cJSON_IsString(startTime) || (startTime->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get startTime JSON key");
		return -1;	
    } 
	strncpy(rpt.startTime,  startTime->valuestring, TIME_LEN);

	endTime = cJSON_GetObjectItemCaseSensitive(json, "endTime");
    if (!cJSON_IsString(endTime) || (endTime->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get endTime JSON key");
		return -1;	
    } 
	strncpy(rpt.endTime,  endTime->valuestring, TIME_LEN);

	isRecorded = cJSON_GetObjectItemCaseSensitive(json, "isRecorded");
    if (!cJSON_IsNumber(isRecorded)) {
		ux_log(UXL_MAJ, "Fail to get isRecorded JSON key");
		return -1;
	}
	rpt.isRecorded = isRecorded->valueint;

	skb_msg_make_header(&sndmsg.header, STOP_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CALL, &sndmsg );
}

int ipmssim_proc_make_call_recording_start_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_call_recording_start_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *serviceID = NULL;

	skb_msg_t sndmsg;

	clicktocall_startrecording_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	skb_msg_make_header(&sndmsg.header, START_RECORDING_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CALL, &sndmsg );
}

int ipmssim_proc_make_call_recording_end_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_call_recording_end_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *serviceID = NULL;

	skb_msg_t sndmsg;

	clicktocall_stoprecording_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	skb_msg_make_header(&sndmsg.header, STOP_RECORDING_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CALL, &sndmsg );
}

int ipmssim_proc_make_record_service_status_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_record_service_status_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *serviceID = NULL;
	const cJSON *status = NULL;

	skb_msg_t sndmsg;

	clicktocallrecording_service_status_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (!cJSON_IsNumber(status)) {
		ux_log(UXL_MAJ, "Fail to get status JSON key");
		return -1;
	}
	rpt.status = status->valueint;

	skb_msg_make_header(&sndmsg.header, SERVICE_STATUS_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_RECORDING, &sndmsg );
}

int ipmssim_proc_make_record_call_start_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_record_call_start_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *serviceID = NULL;
	const cJSON *recordingFileURL = NULL;
	const cJSON *recordingFileName = NULL;

	skb_msg_t sndmsg;

	clicktocallrecording_start_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	recordingFileURL = cJSON_GetObjectItemCaseSensitive(json, "recordingFileURL");
    if (!cJSON_IsString(recordingFileURL) || (recordingFileURL->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get recordingFileURL JSON key");
		return -1;	
    } 
	strncpy(rpt.recordingFileURL,  recordingFileURL->valuestring, RECORDING_FILE_URL_LEN);

	recordingFileName = cJSON_GetObjectItemCaseSensitive(json, "recordingFileName");
    if (!cJSON_IsString(recordingFileName) || (recordingFileName->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get recordingFileName JSON key");
		return -1;	
    } 
	strncpy(rpt.recordingFileName,  recordingFileName->valuestring, RECORDING_FILE_NAME_LEN);


	skb_msg_make_header(&sndmsg.header, START_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_RECORDING, &sndmsg );
}

int ipmssim_proc_make_record_call_end_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_record_call_end_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *returnCode = NULL;
	const cJSON *serviceID = NULL;
	const cJSON *startTime = NULL;
	const cJSON *endTime = NULL;
	const cJSON *callingNumber = NULL;
	const cJSON *calledNumber = NULL;
	const cJSON *isRecorded = NULL;
	const cJSON *recordingFileURL = NULL;
	const cJSON *recordingFileName = NULL;

	skb_msg_t sndmsg;

	clicktocallrecording_stop_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	returnCode = cJSON_GetObjectItemCaseSensitive(json, "returnCode");
    if (!cJSON_IsNumber(returnCode)) {
		ux_log(UXL_MAJ, "Fail to get returnCode JSON key");
		return -1;
	}
	rpt.returnCode = returnCode->valueint;

	serviceID = cJSON_GetObjectItemCaseSensitive(json, "serviceID");
    if (!cJSON_IsString(serviceID) || (serviceID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get serviceID JSON key");
		return -1;	
    } 
	strncpy(rpt.serviceID,  serviceID->valuestring, SERVICE_ID_LEN);

	startTime = cJSON_GetObjectItemCaseSensitive(json, "startTime");
    if (!cJSON_IsString(startTime) || (startTime->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get startTime JSON key");
		return -1;	
    } 
	strncpy(rpt.startTime,  startTime->valuestring, TIME_LEN);

	endTime = cJSON_GetObjectItemCaseSensitive(json, "endTime");
    if (!cJSON_IsString(endTime) || (endTime->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get endTime JSON key");
		return -1;	
    } 
	strncpy(rpt.endTime,  endTime->valuestring, TIME_LEN);

	callingNumber = cJSON_GetObjectItemCaseSensitive(json, "callingNumber");
    if (!cJSON_IsString(callingNumber) || (callingNumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get callingNumber JSON key");
		return -1;	
    } 
	strncpy(rpt.callingNumber,  callingNumber->valuestring, CALL_NUMBER_LEN);

	calledNumber = cJSON_GetObjectItemCaseSensitive(json, "calledNumber");
    if (!cJSON_IsString(calledNumber) || (calledNumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get calledNumber JSON key");
		return -1;	
    } 
	strncpy(rpt.calledNumber,  calledNumber->valuestring, CALL_NUMBER_LEN);

	isRecorded = cJSON_GetObjectItemCaseSensitive(json, "isRecorded");
    if (!cJSON_IsNumber(isRecorded)) {
		ux_log(UXL_MAJ, "Fail to get isRecorded JSON key");
		return -1;
	}
	rpt.isRecorded = isRecorded->valueint;

	recordingFileURL = cJSON_GetObjectItemCaseSensitive(json, "recordingFileURL");
    if (!cJSON_IsString(recordingFileURL) || (recordingFileURL->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get recordingFileURL JSON key");
		return -1;	
    } 
	strncpy(rpt.recordingFileURL,  recordingFileURL->valuestring, RECORDING_FILE_URL_LEN);

	recordingFileName = cJSON_GetObjectItemCaseSensitive(json, "recordingFileName");
    if (!cJSON_IsString(recordingFileName) || (recordingFileName->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get recordingFileName JSON key");
		return -1;	
    } 
	strncpy(rpt.recordingFileName,  recordingFileName->valuestring, RECORDING_FILE_NAME_LEN);


	skb_msg_make_header(&sndmsg.header, STOP_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_RECORDING, &sndmsg );
}

int ipmssim_proc_make_conf_add_party_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_conf_add_party_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *confID = NULL;
	const cJSON *participantName = NULL;
	const cJSON *participantNumber = NULL;

	skb_msg_t sndmsg;

	clicktoconference_add_party_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	confID = cJSON_GetObjectItemCaseSensitive(json, "confID");
    if (!cJSON_IsString(confID) || (confID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get confID JSON key");
		return -1;	
    } 
	strncpy(rpt.confID,  confID->valuestring, CONF_ID_LEN);

	participantName = cJSON_GetObjectItemCaseSensitive(json, "participantName");
    if (!cJSON_IsString(participantName) || (participantName->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get participantName JSON key");
		return -1;	
    } 
	strncpy(rpt.participantName,  participantName->valuestring, PARTICIPANT_NAME_LEN);

	participantNumber = cJSON_GetObjectItemCaseSensitive(json, "participantNumber");
    if (!cJSON_IsString(participantNumber) || (participantNumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get participantNumber JSON key");
		return -1;	
    } 
	strncpy(rpt.participantNumber,  participantNumber->valuestring, PARTICIPANT_NUMBER_LEN);

	skb_msg_make_header(&sndmsg.header, ADD_PARTY_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CONFERENCE, &sndmsg );
}

int ipmssim_proc_make_conf_remove_party_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_conf_remove_party_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *confID = NULL;
	const cJSON *participantNumber = NULL;

	skb_msg_t sndmsg;

	clicktoconference_remove_party_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	confID = cJSON_GetObjectItemCaseSensitive(json, "confID");
    if (!cJSON_IsString(confID) || (confID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get confID JSON key");
		return -1;	
    } 
	strncpy(rpt.confID,  confID->valuestring, CONF_ID_LEN);

	participantNumber = cJSON_GetObjectItemCaseSensitive(json, "participantNumber");
    if (!cJSON_IsString(participantNumber) || (participantNumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get participantNumber JSON key");
		return -1;	
    } 
	strncpy(rpt.participantNumber,  participantNumber->valuestring, CALL_NUMBER_LEN);

	skb_msg_make_header(&sndmsg.header, REMOVE_PARTY_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CONFERENCE, &sndmsg );
}

int ipmssim_proc_make_conf_change_party_media_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_conf_change_party_media_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *confID = NULL;
	const cJSON *participantNumber = NULL;
	const cJSON *mediaType = NULL;

	skb_msg_t sndmsg;

	clicktoconference_change_party_media_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	confID = cJSON_GetObjectItemCaseSensitive(json, "confID");
    if (!cJSON_IsString(confID) || (confID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get confID JSON key");
		return -1;	
    } 
	strncpy(rpt.confID,  confID->valuestring, CONF_ID_LEN);

	participantNumber = cJSON_GetObjectItemCaseSensitive(json, "participantNumber");
    if (!cJSON_IsString(participantNumber) || (participantNumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get participantNumber JSON key");
		return -1;	
    } 
	strncpy(rpt.participantNumber,  participantNumber->valuestring, CALL_NUMBER_LEN);

	mediaType = cJSON_GetObjectItemCaseSensitive(json, "mediaType");
    if (!cJSON_IsNumber(mediaType)) {
		ux_log(UXL_MAJ, "Fail to get mediaType JSON key");
		return -1;
	}
	rpt.mediaType = mediaType->valueint;

	skb_msg_make_header(&sndmsg.header, CHANGE_PARTY_MEDIA_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CONFERENCE, &sndmsg );
}

int ipmssim_proc_make_conf_change_option_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_conf_change_option_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *resultCode = NULL;
	const cJSON *confID = NULL;
	const cJSON *password = NULL;

	skb_msg_t sndmsg;

	clicktoconference_change_option_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	resultCode = cJSON_GetObjectItemCaseSensitive(json, "resultCode");
    if (!cJSON_IsNumber(resultCode)) {
		ux_log(UXL_MAJ, "Fail to get resultCode JSON key");
		return -1;
	}
	rpt.resultCode = resultCode->valueint;

	confID = cJSON_GetObjectItemCaseSensitive(json, "confID");
    if (!cJSON_IsString(confID) || (confID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get confID JSON key");
		return -1;	
    } 
	strncpy(rpt.confID,  confID->valuestring, CONF_ID_LEN);

	password = cJSON_GetObjectItemCaseSensitive(json, "password");
    if (!cJSON_IsString(password) || (password->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get password JSON key");
		return -1;	
    } 
	strncpy(rpt.password,  password->valuestring, PASSWORD_LEN);

	skb_msg_make_header(&sndmsg.header, CHANGE_OPTION_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CONFERENCE, &sndmsg );
}

int ipmssim_proc_make_conf_close_conf_rpt( const char * fname) {
	return 0;
}

int ipmssim_proc_send_conf_close_conf_rpt( const char * fname) {
	cJSON *json = NULL;
	const cJSON *returnCode = NULL;
	const cJSON *confID = NULL;
	const cJSON *startTime = NULL;
	const cJSON *endTime = NULL;
	const cJSON *isRecorded = NULL;

	skb_msg_t sndmsg;

	clicktoconference_stop_rpt_tcp_t rpt;

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	returnCode = cJSON_GetObjectItemCaseSensitive(json, "returnCode");
    if (!cJSON_IsNumber(returnCode)) {
		ux_log(UXL_MAJ, "Fail to get returnCode JSON key");
		return -1;
	}
	rpt.returnCode = returnCode->valueint;

	confID = cJSON_GetObjectItemCaseSensitive(json, "confID");
    if (!cJSON_IsString(confID) || (confID->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get confID JSON key");
		return -1;	
    } 
	strncpy(rpt.confID,  confID->valuestring, CONF_ID_LEN);

	startTime = cJSON_GetObjectItemCaseSensitive(json, "startTime");
    if (!cJSON_IsString(startTime) || (startTime->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get startTime JSON key");
		return -1;	
    } 
	strncpy(rpt.startTime,  startTime->valuestring, TIME_LEN);

	endTime = cJSON_GetObjectItemCaseSensitive(json, "endTime");
    if (!cJSON_IsString(endTime) || (endTime->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get endTime JSON key");
		return -1;	
    } 
	strncpy(rpt.endTime,  endTime->valuestring, TIME_LEN);

	isRecorded = cJSON_GetObjectItemCaseSensitive(json, "isRecorded");
    if (!cJSON_IsNumber(isRecorded)) {
		ux_log(UXL_MAJ, "Fail to get isRecorded JSON key");
		return -1;
	}
	rpt.isRecorded = isRecorded->valueint;

	skb_msg_make_header(&sndmsg.header, STOP_CONFERENCE_REPORT, sizeof(rpt), NULL);
	memcpy(sndmsg.body, &rpt, sizeof(rpt));
	return ipmssim_tcpip_write( TCP_CHANNEL_CONFERENCE, &sndmsg );
}

int ipmssim_proc_get_json_from_file( const char * fname, cJSON **json) {

	int len = 0;
	char linestr[2048], data[IPMSSIM_DATA_SIZE];
	FILE *fp;

	fp = fopen( fname ? fname : "", "r" );
	if( !fp ) {
		uaLog( L_MAJ, "file open failed. fname='%s', err=%d(%s)",
				fname ? fname : "", errno, strerror(errno) );
		return -1;
	}
	while( (fgets(linestr, sizeof(linestr), fp)) ) {
		len += snprintf( data+len, sizeof(data) > len ? sizeof(data)-len : 0, "%s", linestr );
		// if( len >= sizeof(sndmsg.data) ) break;
	}
	fclose( fp );

	*json = cJSON_Parse(data);
	if ( *json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
			ux_log(UXL_MAJ, "Fail to parse HTTP body. err=%s", error_ptr);
        }
		return -1;
	}
	return UX_SUCCESS;
}

int ipmssim_proc_send_test_rpt( const char * fname, int chnl_idx) {
	cJSON *json = NULL;
	const cJSON *header = NULL;
	const cJSON *body = NULL;
	const cJSON *item = NULL;
	const cJSON *intItem = NULL;
	const cJSON *strItem = NULL;
	const cJSON *strlenItem = NULL;

	int8_t int8Value;
	int16_t int16Value;
	int32_t int32Value;
	char strValue[SKB_MSG_MAX_LEN];

	int header_arr_len;
	int body_arr_len;
	int header_size;

	char sndmsg[SKB_MSG_MAX_LEN];

	int i, msg_idx = 0;

	if (chnl_idx < 0 || chnl_idx > 2) {
		ux_log(UXL_MAJ, "Wrong chnl_idx : %d", chnl_idx);
		return -1;
	}

	if (ipmssim_proc_get_json_from_file(fname, &json) < 0) {
		return -1;
	}

	header = cJSON_GetObjectItemCaseSensitive(json, "header");
	if (!cJSON_IsArray(header)) {
		ux_log(UXL_MAJ, "Fail to get returnCode JSON key");
		return -1;
	}
	header_arr_len = cJSON_GetArraySize(header);
	ux_log(UXL_INFO, "header arr len : %d", header_arr_len);

	for (i = 0; i < header_arr_len; i++) {
		item = cJSON_GetArrayItem(header, i);
		strItem = cJSON_GetObjectItemCaseSensitive(item, "t");
		if (!cJSON_IsString(strItem) || (strItem->valuestring == NULL)) {
			ux_log(UXL_MAJ, "Fail to get t JSON key index : %d", i);
			return -1;	
		} 
		if (strcmp(strItem->valuestring, "int8_t") == 0) {
			intItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsNumber(intItem)) {
				ux_log(UXL_MAJ, "Fail to get intItem JSON key");
				return -1;
			}
			int8Value = intItem->valueint;
			memcpy(&sndmsg[msg_idx], &int8Value, sizeof(int8_t));
			msg_idx += sizeof(int8_t);
		} else if (strcmp(strItem->valuestring, "int16_t") == 0) {
			intItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsNumber(intItem)) {
				ux_log(UXL_MAJ, "Fail to get intItem JSON key");
				return -1;
			}
			int16Value = intItem->valueint;
			int16Value = htons(int16Value);
			memcpy(&sndmsg[msg_idx], &int16Value, sizeof(int16_t));
			msg_idx += sizeof(int16_t);
		} else if (strcmp(strItem->valuestring, "int32_t") == 0) {
			intItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsNumber(intItem)) {
				ux_log(UXL_MAJ, "Fail to get intItem JSON key");
				return -1;
			}
			int32Value = intItem->valueint;
			int32Value = htonl(int32Value);
			memcpy(&sndmsg[msg_idx], &int32Value, sizeof(int32_t));
			msg_idx += sizeof(int32_t);
		} else if (strcmp(strItem->valuestring, "string") == 0) {
			strItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsString(strItem) || (strItem->valuestring == NULL)) {
				ux_log(UXL_MAJ, "Fail to get strItem JSON key index : %d", i);
				return -1;	
			} 

			strlenItem = cJSON_GetObjectItemCaseSensitive(item, "l");
			if (!cJSON_IsNumber(strlenItem)) {
				ux_log(UXL_MAJ, "Fail to get strlenItem JSON key");
				return -1;
			}
			strncpy(strValue, strItem->valuestring, strlenItem->valueint);
			memcpy(&sndmsg[msg_idx], &strValue, strlenItem->valueint);
			msg_idx += strlenItem->valueint;
		} else {
			ux_log(UXL_MAJ, "Fail to parsing t : %s", strItem->valuestring);
		}
	}
	header_size = msg_idx;

	body = cJSON_GetObjectItemCaseSensitive(json, "body");
	if (!cJSON_IsArray(body)) {
		ux_log(UXL_MAJ, "Fail to get returnCode JSON key");
		return -1;
	}
	body_arr_len = cJSON_GetArraySize(body);
	ux_log(UXL_INFO, "body arr len : %d", body_arr_len);

	for (i = 0; i < body_arr_len; i++) {
		item = cJSON_GetArrayItem(body, i);
		strItem = cJSON_GetObjectItemCaseSensitive(item, "t");
		if (!cJSON_IsString(strItem) || (strItem->valuestring == NULL)) {
			ux_log(UXL_MAJ, "Fail to get t JSON key index : %d", i);
			return -1;	
		} 
		if (strcmp(strItem->valuestring, "int8_t") == 0) {
			intItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsNumber(intItem)) {
				ux_log(UXL_MAJ, "Fail to get intItem JSON key");
				return -1;
			}
			int8Value = intItem->valueint;
			memcpy(&sndmsg[msg_idx], &int8Value, sizeof(int8_t));
			msg_idx += sizeof(int8_t);
		} else if (strcmp(strItem->valuestring, "int16_t") == 0) {
			intItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsNumber(intItem)) {
				ux_log(UXL_MAJ, "Fail to get intItem JSON key");
				return -1;
			}
			int16Value = intItem->valueint;
			int16Value = htons(int16Value);
			memcpy(&sndmsg[msg_idx], &int16Value, sizeof(int16_t));
			msg_idx += sizeof(int16_t);
		} else if (strcmp(strItem->valuestring, "int32_t") == 0) {
			intItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsNumber(intItem)) {
				ux_log(UXL_MAJ, "Fail to get intItem JSON key");
				return -1;
			}
			int32Value = intItem->valueint;
			int32Value = htonl(int32Value);
			memcpy(&sndmsg[msg_idx], &int32Value, sizeof(int32_t));
			msg_idx += sizeof(int32_t);
		} else if (strcmp(strItem->valuestring, "string") == 0) {
			strItem = cJSON_GetObjectItemCaseSensitive(item, "v");
			if (!cJSON_IsString(strItem) || (strItem->valuestring == NULL)) {
				ux_log(UXL_MAJ, "Fail to get strItem JSON key index : %d", i);
				return -1;	
			} 
			strlenItem = cJSON_GetObjectItemCaseSensitive(item, "l");
			if (!cJSON_IsNumber(strlenItem)) {
				ux_log(UXL_MAJ, "Fail to get strlenItem JSON key");
				return -1;
			}
			strncpy(strValue, strItem->valuestring, strlenItem->valueint);
			memcpy(&sndmsg[msg_idx], &strValue, strlenItem->valueint);
			msg_idx += strlenItem->valueint;
		} else {
			ux_log(UXL_MAJ, "Fail to parsing t : %s", strItem->valuestring);
		}
	}
	ux_log(UXL_MAJ, "sndmsg len : %d , header_len : %d, body_len : %d", msg_idx, header_size, msg_idx - header_size);

	return ipmssim_tcpip_write2(chnl_idx, sndmsg, msg_idx );
}