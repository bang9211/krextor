#include "clicktocallrecording_rpt.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_start_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_start_rpt_tcp_init( clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt) {
	memset(clicktocallrecording_start_rpt, 0, sizeof(clicktocallrecording_start_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_start_rpt_tcp_final( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt) {
    //
}

int clicktocallrecording_start_rpt_encode_to_dbif_msg( clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt, uxc_dbif_t *dbif) {
    int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocallrecording_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocallrecording_start_rpt->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocallrecording_start_rpt->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktocallrecording_start_rpt->recordingFileURL);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktocallrecording_start_rpt->recordingFileName);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_start_rpt_tcp_display(clicktocallrecording_start_rpt_tcp_t *clicktocallrecording_start_rpt) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_start_rpt]");
	ux_log(UXL_INFO, "  [returnCode] %d", clicktocallrecording_start_rpt->resultCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_start_rpt->serviceID);
	ux_log(UXL_INFO, "  [recordingFileURL] %s", clicktocallrecording_start_rpt->recordingFileURL);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocallrecording_start_rpt->recordingFileName);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_stop_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_stop_rpt_tcp_init( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt) {
	memset(clicktocallrecording_stop_rpt, 0, sizeof(clicktocallrecording_stop_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_stop_rpt_tcp_final( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt) {
    //
}

int clicktocallrecording_stop_rpt_encode_to_dbif_msg( clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocallrecording_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocallrecording_stop_rpt->returnCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocallrecording_stop_rpt->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktocallrecording_stop_rpt->startTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktocallrecording_stop_rpt->endTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 4, clicktocallrecording_stop_rpt->callingNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 5, clicktocallrecording_stop_rpt->calledNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 6, clicktocallrecording_stop_rpt->isRecorded);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 7, clicktocallrecording_stop_rpt->recordingFileURL);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 8, clicktocallrecording_stop_rpt->recordingFileName);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_stop_rpt_tcp_display(clicktocallrecording_stop_rpt_tcp_t *clicktocallrecording_stop_rpt) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_stop_rpt]");
	ux_log(UXL_INFO, "  [returnCode] %d", clicktocallrecording_stop_rpt->returnCode);
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_stop_rpt->serviceID);
	ux_log(UXL_INFO, "  [startTime] %s", clicktocallrecording_stop_rpt->startTime);
	ux_log(UXL_INFO, "  [endTime] %s", clicktocallrecording_stop_rpt->endTime);
	ux_log(UXL_INFO, "  [callingNumber] %s", clicktocallrecording_stop_rpt->callingNumber);
	ux_log(UXL_INFO, "  [calledNumber] %s", clicktocallrecording_stop_rpt->calledNumber);
	ux_log(UXL_INFO, "  [isRecorded] %d", clicktocallrecording_stop_rpt->isRecorded);
	ux_log(UXL_INFO, "  [recordingFileURL] %s", clicktocallrecording_stop_rpt->recordingFileURL);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktocallrecording_stop_rpt->recordingFileName);
}

void clicktocallrecording_stop_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocallrecording_stop_rpt]");
	ux_log(UXL_INFO, "  [returnCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [startTime] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [endTime] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [callingNumber] %s",uxc_dbif_get_str(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [calledNumber] %s",uxc_dbif_get_str(dbif, 5, &rv));
	ux_log(UXL_INFO, "  [isRecorded] %d",uxc_dbif_get_int(dbif, 6, &rv));
	ux_log(UXL_INFO, "  [recordingFileURL] %s",uxc_dbif_get_str(dbif, 7, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 8, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocallrecording_service_status_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocallrecording_service_status_rpt_tcp_init( clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt) {
	memset(clicktocallrecording_service_status_rpt, 0, sizeof(clicktocallrecording_service_status_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocallrecording_service_status_rpt_tcp_final( clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt) {
	// 
}

int clicktocallrecording_service_status_rpt_encode_to_dbif_msg( clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocallrecording_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_str( dbif, 0, clicktocallrecording_service_status_rpt->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 1, clicktocallrecording_service_status_rpt->status);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocallrecording_service_status_rpt_tcp_display(clicktocallrecording_service_status_rpt_tcp_t *clicktocallrecording_service_status_rpt) {
	ux_log(UXL_INFO, "TCP [clicktocallrecording_service_status_rpt]");
	ux_log(UXL_INFO, "  [serviceID] %s", clicktocallrecording_service_status_rpt->serviceID);
	ux_log(UXL_INFO, "  [status] %d", clicktocallrecording_service_status_rpt->status);
}

void clicktocallrecording_service_status_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktocallrecording_service_status_rpt]");
	ux_log(UXL_INFO, "  [serviceID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [status] %d",uxc_dbif_get_int(dbif, 1, &rv));
}
