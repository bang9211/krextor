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
	ux_log(UXL_INFO, "TCP [clicktocallrecording_binding_req]");
	ux_log(UXL_INFO, "  [userID] %s", clicktocallrecording_binding_req->userID);
	ux_log(UXL_INFO, "  [password] %s", clicktocallrecording_binding_req->password);
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
	ux_log(UXL_INFO, "TCP [clicktocallrecording_start_req]");
	ux_log(UXL_INFO, "  [subscriberName] %s", clicktocallrecording_start_req->subscriberName);
	ux_log(UXL_INFO, "  [callingNumber] %s", clicktocallrecording_start_req->callingNumber);
	ux_log(UXL_INFO, "  [calledNumber] %s", clicktocallrecording_start_req->calledNumber);
	ux_log(UXL_INFO, "  [callerInfo] %s", clicktocallrecording_start_req->callerInfo);
	ux_log(UXL_INFO, "  [calleeInfo] %s", clicktocallrecording_start_req->calleeInfo);
	ux_log(UXL_INFO, "  [chargingNumber] %s", clicktocallrecording_start_req->chargingNumber);
	ux_log(UXL_INFO, "  [serviceCode] %d", clicktocallrecording_start_req->serviceCode);
	ux_log(UXL_INFO, "  [filler1] %d", clicktocallrecording_start_req->filler1);
	ux_log(UXL_INFO, "  [filler2] %d", clicktocallrecording_start_req->filler2);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocallrecording_start_req->recordingFileName);
	ux_log(UXL_INFO, "  [number050] %s", clicktocallrecording_start_req->number050);
	ux_log(UXL_INFO, "  [filler3] %s", clicktocallrecording_start_req->filler3);
}

void clicktocallrecording_start_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocallrecording_start_req]");
	ux_log(UXL_INFO, "  [sessionID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [gwSessionID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [subscriberName] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [callingNumber] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [calledNumber] %s",uxc_dbif_get_str(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [callerInfo] %s",uxc_dbif_get_str(dbif, 5, &rv));
	ux_log(UXL_INFO, "  [calleeInfo] %s",uxc_dbif_get_str(dbif, 6, &rv));
	ux_log(UXL_INFO, "  [serviceCode] %d",uxc_dbif_get_int(dbif, 7, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 8, &rv));
	ux_log(UXL_INFO, "  [number050] %s",uxc_dbif_get_str(dbif, 9, &rv));
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
	ux_log(UXL_INFO, "TCP [clicktocallrecording_stop_req]");
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_stop_req->serviceID);
}

void clicktocallrecording_stop_req_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocallrecording_stop_req]");
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 0, &rv));
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
	ux_log(UXL_INFO, "TCP [clicktocallrecording_service_status_req]");
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_service_status_req->serviceID);
}
