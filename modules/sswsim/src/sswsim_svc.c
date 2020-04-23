#include "uims/uims_sipmsg.h"
#include "uims/uims_sess.h"
#include "local/sswsim_svc_l.h"

/**
 * @brief REGISTER 메시지로 부터 REGISTER를 위한 ID들을 반환한다.  
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
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

	//TODO 일정 시간 동안 수신된 동일한 Call-ID및 From 정보에 대한 동일 dialog 사용 기능 구현 필요

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

    // contact list 등록
	for( cthdr = mobj->contact; cthdr; cthdr = (usip_contact_hdr_t*)cthdr->base->next ) {
		/* MYSQL 에 contact 등록 */
	}
	
	return USIP_SUCCESS;
}

/**
 * @brief 서비스에서 응답을 전달할 경우 
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
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
 * @brief 서비스에서 오류 발생 시 REGISTER 응답을 만든다. 
 * @param sfcall call fsm node
 * @param params sdm value parameter
 * @return 실행 결과
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

/**
 * @internal SSWSIM regist DAO?? ???????.
 * @param dao SSWSIM regist DAO
 * @param routermgr router manager
 * @return ???? ???
 */
ux_status_t sswsim_dao_init( sswsim_dao_t *dao, uims_dbmgr_t *dbmgr)
{
	dao->dbmgr = dbmgr;
	dao->db = uims_dbmgr_get_n( dbmgr, "TNTDB");
	if(dao->db == NULL) {
		ux_log(UXL_MAJ, "Failed to get TNTDB");
		return UX_ENOENT;
	}

	return UX_SUCCESS;
}

void sswsim_dao_final( sswsim_dao_t *dao)
{
	//Do Nonthing
}

sswsim_regist_t* sswsim_dao_find( sswsim_dao_t *dao, char *aor)
{
	static const char* stmtid = "SSWSIM:FIND";
	static const char* query = "SELECT * FROM 1 WHERE 0=?";

	int rv;
	uint8_t regstatus;
	uint32_t expire;
	uint64_t regdate;
	char *publicuid, *privateuid, *homenetdomain, *contact1, *contact2, *contact3, *contact4, *contact5;
	ux_mem_t *allocator;
	uxc_sesshdr_t *sesshdr;
	uims_sess_t *sess;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset, *rsltset;
	char tag[128];

	sswsim_regist_t registinfo;

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
			"publicuid", UIMS_DBTYPE_STR, aor);
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
		ux_log(UXL_INFO, "Failed to find regist info. publicuid=%s, err=%d,%s", publicuid, rv, uxc_errnostr(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	rv = uims_dbdataset_read( rsltset, 11,
			//name, type, value, [length:octet only]
			"publicuid", UIMS_DBTYPE_STR, &publicuid,
			"privateuid", UIMS_DBTYPE_STR, &privateuid,
			"homenetdomain", UIMS_DBTYPE_STR, &homenetdomain,
			"regdate", UIMS_DBTYPE_UINT64, &regdate,
			"regstatus", UIMS_DBTYPE_UINT8, &regstatus,
			"expire", UIMS_DBTYPE_UINT32, &expire, 
			"contact1", UIMS_DBTYPE_STR, &contact1,
			"contact2", UIMS_DBTYPE_STR, &contact2,
			"contact3", UIMS_DBTYPE_STR, &contact3,
			"contact4", UIMS_DBTYPE_STR, &contact4,
			"contact5", UIMS_DBTYPE_STR, &contact5
			);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to read value from result set. (stmtid=%s, err=%d,%s)",
				stmtid, rv, uims_dberr_to_str(rv));
		uims_dbstmt_close( stmt);
		return NULL;
	}

	ux_log(UXL_INFO, "[SSWSIM:FIND] (publicuid=%s, privateuid=%s, homenetdomain=%s, regdate=%llu, regstatus=%d, expire=%u, "
			"contact1=%s, contact2=%s, contact3=%s, contact4=%s, contact5=%s)",
			publicuid, privateuid, homenetdomain, (unsigned long long)regdate, regstatus, expire, 
			contact1 ? contact1 : "NULL", contact2 ? contact2 : "NULL", contact3 ? contact3 : "NULL", 
			contact4 ? contact4 : "NULL", contact5 ? contact5 : "NULL");

	strcpy(registinfo.privateuid, publicuid);
	if (privateuid != NULL) strcpy(registinfo.privateuid, privateuid);
	if (homenetdomain != NULL) strcpy(registinfo.homenetdomain, homenetdomain);
	registinfo.regdate = regdate;
	registinfo.regstatus = regstatus;
	registinfo.expire = expire;
	if (contact1 != NULL) strcpy(registinfo.contact1, contact1);
	if (contact2 != NULL) strcpy(registinfo.contact2, contact2);
	if (contact3 != NULL) strcpy(registinfo.contact3, contact3);
	if (contact4 != NULL) strcpy(registinfo.contact4, contact4);
	if (contact5 != NULL) strcpy(registinfo.contact5, contact5);

	uims_dbstmt_close( stmt);
	
	return &registinfo;
}

ux_status_t sswsim_dao_insert( sswsim_dao_t *dao, sswsim_regist_t *registinfo)
{
	static const char* stmtid = "SSWSIM:INSERT";
	static const char* query = "INSERT INTO table1 VALUES(?,?,?,?,?, ?,?,?,?,?, ?)";

	int rv;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	registinfo->regdate = time(NULL);

	ux_log(UXL_INFO, "SSWSIM:INSERT. (publicuid=%s)", registinfo->publicuid);

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

	rv = uims_dbdataset_write( paraset, 11,
			//name, type, value, [length:octet only]
			"publicuid", UIMS_DBTYPE_STR, registinfo->publicuid,
			"privateuid", UIMS_DBTYPE_STR, registinfo->privateuid ? registinfo->privateuid : "",
			"homenetdomain", UIMS_DBTYPE_STR, registinfo->homenetdomain ? registinfo->homenetdomain : "",
			"regdate", UIMS_DBTYPE_UINT64, registinfo->regdate,
			"regstatus", UIMS_DBTYPE_UINT8, registinfo->regstatus,
			"expire", UIMS_DBTYPE_UINT32, registinfo->expire,
			"contact1", UIMS_DBTYPE_STR, registinfo->contact1,
			"contact2", UIMS_DBTYPE_STR, registinfo->contact2,
			"contact3", UIMS_DBTYPE_STR, registinfo->contact3,
			"contact4", UIMS_DBTYPE_STR, registinfo->contact4,
			"contact5", UIMS_DBTYPE_STR, registinfo->contact5);
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

ux_status_t sswsim_dao_remove( sswsim_dao_t *dao, sswsim_regist_t *registinfo)
{
	static const char* stmtid = "SSWSIM:REMOVE";
	static const char* query = "DELETE FROM table1 WHERE index0=?";

	int rv;
	uims_dbstmt_t *stmt;
	uims_dbdataset_t *paraset;

	ux_log(UXL_INFO, "SSWSIM:REMOVE. publicuid=%s", registinfo->publicuid);

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
			"publicuid", UIMS_DBTYPE_STR, registinfo->publicuid);
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