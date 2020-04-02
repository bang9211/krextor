#include "clicktocall_req.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_binding_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_binding_req_tcp_init( clicktocall_binding_req_tcp_t *clicktocall_binding_req)
{
	memset(clicktocall_binding_req, 0, sizeof(clicktocall_binding_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_binding_req_tcp_final( clicktocall_binding_req_tcp_t *clicktocall_binding_req)
{
	return;
}

void clicktocall_binding_req_tcp_display(char *headerStr, clicktocall_binding_req_tcp_t *clicktocall_binding_req)
{
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_binding_req]\n"
		"  [userID] %s\n"
		"  [password] %s", 
		headerStr,
		clicktocall_binding_req->userID, 
		clicktocall_binding_req->password);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_start_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_start_req_tcp_init( clicktocall_start_req_tcp_t *clicktocall_start_req)
{
	memset(clicktocall_start_req, 0, sizeof(clicktocall_start_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_start_req_tcp_final( clicktocall_start_req_tcp_t *clicktocall_start_req)
{
	return;
}

int clicktocall_start_req_decode_dbif_msg( clicktocall_start_req_tcp_t *clicktocall_start_req, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif)
{
	int rv;
	
	strncpy(sessionID, uxc_dbif_get_str(dbif, 0, &rv), SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(gwSessionID, uxc_dbif_get_str(dbif, 1, &rv), GW_SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->subscriberName, uxc_dbif_get_str(dbif, 2, &rv), SUBSCRIBER_NAME_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->recordingType = uxc_dbif_get_int(dbif, 3, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->callingNumber, uxc_dbif_get_str(dbif, 4, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->calledNumber, uxc_dbif_get_str(dbif, 5, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->serviceCode = uxc_dbif_get_int(dbif, 6, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->ringBackToneType = uxc_dbif_get_int(dbif, 7, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->waitingMentID = uxc_dbif_get_int(dbif, 8, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->scenarioType = uxc_dbif_get_int(dbif, 9, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->callMentID = uxc_dbif_get_int(dbif, 10, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->callingCID, uxc_dbif_get_str(dbif, 11, &rv), CID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->calledCID, uxc_dbif_get_str(dbif, 12, &rv), CID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->recordingFileName, uxc_dbif_get_str(dbif, 13, &rv), CID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->isAllRecording = uxc_dbif_get_int(dbif, 14, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->endIfRecordingFailed = uxc_dbif_get_int(dbif, 15, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->endIfRecordingEnded = uxc_dbif_get_int(dbif, 16, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->hostingCode = uxc_dbif_get_int(dbif, 17, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->wirelessTimeout = uxc_dbif_get_int(dbif, 18, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocall_start_req->wiredTimeout = uxc_dbif_get_int(dbif, 19, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_start_req->chargingNumber, "", CHARGING_NUMBER_LEN);
	clicktocall_start_req->filler1 = 0;
	clicktocall_start_req->filler2 = 0;
	strncpy(clicktocall_start_req->filler3, "", 32);

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_start_req_tcp_display(char *headerStr, clicktocall_start_req_tcp_t *clicktocall_start_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_start_req]\n"
		"  [subscriberName] %s\n"
		"  [recordingType] %d\n"
		"  [callingNumber] %s\n"
		"  [calledNumber] %s\n"
		"  [chargingNumber] %s\n"
		"  [serviceCode] %d\n"
		"  [ringBackToneType] %d\n"
		"  [waitingMentID] %d\n"
		"  [filler1] %d\n"
		"  [scenarioType] %d\n"
		"  [callMentID] %d\n"
		"  [callingCID] %s\n"
		"  [calledCID] %s\n"
		"  [recordingFileName] %s\n"
		"  [isAllRecording] %d\n"
		"  [endIfRecordingFailed] %d\n"
		"  [endIfRecordingEnded] %d\n"
		"  [hostingCode] %d\n"
		"  [wirelessTimeout] %d\n"
		"  [wiredTimeout] %d\n"
		"  [filler2] %d\n"
		"  [filler3] %s",
		headerStr,
		clicktocall_start_req->subscriberName,
		clicktocall_start_req->recordingType,
		clicktocall_start_req->callingNumber,
		clicktocall_start_req->calledNumber,
		clicktocall_start_req->chargingNumber,
		clicktocall_start_req->serviceCode,
		clicktocall_start_req->ringBackToneType,
		clicktocall_start_req->waitingMentID,
		clicktocall_start_req->filler1,
		clicktocall_start_req->scenarioType,
		clicktocall_start_req->callMentID,
		clicktocall_start_req->callingCID,
		clicktocall_start_req->calledCID,
		clicktocall_start_req->recordingFileName,
		clicktocall_start_req->isAllRecording,
		clicktocall_start_req->endIfRecordingFailed,
		clicktocall_start_req->endIfRecordingEnded,
		clicktocall_start_req->hostingCode,
		clicktocall_start_req->wirelessTimeout,
		clicktocall_start_req->wiredTimeout,
		clicktocall_start_req->filler2,
		clicktocall_start_req->filler3);
}

void clicktocall_start_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktocall_start_req]\n"
		"  [sessionID] %s\n"
		"  [gwSessionID] %s\n"
		"  [subscriberName] %s\n"
		"  [recordingType] %d\n"
		"  [callingNumber] %s\n"
		"  [calledNumber] %s\n"
		"  [serviceCode] %d\n"
		"  [ringBackToneType] %d\n"
		"  [waitingMentID] %d\n"
		"  [scenarioType] %d\n"
		"  [callMentID] %d\n"
		"  [callingCID] %s\n"
		"  [calledCID] %s\n"
		"  [recordingFileName] %s\n"
		"  [isAllRecording] %d\n"
		"  [endIfRecordingFailed] %d\n"
		"  [endIfRecordingEnded] %d\n"
		"  [hostingCode] %d\n"
		"  [wirelessTimeout] %d\n"
		"  [wiredTimeout] %d",
		uxc_dbif_get_str(dbif, 0, &rv),
		uxc_dbif_get_str(dbif, 1, &rv),
		uxc_dbif_get_str(dbif, 2, &rv),
		uxc_dbif_get_int(dbif, 3, &rv),
		uxc_dbif_get_str(dbif, 4, &rv),
		uxc_dbif_get_str(dbif, 5, &rv),
		uxc_dbif_get_int(dbif, 6, &rv),
		uxc_dbif_get_int(dbif, 7, &rv),
		uxc_dbif_get_int(dbif, 8, &rv),
		uxc_dbif_get_int(dbif, 9, &rv),
		uxc_dbif_get_int(dbif, 10, &rv),
		uxc_dbif_get_str(dbif, 11, &rv),
		uxc_dbif_get_str(dbif, 12, &rv),
		uxc_dbif_get_str(dbif, 13, &rv),
		uxc_dbif_get_int(dbif, 14, &rv),
		uxc_dbif_get_int(dbif, 15, &rv),
		uxc_dbif_get_int(dbif, 16, &rv),
		uxc_dbif_get_int(dbif, 17, &rv),
		uxc_dbif_get_int(dbif, 18, &rv),
		uxc_dbif_get_int(dbif, 19, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_stop_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stop_req_tcp_init( clicktocall_stop_req_tcp_t *clicktocall_stop_req)
{
	memset(clicktocall_stop_req, 0, sizeof(clicktocall_stop_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_stop_req_tcp_final( clicktocall_stop_req_tcp_t *clicktocall_stop_req)
{
	return;
}

int clicktocall_stop_req_decode_dbif_msg( clicktocall_stop_req_tcp_t *clicktocall_stop_req, uxc_dbif_t *dbif)
{
	int rv;
	strncpy(clicktocall_stop_req->serviceID, uxc_dbif_get_str(dbif, 0, &rv), SERVICE_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_stop_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_stop_req_tcp_display(char *headerStr, clicktocall_stop_req_tcp_t *clicktocall_stop_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_stop_req]\n"
		"  [serviceID] %s", 
		headerStr,
		clicktocall_stop_req->serviceID);
}

void clicktocall_stop_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktocall_stop_req]\n"
		"  [serviceID] %s",
		uxc_dbif_get_str(dbif, 0, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_startrecording_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_startrecording_req_tcp_init( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req)
{
	memset(clicktocall_startrecording_req, 0, sizeof(clicktocall_startrecording_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_startrecording_req_tcp_final( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req)
{
	return;
}

int clicktocall_startrecording_req_decode_dbif_msg( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req, uxc_dbif_t *dbif)
{
	int rv;
	strncpy(clicktocall_startrecording_req->serviceID, uxc_dbif_get_str(dbif, 0, &rv), SERVICE_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocall_startrecording_req->recordingFileName, uxc_dbif_get_str(dbif, 1, &rv), RECORDING_FILE_NAME_LEN);
	if( rv < eUXC_SUCCESS) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_startrecording_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_startrecording_req_tcp_display(char *headerStr, clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_startrecording_req]\n"
		"  [serviceID] %s\n"
		"  [recordingFileName] %s", 
		headerStr,
		clicktocall_startrecording_req->serviceID, 
		clicktocall_startrecording_req->recordingFileName);
}

void clicktocall_startrecording_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktocall_startrecording_req]\n"
		"  [serviceID] %s",
		uxc_dbif_get_str(dbif, 0, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_stoprecording_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stoprecording_req_tcp_init( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req)
{
	memset(clicktocall_stoprecording_req, 0, sizeof(clicktocall_stoprecording_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_stoprecording_req_tcp_final( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req)
{
	return;
}

int clicktocall_stoprecording_req_decode_dbif_msg( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req, uxc_dbif_t *dbif)
{
	int rv;
	strncpy(clicktocall_stoprecording_req->serviceID, uxc_dbif_get_str(dbif, 0, &rv), SERVICE_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_stoprecording_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_stoprecording_req_tcp_display(char *headerStr, clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_stoprecording_req]\n"
		"  [serviceID] %s", 
		headerStr,
		clicktocall_stoprecording_req->serviceID);
}

void clicktocall_stoprecording_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[clicktocall_stoprecording_req]\n"
		"  [serviceID] %s",
		uxc_dbif_get_str(dbif, 0, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_service_status_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_service_status_req_tcp_init( clicktocall_service_status_req_tcp_t *clicktocall_service_status_req)
{
	memset(clicktocall_service_status_req, 0, sizeof(clicktocall_service_status_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_service_status_req_tcp_final( clicktocall_service_status_req_tcp_t *clicktocall_service_status_req)
{
	return;
}

void clicktocall_service_status_req_tcp_display(char *headerStr, clicktocall_service_status_req_tcp_t *clicktocall_service_status_req) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_service_status_req]\n"
		"  [serviceID] %s", 
		headerStr,
		clicktocall_service_status_req->serviceID);
}