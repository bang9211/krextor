#include "tcp_idprsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_idprsp_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_idprsp_init( tcp_idprsp_t *idprsp)
{
	memset(idprsp, 0, sizeof(tcp_idprsp_t));

	return eUXC_SUCCESS;
}

void tcp_idprsp_final( tcp_idprsp_t *idprsp)
{
	//
}

int tcp_idprsp_encode_ipcmsg( tcp_idprsp_t *idprsp, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*) msg->data;
	ux_log(UXL_INFO, "= idprsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, idprsp->result);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 1, idprsp->callTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, idprsp->finalUnitAction);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, idprsp->lang);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, idprsp->brandId);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}
