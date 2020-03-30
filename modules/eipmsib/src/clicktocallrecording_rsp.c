#include "clicktocallrecording_rsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_binding_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_binding_rsp_tcp_init( clicktocallrecording_binding_rsp_tcp_t *clicktocallrecording_binding_rsp)
{
	memset(clicktocallrecording_binding_rsp, 0, sizeof(clicktocallrecording_binding_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocallrecording_binding_rsp_tcp_final( clicktocallrecording_binding_rsp_tcp_t *clicktocallrecording_binding_rsp)
{
	//
}

void clicktocallrecording_binding_rsp_tcp_display(clicktocallrecording_binding_rsp_tcp_t *clicktocallrecording_binding_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_binding_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocallrecording_binding_rsp->resultCode);
	ux_log(UXL_INFO, "  [userID] %s", clicktocallrecording_binding_rsp->userID);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_start_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_start_rsp_tcp_init( clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp)
{
	memset(clicktocallrecording_start_rsp, 0, sizeof(clicktocallrecording_start_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocallrecording_start_rsp_tcp_final( clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp)
{
	//
}

int clicktocallrecording_start_rsp_encode_to_dbif_msg(clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp, 
	char *sessionID, char *gwSessionID, uxc_dbif_t *dbif)
{
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocallrecording_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_str( dbif, 0, sessionID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, gwSessionID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, clicktocallrecording_start_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktocallrecording_start_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 4, clicktocallrecording_start_rsp->recordingFileURL);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 5, clicktocallrecording_start_rsp->recordingFileName);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_start_rsp_tcp_display(clicktocallrecording_start_rsp_tcp_t *clicktocallrecording_start_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocallrecording_start_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_start_rsp->serviceID);
	ux_log(UXL_INFO, "  [recordingFileURL] %s", clicktocallrecording_start_rsp->recordingFileURL);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocallrecording_start_rsp->recordingFileName);
}

void clicktocallrecording_start_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocallrecording_start_rsp]");
	ux_log(UXL_INFO, "  [sessionID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [gwSessionID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [recordingFileURL] %s",uxc_dbif_get_str(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 5, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_stop_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_stop_rsp_tcp_init( clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp) {
	memset(clicktocallrecording_stop_rsp, 0, sizeof(clicktocallrecording_stop_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocallrecording_stop_rsp_tcp_final( clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp) {
	//
}

int clicktocallrecording_stop_rsp_encode_to_dbif_msg( clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocallrecording_stop_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocallrecording_stop_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_stop_rsp_tcp_display(clicktocallrecording_stop_rsp_tcp_t *clicktocallrecording_stop_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_stop_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocallrecording_stop_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_stop_rsp->serviceID);
}

void clicktocallrecording_stop_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocallrecording_stop_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 1, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_service_status_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_service_status_rsp_tcp_init( clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp) {
	memset(clicktocallrecording_service_status_rsp, 0, sizeof(clicktocallrecording_service_status_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_service_status_rsp_tcp_final( clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp) {
	// 
}

void clicktocallrecording_service_status_rsp_tcp_display(clicktocallrecording_service_status_rsp_tcp_t *clicktocallrecording_service_status_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_service_status_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocallrecording_service_status_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_service_status_rsp->serviceID);
	ux_log(UXL_INFO, "  [status] %d", clicktocallrecording_service_status_rsp->status);
}

