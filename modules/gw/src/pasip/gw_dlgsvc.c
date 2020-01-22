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

	int rv, urilen;
	
	uxc_sessmgr_t *sessmgr = (uxc_sessmgr_t*)params->sdm; 
	uxc_worker_t *worker = (uxc_worker_t*)uxc_sessmgr_get_worker(sessmgr);
	uxc_xcutor_t *xcutor = (uxc_xcutor_t*)uxc_worker_get_xcutor(worker);
	upa_sipsfif_t *sipsfif = (upa_sipsfif_t*) uxc_xcutor_get_pasfif(xcutor, "SIP");
	uxc_sfpainfo_t *painfo = uxc_sfnode_get_painfo( uxc_sfrun_get_owner((uxc_sfrun_t*)sfcall));
	upa_sipmbi_t *mbi = (upa_sipmbi_t*)painfo->buildinfo;
	uxc_sess_t *uxcsess = (uxc_sess_t*)params->sdm->impl;
	upa_sipmsg_t *sipmsg;
	usip_uri_t uri[1];
	const char *method, *requri, *from, *to, *contenttype, *body;
	

	sipmsg = upa_sipmsg_create( upa_sipmbi_get_mflags(mbi));
	if( sipmsg == NULL ) {
		return  UX_EINVAL;
	}

	if(uxcsess) sipmsg->sessinfo->sessid = uxc_sess_get_id(uxcsess);
	else sipmsg->sessinfo->sessid = 0;

	sipmsg->sessinfo->did = upa_sipmbi_get_did(mbi);
	sipmsg->uxcmsg->paif = upa_sipsfif_get_paif(sipsfif);

	
	method = uxc_sdmvars_get_str( params, PARA_METHOD, NULL, &rv);
	if( rv < UX_SUCCESS || method == NULL) return rv;

	requri = uxc_sdmvars_get_str( params, PARA_REQURI, NULL, &rv);
	if( rv < UX_SUCCESS || requri == NULL) return rv;

	from = uxc_sdmvars_get_str( params, PARA_FROM, NULL, &rv);
	if( rv < UX_SUCCESS || from == NULL) return rv;

	to = uxc_sdmvars_get_str( params, PARA_TO, NULL, &rv);
	if( rv < UX_SUCCESS || to == NULL) return rv;

	contenttype = uxc_sdmvars_get_str( params, PARA_CONTENTTYPE, NULL, &rv);
	if( rv < UX_SUCCESS || contenttype == NULL) return rv;

	body = uxc_sdmvars_get_str( params, PARA_BODY, NULL, &rv);
	if( rv < UX_SUCCESS || body == NULL) return rv;

	urilen = strlen( requri);
	usip_uri_init( uri);
	rv = usip_uri_decode( uri, requri, urilen); 
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_INFO, "Failed to decode SIP URI. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}

	rv = usip_mobj_make_request( sipmsg->mobj, method, requri, from, to);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_INFO, "Failed to make SIP request. err=%d,%s)", rv, usip_errstr(rv));
		return rv;
	}




	

	return UX_SUCCESS;
}


