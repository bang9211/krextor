#include "clicktoconference_rpt.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_add_party_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_add_party_rpt_tcp_init( clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt) {
	memset(clicktoconference_add_party_rpt, 0, sizeof(clicktoconference_add_party_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_add_party_rpt_tcp_final( clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt) {
    return;
}

int clicktoconference_add_party_rpt_encode_to_dbif_msg( clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_add_party_rpt->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_add_party_rpt->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_add_party_rpt->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktoconference_add_party_rpt->participantName);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_add_party_rpt_tcp_display(clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt) {
    ux_log(UXL_INFO, "TCP [clicktoconference_remove_party_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_add_party_rpt->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_add_party_rpt->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_add_party_rpt->participantNumber);
	ux_log(UXL_INFO, "  [participantName] %s", clicktoconference_add_party_rpt->participantName);
}

void clicktoconference_add_party_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_remove_party_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [participantName] %s",uxc_dbif_get_str(dbif, 3, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_remove_party_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_remove_party_rpt_tcp_init( clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt) {
	memset(clicktoconference_remove_party_rpt, 0, sizeof(clicktoconference_remove_party_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_remove_party_rpt_tcp_final( clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt) {
    return;
}

int clicktoconference_remove_party_rpt_encode_to_dbif_msg( clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_remove_party_rpt->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_remove_party_rpt->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_remove_party_rpt->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_remove_party_rpt_tcp_display(clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt) {
    ux_log(UXL_INFO, "TCP [clicktoconference_remove_party_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_remove_party_rpt->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_remove_party_rpt->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_remove_party_rpt->participantNumber);
}

void clicktoconference_remove_party_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_remove_party_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_change_party_media_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_change_party_media_rpt_tcp_init( clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt) {
	memset(clicktoconference_change_party_media_rpt, 0, sizeof(clicktoconference_change_party_media_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_change_party_media_rpt_tcp_final( clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt) {
    return;
}

int clicktoconference_change_party_media_rpt_encode_to_dbif_msg( clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_change_party_media_rpt->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_change_party_media_rpt->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_change_party_media_rpt->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, clicktoconference_change_party_media_rpt->mediaType);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_change_party_media_rpt_tcp_display(clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt) {
	ux_log(UXL_INFO, "TCP [clicktoconference_change_party_media_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_change_party_media_rpt->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_change_party_media_rpt->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_change_party_media_rpt->participantNumber);
	ux_log(UXL_INFO, "  [mediaType] %d", clicktoconference_change_party_media_rpt->mediaType);
}

void clicktoconference_change_party_media_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_change_party_media_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [mediaType] %d",uxc_dbif_get_int(dbif, 3, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_change_option_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_change_option_rpt_tcp_init( clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt) {
    memset(clicktoconference_change_option_rpt, 0, sizeof(clicktoconference_change_option_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_change_option_rpt_tcp_final( clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt) {
    return;
}

void clicktoconference_change_option_rpt_tcp_display(clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt) {
	ux_log(UXL_INFO, "TCP [clicktoconference_stop_rpt]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_change_option_rpt->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_change_option_rpt->confID);
	ux_log(UXL_INFO, "  [password] %s", clicktoconference_change_option_rpt->password);
}


///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_stop_rpt_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_stop_rpt_tcp_init( clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt) {
	memset(clicktoconference_stop_rpt, 0, sizeof(clicktoconference_stop_rpt_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_stop_rpt_tcp_final( clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt) {
    return;
}

int clicktoconference_stop_rpt_encode_to_dbif_msg( clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_stop_rpt_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_stop_rpt->returnCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_stop_rpt->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_stop_rpt->startTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktoconference_stop_rpt->endTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, clicktoconference_stop_rpt->isRecorded);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_stop_rpt_tcp_display(clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt) {
	ux_log(UXL_INFO, "TCP [clicktoconference_stop_rpt]");
	ux_log(UXL_INFO, "  [returnCode] %d", clicktoconference_stop_rpt->returnCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_stop_rpt->confID);
	ux_log(UXL_INFO, "  [startTime] %s", clicktoconference_stop_rpt->startTime);
	ux_log(UXL_INFO, "  [endTime] %s", clicktoconference_stop_rpt->endTime);
	ux_log(UXL_INFO, "  [isRecorded] %d", clicktoconference_stop_rpt->isRecorded);
}

void clicktoconference_stop_rpt_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_stop_rpt]");
	ux_log(UXL_INFO, "  [returnCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [startTime] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [endTime] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [isRecorded] %d",uxc_dbif_get_int(dbif, 4, &rv));
}
