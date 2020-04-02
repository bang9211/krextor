#include "clicktoconference_req.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_binding_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_binding_req_tcp_init( clicktoconference_binding_req_tcp_t *clicktoconference_binding_req)
{
	memset(clicktoconference_binding_req, 0, sizeof(clicktoconference_binding_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_binding_req_tcp_final( clicktoconference_binding_req_tcp_t *clicktoconference_binding_req)
{
	return;
}

void clicktoconference_binding_req_tcp_display(char *headerStr, clicktoconference_binding_req_tcp_t *clicktoconference_binding_req)
{
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_binding_req]\n"
		"  [userID] %s\n"
		"  [password] %s"
		, headerStr
		, clicktoconference_binding_req->userID
		, clicktoconference_binding_req->password);
}

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

void clicktoconference_start_req_tcp_display(char *headerStr, clicktoconference_start_req_tcp_t *clicktoconference_start_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_start_req]\n"
		"  [subject] %s\n"
		"  [chairmanName] %s\n"
		"  [chairmanNumber] %s\n"
		"  [chargingNumber] %s\n"
		"  [confType] %d\n"
		"  [participantScenario] %d\n"
		"  [filler1] %d\n"
		"  [cidType] %d\n"
		"  [chairmanCID] %s\n"
		"  [startTime] %s\n"
		"  [smsNoti] %d\n"
		"  [voiceNoti] %d\n"
		"  [isRecording] %d\n"
		"  [hostingCode] %d\n"
		"  [password] %s\n"
		"  [serviceMentGroupID] %d\n"
		"  [wirelessTimeout] %d\n"
		"  [wiredTimeout] %d\n"
		"  [filler2] %d\n"
		"  [callingCategoryType] %d\n"
		"  [msp_id] %s\n"
		"  [sp_id] %s\n"
		"  [service_id] %s\n"
		"  [participantList] %s"
		, headerStr
		, clicktoconference_start_req->subject
		, clicktoconference_start_req->chairmanName
		, clicktoconference_start_req->chairmanNumber
		, clicktoconference_start_req->chargingNumber
		, clicktoconference_start_req->confType
		, clicktoconference_start_req->participantScenario
		, clicktoconference_start_req->filler1
		, clicktoconference_start_req->cidType
		, clicktoconference_start_req->chairmanCID
		, clicktoconference_start_req->startTime
		, clicktoconference_start_req->smsNoti
		, clicktoconference_start_req->voiceNoti
		, clicktoconference_start_req->isRecording
		, clicktoconference_start_req->hostingCode
		, clicktoconference_start_req->password
		, clicktoconference_start_req->serviceMentGroupID
		, clicktoconference_start_req->wirelessTimeout
		, clicktoconference_start_req->wiredTimeout
		, clicktoconference_start_req->filler2
		, clicktoconference_start_req->callingCategoryType
		, clicktoconference_start_req->msp_id
		, clicktoconference_start_req->sp_id
		, clicktoconference_start_req->service_id
		, clicktoconference_start_req->participantList);
}

void clicktoconference_start_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_start_req]\n"
		"  [sessionID] %s\n"
		"  [gwSessionID] %s\n"
		"  [subject] %s\n"
		"  [chairmanName] %s\n"
		"  [chairmanNumber] %s\n"
		"  [chargingNumber] %s\n"
		"  [confType] %d\n"
		"  [participantScenario] %d\n"
		"  [cidType] %d\n"
		"  [chairmanCID] %s\n"
		"  [isRecording] %d\n"
		"  [hostingCode] %d\n"
		"  [password] %s\n"
		"  [serviceMentGroupID] %d\n"
		"  [wirelessTimeout] %d\n"
		"  [wiredTimeout] %d\n"
		"  [callingCategoryType] %d\n"
		"  [participantList] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv)
		,uxc_dbif_get_str(dbif, 3, &rv)
		,uxc_dbif_get_str(dbif, 4, &rv)
		,uxc_dbif_get_str(dbif, 5, &rv)
		,uxc_dbif_get_int(dbif, 6, &rv)
		,uxc_dbif_get_int(dbif, 7, &rv)
		,uxc_dbif_get_int(dbif, 8, &rv)
		,uxc_dbif_get_str(dbif, 9, &rv)
		,uxc_dbif_get_int(dbif, 10, &rv)
		,uxc_dbif_get_int(dbif, 11, &rv)
		,uxc_dbif_get_str(dbif, 12, &rv)
		,uxc_dbif_get_int(dbif, 13, &rv)
		,uxc_dbif_get_int(dbif, 14, &rv)
		,uxc_dbif_get_int(dbif, 15, &rv)
		,uxc_dbif_get_int(dbif, 16, &rv)
		,uxc_dbif_get_str(dbif, 17, &rv));
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

void clicktoconference_add_party_req_tcp_display(char *headerStr, clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_add_party_req]\n"
		"  [confID] %s\n"
		"  [participantList] %s"
		, headerStr
		, clicktoconference_add_party_req->confID
		, clicktoconference_add_party_req->participantList);
}

void clicktoconference_add_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_add_party_req]\n"
		"  [confID] %s\n"
		"  [participantList] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv));
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

void clicktoconference_remove_party_req_tcp_display(char *headerStr, clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_remove_party_req]\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		, headerStr
		, clicktoconference_remove_party_req->confID
		, clicktoconference_remove_party_req->participantNumber);
}

void clicktoconference_remove_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_remove_party_req]\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv));
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

void clicktoconference_change_party_media_req_tcp_display(char *headerStr, clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_change_party_media_req]\n"
		"  [confID] %s\n"
		"  [participantNumber] %s\n"
		"  [mediaType] %d"
		, headerStr
		, clicktoconference_change_party_media_req->confID
		, clicktoconference_change_party_media_req->participantNumber
		, clicktoconference_change_party_media_req->mediaType);
}

void clicktoconference_change_party_media_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_change_party_media_req]\n"
		"  [confID] %s\n"
		"  [participantNumber] %s\n"
		"  [mediaType] %d"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_int(dbif, 2, &rv));
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

void clicktoconference_change_option_req_tcp_display(char *headerStr, clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_change_option_req]\n"
		"  [confID] %s\n"
		"  [password] %s"
		, headerStr
		, clicktoconference_change_option_req->confID
		, clicktoconference_change_option_req->password);
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

void clicktoconference_get_number_of_party_req_tcp_display(char *headerStr, clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_get_number_of_party_req]\n"
		"  [confID] %s", 
		headerStr,
		clicktoconference_get_number_of_party_req->confID);
}

void clicktoconference_get_number_of_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_get_number_of_party_req]\n"
		"  [confID] %s",
		uxc_dbif_get_str(dbif, 0, &rv));
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

void clicktoconference_stop_req_tcp_display(char *headerStr, clicktoconference_stop_req_tcp_t *clicktoconference_stop_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_stop_req]\n"
		"  [confID] %s", 
		headerStr,
		clicktoconference_stop_req->confID)	;
}

void clicktoconference_stop_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_stop_req]\n"
		"  [confID] %s",
		uxc_dbif_get_str(dbif, 0, &rv));
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

void clicktoconference_play_ment_req_tcp_display(char *headerStr, clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_play_ment_req]\n"
		"  [confID] %s\n"
		"  [aupkg] %s"
		, headerStr
		, clicktoconference_play_ment_req->confID
		, clicktoconference_play_ment_req->aupkg);
}

void clicktoconference_play_ment_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_play_ment_req]\n"
		"  [confID] %s\n"
		"  [aupkg] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv));
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

void clicktoconference_get_party_status_req_tcp_display(char *headerStr, clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_play_ment_req]\n"
		"  [confID] %s\n"
		"  [participantName] %s\n"
		"  [participantNumber] %s"
		, headerStr
		, clicktoconference_get_party_status_req->confID
		, clicktoconference_get_party_status_req->participantName
		, clicktoconference_get_party_status_req->participantNumber);
}

void clicktoconference_get_party_status_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_get_party_status_req]\n"
		"  [confID] %s\n"
		"  [participantName] %s\n"
		"  [participantNumber] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv));
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

void clicktoconference_cancel_party_req_tcp_display(char *headerStr, clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktoconference_cancel_party_req]\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		, headerStr
		, clicktoconference_cancel_party_req->confID
		, clicktoconference_cancel_party_req->participantNumber);
}

void clicktoconference_cancel_party_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktoconference_cancel_party_req]\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv));
}