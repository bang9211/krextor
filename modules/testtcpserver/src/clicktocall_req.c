#include "clicktocall_req.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_start_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_start_req_tcp_init( clicktocall_start_req_tcp_t *clicktocall_start_req)
{
	memset(clicktocall_start_req, 0, sizeof(clicktocall_start_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_start_req_tcp_final( clicktocall_start_req_tcp_t *clicktocall_start_req)
{
	return;
}

int clicktocall_start_req_decode_dbif_msg( clicktocall_start_req_tcp_t *clicktocall_start_req, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= clicktocall_start_req_decode_msg =");

	strcpy(clicktocall_start_req->subscriberName, uxc_dbif_get_str( dbif, 0, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->recordingType = uxc_dbif_get_int( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->callingNumber, uxc_dbif_get_str( dbif, 2, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->calledNumber, uxc_dbif_get_str( dbif, 3, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->chargingNumber, uxc_dbif_get_str( dbif, 4, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->serviceCode = uxc_dbif_get_int( dbif, 5, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->ringBackToneType = uxc_dbif_get_int( dbif, 6, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->waitingMentID = uxc_dbif_get_int( dbif, 7, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->fillerInt8 = uxc_dbif_get_int( dbif, 8, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->scenarioType = uxc_dbif_get_int( dbif, 9, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->callMentID = uxc_dbif_get_int( dbif, 10, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->callingCID, uxc_dbif_get_str( dbif, 11, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->calledCID, uxc_dbif_get_str( dbif, 12, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->recordingFileName, uxc_dbif_get_str( dbif, 13, &rv));
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->isAllRecording = uxc_dbif_get_int( dbif, 14, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->endIfRecordingFailed = uxc_dbif_get_int( dbif, 15, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->endIfRecordingEnded = uxc_dbif_get_int( dbif, 16, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->hostingCode = uxc_dbif_get_int( dbif, 17, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->wirelessTimeout = uxc_dbif_get_int( dbif, 18, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->wiredTimeout = uxc_dbif_get_int( dbif, 19, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_start_req->fillerInt16 = uxc_dbif_get_int( dbif, 20, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	strcpy(clicktocall_start_req->filler, uxc_dbif_get_str( dbif, 21, &rv));
	if( rv < eUXC_SUCCESS ) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_clicktocall_start is failed (rv=%d)", rv);
	return rv;
}


///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_stop_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stop_req_tcp_init( clicktocall_stop_req_tcp_t *clicktocall_stop_req)
{
	memset(clicktocall_stop_req, 0, sizeof(clicktocall_stop_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_stop_req_tcp_final( clicktocall_stop_req_tcp_t *clicktocall_stop_req)
{
	return;
}

int clicktocall_stop_req_decode_dbif_msg( clicktocall_stop_req_tcp_t *clicktocall_stop_req, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= clicktocall_stop_req_decode_msg =");

	clicktocall_stop_req->networkType = uxc_dbif_get_int( dbif, 0, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_stop_req->sessionID = uxc_dbif_get_str( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_stop_req->gwSessionID = uxc_dbif_get_str( dbif, 2, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_stop_req->recordingFileName = uxc_dbif_get_str( dbif, 15, &rv);
	if( rv < eUXC_SUCCESS ) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_clicktocall_stop is failed (rv=%d)", rv);
	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_startrecording_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_startrecording_req_tcp_init( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req)
{
	memset(clicktocall_startrecording_req, 0, sizeof(clicktocall_startrecording_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_startrecording_req_tcp_final( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req)
{
	return;
}

int clicktocall_startrecording_req_decode_dbif_msg( clicktocall_startrecording_req_tcp_t *clicktocall_startrecording_req, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= clicktocall_startrecording_req_decode_msg =");

	clicktocall_startrecording_req->networkType = uxc_dbif_get_int( dbif, 0, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_startrecording_req->sessionID = uxc_dbif_get_str( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_startrecording_req->gwSessionID = uxc_dbif_get_str( dbif, 2, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_startrecording_req->recordingFileName = uxc_dbif_get_str( dbif, 15, &rv);
	if( rv < eUXC_SUCCESS ) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_clicktocall_startrecording is failed (rv=%d)", rv);
	return rv;
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_stoprecording_req_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktocall_stoprecording_req_tcp_init( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req)
{
	memset(clicktocall_stoprecording_req, 0, sizeof(clicktocall_stoprecording_req_tcp_t));
	return eUXC_SUCCESS;
}

void clicktocall_stoprecording_req_tcp_final( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req)
{
	return;
}

int clicktocall_stoprecording_req_decode_dbif_msg( clicktocall_stoprecording_req_tcp_t *clicktocall_stoprecording_req, tcp_msg_t *msg)
{
	int rv;
	uxc_dbif_t *dbif;

	dbif = (uxc_dbif_t*)msg->data;
	ux_log(UXL_INFO, "= clicktocall_stoprecording_req_decode_msg =");

	clicktocall_stoprecording_req->networkType = uxc_dbif_get_int( dbif, 0, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_stoprecording_req->sessionID = uxc_dbif_get_str( dbif, 1, &rv);
	if( rv < eUXC_SUCCESS ) goto final;
	clicktocall_stoprecording_req->gwSessionID = uxc_dbif_get_str( dbif, 2, &rv);
	if( rv < eUXC_SUCCESS ) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "uxc_dbif_clicktocall_stoprecording is failed (rv=%d)", rv);
	return rv;
}