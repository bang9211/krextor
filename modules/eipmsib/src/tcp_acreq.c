#include "tcp_acreq.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_acreq_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_acreq_init( tcp_acreq_t *acreq)
{
	memset(acreq, 0, sizeof(tcp_acreq_t));
	return eUXC_SUCCESS; 
}


void tcp_acreq_final( tcp_acreq_t *acreq)
{
	return;
}


int tcp_acreq_decode_msg( tcp_acreq_t *acreq, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= acreq_decode_msg =");

	acreq->ccrCount = uxc_dbif_get_int( dbif, 0, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->continueFlag = uxc_dbif_get_int( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->msisdn = uxc_dbif_get_str( dbif, 2, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->imsi = uxc_dbif_get_str( dbif, 3, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->called = uxc_dbif_get_str( dbif, 4, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->dialed = uxc_dbif_get_str( dbif, 5, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->locationInfo = uxc_dbif_get_octet( dbif, 6, &acreq->locationInfoLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->locationInfoLen = uxc_dbif_get_int( dbif, 7, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->timeAndTimeZone = uxc_dbif_get_octet( dbif, 8, &acreq->timeAndTimeZoneLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->startTime = uxc_dbif_get_int( dbif, 9, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->vlrNumber = uxc_dbif_get_str( dbif, 10, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->redirInfo = uxc_dbif_get_int( dbif, 11, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->vpnFlag = uxc_dbif_get_int( dbif, 12, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->accCode = uxc_dbif_get_str( dbif, 13, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->usedTime = uxc_dbif_get_int( dbif, 14, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->bearerSvc = uxc_dbif_get_octet( dbif, 15, &acreq->bearerSvcLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	acreq->bUcb = uxc_dbif_get_int( dbif, 16, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	if ( acreq->dialed == NULL || acreq->dialed[0] == '\0') {
		acreq->dialed = acreq->called;
	}

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_getXXX is failed (rv=%d)", rv);
	return rv;
}

