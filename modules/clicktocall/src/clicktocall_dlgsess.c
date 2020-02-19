#include "local/clicktocall_dlgsess_l.h"
#include "uims/uims_plugin.h"
#include "uims/uims_util.h"
#include "local/clicktocall_plugin_l.h"
#include <cjson/cJSON.h>

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgstate_t
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(const char*) clicktocall_dlgstate_to_str( clicktocall_dlgstate_e dlgstate)
{
	switch( dlgstate ) {
		case CLICKTOCALL_DLGSTATE_INIT : return "INIT";
		case CLICKTOCALL_DLGSTATE_CALLING_TRYING : return "CALLING_TRYING";
		case CLICKTOCALL_DLGSTATE_CALLING_PROCEEDING : return "CALLING_PROCEEDING";
		case CLICKTOCALL_DLGSTATE_CALLING_ACCEPTED : return "CALLING_ACCEPTED";
		case CLICKTOCALL_DLGSTATE_CALLING_REJECTED : return "CALLING_REJECTED";
		case CLICKTOCALL_DLGSTATE_CALLING_ESTABLISHED : return "CALLING_ESTABLISHED";
		case CLICKTOCALL_DLGSTATE_CALLED_TRYING : return "CALLED_TRYING";
		case CLICKTOCALL_DLGSTATE_CALLED_PROCEEDING : return "CALLED_PROCEEDING";
		case CLICKTOCALL_DLGSTATE_CALLED_ACCEPTED : return "CALLED_ACCEPTED";
		case CLICKTOCALL_DLGSTATE_CALLED_REJECTED : return "CALLED_REJECTED";
		case CLICKTOCALL_DLGSTATE_CALLED_ESTABLISHED : return "CALLED_ESTABLISHED";
		case CLICKTOCALL_DLGSTATE_MS_TRYING : return "MS_TRYING";
		case CLICKTOCALL_DLGSTATE_MS_PROCEEDING : return "MS_PROCEEDING";
		case CLICKTOCALL_DLGSTATE_MS_ACCEPTED : return "MS_ACCEPTED";
		case CLICKTOCALL_DLGSTATE_MS_REJECTED : return "MS_REJECTED";
		case CLICKTOCALL_DLGSTATE_MS_ESTABLISHED : return "MS_ESTABLISHED";
		case CLICKTOCALL_DLGSTATE_TERMINATING : return "TERMINATING";
		case CLICKTOCALL_DLGSTATE_TERMINATED : return "TERMINATED";
		default : break;
	}

	return "UNKNOWN";
}

UX_DECLARE(void) clicktocall_dlgstate_change( clicktocall_dlgsess_t *dlgsess, clicktocall_dlgstate_e dlgstate)
{
	dlgsess->prevstate = dlgsess->dlgstate;
	dlgsess->dlgstate = dlgstate;
}

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgsess_t
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(clicktocall_dlgsess_t*) clicktocall_dlgsess_create( uims_sess_t *imssess, int *err)
{
	ux_mem_t *allocator;
	clicktocall_dlgsess_t *dlgsess;

	allocator = uims_sess_get_allocator(imssess);

	dlgsess = ux_zalloc( allocator, sizeof(clicktocall_dlgsess_t));
	if( dlgsess == NULL) {
		ux_log(UXL_MAJ, "Failed to create registration session. (err=No Memory)");
		if( err ) *err = UX_ENOMEM; 
		return NULL;
	}

	dlgsess->sess = imssess;
	uims_sess_set_data( imssess, dlgsess);

	ux_mpool_register_cleanup( (ux_mpool_t*)allocator, dlgsess, (ux_mpool_cleanup_f)clicktocall_dlgsess_final);
	return dlgsess;
}

ux_status_t clicktocall_dlgsess_final( clicktocall_dlgsess_t *dlgsess)
{
	ux_mem_t *allocator;

	allocator = uims_sess_get_allocator( dlgsess->sess);
	if( dlgsess->sessionid) ux_free( allocator, dlgsess->sessionid);
	if( dlgsess->subscribername) ux_free( allocator, dlgsess->subscribername);
	if( dlgsess->callingnumber) ux_free( allocator, dlgsess->callingnumber);
	if( dlgsess->callednumber) ux_free( allocator, dlgsess->callednumber);
	if( dlgsess->chargingnumber) ux_free( allocator, dlgsess->chargingnumber);
	if( dlgsess->watitngmentid) ux_free( allocator, dlgsess->watitngmentid);
	if( dlgsess->callmentid) ux_free( allocator, dlgsess->callmentid);
	if( dlgsess->callingcid) ux_free( allocator, dlgsess->callingcid);
	if( dlgsess->calledcid) ux_free( allocator, dlgsess->calledcid);
	if( dlgsess->sessionid) ux_free( allocator, dlgsess->sessionid);

	if( dlgsess->ostag) ux_free( allocator, dlgsess->ostag);
	if( dlgsess->ocall_id) ux_free( allocator, dlgsess->ocall_id);
	if( dlgsess->ofrom) ux_free( allocator, dlgsess->ofrom);
	if( dlgsess->oto) ux_free( allocator, dlgsess->oto);
	if( dlgsess->tstag) ux_free( allocator, dlgsess->tstag);
	if( dlgsess->tcall_id) ux_free( allocator, dlgsess->tcall_id);
	if( dlgsess->tfrom) ux_free( allocator, dlgsess->tfrom);
	if( dlgsess->tto) ux_free( allocator, dlgsess->tto);
	if( dlgsess->msstag) ux_free( allocator, dlgsess->msstag);
	if( dlgsess->mscall_id) ux_free( allocator, dlgsess->mscall_id);
	if( dlgsess->msfrom) ux_free( allocator, dlgsess->msfrom);
	if( dlgsess->msto) ux_free( allocator, dlgsess->msto);

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_http_start_req( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *reqmsg) 
{
	int rv;
	char buffer[2048];
	int bufsize = sizeof(buffer); 
	uhttp_body_t *body;
	const char *jsonstr;
	cJSON *json = NULL;
	const cJSON *networkType = NULL;
	const cJSON *sessionid = NULL;
	const cJSON *subscribername = NULL;
	const cJSON *callingnumber = NULL;
	const cJSON *callednumber = NULL;
	const cJSON *recording = NULL;
	const cJSON *chargingnumber = NULL;
	const cJSON *ringbacktonetype = NULL;
	const cJSON *watitngmentid = NULL;
	const cJSON *callmentid = NULL;
	const cJSON *callingcid = NULL;
	const cJSON *calledcid = NULL;
	const cJSON *hostcode = NULL;

	body = uhttp_msg_get_body( reqmsg->msg);
	if( body) {
		jsonstr = uhttp_body_get_str( body, buffer, bufsize, &rv);
		ux_log(UXL_INFO, "get_body(len=%d, %s)", bufsize, buffer);
	} else {
		ux_log(UXL_MAJ, "Fail to get HTTP body");
		return rv;
	}

	dlgsess->prevstate = dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_INIT;

	dlgsess->thread_id = uhttp_msg_get_thread_id(reqmsg->msg);
	dlgsess->conn_id = uhttp_msg_get_conn_id(reqmsg->msg);
	dlgsess->stream_id = uhttp_msg_get_id(reqmsg->msg);
	dlgsess->version = uhttp_msg_get_version(reqmsg->msg);

	json = cJSON_Parse(jsonstr);
	if ( json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
			ux_log(UXL_MAJ, "Fail to parse HTTP body. err=%s", error_ptr);
        }
		return UX_EINVAL;
	}

	networkType = cJSON_GetObjectItemCaseSensitive(json, "networkType");
    if (!cJSON_IsNumber(networkType)) {
		ux_log(UXL_MAJ, "Fail to get networkType JSON key");
		return UX_EINVAL;
	}
	dlgsess->networkType = networkType->valueint;

	sessionid = cJSON_GetObjectItemCaseSensitive(json, "sessionID");
    if (!cJSON_IsString(sessionid) || (sessionid->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get sessionID JSON key");
		return UX_EINVAL;	
    } 
	dlgsess->sessionid = ux_str_dup( sessionid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 

	callingnumber = cJSON_GetObjectItemCaseSensitive(json, "callingNumber");
    if (!cJSON_IsString(callingnumber) || (callingnumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get callingNumber JSON key");
		return UX_EINVAL;
    } 
	dlgsess->callingnumber = ux_str_dup( callingnumber->valuestring, uims_sess_get_allocator(dlgsess->sess)); 

	callednumber = cJSON_GetObjectItemCaseSensitive(json, "calledNumber");
    if (!cJSON_IsString(callednumber) || (callednumber->valuestring == NULL)) {
		ux_log(UXL_MAJ, "Fail to get calledNumber JSON key");
		return UX_EINVAL;
    } 
	dlgsess->callednumber = ux_str_dup( callednumber->valuestring, uims_sess_get_allocator(dlgsess->sess)); 

	recording = cJSON_GetObjectItemCaseSensitive(json, "recording");
    if (!cJSON_IsNumber(recording)) {
		ux_log(UXL_MAJ, "Fail to get recording JSON key");
		return UX_EINVAL;
	}
	dlgsess->recording = recording->valueint;

	subscribername = cJSON_GetObjectItemCaseSensitive(json, "subscriberName");
    if (cJSON_IsString(subscribername) && (subscribername->valuestring != NULL)) {
		dlgsess->subscribername = ux_str_dup( subscribername->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
	} 

	chargingnumber = cJSON_GetObjectItemCaseSensitive(json, "chargingNumber");
    if (cJSON_IsString(chargingnumber) && (chargingnumber->valuestring != NULL)) {
		dlgsess->chargingnumber = ux_str_dup( chargingnumber->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
	} 

	ringbacktonetype = cJSON_GetObjectItemCaseSensitive(json, "ringBackToneType");
    if (cJSON_IsNumber(ringbacktonetype)) {
		dlgsess->ringbacktonetype = ringbacktonetype->valueint;
	}
		
	watitngmentid = cJSON_GetObjectItemCaseSensitive(json, "waitingMentID");
    if (cJSON_IsString(watitngmentid) && (watitngmentid->valuestring != NULL)) {
		dlgsess->watitngmentid = ux_str_dup( watitngmentid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } 

	callmentid = cJSON_GetObjectItemCaseSensitive(json, "callMentID");
    if (cJSON_IsString(callmentid) && (callmentid->valuestring != NULL)) {
		dlgsess->callmentid = ux_str_dup( callmentid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } 
	
	callingcid = cJSON_GetObjectItemCaseSensitive(json, "callingCID");
    if (cJSON_IsString(callingcid) && (callingcid->valuestring != NULL)) {	
		dlgsess->callingcid = ux_str_dup( callingcid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
	}
	
	calledcid = cJSON_GetObjectItemCaseSensitive(json, "calledCID");
    if (cJSON_IsString(calledcid) && (calledcid->valuestring != NULL)) {
		dlgsess->calledcid = ux_str_dup( calledcid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } 
	
	hostcode = cJSON_GetObjectItemCaseSensitive(json, "hostCode");
    if (cJSON_IsNumber(hostcode)) {
		dlgsess->hostcode = hostcode->valueint;
	}

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_http_res( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *resmsg) 
{
	int rv, dlen;
	char *data;
	char sid[64];
	uhttp_hdrs_t *hdrs;
	uhttp_body_t *body;

	uhttp_msg_set_thread_id(resmsg->msg, dlgsess->thread_id);
	uhttp_msg_set_conn_id(resmsg->msg, dlgsess->conn_id);
	uhttp_msg_set_id(resmsg->msg, dlgsess->stream_id);
	uhttp_msg_set_version(resmsg->msg, dlgsess->version);

	cJSON *json = cJSON_CreateObject();

	cJSON_AddItemToObject(json, "networkType", cJSON_CreateNumber(dlgsess->networkType));
    cJSON_AddItemToObject(json, "sessionID", cJSON_CreateString(dlgsess->sessionid));
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(dlgsess->sess));
	cJSON_AddItemToObject(json, "gwSessionID", cJSON_CreateString(sid));
	cJSON_AddItemToObject(json, "resultCode", cJSON_CreateNumber(dlgsess->error));

	data = cJSON_Print(json);
    if (data == NULL) {
		ux_log(UXL_MAJ, "Failed to encode json");
    	return UX_EBADMSG;
    }

	dlen = strlen(data);
	hdrs = uhttp_msg_get_hdrs( resmsg->msg);
	if( data[0]) {
		body = uhttp_body_create_v( uhttp_msg_get_allocator(resmsg->msg), (uint8_t*)data, dlen); 
		if( body == NULL) {
			uxc_trace(UXCTL(1,MIN), "upa_httpcall_set_body: Failed to create body. (body=%s, err=%d,%s)",
					data, UX_ENOMEM, uhttp_errstr(UX_ENOMEM));
			cJSON_Delete(json);
			return UX_ENOMEM;
		}
		uhttp_msg_set_body( resmsg->msg, body);
	}
	rv = uhttp_hdrs_set_int( hdrs, "Content-Length", 0, dlen);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "upa_httpcall_set_body: Failed to set Content-Length value. (value=%d, err=%d,%s)",
				dlen, rv, uhttp_errstr(rv));
		cJSON_Delete(json);
		return UX_EINVAL;
	}
	
	rv = uhttp_hdrs_set_str( hdrs, "Content-Type", 0, "application/json");
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "upa_httpcall_set_body: Failed to set Content-Type value. (content_type=%s, err=%d,%s)",
				 "application/json", rv, uhttp_errstr(rv));
		cJSON_Delete(json);
		return UX_EINVAL;
	}
		
	cJSON_Delete(json);	

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_http_notify( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *resmsg, clicktocall_callto_e callto)
{
	int rv, dlen;
	char *data;
	char sid[64];
	uhttp_hdrs_t *hdrs;
	uhttp_body_t *body;

/*
	uhttp_msg_set_thread_id(resmsg->msg, dlgsess->thread_id);
	uhttp_msg_set_conn_id(resmsg->msg, dlgsess->conn_id);
	uhttp_msg_set_id(resmsg->msg, dlgsess->stream_id);
	uhttp_msg_set_version(resmsg->msg, dlgsess->version);
*/
	cJSON *json = cJSON_CreateObject();

	cJSON_AddItemToObject(json, "networkType", cJSON_CreateNumber(dlgsess->networkType));
    cJSON_AddItemToObject(json, "sessionID", cJSON_CreateString(dlgsess->sessionid));
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(dlgsess->sess));
	cJSON_AddItemToObject(json, "gwSessionID", cJSON_CreateString(sid));
	cJSON_AddItemToObject(json, "event", cJSON_CreateNumber(dlgsess->error));
	switch (callto) {
		case CALL_TO_CALLED:
			cJSON_AddItemToObject(json, "eventNumber", cJSON_CreateString(dlgsess->callingnumber));
			break;
		case CALL_TO_CALLING:
			cJSON_AddItemToObject(json, "eventNumber", cJSON_CreateString(dlgsess->callednumber));
			break;
		default:
			ux_log(UXL_MAJ, "Invalid callto parameter. callto=%d", callto);
    		return UX_EBADMSG;
	}

	data = cJSON_Print(json);
    if (data == NULL) {
		ux_log(UXL_MAJ, "Failed to encode json");
    	return UX_EBADMSG;
    }

	dlen = strlen(data);
	hdrs = uhttp_msg_get_hdrs( resmsg->msg);
	if( data[0]) {
		body = uhttp_body_create_v( uhttp_msg_get_allocator(resmsg->msg), (uint8_t*)data, dlen); 
		if( body == NULL) {
			uxc_trace(UXCTL(1,MIN), "clicktocall_dlgsess_make_http_notify: Failed to create body. (body=%s, err=%d,%s)",
					data, UX_ENOMEM, uhttp_errstr(UX_ENOMEM));
			cJSON_Delete(json);
			return UX_ENOMEM;
		}
		uhttp_msg_set_body( resmsg->msg, body);
	}
	rv = uhttp_hdrs_set_int( hdrs, "Content-Length", 0, dlen);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "clicktocall_dlgsess_make_http_notify: Failed to set Content-Length value. (value=%d, err=%d,%s)",
				dlen, rv, uhttp_errstr(rv));
		cJSON_Delete(json);
		return UX_EINVAL;
	}
	
	rv = uhttp_hdrs_set_str( hdrs, "Content-Type", 0, "application/json");
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "clicktocall_dlgsess_make_http_notify: Failed to set Content-Type value. (content_type=%s, err=%d,%s)",
				 "application/json", rv, uhttp_errstr(rv));
		cJSON_Delete(json);
		return UX_EINVAL;
	}
		
	ux_log(UXL_INFO, "clicktocall_dlgsess_make_http_notify: body=%s, len=%d", data, dlen);	
	cJSON_Delete(json);	

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_http_respond( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *resmsg)
{
	int rv, dlen;
	char *data;
	char sid[64];
	uhttp_hdrs_t *hdrs;
	uhttp_body_t *body;

/*
	uhttp_msg_set_thread_id(resmsg->msg, dlgsess->thread_id);
	uhttp_msg_set_conn_id(resmsg->msg, dlgsess->conn_id);
	uhttp_msg_set_id(resmsg->msg, dlgsess->stream_id);
	uhttp_msg_set_version(resmsg->msg, dlgsess->version);
*/
	cJSON *json = cJSON_CreateObject();

	cJSON_AddItemToObject(json, "networkType", cJSON_CreateNumber(dlgsess->networkType));
    cJSON_AddItemToObject(json, "sessionID", cJSON_CreateString(dlgsess->sessionid));
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(dlgsess->sess));
	cJSON_AddItemToObject(json, "gwSessionID", cJSON_CreateString(sid));
	cJSON_AddItemToObject(json, "returnCode", cJSON_CreateNumber(dlgsess->error));

	data = cJSON_Print(json);
    if (data == NULL) {
		ux_log(UXL_MAJ, "Failed to encode json");
    	return UX_EBADMSG;
    }

	dlen = strlen(data);
	hdrs = uhttp_msg_get_hdrs( resmsg->msg);
	if( data[0]) {
		body = uhttp_body_create_v( uhttp_msg_get_allocator(resmsg->msg), (uint8_t*)data, dlen); 
		if( body == NULL) {
			uxc_trace(UXCTL(1,MIN), "clicktocall_dlgsess_make_http_respond: Failed to create body. (body=%s, err=%d,%s)",
					data, UX_ENOMEM, uhttp_errstr(UX_ENOMEM));
			cJSON_Delete(json);
			return UX_ENOMEM;
		}
		uhttp_msg_set_body( resmsg->msg, body);
	}
	rv = uhttp_hdrs_set_int( hdrs, "Content-Length", 0, dlen);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "clicktocall_dlgsess_make_http_respond: Failed to set Content-Length value. (value=%d, err=%d,%s)",
				dlen, rv, uhttp_errstr(rv));
		cJSON_Delete(json);
		return UX_EINVAL;
	}
	
	rv = uhttp_hdrs_set_str( hdrs, "Content-Type", 0, "application/json");
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "clicktocall_dlgsess_make_http_respond: Failed to set Content-Type value. (content_type=%s, err=%d,%s)",
				 "application/json", rv, uhttp_errstr(rv));
		cJSON_Delete(json);
		return UX_EINVAL;
	}
		
	ux_log(UXL_INFO, "clicktocall_dlgsess_make_http_respond: body=%s, len=%d", data, dlen);	
	cJSON_Delete(json);	

	return UX_SUCCESS;
}


UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_sip_invite_req( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *reqmsg, clicktocall_callto_e callto)
{
	int rv;
	usip_mobj_t *req;

	req = reqmsg->mobj;
	if( req->call_id == NULL || req->from == NULL || req->from->tag == NULL || req->to == NULL || req->cseq == NULL) {
		ux_log( UXL_MAJ, "Missing mandatory header. (method=%s, call_id=%s, from=%s:%s, to=%s:%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_CALLID( req),
				USIP_MOBJ_GET_FROMUSER( req), USIP_MOBJ_GET_FROMTAG( req),
				USIP_MOBJ_GET_TOUSER( req), USIP_MOBJ_GET_TOTAG( req));
		return UX_EBADMSG;
	}
	
	dlgsess->method = req->request->method;

	switch (callto) {
		case CALL_TO_CALLING:
			clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLING_TRYING);
			dlgsess->ocseq = (req->cseq) ? req->cseq->seq : 1;
			dlgsess->ostag = ux_str_dup( req->from->tag, uims_sess_get_allocator(dlgsess->sess)); 

			rv = clicktocall_dlgsess_set_ocall_id( dlgsess, req->call_id->id);
			if( rv < USIP_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set ocall_id. (method=%s, call_id=%s, err=%d,%s)",
				usip_mobj_get_method( req), req->call_id->id, rv, usip_errstr(rv));
			}

			rv = clicktocall_dlgsess_set_ofrom( dlgsess, req->from);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set ofrom. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_oto( dlgsess, req->to);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set oto. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->to->uri), rv, usip_errstr(rv));
				return rv;
			}
			break;
		case CALL_TO_CALLED:
			clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLED_TRYING);
			dlgsess->tcseq = (req->cseq) ? req->cseq->seq : 1;
			dlgsess->tstag = ux_str_dup( req->from->tag, uims_sess_get_allocator(dlgsess->sess)); 
			
			rv = clicktocall_dlgsess_set_tcall_id( dlgsess, req->call_id->id);
			if( rv < USIP_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set tcall_id. (method=%s, call_id=%s, err=%d,%s)",
				usip_mobj_get_method( req), req->call_id->id, rv, usip_errstr(rv));
			}

			rv = clicktocall_dlgsess_set_tfrom( dlgsess, req->from);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set tfrom. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_tto( dlgsess, req->to);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set tto. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->to->uri), rv, usip_errstr(rv));
				return rv;
			}
			break;
		case CALL_TO_MS_CALLING:
		case CALL_TO_MS_CALLED:
			clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_MS_TRYING);
			dlgsess->mscseq = (req->cseq) ? req->cseq->seq : 1;
			dlgsess->msstag = ux_str_dup( req->from->tag, uims_sess_get_allocator(dlgsess->sess)); 
			
			rv = clicktocall_dlgsess_set_mscall_id( dlgsess, req->call_id->id);
			if( rv < USIP_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set mscall_id. (method=%s, call_id=%s, err=%d,%s)",
				usip_mobj_get_method( req), req->call_id->id, rv, usip_errstr(rv));
			}

			rv = clicktocall_dlgsess_set_msfrom( dlgsess, req->from);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set msfrom. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_msto( dlgsess, req->to);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set msto. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->to->uri), rv, usip_errstr(rv));
				return rv;
			}
			break;	
	}

	return USIP_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_sip_invite_res( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *resmsg, clicktocall_callto_e callto)
{
	int rv;
	usip_mobj_t *res;

	if (resmsg != NULL && resmsg->mobj != NULL && resmsg->mobj->status != NULL && resmsg->mobj->status->code <= 100) {
		return USIP_SUCCESS;
	}

	res = resmsg->mobj;
	if( res->call_id == NULL || res->from == NULL || res->from->tag == NULL || res->to == NULL || res->to->tag == NULL || res->cseq == NULL) {
		ux_log( UXL_MAJ, "Missing mandatory header. (method=%s, call_id=%s, from=%s:%s, to=%s:%s)",
				usip_mobj_get_method( res), USIP_MOBJ_GET_CALLID( res),
				USIP_MOBJ_GET_FROMUSER( res), USIP_MOBJ_GET_FROMTAG( res),
				USIP_MOBJ_GET_TOUSER( res), USIP_MOBJ_GET_TOTAG( res));
		return UX_EBADMSG;
	}

	ux_log(UXL_INFO, "clicktocall_dlgsess_handle_sip_invite_res: status=%d", resmsg->mobj->status->code)
	switch (callto) {
		case CALL_TO_CALLING:
			if( resmsg->mobj->status->code < 200) {
				clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLING_PROCEEDING);
			} else {
				if( resmsg->mobj->status->code < 300) {
					clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLING_ACCEPTED);
				} else {
					clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLING_REJECTED);
				}
			}
			rv = clicktocall_dlgsess_set_ocall_id( dlgsess, res->call_id->id);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set ocall-id. (method=%s, call_id=%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_CALLID( res), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_ofrom( dlgsess, res->from);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set ofrom. (method=%s, from=%s:%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_FROMUSER( res), USIP_MOBJ_GET_FROMTAG( res), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_oto( dlgsess, res->to);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set oto. (method=%s, to=%s:%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_TOUSER( res), USIP_MOBJ_GET_TOTAG( res), rv, usip_errstr(rv));
				return rv;
			}
			break;
		case CALL_TO_CALLED:
			if( resmsg->mobj->status->code < 200) {
				clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLED_PROCEEDING);
			} else {
				if( resmsg->mobj->status->code < 300) {
					clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLED_ACCEPTED);
				} else {
					clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLED_REJECTED);
				}
			}
			rv = clicktocall_dlgsess_set_tcall_id( dlgsess, res->call_id->id);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set tcall-id. (method=%s, call_id=%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_CALLID( res), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_tfrom( dlgsess, res->from);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set tfrom. (method=%s, from=%s:%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_FROMUSER( res), USIP_MOBJ_GET_FROMTAG( res), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_tto( dlgsess, res->to);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set tto. (method=%s, to=%s:%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_TOUSER( res), USIP_MOBJ_GET_TOTAG( res), rv, usip_errstr(rv));
				return rv;
			}
			break;
		case CALL_TO_MS_CALLING:
		case CALL_TO_MS_CALLED:
			if( resmsg->mobj->status->code < 200) {
				clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_MS_PROCEEDING);
			} else {
				if( resmsg->mobj->status->code < 300) {
					clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_MS_ACCEPTED);
				} else {
					clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_MS_REJECTED);
				}
			}
			rv = clicktocall_dlgsess_set_mscall_id( dlgsess, res->call_id->id);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set mscall-id. (method=%s, call_id=%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_CALLID( res), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_msfrom( dlgsess, res->from);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set msfrom. (method=%s, from=%s:%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_FROMUSER( res), USIP_MOBJ_GET_FROMTAG( res), rv, usip_errstr(rv));
				return rv;
			}

			rv = clicktocall_dlgsess_set_msto( dlgsess, res->to);
			if( rv < UX_SUCCESS) {
				ux_log( UXL_MAJ, "Failed to set msto. (method=%s, to=%s:%s, err=%d,%s)",
						usip_mobj_get_method( res), USIP_MOBJ_GET_TOUSER( res), USIP_MOBJ_GET_TOTAG( res), rv, usip_errstr(rv));
				return rv;
			}
			break;
	}
	
	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 originator call_id 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 call_id 정보
 */
UX_DECLARE(char*) clicktocall_dlgsess_get_ocall_id( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->ocall_id;
}

/**
 * @brief clicktocall session의 originator call id 값을 설정한다. 
 * @param dlgsess dlg session
 * @param call_id 설정할 call_id 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ocall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id)
{
	char *value;

	if( call_id == NULL) {
		if( dlgsess->ocall_id) ux_free( uims_sess_get_allocator(dlgsess->sess), dlgsess->ocall_id);
		dlgsess->ocall_id = NULL;
		return UX_SUCCESS;
	}

	value = ux_str_dup( call_id, uims_sess_get_allocator(dlgsess->sess));
	if( value == NULL) return UX_ENOMEM;

	if( dlgsess->ocall_id) ux_free( uims_sess_get_allocator(dlgsess->sess), dlgsess->ocall_id);
	dlgsess->ocall_id = value;
	return UX_SUCCESS;
}

/**
 * @brief 해당 세션 terminator call_id 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 call_id 정보
 */
UX_DECLARE(char*) clicktocall_dlgsess_get_tcall_id( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->tcall_id;
}

/**
 * @brief clicktocall session의 terminator call id 값을 설정한다. 
 * @param dlgsess dlg session
 * @param call_id 설정할 call_id 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tcall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id)
{
	char *value;

	if( call_id == NULL) {
		if( dlgsess->tcall_id) ux_free( uims_sess_get_allocator(dlgsess->sess), dlgsess->tcall_id);
		dlgsess->tcall_id = NULL;
		return UX_SUCCESS;
	}

	value = ux_str_dup( call_id, uims_sess_get_allocator(dlgsess->sess));
	if( value == NULL) return UX_ENOMEM;

	if( dlgsess->tcall_id) ux_free( uims_sess_get_allocator(dlgsess->sess), dlgsess->tcall_id);
	dlgsess->tcall_id = value;
	return UX_SUCCESS;
}

/**
 * @brief 해당 세션 ms call_id 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 call_id 정보
 */
UX_DECLARE(char*) clicktocall_dlgsess_get_mscall_id( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->mscall_id;
}

/**
 * @brief clicktocall session의 ms call id 값을 설정한다. 
 * @param dlgsess dlg session
 * @param call_id 설정할 call_id 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_mscall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id)
{
	char *value;

	if( call_id == NULL) {
		if( dlgsess->mscall_id) ux_free( uims_sess_get_allocator(dlgsess->sess), dlgsess->mscall_id);
		dlgsess->mscall_id = NULL;
		return UX_SUCCESS;
	}

	value = ux_str_dup( call_id, uims_sess_get_allocator(dlgsess->sess));
	if( value == NULL) return UX_ENOMEM;

	if( dlgsess->mscall_id) ux_free( uims_sess_get_allocator(dlgsess->sess), dlgsess->mscall_id);
	dlgsess->mscall_id = value;
	return UX_SUCCESS;
}

/**
 * @brief 해당 세션 originator From header 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 from 정보
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_ofrom( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->ofrom;
}

/**
 * @brief clicktocall session의 originator From header 값을 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 발신 from
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ofrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *from;

	from = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( from == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator From header.");
		return UX_ENOMEM;
	}

	if( dlgsess->ofrom) usip_hdr_destroy( (usip_hdr_t*)dlgsess->ofrom);
	dlgsess->ofrom = from;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 terminator From header 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 from 정보
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_tfrom( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->tfrom;
}

/**
 * @brief clicktocall session의 terminator From header 값을 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 발신 from
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tfrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *from;

	from = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( from == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate terminator From header.");
		return UX_ENOMEM;
	}

	if( dlgsess->tfrom) usip_hdr_destroy( (usip_hdr_t*)dlgsess->tfrom);
	dlgsess->tfrom = from;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 ms From header 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 from 정보
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_msfrom( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->msfrom;
}

/**
 * @brief clicktocall session의 ms From header 값을 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 발신 from
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_msfrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *from;

	from = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( from == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate ms From header.");
		return UX_ENOMEM;
	}

	if( dlgsess->msfrom) usip_hdr_destroy( (usip_hdr_t*)dlgsess->msfrom);
	dlgsess->msfrom = from;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 originator To header 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 to 정보
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_oto( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->oto;
}

/**
 * @brief clicktocall session의 originator To header 값을 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 발신 to
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_oto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *to;

	to = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( to == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator To header.");
		return UX_ENOMEM;
	}

	if( dlgsess->oto) usip_hdr_destroy( (usip_hdr_t*)dlgsess->oto);
	dlgsess->oto = to;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 terminator To header 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 to 정보
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_tto( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->tto;
}

/**
 * @brief clicktocall session의 terminator To header 값을 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 발신 to
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *to;

	to = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( to == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate terminator To header.");
		return UX_ENOMEM;
	}

	if( dlgsess->tto) usip_hdr_destroy( (usip_hdr_t*)dlgsess->tto);
	dlgsess->tto = to;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 ms To header 정보를 반환한다.
 * @param dlgsess dlg session
 * @return ms to 정보
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_msto( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->msto;
}

/**
 * @brief clicktocall session의 ms To header 값을 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 ms to
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_msto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *to;

	to = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( to == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate ms To header.");
		return UX_ENOMEM;
	}

	if( dlgsess->msto) usip_hdr_destroy( (usip_hdr_t*)dlgsess->msto);
	dlgsess->msto = to;

	return USIP_SUCCESS;
}

UX_DECLARE(int) clicktocall_dlgsess_sprint( clicktocall_dlgsess_t *dlgsess, char *buffer, int buflen)
{
	int len;

	len =  uims_util_sprint_ttl( buffer,     buflen,     "[CLICKTOCALL SESSION]\n");
	len += uims_util_sprint_u64( buffer+len, buflen-len, " - UXC_SESS     = %lld\n", 
			(unsigned long long)uims_sess_get_id( dlgsess->sess), 20);
	len += uims_util_sprint_u64( buffer+len, buflen-len, " - EXTIME       = %lld\n", 
			(unsigned long long)dlgsess->extime, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, " - ERROR        = %d\n", dlgsess->error, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, " - DLGSTATE     = %s\n", 
			(char*)clicktocall_dlgstate_to_str(dlgsess->dlgstate), 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, " - PREVSTATE     = %s\n", 
			(char*)clicktocall_dlgstate_to_str(dlgsess->prevstate), 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, " - METHOD     = %s\n", 
			(char*)usip_method_to_str( dlgsess->method), 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + HTTP SESSION_ID = %s\n",dlgsess->sessionid, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + CALLING    = %s\n", dlgsess->callingnumber, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + CALLED     = %s\n", dlgsess->callednumber, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, "   + RECORDING  = %d\n", dlgsess->recording, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + SUBSCRIBER = %s\n", dlgsess->subscribername, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + CHARGING   = %s\n", dlgsess->chargingnumber, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, "   + RINGTONE   = %d\n", dlgsess->ringbacktonetype, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + WTMENTID   = %s\n", dlgsess->watitngmentid, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + CALLMENTID = %s\n", dlgsess->callmentid, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + CALLINGCID = %s\n", dlgsess->callingcid, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + CALLEDCID  = %s\n", dlgsess->calledcid, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, "   + HOSTCODE   = %d\n", dlgsess->hostcode, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, "   + OCSEQ      = %d\n", dlgsess->ocseq, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + OSTAG      = %s\n", dlgsess->ostag, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + OCALL_ID   = %s\n", dlgsess->ocall_id, 20);	
	len += uims_util_sprint_hdr( buffer+len, buflen-len, "   + OFROM      = %s\n", 
			(usip_hdr_t*)dlgsess->ofrom, 20);
	len += uims_util_sprint_hdr( buffer+len, buflen-len, "   + OTO        = %s\n", 
			(usip_hdr_t*)dlgsess->oto, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, "   + TCSEQ      = %d\n", dlgsess->tcseq, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + TSTAG      = %s\n", dlgsess->tstag, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + TCALL_ID   = %s\n", dlgsess->tcall_id, 20);
	len += uims_util_sprint_hdr( buffer+len, buflen-len, "   + TFROM      = %s\n", 
			(usip_hdr_t*)dlgsess->tfrom, 20);
	len += uims_util_sprint_hdr( buffer+len, buflen-len, "   + TTO        = %s\n", 
			(usip_hdr_t*)dlgsess->tto, 20);
	len += uims_util_sprint_int( buffer+len, buflen-len, "   + MSCSEQ      = %d\n", dlgsess->mscseq, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + MSSTAG      = %s\n", dlgsess->msstag, 20);
	len += uims_util_sprint_str( buffer+len, buflen-len, "   + MSCALL_ID   = %s\n", dlgsess->mscall_id, 20);
	len += uims_util_sprint_hdr( buffer+len, buflen-len, "   + MSFROM      = %s\n", 
			(usip_hdr_t*)dlgsess->msfrom, 20);
	len += uims_util_sprint_hdr( buffer+len, buflen-len, "   + MSTO        = %s\n", 
			(usip_hdr_t*)dlgsess->msto, 20);

	ux_log(UXL_INFO, "clicktocall_dlgsess_sprint: len=%d", len);
	return len;
}


UX_DECLARE(ux_mem_t*) clicktocall_dlgsess_get_allocator( clicktocall_dlgsess_t *dlgsess)
{
	return uims_sess_get_allocator( dlgsess->sess);
}

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgdao_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal CLICKTOCALL dialog DAO를 초기화한다.
 * @param dao CLICKTOCALL dialog DAO
 * @param routermgr router manager
 * @return 실행 결과
 */
ux_status_t clicktocall_dlgdao_init( clicktocall_dlgdao_t *dao, uims_dbmgr_t *dbmgr, clicktocall_conf_t *conf)
{
	dao->dbmgr = dbmgr;
	dao->db = uims_dbmgr_get_n( dbmgr, "TNTDB");
	if(dao->db == NULL) {
		ux_log(UXL_MAJ, "Failed to get TNTDB");
		return UX_ENOENT;
	}
	dao->conf = conf;

	return UX_SUCCESS;
}

void clicktocall_dlgdao_final( clicktocall_dlgdao_t *dao)
{
	//Do Nonthing
}

clicktocall_dlgsess_t* clicktocall_dlgdao_find( clicktocall_dlgdao_t *dao,
						uims_sessmgr_t *sessmgr, uims_sessid_t sessid)
{
	static const char* stmtid = "DLGSESS:FIND";
	static const char* query = "SELECT * FROM 1 WHERE 0=?";

	int rv;
	uint8_t dlgstate, method, network_type, recording, ringbacktone_type, hostcode;
	uint16_t state;
	uint32_t ocseq, tcseq, mscseq;
	uint64_t rsessid, extime;
	char *http_session_id, *calling_number, *called_number, *subscriber_name, *charging_number, *waitingment_id, *callment_id, *calling_cid, *called_cid;
	char *ostag, *ocall_id, *ofrom, *oto, *tstag, *tcall_id, *tfrom, *tto, *msstag, *mscall_id, *msfrom, *msto;
	ux_mem_t *allocator;
	uxc_sesshdr_t *sesshdr;
	uxc_service_t *service;
	uims_sess_t *sess;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset, *rsltset;
	clicktocall_dlgsess_t *dlgsess;
	upa_sippa_t *sippa;
	upa_sipmsg_t sipmsg[1];
	char tag[128];

	service = uims_sessmgr_get_service( sessmgr, uims_sessid_get_service_id(sessid));
	if( service == NULL) {
		ux_log(UXL_MIN, "Failed to get service from session id. (sessid=%llu, svcid=%u)",
				(unsigned long long)sessid, (uint16_t)uims_sessid_get_service_id(sessid));
		return NULL;
	}

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		return NULL;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		uims_dbstmt_close( stmt);
		return NULL;
	}

	rv = uims_dbdataset_write( paraset, 1,
			//name, type, value, [length:octet only]
			"sess_id", UIMS_DBTYPE_UINT64, sessid);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	rsltset = uims_dbstmt_get_rsltset( stmt);
	if( rsltset == NULL) {
		ux_log(UXL_MIN, "Failed to get result set from statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	rv = uims_dbdataset_next( rsltset);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_INFO, "IMS session for dialog doesn' exist. (sessid=%llu, err=%d,%s)",
				(unsigned long long)sessid, rv, uxc_errnostr(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	rv = uims_dbdataset_read( rsltset, 30,
			//name, type, value, [length:octet only]
			"sess_id", UIMS_DBTYPE_UINT64, &rsessid,
			"extime", UIMS_DBTYPE_UINT64, &extime,
			"state", UIMS_DBTYPE_UINT16, &state,
			"dlgstate", UIMS_DBTYPE_UINT8, &dlgstate,
			"method", UIMS_DBTYPE_UINT8, &method,
			"network_type", UIMS_DBTYPE_UINT8, &network_type, 
			"http_session_id", UIMS_DBTYPE_STR, &http_session_id,
			"calling_number", UIMS_DBTYPE_STR, &calling_number,
			"called_number", UIMS_DBTYPE_STR, &called_number,
			"recording", UIMS_DBTYPE_UINT8, &recording, 
			"subscriber_name", UIMS_DBTYPE_STR, &subscriber_name,
			"charging_number", UIMS_DBTYPE_STR, &charging_number,
			"ringbacktone_type", UIMS_DBTYPE_UINT8, &ringbacktone_type, 
			"waitingment_id", UIMS_DBTYPE_STR, &waitingment_id,
			"callment_id", UIMS_DBTYPE_STR, &callment_id,
			"calling_cid", UIMS_DBTYPE_STR, &calling_cid,
			"called_cid", UIMS_DBTYPE_STR, &called_cid,
			"hostcode", UIMS_DBTYPE_UINT8, &hostcode, 
			"ocseq", UIMS_DBTYPE_UINT32, &ocseq,
			"ocall_id", UIMS_DBTYPE_STR, &ocall_id,
			"ofrom", UIMS_DBTYPE_STR, &ofrom,
			"oto", UIMS_DBTYPE_STR, &oto,
			"tcseq", UIMS_DBTYPE_UINT32, &tcseq,
			"tcall_id", UIMS_DBTYPE_STR, &tcall_id,
			"tfrom", UIMS_DBTYPE_STR, &tfrom,
			"tto", UIMS_DBTYPE_STR, &tto,
			"mscseq", UIMS_DBTYPE_UINT32, &mscseq,
			"mscall_id", UIMS_DBTYPE_STR, &mscall_id,
			"msfrom", UIMS_DBTYPE_STR, &msfrom,
			"msto", UIMS_DBTYPE_STR, &msto	
			);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to read value from result set. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	ux_log(UXL_INFO, "[DLGSESS:FIND] (sess_id=%llu, state=%d, extime=%llu, dlgstate=%d, method=%u, "
			"network_type=%u, http_session_id=%s, calling_number=%s, called_number=%s, recording=%u, "
			"subscriber_name=%s, charging_number=%s, ringbacktone_type=%u, waitingment_id=%s, "
			"callment_id=%s, calling_cid=%s, called_cid=%s, hostcode=%u, "
			"ocseq=%d, ostag=%s, ocall_id=%s, ofrom=%s, oto=%s, "
			"tcseq=%d, tstag=%s, tcall_id=%s, tfrom=%s, tto=%s, "
			"mscseq=%d, msstag=%s, mscall_id=%s, msfrom=%s, msto=%s)",
			(unsigned long long)rsessid, state, (unsigned long long)extime, dlgstate, method, 
			network_type, http_session_id ? http_session_id : "NULL", calling_number ? calling_number : "NULL", called_number ? called_number : "NULL", recording, 
			subscriber_name ? subscriber_name : "NULL", charging_number ? charging_number : "NULL", ringbacktone_type, waitingment_id ? waitingment_id : "NULL",
			callment_id ? callment_id : "NULL", calling_cid ? calling_cid : "NULL", called_cid ? called_cid : "NULL", hostcode,
			ocseq, ostag ? ostag : "NULL", ocall_id ? ocall_id : "NULL", ofrom ? ofrom : "NULL", oto ? oto : "NULL",
			tcseq, tstag ? tstag : "NULL", tcall_id ? tcall_id : "NULL", tfrom ? tfrom : "NULL", tto ? tto : "NULL",
			mscseq, msstag ? msstag : "NULL", mscall_id ? mscall_id : "NULL", msfrom ? msfrom : "NULL", msto ? msto : "NULL");

	sess = uims_sessmgr_alloc_with_id( sessmgr, service, sessid, &rv);
	if( sess == NULL) {
		ux_log(UXL_MIN, "Failed to allocate IMS session for dialog. (sessid=%llu, err=%d,%s)",
				(unsigned long long)sessid, rv, uxc_errnostr(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}
	
	dlgsess = clicktocall_dlgsess_create( sess, &rv);
	if( dlgsess == NULL) {
		ux_log(UXL_MIN, "Failed to allocate dialog session. (sessid=%llu, err=%d,%s)",
			(unsigned long long)sessid, rv, uxc_errnostr(rv));
		uims_dbstmt_close( stmt);
		uims_sessmgr_free( sessmgr, sess);
		return NULL;
	}

	allocator = uims_sess_get_allocator(sess);
	sesshdr = uims_sess_get_sesshdr( sess);
	sesshdr->state = state;
	dlgsess->extime = extime;
	dlgsess->dlgstate = dlgstate;
	dlgsess->method = method;

	dlgsess->networkType = network_type;
	dlgsess->sessionid =  ux_str_dup( http_session_id,  allocator);
	dlgsess->callingnumber =  ux_str_dup( calling_number,  allocator);
	dlgsess->callednumber =  ux_str_dup( called_number,  allocator);
	dlgsess->recording = recording;
	dlgsess->subscribername =  ux_str_dup( subscriber_name,  allocator);
	dlgsess->chargingnumber =  ux_str_dup( charging_number,  allocator);
	dlgsess->ringbacktonetype = ringbacktone_type;
	dlgsess->watitngmentid =  ux_str_dup( waitingment_id,  allocator);
	dlgsess->callmentid =  ux_str_dup( callment_id,  allocator);
	dlgsess->callingcid =  ux_str_dup( calling_cid,  allocator);
	dlgsess->calledcid =  ux_str_dup( called_cid,  allocator);
	dlgsess->hostcode = hostcode;

	sippa = dao->conf->sippa;
	sipmsg->sessinfo->sessid = sessid; 
	sipmsg->sessinfo->did = 0;
	upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
	dlgsess->ostag = ux_str_dup( tag, allocator); 
	dlgsess->ocseq = ocseq;
	dlgsess->ocall_id =  ux_str_dup( ocall_id,  allocator);
	if( ofrom && ofrom[0]) {
		dlgsess->ofrom = (usip_from_hdr_t*)usip_hdr_create_v(
								allocator, usip_from_hdef(), ofrom);
	} else {
		dlgsess->ofrom = NULL;
	}
	if( oto && oto[0]) {
		dlgsess->oto = (usip_to_hdr_t*)usip_hdr_create_v(
								allocator, usip_from_hdef(), oto);
	} else {
		dlgsess->oto = NULL;
	}
	sipmsg->sessinfo->did = 1;
	upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
	dlgsess->tstag = ux_str_dup( tag, allocator); 
	dlgsess->tcseq = tcseq;
	dlgsess->tcall_id =  ux_str_dup( tcall_id,  allocator);
	if( tfrom && tfrom[0]) {
		dlgsess->tfrom = (usip_from_hdr_t*)usip_hdr_create_v(
								allocator, usip_from_hdef(), tfrom);
	} else {
		dlgsess->tfrom = NULL;
	}
	if( tto && tto[0]) {
		dlgsess->tto = (usip_to_hdr_t*)usip_hdr_create_v(
								allocator, usip_to_hdef(), tto);
	} else {
		dlgsess->tto = NULL;
	}
	sipmsg->sessinfo->did = 2;
	upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
	dlgsess->msstag = ux_str_dup( tag, allocator); 
	dlgsess->mscseq = mscseq;
	dlgsess->mscall_id =  ux_str_dup( mscall_id,  allocator);
	if( msfrom && msfrom[0]) {
		dlgsess->msfrom = (usip_from_hdr_t*)usip_hdr_create_v(
								allocator, usip_from_hdef(), msfrom);
	} else {
		dlgsess->msfrom = NULL;
	}
	if( msto && msto[0]) {
		dlgsess->msto = (usip_to_hdr_t*)usip_hdr_create_v(
								allocator, usip_to_hdef(), msto);
	} else {
		dlgsess->msto = NULL;
	}

	uims_dbstmt_close( stmt);

	char buf[4096];
	int buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);
	
	return dlgsess;
}

ux_status_t clicktocall_dlgdao_insert( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:INSERT";
	static const char* query = "INSERT INTO table1 VALUES(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?)";

	int rv, bufsize, buflen;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;
	char *ofrom, *oto, *tfrom, *tto, *msfrom, *msto;
	char *pbuffer, buffer[8192];

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);
	dlgsess->extime = time(NULL);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS INSERT. (sessid=%llu, svcst=%d, extime=%lu)",
			(unsigned long long)uims_sess_get_id(dlgsess->sess), sesshdr->state, dlgsess->extime);

	char buf[4096];
	buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);		

	bufsize = sizeof(buffer); 
	pbuffer = buffer; 
	do {
		buflen = 0;
		ofrom = pbuffer+buflen; 
		if( dlgsess->ofrom) {
			rv = usip_nameaddr_encode( dlgsess->ofrom, ofrom, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) ofrom[0] = '\0';
		}
		buflen++;

		oto = pbuffer+buflen; 
		if( dlgsess->oto) {
			rv = usip_nameaddr_encode( dlgsess->oto, oto, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) oto[0] = '\0';
		}
		buflen++;
	
		tfrom = pbuffer+buflen; 
		if( dlgsess->tfrom) {
			rv = usip_nameaddr_encode( dlgsess->tfrom, tfrom, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) tfrom[0] = '\0';
		}
		buflen++;

		tto = pbuffer+buflen; 
		if( dlgsess->tto) {
			rv = usip_nameaddr_encode( dlgsess->tto, tto, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) tto[0] = '\0';
		}
		buflen++;

		msfrom = pbuffer+buflen; 
		if( dlgsess->msfrom) {
			rv = usip_nameaddr_encode( dlgsess->msfrom, msfrom, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) msfrom[0] = '\0';
		}
		buflen++;

		msto = pbuffer+buflen; 
		if( dlgsess->msto) {
			rv = usip_nameaddr_encode( dlgsess->msto, msto, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) msto[0] = '\0';
		}
		buflen++;

		if( buflen < bufsize) break;
		if( pbuffer != buffer) free(pbuffer);
		bufsize = buflen+1;
		pbuffer = malloc( bufsize);
		if(pbuffer == NULL) {
			return UIMS_DBERR_NO_MEMORY;
		}
	} while(buflen < bufsize);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		if(pbuffer != buffer) free(pbuffer);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		uims_dbstmt_close( stmt);
		if(pbuffer != buffer) free(pbuffer);
		return UX_ERR_FAILED;
	}

	rv = uims_dbdataset_write( paraset, 30,
			//name, type, value, [length:octet only]
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id(dlgsess->sess),
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"method", UIMS_DBTYPE_UINT8, dlgsess->method,
			"network_type", UIMS_DBTYPE_UINT8, dlgsess->networkType, 
			"http_session_id", UIMS_DBTYPE_STR, dlgsess->sessionid,
			"calling_number", UIMS_DBTYPE_STR, dlgsess->callingnumber,
			"called_number", UIMS_DBTYPE_STR, dlgsess->callednumber,
			"recording", UIMS_DBTYPE_UINT8, dlgsess->recording, 
			"subscriber_name", UIMS_DBTYPE_STR, dlgsess->subscribername ? dlgsess->subscribername : "",
			"charging_number", UIMS_DBTYPE_STR, dlgsess->chargingnumber ? dlgsess->chargingnumber : "",
			"ringbacktone_type", UIMS_DBTYPE_UINT8, dlgsess->ringbacktonetype, 
			"waitingment_id", UIMS_DBTYPE_STR, dlgsess->watitngmentid ? dlgsess->watitngmentid : "",
			"callment_id", UIMS_DBTYPE_STR, dlgsess->callmentid ? dlgsess->callmentid : "",
			"calling_cid", UIMS_DBTYPE_STR, dlgsess->callingcid ? dlgsess->callingcid : "",
			"called_cid", UIMS_DBTYPE_STR, dlgsess->calledcid ? dlgsess->calledcid : "",
			"hostcode", UIMS_DBTYPE_UINT8, dlgsess->hostcode, 
			"ocseq", UIMS_DBTYPE_UINT32, dlgsess->ocseq,
			"ocall_id", UIMS_DBTYPE_STR, dlgsess->ocall_id,
			"ofrom", UIMS_DBTYPE_STR, ofrom,
			"oto", UIMS_DBTYPE_STR, oto,
			"tcseq", UIMS_DBTYPE_UINT32, dlgsess->tcseq,
			"tcall_id", UIMS_DBTYPE_STR, dlgsess->tcall_id,
			"tfrom", UIMS_DBTYPE_STR, tfrom,
			"tto", UIMS_DBTYPE_STR, tto,
			"mscseq", UIMS_DBTYPE_UINT32, dlgsess->mscseq,
			"mscall_id", UIMS_DBTYPE_STR, dlgsess->mscall_id,
			"msfrom", UIMS_DBTYPE_STR, msfrom,
			"msto", UIMS_DBTYPE_STR, msto);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if(pbuffer != buffer) free(pbuffer);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if(pbuffer != buffer) free(pbuffer);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);
	if(pbuffer != buffer) free(pbuffer);

	return UX_SUCCESS;
}

ux_status_t clicktocall_dlgdao_remove( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:REMOVE";
	static const char* query = "DELETE FROM table1 WHERE index0=?";

	int rv;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS REMOVE. (sessid=%llu, extime=%lu)",
			(unsigned long long)uims_sess_get_id(dlgsess->sess), dlgsess->extime);

	char buf[4096];
	int buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	rv = uims_dbdataset_write( paraset, 1,
			//name, type, value, [length:octet only]
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id(dlgsess->sess));
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);
	
	return UX_SUCCESS;
}

ux_status_t clicktocall_dlgdao_update( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	dlgsess->extime = time(NULL);

	char buf[4096];
	int buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);

	if( dlgsess->dlgstate < CLICKTOCALL_DLGSTATE_CALLING_PROCEEDING) {
		return UX_SUCCESS;
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLING_PROCEEDING || 
		dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLING_ACCEPTED) {
		if( dlgsess->prevstate != dlgsess->dlgstate) {
			clicktocall_dlgstate_change(dlgsess,  dlgsess->dlgstate);
			return clicktocall_dlgdao_update_calling_p( dao, dlgsess);
		}
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLED_PROCEEDING ||
		dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLED_ACCEPTED) {
		if( dlgsess->prevstate != dlgsess->dlgstate) {
			clicktocall_dlgstate_change(dlgsess,  dlgsess->dlgstate);
			return clicktocall_dlgdao_update_called_p( dao, dlgsess);
		}	
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_MS_PROCEEDING ||
		dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_MS_ACCEPTED) {
		if( dlgsess->prevstate != dlgsess->dlgstate) {
			clicktocall_dlgstate_change(dlgsess,  dlgsess->dlgstate);
			return clicktocall_dlgdao_update_ms_p( dao, dlgsess);
		}		
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLING_ESTABLISHED) {
		return clicktocall_dlgdao_update_calling_e( dao, dlgsess);
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLED_ESTABLISHED) {
		return clicktocall_dlgdao_update_called_e( dao, dlgsess);
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_MS_ESTABLISHED) {
		return clicktocall_dlgdao_update_ms_e( dao, dlgsess);
	}

	return UX_SUCCESS;
}

/**
 * @internal calling proceeding 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_calling_p( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:UPDATE_CALLING_P";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, "
				"column18=?, column19=?, column20=?, column21=? WHERE index0=?";

	int rv, bufsize, buflen;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;
	char *ofrom, *oto;
	char *pbuffer, buffer[8192];

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE_CALLING_P. (sessid=%llu, svcst=%d, state=%s, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state,
			clicktocall_dlgstate_to_str( dlgsess->dlgstate), dlgsess->extime);

	bufsize = sizeof(buffer); 
	pbuffer = buffer; 
	do {
		buflen = 0;
		ofrom = pbuffer+buflen; 
		if( dlgsess->ofrom) {
			rv = usip_nameaddr_encode( dlgsess->ofrom, ofrom, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) ofrom[0] = '\0';
		}
		buflen++;

		oto = pbuffer+buflen; 
		if( dlgsess->oto) {
			rv = usip_nameaddr_encode( dlgsess->oto, oto, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) oto[0] = '\0';
		}
		buflen++;

		if( buflen < bufsize) break;
		if( pbuffer != buffer) free(pbuffer);
		bufsize = buflen+1;
		pbuffer = malloc( bufsize);
		if(pbuffer == NULL) {
			ux_log(UXL_MAJ, "Failed to allocate update buffer.");
			return UIMS_DBERR_NO_MEMORY;
		}
	} while(buflen < bufsize);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		if( pbuffer != buffer) free( pbuffer);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_CALLING_P (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, "
			"ocseq=%u, ocall_id=%s, ofrom=%s, oto=%s)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, 
			dlgsess->ocseq, dlgsess->ocall_id, ofrom, oto);

	rv = uims_dbdataset_write( paraset, 8,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"ocseq", UIMS_DBTYPE_UINT32, dlgsess->ocseq,
			"ocall_id", UIMS_DBTYPE_STR, dlgsess->ocall_id,
			"ofrom", UIMS_DBTYPE_STR, ofrom,
			"oto", UIMS_DBTYPE_STR, oto,
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id( dlgsess->sess));

	

	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);
	if( pbuffer != buffer) free( pbuffer);

	if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_CALLING_ACCEPTED) { 
		clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLING_ESTABLISHED);
	}
	
	return UX_SUCCESS;
}

/**
 * @internal called proceeding 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_called_p( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:UPDATE_CALLED_P";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, "
				"column22=?, column23=?, column24=?, column25=? WHERE index0=?";

	int rv, bufsize, buflen;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;
	char *tfrom, *tto;
	char *pbuffer, buffer[8192];

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE_CALLED_P. (sessid=%llu, svcst=%d, state=%s, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state,
			clicktocall_dlgstate_to_str( dlgsess->dlgstate), dlgsess->extime);

	bufsize = sizeof(buffer); 
	pbuffer = buffer; 
	do {
		buflen = 0;
		tfrom = pbuffer+buflen; 
		if( dlgsess->tfrom) {
			rv = usip_nameaddr_encode( dlgsess->tfrom, tfrom, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) tfrom[0] = '\0';
		}
		buflen++;

		tto = pbuffer+buflen; 
		if( dlgsess->tto) {
			rv = usip_nameaddr_encode( dlgsess->tto, tto, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) tto[0] = '\0';
		}
		buflen++;

		if( buflen < bufsize) break;
		if( pbuffer != buffer) free(pbuffer);
		bufsize = buflen+1;
		pbuffer = malloc( bufsize);
		if(pbuffer == NULL) {
			ux_log(UXL_MAJ, "Failed to allocate update buffer.");
			return UIMS_DBERR_NO_MEMORY;
		}
	} while(buflen < bufsize);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		if( pbuffer != buffer) free( pbuffer);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_CALLED_P (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, "
			"tcseq=%u, tcall_id=%s, tfrom=%s, tto=%s)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, 
			dlgsess->tcseq, dlgsess->tcall_id, tfrom, tto);

	rv = uims_dbdataset_write( paraset, 8,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"tcseq", UIMS_DBTYPE_UINT32, dlgsess->tcseq,
			"tcall_id", UIMS_DBTYPE_STR, dlgsess->tcall_id,
			"tfrom", UIMS_DBTYPE_STR, tfrom,
			"tto", UIMS_DBTYPE_STR, tto,
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id( dlgsess->sess));
	

	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);
	if( pbuffer != buffer) free( pbuffer);

	if( dlgsess->dlgstate >= CLICKTOCALL_DLGSTATE_CALLED_ACCEPTED) { 
		clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_CALLED_ESTABLISHED);
	}
	
	return UX_SUCCESS;
}

/**
 * @internal ms proceeding 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_ms_p( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:UPDATE_MS_P";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, "
				"column26=?, column27=?, column28=?, column29=? WHERE index0=?";

	int rv, bufsize, buflen;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;
	char *msfrom, *msto;
	char *pbuffer, buffer[8192];

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE_MS_P. (sessid=%llu, svcst=%d, state=%s, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state,
			clicktocall_dlgstate_to_str( dlgsess->dlgstate), dlgsess->extime);

	bufsize = sizeof(buffer); 
	pbuffer = buffer; 
	do {
		buflen = 0;
		msfrom = pbuffer+buflen; 
		if( dlgsess->msfrom) {
			rv = usip_nameaddr_encode( dlgsess->msfrom, msfrom, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) msfrom[0] = '\0';
		}
		buflen++;

		msto = pbuffer+buflen; 
		if( dlgsess->msto) {
			rv = usip_nameaddr_encode( dlgsess->msto, msto, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) msto[0] = '\0';
		}
		buflen++;

		if( buflen < bufsize) break;
		if( pbuffer != buffer) free(pbuffer);
		bufsize = buflen+1;
		pbuffer = malloc( bufsize);
		if(pbuffer == NULL) {
			ux_log(UXL_MAJ, "Failed to allocate update buffer.");
			return UIMS_DBERR_NO_MEMORY;
		}
	} while(buflen < bufsize);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		if( pbuffer != buffer) free( pbuffer);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_MS_P (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, "
			"mscseq=%u, mscall_id=%s, msfrom=%s, msto=%s)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, 
			dlgsess->mscseq, dlgsess->mscall_id, msfrom, msto);

	rv = uims_dbdataset_write( paraset, 8,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"mscseq", UIMS_DBTYPE_UINT32, dlgsess->mscseq,
			"mscall_id", UIMS_DBTYPE_STR, dlgsess->mscall_id,
			"msfrom", UIMS_DBTYPE_STR, msfrom,
			"msto", UIMS_DBTYPE_STR, msto,
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id( dlgsess->sess));
	

	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		if( pbuffer != buffer) free( pbuffer);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);
	if( pbuffer != buffer) free( pbuffer);

	if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_MS_ACCEPTED) { 
		clicktocall_dlgstate_change(dlgsess, CLICKTOCALL_DLGSTATE_MS_ESTABLISHED);
	}
	
	return UX_SUCCESS;
}

/**
 * @internal calling established 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_calling_e( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:UPDATE_CALLING_E";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, column18=? WHERE index0=?";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE_CALLING_E. (sessid=%llu, svcst=%d, state=CALLING_ESTABLISHED, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->extime);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_CALLING_E (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, ocseq=%u)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, dlgsess->ocseq);

	rv = uims_dbdataset_write( paraset, 5,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"ocseq", UIMS_DBTYPE_UINT32, dlgsess->ocseq,
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id( dlgsess->sess));

	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);

	return UX_SUCCESS;
}

/**
 * @internal called established 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_called_e( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:UPDATE_CALLED_E";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, column22=? WHERE index0=?";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE_CALLED_E. (sessid=%llu, svcst=%d, state=CALLED_ESTABLISHED, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->extime);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_CALLED_E (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, tcseq=%u)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, dlgsess->tcseq);

	rv = uims_dbdataset_write( paraset, 5,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"tcseq", UIMS_DBTYPE_UINT32, dlgsess->tcseq,
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id( dlgsess->sess));

	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);

	return UX_SUCCESS;
}

/**
 * @internal ms established 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_ms_e( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	static const char* stmtid = "DLGSESS:UPDATE_MS_E";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, column26=? WHERE index0=?";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE_MS_E. (sessid=%llu, svcst=%d, state=MS_ESTABLISHED, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->extime);

	stmt = uims_db_open_stmt( dao->db, stmtid, query, &rv); 
	if( stmt == NULL) {
		ux_log(UXL_MIN, "Failed to open statement. (stmtid=%s)", stmtid);
		return UX_ERR_FAILED;
	}

	paraset = uims_dbstmt_get_paraset( stmt);
	if( paraset == NULL) {
		ux_log(UXL_MIN, "Failed to get parameter set from statement. (stmtid=%s)", stmtid);
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_MS_E (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, mscseq=%u)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, dlgsess->mscseq);

	rv = uims_dbdataset_write( paraset, 5,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"mscseq", UIMS_DBTYPE_UINT32, dlgsess->mscseq,
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id( dlgsess->sess));

	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to set parameters to statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	rv = uims_dbstmt_execute( stmt);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return UX_ERR_FAILED;
	}

	uims_dbstmt_close( stmt);

	return UX_SUCCESS;
}
