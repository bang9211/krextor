#include "tcp_acrsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_acrsp_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_acrsp_init( tcp_acrsp_t *acrsp)
{
	memset(acrsp, 0, sizeof(tcp_acrsp_t));

	return eUXC_SUCCESS;
}

void tcp_acrsp_final( tcp_acrsp_t *acrsp)
{
	return;
}

int tcp_acrsp_encode_ipcmsg( tcp_acrsp_t *acrsp, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t *) msg->data;
	ux_log(UXL_INFO, "= acrsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, acrsp->result);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 1, acrsp->callTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, acrsp->finalUnitAction);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, acrsp->lang);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}


