#include "smsc_msg.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for bind_msg_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int bind_msg_tcp_init( bind_msg_tcp_t *bind_msg)
{
	memset(bind_msg, 0, sizeof(bind_msg_tcp_t));
	return eUXC_SUCCESS;
}

void bind_msg_tcp_final( bind_msg_tcp_t *bind_msg)
{
	return;
}

void bind_msg_tcp_display(char *headerStr, bind_msg_tcp_t *bind_msg)
{
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [bind]\n"
		"  [userID] %s\n"
		"  [password] %s", 
		headerStr,
		bind_msg->userID, 
		bind_msg->password);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for bind_ack_msg_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int bind_ack_msg_tcp_init( bind_ack_msg_tcp_t *bind_ack_msg)
{
	memset(bind_ack_msg, 0, sizeof(bind_ack_msg_tcp_t));
	return eUXC_SUCCESS;
}

void bind_ack_msg_tcp_final( bind_ack_msg_tcp_t *bind_ack_msg)
{
	return;
}

void bind_ack_msg_tcp_display(char *headerStr, bind_ack_msg_tcp_t *bind_ack_msg)
{
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [bind_ack]\n"
		"  [result] %d\n"
		"  [preefix] %s", 
		headerStr,
		bind_ack_msg->result, 
		bind_ack_msg->prefix);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for bind_ack_msg_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int deliver_msg_tcp_init( deliver_msg_tcp_t *deliver_msg)
{
	memset(deliver_msg, 0, sizeof(deliver_msg_tcp_t));
	return eUXC_SUCCESS;
}

void deliver_msg_tcp_final( deliver_msg_tcp_t *deliver_msg)
{
	return;
}

int deliver_msg_decode_dbif_msg( deliver_msg_tcp_t *deliver_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif)
{
	int rv;
	
	strncpy(sessionID, uxc_dbif_get_str(dbif, 0, &rv), SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(gwSessionID, uxc_dbif_get_str(dbif, 1, &rv), GW_SESSION_ID_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	deliver_msg->teleServiceID = uxc_dbif_get_int(dbif, 2, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(deliver_msg->callingNumber, uxc_dbif_get_str(dbif, 4, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(deliver_msg->calledNumber, uxc_dbif_get_str(dbif, 5, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(deliver_msg->callbackNumber, uxc_dbif_get_str(dbif, 6, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(deliver_msg->message, uxc_dbif_get_str(dbif, 7, &rv), MESSAGE_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	deliver_msg->serialNumber = uxc_dbif_get_int(dbif, 8, &rv);
	if( rv < eUXC_SUCCESS) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "deliver_msg_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

int deliver_msg_encode_to_dbif_msg( deliver_msg_tcp_t *deliver_msg, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= deliver_msg_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, deliver_msg->teleServiceID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, deliver_msg->callingNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, deliver_msg->calledNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, deliver_msg->callbackNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 4, deliver_msg->message);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 5, deliver_msg->serialNumber);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;

}

void deliver_msg_tcp_display(char *headerStr, deliver_msg_tcp_t *deliver_msg) {
	ux_log(UXL_INFO, 
		"%s\n"
		"Body [deliver_msg]\n"
		"  [teleServiceID] %s\n"
		"  [callingNumber] %s\n"
		"  [calledNumber] %s\n"
		"  [callbackNumber] %s\n"
		"  [message] %s\n"
		"  [serialNumber] %d",
		headerStr,
		deliver_msg->teleServiceID,
		deliver_msg->callingNumber,
		deliver_msg->calledNumber,
		deliver_msg->callbackNumber,
		deliver_msg->message,
		deliver_msg->serialNumber);
}

void deliver_msg_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"Received DBIF\n"
		"[deliver_msg]\n"
		"  [sessionID] %s\n"
		"  [gwSessionID] %s\n"
		"  [teleServiceID] %d\n"
		"  [callingNumber] %s\n"
		"  [calledNumber] %s\n"
		"  [callbackNumber] %s\n"
		"  [message] %s",
		"  [serialNumber] %d",
		uxc_dbif_get_str(dbif, 0, &rv),
		uxc_dbif_get_str(dbif, 1, &rv),
		uxc_dbif_get_int(dbif, 2, &rv),
		uxc_dbif_get_str(dbif, 3, &rv),
		uxc_dbif_get_str(dbif, 4, &rv),
		uxc_dbif_get_str(dbif, 5, &rv),
		uxc_dbif_get_str(dbif, 6, &rv),
		uxc_dbif_get_int(dbif, 7, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for deliver_ack_msg_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int deliver_ack_msg_tcp_init( deliver_ack_msg_tcp_t *deliver_ack_msg) {
	memset(deliver_ack_msg, 0, sizeof(deliver_ack_msg_tcp_t));
	return eUXC_SUCCESS;
}

void deliver_ack_msg_tcp_final( deliver_ack_msg_tcp_t *deliver_ack_msg) {
	return;
}

int deliver_ack_msg_decode_dbif_msg( deliver_ack_msg_tcp_t *deliver_ack_msg, uxc_dbif_t *dbif) {
	int rv;
	
	deliver_ack_msg->result = uxc_dbif_get_int(dbif, 0, &rv);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(deliver_ack_msg->callingNumber, uxc_dbif_get_str(dbif, 1, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	strncpy(deliver_ack_msg->calledNumber, uxc_dbif_get_str(dbif, 2, &rv), CALL_NUMBER_LEN);
	if( rv < eUXC_SUCCESS) goto final;
	deliver_ack_msg->serialNumber = uxc_dbif_get_int(dbif, 3, &rv);
	if( rv < eUXC_SUCCESS) goto final;

	return eUXC_SUCCESS;

final:
	ux_log( UXL_CRT, "deliver_ack_msg_decode_dbif_msg is failed (rv=%d)", rv);
	return rv;
}

int deliver_ack_msg_encode_to_dbif_msg( deliver_ack_msg_tcp_t *deliver_ack_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= deliver_msg_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, deliver_ack_msg->result);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, deliver_ack_msg->callingNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, deliver_ack_msg->calledNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, deliver_ack_msg->serialNumber);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;

}

void deliver_ack_msg_tcp_display(char *headerStr, deliver_ack_msg_tcp_t *deliver_ack_msg) {

}

void deliver_ack_msg_dbif_display(uxc_dbif_t *dbif) {

}

///////////////////////////////////////////////////////////////////////////////////
// functions for report_msg_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int report_msg_tcp_init( report_msg_tcp_t *report_msg) {
	memset(report_msg, 0, sizeof(report_msg_tcp_t));
	return eUXC_SUCCESS;
}

void report_msg_tcp_final( report_msg_tcp_t *report_msg) {
	return;
}

int report_msg_decode_dbif_msg( report_msg_tcp_t *report_msg, uxc_dbif_t *dbif) {

}

int report_msg_encode_to_dbif_msg( report_msg_tcp_t *report_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif) {

}

void report_msg_tcp_display(char *headerStr, report_msg_tcp_t *report_msg) {

}

void report_msg_dbif_display(uxc_dbif_t *dbif) {

}


///////////////////////////////////////////////////////////////////////////////////
// functions for report_ack_msg_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int report_ack_msg_tcp_init( report_ack_msg_tcp_t *report_ack_msg) {
	memset(report_ack_msg, 0, sizeof(report_ack_msg_tcp_t));
	return eUXC_SUCCESS;
}

void report_ack_msg_tcp_final( report_ack_msg_tcp_t *report_ack_msg) {
	return;
}

int report_ack_msg_decode_dbif_msg( report_ack_msg_tcp_t *report_ack_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif) {

}

int report_ack_msg_encode_to_dbif_msg( report_ack_msg_tcp_t *report_ack_msg, uxc_dbif_t *dbif) {

}

void report_ack_msg_tcp_display(char *headerStr, report_ack_msg_tcp_t *report_ack_msg) {

}

void report_ack_msg_dbif_display(uxc_dbif_t *dbif) {

}

