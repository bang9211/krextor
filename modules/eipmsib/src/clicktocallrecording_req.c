#include "clicktocallrecording_req.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_binding_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_binding_req_tcp_init( clicktocallrecording_binding_req_tcp_t *clicktocallrecording_binding_req)
{
	memset(clicktocallrecording_binding_req, 0, sizeof(clicktocallrecording_binding_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_binding_req_tcp_final( clicktocallrecording_binding_req_tcp_t *clicktocallrecording_binding_req)
{
	return;
}

void clicktocallrecording_binding_req_tcp_display(clicktocallrecording_binding_req_tcp_t *clicktocallrecording_binding_req)
{
	ux_log(UXL_INFO, 
		"TCP [clicktocallrecording_binding_req]"
		"  [userID] %s"
		"  [password] %s", 
		clicktocallrecording_binding_req->userID,
		clicktocallrecording_binding_req->password);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_start_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_start_req_tcp_init( clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req) {
	memset(clicktocallrecording_start_req, 0, sizeof(clicktocallrecording_start_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_start_req_tcp_final( clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req) {
    return;
}

int clicktocallrecording_start_req_decode_dbif_msg( clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif) {
	int rv;
	
	strncpy(sessionID, uxc_dbif_get_str(dbif, 0, &rv), SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(gwSessionID, uxc_dbif_get_str(dbif, 1, &rv), GW_SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->subscriberName, uxc_dbif_get_str(dbif, 2, &rv), SUBSCRIBER_NAME_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->callingNumber, uxc_dbif_get_str(dbif, 3, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->calledNumber, uxc_dbif_get_str(dbif, 4, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->callerInfo, uxc_dbif_get_str(dbif, 5, &rv), CALLER_INFO_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->calleeInfo, uxc_dbif_get_str(dbif, 6, &rv), CALLEE_INFO_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	clicktocallrecording_start_req->serviceCode = uxc_dbif_get_int(dbif, 7, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->recordingFileName, uxc_dbif_get_str(dbif, 8, &rv), RECORDING_FILE_NAME_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->number050, uxc_dbif_get_str(dbif, 9, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(clicktocallrecording_start_req->chargingNumber, "", CHARGING_NUMBER_LEN);
	clicktocallrecording_start_req->filler1 = 0;
	clicktocallrecording_start_req->filler2 = 0;
	strncpy(clicktocallrecording_start_req->filler3, "", 8);

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocall_start_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_start_req_tcp_display(clicktocallrecording_start_req_tcp_t *clicktocallrecording_start_req) {
	ux_log(UXL_INFO, 
		"TCP [clicktocallrecording_start_req]\n"
		"  [subscriberName] %s\n"
		"  [callingNumber] %s\n"
		"  [calledNumber] %s\n"
		"  [callerInfo] %s\n"
		"  [calleeInfo] %s\n"
		"  [chargingNumber] %s\n"
		"  [serviceCode] %d\n"
		"  [filler1] %d\n"
		"  [filler2] %d\n"
		"  [recordingFileName] %s\n"
		"  [number050] %s\n"
		"  [filler3] %s"
		, clicktocallrecording_start_req->subscriberName
		, clicktocallrecording_start_req->callingNumber
		, clicktocallrecording_start_req->calledNumber
		, clicktocallrecording_start_req->callerInfo
		, clicktocallrecording_start_req->calleeInfo
		, clicktocallrecording_start_req->chargingNumber
		, clicktocallrecording_start_req->serviceCode
		, clicktocallrecording_start_req->filler1
		, clicktocallrecording_start_req->filler2
		, clicktocallrecording_start_req->recordingFileName
		, clicktocallrecording_start_req->number050
		, clicktocallrecording_start_req->filler3);
}

void clicktocallrecording_start_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktocallrecording_start_req]\n"
		"  [sessionID] %s\n"
		"  [gwSessionID] %s\n"
		"  [subscriberName] %s\n"
		"  [callingNumber] %s\n"
		"  [calledNumber] %s\n"
		"  [callerInfo] %s\n"
		"  [calleeInfo] %s\n"
		"  [serviceCode] %d\n"
		"  [recordingFileName] %s\n"
		"  [number050] %s"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv)
		,uxc_dbif_get_str(dbif, 3, &rv)
		,uxc_dbif_get_str(dbif, 4, &rv)
		,uxc_dbif_get_str(dbif, 5, &rv)
		,uxc_dbif_get_str(dbif, 6, &rv)
		,uxc_dbif_get_int(dbif, 7, &rv)
		,uxc_dbif_get_str(dbif, 8, &rv)
		,uxc_dbif_get_str(dbif, 9, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_start_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_stop_req_tcp_init( clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req) {
    memset(clicktocallrecording_stop_req, 0, sizeof(clicktocallrecording_stop_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_stop_req_tcp_final( clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req) {
    return;
}

int clicktocallrecording_stop_req_decode_dbif_msg( clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req, uxc_dbif_t *dbif) {
int rv;
	
	strncpy(clicktocallrecording_stop_req->serviceID, uxc_dbif_get_str(dbif, 0, &rv), SERVICE_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	
	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "clicktocallrecording_stop_req_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_stop_req_tcp_display(clicktocallrecording_stop_req_tcp_t *clicktocallrecording_stop_req) {
	ux_log(UXL_INFO, 
		"TCP [clicktocallrecording_stop_req]\n"
		"  [serviceID] %s"
		, clicktocallrecording_stop_req->serviceID);
}

void clicktocallrecording_stop_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktocallrecording_stop_req]\n"
		"  [serviceID] %s"
		,uxc_dbif_get_str(dbif, 0, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_start_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_service_status_req_tcp_init( clicktocallrecording_service_status_req_tcp_t *clicktocallrecording_service_status_req) {
    memset(clicktocallrecording_service_status_req, 0, sizeof(clicktocallrecording_service_status_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_service_status_req_tcp_final( clicktocallrecording_service_status_req_tcp_t *clicktocallrecording_service_status_req) {
    return;
}

void clicktocallrecording_service_status_req_tcp_display(clicktocallrecording_service_status_req_tcp_t *clicktocallrecording_service_status_req) {
	ux_log(UXL_INFO, 
		"TCP [clicktocallrecording_service_status_req]\n"
		"  [serviceID] %s"
		, clicktocallrecording_service_status_req->serviceID);
}
