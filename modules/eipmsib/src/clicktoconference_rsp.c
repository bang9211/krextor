#include "clicktoconference_rsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_start_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_start_rsp_tcp_init( clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp)
{
	memset(clicktoconference_start_rsp, 0, sizeof(clicktoconference_start_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_start_rsp_tcp_final( clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp)
{
	return;
}

int clicktoconference_start_rsp_encode_to_dbif_msg(clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp, 
	char *sessionID, char *gwSessionID, uxc_dbif_t *dbif)
{
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_str( dbif, 0, sessionID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, gwSessionID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, clicktoconference_start_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktoconference_start_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 4, clicktoconference_start_rsp->chairmanName);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 5, clicktoconference_start_rsp->chairmanNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 6, clicktoconference_start_rsp->chargingNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 7, clicktoconference_start_rsp->confType);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 8, clicktoconference_start_rsp->recordingFilePath);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 9, clicktoconference_start_rsp->recordingFileURL);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 10, clicktoconference_start_rsp->recordingFileName);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 11, clicktoconference_start_rsp->password);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 12, clicktoconference_start_rsp->serviceMentID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 13, clicktoconference_start_rsp->callingCategoryType);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_start_rsp_tcp_display(clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_start_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_start_rsp->confID);
	ux_log(UXL_INFO, "  [chairmanName] %s", clicktoconference_start_rsp->chairmanName);
	ux_log(UXL_INFO, "  [chairmanNumber] %s", clicktoconference_start_rsp->chairmanNumber);
	ux_log(UXL_INFO, "  [chargingNumber] %s", clicktoconference_start_rsp->chargingNumber);
	ux_log(UXL_INFO, "  [confType] %d", clicktoconference_start_rsp->confType);
	ux_log(UXL_INFO, "  [recordingFilePath] %d", clicktoconference_start_rsp->recordingFilePath);
	ux_log(UXL_INFO, "  [filler1] %d", clicktoconference_start_rsp->filler1);
	ux_log(UXL_INFO, "  [startTime] %s", clicktoconference_start_rsp->startTime);
	ux_log(UXL_INFO, "  [smsNoti] %d", clicktoconference_start_rsp->smsNoti);
	ux_log(UXL_INFO, "  [voiceNoti] %d", clicktoconference_start_rsp->voiceNoti);
	ux_log(UXL_INFO, "  [filler2] %d", clicktoconference_start_rsp->filler2);
	ux_log(UXL_INFO, "  [recordingFileID] %d", clicktoconference_start_rsp->recordingFileID);
	ux_log(UXL_INFO, "  [recordingFileURL] %s", clicktoconference_start_rsp->recordingFileURL);
	ux_log(UXL_INFO, "  [recordingFileName] %s", clicktoconference_start_rsp->recordingFileName);
	ux_log(UXL_INFO, "  [password] %s", clicktoconference_start_rsp->password);
	ux_log(UXL_INFO, "  [serviceMentID] %d", clicktoconference_start_rsp->serviceMentID);
	ux_log(UXL_INFO, "  [callingCategoryType] %d", clicktoconference_start_rsp->callingCategoryType);
	ux_log(UXL_INFO, "  [msp_id] %s", clicktoconference_start_rsp->msp_id);
	ux_log(UXL_INFO, "  [sp_id] %s", clicktoconference_start_rsp->sp_id);
	ux_log(UXL_INFO, "  [service_id] %s", clicktoconference_start_rsp->service_id);
}

void clicktoconference_start_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_start_rsp]");
	ux_log(UXL_INFO, "  [sessionID] %s",uxc_dbif_get_str(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [gwSessionID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [chairmanName] %s",uxc_dbif_get_str(dbif, 4, &rv));
	ux_log(UXL_INFO, "  [chairmanNumber] %s",uxc_dbif_get_str(dbif, 5, &rv));
	ux_log(UXL_INFO, "  [chargingNumber] %s",uxc_dbif_get_str(dbif, 6, &rv));
	ux_log(UXL_INFO, "  [confType] %d",uxc_dbif_get_int(dbif, 7, &rv));
	ux_log(UXL_INFO, "  [recordingFilePath] %d",uxc_dbif_get_int(dbif, 8, &rv));
	ux_log(UXL_INFO, "  [recordingFileURL] %s",uxc_dbif_get_str(dbif, 9, &rv));
	ux_log(UXL_INFO, "  [recordingFileName] %s",uxc_dbif_get_str(dbif, 10, &rv));
	ux_log(UXL_INFO, "  [password] %s",uxc_dbif_get_str(dbif, 11, &rv));
	ux_log(UXL_INFO, "  [serviceMentID] %d",uxc_dbif_get_int(dbif, 12, &rv));
	ux_log(UXL_INFO, "  [callingCategoryType] %d",uxc_dbif_get_int(dbif, 13, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_add_party_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_add_party_rsp_tcp_init( clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp) {
	memset(clicktoconference_add_party_rsp, 0, sizeof(clicktoconference_add_party_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_add_party_rsp_tcp_final( clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp) {
	return;
}

int clicktoconference_add_party_rsp_encode_to_dbif_msg( clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_add_party_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_add_party_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_add_party_rsp->participantList);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_add_party_rsp_tcp_display(clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_add_party_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_add_party_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_add_party_rsp->confID);
	ux_log(UXL_INFO, "  [participantList] %s", clicktoconference_add_party_rsp->participantList);
}

void clicktoconference_add_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantList] %s",uxc_dbif_get_str(dbif, 2, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_remove_party_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_remove_party_rsp_tcp_init( clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp) {
	memset(clicktoconference_remove_party_rsp, 0, sizeof(clicktoconference_remove_party_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_remove_party_rsp_tcp_final( clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp) {
	return;
}

int clicktoconference_remove_party_rsp_encode_to_dbif_msg( clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_remove_party_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_remove_party_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_remove_party_rsp->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_remove_party_rsp_tcp_display(clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_remove_party_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_remove_party_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_remove_party_rsp->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_remove_party_rsp->participantNumber);
}

void clicktoconference_remove_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_change_party_media_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_change_party_media_rsp_tcp_init( clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp) {
	memset(clicktoconference_change_party_media_rsp, 0, sizeof(clicktoconference_change_party_media_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_change_party_media_rsp_tcp_final( clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp) {
	return;
}

int clicktoconference_change_party_media_rsp_encode_to_dbif_msg( clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_change_party_media_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_change_party_media_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_change_party_media_rsp->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 3, clicktoconference_change_party_media_rsp->mediaType);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_change_party_media_rsp_tcp_display(clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_add_party_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_change_party_media_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_change_party_media_rsp->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_change_party_media_rsp->participantNumber);
	ux_log(UXL_INFO, "  [mediaType] %d", clicktoconference_change_party_media_rsp->mediaType);
}

void clicktoconference_change_party_media_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [mediaType] %d",uxc_dbif_get_int(dbif, 3, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_change_option_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_change_option_rsp_tcp_init( clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp) {
	memset(clicktoconference_change_option_rsp, 0, sizeof(clicktoconference_change_option_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_change_option_rsp_tcp_final( clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp) {
	return;
}

void clicktoconference_change_option_rsp_tcp_display(clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_add_party_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_change_option_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_change_option_rsp->confID);
	ux_log(UXL_INFO, "  [password] %s", clicktoconference_change_option_rsp->password);
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_get_number_of_party_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_get_number_of_party_rsp_tcp_init( clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp) {

	memset(clicktoconference_get_number_of_party_rsp, 0, sizeof(clicktoconference_get_number_of_party_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_get_number_of_party_rsp_tcp_final( clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp) {
	return;
}

int clicktoconference_get_number_of_party_rsp_encode_to_dbif_msg( clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp, uxc_dbif_t *dbif) {
	int rv;
	// uxc_dbif_t *dbif;

	// dbif = (uxc_dbif_t*) msg->data;
	// ux_log(UXL_INFO, "= clicktoconference_start_rsp_encode_msg =");

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_get_number_of_party_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_get_number_of_party_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 2, clicktoconference_get_number_of_party_rsp->number);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_get_number_of_party_rsp_tcp_display(clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_add_party_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_get_number_of_party_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_get_number_of_party_rsp->confID);
	ux_log(UXL_INFO, "  [number] %d", clicktoconference_get_number_of_party_rsp->number);
}

void clicktoconference_get_number_of_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_start_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [number] %d",uxc_dbif_get_int(dbif, 2, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_stop_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_stop_rsp_tcp_init( clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp) {
	memset(clicktoconference_stop_rsp, 0, sizeof(clicktoconference_stop_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_stop_rsp_tcp_final( clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp) {
	return;
}

int clicktoconference_stop_rsp_encode_to_dbif_msg( clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_stop_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_stop_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_stop_rsp->startTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktoconference_stop_rsp->endTime);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_int( dbif, 4, clicktoconference_stop_rsp->recordingFileID);
	if( rv < eUXC_SUCCESS ) goto final; 

	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_stop_rsp_tcp_display(clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_stop_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_stop_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_stop_rsp->confID);
	ux_log(UXL_INFO, "  [startTime] %s", clicktoconference_stop_rsp->startTime);
	ux_log(UXL_INFO, "  [endTime] %s", clicktoconference_stop_rsp->endTime);
	ux_log(UXL_INFO, "  [recordingFileID] %d", clicktoconference_stop_rsp->recordingFileID);
}

void clicktoconference_stop_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_stop_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [startTime] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [endTime] %s",uxc_dbif_get_str(dbif, 3, &rv));
	ux_log(UXL_INFO, "  [recordingFileID] %d",uxc_dbif_get_int(dbif, 4, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_play_ment_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_play_ment_rsp_tcp_init( clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp) {

	memset(clicktoconference_play_ment_rsp, 0, sizeof(clicktoconference_play_ment_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_play_ment_rsp_tcp_final( clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp) {
	return;
}

int clicktoconference_play_ment_rsp_encode_to_dbif_msg( clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_play_ment_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_play_ment_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_play_ment_rsp_tcp_display(clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_play_ment_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_play_ment_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_play_ment_rsp->confID);
}

void clicktoconference_play_ment_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_play_ment_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_get_party_status_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_get_party_status_rsp_tcp_init( clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp) {

	memset(clicktoconference_get_party_status_rsp, 0, sizeof(clicktoconference_get_party_status_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_get_party_status_rsp_tcp_final( clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp) {
	return;
}

int clicktoconference_get_party_status_rsp_encode_to_dbif_msg( clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp,  uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_get_party_status_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_get_party_status_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_get_party_status_rsp->participantName);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 3, clicktoconference_get_party_status_rsp->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_get_party_status_rsp_tcp_display(clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_get_party_status_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_get_party_status_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_get_party_status_rsp->confID);
	ux_log(UXL_INFO, "  [participantName] %s", clicktoconference_get_party_status_rsp->participantName);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_get_party_status_rsp->participantNumber);
}

void clicktoconference_get_party_status_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "DBIF [clicktoconference_get_party_status_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantName] %s",uxc_dbif_get_str(dbif, 2, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 3, &rv));
}

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_cancel_party_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_cancel_party_rsp_tcp_init( clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp) {
	memset(clicktoconference_cancel_party_rsp, 0, sizeof(clicktoconference_cancel_party_rsp_tcp_t));
	return eUXC_SUCCESS;
}

void clicktoconference_cancel_party_rsp_tcp_final( clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp) {
	return;
}

int clicktoconference_cancel_party_rsp_encode_to_dbif_msg( clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp, uxc_dbif_t *dbif) {
	int rv;

	rv = uxc_dbif_init( dbif);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_CRT, "uxc_dbif_init failed(rv=%d)", rv);
		return rv;
	}

	rv = uxc_dbif_set_int( dbif, 0, clicktoconference_cancel_party_rsp->resultCode);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 1, clicktoconference_cancel_party_rsp->confID);
	if( rv < eUXC_SUCCESS ) goto final; 
	rv = uxc_dbif_set_str( dbif, 2, clicktoconference_cancel_party_rsp->participantNumber);
	if( rv < eUXC_SUCCESS ) goto final; 
	return rv;

final:
	ux_log( UXL_CRT, "uxc_dbif_set_XXX is failed (rv=%d)", rv);
	return rv;
}

void clicktoconference_cancel_party_rsp_tcp_display(clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp) {
	ux_log(UXL_INFO, "TCP [clicktoconference_cancel_party_rsp]");
	ux_log(UXL_INFO, "  [resultCode] %d", clicktoconference_cancel_party_rsp->resultCode);
	ux_log(UXL_INFO, "  [confID] %s", clicktoconference_cancel_party_rsp->confID);
	ux_log(UXL_INFO, "  [participantNumber] %s", clicktoconference_cancel_party_rsp->participantNumber);
}

void clicktoconference_cancel_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, "  [resultCode] %d",uxc_dbif_get_int(dbif, 0, &rv));
	ux_log(UXL_INFO, "  [confID] %s",uxc_dbif_get_str(dbif, 1, &rv));
	ux_log(UXL_INFO, "  [participantNumber] %s",uxc_dbif_get_str(dbif, 2, &rv));
}

