#include "uims/uims_xcpt.h"
#include "local/uims_plugin_l.h"

/**
 * @brief UIMS DIAMETER 메시지 기본 정보를 설정한다. 
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 * @remark
 * para[0] = sess_id 
 * para[1] = Origin-Realm 
 * para[2] = Origin-Host 
 * para[3] = Destination-Realm 
 * para[4] = Destination-Host 
 */
UX_DECLARE(int) uims_xcpt_init_httpmsg( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	/*
	enum { PARA_SESSID, PARA_OREALM, PARA_OHOST, PARA_DREALM, PARA_DHOST };

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg;
	udia_avplist_t *avplist;
	upa_diamsg_t *diamsg;
	uims_sess_t *sess;
	const char *sess_id, *orealm, *ohost, *drealm, *dhost;
	char sidbuf[512];

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sess = (uxcsess) ? (uims_sess_t*)uxc_sess_get_user_data( uxcsess) : NULL;
	if( sess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_init_httpmsg: "
				"Failed to get IMS session");
		return UX_EINVAL;
	}

	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_init_httpmsg: "
				"Dont' exist send message instance in session");
		return UX_EINVAL;
	}

	diamsg = (upa_diamsg_t*)sndmsg->data;

	if( uxc_sdmvars_is_set(params, PARA_OREALM) ) {
		orealm = uxc_sdmvars_get_str( params, PARA_OREALM, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Failed to get OREALM parameter");
			return rv;
		}
	} else {
		orealm = uims_plugin_get_dia_realm( uims_plugin_instance());
	}
	if( orealm == NULL || orealm[0] == '\0') {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Invalid OREALM parameter");
		return UX_EINVAL;
	}
	if( uxc_sdmvars_is_set(params, PARA_OHOST) ) {
		ohost = uxc_sdmvars_get_str( params, PARA_OHOST, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Failed to get OHOST parameter");
			return rv;
		}
	} else {
		ohost = uims_plugin_get_dia_host( uims_plugin_instance());
	}
	if( ohost == NULL || ohost[0] == '\0') {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Invalid OHOST parameter");
		return UX_EINVAL;
	}
	if( uxc_sdmvars_is_set(params, PARA_SESSID) ) {
		sess_id = uxc_sdmvars_get_str( params, PARA_SESSID, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Failed to get SESSID parameter");
			return rv;
		}
	} else {
		snprintf(sidbuf, sizeof(sidbuf), "%s;%lx;%llx",
				ohost, (long)time(NULL), (unsigned long long)sess->id); 
		sess_id = sidbuf;
	}
	if( sess_id == NULL || sess_id[0] == '\0') {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Invalid sess_id parameter");
		return UX_EINVAL;
	}
	if( uxc_sdmvars_is_set(params, PARA_DREALM) ) {
		drealm = uxc_sdmvars_get_str( params, PARA_DREALM, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Failed to get DHOST parameter");
			return rv;
		}
	} else {
		drealm = NULL;
	}
	if( uxc_sdmvars_is_set(params, PARA_DHOST) ) {
		dhost = uxc_sdmvars_get_str( params, PARA_DHOST, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: Failed to get DREALM parameter");
			return rv;
		}
	} else {
		dhost = NULL;
	}

	avplist = udia_msg_get_root( diamsg->stackmsg);

	rv = udia_avplist_set_str( avplist, UDIA_AVP_SESSION_ID, 0, sess_id);
	if( rv < UDIA_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: "
				"Failed to set Session-Id AVP (value=%s, err=%d,%s)",
				sess_id, rv, udia_errstr(rv));
		return rv;
	}
	rv = udia_avplist_set_str( avplist, UDIA_AVP_ORIGIN_REALM, 0, orealm);
	if( rv < UDIA_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: "
				"Failed to set Origin-Realm AVP (value=%s, err=%d,%s)",
				sess_id, rv, udia_errstr(rv));
		return rv;
	}
	rv = udia_avplist_set_str( avplist, UDIA_AVP_ORIGIN_HOST, 0, ohost);
	if( rv < UDIA_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: "
				"Failed to set Origin-Realm AVP (value=%s, err=%d,%s)",
				sess_id, rv, udia_errstr(rv));
		return rv;
	}
	if( drealm) {
		rv = udia_avplist_set_str( avplist, UDIA_AVP_DESTINATION_REALM, 0, orealm);
		if( rv < UDIA_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: "
					"Failed to set Destination-Realm AVP (value=%s, err=%d,%s)",
					sess_id, rv, udia_errstr(rv));
			return rv;
		}
	}
	if( dhost) {
		rv = udia_avplist_set_str( avplist, UDIA_AVP_DESTINATION_HOST, 0, ohost);
		if( rv < UDIA_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_init_httpmsg: "
					"Failed to set Destination-Host AVP (value=%s, err=%d,%s)",
					sess_id, rv, udia_errstr(rv));
			return rv;
		}
	}
	*/

	return UX_SUCCESS;
}

/**
 * @brief UIMS CDR 시작
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 * @remark
 * para[0] = cdr name 
 * para[1] = in peer 
 */
UX_DECLARE(int) uims_xcpt_cdr_start( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_CDR, PARA_INPEER };

	int rv;
	uxc_msg_t *rcvmsg;
	uxc_sess_t *uxcsess;
	uims_sess_t *sess;
	upa_sipmsg_t *sipmsg;
	uims_cdrmgr_t *cdrmgr;
	uims_plugin_t *plugin;
	uims_cdrinfo_t *cdrinfo;
	const char *cdrname, *inpeer;

	plugin = uims_plugin_instance();
	if( plugin == NULL) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_start: Failed to get IMS plugin");
		return UX_EINVAL;
	}

	cdrmgr = plugin->cdrmgr;
	if( cdrmgr == NULL) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_start: Not found IMS cdr manager");
		return UX_EINVAL;
	}

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sess = (uxcsess) ? (uims_sess_t*)uxc_sess_get_user_data( uxcsess) : NULL;
	if( sess == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_start: "
				"Failed to get IMS session");
		return UX_EINVAL;
	}

	if( !uxc_sdmvars_is_set( params, PARA_CDR) ) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_start: Not found to get CDR_NAME parameter");
		return UX_EINVAL;
	}

	cdrname = uxc_sdmvars_get_str( params, PARA_CDR, NULL, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_start: Failed to get CDR_NAME parameter");
		return rv;
	}

	if( sess->cdrbuff == NULL) {
		cdrinfo = uims_cdrmgr_get_info( cdrmgr, (char*) cdrname);
		if( cdrinfo == NULL) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_start: Failed to get IMS cdr info. (cdr=%s)", cdrname);
			return UX_EINVAL;
		}

		sess->cdrbuff = uims_cdrbuff_create( cdrinfo);
		if( sess->cdrbuff == NULL) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_start: Failed to create IMS cdr buff. (cdr=%s)", cdrname);
			return UX_EINVAL;
		}
	}

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_start: Don't exist recv message instance in session");
		return UX_EINVAL;
	}
	sipmsg = (upa_sipmsg_t*)rcvmsg->data;

	if( uxc_sdmvars_is_set( params, PARA_INPEER) ) {
		inpeer = uxc_sdmvars_get_str( params, PARA_INPEER, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_start: Failed to get in_peer parameter");
			return rv;
		}
	} else {
		inpeer = NULL;
	}

	uims_cdrbuff_start( sess->cdrbuff, sipmsg->mobj->base->msg, (char*)inpeer);

	return UX_SUCCESS;
}

/**
 * @brief UIMS CDR 변경
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 * @remark
 * para[0] = out peer 
 */
UX_DECLARE(int) uims_xcpt_cdr_update( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_OUTPEER };

	int rv;
	uxc_msg_t *sndmsg, *rcvmsg;
	uxc_sess_t *uxcsess;
	uims_sess_t *sess;
	upa_sipmsg_t *sipmsg;
	usip_msg_t *req, *res;
	const char *outpeer;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sess = (uxcsess) ? (uims_sess_t*)uxc_sess_get_user_data( uxcsess) : NULL;
	if( sess == NULL) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_update: Failed to get IMS session");
		return UX_EINVAL;
	}

	if( sess->cdrbuff == NULL) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_update: Not found cdrbuff in session");
		return UX_EINVAL;
	}

	req = NULL;
	res = NULL;

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg != NULL ) {
		sipmsg = (upa_sipmsg_t*)rcvmsg->data;
		if( usip_mobj_is_request( sipmsg->mobj)) {
			req = sipmsg->mobj->base->msg;
		} else {
			res = sipmsg->mobj->base->msg;
		}
	}

	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg != NULL ) {
		sipmsg = (upa_sipmsg_t*)sndmsg->data;
		if( usip_mobj_is_request( sipmsg->mobj)) {
			req = sipmsg->mobj->base->msg;
		} else {
			res = sipmsg->mobj->base->msg;
		}
	}

	if( uxc_sdmvars_is_set( params, PARA_OUTPEER) ) {
		outpeer = uxc_sdmvars_get_str( params, PARA_OUTPEER, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_update: Failed to get out_peer parameter");
			return rv;
		}
	} else {
		outpeer = NULL;
	}

	uims_cdrbuff_update( sess->cdrbuff, req, res, (char*)outpeer);

	return UX_SUCCESS;
}

/**
 * @brief UIMS CDR 종료
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
 * @remark
 * para[0] = cdr name 
 * para[1] = in peer 
 */
UX_DECLARE(int) uims_xcpt_cdr_end( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	enum { PARA_OUTPEER, PARA_ISSEND };

	int rv, issend;
	uxc_msg_t *uxcmsg;
	uxc_sess_t *uxcsess;
	uims_sess_t *sess;
	upa_sipmsg_t *sipmsg;
	const char *outpeer;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sess = (uxcsess) ? (uims_sess_t*)uxc_sess_get_user_data( uxcsess) : NULL;
	if( sess == NULL) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_end: Failed to get IMS session");
		return UX_EINVAL;
	}

	if( sess->cdrbuff == NULL) {
		uxc_trace(UXCTL(1,MAJ), "uims_xcpt_cdr_end: Not found cdrbuff in session");
		return UX_EINVAL;
	}

	if( !uxc_sdmvars_is_set( params, PARA_ISSEND) ) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_end: Not found to get IS_SEND parameter");
		return UX_EINVAL;
	}

	issend = uxc_sdmvars_get_int( params, PARA_ISSEND, 0, &rv);
	if( rv < UX_SUCCESS) {
		uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_end: Failed to get IS_SEND parameter");
		return rv;
	}

	if( issend == 0) {
		uxcmsg = uxc_sess_get_rcvmsg( uxcsess);
	} else {
		uxcmsg = uxc_sess_get_sndmsg( uxcsess);
	}
	if( uxcmsg != NULL) {
		sipmsg = (upa_sipmsg_t*)uxcmsg->data;
	} else {
		sipmsg = NULL;
	}

	if( uxc_sdmvars_is_set( params, PARA_OUTPEER) ) {
		outpeer = uxc_sdmvars_get_str( params, PARA_OUTPEER, NULL, &rv);
		if( rv < UX_SUCCESS) {
			uxc_trace(UXCTL(1,MIN), "uims_xcpt_cdr_update: Failed to get out_peer parameter");
			return rv;
		}
	} else {
		outpeer = NULL;
	}

	uims_cdrbuff_end( sess->cdrbuff, sipmsg? sipmsg->mobj->base->msg:NULL, (char*)outpeer, issend);

	return UX_SUCCESS;
}

