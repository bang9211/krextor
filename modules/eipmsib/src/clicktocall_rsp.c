#include "clicktocall_rsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_start_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_start_rsp_tcp_init( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp)
{
	memset(clicktocall_start_rsp, 0, sizeof(clicktocall_start_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocall_start_rsp_tcp_final( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp)
{
	//
}

int clicktocall_start_rsp_encode_to_dbif_msg(clicktocall_start_rsp_tcp_t *clicktocall_start_rsp, 
	char *sessionID, char *gwSessionID, uxc_dbif_t *dbif)
{
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocall_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_str( dbif, 0, sessionID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, gwSessionID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, clicktocall_start_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktocall_start_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, clicktocall_start_rsp->isRecording);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 5, clicktocall_start_rsp->recordingFileURL);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 6, clicktocall_start_rsp->recordingFileName);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_start_rsp_tcp_display(clicktocall_start_rsp_tcp_t *clicktocall_start_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocall_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocall_start_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_start_rsp->serviceID);
	ux_log(UXL_INFO, "  [isRecording] %d", clicktocall_start_rsp->isRecording);
	ux_log(UXL_INFO, "  [filler1] %d", clicktocall_start_rsp->filler1);
	ux_log(UXL_INFO, "  [filler2] %d", clicktocall_start_rsp->filler2);
	ux_log(UXL_INFO, "  [recordingFileURL] %s", clicktocall_start_rsp->recordingFileURL);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocall_start_rsp->recordingFileName);
}

void clicktocall_start_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_start_rsp]");
	ux_log(UXL_INFO, "  [sessionID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [gwSessionID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [isRecording] %d",uxc_dbif_get_int(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [recordingFileURL] %s",uxc_dbif_get_str(dbif, 5, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 6, &rv));
}


int clicktocall_stop_rsp_tcp_init( clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp) {
	memset(clicktocall_stop_rsp, 0, sizeof(clicktocall_stop_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocall_stop_rsp_tcp_final( clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp) {
	//
}

int clicktocall_stop_rsp_encode_to_dbif_msg( clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocall_stop_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocall_stop_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_stop_rsp_tcp_display(clicktocall_stop_rsp_tcp_t *clicktocall_stop_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocall_stop_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocall_stop_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_stop_rsp->serviceID);
}

void clicktocall_stop_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_stop_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 1, &rv));
}



int clicktocall_startrecording_rsp_tcp_init( clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp) {
	memset(clicktocall_startrecording_rsp, 0, sizeof(clicktocall_startrecording_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocall_startrecording_rsp_tcp_final( clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp) {
	//
}

int clicktocall_startrecording_rsp_encode_to_dbif_msg( clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocall_startrecording_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocall_startrecording_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktocall_startrecording_rsp->recordingFileURL);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktocall_startrecording_rsp->recordingFileName);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_startrecording_rsp_tcp_display(clicktocall_startrecording_rsp_tcp_t *clicktocall_startrecording_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocall_startrecording_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocall_startrecording_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_startrecording_rsp->serviceID);
	ux_log(UXL_INFO, "  [recordingFileURL] %s", clicktocall_startrecording_rsp->recordingFileURL);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocall_startrecording_rsp->recordingFileName);
}

void clicktocall_startrecording_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_startrecording_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [recordingFileURL] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 3, &rv));

}



int clicktocall_stoprecording_rsp_tcp_init( clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp) {
	memset(clicktocall_stoprecording_rsp, 0, sizeof(clicktocall_stoprecording_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktocall_stoprecording_rsp_tcp_final( clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp) {
	//
}

int clicktocall_stoprecording_rsp_encode_to_dbif_msg( clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocall_stoprecording_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocall_stoprecording_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final;

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_stoprecording_rsp_tcp_display(clicktocall_stoprecording_rsp_tcp_t *clicktocall_stoprecording_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocall_stoprecording_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocall_stoprecording_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_stoprecording_rsp->serviceID);
}

void clicktocall_stoprecording_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocall_stoprecording_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 1, &rv));
}


int clicktocall_service_status_rsp_tcp_init( clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp) {
	memset(clicktocall_service_status_rsp, 0, sizeof(clicktocall_service_status_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_service_status_rsp_tcp_final( clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp) {
	// 
}

void clicktocall_service_status_rsp_tcp_display(clicktocall_service_status_rsp_tcp_t *clicktocall_service_status_rsp) {
	ux_log(UXL_INFO, "TCP [clicktocall_service_status_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktocall_service_status_rsp->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocall_service_status_rsp->serviceID);
	ux_log(UXL_INFO, "  [status] %d", clicktocall_service_status_rsp->status);
}

