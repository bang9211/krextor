#include "skb_msg.h"
 
////////////////////////////////////////////////////////////////////////////////////
// functions for skb_msg_t
////////////////////////////////////////////////////////////////////////////////////

void skb_msg_init(int heartbeat_display) {
	srand(time(NULL));
	create_skb_map();
	_heartbeat_display = heartbeat_display;
}

void skb_msg_end() {
	destroy_skb_map();
}

/**
 * @brief DBIF messageID를 eIPMS messageID로 바꾼다.
 * @param dbif_msgID DBIF message ID
 * @return eIPMS message ID, except -1
 */
int skb_msg_cvt_to_skb_msg_id(int dbif_msgID) {
	switch (dbif_msgID) {
		case DBIF_CALL_START_REQUEST:
			return START_REQUEST;
		case DBIF_CALL_STOP_REQUEST:
			return STOP_REQUEST;
		case DBIF_CALL_START_RECORDING_REQUEST:
			return START_RECORDING_REQUEST;
		case DBIF_CALL_STOP_RECORDING_REQUEST:
			return STOP_RECORDING_REQUEST;
		case DBIF_CALL_START_RESPONSE:
			return START_RESPONSE;
		case DBIF_CALL_STOP_RESPONSE:
			return STOP_RESPONSE;
		case DBIF_CALL_START_RECORDING_RESPONSE:
			return START_RECORDING_RESPONSE;
		case DBIF_CALL_STOP_RECORDING_RESPONSE:
			return STOP_RECORDING_RESPONSE;
		case DBIF_CALL_SERVICE_STATUS_REPORT:
			return SERVICE_STATUS_REPORT;
		case DBIF_CALL_END_REPORT:
			return STOP_REPORT;
		case DBIF_RECORDING_START_REQUEST:
			return START_REQUEST;
		case DBIF_RECORDING_STOP_REQUEST:
			return STOP_REQUEST;
		case DBIF_RECORDING_START_RESPONSE:
			return STOP_REQUEST;
		case DBIF_RECORDING_STOP_RESPONSE:
			return STOP_RESPONSE;
		case DBIF_RECORDING_CALL_SERVICE_STATUS_REPORT:
			return SERVICE_STATUS_REPORT;
		case DBIF_RECORDING_CALL_END_REPORT:
			return STOP_REPORT;
		case DBIF_CONFERENCE_START_REQUEST:
			return START_CONFERENCE_REQUEST;
		case DBIF_CONFERENCE_STOP_REQUEST:
			return STOP_CONFERENCE_REQUEST;
		case DBIF_ADD_PARTY_REQUEST:
			return ADD_PARTY_REQUEST;
		case DBIF_REMOVE_PARTY_REQUEST:
			return REMOVE_PARTY_REQUEST;
		case DBIF_CANCEL_PARTY_REQUEST:
			return CANCEL_PARTY_REQUEST;
		case DBIF_CHANGE_PARTY_MEDIA_REQUEST:
			return CHANGE_PARTY_MEDIA_REQUEST;
		case DBIF_PLAY_MENT_REQUEST:
			return PLAY_MENT_REQUEST;
		case DBIF_GET_NUMBER_OF_PARTY_REQUEST:
			return GET_NUMBER_OF_PARTY_REQUEST;
		case DBIF_GET_PARTY_STATUS_REQUEST:
			return GET_PARTY_STATUS_REQUEST;
		case DBIF_CONFERENCE_START_RESPONSE:
			return START_CONFERENCE_RESPONSE;
		case DBIF_CONFERENCE_STOP_RESPONSE:
			return STOP_CONFERENCE_RESPONSE;
		case DBIF_ADD_PARTY_RESPONSE:
			return ADD_PARTY_RESPONSE;
		case DBIF_REMOVE_PARTY_RESPONSE:
			return REMOVE_PARTY_RESPONSE;
		case DBIF_CANCEL_PARTY_RESPONSE:
			return CANCEL_PARTY_RESPONSE;
		case DBIF_CHANGE_PARTY_MEDIA_RESPONSE:
			return CHANGE_PARTY_MEDIA_RESPONSE;
		case DBIF_PLAY_MENT_RESPONSE:
			return PLAY_MENT_RESPONSE;
		case DBIF_GET_NUMBER_OF_PARTY_RESPONSE:
			return GET_NUMBER_OF_PARTY_RESPONSE;
		case DBIF_GET_PARTY_STATUS_RESPONSE:
			return GET_PARTY_STATUS_RESPONSE;
		case DBIF_ADD_PARTY_REPORT:
			return ADD_PARTY_REPORT;
		case DBIF_REMOVE_PARTY_REPORT:
			return REMOVE_PARTY_REPORT;
		case DBIF_CHANGE_PARTY_MEDIA_REPORT:
			return CHANGE_PARTY_MEDIA_REPORT;
		case DBIF_CLOSE_CONF_REPORT:
			return STOP_CONFERENCE_REPORT;
		default:
			ux_log(UXL_CRT, "unsupported dbif msg id : %d", dbif_msgID);
			return -1;
	}
}

/**
 * @brief eIPMS messageID를 DBIF messageID로 바꾼다.
 * @param msgID eIPMS message ID
 * @return DBIF message ID, except -1 
 */
int skb_msg_cvt_to_dbif_msg_id(int chnl_idx, int msgID) {
	switch (chnl_idx) {
		case TCP_CHANNEL_CALL:
			switch (msgID) {
				case START_REQUEST:
					return DBIF_CALL_START_REQUEST;
				case STOP_REQUEST:
					return DBIF_CALL_STOP_REQUEST;
				case START_RECORDING_REQUEST:
					return DBIF_CALL_START_RECORDING_REQUEST;
				case STOP_RECORDING_REQUEST:
					return DBIF_CALL_STOP_RECORDING_REQUEST;
				case START_RESPONSE:
					return DBIF_CALL_START_RESPONSE;
				case STOP_RESPONSE:
					return DBIF_CALL_STOP_RESPONSE;
				case START_RECORDING_RESPONSE:
					return DBIF_CALL_START_RECORDING_RESPONSE;
				case STOP_RECORDING_RESPONSE:
					return DBIF_CALL_STOP_RECORDING_RESPONSE;
				case SERVICE_STATUS_REPORT:
					return DBIF_CALL_SERVICE_STATUS_REPORT;
				case STOP_REPORT:
					return DBIF_CALL_END_REPORT;
				case BINDING_REQUEST:
				case BINDING_RESPONSE:
				case HEARTBEAT_REQUEST:
				case HEARTBEAT_RESPONSE:
					return NONE_DBIF_MESSAGE;
				default:
					ux_log(UXL_CRT, "unsupported message id : %#010x", msgID);
					return -1;
			}
		case TCP_CHANNEL_RECORDING:
			switch (msgID) {
				case START_REQUEST:
					return DBIF_RECORDING_START_REQUEST;
				case STOP_REQUEST:
					return DBIF_RECORDING_STOP_REQUEST;
				case START_RESPONSE:
					return DBIF_RECORDING_START_RESPONSE;
				case STOP_RESPONSE:
					return DBIF_RECORDING_STOP_RESPONSE;
				case SERVICE_STATUS_REPORT:
					return DBIF_RECORDING_CALL_SERVICE_STATUS_REPORT;
				case STOP_REPORT:
					return DBIF_RECORDING_CALL_END_REPORT;
				case BINDING_REQUEST:
				case BINDING_RESPONSE:
				case HEARTBEAT_REQUEST:
				case HEARTBEAT_RESPONSE:
					return NONE_DBIF_MESSAGE;
				default:
					ux_log(UXL_CRT, "unsupported message id : %#010x", msgID);
					return -1;
			}
		case TCP_CHANNEL_CONFERENCE:
			switch (msgID) {
				case START_CONFERENCE_REQUEST:
					return DBIF_CONFERENCE_START_REQUEST;
				case STOP_CONFERENCE_REQUEST:
					return DBIF_CONFERENCE_STOP_REQUEST;
				case ADD_PARTY_REQUEST:
					return DBIF_ADD_PARTY_REQUEST;
				case REMOVE_PARTY_REQUEST:
					return DBIF_REMOVE_PARTY_REQUEST;
				case CANCEL_PARTY_REQUEST:
					return DBIF_CANCEL_PARTY_REQUEST;
				case CHANGE_PARTY_MEDIA_REQUEST:
					return DBIF_CHANGE_PARTY_MEDIA_REQUEST;
				case PLAY_MENT_REQUEST:
					return DBIF_PLAY_MENT_REQUEST;
				case GET_NUMBER_OF_PARTY_REQUEST:
					return DBIF_GET_NUMBER_OF_PARTY_REQUEST;
				case GET_PARTY_STATUS_REQUEST:
					return DBIF_GET_PARTY_STATUS_REQUEST;
				case START_CONFERENCE_RESPONSE:
					return DBIF_CONFERENCE_START_RESPONSE;
				case STOP_CONFERENCE_RESPONSE:
					return DBIF_CONFERENCE_STOP_RESPONSE;
				case ADD_PARTY_RESPONSE:
					return DBIF_ADD_PARTY_RESPONSE;
				case REMOVE_PARTY_RESPONSE:
					return DBIF_REMOVE_PARTY_RESPONSE;
				case CANCEL_PARTY_RESPONSE:
					return DBIF_CANCEL_PARTY_RESPONSE;
				case CHANGE_PARTY_MEDIA_RESPONSE:
					return DBIF_CHANGE_PARTY_MEDIA_RESPONSE;
				case PLAY_MENT_RESPONSE:
					return DBIF_PLAY_MENT_RESPONSE;
				case GET_NUMBER_OF_PARTY_RESPONSE:
					return DBIF_GET_NUMBER_OF_PARTY_RESPONSE;
				case GET_PARTY_STATUS_RESPONSE:
					return DBIF_GET_PARTY_STATUS_RESPONSE;
				case ADD_PARTY_REPORT:
					return DBIF_ADD_PARTY_REPORT;
				case REMOVE_PARTY_REPORT:
					return DBIF_REMOVE_PARTY_REPORT;
				case CHANGE_PARTY_MEDIA_REPORT:
					return DBIF_CHANGE_PARTY_MEDIA_REPORT;
				case STOP_CONFERENCE_REPORT:
					return DBIF_CLOSE_CONF_REPORT;
				case BINDING_REQUEST:
				case BINDING_RESPONSE:
				case HEARTBEAT_REQUEST:
				case HEARTBEAT_RESPONSE:
					return NONE_DBIF_MESSAGE;
				default:
					ux_log(UXL_CRT, "unsupported message id : %#010x", msgID);
					return -1;
			}
		default:
			ux_log(UXL_CRT, "unsupported chnl_idx : %d", chnl_idx);
			return -1;
	}
}

/**
 * @brief network eIPMS 메시지의 값들을 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton(skb_msg_t *msg, int dbif_msgID)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;

	//clicktocall
    clicktocall_start_req_tcp_t *clicktocall_start_req;

	clicktocall_start_rsp_tcp_t *clicktocall_start_rsp;
	clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp;
	clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp;
	clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp;

	clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt;
	clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt;

	//clicktocallrecording
    clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req;

	clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp;
	clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp;

	clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt;
	clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt;

	//clicktoconference
    clicktoconference_start_req_tcp_t *clicktoconference_start_req;
    clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req;

    clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp;
    clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp;
    clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp;
    clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp;
    // clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp;
    clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp;
    clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp;
    clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp;
    clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp;
    clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp;

    clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt;
    clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt;
    clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt;
    // clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt;
    clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt;

    header = &msg->header;
	
    switch(dbif_msgID) {
	//clicktocall
    case DBIF_CALL_START_REQUEST:
		clicktocall_start_req = (clicktocall_start_req_tcp_t *) msg->body;
        clicktocall_start_req->waitingMentID = htons(clicktocall_start_req->waitingMentID);
        clicktocall_start_req->callMentID = htons(clicktocall_start_req->callMentID);
        clicktocall_start_req->filler2 = htons(clicktocall_start_req->filler2);
        break;
    case DBIF_CALL_STOP_REQUEST:				//처리 필요 없음
        break;
    case DBIF_CALL_START_RECORDING_REQUEST:		//처리 필요 없음
        break;
    case DBIF_CALL_STOP_RECORDING_REQUEST:		//처리 필요 없음
        break;
	case DBIF_CALL_START_RESPONSE:
		clicktocall_start_rsp = (clicktocall_start_rsp_tcp_t *) msg->body;
        clicktocall_start_rsp->resultCode = htonl(clicktocall_start_rsp->resultCode);
        clicktocall_start_rsp->filler2 = htons(clicktocall_start_rsp->filler2);
		break;
	case DBIF_CALL_STOP_RESPONSE:
		clicktocall_stop_rsp = (clicktocall_stop_rsp_tcp_t *) msg->body;
        clicktocall_stop_rsp->resultCode = htonl(clicktocall_stop_rsp->resultCode);
		break;
	case DBIF_CALL_START_RECORDING_RESPONSE:
		clicktocall_startrecording_rsp = (clicktocall_startrecording_rsp_tcp_t *) msg->body;
        clicktocall_startrecording_rsp->resultCode = htonl(clicktocall_startrecording_rsp->resultCode);
		break;
	case DBIF_CALL_STOP_RECORDING_RESPONSE:
		clicktocall_stoprecording_rsp = (clicktocall_stoprecording_rsp_tcp_t *) msg->body;
        clicktocall_stoprecording_rsp->resultCode = htonl(clicktocall_stoprecording_rsp->resultCode);
		break;
	case DBIF_CALL_SERVICE_STATUS_REPORT:
		clicktocall_service_status_rpt = (clicktocall_service_status_rpt_tcp_t *) msg->body;
        clicktocall_service_status_rpt->status = htonl(clicktocall_service_status_rpt->status);
		break;
	case DBIF_CALL_END_REPORT:
		clicktocall_stop_rpt = (clicktocall_stop_rpt_tcp_t *) msg->body;
        clicktocall_stop_rpt->returnCode = htonl(clicktocall_stop_rpt->returnCode);
        clicktocall_stop_rpt->isRecorded = htonl(clicktocall_stop_rpt->isRecorded);
		break;
	//clicktocallrecording
	case DBIF_RECORDING_START_REQUEST:
		clicktocallrecording_start_req = (clicktocallrecording_start_req_tcp_t *) msg->body;
        clicktocallrecording_start_req->filler2 = htons(clicktocallrecording_start_req->filler2);
		break;
	case DBIF_RECORDING_STOP_REQUEST:		//처리 필요 없음
		break;
	case DBIF_RECORDING_START_RESPONSE:
		clicktocallrecording_start_rsp = (clicktocallrecording_start_rsp_tcp_t *) msg->body;
        clicktocallrecording_start_rsp->resultCode = htonl(clicktocallrecording_start_rsp->resultCode);
		break;
	case DBIF_RECORDING_STOP_RESPONSE:
		clicktocallrecording_stop_rsp = (clicktocallrecording_stop_rsp_tcp_t *) msg->body;
        clicktocallrecording_stop_rsp->resultCode = htonl(clicktocallrecording_stop_rsp->resultCode);
		break;
	case DBIF_RECORDING_CALL_SERVICE_STATUS_REPORT:
		clicktocallrecording_service_status_rpt = (clicktocallrecording_service_status_rpt_tcp_t *) msg->body;
        clicktocallrecording_service_status_rpt->status = htonl(clicktocallrecording_service_status_rpt->status);
		break;
	case DBIF_RECORDING_CALL_END_REPORT:
		clicktocallrecording_stop_rpt = (clicktocallrecording_stop_rpt_tcp_t *) msg->body;
        clicktocallrecording_stop_rpt->returnCode = htonl(clicktocallrecording_stop_rpt->returnCode);
        clicktocallrecording_stop_rpt->isRecorded = htonl(clicktocallrecording_stop_rpt->isRecorded);
		break;
	//clicktoconference
	case DBIF_CONFERENCE_START_REQUEST:
		clicktoconference_start_req = (clicktoconference_start_req_tcp_t *) msg->body;
        clicktoconference_start_req->callingCategoryType = htonl(clicktoconference_start_req->callingCategoryType);
		break;
	case DBIF_CONFERENCE_STOP_REQUEST:			//처리 필요 없음
		break;
	case DBIF_ADD_PARTY_REQUEST:				//처리 필요 없음
		break;
	case DBIF_REMOVE_PARTY_REQUEST:				//처리 필요 없음
		break;
	case DBIF_CHANGE_PARTY_MEDIA_REQUEST:
		clicktoconference_change_party_media_req = (clicktoconference_change_party_media_req_tcp_t *) msg->body;
        clicktoconference_change_party_media_req->mediaType = htonl(clicktoconference_change_party_media_req->mediaType);
		break;
	case DBIF_PLAY_MENT_REQUEST:				//처리 필요 없음
		break;
	case DBIF_GET_NUMBER_OF_PARTY_REQUEST:		//처리 필요 없음
		break;
	case DBIF_GET_PARTY_STATUS_REQUEST:			//처리 필요 없음
		break;
	case DBIF_CANCEL_PARTY_REQUEST:				//처리 필요 없음
		break;
	case DBIF_CONFERENCE_START_RESPONSE:
		clicktoconference_start_rsp = (clicktoconference_start_rsp_tcp_t *) msg->body;
        clicktoconference_start_rsp->resultCode = htonl(clicktoconference_start_rsp->resultCode);
        clicktoconference_start_rsp->filler1 = htons(clicktoconference_start_rsp->filler1);
        clicktoconference_start_rsp->filler2 = htons(clicktoconference_start_rsp->filler2);
        clicktoconference_start_rsp->recordingFileID = htonl(clicktoconference_start_rsp->recordingFileID);
        clicktoconference_start_rsp->serviceMentID = htonl(clicktoconference_start_rsp->serviceMentID);
        clicktoconference_start_rsp->callingCategoryType = htonl(clicktoconference_start_rsp->callingCategoryType);
		break;
	case DBIF_CONFERENCE_STOP_RESPONSE:
		clicktoconference_stop_rsp = (clicktoconference_stop_rsp_tcp_t *) msg->body;
        clicktoconference_stop_rsp->resultCode = htonl(clicktoconference_stop_rsp->resultCode);
        clicktoconference_stop_rsp->recordingFileID = htonl(clicktoconference_stop_rsp->recordingFileID);
		break;
	case DBIF_ADD_PARTY_RESPONSE:
		clicktoconference_add_party_rsp = (clicktoconference_add_party_rsp_tcp_t *) msg->body;
        clicktoconference_add_party_rsp->resultCode = htonl(clicktoconference_add_party_rsp->resultCode);
		break;
	case DBIF_REMOVE_PARTY_RESPONSE:
		clicktoconference_remove_party_rsp = (clicktoconference_remove_party_rsp_tcp_t *) msg->body;
        clicktoconference_remove_party_rsp->resultCode = htonl(clicktoconference_remove_party_rsp->resultCode);
		break;
	case DBIF_CHANGE_PARTY_MEDIA_RESPONSE:
		clicktoconference_change_party_media_rsp = (clicktoconference_change_party_media_rsp_tcp_t *) msg->body;
        clicktoconference_change_party_media_rsp->resultCode = htonl(clicktoconference_change_party_media_rsp->resultCode);
        clicktoconference_change_party_media_rsp->mediaType = htonl(clicktoconference_change_party_media_rsp->mediaType);
		break;
	case DBIF_PLAY_MENT_RESPONSE:
		clicktoconference_play_ment_rsp = (clicktoconference_play_ment_rsp_tcp_t *) msg->body;
        clicktoconference_play_ment_rsp->resultCode = htonl(clicktoconference_play_ment_rsp->resultCode);
		break;
	case DBIF_GET_NUMBER_OF_PARTY_RESPONSE:
		clicktoconference_get_number_of_party_rsp = (clicktoconference_get_number_of_party_rsp_tcp_t *) msg->body;
        clicktoconference_get_number_of_party_rsp->resultCode = htonl(clicktoconference_get_number_of_party_rsp->resultCode);
        clicktoconference_get_number_of_party_rsp->number = htonl(clicktoconference_get_number_of_party_rsp->number);
		break;
	case DBIF_GET_PARTY_STATUS_RESPONSE:
		clicktoconference_get_party_status_rsp = (clicktoconference_get_party_status_rsp_tcp_t *) msg->body;
        clicktoconference_get_party_status_rsp->resultCode = htonl(clicktoconference_get_party_status_rsp->resultCode);
		break;
	case DBIF_CANCEL_PARTY_RESPONSE:
		clicktoconference_cancel_party_rsp = (clicktoconference_cancel_party_rsp_tcp_t *) msg->body;
        clicktoconference_cancel_party_rsp->resultCode = htonl(clicktoconference_cancel_party_rsp->resultCode);
		break;
	case DBIF_ADD_PARTY_REPORT:
		clicktoconference_add_party_rpt = (clicktoconference_add_party_rpt_tcp_t *) msg->body;
        clicktoconference_add_party_rpt->resultCode = htonl(clicktoconference_add_party_rpt->resultCode);
		break;
	case DBIF_REMOVE_PARTY_REPORT:
		clicktoconference_remove_party_rpt = (clicktoconference_remove_party_rpt_tcp_t *) msg->body;
        clicktoconference_remove_party_rpt->resultCode = htonl(clicktoconference_remove_party_rpt->resultCode);
		break;
	case DBIF_CHANGE_PARTY_MEDIA_REPORT:
		clicktoconference_change_party_media_rpt = (clicktoconference_change_party_media_rpt_tcp_t *) msg->body;
        clicktoconference_change_party_media_rpt->resultCode = htonl(clicktoconference_change_party_media_rpt->resultCode);
        clicktoconference_change_party_media_rpt->mediaType = htonl(clicktoconference_change_party_media_rpt->mediaType);
		break;
	case DBIF_CLOSE_CONF_REPORT:
		clicktoconference_stop_rpt = (clicktoconference_stop_rpt_tcp_t *) msg->body;
        clicktoconference_stop_rpt->returnCode = htonl(clicktoconference_stop_rpt->returnCode);
		clicktoconference_stop_rpt->isRecorded = htonl(clicktoconference_stop_rpt->isRecorded);
		break;
	default:
		ux_log( UXL_INFO, "unknown DBIF Msg Id : [%d]\n", dbif_msgID);
		break;
    }

	/* skb header convert */
	header->length = htons(header->length);
	header->messageID = htonl(header->messageID);
	header->requestID = htonl(header->requestID);
#endif

	return UX_SUCCESS;
}

/**
 * @brief network eIPMS 메시지의 header를 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton2(skb_msg_t *msg)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;
    header = &msg->header;

	/* skb header convert */
	header->length = htons(header->length);
	header->messageID = htonl(header->messageID);
	header->requestID = htonl(header->requestID);
#endif

	return UX_SUCCESS;
}

/**
 * @brief network eIPMS binding response 메시지를 network byte ordering으로 바꾼다.
 * @param msg network eIPMS message
 * @return 실행 결과 
 */
int skb_msg_cvt_order_hton3(skb_msg_t *msg, int chnl_idx)
{
	if (msg == NULL) return -1;

#if !UX_BIGENDIAN
    skb_header_t *header;
	clicktocall_binding_rsp_tcp_t *clicktocall_binding_rsp;
	clicktocallrecording_binding_rsp_tcp_t *clicktocallrecording_binding_rsp;
	clicktoconference_binding_rsp_tcp_t *clicktoconference_binding_rsp;

	switch(chnl_idx) {
	case TCP_CHANNEL_CALL:
		clicktocall_binding_rsp = (clicktocall_binding_rsp_tcp_t *) msg->body;
        clicktocall_binding_rsp->resultCode = htonl(clicktocall_binding_rsp->resultCode);
		break;
	case TCP_CHANNEL_RECORDING:
		clicktocallrecording_binding_rsp = (clicktocallrecording_binding_rsp_tcp_t *) msg->body;
        clicktocallrecording_binding_rsp->resultCode = htonl(clicktocallrecording_binding_rsp->resultCode);
		break;
	case TCP_CHANNEL_CONFERENCE:
		clicktoconference_binding_rsp = (clicktoconference_binding_rsp_tcp_t *) msg->body;
        clicktoconference_binding_rsp->resultCode = htonl(clicktoconference_binding_rsp->resultCode);
		break;
	default:
		break;
	}

    header = &msg->header;

	/* skb header convert */
	header->length = htons(header->length);
	header->messageID = htonl(header->messageID);
	header->requestID = htonl(header->requestID);
#endif

	return UX_SUCCESS;
}


/**
 * @brief network eIPMS 메시지의 값들을 host byte ordering으로 바꾸고, 실행 결과와 DBIF msgId를 반환한다.
 * @param msg network eIPMS message
 * @return 실행 결과, msgId(response의 경우 NULL)
 */
int skb_msg_cvt_order_ntoh(skb_msg_t *msg, int chnIdx, int *msgId)
{
#if !UX_BIGENDIAN
    skb_header_t *header;

	//clicktocall
	clicktocall_start_req_tcp_t *clicktocall_start_req;							//for test

	clicktocall_binding_rsp_tcp_t *clicktocall_binding_rsp;
    clicktocall_start_rsp_tcp_t *clicktocall_start_rsp;
	clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp;
	clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp;
	clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp;
	clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp;

	clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt;
	clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt;
	clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt;
    clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt;

	//clicktocallrecording
	clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req;

	clicktocallrecording_binding_rsp_tcp_t *clicktocallrecording_binding_rsp;
	clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp;
	clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp;
	clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp;

	clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt;
	clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt;
	clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt;

	//clicktoconference
	clicktoconference_start_req_tcp_t *clicktoconference_start_req;
	clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req;

	clicktoconference_binding_rsp_tcp_t *clicktoconference_binding_rsp;
	clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp;
	clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp;
	clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp;
	clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp;
	clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp;
	clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp;
	clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp;
	clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp;
	clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp;
	clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp;

	clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt;
	clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt;
	clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt;
	clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt;
	clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt;

	// char headerLog[256];

	if (msg == NULL) return -1;

    header = &msg->header;

	/* skb header convert */
	header->length = ntohs(header->length);
	header->messageID = ntohl(header->messageID);
	header->requestID = ntohl(header->requestID);

	//body 복구
	switch(chnIdx) {
	case TCP_CHANNEL_CALL:
		switch(msg->header.messageID)
		{
			case HEARTBEAT_REQUEST:				//처리 필요 없음
				break;
			case HEARTBEAT_RESPONSE:			//처리 필요 없음
				break;
			case BINDING_REQUEST:				//처리 필요 없음
				break;
			case BINDING_RESPONSE:
				clicktocall_binding_rsp = (clicktocall_binding_rsp_tcp_t *) msg->body;
				clicktocall_binding_rsp->resultCode = ntohl(clicktocall_binding_rsp->resultCode);
				break;
			//받은 메시지가 요청인 경우(simulator case)
			case START_REQUEST:
				clicktocall_start_req = (clicktocall_start_req_tcp_t *) msg->body;
				clicktocall_start_req->waitingMentID = ntohs(clicktocall_start_req->waitingMentID);
				clicktocall_start_req->callMentID = ntohs(clicktocall_start_req->callMentID);
				clicktocall_start_req->filler2 = ntohs(clicktocall_start_req->filler2);
				break;
			case STOP_REQUEST:					//처리 필요 없음
				break;
			case START_RECORDING_REQUEST:		//처리 필요 없음
				break;
			case STOP_RECORDING_REQUEST:		//처리 필요 없음
				break;
			//받은 메시지가 응답, 보고의 경우(normal case)
			case START_RESPONSE:
				clicktocall_start_rsp = (clicktocall_start_rsp_tcp_t *) msg->body;
				clicktocall_start_rsp->resultCode = ntohl(clicktocall_start_rsp->resultCode);
				clicktocall_start_rsp->filler2 = ntohs(clicktocall_start_rsp->filler2);
				break;
			case STOP_RESPONSE:
				clicktocall_stop_rsp = (clicktocall_stop_rsp_tcp_t *) msg->body;
				clicktocall_stop_rsp->resultCode = ntohl(clicktocall_stop_rsp->resultCode);
				break;
			case START_RECORDING_RESPONSE:
				clicktocall_startrecording_rsp = (clicktocall_startrecording_rsp_tcp_t *) msg->body;
				clicktocall_startrecording_rsp->resultCode = ntohl(clicktocall_startrecording_rsp->resultCode);
				break;
			case STOP_RECORDING_RESPONSE:
				clicktocall_stoprecording_rsp = (clicktocall_stoprecording_rsp_tcp_t *) msg->body;
				clicktocall_stoprecording_rsp->resultCode = ntohl(clicktocall_stoprecording_rsp->resultCode);
				break;
			case SERVICE_STATUS_RESPONSE:
				clicktocall_service_status_rsp = (clicktocall_service_status_rsp_tcp_t *) msg->body;
				clicktocall_service_status_rsp->resultCode = ntohl(clicktocall_service_status_rsp->resultCode);
				clicktocall_service_status_rsp->status = ntohl(clicktocall_service_status_rsp->status);
				break;
			case STOP_REPORT:
				clicktocallrecording_stop_rpt = (clicktocallrecording_stop_rpt_tcp_t *) msg->body;
				clicktocall_stop_rpt->returnCode = ntohl(clicktocall_stop_rpt->returnCode);
				clicktocall_stop_rpt->isRecorded = ntohl(clicktocall_stop_rpt->isRecorded);
				break;
			case START_RECORDING_REPORT:
				clicktocall_startrecording_rpt = (clicktocall_startrecording_rpt_tcp_t *) msg->body;
				clicktocall_startrecording_rpt->resultCode = ntohl(clicktocall_startrecording_rpt->resultCode);
				break;
			case STOP_RECORDING_REPORT:
				clicktocall_stoprecording_rpt = (clicktocall_stoprecording_rpt_tcp_t *) msg->body;
				clicktocall_stoprecording_rpt->resultCode = ntohl(clicktocall_stoprecording_rpt->resultCode);
				break;
			case SERVICE_STATUS_REPORT:
				clicktocall_service_status_rpt = (clicktocall_service_status_rpt_tcp_t *) msg->body;
				clicktocall_service_status_rpt->status = ntohl(clicktocall_service_status_rpt->status);
				break;
			default:
				ux_log(UXL_CRT, "unsupported message id : %#010x", msg->header.messageID);
				// skb_msg_get_header_display(&msg->header, headerLog);
				// ux_log(UXL_CRT, "unsupported message id : %s", headerLog);
				return -1;
				// break;
		}
		break;
	case TCP_CHANNEL_RECORDING:
		switch(msg->header.messageID)
		{
			case HEARTBEAT_REQUEST:				//처리 필요 없음
				break;
			case HEARTBEAT_RESPONSE:			//처리 필요 없음
				break;
			case BINDING_REQUEST:				//처리 필요 없음
				break;
			case BINDING_RESPONSE:
				clicktocallrecording_binding_rsp = (clicktocallrecording_binding_rsp_tcp_t *) msg->body;
				clicktocallrecording_binding_rsp->resultCode = ntohl(clicktocallrecording_binding_rsp->resultCode);
				break;
			//받은 메시지가 요청인 경우(simulator case)
			case START_REQUEST:
				clicktocallrecording_start_req = (clicktocallrecording_start_req_tcp_t *) msg->body;
				clicktocallrecording_start_req->filler2 = ntohs(clicktocallrecording_start_req->filler2);
				break;
			case STOP_REQUEST:					//처리 필요 없음
				break;
			case SERVICE_STATUS_REQUEST:		//처리 필요 없음
				break;
			//받은 메시지가 응답, 보고의 경우(normal case)
			case START_RESPONSE:
				clicktocallrecording_start_rsp = (clicktocallrecording_start_rsp_tcp_t *) msg->body;
				clicktocallrecording_start_rsp->resultCode = ntohl(clicktocallrecording_start_rsp->resultCode);
				break;
			case STOP_RESPONSE:
				clicktocallrecording_stop_rsp = (clicktocallrecording_stop_rsp_tcp_t *) msg->body;
				clicktocallrecording_stop_rsp->resultCode = ntohl(clicktocallrecording_stop_rsp->resultCode);
				break;
			case SERVICE_STATUS_RESPONSE:
				clicktocallrecording_service_status_rsp = (clicktocallrecording_service_status_rsp_tcp_t *) msg->body;
				clicktocallrecording_service_status_rsp->resultCode = ntohl(clicktocallrecording_service_status_rsp->resultCode);
				clicktocallrecording_service_status_rsp->status = ntohl(clicktocallrecording_service_status_rsp->status);
				break;
			case START_REPORT:
				clicktocallrecording_start_rpt = (clicktocallrecording_start_rpt_tcp_t *) msg->body;
				clicktocallrecording_start_rpt->resultCode = ntohl(clicktocallrecording_start_rpt->resultCode);
				break;
			case STOP_REPORT:
				clicktocallrecording_stop_rpt = (clicktocallrecording_stop_rpt_tcp_t *) msg->body;
				clicktocallrecording_stop_rpt->returnCode = ntohl(clicktocallrecording_stop_rpt->returnCode);
				clicktocallrecording_stop_rpt->isRecorded = ntohl(clicktocallrecording_stop_rpt->isRecorded);
				break;
			case SERVICE_STATUS_REPORT:
				clicktocallrecording_service_status_rpt = (clicktocallrecording_service_status_rpt_tcp_t *) msg->body;
				clicktocallrecording_service_status_rpt->status = ntohl(clicktocallrecording_service_status_rpt->status);
				break;
			default:
				ux_log(UXL_CRT, "unsupported message id : %#010x", msg->header.messageID)
				return -1;
				// break;
		}
		break;
	case TCP_CHANNEL_CONFERENCE:
		switch(msg->header.messageID)
		{
			case HEARTBEAT_REQUEST:				//처리 필요 없음
				break;
			case HEARTBEAT_RESPONSE:			//처리 필요 없음
				break;
			case BINDING_REQUEST:				//처리 필요 없음
				break;
			case BINDING_RESPONSE:
				clicktoconference_binding_rsp = (clicktoconference_binding_rsp_tcp_t *) msg->body;
				clicktoconference_binding_rsp->resultCode = ntohl(clicktoconference_binding_rsp->resultCode);
				break;
			//받은 메시지가 요청인 경우(simulator case)
			case START_CONFERENCE_REQUEST:
				clicktoconference_start_req = (clicktoconference_start_req_tcp_t *) msg->body;
				clicktoconference_start_req->callingCategoryType = ntohl(clicktoconference_start_req->callingCategoryType);
				break;
			case STOP_CONFERENCE_REQUEST:		//처리 필요 없음
				break;
			case ADD_PARTY_REQUEST:				//처리 필요 없음
				break;
			case REMOVE_PARTY_REQUEST:			//처리 필요 없음
				break;
			case CHANGE_PARTY_MEDIA_REQUEST:
				clicktoconference_change_party_media_req = (clicktoconference_change_party_media_req_tcp_t *) msg->body;
				clicktoconference_change_party_media_req->mediaType = ntohl(clicktoconference_change_party_media_req->mediaType);
				break;
			case CHANGE_OPTION_REQUEST:			//처리 필요 없음
				break;
			case GET_NUMBER_OF_PARTY_REQUEST:	//처리 필요 없음
				break;
			case PLAY_MENT_REQUEST:				//처리 필요 없음
				break;
			case GET_PARTY_STATUS_REQUEST:		//처리 필요 없음
				break;
			case CANCEL_PARTY_REQUEST:			//처리 필요 없음
				break;
			//받은 메시지가 응답, 보고의 경우(normal case)
			case START_CONFERENCE_RESPONSE:
				clicktoconference_start_rsp = (clicktoconference_start_rsp_tcp_t *) msg->body;
				clicktoconference_start_rsp->resultCode = ntohl(clicktoconference_start_rsp->resultCode);
				clicktoconference_start_rsp->filler1 = ntohs(clicktoconference_start_rsp->filler1);
				clicktoconference_start_rsp->filler2 = ntohs(clicktoconference_start_rsp->filler2);
				clicktoconference_start_rsp->recordingFileID = ntohl(clicktoconference_start_rsp->recordingFileID);
				clicktoconference_start_rsp->serviceMentID = ntohl(clicktoconference_start_rsp->serviceMentID);
				clicktoconference_start_rsp->callingCategoryType = ntohl(clicktoconference_start_rsp->callingCategoryType);
				break;
			case STOP_CONFERENCE_RESPONSE:
				clicktoconference_stop_rsp = (clicktoconference_stop_rsp_tcp_t *) msg->body;
				clicktoconference_stop_rsp->resultCode = ntohl(clicktoconference_stop_rsp->resultCode);
				clicktoconference_stop_rsp->recordingFileID = ntohl(clicktoconference_stop_rsp->recordingFileID);
				break;
			case ADD_PARTY_RESPONSE:
				clicktoconference_add_party_rsp = (clicktoconference_add_party_rsp_tcp_t *) msg->body;
				clicktoconference_add_party_rsp->resultCode = ntohl(clicktoconference_add_party_rsp->resultCode);
				break;
			case REMOVE_PARTY_RESPONSE:
				clicktoconference_remove_party_rsp = (clicktoconference_remove_party_rsp_tcp_t *) msg->body;
				clicktoconference_remove_party_rsp->resultCode = ntohl(clicktoconference_remove_party_rsp->resultCode);
				break;
			case CHANGE_PARTY_MEDIA_RESPONSE:
				clicktoconference_change_party_media_rsp = (clicktoconference_change_party_media_rsp_tcp_t *) msg->body;
				clicktoconference_change_party_media_rsp->resultCode = ntohl(clicktoconference_change_party_media_rsp->resultCode);
				clicktoconference_change_party_media_rsp->mediaType = ntohl(clicktoconference_change_party_media_rsp->mediaType);
				break;
			case CHANGE_OPTION_RESPONSE:
				clicktoconference_change_option_rsp = (clicktoconference_change_option_rsp_tcp_t *) msg->body;
				clicktoconference_change_option_rsp->resultCode = ntohl(clicktoconference_change_option_rsp->resultCode);
				break;
			case PLAY_MENT_RESPONSE:
				clicktoconference_play_ment_rsp = (clicktoconference_play_ment_rsp_tcp_t *) msg->body;
				clicktoconference_play_ment_rsp->resultCode = ntohl(clicktoconference_play_ment_rsp->resultCode);
				break;
			case GET_NUMBER_OF_PARTY_RESPONSE:
				clicktoconference_get_number_of_party_rsp = (clicktoconference_get_number_of_party_rsp_tcp_t *) msg->body;
				clicktoconference_get_number_of_party_rsp->resultCode = ntohl(clicktoconference_get_number_of_party_rsp->resultCode);
				clicktoconference_get_number_of_party_rsp->number = ntohl(clicktoconference_get_number_of_party_rsp->number);
				break;
			case GET_PARTY_STATUS_RESPONSE:
				clicktoconference_get_party_status_rsp = (clicktoconference_get_party_status_rsp_tcp_t *) msg->body;
				clicktoconference_get_party_status_rsp->resultCode = ntohl(clicktoconference_get_party_status_rsp->resultCode);
				break;
			case CANCEL_PARTY_RESPONSE:
				clicktoconference_cancel_party_rsp = (clicktoconference_cancel_party_rsp_tcp_t *) msg->body;
				clicktoconference_cancel_party_rsp->resultCode = ntohl(clicktoconference_cancel_party_rsp->resultCode);
				break;
			case ADD_PARTY_REPORT:
				clicktoconference_add_party_rpt = (clicktoconference_add_party_rpt_tcp_t *) msg->body;
				clicktoconference_add_party_rpt->resultCode = ntohl(clicktoconference_add_party_rpt->resultCode);
				break;
			case REMOVE_PARTY_REPORT:
				clicktoconference_remove_party_rpt = (clicktoconference_remove_party_rpt_tcp_t *) msg->body;
				clicktoconference_remove_party_rpt->resultCode = ntohl(clicktoconference_remove_party_rpt->resultCode);
				break;
			case CHANGE_PARTY_MEDIA_REPORT:
				clicktoconference_change_party_media_rpt = (clicktoconference_change_party_media_rpt_tcp_t *) msg->body;
				clicktoconference_change_party_media_rpt->resultCode = ntohl(clicktoconference_change_party_media_rpt->resultCode);
				clicktoconference_change_party_media_rpt->mediaType = ntohl(clicktoconference_change_party_media_rpt->mediaType);
				break;
			case CHANGE_OPTION_REPORT:
				clicktoconference_change_option_rpt = (clicktoconference_change_option_rpt_tcp_t *) msg->body;
				clicktoconference_change_option_rpt->resultCode = ntohl(clicktoconference_change_option_rpt->resultCode);
				break;
			case STOP_CONFERENCE_REPORT:
				clicktoconference_stop_rpt = (clicktoconference_stop_rpt_tcp_t *) msg->body;
				clicktoconference_stop_rpt->returnCode = ntohl(clicktoconference_stop_rpt->returnCode);
				clicktoconference_stop_rpt->isRecorded = ntohl(clicktoconference_stop_rpt->isRecorded);
				break;
			default:
				ux_log(UXL_CRT, "unsupported message id : %#010x", msg->header.messageID)
				return -1;
				// break;
		}
		break;
	default:
		ux_log(UXL_CRT, "unsupported channel index : %d", chnIdx)
		return -1;
	}
#endif
	//DBIF로 보내져야 하는 메시지인 경우, DBIF msgId 설정
	switch(chnIdx) {
		case TCP_CHANNEL_CALL:
			switch(msg->header.messageID) {
			//받은 메시지가 응답, 보고의 경우(normal case)
			case START_RESPONSE:
				*msgId = DBIF_CALL_START_RESPONSE;
				break;
			case STOP_RESPONSE:
				*msgId = DBIF_CALL_STOP_RESPONSE;
				break;
			case START_RECORDING_RESPONSE:
				*msgId = DBIF_CALL_START_RECORDING_RESPONSE;
				break;
			case STOP_RECORDING_RESPONSE:
				*msgId = DBIF_CALL_STOP_RECORDING_RESPONSE;
				break;
			case STOP_REPORT:				//CallEndReport
				*msgId = DBIF_CALL_END_REPORT;
				break;
			case SERVICE_STATUS_REPORT:		//CallServiceStatusReport
				*msgId = DBIF_CALL_SERVICE_STATUS_REPORT;
				break;
			default:
				*msgId = NONE_DBIF_MESSAGE;
				break;
			}
			break;
		case TCP_CHANNEL_RECORDING:
			switch(msg->header.messageID) {
			case START_RECORDING_RESPONSE:
				*msgId = DBIF_RECORDING_START_RESPONSE;
				break;
			case STOP_RECORDING_RESPONSE:
				*msgId = DBIF_RECORDING_STOP_RESPONSE;
				break;
			case SERVICE_STATUS_RESPONSE:
				*msgId = DBIF_RECORDING_CALL_SERVICE_STATUS_REPORT;
				break;
			case SERVICE_STATUS_REPORT:
				*msgId = DBIF_RECORDING_CALL_END_REPORT;
				break;
			default:
				*msgId = NONE_DBIF_MESSAGE;
				break;
			}
			break;
		case TCP_CHANNEL_CONFERENCE:
			switch(msg->header.messageID) {
			case START_CONFERENCE_RESPONSE:
				*msgId = DBIF_CONFERENCE_START_RESPONSE;
				break;
			case STOP_CONFERENCE_RESPONSE:
				*msgId = DBIF_CONFERENCE_STOP_RESPONSE;
				break;
			case ADD_PARTY_RESPONSE:
				*msgId = DBIF_ADD_PARTY_RESPONSE;
				break;
			case REMOVE_PARTY_RESPONSE:
				*msgId = DBIF_REMOVE_PARTY_RESPONSE;
				break;
			case CANCEL_PARTY_RESPONSE:
				*msgId = DBIF_CANCEL_PARTY_RESPONSE;
				break;
			case CHANGE_PARTY_MEDIA_RESPONSE:
				*msgId = DBIF_CHANGE_PARTY_MEDIA_RESPONSE;
				break;
			case PLAY_MENT_RESPONSE:
				*msgId = DBIF_PLAY_MENT_RESPONSE;
				break;
			case GET_NUMBER_OF_PARTY_RESPONSE:
				*msgId = DBIF_GET_NUMBER_OF_PARTY_RESPONSE;
				break;
			case GET_PARTY_STATUS_RESPONSE:
				*msgId = DBIF_GET_PARTY_STATUS_RESPONSE;
				break;
			case ADD_PARTY_REPORT:
				*msgId = DBIF_ADD_PARTY_REPORT;
				break;
			case REMOVE_PARTY_REPORT:
				*msgId = DBIF_REMOVE_PARTY_REPORT;
				break;
			case CHANGE_PARTY_MEDIA_REPORT:
				*msgId = DBIF_CHANGE_PARTY_MEDIA_REPORT;
				break;
			case STOP_CONFERENCE_REPORT:
				*msgId = DBIF_CLOSE_CONF_REPORT;
				break;
			default:
				*msgId = NONE_DBIF_MESSAGE;
				break;
			}
			break;
		default:
			ux_log(UXL_CRT, "unsupported channel index : %d", chnIdx)
			return -1;
	}

	return UX_SUCCESS;
}

/**
 * @brief encoding 한 ipcmsg 를 uxcutor 로 전송한다. 
 * @param msg ipcmsg, message
 * @param tcp tcp pa
 * @param peerkey peer key
 * @return 실행 결과 
 */
int skb_msg_send( skb_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey, int dbif_msgID)
{
	ux_log( UXL_INFO, "= msg_send_skbmsg =");
	int rv, msg_size;
		
	msg_size = msg->header.length; 

	rv = skb_msg_cvt_order_hton(msg, dbif_msgID);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	rv = upa_tcp_send2(tcp, peerkey, msg, msg_size, 1);
	if ( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data.");
		return -1;
	} else {
		ux_log( UXL_INFO, "= sent data. length = %d\n ", msg_size);
		return eUXC_SUCCESS;
	}
}

int check_header(skb_header_t* header) {
	if (header->frameStart0 != (int8_t)0xFE ||
		header->frameStart1 != (int8_t)0xFE ||
		header->version0 != (int8_t)0x00 ||
		header->version1 != (int8_t)0x01) {
		return -1;
	}
	return 0;
}

void skb_msg_make_header(skb_header_t* header, int32_t messageID, int16_t bodySize, int32_t *requestID) {
	int32_t temp;

	header->frameStart0 = 0Xfe;
	header->frameStart1 = 0Xfe;
	header->length = sizeof(skb_header_t) + bodySize;
	header->messageID = messageID;
	if (requestID == NULL) {
		temp = skb_msg_generate_requestID();
		while (uh_rid_is_exist(reqID_Set, temp)) {
			ux_log( UXL_INFO, "generated requestID(%d) is already existed, try to regenerate", temp);
			temp = skb_msg_generate_requestID();
		}
		uh_rid_put(reqID_Set, temp);
		requestID = &temp;
	}
	header->requestID = *requestID;
	header->version0 = 0x00;
	header->version1 = 0x01;
	header->userID = 1;
	header->filler = 0;
}

int skb_msg_make_bind_request(skb_msg_t *skbmsg, int chnl_idx, char *id, char *pw) {
	switch(chnl_idx) {
	case TCP_CHANNEL_CALL:
		skb_msg_process_clicktocall_binding_req(skbmsg, id, pw);
		break;
	case TCP_CHANNEL_RECORDING:
		skb_msg_process_clicktocallrecording_binding_req(skbmsg, id, pw);
		break;
	case TCP_CHANNEL_CONFERENCE:
		skb_msg_process_clicktoconference_binding_req(skbmsg, id, pw);
		break;
	default:
		ux_log(UXL_CRT, "failed to skb_msg_make_bind_request");
		return -1;
	}
	return UX_SUCCESS;
}

void skb_msg_get_header_display(skb_header_t* header, char *log) {
	char temp[SKB_HEADER_DISPLAY_SIZE] = "";
	sprintf(temp,
	"  [frameStart0] 0x%hhX\n"
	"  [frameStart1] 0x%hhX\n"
	"  [length] %d\n"
	"  [messageID] %#010x\n"
	"  [requestID] %d (%x)\n"
	"  [version0] 0x%hhX\n"
	"  [version1] 0x%hhX\n"
	"  [userID] %d\n"
	"  [filler] %d", 
		header->frameStart0, 
		header->frameStart1, 
		header->length, 
		header->messageID, 
		header->requestID, 
		header->requestID,
		header->version0, 
		header->version1, 
		header->userID, 
		header->filler);
	strcat(log, temp);
}

void skb_msg_get_send_header_display(skb_header_t* header, char *log) {
	strcpy(log, "");
	strcat(log, "Sending TCP \nHeader\n");
	skb_msg_get_header_display(header, log);
}

void skb_msg_get_recv_header_display(skb_header_t* header, char *log) {
	strcpy(log, "");
	strcat(log, "Received TCP \nHeader\n");
	skb_msg_get_header_display(header, log);
}

void skb_msg_display_send_header(skb_header_t* header) {
	char log[SKB_HEADER_DISPLAY_SIZE] = "";
	skb_msg_get_send_header_display(header, log);
	ux_log(UXL_INFO, "%s", log);
}

void skb_msg_display_recv_header(skb_header_t* header) {
	char log[SKB_HEADER_DISPLAY_SIZE] = "";
	skb_msg_get_recv_header_display(header, log);
	ux_log(UXL_INFO, "%s", log);
}

void skb_msg_display_send_heartbeat_req(skb_header_t* header, int chnl_idx) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Sending TCP Heartbeat req(%d) :\t%x", chnl_idx, header->requestID);
}

void skb_msg_display_recv_heartbeat_req(skb_header_t* header, int chnl_idx) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Received TCP Heartbeat req(%d) :\t%x", chnl_idx, header->requestID);
}

void skb_msg_display_send_heartbeat_rsp(skb_header_t* header, int chnl_idx) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Sending TCP Heartbeat rsp(%d) :\t%x", chnl_idx, header->requestID);
}

void skb_msg_display_recv_heartbeat_rsp(skb_header_t* header, int chnl_idx) {
	if (_heartbeat_display) ux_log(UXL_INFO, "Received TCP Heartbeat rsp(%d) :\t%x", chnl_idx, header->requestID);
}

int32_t skb_msg_generate_requestID() {
	return getRandomInt32();
}

int32_t getRandomInt32() {
	int32_t x;
	x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	x |= (rand() & 0xff) << 16;
	x |= (rand() & 0xff) << 24;
	return x;
}

void create_skb_map() {
	reqID_Set = uh_rid_init();
	reqID_timestamp_Map = uh_tmt_init();
	reqID_SID_Map = uh_int_init();    
	reqID_GWSID_Map = uh_int_init();
	reqID_IPC_Map = uh_ipc_init();
}

void destroy_skb_map() {
//    uxc_ixpc_t* tval = 0;
//    khiter_t k;
//    for (k = kh_begin(reqID_IPC_Map->h); k != kh_end(reqID_IPC_Map->h); ++k) {
//       if (kh_exist(reqID_IPC_Map->h, k)) {
//          const char *key = kh_key(reqID_IPC_Map->h, k);
//          tval = kh_value(reqID_IPC_Map->h, k);
// 		 free(tval);
//       }
//    }
	uh_rid_destroy(reqID_Set);
	uh_tmt_destroy(reqID_timestamp_Map);
	uh_int_destroy(reqID_SID_Map);
	uh_int_destroy(reqID_GWSID_Map);
	uh_ipc_destroy(reqID_IPC_Map);
}

///////////////////////////////////////////////////////////////////////////////////
// ClickToCall Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_clicktocall_heartbeat_req(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, HEARTBEAT_RESPONSE, 0, &skbmsg->header.requestID);
}

void skb_msg_process_clicktocall_binding_req(skb_msg_t *skbmsg, char *userID, char *password) {
	clicktocall_binding_req_tcp_t clicktocall_binding_req;
	int msg_size = sizeof(clicktocall_binding_req);
	// TCP Body 설정
	strncpy(clicktocall_binding_req.userID, userID, BINDING_USER_ID_LEN);
	strncpy(clicktocall_binding_req.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &clicktocall_binding_req, msg_size);
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BINDING_REQUEST, msg_size, NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	clicktocall_binding_req_tcp_display(headerLog, &clicktocall_binding_req);
}

int skb_msg_process_clicktocall_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID) {
	int rv;
	clicktocall_start_req_tcp_t clicktocall_start_req;

	// DBIF를 body로 변환
	clicktocall_start_req_dbif_display(dbif);
	rv = clicktocall_start_req_decode_dbif_msg(&clicktocall_start_req, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_REQUEST, sizeof(clicktocall_start_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_start_req, sizeof(clicktocall_start_req));
	clicktocall_start_req_tcp_display(headerLog, &clicktocall_start_req);

	return rv;
}

int skb_msg_process_clicktocall_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stop_req_tcp_t clicktocall_stop_req;

	// DBIF를 body로 변환
	clicktocall_stop_req_dbif_display(dbif);
	rv = clicktocall_stop_req_decode_dbif_msg(&clicktocall_stop_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktocall_stop_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_stop_req, sizeof(clicktocall_stop_req));
	clicktocall_stop_req_tcp_display(headerLog, &clicktocall_stop_req);

	return rv;
}

int skb_msg_process_clicktocall_startrecording_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_startrecording_req_tcp_t clicktocall_startrecording_req;

	// DBIF를 body로 변환
	clicktocall_startrecording_req_dbif_display(dbif);
	rv = clicktocall_startrecording_req_decode_dbif_msg(&clicktocall_startrecording_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_RECORDING_REQUEST, sizeof(clicktocall_startrecording_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_startrecording_req, sizeof(clicktocall_startrecording_req));
	clicktocall_startrecording_req_tcp_display(headerLog, &clicktocall_startrecording_req);
	
	return rv;
}

int skb_msg_process_clicktocall_stoprecording_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stoprecording_req_tcp_t clicktocall_stoprecording_req;

	// DBIF를 body로 변환
	clicktocall_stoprecording_req_dbif_display(dbif);
	rv = clicktocall_stoprecording_req_decode_dbif_msg(&clicktocall_stoprecording_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_RECORDING_REQUEST, sizeof(clicktocall_stoprecording_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocall_stoprecording_req, sizeof(clicktocall_stoprecording_req));
	clicktocall_stoprecording_req_tcp_display(headerLog, &clicktocall_stoprecording_req);

	return rv;
}

void skb_msg_process_clicktocall_heartbeat_rsp(skb_msg_t *skbmsg) {
	// skb_msg_get_header_display(&skbmsg->header, headerLog);
}

int skb_msg_process_clicktocall_binding_rsp(skb_msg_t *skbmsg) {
	clicktocall_binding_rsp_tcp_t clicktocall_binding_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_binding_rsp, skbmsg->body, sizeof(clicktocall_binding_rsp_tcp_t));
	clicktocall_binding_rsp_tcp_display(headerLog, clicktocall_binding_rsp);
	
	if (clicktocall_binding_rsp->resultCode < 2) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind(resultCode : %d)", clicktocall_binding_rsp->resultCode);
	return -1;
}

int skb_msg_process_clicktocall_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_start_rsp_tcp_t clicktocall_start_rsp[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_start_rsp, skbmsg->body, sizeof(clicktocall_start_rsp_tcp_t));
	clicktocall_start_rsp_tcp_display(headerLog, clicktocall_start_rsp);
	//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(reqID_SID_Map, skbmsg->header.requestID), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no sessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		uh_int_del(reqID_SID_Map, skbmsg->header.requestID);
	}
	strncpy(gwSessionID, uh_int_get(reqID_GWSID_Map, skbmsg->header.requestID), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no gwSessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		uh_int_del(reqID_GWSID_Map, skbmsg->header.requestID);
	}
	rv = clicktocall_start_rsp_encode_to_dbif_msg(clicktocall_start_rsp, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_start_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stop_rsp_tcp_t clicktocall_stop_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_stop_rsp, skbmsg->body, sizeof(clicktocall_stop_rsp_tcp_t));
	clicktocall_stop_rsp_tcp_display(headerLog, clicktocall_stop_rsp);
	rv = clicktocall_stop_rsp_encode_to_dbif_msg(clicktocall_stop_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_stop_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_startrecording_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_startrecording_rsp_tcp_t clicktocall_startrecording_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_startrecording_rsp, skbmsg->body, sizeof(clicktocall_startrecording_rsp_tcp_t));
	clicktocall_startrecording_rsp_tcp_display(headerLog, clicktocall_startrecording_rsp);
	rv = clicktocall_startrecording_rsp_encode_to_dbif_msg(clicktocall_startrecording_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_startrecording_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_stoprecording_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stoprecording_rsp_tcp_t clicktocall_stoprecording_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_stoprecording_rsp, skbmsg->body, sizeof(clicktocall_stoprecording_rsp_tcp_t));
	clicktocall_stoprecording_rsp_tcp_display(headerLog, clicktocall_stoprecording_rsp);
	rv = clicktocall_stoprecording_rsp_encode_to_dbif_msg(clicktocall_stoprecording_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_stoprecording_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_service_status_rsp( skb_msg_t *skbmsg) {
	clicktocall_service_status_rsp_tcp_t clicktocall_service_status_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_service_status_rsp, skbmsg->body, sizeof(clicktocall_service_status_rsp_tcp_t));
	clicktocall_service_status_rsp_tcp_display(headerLog, clicktocall_service_status_rsp);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocall_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocall_stop_rpt_tcp_t clicktocall_stop_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_stop_rpt, skbmsg->body, sizeof(clicktocall_stop_rpt_tcp_t));
	clicktocall_stop_rpt_tcp_display(headerLog, clicktocall_stop_rpt);
	rv = clicktocall_stop_rpt_encode_to_dbif_msg(clicktocall_stop_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocall_stop_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocall_startrecording_rpt( skb_msg_t *skbmsg) {
	clicktocall_startrecording_rpt_tcp_t clicktocall_startrecording_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_startrecording_rpt, skbmsg->body, sizeof(clicktocall_startrecording_rpt_tcp_t));
	clicktocall_startrecording_rpt_tcp_display(headerLog, clicktocall_startrecording_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocall_stoprecording_rpt( skb_msg_t *skbmsg) {
	clicktocall_stoprecording_rpt_tcp_t clicktocall_stoprecording_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_stoprecording_rpt, skbmsg->body, sizeof(clicktocall_stoprecording_rpt_tcp_t));
	clicktocall_stoprecording_rpt_tcp_display(headerLog, clicktocall_stoprecording_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocall_service_status_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif, int *status) {
	int rv = eUXC_SUCCESS;
	clicktocall_service_status_rpt_tcp_t clicktocall_service_status_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_service_status_rpt, skbmsg->body, sizeof(clicktocall_service_status_rpt_tcp_t));
	clicktocall_service_status_rpt_tcp_display(headerLog, clicktocall_service_status_rpt);
	*status = clicktocall_service_status_rpt->status;
	//착신자 호출 중(2), 정상 서비스 중(0)일 경우에만 gw로 포워딩
	if (*status == 2 || *status == 0) {	
		rv = clicktocall_service_status_rpt_encode_to_dbif_msg(clicktocall_service_status_rpt, dbif);
		if (rv <eUXC_SUCCESS) return rv;
		clicktocall_service_status_rpt_dbif_display(dbif);
	}

	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
// ClickToCallRecording Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_clicktocallrecording_heartbeat_req(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, HEARTBEAT_RESPONSE, 0, &skbmsg->header.requestID);
}

void skb_msg_process_clicktocallrecording_binding_req(skb_msg_t *skbmsg, char *userID, char *password) {
	clicktocallrecording_binding_req_tcp_t clicktocallrecording_binding_req;
	int msg_size = sizeof(clicktocallrecording_binding_req);
	// TCP Body 설정
	strncpy(clicktocallrecording_binding_req.userID, userID, BINDING_USER_ID_LEN);
	strncpy(clicktocallrecording_binding_req.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &clicktocallrecording_binding_req, msg_size);
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BINDING_REQUEST, msg_size, NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	clicktocallrecording_binding_req_tcp_display(headerLog, &clicktocallrecording_binding_req);
}

int skb_msg_process_clicktocallrecording_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID) {
	int rv;
	clicktocallrecording_start_req_tcp_t clicktocallrecording_start_req;

	// DBIF를 body로 변환
	clicktocallrecording_start_req_dbif_display(dbif);
	rv = clicktocallrecording_start_req_decode_dbif_msg(&clicktocallrecording_start_req, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_REQUEST, sizeof(clicktocallrecording_start_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocallrecording_start_req, sizeof(clicktocallrecording_start_req));
	clicktocallrecording_start_req_tcp_display(headerLog, &clicktocallrecording_start_req);

	return rv;
}

int skb_msg_process_clicktocallrecording_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_stop_req_tcp_t clicktocallrecording_stop_req;

	// DBIF를 body로 변환
	clicktocallrecording_stop_req_dbif_display(dbif);
	rv = clicktocallrecording_stop_req_decode_dbif_msg(&clicktocallrecording_stop_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_REQUEST, sizeof(clicktocallrecording_stop_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktocallrecording_stop_req, sizeof(clicktocallrecording_stop_req));
	clicktocallrecording_stop_req_tcp_display(headerLog, &clicktocallrecording_stop_req);

	return rv;
}

int skb_msg_process_clicktocallrecording_service_status_req( skb_msg_t *skbmsg) {
	clicktocallrecording_service_status_req_tcp_t clicktocallrecording_service_status_req;

	memcpy(&clicktocallrecording_service_status_req, skbmsg->body, sizeof(clicktocallrecording_service_status_req));
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	clicktocallrecording_service_status_req_tcp_display(headerLog, &clicktocallrecording_service_status_req);

	return eUXC_SUCCESS;
}

void skb_msg_process_clicktocallrecording_heartbeat_rsp(skb_msg_t *skbmsg) {
	// skb_msg_get_header_display(&skbmsg->header, headerLog);
}

int skb_msg_process_clicktocallrecording_binding_rsp(skb_msg_t *skbmsg) {
	clicktocallrecording_binding_rsp_tcp_t clicktocallrecording_binding_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocallrecording_binding_rsp, skbmsg->body, sizeof(clicktocallrecording_binding_rsp_tcp_t));
	clicktocallrecording_binding_rsp_tcp_display(headerLog, clicktocallrecording_binding_rsp);
	
	if (clicktocallrecording_binding_rsp->resultCode < 2) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind(resultCode : %d)", clicktocallrecording_binding_rsp->resultCode);
	return -1;
}

int skb_msg_process_clicktocallrecording_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_start_rsp_tcp_t clicktocallrecording_start_rsp[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocallrecording_start_rsp, skbmsg->body, sizeof(clicktocallrecording_start_rsp_tcp_t));
	clicktocallrecording_start_rsp_tcp_display(headerLog, clicktocallrecording_start_rsp);
	//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(reqID_SID_Map, skbmsg->header.requestID), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no sessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		uh_int_del(reqID_SID_Map, skbmsg->header.requestID);
	}
	strncpy(gwSessionID, uh_int_get(reqID_GWSID_Map, skbmsg->header.requestID), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no gwSessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		uh_int_del(reqID_GWSID_Map, skbmsg->header.requestID);
	}
	rv = clicktocallrecording_start_rsp_encode_to_dbif_msg(clicktocallrecording_start_rsp, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_start_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocallrecording_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_stop_rsp_tcp_t clicktocallrecording_stop_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocallrecording_stop_rsp, skbmsg->body, sizeof(clicktocallrecording_stop_rsp_tcp_t));
	clicktocallrecording_stop_rsp_tcp_display(headerLog, clicktocallrecording_stop_rsp);
	rv = clicktocallrecording_stop_rsp_encode_to_dbif_msg(clicktocallrecording_stop_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_stop_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocallrecording_service_status_rsp( skb_msg_t *skbmsg) {
	clicktocall_service_status_rsp_tcp_t clicktocall_service_status_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocall_service_status_rsp, skbmsg->body, sizeof(clicktocall_service_status_rsp_tcp_t));
	clicktocall_service_status_rsp_tcp_display(headerLog, clicktocall_service_status_rsp);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocallrecording_start_rpt( skb_msg_t *skbmsg) {
	clicktocallrecording_start_rpt_tcp_t clicktocallrecording_start_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocallrecording_start_rpt, skbmsg->body, sizeof(clicktocallrecording_start_rpt_tcp_t));
	clicktocallrecording_start_rpt_tcp_display(headerLog, clicktocallrecording_start_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktocallrecording_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_stop_rpt_tcp_t clicktocallrecording_stop_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocallrecording_stop_rpt, skbmsg->body, sizeof(clicktocallrecording_stop_rpt_tcp_t));
	clicktocallrecording_stop_rpt_tcp_display(headerLog, clicktocallrecording_stop_rpt);
	rv = clicktocallrecording_stop_rpt_encode_to_dbif_msg(clicktocallrecording_stop_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_stop_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktocallrecording_service_status_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktocallrecording_service_status_rpt_tcp_t clicktocallrecording_service_status_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktocallrecording_service_status_rpt, skbmsg->body, sizeof(clicktocallrecording_service_status_rpt_tcp_t));
	clicktocallrecording_service_status_rpt_tcp_display(headerLog, clicktocallrecording_service_status_rpt);
	rv = clicktocallrecording_service_status_rpt_encode_to_dbif_msg(clicktocallrecording_service_status_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktocallrecording_service_status_rpt_dbif_display(dbif);

	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Functions 
///////////////////////////////////////////////////////////////////////////////////

void skb_msg_process_clicktoconference_heartbeat_req(skb_msg_t *skbmsg) {
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, HEARTBEAT_RESPONSE, 0, &skbmsg->header.requestID);
}

void skb_msg_process_clicktoconference_binding_req(skb_msg_t *skbmsg, char *userID, char *password) {
	clicktoconference_binding_req_tcp_t clicktoconference_binding_req;
	int msg_size = sizeof(clicktoconference_binding_req);
	// TCP Body 설정
	strncpy(clicktoconference_binding_req.userID, userID, BINDING_USER_ID_LEN);
	strncpy(clicktoconference_binding_req.password, password, BINDING_PASSWORD_LEN);
	memcpy(skbmsg->body, &clicktoconference_binding_req, msg_size);
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, BINDING_REQUEST, msg_size, NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	clicktoconference_binding_req_tcp_display(headerLog, &clicktoconference_binding_req);
}


int skb_msg_process_clicktoconference_start_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif, char *sessionID, char *gwSessionID) {
	int rv;
	clicktoconference_start_req_tcp_t clicktoconference_start_req;

	// DBIF를 body로 변환
	clicktoconference_start_req_dbif_display(dbif);
	rv = clicktoconference_start_req_decode_dbif_msg(&clicktoconference_start_req, sessionID, gwSessionID, dbif);
	if (rv < eUXC_SUCCESS) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, START_CONFERENCE_REQUEST, sizeof(clicktoconference_start_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_start_req, sizeof(clicktoconference_start_req));
	clicktoconference_start_req_tcp_display(headerLog, &clicktoconference_start_req);

	return rv;
}

int skb_msg_process_clicktoconference_add_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_add_party_req_tcp_t clicktoconference_add_party_req;

	// DBIF를 body로 변환
	clicktoconference_add_party_req_dbif_display(dbif);
	rv = clicktoconference_add_party_req_decode_dbif_msg(&clicktoconference_add_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, ADD_PARTY_REQUEST, sizeof(clicktoconference_add_party_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_add_party_req, sizeof(clicktoconference_add_party_req));
	clicktoconference_add_party_req_tcp_display(headerLog, &clicktoconference_add_party_req);

	return rv;
}

int skb_msg_process_clicktoconference_remove_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_remove_party_req_tcp_t clicktoconference_remove_party_req;

	// DBIF를 body로 변환
	clicktoconference_remove_party_req_dbif_display(dbif);
	rv = clicktoconference_remove_party_req_decode_dbif_msg(&clicktoconference_remove_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, REMOVE_PARTY_REQUEST, sizeof(clicktoconference_remove_party_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_remove_party_req, sizeof(clicktoconference_remove_party_req));
	clicktoconference_remove_party_req_tcp_display(headerLog, &clicktoconference_remove_party_req);

	return rv;
}

int skb_msg_process_clicktoconference_change_party_media_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_change_party_media_req_tcp_t clicktoconference_change_party_media_req;

	// DBIF를 body로 변환
	clicktoconference_change_party_media_req_dbif_display(dbif);
	rv = clicktoconference_change_party_media_req_decode_dbif_msg(&clicktoconference_change_party_media_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, CHANGE_PARTY_MEDIA_REQUEST, sizeof(clicktoconference_change_party_media_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_change_party_media_req, sizeof(clicktoconference_change_party_media_req));
	clicktoconference_change_party_media_req_tcp_display(headerLog, &clicktoconference_change_party_media_req);

	return rv;
}

int skb_msg_process_clicktoconference_change_option_req( skb_msg_t *skbmsg) {
	clicktoconference_change_option_req_tcp_t clicktoconference_change_option_req;

	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);

	// TCP Body 설정
	memcpy(&clicktoconference_change_option_req, skbmsg->body, sizeof(clicktoconference_change_option_req));
	clicktoconference_change_option_req_tcp_display(headerLog, &clicktoconference_change_option_req);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktoconference_get_number_of_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_number_of_party_req_tcp_t clicktoconference_get_number_of_party_req;

	// DBIF를 body로 변환
	clicktoconference_get_number_of_party_req_dbif_display(dbif);
	rv = clicktoconference_get_number_of_party_req_decode_dbif_msg(&clicktoconference_get_number_of_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, GET_NUMBER_OF_PARTY_REQUEST, sizeof(clicktoconference_get_number_of_party_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_get_number_of_party_req, sizeof(clicktoconference_get_number_of_party_req));
	clicktoconference_get_number_of_party_req_tcp_display(headerLog, &clicktoconference_get_number_of_party_req);

	return rv;
}

int skb_msg_process_clicktoconference_stop_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_stop_req_tcp_t clicktoconference_stop_req;

	// DBIF를 body로 변환
	clicktoconference_stop_req_dbif_display(dbif);
	rv = clicktoconference_stop_req_decode_dbif_msg(&clicktoconference_stop_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, STOP_CONFERENCE_REQUEST, sizeof(clicktoconference_stop_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_stop_req, sizeof(clicktoconference_stop_req));
	clicktoconference_stop_req_tcp_display(headerLog, &clicktoconference_stop_req);

	return rv;
}

int skb_msg_process_clicktoconference_play_ment_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_play_ment_req_tcp_t clicktoconference_play_ment_req;

	// DBIF를 body로 변환
	clicktoconference_play_ment_req_dbif_display(dbif);
	rv = clicktoconference_play_ment_req_decode_dbif_msg(&clicktoconference_play_ment_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, PLAY_MENT_REQUEST, sizeof(clicktoconference_play_ment_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_play_ment_req, sizeof(clicktoconference_play_ment_req));
	clicktoconference_play_ment_req_tcp_display(headerLog, &clicktoconference_play_ment_req);

	return rv;
}

int skb_msg_process_clicktoconference_get_party_status_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_party_status_req_tcp_t clicktoconference_get_party_status_req;

	// DBIF를 body로 변환
	clicktoconference_get_party_status_req_dbif_display(dbif);
	rv = clicktoconference_get_party_status_req_decode_dbif_msg(&clicktoconference_get_party_status_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, GET_PARTY_STATUS_REQUEST, sizeof(clicktoconference_get_party_status_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_get_party_status_req, sizeof(clicktoconference_get_party_status_req));
	clicktoconference_get_party_status_req_tcp_display(headerLog, &clicktoconference_get_party_status_req);

	return rv;
}

int skb_msg_process_clicktoconference_cancel_party_req( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_cancel_party_req_tcp_t clicktoconference_cancel_party_req;

	// DBIF를 body로 변환
	clicktoconference_cancel_party_req_dbif_display(dbif);
	rv = clicktoconference_cancel_party_req_decode_dbif_msg(&clicktoconference_cancel_party_req, dbif);
	if (rv < 0) return rv;
	// TCP Header 설정
	skb_msg_make_header(&skbmsg->header, CANCEL_PARTY_REQUEST, sizeof(clicktoconference_cancel_party_req), NULL);
	char headerLog[256];
	skb_msg_get_send_header_display(&skbmsg->header, headerLog);
	// TCP Body 설정
	memcpy(skbmsg->body, &clicktoconference_cancel_party_req, sizeof(clicktoconference_cancel_party_req));
	clicktoconference_cancel_party_req_tcp_display(headerLog, &clicktoconference_cancel_party_req);

	return rv;
}

void skb_msg_process_clicktoconference_heartbeat_rsp(skb_msg_t *skbmsg) {
	// skb_msg_get_header_display(&skbmsg->header, headerLog);
}

int skb_msg_process_clicktoconference_binding_rsp(skb_msg_t *skbmsg) {
	clicktoconference_binding_rsp_tcp_t clicktoconference_binding_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_binding_rsp, skbmsg->body, sizeof(clicktoconference_binding_rsp_tcp_t));
	clicktoconference_binding_rsp_tcp_display(headerLog, clicktoconference_binding_rsp);
	
	if (clicktoconference_binding_rsp->resultCode < 2) {
		return eUXC_SUCCESS;
	}
	ux_log(UXL_CRT, "failed to bind(resultCode: %d)", clicktoconference_binding_rsp->resultCode);
	return -1;
}

int skb_msg_process_clicktoconference_start_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_start_rsp_tcp_t clicktoconference_start_rsp[1];
	char sessionID[SESSION_ID_LEN];
	char gwSessionID[GW_SESSION_ID_LEN];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_start_rsp, skbmsg->body, sizeof(clicktoconference_start_rsp_tcp_t));
	clicktoconference_start_rsp_tcp_display(headerLog, clicktoconference_start_rsp);
	//requestID에 따라 기존에 저장한 sessionID, gwSessionID 추가하여 dbif 메시지 생성
	strncpy(sessionID, uh_int_get(reqID_SID_Map, skbmsg->header.requestID), SESSION_ID_LEN);
	if (sessionID == NULL || strcmp(sessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no sessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		uh_int_del(reqID_SID_Map, skbmsg->header.requestID);
	}
	strncpy(gwSessionID, uh_int_get(reqID_GWSID_Map, skbmsg->header.requestID), GW_SESSION_ID_LEN);
	if (gwSessionID == NULL || strcmp(gwSessionID, "") == 0) {
		ux_log(UXL_CRT, "there is no gwSessionID of reqID(%d)", skbmsg->header.requestID);
		return -1;
	} else {
		uh_int_del(reqID_GWSID_Map, skbmsg->header.requestID);
	}
	rv = clicktoconference_start_rsp_encode_to_dbif_msg(clicktoconference_start_rsp, sessionID, gwSessionID, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_start_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_add_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_add_party_rsp_tcp_t clicktoconference_add_party_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_add_party_rsp, skbmsg->body, sizeof(clicktoconference_add_party_rsp_tcp_t));
	clicktoconference_add_party_rsp_tcp_display(headerLog, clicktoconference_add_party_rsp);
	rv = clicktoconference_add_party_rsp_encode_to_dbif_msg(clicktoconference_add_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_add_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_remove_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_remove_party_rsp_tcp_t clicktoconference_remove_party_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_remove_party_rsp, skbmsg->body, sizeof(clicktoconference_remove_party_rsp_tcp_t));
	clicktoconference_remove_party_rsp_tcp_display(headerLog, clicktoconference_remove_party_rsp);
	rv = clicktoconference_remove_party_rsp_encode_to_dbif_msg(clicktoconference_remove_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_remove_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_party_media_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_change_party_media_rsp_tcp_t clicktoconference_change_party_media_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_change_party_media_rsp, skbmsg->body, sizeof(clicktoconference_change_party_media_rsp_tcp_t));
	clicktoconference_change_party_media_rsp_tcp_display(headerLog, clicktoconference_change_party_media_rsp);
	rv = clicktoconference_change_party_media_rsp_encode_to_dbif_msg(clicktoconference_change_party_media_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_change_party_media_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_option_rsp( skb_msg_t *skbmsg) {
	clicktoconference_change_option_rsp_tcp_t clicktoconference_change_option_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_change_option_rsp, skbmsg->body, sizeof(clicktoconference_change_option_rsp_tcp_t));
	clicktoconference_change_option_rsp_tcp_display(headerLog, clicktoconference_change_option_rsp);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktoconference_get_number_of_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_number_of_party_rsp_tcp_t clicktoconference_get_number_of_party_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_get_number_of_party_rsp, skbmsg->body, sizeof(clicktoconference_get_number_of_party_rsp_tcp_t));
	clicktoconference_get_number_of_party_rsp_tcp_display(headerLog, clicktoconference_get_number_of_party_rsp);
	rv = clicktoconference_get_number_of_party_rsp_encode_to_dbif_msg(clicktoconference_get_number_of_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_get_number_of_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_stop_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_stop_rsp_tcp_t clicktoconference_stop_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_stop_rsp, skbmsg->body, sizeof(clicktoconference_stop_rsp_tcp_t));
	clicktoconference_stop_rsp_tcp_display(headerLog, clicktoconference_stop_rsp);
	rv = clicktoconference_stop_rsp_encode_to_dbif_msg(clicktoconference_stop_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_stop_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_play_ment_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_play_ment_rsp_tcp_t clicktoconference_play_ment_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_play_ment_rsp, skbmsg->body, sizeof(clicktoconference_play_ment_rsp_tcp_t));
	clicktoconference_play_ment_rsp_tcp_display(headerLog, clicktoconference_play_ment_rsp);
	rv = clicktoconference_play_ment_rsp_encode_to_dbif_msg(clicktoconference_play_ment_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_play_ment_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_get_party_status_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_get_party_status_rsp_tcp_t clicktoconference_get_party_status_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_get_party_status_rsp, skbmsg->body, sizeof(clicktoconference_get_party_status_rsp_tcp_t));
	clicktoconference_get_party_status_rsp_tcp_display(headerLog, clicktoconference_get_party_status_rsp);
	rv = clicktoconference_get_party_status_rsp_encode_to_dbif_msg(clicktoconference_get_party_status_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_get_party_status_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_cancel_party_rsp( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_cancel_party_rsp_tcp_t clicktoconference_cancel_party_rsp[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_cancel_party_rsp, skbmsg->body, sizeof(clicktoconference_cancel_party_rsp_tcp_t));
	clicktoconference_cancel_party_rsp_tcp_display(headerLog, clicktoconference_cancel_party_rsp);
	rv = clicktoconference_cancel_party_rsp_encode_to_dbif_msg(clicktoconference_cancel_party_rsp, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_cancel_party_rsp_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_add_party_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_add_party_rpt_tcp_t clicktoconference_add_party_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_add_party_rpt, skbmsg->body, sizeof(clicktoconference_add_party_rpt_tcp_t));
	clicktoconference_add_party_rpt_tcp_display(headerLog, clicktoconference_add_party_rpt);
	rv = clicktoconference_add_party_rpt_encode_to_dbif_msg(clicktoconference_add_party_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_add_party_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_remove_party_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_remove_party_rpt_tcp_t clicktoconference_remove_party_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_remove_party_rpt, skbmsg->body, sizeof(clicktoconference_remove_party_rpt_tcp_t));
	clicktoconference_remove_party_rpt_tcp_display(headerLog, clicktoconference_remove_party_rpt);
	rv = clicktoconference_remove_party_rpt_encode_to_dbif_msg(clicktoconference_remove_party_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_remove_party_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_party_media_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_change_party_media_rpt_tcp_t clicktoconference_change_party_media_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_change_party_media_rpt, skbmsg->body, sizeof(clicktoconference_change_party_media_rpt_tcp_t));
	clicktoconference_change_party_media_rpt_tcp_display(headerLog, clicktoconference_change_party_media_rpt);
	rv = clicktoconference_change_party_media_rpt_encode_to_dbif_msg(clicktoconference_change_party_media_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_change_party_media_rpt_dbif_display(dbif);

	return rv;
}

int skb_msg_process_clicktoconference_change_option_rpt( skb_msg_t *skbmsg) {
	clicktoconference_change_option_rpt_tcp_t clicktoconference_change_option_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_change_option_rpt, skbmsg->body, sizeof(clicktoconference_change_option_rpt_tcp_t));
	clicktoconference_change_option_rpt_tcp_display(headerLog, clicktoconference_change_option_rpt);

	return eUXC_SUCCESS;
}

int skb_msg_process_clicktoconference_stop_rpt( skb_msg_t *skbmsg, uxc_dbif_t *dbif) {
	int rv;
	clicktoconference_stop_rpt_tcp_t clicktoconference_stop_rpt[1];

	char headerLog[256];
	skb_msg_get_recv_header_display(&skbmsg->header, headerLog);
	memcpy(clicktoconference_stop_rpt, skbmsg->body, sizeof(clicktoconference_stop_rpt_tcp_t));
	clicktoconference_stop_rpt_tcp_display(headerLog, clicktoconference_stop_rpt);
	rv = clicktoconference_stop_rpt_encode_to_dbif_msg(clicktoconference_stop_rpt, dbif);
	if (rv <eUXC_SUCCESS) return rv;
	clicktoconference_stop_rpt_dbif_display(dbif);

	return rv;
}