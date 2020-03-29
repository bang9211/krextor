#include "clicktoconference_req.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_start_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_start_req_tcp_init( clicktoconference_start_req_tcp_t *clicktoconference_start_req) {
	memset(clicktoconference_start_req, 0, sizeof(clicktoconference_start_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_start_req_tcp_final( clicktoconference_start_req_tcp_t *clicktoconference_start_req) {
    return;
}

int clicktoconference_start_req_decode_dbif_msg( clicktoconference_start_req_tcp_t *clicktoconference_start_req, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif) {
	int rv;

	strncpy(sessionID, uxc_dbif_get_str(dbif, 0, &rv), SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(gwSessionID, uxc_dbif_get_str(dbif, 1, &rv), GW_SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->subject, uxc_dbif_get_str(dbif, 2, &rv), SUBJECT_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->chairmanName, uxc_dbif_get_str(dbif, 3, &rv), CHAIRMAN_NAME_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->chairmanNumber, uxc_dbif_get_str(dbif, 4, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->chargingNumber, uxc_dbif_get_str(dbif, 5, &rv), CHARGING_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->confType = uxc_dbif_get_int(dbif, 6, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->participantScenario = uxc_dbif_get_int(dbif, 7, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->cidType = uxc_dbif_get_int(dbif, 8, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->chairmanCID, uxc_dbif_get_str(dbif, 9, &rv), CID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->isRecording = uxc_dbif_get_int(dbif, 10, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->hostingCode = uxc_dbif_get_int(dbif, 11, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->password, uxc_dbif_get_str(dbif, 12, &rv), PASSWORD_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->serviceMentGroupID = uxc_dbif_get_int(dbif, 13, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->wirelessTimeout = uxc_dbif_get_int(dbif, 14, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->wiredTimeout = uxc_dbif_get_int(dbif, 15, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_start_req->callingCategoryType = uxc_dbif_get_int(dbif, 16, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_start_req->participantList, uxc_dbif_get_str(dbif, 17, &rv), MAX_PARTICIPANT_LIST_LEN);
	if( rv < eUXC_SUCCESS) goto final;
    
	clicktoconference_start_req->filler1 = 0;
	strncpy(clicktoconference_start_req->startTime, "", TIME_LEN);
	clicktoconference_start_req->smsNoti = 0;
	clicktoconference_start_req->voiceNoti = 0;
	clicktoconference_start_req->filler2 = 0;
	strncpy(clicktoconference_start_req->msp_id, "", 4);
	strncpy(clicktoconference_start_req->sp_id, "", 4);
	strncpy(clicktoconference_start_req->service_id, "", 4);

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_start_req_tcp_display(clicktoconference_start_req_tcp_t *clicktoconference_start_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_start_req]");
	ux_log(UXL_INFO, "  [subject] %s", clicktoconference_start_req->subject);
	ux_log(UXL_INFO, "  [chairmanName] %s", clicktoconference_start_req->chairmanName);
	ux_log(UXL_INFO, "  [chairmanNumber] %s", clicktoconference_start_req->chairmanNumber);
	ux_log(UXL_INFO, "  [chargingNumber] %s", clicktoconference_start_req->chargingNumber);
	ux_log(UXL_INFO, "  [confType] %d", clicktoconference_start_req->confType);
	ux_log(UXL_INFO, "  [participantScenario] %d", clicktoconference_start_req->participantScenario);
	ux_log(UXL_INFO, "  [filler1] %d", clicktoconference_start_req->filler1);
	ux_log(UXL_INFO, "  [cidType] %d", clicktoconference_start_req->cidType);
	ux_log(UXL_INFO, "  [chairmanCID] %s", clicktoconference_start_req->chairmanCID);
	ux_log(UXL_INFO, "  [startTime] %s", clicktoconference_start_req->startTime);
	ux_log(UXL_INFO, "  [smsNoti] %d", clicktoconference_start_req->smsNoti);
	ux_log(UXL_INFO, "  [voiceNoti] %d", clicktoconference_start_req->voiceNoti);
	ux_log(UXL_INFO, "  [isRecording] %d", clicktoconference_start_req->isRecording);
	ux_log(UXL_INFO, "  [hostingCode] %d", clicktoconference_start_req->hostingCode);
	ux_log(UXL_INFO, "  [password] %s", clicktoconference_start_req->password);
	ux_log(UXL_INFO, "  [serviceMentGroupID] %d", clicktoconference_start_req->serviceMentGroupID);
	ux_log(UXL_INFO, "  [wirelessTimeout] %d", clicktoconference_start_req->wirelessTimeout);
	ux_log(UXL_INFO, "  [wiredTimeout] %d", clicktoconference_start_req->wiredTimeout);
	ux_log(UXL_INFO, "  [filler2] %d", clicktoconference_start_req->filler2);
	ux_log(UXL_INFO, "  [callingCategoryType] %d", clicktoconference_start_req->callingCategoryType);
	ux_log(UXL_INFO, "  [msp_id] %s", clicktoconference_start_req->msp_id);
	ux_log(UXL_INFO, "  [sp_id] %s", clicktoconference_start_req->sp_id);
	ux_log(UXL_INFO, "  [service_id] %s", clicktoconference_start_req->service_id);
	ux_log(UXL_INFO, "  [participantList] %s", clicktoconference_start_req->participantList);
}

void clicktoconference_start_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_start_req]");
	ux_log(UXL_INFO, "  [sessionID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [gwSessionID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [subject] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [chairmanName] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [chairmanNumber] %s",uxc_dbif_get_str(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [chargingNumber] %s",uxc_dbif_get_str(dbif, 5, &rv));
	ux_log(UXL_INFO, "  [confType] %d",uxc_dbif_get_int(dbif, 6, &rv));
	ux_log(UXL_INFO, "  [participantScenario] %d",uxc_dbif_get_int(dbif, 7, &rv));
	ux_log(UXL_INFO, "  [cidType] %d",uxc_dbif_get_int(dbif, 8, &rv));
	ux_log(UXL_INFO, "  [chairmanCID] %s",uxc_dbif_get_str(dbif, 9, &rv));
	ux_log(UXL_INFO, "  [isRecording] %d",uxc_dbif_get_int(dbif, 10, &rv));
	ux_log(UXL_INFO, "  [hostingCode] %d",uxc_dbif_get_int(dbif, 11, &rv));
	ux_log(UXL_INFO, "  [password] %s",uxc_dbif_get_str(dbif, 12, &rv));
	ux_log(UXL_INFO, "  [serviceMentGroupID] %d",uxc_dbif_get_int(dbif, 13, &rv));
	ux_log(UXL_INFO, "  [wirelessTimeout] %d",uxc_dbif_get_int(dbif, 14, &rv));
	ux_log(UXL_INFO, "  [wiredTimeout] %d",uxc_dbif_get_int(dbif, 15, &rv));
	ux_log(UXL_INFO, "  [callingCategoryType] %d",uxc_dbif_get_int(dbif, 16, &rv));
	ux_log(UXL_INFO, "  [participantList] %s",uxc_dbif_get_str(dbif, 17, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_add_party_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_add_party_req_tcp_init( clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req) {
	memset(clicktoconference_add_party_req, 0, sizeof(clicktoconference_add_party_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_add_party_req_tcp_final( clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req) {
    return;
}

int clicktoconference_add_party_req_decode_dbif_msg( clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req, uxc_dbif_t *dbif) {
    int rv;

	strncpy(clicktoconference_add_party_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_add_party_req->participantList, uxc_dbif_get_str(dbif, 1, &rv), MAX_PARTICIPANT_LIST_LEN);
	if( rv < eUXC_SUCCESS) goto final;
    
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_add_party_req_tcp_display(clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_add_party_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_add_party_req->confID);
	ux_log(UXL_INFO, "  [participantList] %s", clicktoconference_add_party_req->participantList);
}

void clicktoconference_add_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_add_party_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [participantList] %s",uxc_dbif_get_str(dbif, 1, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_remove_party_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_remove_party_req_tcp_init( clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req) {
	memset(clicktoconference_remove_party_req, 0, sizeof(clicktoconference_remove_party_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_remove_party_req_tcp_final( clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req) {
    return;
}

int clicktoconference_remove_party_req_decode_dbif_msg( clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req, uxc_dbif_t *dbif) {
    int rv;

	strncpy(clicktoconference_remove_party_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_remove_party_req->participantNumber, uxc_dbif_get_str(dbif, 1, &rv), PARTICIPANT_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
    
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_remove_party_req_tcp_display(clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_remove_party_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_remove_party_req->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_remove_party_req->participantNumber);
}

void clicktoconference_remove_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_remove_party_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 1, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_change_party_media_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_change_party_media_req_tcp_init( clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req) {
	memset(clicktoconference_change_party_media_req, 0, sizeof(clicktoconference_change_party_media_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_change_party_media_req_tcp_final( clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req) {
    return;
}

int clicktoconference_change_party_media_req_decode_dbif_msg( clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req, uxc_dbif_t *dbif) {
    int rv;

	strncpy(clicktoconference_change_party_media_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_change_party_media_req->participantNumber, uxc_dbif_get_str(dbif, 1, &rv), PARTICIPANT_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktoconference_change_party_media_req->mediaType = uxc_dbif_get_int(dbif, 2, &rv);
	if( rv < eUXC_SUCCESS) goto final;
    
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_change_party_media_req_tcp_display(clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_change_party_media_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_change_party_media_req->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_change_party_media_req->participantNumber);
	ux_log(UXL_INFO, "  [mediaType] %d", clicktoconference_change_party_media_req->mediaType);
}

void clicktoconference_change_party_media_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_change_party_media_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [mediaType] %d",uxc_dbif_get_int(dbif, 2, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_change_option_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_change_option_req_tcp_init( clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req) {
	memset(clicktoconference_change_option_req, 0, sizeof(clicktoconference_change_option_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_change_option_req_tcp_final( clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req) {
    return;
}

void clicktoconference_change_option_req_tcp_display(clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_change_option_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_change_option_req->confID);
	ux_log(UXL_INFO, "  [password] %s", clicktoconference_change_option_req->password);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_get_number_of_party_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_get_number_of_party_req_tcp_init( clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req) {
	memset(clicktoconference_get_number_of_party_req, 0, sizeof(clicktoconference_get_number_of_party_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_get_number_of_party_req_tcp_final( clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req) {
    return;
}

int clicktoconference_get_number_of_party_req_decode_dbif_msg( clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req, uxc_dbif_t *dbif) {
    int rv;

	strncpy(clicktoconference_get_number_of_party_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
    
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_get_number_of_party_req_tcp_display(clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_get_number_of_party_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_get_number_of_party_req->confID);
}

void clicktoconference_get_number_of_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_get_number_of_party_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
}


///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_stop_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_stop_req_tcp_init( clicktoconference_stop_req_tcp_t *clicktoconference_stop_req) {
    memset(clicktoconference_stop_req, 0, sizeof(clicktoconference_stop_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_stop_req_tcp_final( clicktoconference_stop_req_tcp_t *clicktoconference_stop_req) {
    return;
}

int clicktoconference_stop_req_decode_dbif_msg( clicktoconference_stop_req_tcp_t *clicktoconference_stop_req, uxc_dbif_t *dbif) {
    int rv;
	
	strncpy(clicktoconference_stop_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktoconference_stop_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_stop_req_tcp_display(clicktoconference_stop_req_tcp_t *clicktoconference_stop_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_stop_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_stop_req->confID);
}

void clicktoconference_stop_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_stop_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_play_ment_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_play_ment_req_tcp_init( clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req) {
	memset(clicktoconference_play_ment_req, 0, sizeof(clicktoconference_play_ment_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_play_ment_req_tcp_final( clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req) {
    return;
}

int clicktoconference_play_ment_req_decode_dbif_msg( clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req, uxc_dbif_t *dbif) {
    int rv;
	
	strncpy(clicktoconference_play_ment_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_play_ment_req->aupkg, uxc_dbif_get_str(dbif, 1, &rv), AUPKG_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktoconference_stop_req_decode_dbif_msg is failed (rv=%d)", rv);
return rv;
}

void clicktoconference_play_ment_req_tcp_display(clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_play_ment_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_play_ment_req->confID);
	ux_log(UXL_INFO, "  [aupkg] %s", clicktoconference_play_ment_req->aupkg);
}

void clicktoconference_play_ment_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_play_ment_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [aupkg] %s",uxc_dbif_get_str(dbif, 1, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_get_party_status_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_get_party_status_req_tcp_init( clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req) {
	memset(clicktoconference_get_party_status_req, 0, sizeof(clicktoconference_get_party_status_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_get_party_status_req_tcp_final( clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req) {
    return;
}

int clicktoconference_get_party_status_req_decode_dbif_msg( clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req,  uxc_dbif_t *dbif) {
    int rv;
	
	strncpy(clicktoconference_get_party_status_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_get_party_status_req->participantName, uxc_dbif_get_str(dbif, 1, &rv), PARTICIPANT_NAME_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_get_party_status_req->participantNumber, uxc_dbif_get_str(dbif, 2, &rv), PARTICIPANT_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktoconference_stop_req_decode_dbif_msg is failed (rv=%d)", rv);
return rv;
}

void clicktoconference_get_party_status_req_tcp_display(clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_play_ment_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_get_party_status_req->confID);
	ux_log(UXL_INFO, "  [participantName] %s", clicktoconference_get_party_status_req->participantName);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_get_party_status_req->participantNumber);
}

void clicktoconference_get_party_status_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_get_party_status_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [participantName] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_cancel_party_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_cancel_party_req_tcp_init( clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req) {
	memset(clicktoconference_cancel_party_req, 0, sizeof(clicktoconference_cancel_party_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_cancel_party_req_tcp_final( clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req) {
    return;
}

int clicktoconference_cancel_party_req_decode_dbif_msg( clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req, uxc_dbif_t *dbif) {
    int rv;

	strncpy(clicktoconference_cancel_party_req->confID, uxc_dbif_get_str(dbif, 0, &rv), CONF_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktoconference_cancel_party_req->participantNumber, uxc_dbif_get_str(dbif, 1, &rv), PARTICIPANT_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
    
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_cancel_party_req_tcp_display(clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req) {
	ux_log(UXL_INFO, "TCP [clicktoconference_cancel_party_req]");
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_cancel_party_req->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_cancel_party_req->participantNumber);
}

void clicktoconference_cancel_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_cancel_party_req]");
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 1, &rv));
}