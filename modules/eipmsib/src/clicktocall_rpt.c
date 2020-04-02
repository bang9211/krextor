#include "clicktocall_rpt.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_stop_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stop_rpt_tcp_init( clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt) {
	memset(clicktocall_stop_rpt, 0, sizeof(clicktocall_stop_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_stop_rpt_tcp_final( clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt) {
    //
}

int clicktocall_stop_rpt_encode_to_dbif_msg( clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocall_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktocall_stop_rpt->returnCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocall_stop_rpt->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktocall_stop_rpt->startTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktocall_stop_rpt->endTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, clicktocall_stop_rpt->isRecorded);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_stop_rpt_tcp_display(char *headerStr, clicktocall_stop_rpt_tcp_t *clicktocall_stop_rpt) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_stop_rpt]\n"
		"  [returnCode] %d\n"
		"  [serviceID] %s\n"
		"  [startTime] %s\n"
		"  [endTime] %s\n"
		"  [isRecorded] %d\n"
		, headerStr
		, clicktocall_stop_rpt->returnCode
		, clicktocall_stop_rpt->serviceID
		, clicktocall_stop_rpt->startTime
		, clicktocall_stop_rpt->endTime
		, clicktocall_stop_rpt->isRecorded);
}

void clicktocall_stop_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Sending DBIF\n"
		"[clicktocall_stop_rpt]\n"
		"  [returnCode] %d\n"
		"  [serviceID] %s\n"
		"  [startTime] %s\n"
		"  [endTime] %s\n"
		"  [isRecorded] %d"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv)
		,uxc_dbif_get_str(dbif, 3, &rv)
		,uxc_dbif_get_int(dbif, 4, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_startrecording_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////


int clicktocall_startrecording_rpt_tcp_init( clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt) {
	memset(clicktocall_startrecording_rpt, 0, sizeof(clicktocall_startrecording_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_startrecording_rpt_tcp_final( clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt) {
	// 
}

void clicktocall_startrecording_rpt_tcp_display(char *headerStr, clicktocall_startrecording_rpt_tcp_t *clicktocall_startrecording_rpt) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_startrecording_rpt]\n"
		"  [resultCode] %d\n"
		"  [serviceID] %s", 
		headerStr,
		clicktocall_startrecording_rpt->resultCode,
		clicktocall_startrecording_rpt->serviceID);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_stoprecording_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stoprecording_rpt_tcp_init( clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt) {
	memset(clicktocall_stoprecording_rpt, 0, sizeof(clicktocall_stoprecording_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_stoprecording_rpt_tcp_final( clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt) {
	// 
}

void clicktocall_stoprecording_rpt_tcp_display(char *headerStr, clicktocall_stoprecording_rpt_tcp_t *clicktocall_stoprecording_rpt) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_stoprecording_rpt]\n"
		"  [resultCode] %d\n"
		"  [serviceID] %s\n",
		headerStr,
		clicktocall_stoprecording_rpt->resultCode,
		clicktocall_stoprecording_rpt->serviceID);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_service_status_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_service_status_rpt_tcp_init( clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt) {
	memset(clicktocall_service_status_rpt, 0, sizeof(clicktocall_service_status_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_service_status_rpt_tcp_final( clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt) {
	// 
}

int clicktocall_service_status_rpt_encode_to_dbif_msg( clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktocall_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_str( dbif, 0, clicktocall_service_status_rpt->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 1, clicktocall_service_status_rpt->status);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktocall_service_status_rpt_tcp_display(char *headerStr, clicktocall_service_status_rpt_tcp_t *clicktocall_service_status_rpt) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [clicktocall_service_status_rpt]\n"
		"  [serviceID] %s\n"
		"  [status] %d"
		, headerStr
		, clicktocall_service_status_rpt->serviceID
		, clicktocall_service_status_rpt->status);
}

void clicktocall_service_status_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Sending DBIF\n"
		"[clicktocall_service_status_rpt]\n"
		"  [serviceID] %s\n"
		"  [status] %d"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_int(dbif, 1, &rv));
}
