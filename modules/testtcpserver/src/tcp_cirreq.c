#include "tcp_cirreq.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for tcp_cirreq_t 
///////////////////////////////////////////////////////////////////////////////////

int tcp_cirreq_init( tcp_cirreq_t *cirreq)
{
	memset(cirreq, 0, sizeof(tcp_cirreq_t));
	return eUXC_SUCCESS;
}


void tcp_cirreq_final( tcp_cirreq_t *cirreq)
{
	return;
}


int tcp_cirreq_decode_msg( tcp_cirreq_t *cirreq, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= cirreq_decode_msg =");

	cirreq->ccrCount = uxc_dbif_get_int( dbif, 0, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->continueFlag = uxc_dbif_get_int( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->msisdn = uxc_dbif_get_str( dbif, 2, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->imsi = uxc_dbif_get_str( dbif, 3, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->called = uxc_dbif_get_str( dbif, 4, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->dialed = uxc_dbif_get_str( dbif, 5, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->callTime = uxc_dbif_get_int( dbif, 6, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->endTime = uxc_dbif_get_int( dbif, 7, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->timeAndTimeZone = uxc_dbif_get_octet( dbif, 8, &cirreq->timeAndTimeZoneLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->locationInfo = uxc_dbif_get_octet( dbif, 9, &cirreq->locationInfoLen, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->locationInfoLen = uxc_dbif_get_int( dbif, 10, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->reportingReason = uxc_dbif_get_int( dbif, 11, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->callAttemptElapsedTime = uxc_dbif_get_int( dbif, 12, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->callRefNumber = uxc_dbif_get_str( dbif, 13, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->scpAddr = uxc_dbif_get_str( dbif, 14, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->dest = uxc_dbif_get_str( dbif, 15, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->vlrNumber = uxc_dbif_get_str( dbif, 16, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->redirInfo = uxc_dbif_get_int( dbif, 17, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->vpnFlag = uxc_dbif_get_int( dbif, 18, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->accCode = uxc_dbif_get_str( dbif, 19, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	cirreq->bUcb = uxc_dbif_get_int( dbif, 20, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	
	if ( cirreq->dialed == NULL || cirreq->dialed[0] == '\0') {
		cirreq->dialed = cirreq->called;
	}

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_getXXX is failed (rv=%d)", rv);
	return rv;
}

