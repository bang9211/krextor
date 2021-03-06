#include <usip/usip_def.h>
#include <uxcutor/uxc_sdm.h>
#include <uxcutor/uxc_sfsm.h>
#include <upa/upa_sippa.h>
#include <upa/upa_httppa.h>
#include <uxcutor/uxc_dbif.h>


#include "uims/uims_sipmsg.h"
#include "local/clicktocall_dlgsess_l.h"
#include "clicktocall/clicktocall_error.h"

#include <stdio.h>

/**
 * @brief CLICKTOCALL START REQUEST(HTTP) outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_http_start_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALLING_NUMBER, PARA_CALLED_NUMBER, PARA_RINGBACKTONE_TYPE, PARA_WAITNG_MENTID, PARA_CALL_MENTID, PARA_CALLING_CID, PARA_CALLED_CID };
	static const char *func = "clicktocall_dlgsvc_on_recv_http_start_req";

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

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL START REQUEST(HTTP) outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_http_res( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_RESULT_CODE };
	static const char *func = "clicktocall_dlgsvc_on_send_http_res";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg;
	upa_httpmsg_t *httpmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	httpmsg = (upa_httpmsg_t*)sndmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	dlgsess->error = uxc_sdmvars_get_int( params, PARA_RESULT_CODE, 0, &rv);
	if( rv < UX_SUCCESS) return rv;

	rv = clicktocall_dlgsess_make_http_res( dlgsess, httpmsg);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to make response. (phrase=%s, err=%d,%s)",
				func,
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL STOP REQUEST(HTTP) outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_http_stop_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "clicktocall_dlgsvc_on_recv_http_stop_req";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	upa_httpmsg_t *httpmsg;
	uims_sess_t *imssess;
	clicktocall_dlgsess_t *dlgsess;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	httpmsg = (upa_httpmsg_t*)rcvmsg->data;

	imssess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (imssess) ? uims_sess_get_data( imssess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	rv = clicktocall_dlgsess_handle_http_stop_req( dlgsess, httpmsg);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to handle %s request. (phrase=%s, err=%d,%s)",
				func, uhttp_req_get_path((uhttp_req_t*)httpmsg->msg),
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL NOTI REQUEST(HTTP) outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_http_notify( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_EVENT};
	static const char *func = "clicktocall_dlgsvc_on_send_http_notify";

	int rv;
	clicktocall_callto_e callto;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg;
	upa_httpmsg_t *httpmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uhttp_hdrs_t *hdrs;
	char sid[64];
	
	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	httpmsg = (upa_httpmsg_t*)sndmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	callto = uxc_sdmvars_get_int( params, PARA_CALL_TO, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Fail to get CALL_TO parameter. err=%d", func, rv);
		return rv;
	}

	dlgsess->error = uxc_sdmvars_get_int( params, PARA_EVENT, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Fail to get EVENT_NUMBER parameter. err=%d", func, rv);
		return rv;
	}

	rv = clicktocall_dlgsess_make_http_notify( dlgsess, httpmsg, callto);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to make response. (phrase=%s, err=%d,%s)",
				func,
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	hdrs = uhttp_msg_get_hdrs( httpmsg->msg);
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(dlgsess->sess));
	rv = uhttp_hdrs_set_str( hdrs, "SessionID", 0, dlgsess->sessionid);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set header value. (name=%s[%d], sval=%s, err=%d,%s)", 
			func, "SessionID", 0, dlgsess->sessionid, rv, uhttp_errstr(rv));
		return UX_EINVAL;
	}
	rv = uhttp_hdrs_set_str( hdrs, "GW-SessionID", 0, sid);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set header value. (name=%s[%d], sval=%s, err=%d,%s)", 
			func, "GW-SessionID", 0, dlgsess->sessionid, rv, uhttp_errstr(rv));
		return UX_EINVAL;
	}

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL RESPOND REQUEST(HTTP) outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_http_respond( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_RESULT_CODE};
	static const char *func = "clicktocall_dlgsvc_on_send_http_respond";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg;
	upa_httpmsg_t *httpmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uhttp_hdrs_t *hdrs;
	char sid[64];
	
	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	httpmsg = (upa_httpmsg_t*)sndmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}


	dlgsess->error = uxc_sdmvars_get_int( params, PARA_RESULT_CODE, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Fail to get EVENT_NUMBER parameter. err=%d", func, rv);
		return rv;
	}

	rv = clicktocall_dlgsess_make_http_respond( dlgsess, httpmsg);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to make response. (phrase=%s, err=%d,%s)",
				func,
				clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
		return rv;
	}

	hdrs = uhttp_msg_get_hdrs( httpmsg->msg);
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(dlgsess->sess));
	rv = uhttp_hdrs_set_str( hdrs, "SessionID", 0, dlgsess->sessionid);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set header value. (name=%s[%d], sval=%s, err=%d,%s)", 
			func, "SessionID", 0, dlgsess->sessionid, rv, uhttp_errstr(rv));
		return UX_EINVAL;
	}
	rv = uhttp_hdrs_set_str( hdrs, "GW-SessionID", 0, sid);
	if( rv < UHTTP_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set header value. (name=%s[%d], sval=%s, err=%d,%s)", 
			func, "GW-SessionID", 0, dlgsess->sessionid, rv, uhttp_errstr(rv));
		return UX_EINVAL;
	}

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) ���ο� outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
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
			sipmsg->sessinfo->did = 0;
			upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
			sprintf(requri, "sip:%s@%s;ServiceKey=%d;DP=%d", called, ssw, servicekey, dp);
			sprintf(from, "sip:%s@%s;tag=%s", calling, host, tag);
			sprintf(to, "sip:%s@%s", called, ssw);
			break;
		case CALL_TO_CALLED:
			if (dlgsess->callingcid == NULL || strcmp(dlgsess->callingcid, "") == 0) {
				calling = dlgsess->callingnumber;
			} else {
				calling = dlgsess->callingcid;
			}
			called = dlgsess->callednumber;
			sipmsg->sessinfo->did = 1;
			upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
			sprintf(requri, "sip:%s@%s;ServiceKey=%d;DP=%d", called, ssw_bye, servicekey, dp);
			sprintf(from, "sip:%s@%s;tag=%s", calling, host, tag);
			sprintf(to, "sip:%s@%s", called, ssw_bye);
			break;
		case CALL_TO_MS_CALLING:
			calling = dlgsess->callingnumber;
			called = msuser;
			sipmsg->sessinfo->did = 2;
			upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
			sprintf(requri, "sip:%s@%s", called, ms);
			sprintf(from, "sip:%s@%s;tag=%s", calling, host, tag);
			sprintf(to, "sip:%s@%s", called, ms);
			break;
		case CALL_TO_MS_CALLED:
			calling = dlgsess->callednumber;
			called = msuser;
			sipmsg->sessinfo->did = 3;
			upa_sippa_write_sessinfo( sippa, sipmsg, tag, sizeof(tag));
			sprintf(requri, "sip:%s@%s", called, ms);
			sprintf(from, "sip:%s@%s;tag=%s", calling, host, tag);
			sprintf(to, "sip:%s@%s", called, ms);
			break;
		default: 
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid CALL_TO parameter value=%d.", func, callto);
			return UX_EINVAL;
	}

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

	ux_log(UXL_DBG1, "%s completed", func);	

	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) ���ο� outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_sip_reinvite_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_BODY};

	static const char *func = "clicktocall_dlgsvc_on_send_sip_reinvite_req";
	char *ssw = "127.0.0.1:5068";
	char *ssw_bye = "127.0.0.1:5069";
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

	ux_log(UXL_DBG1, "%s completed", func);	

	return UX_SUCCESS;
}


/**
 * @brief DIALOG INITIAL REQUEST(INVITE) ���ο� outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
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

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) ���ο� outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
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

	ux_log(UXL_DBG1, "%s completed", func);		

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
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	clicktocall_callto_e callto;

	uxcsess = (uxc_sess_t*)params->sdm->impl;

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)rcvmsg->data;
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

	if ( sipmsg->mobj->status->code > 100) {
		rv = clicktocall_dlgsess_handle_sip_invite_res( dlgsess, sipmsg, callto);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MAJ), "%s: Failed to make response. (phrase=%s, err=%d,%s)",
					func,
					clicktocall_err_to_phrase( dlgsess->error), rv, ux_errnostr(rv));
			return rv;
		}
	}

	rv = uxc_sdmvars_set_int( params, PARA_STATUS, sipmsg->mobj->status->code);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set STATUS parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
	}

	if (sipmsg->mobj->payload != NULL) {
		rv = uxc_sdmvars_set_str( params, PARA_BODY, sipmsg->mobj->payload->data);
		if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set BODY parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
		}
	}

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE) ���ο� outgoing �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_sip_dlgtransc_res( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CALL_TO, PARA_STATUS, PARA_BODY};
	static const char *func = "clicktocall_dlgsvc_on_recv_sip_dlgtransc_res";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	clicktocall_callto_e callto;

	uxcsess = (uxc_sess_t*)params->sdm->impl;

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)rcvmsg->data;
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

	rv = uxc_sdmvars_set_int( params, PARA_STATUS, sipmsg->mobj->status->code);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set STATUS parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
	}

	if (sipmsg->mobj->payload != NULL) {
		rv = uxc_sdmvars_set_str( params, PARA_BODY, sipmsg->mobj->payload->data);
		if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "%s: Failed to set BODY parameter. (err=%d,%s)",
				func, rv, uxc_errnostr(rv));
		return rv;
		}
	}

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief DIALOG INITIAL REQUEST(INVITE, SUBSCRIBE) �޽����� �����ϱ� ���� ȣ��Ǵ� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_rsp( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_STATUS};

	static const char *func = "clicktocall_dlgsvc_on_send_rsp";

	int rv, status;

	upa_sipmsg_t *sipmsg, *reqmsg;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg, *rcvmsg;
	sndmsg = uxc_sess_get_sndmsg(uxcsess);
	if( sndmsg == NULL ) {
		ux_log(UXL_MAJ, "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)(sndmsg->data);

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		ux_log(UXL_MAJ, "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	reqmsg = (upa_sipmsg_t*)rcvmsg->data;
	
	status = uxc_sdmvars_get_int( params, PARA_STATUS, 0, &rv);
	if( rv < UX_SUCCESS) return rv;
	
	
	rv = usip_mobj_make_response( sipmsg->mobj, clicktocall_err_to_rcode( status),
					clicktocall_err_to_phrase(status), reqmsg->mobj);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "%s: Failed to make SIP response. (status=%d,%s, err=%d,%s)",
				func, status, clicktocall_err_to_phrase(status), rv, usip_errstr(rv));
		return rv;
	}


	rv = usip_mobj_complete( sipmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	ux_log(UXL_DBG1, "%s completed", func);	

	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL START REQUEST(DBIF) ���� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_dbif_start_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "clicktocall_dlgsvc_on_send_dbif_start_req";

	int rv;
	char sid[64];
	uxc_ipcmsg_t *ipcmsg;
	uxc_dbif_t *dbif;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "Dont' exist send message instance in session");
		return UX_EINVAL;
	}
	ipcmsg = (uxc_ipcmsg_t*)sndmsg->data;
	dbif = (uxc_dbif_t *)ipcmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	ipcmsg->header.cmdId = uxc_sess_get_id(uxcsess);
	ipcmsg->header.userData = uxc_sess_get_id(uxcsess);

	uxc_dbif_set_str(dbif, 0, dlgsess->sessionid);
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(sess));
	uxc_dbif_set_str(dbif, 1, sid);
	uxc_dbif_set_int(dbif, 2, 1); //voip
	uxc_dbif_set_str(dbif, 3, dlgsess->callingnumber);
	uxc_dbif_set_str(dbif, 4, dlgsess->callednumber);
	uxc_dbif_set_int(dbif, 5, 1); // service code
	uxc_dbif_set_int(dbif, 6, dlgsess->ringbacktonetype);
	uxc_dbif_set_int(dbif, 7, dlgsess->watitngmentid);
	uxc_dbif_set_int(dbif, 8, 0);
	uxc_dbif_set_int(dbif, 9, dlgsess->callmentid);
	uxc_dbif_set_str(dbif, 10, dlgsess->callingcid);
	uxc_dbif_set_str(dbif, 11, dlgsess->calledcid);
	uxc_dbif_set_str(dbif, 12, dlgsess->recordingfile);
	switch (dlgsess->ringbacktonetype) {
		case 1: uxc_dbif_set_int(dbif, 13, 1); break;
		case 2: uxc_dbif_set_int(dbif, 13, 0); break;
		default:
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid recording type.", func);
			return UX_EINVAL;
	}
	uxc_dbif_set_int(dbif, 14, 1); 
	uxc_dbif_set_int(dbif, 15, 1);
	uxc_dbif_set_int(dbif, 16, dlgsess->hostcode);
	uxc_dbif_set_int(dbif, 17, 0);
	uxc_dbif_set_int(dbif, 18, 0);

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL START RESPONSE(DBIF) ���� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_dbif_start_res( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "clicktocall_dlgsvc_on_recv_http_res";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	uxc_ipcmsg_t *ipcmsg;
	uxc_dbif_t *dbif;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	int64_t ival;
	const char *sval;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	ipcmsg = (uxc_ipcmsg_t*)rcvmsg->data;
	dbif = ipcmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	ival = uxc_dbif_get_str( dbif, 2, &rv); 
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Can't get int value of dbif[%d](err=%d,%s)",
			func, 2, rv, uxc_errnostr(rv));
		return rv;
	}
	dlgsess->error = ival;

	sval = uxc_dbif_get_str( dbif, 3, &rv); 
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Can't get string value of dbif[%d](err=%d,%s)",
			func, 3, rv, uxc_errnostr(rv));
		return rv;
	}
	dlgsess->serviceid = ux_str_dup( sval, uims_sess_get_allocator(sess)); 

	sval = uxc_dbif_get_str( dbif, 5, &rv); 
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Can't get string value of dbif[%d](err=%d,%s)",
			func, 5, rv, uxc_errnostr(rv));
		return rv;
	}
	dlgsess->recordingfile = ux_str_dup( sval, uims_sess_get_allocator(sess)); 

	ux_log(UXL_DBG1, "%s: ipcmsg cmdId=%d", func, ipcmsg->header.cmdId);
	ux_log(UXL_DBG1, "%s: ipcmsg userData=%d", func, ipcmsg->header.userData);	
	ux_log(UXL_DBG1, "%s: ipcmsg srcSubSysId=%d", func, ipcmsg->header.srcSubSysId);	
	ux_log(UXL_DBG1, "%s: ipcmsg srcProcId=%d", func, ipcmsg->header.srcProcId);	
	ux_log(UXL_DBG1, "%s: ipcmsg srcQid=%d", func, ipcmsg->header.srcQid);	
	ux_log(UXL_DBG1, "%s: ipcmsg result=%d", func, ipcmsg->header.result);	

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL START RECORDING REQUEST(DBIF) ���� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_send_dbif_start_recording_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "clicktocall_dlgsvc_on_send_dbif_start_recording_req";

	int rv;
	char sid[64];
	uxc_ipcmsg_t *ipcmsg;
	uxc_dbif_t *dbif;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "Dont' exist send message instance in session");
		return UX_EINVAL;
	}
	ipcmsg = (uxc_ipcmsg_t*)sndmsg->data;
	dbif = ipcmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn' exist.", func);
		return UX_EINVAL;
	}

	ipcmsg->header.cmdId = uxc_sess_get_id(uxcsess);
	ipcmsg->header.userData = uxc_sess_get_id(uxcsess);

	uxc_dbif_set_str(dbif, 0, dlgsess->sessionid);
	sprintf( sid, "%llu", (unsigned long long)uims_sess_get_id(sess));
	uxc_dbif_set_str(dbif, 1, sid);
	uxc_dbif_set_int(dbif, 2, 1); //voip
	uxc_dbif_set_str(dbif, 3, dlgsess->callingnumber);
	uxc_dbif_set_str(dbif, 4, dlgsess->callednumber);
	uxc_dbif_set_int(dbif, 5, 1); // service code
	uxc_dbif_set_int(dbif, 6, dlgsess->ringbacktonetype);
	uxc_dbif_set_int(dbif, 7, dlgsess->watitngmentid);
	uxc_dbif_set_int(dbif, 8, 0);
	uxc_dbif_set_int(dbif, 9, dlgsess->callmentid);
	uxc_dbif_set_str(dbif, 10, dlgsess->callingcid);
	uxc_dbif_set_str(dbif, 11, dlgsess->calledcid);
	uxc_dbif_set_str(dbif, 12, dlgsess->recordingfile);
	switch (dlgsess->ringbacktonetype) {
		case 1: uxc_dbif_set_int(dbif, 13, 1); break;
		case 2: uxc_dbif_set_int(dbif, 13, 0); break;
		default:
			uxc_trace(UXCTL(1,MAJ), "%s: Invalid recording type.", func);
			return UX_EINVAL;
	}
	uxc_dbif_set_int(dbif, 14, 1); 
	uxc_dbif_set_int(dbif, 15, 1);
	uxc_dbif_set_int(dbif, 16, dlgsess->hostcode);
	uxc_dbif_set_int(dbif, 17, 0);
	uxc_dbif_set_int(dbif, 18, 0);

	ux_log(UXL_DBG1, "%s completed", func);	
	return UX_SUCCESS;
}

/**
 * @brief CLICKTOCALL START RECORDING RESPONSE(DBIF) ���� �Լ�
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) clicktocall_dlgsvc_on_recv_dbif_start_recording_res( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "clicktocall_dlgsvc_on_recv_dbif_start_recording_res";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	uxc_ipcmsg_t *ipcmsg;
	uxc_dbif_t *dbif;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	int64_t ival;
	const char *sval;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Recv message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}
	ipcmsg = (uxc_ipcmsg_t*)rcvmsg->data;
	dbif = ipcmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);
	dlgsess = (sess) ? uims_sess_get_data( sess) : NULL;
	if( dlgsess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: dialog session doesn't exist.", func);
		return UX_EINVAL;
	}

	ival = uxc_dbif_get_str( dbif, 2, &rv); 
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Can't get int value of dbif[%d](err=%d,%s)",
			func, 2, rv, uxc_errnostr(rv));
		return rv;
	}
	dlgsess->error = ival;

	sval = uxc_dbif_get_str( dbif, 3, &rv); 
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Can't get string value of dbif[%d](err=%d,%s)",
			func, 3, rv, uxc_errnostr(rv));
		return rv;
	}
	dlgsess->serviceid = ux_str_dup( sval, uims_sess_get_allocator(sess)); 

	sval = uxc_dbif_get_str( dbif, 5, &rv); 
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Can't get string value of dbif[%d](err=%d,%s)",
			func, 5, rv, uxc_errnostr(rv));
		return rv;
	}
	dlgsess->recordingfile = ux_str_dup( sval, uims_sess_get_allocator(sess)); 

	ux_log(UXL_DBG1, "%s: ipcmsg cmdId=%d", func, ipcmsg->header.cmdId);
	ux_log(UXL_DBG1, "%s: ipcmsg userData=%d", func, ipcmsg->header.userData);
	ux_log(UXL_DBG1, "%s: ipcmsg srcSubSysId=%d", func, ipcmsg->header.srcSubSysId);
	ux_log(UXL_DBG1, "%s: ipcmsg srcProcId=%d", func, ipcmsg->header.srcProcId);
	ux_log(UXL_DBG1, "%s: ipcmsg srcQid=%d", func, ipcmsg->header.srcQid);
	ux_log(UXL_DBG1, "%s: ipcmsg result=%d", func, ipcmsg->header.result);

	ux_log(UXL_DBG1, "%s completed", func);
	return UX_SUCCESS;
}