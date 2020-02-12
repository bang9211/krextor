#include <usip/usip_def.h>
#include <uxcutor/uxc_sdm.h>
#include <uxcutor/uxc_sfsm.h>
#include <upa/upa_sippa.h>
#include <upa/upa_httppa.h>
#include "uims/uims_sipmsg.h"
#include "local/clicktocall_dlgsess_l.h"
#include "clicktocall/clicktocall_error.h"

#include <stdio.h>

/**
 * @brief CLICKTOCALL START REQUEST(HTTP) outgoing 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_http_start_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALLING_NUMBER, PARA_CALLED_NUMBER, PARA_RINGBACKTONE_TYPE, PARA_WAITNG_MENTID, PARA_CALL_MENTID, PARA_CALLING_CID, PARA_CALLED_CID };
	static const char *func = "";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	upa_httpmsg_t *reqmsg;
	uims_sess_t *imssess;
	clicktocall_dlgsess_t *dlgsess;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	reqmsg = (upa_httpmsg_t*)rcvmsg->data;

	imssess = uxc_sess_get_user_data( uxcsess);
	if( imssess == NULL) {
		uxc_trace(UXCTL(1,MAJ), "%s: IMS session instance in uxcutor session doesn't exist.", func);
		return UX_EINVAL;
	}

	dlgsess = clicktocall_dlgsess_create( imssess, &rv); 
	if( dlgsess == NULL) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to create %s dialog session. (err=%d,%s)",
				func, uhttp_req_get_path((uhttp_req_t*)reqmsg->msg), rv, ux_errnostr(rv));
		return UX_ENOMEM;
	}

	rv = clicktocall_dlgsess_handle_http_start_req( dlgsess, reqmsg);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to handle %s request. (phrase=%s, err=%d,%s)",
				func, uhttp_req_get_path((uhttp_req_t*)reqmsg->msg),
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	rv = uxc_sdmvars_set_str( params, PARA_CALLING_NUMBER, dlgsess->callingnumber);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set CALLING_NUMBER parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
	}

	rv = uxc_sdmvars_set_str( params, PARA_CALLED_NUMBER, dlgsess->callednumber);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set CALLED_NUMBER parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
	}

	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL START REQUEST(HTTP) outgoing 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_http_start_res( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_RESULT_CODE };
	static const char *func = "clicktocall_dlgsvc_on_send_http_start_res";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg;
	upa_httpmsg_t *resmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	resmsg = (upa_httpmsg_t*)sndmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	dlgsess->error = uxc_sdmvars_get_int( params, PARA_RESULT_CODE, 0, &rv);
	if( rv < UX_SUCCESS) return rv;

	rv = clicktocall_dlgsess_make_http_start_res( dlgsess, resmsg);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to make response. (phrase=%s, err=%d,%s)",
				func,
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) 새로운 outgoing 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_sip_invite_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_BODY};

	static const char *func = "clicktocall_dlgsvc_on_send_sip_invite_req";

	char *ssw = "127.0.0.1:5068";
	char *ssw_bye = "127.0.0.1:5069";
	char *host = "127.0.0.1";
	char *ms = "127.0.0.1:5070";
	char *msuser = "99980000";
	int servicekey = 99;
	int dp = 12;

	int rv, urisize, bodysize;
	clicktocall_callto_e callto;
	usip_uri_t uri[1];
	ux_time_t curtime[1];
	const char *calling, *called;
	char requri[128], from[128], to[128];
	uint8_t *body;
	usip_payload_t *payload;
	upa_sippa_t *sippa;
	char tag[128];
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;

	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg(uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)(sndmsg->data);

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}
	
	callto = uxc_sdmvars_get_int( params, PARA_CALL_TO, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Fail to get CALL_TO parameter.", func);
		return rv;
	}

	sippa = (upa_sippa_t*)sipmsg->uxcmsg->paif;
	switch (callto) {
		case CALL_TO_CALLING:
			if (dlgsess->calledcid == NULL || strcmp(dlgsess->calledcid, "") == 0) {
				calling = dlgsess->callednumber;
			} else {
				calling = dlgsess->calledcid;
			}
			called = dlgsess->callingnumber;
			sprintf(requri, "sip:%s@%s;ServiceKey=%d;DP=%d", called, ssw, servicekey, dp);
			sipmsg->sessinfo->did = 0;
			
			break;
		case CALL_TO_CALLED:
			if (dlgsess->callingcid == NULL || strcmp(dlgsess->callingcid, "") == 0) {
				calling = dlgsess->callingnumber;
			} else {
				calling = dlgsess->callingcid;
			}
			called = dlgsess->callednumber;
			sprintf(requri, "sip:%s@%s;ServiceKey=%d;DP=%d", called, ssw_bye, servicekey, dp);
			sipmsg->sessinfo->did = 1;
			break;
		case CALL_TO_MS_CALLING:
			calling = dlgsess->callingnumber;
			called = msuser;
			sprintf(requri, "sip:%s@%s", called, ms);
			sipmsg->sessinfo->did = 2;
			break;
		case CALL_TO_MS_CALLED:
			calling = dlgsess->callednumber;
			called = msuser;
			sprintf(requri, "sip:%s@%s", called, ms);
			sipmsg->sessinfo->did = 3;
			break;
		default: 
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid CALL_TO parameter value=%d.", func, callto);
			return UX_EINVAL;
	}
	upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));

	if( uxc_sdmvars_is_set(params, PARA_BODY) ) {
		body = uxc_sdmvars_get_oct(params, PARA_BODY, NULL, &bodysize); 
		if( body == NULL) {
			uxc_trace(UXCTL(1,MAJ), "%s: Fail to get BODY parameter.", func);
			return UX_EINVAL;
		}
	} else {
		body = NULL;
	}
	
	urisize = strlen( requri);
	usip_uri_init( uri);
	rv = usip_uri_decode( uri, (char *)requri, urisize); 
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to decode SIP URI. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}
	sprintf(from, "sip:%s@%s;tag=%s", calling, host, tag);
	sprintf(to, "sip:%s@%s", called, ssw);

	rv = usip_mobj_make_request( sipmsg->mobj, "INVITE", uri, from, to);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make SIP request. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	ux_time_get_current( curtime);
	rv = usip_mobj_set_cseq( sipmsg->mobj, (curtime->tv_sec * 1000 + curtime->tv_usec / 1000) & 0xFFFFF, "INVITE");
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make SIP CSeq header . err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}
	
	if (body != NULL) {
		rv = usip_mobj_set_content_type(sipmsg->mobj, "application/sdp");
		if( rv < USIP_SUCCESS) {
			return rv;
		}

		payload = usip_payload_create( usip_msg_get_allocator(sipmsg->mobj->base->msg), (void*)body, bodysize);
		if( payload == NULL) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(bodysize));
			return bodysize;
		}

		rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, payload->base, 0);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}

	rv = usip_mobj_complete_request( sipmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	rv = clicktocall_dlgsess_handle_sip_invite_req( dlgsess, sipmsg, callto);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to handle %s request. (phrase=%s, err=%d,%s)",
				func,usip_mobj_get_method(sipmsg->mobj),
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	ux_log(UXL_INFO, "%s: requri="USIP_URI_PRINT_FORMAT", call_id=%s, from=%s:%s, to=%s:%s", 
		 func, USIP_URI_PRINT_ARGS(uri), USIP_MOBJ_GET_CALLID( sipmsg->mobj), 
		 USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj),
			USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj));


	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) 새로운 outgoing 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_sip_reinvite_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_BODY};

	static const char *func = "clicktocall_dlgsvc_on_send_sip_reinvite_req";
	char *ssw = "127.0.0.1:5068";
	char *ssw_bye = "127.0.0.1:5069";
	char *host = "127.0.0.1";
	char *ms = "127.0.0.1:5070";
	char *msuser = "99980000";
	int servicekey = 99;
	int dp = 12;

	int rv, urisize, bodysize;
	usip_uri_t uri[1];
	uint8_t *body;
	char requri[128];
	char *call_id;
	uint32_t cseq;
	usip_nameaddr_t *from, *to;
	clicktocall_callto_e callto;
	usip_payload_t *payload;
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg(uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)(sndmsg->data);

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	char buf[1024];
	int buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);

	callto = uxc_sdmvars_get_int( params, PARA_CALL_TO, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Fail to get CALL_TO parameter.", func);
		return rv;
	}

	switch (callto) {
		case CALL_TO_CALLING:
			dlgsess->ocseq++;
			cseq = dlgsess->ocseq;
			call_id = dlgsess->ocall_id;
			from = dlgsess->ofrom;
			to = dlgsess->oto;
			sprintf(requri, "sip:%s@%s;ServiceKey=%d;DP=%d", dlgsess->callingnumber, ssw, servicekey, dp);
			break;
		case CALL_TO_CALLED:
			dlgsess->tcseq++;
			cseq = dlgsess->tcseq;
			call_id = dlgsess->tcall_id;
			from = dlgsess->tfrom;
			to = dlgsess->tto;
			sprintf(requri, "sip:%s@%s;ServiceKey=%d;DP=%d", dlgsess->callednumber, ssw_bye, servicekey, dp);
			break;
		case CALL_TO_MS_CALLING:
		case CALL_TO_MS_CALLED:
			dlgsess->mscseq++;
			cseq = dlgsess->mscseq;
			call_id = dlgsess->mscall_id;
			from = dlgsess->msfrom;
			to = dlgsess->msto;
			sprintf(requri, "sip:%s@%s", msuser, ms);
			break;
		default:
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid CALL_TO parameter value=%d.", func, callto);
			return UX_EINVAL;
	}
	
	if( uxc_sdmvars_is_set(params, PARA_BODY) ) {
		body = uxc_sdmvars_get_oct(params, PARA_BODY, NULL, &bodysize); 
		if( body == NULL) return UX_EINVAL;
	} else {
		body = NULL;
	}

	urisize = strlen( requri);
	usip_uri_init( uri);
	rv = usip_uri_decode( uri, (char *)requri, urisize); 
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to decode SIP URI. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_set_cseq( sipmsg->mobj, cseq, "INVITE");
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make cseq header. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_set_call_id( sipmsg->mobj, call_id);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make call_id header. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	usip_from_hdr_t *hfrom = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)from, usip_msg_get_allocator(sipmsg->mobj->base->msg));
	if( hfrom  == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator From header.");
		return UX_ENOMEM;
	}
	rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, hfrom->base, 0); 
	if( rv < UX_SUCCESS) {
		usip_log(USL_CRT, "Failed to add 'From' header. (value=%s:%s, err=%d,%s)",
				USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj), 
				rv, usip_errstr(rv));
		return rv;
	}

	usip_to_hdr_t *hto= (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)to, usip_msg_get_allocator(sipmsg->mobj->base->msg));
	if( hto  == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator To header.");
		return UX_ENOMEM;
	}
	rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, hto->base, 0); 
	if( rv < UX_SUCCESS) {
		usip_log(USL_CRT, "Failed to add 'To' header. (value=%s:%s, err=%d,%s)",
				USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj), 
				rv, usip_errstr(rv));
		return rv;
	}
	
	rv = usip_mobj_set_request( sipmsg->mobj, "INVITE", uri);
	if( rv < USIP_SUCCESS) return rv; 
	
	if (body != NULL) {
		rv = usip_mobj_set_content_type(sipmsg->mobj, "application/sdp");
		if( rv < USIP_SUCCESS) {
			return rv;
		}

		payload = usip_payload_create( usip_msg_get_allocator(sipmsg->mobj->base->msg), (void*)body, bodysize);
		if( payload == NULL) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(bodysize));
			return bodysize;
		}

		rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, payload->base, 0);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}

	rv = usip_mobj_complete_request( sipmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	ux_log(UXL_DEBUG, "%s: requri="USIP_URI_PRINT_FORMAT", call_id=%s, from=%s:%s, to=%s:%s", 
		 func, USIP_URI_PRINT_ARGS(uri), USIP_MOBJ_GET_CALLID( sipmsg->mobj), 
		 USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj),
			USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj));

	return UX_SUCCESS;
}


/**
 * @brief DIALOG INITIAL REQUEST(INVITE) 새로운 outgoing 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_sip_ack( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_BODY};

	static const char *func = "clicktocall_dlgsvc_on_send_sip_ack";
	char *ssw = "127.0.0.1:5068";
	char *ssw_bye = "127.0.0.1:5069";
	char *ms = "127.0.0.1:5070";
	char *msuser = "99980000";

	int rv, urisize, bodysize;
	usip_uri_t uri[1];
	uint8_t *body;
	char requri[128];
	char *call_id;
	uint32_t cseq;
	usip_nameaddr_t *from, *to;
	clicktocall_callto_e callto;
	usip_payload_t *payload;
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg(uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)(sndmsg->data);

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	char buf[1024];
	int buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);

	callto = uxc_sdmvars_get_int( params, PARA_CALL_TO, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Fail to get CALL_TO parameter.", func);
		return rv;
	}

	switch (callto) {
		case CALL_TO_CALLING:
			cseq = dlgsess->ocseq;
			call_id = dlgsess->ocall_id;
			from = dlgsess->ofrom;
			to = dlgsess->oto;
			sprintf(requri, "sip:%s@%s", dlgsess->callingnumber, ssw);
			break;
		case CALL_TO_CALLED:
			cseq = dlgsess->tcseq;
			call_id = dlgsess->tcall_id;
			from = dlgsess->tfrom;
			to = dlgsess->tto;
			sprintf(requri, "sip:%s@%s", dlgsess->callednumber, ssw_bye);
			break;
		case CALL_TO_MS_CALLING:
		case CALL_TO_MS_CALLED:
			cseq = dlgsess->mscseq;
			call_id = dlgsess->mscall_id;
			from = dlgsess->msfrom;
			to = dlgsess->msto;
			sprintf(requri, "sip:%s@%s", msuser, ms);
			break;
		default:
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid CALL_TO parameter value=%d.", func, callto);
			return UX_EINVAL;
	}
	
	if( uxc_sdmvars_is_set(params, PARA_BODY) ) {
		body = uxc_sdmvars_get_oct(params, PARA_BODY, NULL, &bodysize); 
		if( body == NULL) return UX_EINVAL;
	} else {
		body = NULL;
	}

	urisize = strlen( requri);
	usip_uri_init( uri);
	rv = usip_uri_decode( uri, (char *)requri, urisize); 
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to decode SIP URI. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_set_cseq( sipmsg->mobj, cseq, "ACK");
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make cseq header. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_set_call_id( sipmsg->mobj, call_id);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make call_id header. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	usip_from_hdr_t *hfrom = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)from, usip_msg_get_allocator(sipmsg->mobj->base->msg));
	if( hfrom  == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator From header.");
		return UX_ENOMEM;
	}
	rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, hfrom->base, 0); 
	if( rv < UX_SUCCESS) {
		usip_log(USL_CRT, "Failed to add 'From' header. (value=%s:%s, err=%d,%s)",
				USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj), 
				rv, usip_errstr(rv));
		return rv;
	}

	usip_to_hdr_t *hto= (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)to, usip_msg_get_allocator(sipmsg->mobj->base->msg));
	if( hto  == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator To header.");
		return UX_ENOMEM;
	}
	rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, hto->base, 0); 
	if( rv < UX_SUCCESS) {
		usip_log(USL_CRT, "Failed to add 'To' header. (value=%s:%s, err=%d,%s)",
				USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj), 
				rv, usip_errstr(rv));
		return rv;
	}
	
	rv = usip_mobj_set_request( sipmsg->mobj, "ACK", uri);
	if( rv < USIP_SUCCESS) return rv; 
	
	if (body != NULL) {
		rv = usip_mobj_set_content_type(sipmsg->mobj, "application/sdp");
		if( rv < USIP_SUCCESS) {
			return rv;
		}

		payload = usip_payload_create( usip_msg_get_allocator(sipmsg->mobj->base->msg), (void*)body, bodysize);
		if( payload == NULL) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(bodysize));
			return bodysize;
		}

		rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, payload->base, 0);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}

	rv = usip_mobj_complete_request( sipmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	ux_log(UXL_DEBUG, "%s: requri="USIP_URI_PRINT_FORMAT", call_id=%s, from=%s:%s, to=%s:%s", 
		 func, USIP_URI_PRINT_ARGS(uri), USIP_MOBJ_GET_CALLID( sipmsg->mobj), 
		 USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj),
			USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj));

	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) 새로운 outgoing 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_sip_dlgtransc_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_METHOD, PARA_CONTENTTYPE, PARA_BODY};

	static const char *func = "clicktocall_dlgsvc_on_send_sip_dlgtransc_req";
	char *ssw = "127.0.0.1:5068";
	char *ssw_bye = "127.0.0.1:5069";
	char *ms = "127.0.0.1:5070";
	char *msuser = "99980000";

	int rv, urisize, bodysize;
	usip_uri_t uri[1];
	uint8_t *body;
	char requri[128];
	const char *call_id, *method, *contenttype;
	uint32_t cseq;
	usip_nameaddr_t *from, *to;
	clicktocall_callto_e callto;
	usip_payload_t *payload;
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg(uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)(sndmsg->data);

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	char buf[1024];
	int buflen = sizeof(buf);
	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);

	callto = uxc_sdmvars_get_int( params, PARA_CALL_TO, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Fail to get CALL_TO parameter.", func);
		return rv;
	}

	switch (callto) {
		case CALL_TO_CALLING:
			dlgsess->ocseq++;
			cseq = dlgsess->ocseq;
			call_id = dlgsess->ocall_id;
			from = dlgsess->ofrom;
			to = dlgsess->oto;
			sprintf(requri, "sip:%s@%s", dlgsess->callingnumber, ssw);
			break;
		case CALL_TO_CALLED:
			dlgsess->tcseq++;
			cseq = dlgsess->tcseq;
			call_id = dlgsess->tcall_id;
			from = dlgsess->tfrom;
			to = dlgsess->tto;
			sprintf(requri, "sip:%s@%s", dlgsess->callednumber, ssw_bye);
			break;
		case CALL_TO_MS_CALLING:
		case CALL_TO_MS_CALLED:
			dlgsess->mscseq++;
			cseq = dlgsess->mscseq;
			call_id = dlgsess->mscall_id;
			from = dlgsess->msfrom;
			to = dlgsess->msto;
			sprintf(requri, "sip:%s@%s", msuser, ms);
			break;
		default:
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid CALL_TO parameter value=%d.", func, callto);
			return UX_EINVAL;
	}

	method = uxc_sdmvars_get_str( params, PARA_METHOD, NULL, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Fail to get PARA_METHOD parameter. err=%d", func, rv);
		return rv;
	}
	
	if( uxc_sdmvars_is_set(params, PARA_CONTENTTYPE) ) {
		contenttype = uxc_sdmvars_get_str( params, PARA_CONTENTTYPE, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "%s: Fail to get PARA_CONTENTTYPE parameter. err=%d", func, rv);
			return rv;
		}
	} else {
		contenttype = NULL;
	}
	if( uxc_sdmvars_is_set(params, PARA_BODY) ) {
		body = uxc_sdmvars_get_oct(params, PARA_BODY, NULL, &bodysize); 
		if( body == NULL) return UX_EINVAL;
	} else {
		body = NULL;
	}

	urisize = strlen( requri);
	usip_uri_init( uri);
	rv = usip_uri_decode( uri, (char *)requri, urisize); 
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to decode SIP URI. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_set_cseq( sipmsg->mobj, cseq, method);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make cseq header. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_set_call_id( sipmsg->mobj, call_id);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make call_id header. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	usip_from_hdr_t *hfrom = (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)from, usip_msg_get_allocator(sipmsg->mobj->base->msg));
	if( hfrom  == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator From header.");
		return UX_ENOMEM;
	}
	rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, hfrom->base, 0); 
	if( rv < UX_SUCCESS) {
		usip_log(USL_CRT, "Failed to add 'From' header. (value=%s:%s, err=%d,%s)",
				USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj), 
				rv, usip_errstr(rv));
		return rv;
	}

	usip_to_hdr_t *hto= (usip_nameaddr_t*)usip_hdr_duplicate( (usip_hdr_t*)to, usip_msg_get_allocator(sipmsg->mobj->base->msg));
	if( hto  == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate originator To header.");
		return UX_ENOMEM;
	}
	rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, hto->base, 0); 
	if( rv < UX_SUCCESS) {
		usip_log(USL_CRT, "Failed to add 'To' header. (value=%s:%s, err=%d,%s)",
				USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj), 
				rv, usip_errstr(rv));
		return rv;
	}
	
	rv = usip_mobj_set_request( sipmsg->mobj, method, uri);
	if( rv < USIP_SUCCESS) return rv; 
	
	if (contenttype != NULL && body != NULL) {
		rv = usip_mobj_set_content_type(sipmsg->mobj, contenttype);
		if( rv < USIP_SUCCESS) {
			return rv;
		}

		payload = usip_payload_create( usip_msg_get_allocator(sipmsg->mobj->base->msg), (void*)body, bodysize);
		if( payload == NULL) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(bodysize));
			return bodysize;
		}

		rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, payload->base, 0);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set SIP payload. err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}

	rv = usip_mobj_complete_request( sipmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	ux_log(UXL_INFO, "%s: call_id=%s, from=%s:%s, to=%s:%s", 
		 func, USIP_MOBJ_GET_CALLID( sipmsg->mobj), 
		 USIP_MOBJ_GET_FROMUSER( sipmsg->mobj), USIP_MOBJ_GET_FROMTAG( sipmsg->mobj),
			USIP_MOBJ_GET_TOUSER( sipmsg->mobj), USIP_MOBJ_GET_TOTAG( sipmsg->mobj));

	return UX_SUCCESS;
}


/**
 * @brief DIALOG INITIAL response(INVITE, SUBSCRIBE)?? ???? ???? ??? ????? ??? 
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ???? ???
 * @remark
 * para[0] = STATUS - response code
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_sip_invite_res( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_STATUS, PARA_BODY};
	static const char *func = "clicktocall_dlgsvc_on_recv_sip_invite_res";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	upa_sipmsg_t *resmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	clicktocall_callto_e callto;
	char buf[1024];
	int buflen = sizeof(buf);

	uxcsess = (uxc_sess_t*)params->sdm->impl;

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	resmsg = (upa_sipmsg_t*)rcvmsg->data;
	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	callto = uxc_sdmvars_get_int( params, PARA_CALL_TO, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Fail to get CALL_TO parameter.", func);
		return rv;
	}

	rv = clicktocall_dlgsess_handle_sip_invite_res( dlgsess, resmsg, callto);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to make response. (phrase=%s, err=%d,%s)",
				func,
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	rv = uxc_sdmvars_set_int( params, PARA_STATUS, resmsg->mobj->status->code);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set STATUS parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
	}

	if (resmsg->mobj->payload != NULL) {
		rv = uxc_sdmvars_set_str( params, PARA_BODY, resmsg->mobj->payload->data);
		if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set BODY parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
		}
	}

	clicktocall_dlgsess_sprint(dlgsess, buf, buflen);
	ux_log(UXL_INFO, "%s", buf);

	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE, SUBSCRIBE) 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_rsp( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_STATUS};

	static const char *func = "clicktocall_dlgsvc_on_send_rsp";

	int rv, status;

	upa_sipmsg_t *rspmsg, *reqmsg;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg, *rcvmsg;
	sndmsg = uxc_sess_get_sndmsg(uxcsess);
	if( sndmsg == NULL ) {
		ux_log(UXL_MAJ, "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	rspmsg = (upa_sipmsg_t*)(sndmsg->data);

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		ux_log(UXL_MAJ, "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	reqmsg = (upa_sipmsg_t*)rcvmsg->data;

	ux_log(UXL_INFO, "clicktocall_dlgsvc_on_send_rsp: #1")
	
	status = uxc_sdmvars_get_int( params, PARA_STATUS, 0, &rv);
	if( rv < UX_SUCCESS) return rv;
	
	
	rv = usip_mobj_make_response( rspmsg->mobj, clicktocall_err_to_rcode( status),
					clicktocall_err_to_phrase(status), reqmsg->mobj);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "%s: Failed to make SIP response. (status=%d,%s, err=%d,%s)",
				func, status, clicktocall_err_to_phrase(status), rv, usip_errstr(rv));
		return rv;
	}


	rv = usip_mobj_complete( rspmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	ux_log(UXL_INFO, "clicktocall_dlgsvc_on_send_rsp: #7");

	return UX_SUCCESS;
}
