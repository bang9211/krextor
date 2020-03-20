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

int clicktocall_start_rsp_encode_to_dbif_msg( clicktocall_start_rsp_tcp_t *clicktocall_start_rsp, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*) msg->data;
	ux_log(UXL_INFO, "= clicktocall_start_rsp_encode_msg =");

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
