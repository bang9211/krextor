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

int clicktocall_start_rsp_encode_to_dbif_msg( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp, uxc_dbif_t *dbif)
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

	rv = uxc_dbif_set_int( dbif, 0, clicktocall_start_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktocall_start_rsp->serviceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, clicktocall_start_rsp->isRecording);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, clicktocall_start_rsp->filler1);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, clicktocall_start_rsp->filler2);
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



int clicktocall_stoprecording_rsp_tcp_init( tcp_clicktocall_stoprecording_rsp_t *clicktocall_stoprecording_rsp) {
	memset(clicktocall_stoprecording_rsp, 0, sizeof(tcp_clicktocall_stoprecording_rsp_t));

	return eUXC_SUCCESS;
}

void clicktocall_stoprecording_rsp_tcp_final( tcp_clicktocall_stoprecording_rsp_t *clicktocall_stoprecording_rsp) {
	//
}

int clicktocall_stoprecording_rsp_encode_to_dbif_msg( tcp_clicktocall_stoprecording_rsp_t *clicktocall_stoprecording_rsp, uxc_dbif_t *dbif) {
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
