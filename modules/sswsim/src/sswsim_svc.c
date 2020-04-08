#include "uims/uims_sipmsg.h"
#include "uims/uims_sess.h"

/**
 * @brief REGISTER �޽����� ���� REGISTER�� ���� ID���� ��ȯ�Ѵ�.  
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 */
UX_DECLARE(int) sswsim_regsvc_on_recv_register( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "sswsim_regsvc_on_recv_register";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *rcvmsg;
	upa_sipmsg_t *reqmsg;
	uims_sess_t *imssess;
    uint64_t expires;
    uint8_t auth_type;
    usip_mobj_t *mobj;
    usip_contact_hdr_t *cthdr;

	//TODO ���� �ð� ���� ���ŵ� ������ Call-ID�� From ������ ���� ���� dialog ��� ��� ���� �ʿ�

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Not exists recv message instance in session.", func);
		return UX_EINVAL;
	}
	reqmsg = (upa_sipmsg_t*)rcvmsg->data;
    mobj = reqmsg->mobj;

	if( reqmsg->mobj->max_forwards && reqmsg->mobj->max_forwards->value <= 0) {
		ux_log( UXL_INFO, "%s: Can't process message. Exceeds max_forwards. "
				"(method=%s, call_id=%s, from=%s:%s, to=%s:%s, max_forward=%llu)",
				func, usip_mobj_get_method( reqmsg->mobj), USIP_MOBJ_GET_CALLID( reqmsg->mobj),
				USIP_MOBJ_GET_FROMUSER( reqmsg->mobj), USIP_MOBJ_GET_FROMTAG( reqmsg->mobj),
				USIP_MOBJ_GET_TOUSER( reqmsg->mobj), USIP_MOBJ_GET_TOTAG( reqmsg->mobj),
				(unsigned long long)reqmsg->mobj->max_forwards->value);
		return UX_EINVAL;
	}

	imssess = uxc_sess_get_user_data( uxcsess);
	if( imssess == NULL) {
		uxc_trace(UXCTL(1,MAJ), "%s: IMS session instance in uxcutor session doesn't exist.", func);
		return UX_EINVAL;
	}


    if( mobj->contact == NULL || mobj->path == NULL) return eUXC_INVALID_PARAM;

    expires = uims_sipmsg_get_expires( reqmsg->mobj); 
    auth_type = (expires) ? UIMS_AUTHTYPE_REG : UIMS_AUTHTYPE_DEREG;

    // contact list ���
	for( cthdr = mobj->contact; cthdr; cthdr = (usip_contact_hdr_t*)cthdr->base->next ) {
		/* MYSQL �� contact ��� */
	}
	
	return USIP_SUCCESS;
}

/**
 * @brief ���񽺿��� ������ ������ ��� 
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 * @remark
 */
UX_DECLARE(int) sswsim_regsvc_on_forward_response( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "sswsim_regsvc_on_forward_response";

	int rv;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg, *rcvmsg;
	upa_sipmsg_t *fwdmsg, *rspmsg;
	uims_sess_t *sess;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Not exists send message instance in session", func);
		return UX_EINVAL;
	}
	fwdmsg = (upa_sipmsg_t*)sndmsg->data;

	rcvmsg = uxc_sess_get_rcvmsg( uxcsess);
	if( rcvmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Not exists recv message instance in session", func);
		return UX_EINVAL;
	}
	rspmsg = (upa_sipmsg_t*)rcvmsg->data;

	sess = uxc_sess_get_user_data( uxcsess);

	return UX_SUCCESS;
}
	
/**
 * @brief ���񽺿��� ���� �߻� �� REGISTER ������ �����. 
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return ���� ���
 * @remark
 */
UX_DECLARE(int) sswsim_regsvc_on_send_error( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params)
{
	static const char *func = "sswsim_regsvc_on_send_error";

	int rv, err;
	uxc_sess_t *uxcsess;
	uxc_msg_t *sndmsg, *rcvmsg;
	upa_sipmsg_t *reqmsg, *rspmsg;
	uims_sess_t *sess;

	uxcsess = (uxc_sess_t*)params->sdm->impl;
	sndmsg = uxc_sess_get_sndmsg( uxcsess);
	if( sndmsg == NULL ) {
		uxc_trace(UXCTL(1,MAJ), "%s: Not exists send message instance in session", func);
		return UX_EINVAL;
	}
	rspmsg = (upa_sipmsg_t*)(sndmsg->data);

	sess = uxc_sess_get_user_data( uxcsess);


	rv = usip_mobj_make_response( rspmsg->mobj, sswsim_err_to_rcode( err),
					sswsim_err_to_phrase(err), rspmsg->mobj);

	if( rv < USIP_SUCCESS) {
		uxc_trace(UXCTL(1,MAJ), "%s: Failed to make SIP response. (status=%d,%s, err=%d,%s)",
				func, err, sswsim_err_to_phrase(err), rv, usip_errstr(rv));
		return rv;
	}

	return UX_SUCCESS;
}
