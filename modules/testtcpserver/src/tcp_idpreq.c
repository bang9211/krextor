#include "tcp_idpreq.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_idpreq_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_idpreq_init( tcp_idpreq_t *idpreq)
{
	memset(idpreq, 0, sizeof(tcp_idpreq_t));
	return eUXC_SUCCESS;
}


void tcp_idpreq_final( tcp_idpreq_t *idpreq)
{
	return;
}


int tcp_idpreq_decode_msg( tcp_idpreq_t *idpreq, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= idprsp_decode_msg =");

	idpreq->ccrCount = uxc_dbif_get_int( dbif, 0, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->continueFlag = uxc_dbif_get_int( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->msisdn = uxc_dbif_get_str( dbif, 2, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->imsi = uxc_dbif_get_str( dbif, 3, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->called = uxc_dbif_get_str( dbif, 4, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->dialed = uxc_dbif_get_str( dbif, 5, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->locationInfo = uxc_dbif_get_octet( dbif, 6, &idpreq->locationInfoLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->locationInfoLen = uxc_dbif_get_int( dbif, 7, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->timeAndTimeZone = uxc_dbif_get_octet( dbif, 8, &idpreq->timeAndTimeZoneLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->startTime = uxc_dbif_get_int( dbif, 9, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->vlrNumber = uxc_dbif_get_str( dbif, 10, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->redirInfo = uxc_dbif_get_int( dbif, 11, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->vpnFlag = uxc_dbif_get_int( dbif, 12, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->accCode = uxc_dbif_get_str( dbif, 13, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->usedTime = uxc_dbif_get_int( dbif, 14, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->bearerSvc = uxc_dbif_get_octet( dbif, 15, &idpreq->bearerSvcLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	idpreq->bUcb = uxc_dbif_get_int( dbif, 16, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	if ( idpreq->dialed == NULL || idpreq->dialed[0] == '\0') {
		idpreq->dialed = idpreq->called;
	}

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_getXXX is failed (rv=%d)", rv);
	return rv;
}
