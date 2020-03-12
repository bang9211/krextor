#include "tcp_cirrsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_cirrsp_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_cirrsp_init( tcp_cirrsp_t *cirrsp)
{
	memset(cirrsp, 0, sizeof(tcp_cirrsp_t));
	return eUXC_SUCCESS;
}

void tcp_cirrsp_final( tcp_cirrsp_t *cirrsp)
{
	return;
}

int tcp_cirrsp_encode_ipcmsg( tcp_cirrsp_t *cirrsp, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*) msg->data;
	ux_log(UXL_INFO, "= cirrsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, cirrsp->result);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 1, cirrsp->callTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, cirrsp->finalUnitAction);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, cirrsp->lang);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, cirrsp->brandId);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}


