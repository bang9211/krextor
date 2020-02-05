
#include <uxcutor/uxc_sdm.h>
#include <uxcutor/uxc_sfsm.h>
#include "upa/upa_httpcall.h"
#include "upa/upa_httpmsg.h"
#include "gw/gw_error.h"


/**
 * @brief DIALOG INITIAL REQUEST(INVITE, SUBSCRIBE) 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) gw_http_dlgsvc_on_send_clicktocall_noti_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_NAME, PARA_IDX, PARA_VALUE };

	int rv, idx, olen;
	int64_t ival;
	double dval;
	uint8_t *oval;
	const char *name, *sval;
	uxc_sess_t *sess;
	uxc_msg_t *sndmsg;
	uhttp_hdrs_t *hdrs;
	upa_httpmsg_t *httpmsg;

	sess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( sess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Dont' exist send message instance in session");
		return UX_EINVAL;
	}

	if( uxc_sdmvars_is_set(params, PARA_NAME) ) {
		name = uxc_sdmvars_get_str( params, PARA_NAME, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to get header name parameter");
			return rv;
		}
	} else {
		name = NULL;
	}
	if( name == NULL || name[0] == '\0') {
		uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Invalid header name parameter");
		return UX_EINVAL;
	}
	if( uxc_sdmvars_is_set(params, PARA_IDX) ) {
		idx = uxc_sdmvars_get_int( params, PARA_IDX, 0, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to get header index parameter");
			return rv;
		}
	} else {
		idx = 0;
	}

	if( !uxc_sdmvars_is_set(params, PARA_VALUE) ) {
		uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Invalid header value parameter");
		return UX_EINVAL;
	}

	httpmsg = (upa_httpmsg_t*)sndmsg->data;
	hdrs = uhttp_msg_get_hdrs( httpmsg->msg);

	if( uxc_sdmvars_is_int( params, PARA_VALUE)) {
		ival = uxc_sdmvars_get_int( params, PARA_VALUE, 0, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to get header value parameter. (name=%s[%d], err=%d,%s)",
					name, idx, rv, ux_errnostr(rv));
			return rv;
		}
		rv = uhttp_hdrs_set_int( hdrs, name, idx, ival);
		if( rv < UHTTP_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to set header value. (name=%s[%d] ival=%lld, err=%d,%s)",
					name, idx, (long long)ival, rv, uhttp_errstr(rv));
			return UX_EINVAL;
		}
	} else if( uxc_sdmvars_is_float( params, PARA_VALUE)) {
		dval = uxc_sdmvars_get_float( params, PARA_VALUE, 0, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to get header value parameter. (name=%s[%d], err=%d,%s)",
					name, idx, rv, ux_errnostr(rv));
			return rv;
		}
		rv = uhttp_hdrs_set_double( hdrs, name, idx, dval);
		if( rv < UHTTP_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to set header value. (name=%s[%d], dval=%f, err=%d,%s)",
					name, idx, dval, rv, uhttp_errstr(rv));
			return UX_EINVAL;
		}
	} else if( uxc_sdmvars_is_str( params, PARA_VALUE)) {
		sval = uxc_sdmvars_get_str( params, PARA_VALUE, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to get header value parameter. (name=%s, err=%d,%s)",
					name, rv, ux_errnostr(rv));
			return rv;
		}
		rv = uhttp_hdrs_set_str( hdrs, name, idx, sval);
		if( rv < UHTTP_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to set header value. (name=%s[%d], sval=%s, err=%d,%s)",
					name, idx, sval, rv, uhttp_errstr(rv));
			return UX_EINVAL;
		}
	} else {
		oval = uxc_sdmvars_get_oct(params, PARA_VALUE, NULL, &olen); 
		if( oval == NULL) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to get header value parameter. (name=%s[%d], err=%d,%s)",
					name, idx, olen, ux_errnostr(olen));
			return olen;
		}
		rv = uhttp_hdrs_set_oct( hdrs, name, idx, oval, olen);
		if( rv < UHTTP_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "gw_http_dlgsvc_on_send_clicktocall_noti_req: Failed to set header value. (name=%s[%d], oval=%p,%d, err=%d,%s)",
					name, idx, oval,olen, rv, uhttp_errstr(rv));
			return UX_EINVAL;
		}
	}

	return UX_SUCCESS;
}

