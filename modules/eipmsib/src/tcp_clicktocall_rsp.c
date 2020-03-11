#include "tcp_clicktocall_rsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_clicktocall_start_rsp_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_clicktocall_start_rsp_init( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp)
{
	memset(clicktocall_start_rsp, 0, sizeof(tcp_clicktocall_start_rsp_t));

	return eUXC_SUCCESS;
}

void tcp_clicktocall_start_rsp_final( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp)
{
	//
}

int tcp_clicktocall_start_rsp_encode_ipcmsg( tcp_clicktocall_start_rsp_t *clicktocall_start_rsp, tcp_msg_t *msg)
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

	rv = uxc_dbif_set_int( dbif, 0, clicktocall_start_rsp->result);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 1, clicktocall_start_rsp->callTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, clicktocall_start_rsp->finalUnitAction);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, clicktocall_start_rsp->lang);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, clicktocall_start_rsp->brandId);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}
