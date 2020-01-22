#include <usip/usip_def.h>
#include <uxcutor/uxc_sdm.h>
#include <uxcutor/uxc_sfsm.h>
#include <upa/upa_sippa.h>


/**
 * @brief DIALOG INITIAL REQUEST(INVITE, SUBSCRIBE) 메시지를 전달하기 위해 호출되는 함수
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 */
UX_DECLARE(int) gw_dlgsvc_on_send_outgoing_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_METHOD, PARA_REQURI, PARA_FROM, PARA_TO, PARA_CONTENTTYPE, PARA_BODY};

	static const char *func = "gw_dlgsvc_on_send_outgoing_req";

	int rv, urisize, bodysize;

	usip_uri_t uri[1];
	ux_time_t curtime[1];
	const char *method, *requri, *from, *to, *contenttype;
	uint8_t *body;
	usip_payload_t *payload;
	upa_sipmsg_t *sipmsg;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	uxc_msg_t *sndmsg = uxc_sess_get_sndmsg(uxcsess);
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Send message instance in session doesn't exist.", func);
		return UX_EINVAL;
	}

	ux_time_get_current( curtime);

	sipmsg = (upa_sipmsg_t*)(sndmsg->data);

	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #1")
	
	
	method = uxc_sdmvars_get_str( params, PARA_METHOD, NULL, &rv);
	if( rv < UX_SUCCESS || method == NULL) return rv;

	requri = uxc_sdmvars_get_str( params, PARA_REQURI, NULL, &rv);
	if( rv < UX_SUCCESS || requri == NULL) return rv;

	from = uxc_sdmvars_get_str( params, PARA_FROM, NULL, &rv);
	if( rv < UX_SUCCESS || from == NULL) return rv;

	to = uxc_sdmvars_get_str( params, PARA_TO, NULL, &rv);
	if( rv < UX_SUCCESS || to == NULL) return rv;

	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #2")
	if( uxc_sdmvars_is_set(params, PARA_CONTENTTYPE) ) {
		contenttype = uxc_sdmvars_get_str( params, PARA_CONTENTTYPE, NULL, &rv);
		if( rv < UX_SUCCESS) return rv;
	} else {
		contenttype = NULL;
	}

	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #3")
	if( uxc_sdmvars_is_set(params, PARA_BODY) ) {
		body = uxc_sdmvars_get_oct(params, PARA_BODY, NULL, &bodysize); 
		if( rv < UX_SUCCESS) return rv;
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

	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #4 uri= " USIP_URI_PRINT_FORMAT ", size=%d", USIP_URI_PRINT_ARGS(uri), urisize)

	rv = usip_mobj_make_request( sipmsg->mobj, method, uri, from, to);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to make SIP request. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	ux_time_get_current( curtime);
	rv = usip_mobj_set_cseq( sipmsg->mobj, (curtime->tv_sec * 1000 + curtime->tv_usec / 1000) & 0xFFFFF, sipmsg->mobj->request->method_name);


	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #5")
	if (contenttype != NULL && body != NULL) {
		rv = usip_mobj_set_content_type(sipmsg->mobj, contenttype);
		if( rv < USIP_SUCCESS) {
			return rv;
		}

		payload = usip_payload_create( usip_msg_get_allocator(sipmsg->mobj->base->msg), (void*)body, bodysize);
		if( payload == NULL) {
			ux_log(UXL_MAJ, "Failed to make SIP payload. err=%d,%s)", rv, usip_errstr(bodysize));
			return bodysize;
		}

		rv = usip_msg_add_hdr( sipmsg->mobj->base->msg, payload->base, 0);
		if( rv < USIP_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to make SIP payload. err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}
	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #6")

	rv = usip_mobj_complete_request( sipmsg->mobj);
	if( rv < USIP_SUCCESS) return rv;

	ux_log(UXL_INFO, "gw_dlgsvc_on_send_outgoing_req: #7. request=%p", sipmsg->mobj->request);

	return UX_SUCCESS;
}


