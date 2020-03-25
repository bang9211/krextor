#include "clicktocall_req.h"

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
	clicktocall_start_req->fillerInt8 = 0;
	clicktocall_start_req->fillerInt16 = 0;
	strncpy(clicktocall_start_req->filler, "", 32);

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_start_req_tcp_display(clicktocall_start_req_tcp_t *clicktocall_start_req) {
	ux_log(UXL_INFO, "TCP [clicktocall_start_req]");
	ux_log(UXL_INFO, "  [subscriberName] %s", clicktocall_start_req->subscriberName);
	ux_log(UXL_INFO, "  [recordingType] %d", clicktocall_start_req->recordingType);
	ux_log(UXL_INFO, "  [callingNumber] %s", clicktocall_start_req->callingNumber);
	ux_log(UXL_INFO, "  [calledNumber] %s", clicktocall_start_req->calledNumber);
	ux_log(UXL_INFO, "  [chargningNumber] %s", clicktocall_start_req->chargingNumber);
	ux_log(UXL_INFO, "  [serviceCode] %d", clicktocall_start_req->serviceCode);
	ux_log(UXL_INFO, "  [ringBackToneType] %d", clicktocall_start_req->ringBackToneType);
	ux_log(UXL_INFO, "  [waitingMentID] %d", clicktocall_start_req->waitingMentID);
	ux_log(UXL_INFO, "  [fillerInt8] %d", clicktocall_start_req->fillerInt8);
	ux_log(UXL_INFO, "  [scenarioType] %d", clicktocall_start_req->scenarioType);
	ux_log(UXL_INFO, "  [callMentID] %d", clicktocall_start_req->callMentID);
	ux_log(UXL_INFO, "  [callingCID] %s", clicktocall_start_req->callingCID);
	ux_log(UXL_INFO, "  [calledCID] %s", clicktocall_start_req->calledCID);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocall_start_req->recordingFileName);
	ux_log(UXL_INFO, "  [isAllRecording] %d", clicktocall_start_req->isAllRecording);
	ux_log(UXL_INFO, "  [endIfRecordingFailed] %d", clicktocall_start_req->endIfRecordingFailed);
	ux_log(UXL_INFO, "  [endIfRecordingEnded] %d", clicktocall_start_req->endIfRecordingEnded);
	ux_log(UXL_INFO, "  [hostingCode] %d", clicktocall_start_req->hostingCode);
	ux_log(UXL_INFO, "  [wirelessTimeout] %d", clicktocall_start_req->wirelessTimeout);
	ux_log(UXL_INFO, "  [wiredTimeout] %d", clicktocall_start_req->wiredTimeout);
	ux_log(UXL_INFO, "  [fillerInt16] %d", clicktocall_start_req->fillerInt16);
	ux_log(UXL_INFO, "  [filler] %s", clicktocall_start_req->filler);
}

void clicktocall_start_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_start_req]");
	ux_log(UXL_INFO, "  [sessionID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [gwSessionID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [subscriberName] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [recordingType] %d",uxc_dbif_get_int(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [callingNumber] %s",uxc_dbif_get_str(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [calledNumber] %s",uxc_dbif_get_str(dbif, 5, &rv));
	ux_log(UXL_INFO, "  [serviceCode] %d",uxc_dbif_get_int(dbif, 6, &rv));
	ux_log(UXL_INFO, "  [ringBackToneType] %d",uxc_dbif_get_int(dbif, 7, &rv));
	ux_log(UXL_INFO, "  [waitingMentID] %d",uxc_dbif_get_int(dbif, 8, &rv));
	ux_log(UXL_INFO, "  [scenarioType] %d",uxc_dbif_get_int(dbif, 9, &rv));
	ux_log(UXL_INFO, "  [callMentID] %d",uxc_dbif_get_int(dbif, 10, &rv));
	ux_log(UXL_INFO, "  [callingCID] %s",uxc_dbif_get_str(dbif, 11, &rv));
	ux_log(UXL_INFO, "  [calledCID] %s",uxc_dbif_get_str(dbif, 12, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 13, &rv));
	ux_log(UXL_INFO, "  [isAllRecording] %d",uxc_dbif_get_int(dbif, 14, &rv));
	ux_log(UXL_INFO, "  [endIfRecordingFailed] %d",uxc_dbif_get_int(dbif, 15, &rv));
	ux_log(UXL_INFO, "  [endIfRecordingEnded] %d",uxc_dbif_get_int(dbif, 16, &rv));
	ux_log(UXL_INFO, "  [hostingCode] %d",uxc_dbif_get_int(dbif, 17, &rv));
	ux_log(UXL_INFO, "  [wirelessTimeout] %d",uxc_dbif_get_int(dbif, 18, &rv));
	ux_log(UXL_INFO, "  [wiredTimeout] %d",uxc_dbif_get_int(dbif, 19, &rv));
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

void clicktocall_stop_req_tcp_display(clicktocall_stop_req_tcp_t *clicktocall_stop_req) {
	ux_log(UXL_INFO, "TCP [clicktocall_stop_req]");
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_stop_req->serviceID);
}

void clicktocall_stop_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_stop_req]");
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 0, &rv));
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

void clicktocall_startrecording_req_tcp_display(clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req) {
	ux_log(UXL_INFO, "TCP [clicktocall_startrecording_req]");
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_startrecording_req->serviceID);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocall_startrecording_req->recordingFileName);
}

void clicktocall_startrecording_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_startrecording_req]");
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 0, &rv));
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
void clicktocall_stoprecording_req_tcp_display(clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req) {
	ux_log(UXL_INFO, "TCP [clicktocall_stoprecording_req]");
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_stoprecording_req->serviceID);
}

void clicktocall_stoprecording_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_stoprecording_req]");
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 0, &rv));
}