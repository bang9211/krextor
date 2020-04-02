#include "clicktoconference_rsp.h"

///////////////////////////////////////////////////////////////////////////////////
// functions for clicktoconference_binding_rsp_tcp_t 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_binding_rsp_tcp_init( clicktoconference_binding_rsp_tcp_t *clicktoconference_binding_rsp)
{
	memset(clicktoconference_binding_rsp, 0, sizeof(clicktoconference_binding_rsp_tcp_t));

	return eUXC_SUCCESS;
}

void clicktoconference_binding_rsp_tcp_final( clicktoconference_binding_rsp_tcp_t *clicktoconference_binding_rsp)
{
	//
}

void clicktoconference_binding_rsp_tcp_display(clicktoconference_binding_rsp_tcp_t *clicktoconference_binding_rsp) {
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_binding_rsp]\n"
		"  [resultCode] %d\n"
		"  [userID] %s\n"
		"  [password] %s"
		, clicktoconference_binding_rsp->resultCode
		, clicktoconference_binding_rsp->userID
		, clicktoconference_binding_rsp->password);
}

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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_start_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [chairmanName] %s\n"
		"  [chairmanNumber] %s\n"
		"  [chargingNumber] %s\n"
		"  [confType] %d\n"
		"  [recordingFilePath] %d\n"
		"  [filler1] %d\n"
		"  [startTime] %s\n"
		"  [smsNoti] %d\n"
		"  [voiceNoti] %d\n"
		"  [filler2] %d\n"
		"  [recordingFileID] %d\n"
		"  [recordingFileURL] %s\n"
		"  [recordingFileName] %s\n"
		"  [password] %s\n"
		"  [serviceMentID] %d\n"
		"  [callingCategoryType] %d\n"
		"  [msp_id] %s\n"
		"  [sp_id] %s\n"
		"  [service_id] %s"
		, clicktoconference_start_rsp->resultCode
		, clicktoconference_start_rsp->confID
		, clicktoconference_start_rsp->chairmanName
		, clicktoconference_start_rsp->chairmanNumber
		, clicktoconference_start_rsp->chargingNumber
		, clicktoconference_start_rsp->confType
		, clicktoconference_start_rsp->recordingFilePath
		, clicktoconference_start_rsp->filler1
		, clicktoconference_start_rsp->startTime
		, clicktoconference_start_rsp->smsNoti
		, clicktoconference_start_rsp->voiceNoti
		, clicktoconference_start_rsp->filler2
		, clicktoconference_start_rsp->recordingFileID
		, clicktoconference_start_rsp->recordingFileURL
		, clicktoconference_start_rsp->recordingFileName
		, clicktoconference_start_rsp->password
		, clicktoconference_start_rsp->serviceMentID
		, clicktoconference_start_rsp->callingCategoryType
		, clicktoconference_start_rsp->msp_id
		, clicktoconference_start_rsp->sp_id
		, clicktoconference_start_rsp->service_id);
}

void clicktoconference_start_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_start_rsp]\n"
		"  [sessionID] %s\n"
		"  [gwSessionID] %s\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [chairmanName] %s\n"
		"  [chairmanNumber] %s\n"
		"  [chargingNumber] %s\n"
		"  [confType] %d\n"
		"  [recordingFilePath] %d\n"
		"  [recordingFileURL] %s\n"
		"  [recordingFileName] %s\n"
		"  [password] %s\n"
		"  [serviceMentID] %d\n"
		"  [callingCategoryType] %d"
		,uxc_dbif_get_str(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_int(dbif, 2, &rv)
		,uxc_dbif_get_str(dbif, 3, &rv)
		,uxc_dbif_get_str(dbif, 4, &rv)
		,uxc_dbif_get_str(dbif, 5, &rv)
		,uxc_dbif_get_str(dbif, 6, &rv)
		,uxc_dbif_get_int(dbif, 7, &rv)
		,uxc_dbif_get_int(dbif, 8, &rv)
		,uxc_dbif_get_str(dbif, 9, &rv)
		,uxc_dbif_get_str(dbif, 10, &rv)
		,uxc_dbif_get_str(dbif, 11, &rv)
		,uxc_dbif_get_int(dbif, 12, &rv)
		,uxc_dbif_get_int(dbif, 13, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_add_party_rsp]"
		"  [resultCode] %d"
		"  [confID] %s"
		"  [participantList] %s"
		, clicktoconference_add_party_rsp->resultCode
		, clicktoconference_add_party_rsp->confID
		, clicktoconference_add_party_rsp->participantList);
}

void clicktoconference_add_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_start_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantList] %s"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_remove_party_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		, clicktoconference_remove_party_rsp->resultCode
		, clicktoconference_remove_party_rsp->confID
		, clicktoconference_remove_party_rsp->participantNumber);
}

void clicktoconference_remove_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_start_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_add_party_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantNumber] %s\n"
		"  [mediaType] %d"
		, clicktoconference_change_party_media_rsp->resultCode
		, clicktoconference_change_party_media_rsp->confID
		, clicktoconference_change_party_media_rsp->participantNumber
		, clicktoconference_change_party_media_rsp->mediaType);
}

void clicktoconference_change_party_media_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_start_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantNumber] %s\n"
		"  [mediaType] %d"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv)
		,uxc_dbif_get_int(dbif, 3, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_add_party_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [password] %s"
		, clicktoconference_change_option_rsp->resultCode
		, clicktoconference_change_option_rsp->confID
		, clicktoconference_change_option_rsp->password);
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_add_party_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [number] %d"
		, clicktoconference_get_number_of_party_rsp->resultCode
		, clicktoconference_get_number_of_party_rsp->confID
		, clicktoconference_get_number_of_party_rsp->number);
}

void clicktoconference_get_number_of_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_start_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [number] %d"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_int(dbif, 2, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_stop_rsp]\b"
		"  [resultCode] %d\b"
		"  [confID] %s\b"
		"  [startTime] %s\b"
		"  [endTime] %s\b"
		"  [recordingFileID] %d"
		, clicktoconference_stop_rsp->resultCode
		, clicktoconference_stop_rsp->confID
		, clicktoconference_stop_rsp->startTime
		, clicktoconference_stop_rsp->endTime
		, clicktoconference_stop_rsp->recordingFileID);
}

void clicktoconference_stop_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_stop_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [startTime] %s\n"
		"  [endTime] %s\n"
		"  [recordingFileID] %d"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv)
		,uxc_dbif_get_str(dbif, 3, &rv)
		,uxc_dbif_get_int(dbif, 4, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_play_ment_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s"
		, clicktoconference_play_ment_rsp->resultCode
		, clicktoconference_play_ment_rsp->confID);
}

void clicktoconference_play_ment_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_play_ment_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_get_party_status_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantName] %s\n"
		"  [participantNumber] %s"
		, clicktoconference_get_party_status_rsp->resultCode
		, clicktoconference_get_party_status_rsp->confID
		, clicktoconference_get_party_status_rsp->participantName
		, clicktoconference_get_party_status_rsp->participantNumber);
}

void clicktoconference_get_party_status_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_get_party_status_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantName] %s\n"
		"  [participantNumber] %s"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv)
		,uxc_dbif_get_str(dbif, 3, &rv));
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
	ux_log(UXL_INFO, 
		"TCP [clicktoconference_cancel_party_rsp]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		, clicktoconference_cancel_party_rsp->resultCode
		, clicktoconference_cancel_party_rsp->confID
		, clicktoconference_cancel_party_rsp->participantNumber);
}

void clicktoconference_cancel_party_rsp_dbif_display(uxc_dbif_t *dbif) {
	int rv;
	ux_log(UXL_INFO, 
		"DBIF [clicktoconference_cancel_party_req]\n"
		"  [resultCode] %d\n"
		"  [confID] %s\n"
		"  [participantNumber] %s"
		,uxc_dbif_get_int(dbif, 0, &rv)
		,uxc_dbif_get_str(dbif, 1, &rv)
		,uxc_dbif_get_str(dbif, 2, &rv));
}

