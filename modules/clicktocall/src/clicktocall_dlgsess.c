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
		case CLICKTOCALL_DLGSTATE_PROCEEDING : return "PROCEEDING";
		case CLICKTOCALL_DLGSTATE_ACCEPTED : return "ACCEPTED";
		case CLICKTOCALL_DLGSTATE_REJECTED : return "REJECTED";
		case CLICKTOCALL_DLGSTATE_ESTABLISHED : return "ESTABLISHED";
		case CLICKTOCALL_DLGSTATE_TERMINATING : return "TERMINATING";
		case CLICKTOCALL_DLGSTATE_TERMINATED : return "TERMINATED";
		default : break;
	}

	return "UNKNOWN";
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

	if( dlgsess->reqmsg ) {
		upa_sipmsg_unref( dlgsess->reqmsg);
		dlgsess->reqmsg = NULL;
	}

	allocator = uims_sess_get_allocator( dlgsess->sess);
	if( dlgsess->ouser) ux_free( allocator, dlgsess->ouser);
	if( dlgsess->ocontact) ux_free( allocator, dlgsess->ocontact);
	if( dlgsess->oroute) ux_free( allocator, dlgsess->oroute);
	if( dlgsess->tuser) ux_free( allocator, dlgsess->tuser);
	if( dlgsess->tcontact) ux_free( allocator, dlgsess->tcontact);
	if( dlgsess->troute) ux_free( allocator, dlgsess->troute);

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
	const cJSON *sessionid = NULL;
	const cJSON *subscribername = NULL;
	const cJSON *callingnumber = NULL;
	const cJSON *callednumber = NULL;
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

	sessionid = cJSON_GetObjectItemCaseSensitive(json, "sessionID");
    if (cJSON_IsString(sessionid) && (sessionid->valuestring != NULL)) {
		dlgsess->sessionid = ux_str_dup( sessionid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } else {
		ux_log(UXL_MAJ, "Fail to get sessionID JSON key");
		return UX_EINVAL;
	}

	subscribername = cJSON_GetObjectItemCaseSensitive(json, "subscriberName");
    if (cJSON_IsString(subscribername) && (subscribername->valuestring != NULL)) {
		dlgsess->subscribername = ux_str_dup( subscribername->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
		ux_log(UXL_MAJ, "Fail to get subscriberName JSON key");
		return UX_EINVAL;
	}

	callingnumber = cJSON_GetObjectItemCaseSensitive(json, "callingNumber");
    if (cJSON_IsString(callingnumber) && (callingnumber->valuestring != NULL)) {
		dlgsess->callingnumber = ux_str_dup( callingnumber->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } else {
		ux_log(UXL_MAJ, "Fail to get callingNumber JSON key");
		return UX_EINVAL;
	}

	callednumber = cJSON_GetObjectItemCaseSensitive(json, "calledNumber");
    if (cJSON_IsString(callednumber) && (callednumber->valuestring != NULL)) {
		dlgsess->callednumber = ux_str_dup( callednumber->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } else {
		ux_log(UXL_MAJ, "Fail to get calledNumber JSON key");
		return UX_EINVAL;
	}

	chargingnumber = cJSON_GetObjectItemCaseSensitive(json, "chargingNumber");
    if (cJSON_IsString(chargingnumber) && (chargingnumber->valuestring != NULL)) {
		dlgsess->chargingnumber = ux_str_dup( chargingnumber->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
		ux_log(UXL_MAJ, "Fail to get chargingNumber JSON key");
		return UX_EINVAL;
	}

	ringbacktonetype = cJSON_GetObjectItemCaseSensitive(json, "ringBackToneType");
    if (cJSON_IsNumber(ringbacktonetype)) {
		dlgsess->ringbacktonetype = ringbacktonetype->valueint;
		ux_log(UXL_MAJ, "Fail to get ringBackToneType JSON key");
		return UX_EINVAL;
	}

	watitngmentid = cJSON_GetObjectItemCaseSensitive(json, "waitingMentID");
    if (cJSON_IsString(watitngmentid) && (watitngmentid->valuestring != NULL)) {
		dlgsess->watitngmentid = ux_str_dup( watitngmentid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } else {
		ux_log(UXL_MAJ, "Fail to get waitingMentID JSON key");
		return UX_EINVAL;
	}

	callmentid = cJSON_GetObjectItemCaseSensitive(json, "callMentID");
    if (cJSON_IsString(callmentid) && (callmentid->valuestring != NULL)) {
		dlgsess->callmentid = ux_str_dup( callmentid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } else {
		ux_log(UXL_MAJ, "Fail to get callMentID JSON key");
		return UX_EINVAL;
	}

	callingcid = cJSON_GetObjectItemCaseSensitive(json, "callingCID");
    if (cJSON_IsString(callingcid) && (callingcid->valuestring != NULL)) {
		dlgsess->callingcid = ux_str_dup( callingcid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
		ux_log(UXL_MAJ, "Fail to get callingCID JSON key");
		return UX_EINVAL;
	}

	calledcid = cJSON_GetObjectItemCaseSensitive(json, "calledCID");
    if (cJSON_IsString(calledcid) && (calledcid->valuestring != NULL)) {
		dlgsess->calledcid = ux_str_dup( calledcid->valuestring, uims_sess_get_allocator(dlgsess->sess)); 
    } else {
		ux_log(UXL_MAJ, "Fail to get calledCID JSON key");
		return UX_EINVAL;
	}

	hostcode = cJSON_GetObjectItemCaseSensitive(json, "hostCode");
    if (cJSON_IsNumber(hostcode)) {
		dlgsess->hostcode = hostcode->valueint;
		ux_log(UXL_MAJ, "Fail to get hostCode JSON key");
		return UX_EINVAL;
	}

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_http_start_res( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *resmsg) 
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

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_ssw_outgoing_req( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *reqmsg)
{
	int rv;
	usip_mobj_t *req;
	upa_sippa_t *sippa;
	char tag[128];

	req = reqmsg->mobj;
	if( req->call_id == NULL || req->from == NULL || req->from == NULL || req->to == NULL || req->cseq == NULL) {
		ux_log( UXL_MAJ, "Missing mandatory header. (method=%s, call_id=%s, from=%s:%s, to=%s:%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_CALLID( req),
				USIP_MOBJ_GET_FROMUSER( req), USIP_MOBJ_GET_FROMTAG( req),
				USIP_MOBJ_GET_TOUSER( req), USIP_MOBJ_GET_TOTAG( req));
		return UX_EBADMSG;
	}

	sippa = (upa_sippa_t*)reqmsg->uxcmsg->paif;
	dlgsess->method = req->request->method;
	dlgsess->ocseq = (req->cseq) ? req->cseq->seq : 1;
	dlgsess->reqmsg = upa_sipmsg_ref( reqmsg);
	dlgsess->prevstate = dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_INIT;
	dlgsess->hasreq = USIP_TRUE;

	reqmsg->sessinfo->did = 0;
	upa_sippa_write_sessinfo( sippa, reqmsg, tag, sizeof(tag));
	dlgsess->ostag = ux_str_dup( tag, uims_sess_get_allocator(dlgsess->sess)); 

	rv = uims_sess_set_call_id( dlgsess->sess, req->call_id->id);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set call-id. (method=%s, call_id=%s, err=%d,%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_CALLID( req), rv, usip_errstr(rv));
		return rv;
	}

	rv = uims_sess_set_ltag( dlgsess->sess, req->from->tag);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set tag of From header. (method=%s, tag=%s, err=%d,%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_FROMTAG( req), rv, usip_errstr(rv));
	}

	rv = clicktocall_dlgsess_set_ouser( dlgsess, req->from);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set From header. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
		return rv;
	}

	if( req->contact) {
		rv = clicktocall_dlgsess_set_ocontact( dlgsess, req->contact);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to set Contact header. (method=%s, Contact=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
					usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->contact->uri), rv, usip_errstr(rv));
			return rv;
		}	
	}

	rv = clicktocall_dlgsess_set_tuser( dlgsess, req->to);
	if( rv < USIP_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set From URI. (method=%s, from=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
	}

	return USIP_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_load_from_initial_req( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *reqmsg)
{
	int rv;
	usip_mobj_t *req;
	upa_sippa_t *sippa;
	char tag[128];

	req = reqmsg->mobj;
	if( req->call_id == NULL || req->from == NULL || req->from->tag == NULL || req->to == NULL || req->cseq == NULL) {
		ux_log( UXL_MAJ, "Missing mandatory header. (method=%s, call_id=%s, from=%s:%s, to=%s:%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_CALLID( req),
				USIP_MOBJ_GET_FROMUSER( req), USIP_MOBJ_GET_FROMTAG( req),
				USIP_MOBJ_GET_TOUSER( req), USIP_MOBJ_GET_TOTAG( req));
		return UX_EBADMSG;
	}

	sippa = (upa_sippa_t*)reqmsg->uxcmsg->paif;
	dlgsess->method = req->request->method;
	dlgsess->ocseq = (req->cseq) ? req->cseq->seq : 1;
	dlgsess->reqmsg = upa_sipmsg_ref( reqmsg);
	dlgsess->prevstate = dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_INIT;
	dlgsess->hasreq = USIP_TRUE;

	reqmsg->sessinfo->did = 1;
	upa_sippa_write_sessinfo( sippa, reqmsg, tag, sizeof(tag));
	dlgsess->tstag = ux_str_dup( tag, uims_sess_get_allocator(dlgsess->sess)); 

	reqmsg->sessinfo->did = 0;
	upa_sippa_write_sessinfo( sippa, reqmsg, tag, sizeof(tag));
	dlgsess->ostag = ux_str_dup( tag, uims_sess_get_allocator(dlgsess->sess)); 

	rv = uims_sess_set_call_id( dlgsess->sess, req->call_id->id);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set call-id. (method=%s, call_id=%s, err=%d,%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_CALLID( req), rv, usip_errstr(rv));
		return rv;
	}

	rv = uims_sess_set_ltag( dlgsess->sess, req->from->tag);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set tag of From header. (method=%s, tag=%s, err=%d,%s)",
				usip_mobj_get_method( req), USIP_MOBJ_GET_FROMTAG( req), rv, usip_errstr(rv));
	}

	rv = clicktocall_dlgsess_set_ouser( dlgsess, req->from);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set From header. (method=%s, From=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
		return rv;
	}

	if( req->contact) {
		rv = clicktocall_dlgsess_set_ocontact( dlgsess, req->contact);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to set Contact header. (method=%s, Contact=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
					usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->contact->uri), rv, usip_errstr(rv));
			return rv;
		}	
	}

	if( req->record_route) {
		rv = clicktocall_dlgsess_set_oroute( dlgsess, req->record_route);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to set Contact URI. (method=%s, Record-Route=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
					usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->record_route->uri), rv, usip_errstr(rv));
			return rv;
		}	
	}

	rv = clicktocall_dlgsess_set_tuser( dlgsess, req->to);
	if( rv < USIP_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to set From URI. (method=%s, from=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
				usip_mobj_get_method( req), USIP_URI_PRINT_ARGS(req->from->uri), rv, usip_errstr(rv));
	}

	return USIP_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_load_from_initial_rsp( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *rspmsg)
{
	int rv;
	usip_mobj_t *rsp;

	rsp = rspmsg->mobj;
	if( rsp->to->tag && uims_sess_get_rtag( dlgsess->sess) == NULL) {
		rv = uims_sess_set_rtag( dlgsess->sess, rsp->to->tag);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to set tag of To header. (method=%s, tag=%s, err=%d,%s)",
					usip_mobj_get_method( rsp), USIP_MOBJ_GET_TOTAG( rsp), rv, usip_errstr(rv));
			return rv;
		}
	}
	if( rsp->contact && dlgsess->tcontact == NULL) {
		rv = clicktocall_dlgsess_set_tcontact( dlgsess, rsp->contact);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to set Contact header. (method=%s, Contact=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
					usip_mobj_get_method( rsp), USIP_URI_PRINT_ARGS(rsp->contact->uri), rv, usip_errstr(rv));
			return rv;
		}	
	}
	if( rsp->record_route && dlgsess->troute) {
		rv = clicktocall_dlgsess_set_troute( dlgsess, rsp->record_route);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to set Contact URI. (method=%s, Record-Route=" USIP_URI_PRINT_FORMAT ", err=%d,%s)",
					usip_mobj_get_method( rsp), USIP_URI_PRINT_ARGS(rsp->record_route->uri), rv, usip_errstr(rv));
			return rv;
		}	
	}

	return USIP_SUCCESS;
}

UX_DECLARE(ux_mem_t*) clicktocall_dlgsess_get_allocator( clicktocall_dlgsess_t *dlgsess)
{
	return uims_sess_get_allocator( dlgsess->sess);
}

/**
 * @brief 해당 호 발신 user를 반환한다.
 * @param dlgsess dlg session
 * @return 호 발신 user
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_ouser( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->ouser;
}

/**
 * @brief 해당 호 발신 user를 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 발신 user
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ouser( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *ouser;

	ouser = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( ouser == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator nameaddr.");
		return UX_ENOMEM;
	}

	if( dlgsess->ouser) usip_hdr_destroy( (usip_hdr_t*)dlgsess->ouser);
	dlgsess->ouser = ouser;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 발신자 contact 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 contact 정보
 */
UX_DECLARE(usip_contact_hdr_t*) clicktocall_dlgsess_get_ocontact( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->ocontact;
}

/**
 * @brief 해당 세션 발신자 contact 정보를 설정한다. 
 * @param dlgsess dlg session
 * @param value 발신자 contact 정보
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ocontact( clicktocall_dlgsess_t *dlgsess, usip_contact_hdr_t *value)
{
	usip_contact_hdr_t *ocontact;

	ocontact = (usip_contact_hdr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( ocontact == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator contact.");
		return UX_ENOMEM;
	}

	if( dlgsess->ocontact) usip_hdr_destroy( (usip_hdr_t*)dlgsess->ocontact);
	dlgsess->ocontact = ocontact;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 발신자 route 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 route 정보
 */
UX_DECLARE(usip_route_hdr_t*) clicktocall_dlgsess_get_oroute( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->oroute;
}

/**
 * @brief 해당 세션 발신자 route 정보를 설정한다. 
 * @param dlgsess dlg session
 * @param value 발신자 route 정보
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_oroute( clicktocall_dlgsess_t *dlgsess, usip_route_hdr_t *value)
{
	usip_route_hdr_t *oroute;

	oroute = usip_route_fixdup( value, uims_sess_get_allocator(dlgsess->sess));
	if( oroute == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator route.");
		return UX_ENOMEM;
	}

	if( dlgsess->oroute) usip_hdr_destroy( (usip_hdr_t*)dlgsess->oroute);
	dlgsess->oroute = oroute;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 호 착신 user를 반환한다.
 * @param dlgsess dlg session
 * @return 호 착신 user
 */
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_tuser( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->tuser;
}

/**
 * @brief 해당 호 착신 user를 설정한다. 
 * @param dlgsess dlg session
 * @param value 호 착신 user
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tuser( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value)
{
	usip_nameaddr_t *tuser;

	tuser = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( tuser == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate terminator name addr.");
		return UX_ENOMEM;
	}

	if( dlgsess->tuser) usip_hdr_destroy( (usip_hdr_t*)dlgsess->tuser);
	dlgsess->tuser = tuser;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 착신자 contact 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 착신자 contact 정보
 */
UX_DECLARE(usip_contact_hdr_t*) clicktocall_dlgsess_get_tcontact( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->tcontact;
}

/**
 * @brief 해당 세션 착신자 contact 정보를 설정한다. 
 * @param dlgsess dlg session
 * @param value 착신자 contact 정보
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tcontact( clicktocall_dlgsess_t *dlgsess, usip_contact_hdr_t *value)
{
	usip_contact_hdr_t *tcontact;

	tcontact = (usip_contact_hdr_t*)usip_hdr_duplicate( (usip_hdr_t*)value, uims_sess_get_allocator(dlgsess->sess));
	if( tcontact == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate terminator contact.");
		return UX_ENOMEM;
	}

	if( dlgsess->tcontact) usip_hdr_destroy( (usip_hdr_t*)dlgsess->tcontact);
	dlgsess->tcontact = tcontact;

	return USIP_SUCCESS;
}

/**
 * @brief 해당 세션 착신자 route 정보를 반환한다.
 * @param dlgsess dlg session
 * @return 발신자 route 정보
 */
UX_DECLARE(usip_route_hdr_t*) clicktocall_dlgsess_get_troute( clicktocall_dlgsess_t *dlgsess)
{
	return dlgsess->troute;
}

/**
 * @brief 해당 세션 착신자 route 정보를 설정한다. 
 * @param dlgsess dlg session
 * @param value 착신자 route 정보
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_troute( clicktocall_dlgsess_t *dlgsess, usip_route_hdr_t *value)
{
	const char *lr;
	ux_mem_t *allocator;
	usip_route_hdr_t *dst, *troute;

	allocator = uims_sess_get_allocator(dlgsess->sess);
	for( troute = NULL; value; value = (usip_route_hdr_t*)value->base->next ) {
		if( dlgsess->ostag) {
			if( usip_str_cmp(value->uri->user, dlgsess->ostag) == 0) break;
		} else {
			if( usip_str_cmp(value->uri->host, "192.168.1.182") == 0 &&
					usip_str_cmp(value->uri->port, "15064") == 0) {
				break;
			}
		}

		dst = (usip_route_hdr_t*)usip_hdr_create_c(allocator, usip_route_hdef(), value->base);
		if( dst == NULL) {
			ux_log(UXL_CRT, "Failed to creae terminator Route header");
			return UX_ENOMEM;
		}
		// Fix broken (Record-)Routes without <>
		if( dst->uri->params == NULL && dst->params) {
			lr = usip_params_get_value( dst->params, "lr");
			if(lr) {
				dst->uri->params = usip_str_fmtdup( allocator, "lr%s%s", lr[0] ? "=" : "", lr);
				usip_params_remove_value( dst->params, "lr", allocator);
			}
		}
		dst->base->next = (usip_hdr_t *)troute;
		troute = dst;
	}

	if( dlgsess->troute) usip_hdr_destroy( (usip_hdr_t*)dlgsess->troute);
	dlgsess->troute = troute;

	return USIP_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_initial_req( clicktocall_dlgsess_t *dlgsess,
						uims_dbmgr_t *dbmgr, upa_sipmsg_t *reqmsg)
{
	/*
	int rv;
	usip_mobj_t *mobj;
	clicktocall_peer_t *tpeer;

	mobj = reqmsg->mobj;
	if( mobj->request == NULL) {
		dlgsess->error = CLICKTOCALL_ERR_BAD_REQUEST;
		return UX_EINVAL;
	}
	if( mobj->request->method != USIP_METHOD_INVITE && mobj->request->method != USIP_METHOD_SUBSCRIBE) {
		dlgsess->error = CLICKTOCALL_ERR_BAD_REQUEST;
		return UX_EINVAL;
	}

	rv = clicktocall_dlgsess_load_from_initial_req( dlgsess, reqmsg);
	if( rv < UX_SUCCESS) {
		dlgsess->error = CLICKTOCALL_ERR_BAD_REQUEST;
		return UX_EINVAL;
	}

	if( mobj->route && mobj->route->base->next) {
		//만약 Route 헤더가 자신 헤더 이외에 더 있다면 ROUTER mode로 동작하도록 한다.
		dlgsess->rtpolicy->opeer_id = mobj->peer_id;
		dlgsess->rtpolicy->tpeer_id = -1;
		dlgsess->rtpolicy->svcmode = CLICKTOCALL_SVCMODE_ROUTER;
		dlgsess->rtpolicy->way = CLICKTOCALL_SVCWAY_INTERNAL_TO_INTERNAL;
		dlgsess->rtpolicy->opt = 0;
		dlgsess->tpeer = NULL;
	} else {
		rv = clicktocall_router_get_policy( router, reqmsg, dlgsess->rtpolicy);
		if( rv < UX_SUCCESS) {
			dlgsess->error = CLICKTOCALL_ERR_BAD_REQUEST;
			return rv;
		}

		tpeer = clicktocall_router_get_peer( router, dlgsess->rtpolicy->tpeer_id);
		if( tpeer) {
			dlgsess->tpeer = ux_str_fmtdup( uims_sess_get_allocator( dlgsess->sess),
					"sip:%s:%u", tpeer->host, tpeer->port);
		} else {
			dlgsess->tpeer = NULL;
		}
	}
	dlgsess->error = CLICKTOCALL_ERR_SUCCESS;
*/
	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_forward_initial_req( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *fwdmsg)
{
	/*
	int rv, priv_type, thig_flag;
	upa_sippa_t *sippa;
	clicktocall_plugin_t *plugin;

	plugin = clicktocall_plugin_instance();
	thig_flag = 0;

	if( dlgsess->tpeer == NULL && dlgsess->rtpolicy->svcmode != CLICKTOCALL_SVCMODE_ROUTER) {
		ux_log(UXL_MAJ, "Selected target peer URI doesn't exist.");
		dlgsess->error = CLICKTOCALL_ERR_NO_SCSCF; 
		return UX_EINVAL;
	}
	if( dlgsess->reqmsg == NULL) {
		ux_log(UXL_MAJ, "No request message.");
		dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC; 
		return UX_EINVAL;
	}

	sippa = (upa_sippa_t*)fwdmsg->uxcmsg->paif;
	rv = usip_mobj_copy( fwdmsg->mobj, dlgsess->reqmsg->mobj);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to copy SIP message. (err=%d,%s)", rv, usip_errstr(rv));
		dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
		return rv;
	}

	//Max-Forwards 헤더 값 감소
	if( fwdmsg->mobj->max_forwards) --fwdmsg->mobj->max_forwards->value;
	//첫번 째 Route 헤더 삭제 - CLICKTOCALL 자신의 주소
	if( fwdmsg->mobj->route) usip_msg_remove_hdr( fwdmsg->mobj->base->msg, fwdmsg->mobj->route->base);

	if( dlgsess->rtpolicy->svcmode == CLICKTOCALL_SVCMODE_B2BUA) {
		//Via thig 적용
		if( plugin->conf->use_thig) {
			rv = clicktocall_thig_encrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_VIA);
			if( rv < USIP_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to adjust THIG Via header on SIP message. (target=%s, err=%d,%s)",
						dlgsess->tpeer, rv, usip_errstr(rv));
				dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
				return rv;
			}
			thig_flag += rv;
		}
		
		//Route 관련 정보 모두 삭제
		if(fwdmsg->mobj->route) usip_msg_remove_all_hdr_d( fwdmsg->mobj->base->msg, usip_route_hdef()); 
		if(fwdmsg->mobj->record_route) usip_msg_remove_all_hdr_d( fwdmsg->mobj->base->msg, usip_record_route_hdef());
		//만약 Contact이 GRUU가 아닐 경우 change Contact (contact@ clicktocall 주소)
		if( fwdmsg->mobj->contact) {
			fwdmsg->mobj->contact->uri->host = ux_str_dup("192.168.1.182", usip_mobj_get_allocator(fwdmsg->mobj));
			fwdmsg->mobj->contact->uri->port = ux_str_dup("15064", usip_mobj_get_allocator(fwdmsg->mobj));
		}
		
		//착신지가 외부 peer일 경우 동작 수행
		//From tag 값 terminator session tag로 변경
		rv = usip_nameaddr_set_param( fwdmsg->mobj->from, "tag", dlgsess->tstag);
		if( rv < UX_SUCCESS) return rv;

		// Privacy protection ( RFC3323/RFC3325)
		priv_type = clicktocall_util_check_privacy( plugin->conf, fwdmsg->mobj);
		if( priv_type == CLICKTOCALL_PRIVACY_ID) {
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "P-Asserted-Identity"); 
		} else if( priv_type == CLICKTOCALL_PRIVACY_HEADER && thig_flag == 0) {
			// TODO : Remove all Via header.
			// incoming transaction 을 찾을 수 있는 값을 fwdmsg 에 설정하고 응답에 존재해야 한다.
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "Subject"); 
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "Call-Info"); 
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "User-Agent"); 
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "Reply-To"); 
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "In-Reply-To"); 
			usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "Organization"); 
			// History-Info privacy 적용(RFC7044)
		}

	} else if( dlgsess->rtpolicy->svcmode == CLICKTOCALL_SVCMODE_PROXY) {	// External->Internal
		//만약 thig 적용 시 Via, Record-Route THIG decryption 한다.
		if( plugin->conf->use_thig) {
			// Route header decryption
			rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_ROUTE);
			if( rv < USIP_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to adjust decrypted Route header on SIP message. (from=%s, err=%d,%s)",
						dlgsess->tpeer, rv, usip_errstr(rv));
				dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
				return rv;
			}
			// Record-Route header decryption
			rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_RECROUTE);
			if( rv < USIP_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to adjust decrypted Record-Route header on SIP message. (from=%s, err=%d,%s)",
						dlgsess->tpeer, rv, usip_errstr(rv));
				dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
				return rv;
			}
		}

		//add Record-Route with localtag@clicktocall address 
		rv = usip_msg_insert_fmtval( fwdmsg->mobj->base->msg, usip_record_route_hdef(), 0, NULL, "<sip:%s@%s:%d;lr>",
				dlgsess->ostag, "192.168.1.182", 15064);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set route address into SIP message. (taget=%s, err=%d,%s)",
					dlgsess->tpeer, rv, usip_errstr(rv));
			dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
			return rv;
		}
	} else {
		//Do nothing
		// TODO : 임시. add Record-Route with clicktocall address 
		rv = usip_msg_insert_fmtval( fwdmsg->mobj->base->msg, usip_record_route_hdef(), 0, NULL, "<sip:%s@%s:%d;lr>",
				dlgsess->ostag, "192.168.1.182", 15064);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set route address into SIP message. (taget=%s, err=%d,%s)",
					dlgsess->tpeer, rv, usip_errstr(rv));
			dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
			return rv;
		}
	}

	//다음 착신지 설정
	if( dlgsess->tpeer) {
		rv = usip_msg_insert_fmtval( fwdmsg->mobj->base->msg, usip_route_hdef(), 0, NULL, "<%s;lr>", dlgsess->tpeer);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set route address into SIP message. (taget=%s, err=%d,%s)",
					dlgsess->tpeer, rv, usip_errstr(rv));
			dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
			return rv;
		}
	}

	//domain name을 포함하는 P-Prviate-Network-Indication 헤더 필드 삭제

	//외부 전송시 P-Charging-Vector 처리 
	if( CLICKTOCALL_SVCWAY_IS_TO_EXTERNAL(dlgsess->rtpolicy->way)) {
		//정책에 따라
		usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "P-Charging-Vector"); 
		//store P-Charging-Function-Address	
		usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "P-Charging-Function-Addresses"); 
	}
	usip_msg_remove_all_hdr_n( fwdmsg->mobj->base->msg, "P-Profile-Key"); 
	
	//필요 시 Session Refreshment를 위한 Session-Expire 값들을 설정

	fwdmsg->sessinfo->flags |= UPA_SIPSESSINFO_PROXY_MODE;

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_initial_rsp( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *rspmsg, int *status)
{
	int rv;
	char *tpeer, uristr[1024];

	//TODO P-Charing-Vector가 있고 term-ioi가 있으면 해당 값을 세션에 저장

	//TODO P-Charging-Vector handling

	if( rspmsg->mobj->status->code == 305) {
		*status = 403; //Forbidden
		if( rspmsg->mobj->contact == NULL) return UX_SUCCESS;
		rv = usip_uri_encode( rspmsg->mobj->contact->uri, uristr, sizeof(uristr), 0);
		if( rv < UX_SUCCESS) return UX_SUCCESS;
		if( dlgsess->tpeer && strcasecmp(uristr, dlgsess->tpeer) == 0) return UX_SUCCESS; 
		tpeer = ux_str_dup( uristr, uims_sess_get_allocator(dlgsess->sess));
		if( tpeer == NULL ) return UX_SUCCESS; 
		dlgsess->tpeer = tpeer;
		dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_PROCEEDING;
		*status = 305;
		return UX_SUCCESS;
	}

	*status = rspmsg->mobj->status->code;
	if( rspmsg->mobj->status->code > 100) {
		rv = clicktocall_dlgsess_load_from_initial_rsp( dlgsess, rspmsg);
		if( rv < UX_SUCCESS) {
			dlgsess->error = CLICKTOCALL_ERR_BAD_REQUEST;
			return rv;
		}
		if( rspmsg->mobj->status->code < 200) {
			dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_PROCEEDING;
		} else {
			if( rspmsg->mobj->status->code < 300) {
				dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_ACCEPTED;
			} else {
				dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_REJECTED;
			}
			if( dlgsess->reqmsg ) {
				upa_sipmsg_unref( dlgsess->reqmsg);
				dlgsess->reqmsg = NULL;
			}
		}
	}
	*/

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_forward_initial_rsp( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *rspmsg, upa_sipmsg_t *fwdmsg)
{
	/*
	int rv;
	upa_sippa_t *sippa;
	clicktocall_plugin_t *plugin;

	plugin = clicktocall_plugin_instance();

	sippa = (upa_sippa_t*)fwdmsg->uxcmsg->paif;

	rv = usip_mobj_copy( fwdmsg->mobj, rspmsg->mobj);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to copy SIP message. (err=%d,%s)", rv, usip_errstr(rv));
		dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
		return rv;
	}
	usip_msg_remove_hdr_d( fwdmsg->mobj->base->msg, usip_via_hdef(), 0);

	if( dlgsess->rtpolicy->svcmode == CLICKTOCALL_SVCMODE_B2BUA) {
		rv = usip_nameaddr_set_param( fwdmsg->mobj->from, "tag", uims_sess_get_ltag( dlgsess->sess));
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set tag of From header. (err=%d,%s)", rv, usip_errstr(rv));
			dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
			return rv;
		}
		if( fwdmsg->mobj->to->tag) {
			rv = usip_nameaddr_set_param( fwdmsg->mobj->to, "tag", dlgsess->ostag);
			if( rv < UX_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to set tag of To header. (err=%d,%s)", rv, usip_errstr(rv));
				return rv;
			}
		}
		if( dlgsess->oroute) {
			rv = uims_sipmsg_set_record_route( fwdmsg->mobj, dlgsess->oroute);
			if( rv < UX_SUCCESS) { 
				ux_log(UXL_MAJ, "Failed to set Record-Route header. (err=%d,%s)", rv, usip_errstr(rv));
				return rv;
			}
		}
		if( fwdmsg->mobj->contact) {
			fwdmsg->mobj->contact->uri->host = ux_str_dup("192.168.1.182", usip_mobj_get_allocator(fwdmsg->mobj));
			fwdmsg->mobj->contact->uri->port = ux_str_dup("15064", usip_mobj_get_allocator(fwdmsg->mobj));
		}
	
		//Via thig 해제
		if( plugin->conf->use_thig) {
			rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_VIA);
			if( rv < USIP_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to adjust decrypted Via header on SIP message. (from=%s, err=%d,%s)",
						dlgsess->tpeer, rv, usip_errstr(rv));
				dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
				return rv;
			}
		}
		
	} else if( dlgsess->rtpolicy->svcmode == CLICKTOCALL_SVCMODE_PROXY) {
		if( plugin->conf->use_thig) {
			// Route thig 적용
			rv = clicktocall_thig_encrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_ROUTE);
			if( rv < USIP_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to adjust THIG Route header on SIP message. (target=%s, err=%d,%s)",
						dlgsess->tpeer, rv, usip_errstr(rv));
				dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
				return rv;
			}
			// Record-Route thig 적용
			rv = clicktocall_thig_encrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_RECROUTE);
			if( rv < USIP_SUCCESS) {
				ux_log(UXL_MAJ, "Failed to adjust THIG Record-Route header on SIP message. (target=%s, err=%d,%s)",
						dlgsess->tpeer, rv, usip_errstr(rv));
				dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
				return rv;
			}
		}
	} else {
		;
	}
	*/

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_notify_req( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *reqmsg, int *is_terminated)
{
	if( reqmsg->mobj->subs_state && reqmsg->mobj->subs_state->state) {
		*is_terminated = (strcasecmp(reqmsg->mobj->subs_state->state, "terminated") == 0);
		if( *is_terminated) {
			dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_TERMINATING;
		}
	} else {
		*is_terminated = UX_FALSE;
	}

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_forward_msg( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *srcmsg, upa_sipmsg_t *fwdmsg)
{
	/*
	int rv;
	const char *otag, *ttag;
	ux_mem_t *allocator;
	upa_sippa_t *sippa;
	clicktocall_plugin_t *plugin;

	plugin = clicktocall_plugin_instance();

	sippa = (upa_sippa_t*)srcmsg->uxcmsg->paif;
	rv = usip_mobj_copy( fwdmsg->mobj, srcmsg->mobj);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to copy SIP message. (err=%d,%s)", rv, usip_errstr(rv));
		dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
		return rv;
	}

	allocator = usip_mobj_get_allocator(fwdmsg->mobj);
	//TODO ocseq, tcseq update

	if( dlgsess->rtpolicy->svcmode == CLICKTOCALL_SVCMODE_B2BUA) {
		if( srcmsg->mobj->request ) {
			if(srcmsg->mobj->request->method == USIP_METHOD_BYE) {
				dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_TERMINATING;
			}
			dlgsess->hasreq = USIP_TRUE;
			otag = uims_sess_get_ltag( dlgsess->sess);
			usip_msg_remove_all_hdr_d( fwdmsg->mobj->base->msg, usip_route_hdef()); 
			usip_msg_remove_all_hdr_d( fwdmsg->mobj->base->msg, usip_record_route_hdef());
	
			if( fwdmsg->mobj->contact) {
				//external이냐 internal이냐에 따라 다른 정보 설정
				fwdmsg->mobj->contact->uri->host = ux_str_dup("192.168.1.182", allocator);
				fwdmsg->mobj->contact->uri->port = ux_str_dup("15064", allocator);
			}

			if( srcmsg->sessinfo->did == 0) { // Internal -> External Request
				if( srcmsg->mobj->cseq && srcmsg->mobj->cseq->method != USIP_METHOD_ACK) {
					dlgsess->ocseq = srcmsg->mobj->cseq->seq;
				}
				if( dlgsess->tcontact) {
					rv = usip_uri_copy( fwdmsg->mobj->request->uri, dlgsess->tcontact->uri, allocator); 
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to set Request-URI. (err=%d,%s)", rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
				rv = usip_nameaddr_set_param( fwdmsg->mobj->from, "tag", dlgsess->tstag);
				if( rv < USIP_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of From header. (err=%d,%s)", rv, usip_errstr(rv));
					dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
					return rv;
				}
				rv = usip_nameaddr_set_param( fwdmsg->mobj->to, "tag", uims_sess_get_rtag(dlgsess->sess));
				if( rv < UX_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of To header. (err=%d,%s)", rv, usip_errstr(rv));
					dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
					return rv;
				}
				if( dlgsess->troute) {
					rv = usip_msg_add_hdr( fwdmsg->mobj->base->msg, (usip_hdr_t*)dlgsess->troute, USIP_HFLAG_APPEND);
					if( rv < UX_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to set Route header. (err=%d,%s)", rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
				// external 전송 시 Via에 thig 적용
				if( plugin->conf->use_thig) {
					rv = clicktocall_thig_encrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_VIA);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust THIG Via header on SIP message. (target=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			} else {	// External -> Internal Request
				if( srcmsg->mobj->cseq && srcmsg->mobj->cseq->method != USIP_METHOD_ACK) {
					dlgsess->tcseq = srcmsg->mobj->cseq->seq;
				}
				if( dlgsess->ocontact) {
					rv = usip_uri_copy( fwdmsg->mobj->request->uri, dlgsess->ocontact->uri, allocator); 
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to set Request-URI. (err=%d,%s)", rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
				rv = usip_nameaddr_set_param( fwdmsg->mobj->from, "tag", dlgsess->ostag);
				if( rv < USIP_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of From header. (err=%d,%s)", rv, usip_errstr(rv));
					dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
					return rv;
				}
				rv = usip_nameaddr_set_param( fwdmsg->mobj->to, "tag", uims_sess_get_ltag(dlgsess->sess));
				if( rv < UX_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of To header. (err=%d,%s)", rv, usip_errstr(rv));
					dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
					return rv;
				}
				if( dlgsess->oroute) {
					rv = usip_msg_add_hdr( fwdmsg->mobj->base->msg, (usip_hdr_t*)dlgsess->oroute, USIP_HFLAG_APPEND);
					if( rv < UX_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to set Route header. (err=%d,%s)", rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			}
		} else {
			ttag = uims_sess_get_rtag( dlgsess->sess);
			usip_msg_remove_hdr_d( fwdmsg->mobj->base->msg, usip_via_hdef(), 0);
			usip_msg_remove_all_hdr_d( fwdmsg->mobj->base->msg, usip_record_route_hdef());
			if( fwdmsg->mobj->contact) {
				//external이냐 internal이냐에 따라 다른 정보 설정
				fwdmsg->mobj->contact->uri->host = ux_str_dup("192.168.1.182", allocator);
				fwdmsg->mobj->contact->uri->port = ux_str_dup("15064", allocator);
			}

			if( srcmsg->sessinfo->did == 0 ) {	// Internal -> External Response
				rv = usip_nameaddr_set_param( fwdmsg->mobj->from, "tag", uims_sess_get_rtag( dlgsess->sess));
				if( rv < USIP_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of From header. (err=%d,%s)", rv, usip_errstr(rv));
					dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
					return rv;
				}
				rv = usip_nameaddr_set_param( fwdmsg->mobj->to, "tag", dlgsess->tstag);
				if( rv < UX_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of To header. (err=%d,%s)", rv, usip_errstr(rv));
					return rv;
				}
				if( srcmsg->mobj->record_route) {
					//Record-Route가 있다면 oroute를 record-route에 설정 ?
				}
			} else {	// External -> Internal Response
				rv = usip_nameaddr_set_param( fwdmsg->mobj->from, "tag", uims_sess_get_ltag( dlgsess->sess));
				if( rv < USIP_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of From header. (err=%d,%s)", rv, usip_errstr(rv));
					dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
					return rv;
				}
				rv = usip_nameaddr_set_param( fwdmsg->mobj->to, "tag", dlgsess->ostag);
				if( rv < UX_SUCCESS) {
					ux_log(UXL_MAJ, "Failed to set tag of To header. (err=%d,%s)", rv, usip_errstr(rv));
					return rv;
				}
				if( srcmsg->mobj->record_route) {
					//Record-Route가 있다면 oroute를 record-route에 설정 ?
				}
				// Via thig 해제
				if( plugin->conf->use_thig) {
					rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_VIA);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust decrypted Via header on SIP message. (from=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			}
		}
	} else if( dlgsess->rtpolicy->svcmode == CLICKTOCALL_SVCMODE_PROXY) {
		if( srcmsg->mobj->request ) {
			if(srcmsg->mobj->request->method == USIP_METHOD_BYE) {
				dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_TERMINATING;
			}
			dlgsess->hasreq = USIP_TRUE;
			otag = uims_sess_get_ltag( dlgsess->sess);
			usip_msg_remove_hdr_d( fwdmsg->mobj->base->msg, usip_route_hdef(), 0); 
			if( srcmsg->mobj->cseq && srcmsg->mobj->cseq->method != USIP_METHOD_ACK) {
				if( usip_str_cmp( srcmsg->mobj->from->tag, otag) == 0 &&
					usip_uri_compare( srcmsg->mobj->from->uri, dlgsess->ouser->uri) == 0)
				{
					dlgsess->ocseq = srcmsg->mobj->cseq->seq;
				} else {
					dlgsess->tcseq = srcmsg->mobj->cseq->seq;
				}
			}
			
			// External -> Internal Request
			if( usip_str_cmp( srcmsg->mobj->from->tag, otag) == 0) {
				if( plugin->conf->use_thig) {
					// Route thig 해제
					rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_ROUTE);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust decrypted Route header on SIP message. (from=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
					// Record-Route thig 해제
					rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_RECROUTE);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust decrypted Record-Route header on SIP message. (from=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			} else {	// Internal -> External Request
				if( plugin->conf->use_thig) { // Via thig 적용
					rv = clicktocall_thig_encrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_VIA);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust THIG Via header on SIP message. (target=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			}
		} else {
			ttag = uims_sess_get_rtag( dlgsess->sess);
			usip_msg_remove_hdr_d( fwdmsg->mobj->base->msg, usip_via_hdef(), 0);
			// Internal -> External Response
			if( usip_str_cmp( srcmsg->mobj->to->tag, ttag) == 0) {
				if( plugin->conf->use_thig) {
					// Record-Route thig 적용
					rv = clicktocall_thig_encrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_RECROUTE);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust THIG Record-Route header on SIP message. (target=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			} else {	// External -> Internal Response
				//Via header thig 해제
				if( plugin->conf->use_thig) {
					rv = clicktocall_thig_decrypt_header( plugin->thid, fwdmsg, CLICKTOCALL_THIG_HDR_VIA);
					if( rv < USIP_SUCCESS) {
						ux_log(UXL_MAJ, "Failed to adjust decrypted Via header on SIP message. (from=%s, err=%d,%s)",
								dlgsess->tpeer, rv, usip_errstr(rv));
						dlgsess->error = CLICKTOCALL_ERR_SERVICE_LOGIC;
						return rv;
					}
				}
			}
		}
	} else {
		if( srcmsg->mobj->request ) {
			if(srcmsg->mobj->request->method == USIP_METHOD_BYE) {
				dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_TERMINATING;
			}
			// TODO : for TEST
			if( fwdmsg->mobj->route)
				usip_msg_remove_hdr_d( fwdmsg->mobj->base->msg, usip_route_hdef(), 0); 
		} else {
			// TODO : for TEST
			usip_msg_remove_hdr_d( fwdmsg->mobj->base->msg, usip_via_hdef(), 0);
		}
	}
	*/

	return UX_SUCCESS;
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
ux_status_t clicktocall_dlgdao_init( clicktocall_dlgdao_t *dao, uims_dbmgr_t *dbmgr)
{
	dao->dbmgr = dbmgr;
	dao->db = uims_dbmgr_get_n( dbmgr, "TNTDB");
	if(dao->db == NULL) {
		ux_log(UXL_MAJ, "Failed to get TNTDB");
		return UX_ENOENT;
	}

	return UX_SUCCESS;
}

void clicktocall_dlgdao_final( clicktocall_dlgdao_t *dao)
{
	//Do Nonthing
}

clicktocall_dlgsess_t* clicktocall_dlgdao_find( clicktocall_dlgdao_t *dao,
						uims_sessmgr_t *sessmgr, uims_sessid_t sessid)
{
	/*
	static const char* stmtid = "DLGSESS:FIND";
	static const char* query = "SELECT * FROM 1 WHERE 0=?";

	int rv;
	uint8_t dlgstate, method, svcmode, way;
	uint16_t state;
	uint32_t ocseq, tcseq, opt;
	int32_t opeer_id, tpeer_id;
	uint64_t rsessid, extime;
	char *call_id, *ouser, *otag, *ocontact, *oroute;
	char *tuser, *ttag, *tcontact, *troute, *tpeer;
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

	rv = uims_dbdataset_read( rsltset, 22,
			//name, type, value, [length:octet only]
			"sess_id", UIMS_DBTYPE_UINT64, &rsessid,
			"extime", UIMS_DBTYPE_UINT64, &extime,
			"state", UIMS_DBTYPE_UINT16, &state,
			"dlgstate", UIMS_DBTYPE_UINT8, &dlgstate,
			"ocseq", UIMS_DBTYPE_UINT32, &ocseq,
			"tcseq", UIMS_DBTYPE_UINT32, &tcseq,
			"method", UIMS_DBTYPE_UINT8, &method,
			"svcmode", UIMS_DBTYPE_UINT8, &svcmode,
			"way", UIMS_DBTYPE_UINT8, &way,
			"opt", UIMS_DBTYPE_UINT32, &opt,
			"opeer_id", UIMS_DBTYPE_INT32, &opeer_id,
			"tpeer_id", UIMS_DBTYPE_INT32, &tpeer_id,
			"call_id", UIMS_DBTYPE_STR, &call_id,
			"ouser", UIMS_DBTYPE_STR, &ouser,
			"otag", UIMS_DBTYPE_STR, &otag,
			"ocontact", UIMS_DBTYPE_STR, &ocontact,
			"oroute", UIMS_DBTYPE_STR, &oroute,
			"tuser", UIMS_DBTYPE_STR, &tuser,
			"ttag", UIMS_DBTYPE_STR, &ttag,
			"tcontact", UIMS_DBTYPE_STR, &tcontact,
			"troute", UIMS_DBTYPE_STR, &troute,
			"tpeer", UIMS_DBTYPE_STR, &tpeer);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to read value from result set. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	ux_log(UXL_INFO, "[DLGSESS:FIND] (sessid=%llu, state=%d, dlgstate=%d, extime=%llu, ocseq=%u, tcseq=%u, method=%u, "
			"svcmode=%d, way=%d, opt=%u, opeer_id=%d, tpeer_id=%d, call_id=%s, ouser=%s, otag=%s, ocontact=%s, "
			"oroute=%s, tuser=%s, ttag=%s, tcontact=%s, troute=%s, tpeer=%s)",
			(unsigned long long)rsessid, state, dlgstate, (unsigned long long)extime, ocseq, tcseq, method,
			svcmode, way, opt, opeer_id,
			tpeer_id, call_id ? call_id : "NULL", ouser ? ouser : "NULL", otag ? otag : "NULL", ocontact ? ocontact : "NULL",
			oroute ? oroute : "NULL", tuser ? tuser : "NULL", ttag ? ttag : "NULL", tcontact ? tcontact : "NULL",
			troute ? troute : "NULL", tpeer ? tpeer : "NULL");

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
	dlgsess->dlgstate = dlgstate;
	dlgsess->extime = extime;
	dlgsess->ocseq = ocseq;
	dlgsess->tcseq = tcseq;
	dlgsess->method = method;
	dlgsess->rtpolicy->svcmode = svcmode;
	dlgsess->rtpolicy->way = way;
	dlgsess->rtpolicy->opt = opt;
	dlgsess->rtpolicy->opeer_id = opeer_id;
	dlgsess->rtpolicy->tpeer_id = tpeer_id;
	
	uims_sess_set_call_id( sess, call_id);
	uims_sess_set_ltag( sess, otag);
	uims_sess_set_rtag( sess, ttag);
	dlgsess->tpeer = ux_str_dup( tpeer,  allocator);

	if( ouser && ouser[0]) {
		dlgsess->ouser = (usip_from_hdr_t*)usip_hdr_create_v(
								allocator, usip_from_hdef(), ouser);
	} else {
		dlgsess->ouser = NULL;
	}
	if( ocontact && ocontact[0]) {
		dlgsess->ocontact = (usip_contact_hdr_t*)usip_hdr_create_v(
								allocator, usip_contact_hdef(), ocontact);
	} else {
		dlgsess->ocontact = NULL;
	}
	if( oroute && oroute[0]) {
		dlgsess->oroute = (usip_route_hdr_t*)usip_hdr_create_v(
								allocator, usip_route_hdef(), oroute);
	} else {
		dlgsess->oroute = NULL;
	}
	if( tuser && tuser[0]) {
		dlgsess->tuser = (usip_from_hdr_t*)usip_hdr_create_v(
								allocator, usip_from_hdef(), tuser);
	} else {
		dlgsess->tuser = NULL;
	}
	if( tcontact && tcontact[0]) {
		dlgsess->tcontact = (usip_contact_hdr_t*)usip_hdr_create_v(
								allocator, usip_contact_hdef(), tcontact);
	} else {
		dlgsess->tcontact = NULL;
	}
	if( troute && troute[0]) {
		dlgsess->troute = (usip_route_hdr_t*)usip_hdr_create_v(
								allocator, usip_route_hdef(), troute);
	} else {
		dlgsess->troute = NULL;
	}

	sippa = dao->routermgr->conf->sippa;
	sipmsg->sessinfo->sessid = sessid; 
	sipmsg->sessinfo->did = 1;
	upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
	dlgsess->tstag = ux_str_dup( tag, allocator); 

	sipmsg->sessinfo->did = 0;
	upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
	dlgsess->ostag = ux_str_dup( tag, allocator); 

	uims_dbstmt_close( stmt);
	return dlgsess;
	*/
	return NULL;
}

ux_status_t clicktocall_dlgdao_insert( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	/*
	static const char* stmtid = "DLGSESS:INSERT";
	static const char* query = "INSERT INTO table1 VALUES(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?)";

	int rv, bufsize, buflen, idx;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;
	usip_route_hdr_t *rthdr;
	char *ouser, *ocontact, *oroute, *tuser, *route;
	char *pbuffer, buffer[8192];

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);
	dlgsess->extime = time(NULL);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS INSERT. (sessid=%llu, svcst=%d, extime=%lu)",
			(unsigned long long)uims_sess_get_id(dlgsess->sess), sesshdr->state, dlgsess->extime);

	bufsize = sizeof(buffer); 
	pbuffer = buffer; 
	do {
		buflen = 0;
		ouser = pbuffer+buflen; 
		if( dlgsess->ouser) {
			rv = usip_nameaddr_encode( dlgsess->ouser, ouser, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) ouser[0] = '\0';
		}
		buflen++;

		tuser = pbuffer+buflen; 
		if( dlgsess->tuser) {
			rv = usip_nameaddr_encode( dlgsess->tuser, tuser, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) tuser[0] = '\0';
		}
		buflen++;
	
		ocontact = pbuffer+buflen; 
		if( dlgsess->ocontact) {
			rv = usip_contact_encode( dlgsess->ocontact, ocontact, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
		} else {
			if(buflen < bufsize) ocontact[0] = '\0';
		}
		buflen++;

		oroute = pbuffer+buflen; 
		rthdr = dlgsess->oroute;
		route = oroute;
		idx = 0;
		while( rthdr ) {
			if( route != oroute ) {
				if( bufsize > buflen+2) strlcpy(route, ", ", bufsize-buflen);
				buflen += 2;
				route += 2;
			}
			rv = usip_route_encode( rthdr, route, bufsize-buflen, 0);
			if( rv < 0) return rv; 
			buflen += rv;
			route += rv;
			rthdr = (usip_route_hdr_t*)rthdr->base->next;
		}
		if(buflen < bufsize) route[0] = '\0';
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

	rv = uims_dbdataset_write( paraset, 22,
			//name, type, value, [length:octet only]
			"sess_id", UIMS_DBTYPE_UINT64, uims_sess_get_id(dlgsess->sess),
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"ocseq", UIMS_DBTYPE_UINT32, dlgsess->ocseq,
			"tcseq", UIMS_DBTYPE_UINT32, dlgsess->tcseq,
			"method", UIMS_DBTYPE_UINT8, dlgsess->method,
			"svcmode", UIMS_DBTYPE_UINT8, dlgsess->rtpolicy->svcmode,
			"way", UIMS_DBTYPE_UINT8, dlgsess->rtpolicy->way,
			"opt", UIMS_DBTYPE_UINT32, dlgsess->rtpolicy->opt,
			"opeer", UIMS_DBTYPE_INT32, dlgsess->rtpolicy->opeer_id,
			"tpeer", UIMS_DBTYPE_INT32, dlgsess->rtpolicy->tpeer_id,
			"call_id", UIMS_DBTYPE_STR, uims_sess_get_call_id(dlgsess->sess),
			"ouser", UIMS_DBTYPE_STR, ouser,
			"otag", UIMS_DBTYPE_STR, uims_sess_get_ltag(dlgsess->sess),
			"ocontact", UIMS_DBTYPE_STR, ocontact,
			"oroute", UIMS_DBTYPE_STR, oroute,
			"tuser", UIMS_DBTYPE_STR, tuser,
			"ttag", UIMS_DBTYPE_STR, "",
			"tcontact", UIMS_DBTYPE_STR, "",
			"troute", UIMS_DBTYPE_STR, "",
			"tpeer", UIMS_DBTYPE_STR, dlgsess->tpeer ? dlgsess->tpeer : "");
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
	dlgsess->hasreq = USIP_FALSE;
	*/
	return UX_SUCCESS;
}

ux_status_t clicktocall_dlgdao_remove( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	/*
	static const char* stmtid = "DLGSESS:REMOVE";
	static const char* query = "DELETE FROM table1 WHERE index0=?";

	int rv;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS REMOVE. (sessid=%llu, extime=%lu)",
			(unsigned long long)uims_sess_get_id(dlgsess->sess), dlgsess->extime);

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
	*/
	return UX_SUCCESS;
}

ux_status_t clicktocall_dlgdao_update( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	/*
	dlgsess->extime = time(NULL);

	if( dlgsess->dlgstate < CLICKTOCALL_DLGSTATE_PROCEEDING) {
		return UX_SUCCESS;
	} else if( dlgsess->dlgstate < CLICKTOCALL_DLGSTATE_ESTABLISHED) {
		if( dlgsess->prevstate != dlgsess->dlgstate) {
			dlgsess->prevstate = dlgsess->dlgstate;
			return clicktocall_dlgdao_update_p( dao, dlgsess);
		}
	} else if( dlgsess->dlgstate == CLICKTOCALL_DLGSTATE_ESTABLISHED) {
		if( dlgsess->hasreq) {
			dlgsess->hasreq = UX_FALSE;
			return clicktocall_dlgdao_update_e( dao, dlgsess);
		}
	}
	*/

	return UX_SUCCESS;
}

/**
 * @internal proceeding 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_p( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	/*
	static const char* stmtid = "DLGSESS:UPDATE_P";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, column4=?, "
				"column5=?, column18=?, column19=?, column20=? WHERE index0=?";

	int rv, bufsize, buflen;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;
	usip_route_hdr_t *rthdr;
	char *tcontact, *troute, *route;
	char *pbuffer, buffer[8192];

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE. (sessid=%llu, svcst=%d, state=%s, extime=%lu)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state,
			clicktocall_dlgstate_to_str( dlgsess->dlgstate), dlgsess->extime);

	bufsize = sizeof(buffer); 
	pbuffer = buffer; 
	do {
		buflen = 0;
	
		tcontact = pbuffer+buflen; 
		if( dlgsess->tcontact) {
			rv = usip_contact_encode( dlgsess->tcontact, tcontact, bufsize-buflen, 0);
			if( rv < 0) {
				ux_log(UXL_MAJ, "Failed to encode contact. (err=%d,%s)", rv, usip_errstr(rv));
				return rv; 
			}
			buflen += rv;
		} else {
			if(buflen < bufsize) tcontact[0] = '\0';
		}
		buflen++;

		troute = pbuffer+buflen; 
		rthdr = dlgsess->troute;
		route = troute;
		while( rthdr ) {
			if( route != troute ) {
				if( bufsize > buflen+2) strlcpy(route, ", ", bufsize-buflen);
				buflen += 2;
				route += 2;
			}
			rv = usip_route_encode( rthdr, route, bufsize-buflen, 0);
			if( rv < 0) {
				ux_log(UXL_MAJ, "Failed to encode contact. (err=%d,%s)", rv, usip_errstr(rv));
				return rv; 
			}
			buflen += rv;
			route += rv;
			rthdr = (usip_route_hdr_t*)rthdr->base->next;
		}
		if(buflen < bufsize) route[0] = '\0';
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

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_P (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, ocseq=%u, "
			"tcseq=%u, ttag=%s, tcontact=%s, troute=%s)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, dlgsess->ocseq, dlgsess->tcseq,
			uims_sess_get_rtag(dlgsess->sess), tcontact, troute);

	rv = uims_dbdataset_write( paraset, 9,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"ocseq", UIMS_DBTYPE_UINT32, dlgsess->ocseq,
			"tcseq", UIMS_DBTYPE_UINT32, dlgsess->tcseq,
			"ttag", UIMS_DBTYPE_STR, uims_sess_get_rtag(dlgsess->sess),
			"tcontact", UIMS_DBTYPE_STR, tcontact,
			"troute", UIMS_DBTYPE_STR, troute, 
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

	if( dlgsess->dlgstate >= CLICKTOCALL_DLGSTATE_ACCEPTED) { 
		dlgsess->dlgstate = CLICKTOCALL_DLGSTATE_ESTABLISHED;
		dlgsess->prevstate = dlgsess->dlgstate;
	}
	*/
	return UX_SUCCESS;
}

/**
 * @internal established 상태에서 session을 update 한다.
 * @param dao DAO
 * @param dlgsess dialog session
 */
ux_status_t clicktocall_dlgdao_update_e( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess)
{
	/*
	static const char* stmtid = "DLGSESS:UPDATE_E";
	static const char* query = "UPDATE table1 SET column1=?, column2=?, column3=?, column4=?, column5=? WHERE index0=?";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_sesshdr_t *sesshdr;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	uxcsess = uims_sess_get_uxcsess( dlgsess->sess);
	sesshdr = uxc_sess_get_hdr( uxcsess);

	ux_log(UXL_INFO, "CLICKTOCALL DLGSESS UPDATE. (sessid=%llu, svcst=%d, state=ESTABLISHED, extime=%lu)",
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

	ux_log(UXL_DBG1, "DLGSESS:UPDATE_E (sess_id=%llu, state=%d, dlgstate=%d, extime=%llu, ocseq=%u, tcseq=%u)",
			(unsigned long long)uims_sess_get_id( dlgsess->sess), sesshdr->state, dlgsess->dlgstate,
			(unsigned long long)dlgsess->extime, dlgsess->ocseq, dlgsess->tcseq);

	rv = uims_dbdataset_write( paraset, 6,
			//name, type, value, [length:octet only]
			"extime", UIMS_DBTYPE_UINT64, dlgsess->extime,
			"state", UIMS_DBTYPE_UINT16, sesshdr->state,
			"dlgstate", UIMS_DBTYPE_UINT8, dlgsess->dlgstate,
			"ocseq", UIMS_DBTYPE_UINT32, dlgsess->ocseq,
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
	*/
	return UX_SUCCESS;
}


