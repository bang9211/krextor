#include "local/uims_plugin_l.h"
// #include "uims/uims_tntdb.h"

static uims_plugin_t *_g_plugin = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////
// functions for icsf_plugin_t 
//////////////////////////////////////////////////////////////////////////////////////////////
static void _uims_plugin_destroy( uxc_plugin_t *pi);
static int _uims_plugin_start( uxc_plugin_t *pid);
static int _uims_plugin_reload( uxc_plugin_t *pi, void *pworker);
static int _uims_plugin_reconf( uxc_plugin_t *pi);
static int _uims_plugin_get_sip_thrid( uxc_paif_t *paif, uxc_msg_t *msg);
static int _uims_plugin_get_http_thrid( uxc_paif_t *paif, uxc_msg_t *msg);
static uxc_msg_t* _uims_plugin_create_sipmsg( uxc_pasfif_t *pasfif, uxc_sfnode_t *sfnode, uxc_sdm_t *sdm);

/**
 * @brief UIMS 공통 plugin을 instance를 반환한다
 * @return UIMS 공통 plugin 
 */
UX_DECLARE(uims_plugin_t*) uims_plugin_instance()
{
	return _g_plugin;
}

/**
 * @brief UIMS plugin을 생성한다.
 * @param xcutor uxcutor instance
 * @param cfile configuration file name
 * @return 생성된 UIMS plugin
 */
UX_DECLARE(uxc_plugin_t*) uims_plugin_create( void *xcutor, const char *cfile)
{
	int rv;
	uims_plugin_t *plugin;

	plugin = (uims_plugin_t*)ux_zalloc( ux_mem_default(), sizeof(uims_plugin_t));
	if( plugin == NULL) {
		ux_log(UXL_CRT, "Can't allocate memory for uims_plugin_t.");
		return NULL;
	}

	rv = uims_plugin_init( plugin, (uxc_xcutor_t*)xcutor, cfile);
	if( rv < UX_SUCCESS) {
		ux_free( ux_mem_default(), plugin);
		return NULL;
	}

	ux_log( UXL_INFO, "uims plugin created.");

	_g_plugin = plugin;
	return plugin->base;
}

/**
 * @internal UIMS plugin을 초기화한다.
 * @param plugin UIMS plugin
 * @param xcutor uxcutor
 * @param cfile configuration file name
 * @return 실행 결과
 */
ux_status_t uims_plugin_init( uims_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile)
{
	int rv;
	upa_sippa_t *sippa;
	upa_httppa_t *httppa;
	uxc_pasfif_t *pasfif;

	sippa = (upa_sippa_t*)uxc_xcutor_get_paif( xcutor, "PA_SIP");
	if( sippa == NULL) {
		ux_log(UXL_CRT, "Can't find Protocol Adapter PA_SIP.");
		return UX_ENOENT;
	}
	httppa = (upa_httppa_t*)uxc_xcutor_get_paif( xcutor, "PA_HTTP");
	if( httppa == NULL) {
		ux_log(UXL_INFO, "Can't find Protocol Adapter PA_HTTP.");
		return UX_ENOENT;
	}

	rv = uxc_plugin_init( plugin->base, xcutor, cfile);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_ERR, "Failed to uxc_plugin_init. (err=%d,%s)", rv, ux_errnostr(rv));
		return rv;
	}

	// initiate & load plugin configuration
	rv = uims_conf_init( plugin->conf, cfile);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_ERR, "Failed to uims_conf_init. (err=%d,%s)", rv, ux_errnostr(rv));
		uxc_plugin_final( plugin->base);
		return rv;
	}

	plugin->conf->xcutor = xcutor;
	plugin->conf->sippa = sippa;
	plugin->conf->httppa = httppa;
	plugin->sesscdata->conf = plugin->conf;
	plugin->sesscdata->sessdb = NULL;
	plugin->base->destroy = _uims_plugin_destroy;
	plugin->base->start = _uims_plugin_start;
	plugin->base->reconf = _uims_plugin_reconf;
	plugin->base->reload = _uims_plugin_reload;
	plugin->base->heartbeat = NULL;

	uxc_paif_set_thrid_func((uxc_paif_t*)sippa, _uims_plugin_get_sip_thrid);
	uxc_paif_set_thrid_func((uxc_paif_t*)httppa, _uims_plugin_get_http_thrid);
	pasfif = uxc_xcutor_get_pasfif( xcutor, "SIP");
	if( pasfif ) pasfif->create_msg = _uims_plugin_create_sipmsg;

	plugin->dbmgr = uims_dbmgr_create( plugin->conf->max_db);
	if( !plugin->dbmgr) {
		ux_log( UXL_ERR, "Failed to create DB manager. (err=%d,%s)", rv, ux_errnostr(rv));
		uxc_plugin_final( plugin->base);
		return rv;
	}

	uims_dbmgr_load( plugin->dbmgr, cfile, "[UXCUTOR/PLUGIN/UIMS/DB]");

	plugin->cdrmgr = uims_cdrmgr_create( cfile);
	if( plugin->cdrmgr == NULL) {
		ux_log( UXL_INFO, "Not support cdr.");
	} else {
		rv = uims_cdrmgr_load( plugin->cdrmgr);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_ERR, "Failed to load cdr manager. (err=%d,%s)", rv, ux_errnostr(rv));
			uxc_plugin_final( plugin->base);
			return rv;
		}
	}

	return UX_SUCCESS;
}

/**
 * @internal UIMS plugin의 사용을 종료한다.
 * @param plugin UIMS plugin
 */
void uims_plugin_final( uims_plugin_t *plugin)
{
	int i;
	uxc_worker_t *worker;
	uxc_sessmgr_t *uxcsessmgr;
	uims_sessmgr_t *sessmgr;

	for( i = uxc_xcutor_get_worker_count( plugin->conf->xcutor); i >=0; --i) {
		worker = uxc_xcutor_get_worker( plugin->conf->xcutor, i);
		if( worker == NULL) continue;
		uxcsessmgr = uxc_worker_get_sessmgr( worker);
		if( uxcsessmgr == NULL) continue;
		sessmgr = uxc_sessmgr_get_user_data( uxcsessmgr);	
		if( sessmgr == NULL) continue;
		uims_sessmgr_destroy( sessmgr);
	}

	if( plugin->cdrmgr) uims_cdrmgr_destroy( plugin->cdrmgr);

	uims_conf_final( plugin->conf);
	uxc_plugin_final( plugin->base);
}

/**
 * @internal UIMS plugin을 소멸한다.
 * @param pi UIMS plugin
 */
static void _uims_plugin_destroy( uxc_plugin_t *pi)
{
	uims_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _uims_plugin_destroy.");

	plugin = (uims_plugin_t*)pi;
	uims_plugin_final( plugin);
	ux_free( ux_mem_default(), plugin);
}

/**
 * @internal UIMS plugin의 구동을 시작시 수행할 함수
 * @param pi UIMS plugin
 * @return 실행 결과
 */
static int _uims_plugin_start( uxc_plugin_t *pi)
{
	int i;
	uxc_worker_t *worker;
	uxc_sessmgr_t *uxcsessmgr;
	uims_sessmgr_t *sessmgr;
	uims_plugin_t *plugin;

	plugin = (uims_plugin_t*)pi;
	for( i = uxc_xcutor_get_worker_count( plugin->conf->xcutor); i >=0; --i) {
		worker = uxc_xcutor_get_worker( plugin->conf->xcutor, i);
		if( worker == NULL) continue;
		uxcsessmgr = uxc_worker_get_sessmgr( worker);
		if( uxcsessmgr == NULL) continue;
		sessmgr = uxc_sessmgr_get_user_data( uxcsessmgr);	
		if( sessmgr) continue;
		sessmgr = uims_sessmgr_create( plugin->sesscdata, uxcsessmgr); 
		if( sessmgr == NULL) {
			ux_log(UXL_CRT, "Failed to create UIMS session manager. (thread_id=%d)", i);
			return UX_ENOMEM;
		}
	}

	if( plugin->cdrmgr) uims_cdrmgr_start( plugin->cdrmgr);

	return UX_SUCCESS;
}

/**
 * @internal UIMS plugin의 worker별 configuration reload 명령 시 수행할 함수
 * @param pi UIMS plugin
 * @return 실행 결과
 */
static int _uims_plugin_reload( uxc_plugin_t *pi, void *pworker)
{
	//uims_plugin_t *plugin;
	//uxc_worker_t *worker;

	//plugin = ( uims_plugin_t*)pi;
	//worker = ( uxc_worker_t*)pworker;
	ux_log( UXL_DEBUG, "Process _uims_plugin_reload.");

	return eUXC_SUCCESS;
}

/**
 * @internal UIMS plugin의 main 쪽 configuration reload 명령 시 수행할 함수
 * @param pi UIMS plugin
 * @return 실행 결과
 */
static int _uims_plugin_reconf( uxc_plugin_t *pi)
{
	ux_log( UXL_DEBUG, "Process _uims_plugin_reconf.");
	// TODO
	return eUXC_SUCCESS;
}

/**
 * @internal SIP PA에 등록 할 uxcpt get thread id 함수
 * @param paif protocol adapter
 * @param msg uxcutor message
 * @return worker(thread) id
 */
static int _uims_plugin_get_sip_thrid( uxc_paif_t *paif, uxc_msg_t *msg)
{
	uint32_t idx, thrid, nworker;
	upa_sipmsg_t *sipmsg;
	
	sipmsg = (upa_sipmsg_t*)msg->data;
	nworker = uxc_xcutor_get_worker_count( paif->xcutor);
	if( nworker <= 0 ) return 0;

	if( sipmsg->sessinfo->sessid != UIMS_SESSID_NULL) {
		ux_log(UXL_DBG1, "uims_plugin_get_sip_thrid : sessid=%llu", (unsigned long long)sipmsg->sessinfo->sessid);
		thrid = uims_sessid_get_thread_id( sipmsg->sessinfo->sessid)-1;
	} else {
		thrid = sipmsg->mobj->call_id ? sipmsg->mobj->call_id->hash : 0;
	}

	idx = thrid % nworker;
	return idx;
}

/**
 * @internal HTTP PA에 등록 할 uxcpt HTTP get thread id 함수
 * @param paif protocol adapter
 * @param msg uxcutor message
 * @return worker(thread) id
 */
static int _uims_plugin_get_http_thrid( uxc_paif_t *paif, uxc_msg_t *msg)
{
	uint32_t idx, thrid, nworker;
	uhttp_hdrs_t *hdrs;
	uhttp_value_t *value;
	char *sessid = NULL;
	char *gwsessid = NULL;
	char temp[64];
	upa_httpmsg_t *httpmsg = (upa_httpmsg_t*)msg->data;

	nworker = uxc_xcutor_get_worker_count( paif->xcutor);
	if( nworker <= 0 ) return 0;

	hdrs = uhttp_msg_get_hdrs( httpmsg->msg);
	value = uhttp_hdrs_get_value( hdrs, "SessionID", 0);
	if (value != NULL) {
		sessid = uhttp_value_get_str( value, temp, sizeof(temp), NULL);
		ux_log( UXL_INFO, "_uims_plugin_get_http_thrid: SessionID HDR=%s", sessid );
	}

	value = uhttp_hdrs_get_value( hdrs, "GW-SessionID", 0);
	if (value != NULL) {
		gwsessid = uhttp_value_get_str( value, temp, sizeof(temp), NULL);
		ux_log( UXL_INFO, "_uims_plugin_get_http_thrid: GW-SessionID HDR=%s", gwsessid );
	}

	if( httpmsg->sessid != UXC_SESSID_NULL) {
		thrid = uxc_sessid_get_thread_id( httpmsg->sessid)-1;
		ux_log( UXL_INFO, "idx: idx=%d", thrid % nworker );
	} else if (sessid != NULL) {
		thrid = (uint32_t)ux_str_hash(sessid);
		if (gwsessid != NULL) {
			httpmsg->sessid = (uint32_t)atoi(gwsessid);
		}
		ux_log( UXL_INFO, "idx: idx=%d, httpmsg->sessid=%d", thrid % nworker, httpmsg->sessid );
	} else {
		thrid = httpmsg->connid;
		ux_log( UXL_INFO, "idx: idx=%d", thrid % nworker );
	}

	idx = thrid % nworker;
	return idx; 
}

/**
 * @internal 전달된 service FSM node와 관련된 message 객체를 생성한다.
 * @param pasfif PA service FSM interface
 * @param sfnode service FSM node
 * @param sdm SDM
 * @return 생성된 message 객체
 */
static uxc_msg_t* _uims_plugin_create_sipmsg( uxc_pasfif_t *pasfif, uxc_sfnode_t *sfnode, uxc_sdm_t *sdm)
{
	upa_sipmsg_t *sipmsg;
	uxc_sess_t *uxcsess = (uxc_sess_t*)sdm->impl;
	uims_sess_t *sess = uxc_sess_get_user_data( uxcsess); 
	upa_sipsfif_t *sipsfif = (upa_sipsfif_t*)pasfif;
	uxc_sfpainfo_t *painfo = uxc_sfnode_get_painfo( sfnode);
	upa_sipmbi_t *mbi = (upa_sipmbi_t*)painfo->buildinfo;

	sipmsg = upa_sipmsg_create( upa_sipmbi_get_mflags(mbi));
	if( sipmsg == NULL ) {
		uxc_trace( UXCTL(1,MAJ), "<%s(%u)>[%s:%s] Could not create SIP message",
				uxc_sfnode_get_svcname(sfnode), uxc_sess_get_id( uxcsess),
				uxc_sfkind_to_str(upa_sipmbi_get_kind(mbi)), uxc_sfnode_get_name(sfnode));
		return NULL;
	}

	if(sess) sipmsg->sessinfo->sessid = sess->id;
	else sipmsg->sessinfo->sessid = 0;
	sipmsg->sessinfo->did = upa_sipmbi_get_did(mbi);
	sipmsg->uxcmsg->paif = upa_sipsfif_get_paif(sipsfif);
	sipmsg->sessinfo->flags |= UPA_SIPSESSINFO_PROXY_MODE;
	ux_log(UXL_DBG1, "uims_plugin_create_sipmsg : sessid=%llu", (unsigned long long)sipmsg->sessinfo->sessid);
	return sipmsg->uxcmsg;
}

uims_dbmgr_t* uims_plugin_get_dbmgr( uims_plugin_t *plugin)
{
	return plugin->dbmgr;
}

uims_sessdb_t* uims_plugin_get_sessdb( uims_plugin_t *plugin)
{
	return plugin->sesscdata->sessdb;
}

void uims_plugin_set_sessdb( uims_plugin_t *plugin, uims_sessdb_t *sessdb)
{
	plugin->sesscdata->sessdb = sessdb;
}

////////////////////////////////////////////////////////////////////////////////
// UIMS action 함수 들
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief UIMS SIP action 함수
 * @param action action
 * @param worker worker
 * @param msg 수신된 message
 * @return 실행 결과
 */
int uims_plugin_handle_sip( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	int rv, opcode, proto, need_create_session;
	//int thread_id;
	ux_mem_t *allocator;
	uxc_tracer_t *tracer;
	uxc_svcselinfo_t *ssinfo;
	uxc_service_t *service;
	uxc_svcselmgr_t *svcselmgr;
	usip_mobj_t *mobj;
	upa_sippa_trace_f msg_trace_f;
	upa_sippa_t *sippa;
	upa_sipmsg_t *sipmsg;
	uims_sess_t *sess;
	uims_sessmgr_t *sessmgr;

	sippa = (upa_sippa_t*)msg->paif;
	sipmsg = (upa_sipmsg_t*)msg->data;
	mobj = sipmsg->mobj;
	svcselmgr = uxc_worker_get_svcselmgr( worker);
	tracer = uxc_worker_get_tracer( worker);
	//thread_id = uxc_worker_get_thread_id( worker);
	sessmgr = uxc_sessmgr_get_user_data( uxc_worker_get_sessmgr( worker));

	opcode = sipmsg->mobj->cseq ? sipmsg->mobj->cseq->method : USIP_METHOD_UNKNOWN;
	proto = 0;
	need_create_session = (mobj->request && sipmsg->sessinfo->sessid == UPA_SIPSESSID_NULL);
	if( !need_create_session ) {
		if( sipmsg->sessinfo->sessid != UPA_SIPSESSID_NULL ) {
			sess = uims_sessmgr_find( sessmgr, sipmsg->sessinfo->sessid);
			if( sess == NULL ) {
				ux_log( UXL_MAJ, "Can't find session for SIP message. "
						"(sessid=%llu, did=%u, %s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s)",
						(unsigned long long)sipmsg->sessinfo->sessid,
						sipmsg->sessinfo->did, mobj->request ? "request" : "response",
						USIP_MOBJ_GET_CSEQMETHOD(mobj), mobj->status ? mobj->status->code : 0,
						USIP_MOBJ_GET_CALLID(mobj),
						USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
						USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj));
				if( mobj->request) {
					upa_sippa_reply( sippa, sipmsg, USIP_STATUS_CALL_TRANSC_DOESNOT_EXIST, "Call session does not exist");
				}
				return UX_ENOENT;
			}
			if( !sipmsg->sessinfo->is_trace && uxc_sess_is_trace_on( sess->uxcsess) &&
					uxc_sess_get_trckey(sess->uxcsess) && (msg_trace_f = upa_sippa_get_on_trace_f( sippa)))
			{
				sipmsg->sessinfo->is_trace = USIP_TRUE;
				sipmsg->sessinfo->key = ux_str_dup( uxc_sess_get_trckey(sess->uxcsess), usip_mobj_get_allocator( mobj));
				sipmsg->sessinfo->log_on = uxc_sess_get_trclog( sess->uxcsess);
				sipmsg->sessinfo->level = uxc_sess_get_trclvl( sess->uxcsess);
				msg_trace_f( sippa, tracer, UPA_SIPMSGDIR_RECV_FROM_PEER, NULL, mobj, sipmsg->sessinfo);
			}
		} else {
			ux_log( UXL_MAJ, "Can't find session for SIP message. (%s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s)",
					mobj->request ? "request" : "response", USIP_MOBJ_GET_CSEQMETHOD(mobj),
					mobj->status ? mobj->status->code : 0, USIP_MOBJ_GET_CALLID(mobj),
					USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
					USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj));
			if( mobj->request) {
				upa_sippa_reply( sippa, sipmsg, USIP_STATUS_CALL_TRANSC_DOESNOT_EXIST, "Call session does not Exist");
			}
			return UX_ENOENT;
		}
	} else {
		ssinfo = svcselmgr ? uxc_svcselmgr_select( svcselmgr, proto, msg) : NULL;
		if( ssinfo == NULL ) {
			ux_log( UXL_MAJ, "Can't find SIP service name. "
					"(proto=%d, svcselmgr=%p, %s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s)",
					proto, svcselmgr,
					mobj->request ? "request" : "response", USIP_MOBJ_GET_CSEQMETHOD(mobj),
					mobj->status ? mobj->status->code : 0, USIP_MOBJ_GET_CALLID(mobj),
					USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
					USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj));
			upa_sippa_reply( sippa, sipmsg, USIP_STATUS_SERVICE_UNAVAILABLE, "Colud not found service name");
			return UX_ENOENT;
		}
		service = uxc_svcselinfo_get_service(ssinfo);
		if( service == NULL ) {
			service = uims_sessmgr_find_service( sessmgr, uxc_svcselinfo_get_svcname(ssinfo));
			if ( service == NULL ) {
				ux_log( UXL_MAJ, "Can't find service for key. "
						"(proto=%d, svcname=%s, %s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s)",
						proto, uxc_svcselinfo_get_svcname(ssinfo),
						mobj->request ? "request" : "response", USIP_MOBJ_GET_CSEQMETHOD(mobj),
						mobj->status ? mobj->status->code : 0, USIP_MOBJ_GET_CALLID(mobj),
						USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
						USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj));
				upa_sippa_reply( sippa, sipmsg, USIP_STATUS_SERVICE_UNAVAILABLE, "Could not found serivce");
				return UX_ENOENT;
			}
			uxc_svcselinfo_set_service(ssinfo, service);
		}

		sess = uims_sessmgr_alloc( sessmgr, service, &rv);
		if ( sess == NULL ) {
			ux_log( UXL_MAJ, "Failed to create SIP session. "
						"(proto=%d, svcname=%s, %s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s, err=%d,%s)",
						proto, uxc_svcselinfo_get_svcname(ssinfo),
						mobj->request ? "request" : "response",
						USIP_MOBJ_GET_CSEQMETHOD(mobj), mobj->status ? mobj->status->code : 0,
						USIP_MOBJ_GET_CALLID(mobj),
						USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
						USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj),
						rv, ux_errnostr(rv));
			upa_sippa_reply( sippa, sipmsg, USIP_STATUS_SERVER_INTERNAL_ERROR, "Session memory is full");
			return UX_ENOENT;
		}

		allocator = uims_sess_get_allocator( sess);
		sess->call_id = (mobj->call_id && mobj->call_id->id) ? ux_str_dup(mobj->call_id->id, allocator) : NULL;
		sess->ltag = (mobj->from && mobj->from->tag) ? ux_str_dup(mobj->from->tag, allocator) : NULL;

		sipmsg->sessinfo->sessid = sess->id;
		sipmsg->sessinfo->did = 0;
		if( sipmsg->sessinfo->is_trace && sipmsg->sessinfo->key ) {
			uxc_sess_set_trckey( sess->uxcsess, ux_str_dup( sipmsg->sessinfo->key, uxc_sess_get_allocator(sess->uxcsess)));
			if( uxc_sess_get_trckey( sess->uxcsess)) {
				uxc_sess_set_trclvl( sess->uxcsess, sipmsg->sessinfo->level);
				uxc_sess_set_trclog( sess->uxcsess, sipmsg->sessinfo->log_on);
				uxc_sess_set_trace_on( sess->uxcsess, sipmsg->sessinfo->is_trace ? UX_TRUE : UX_FALSE);
			}
		}
	}

	if( uxc_sess_is_trace_on( sess->uxcsess) ) {
		uxc_tracer_turn_on( tracer, uxc_sess_get_trckey(sess->uxcsess), uxc_sess_get_trclog(sess->uxcsess),
				uxc_sess_get_trclvl(sess->uxcsess)); 
	}

	rv = uims_sessmgr_run( sessmgr, sess, msg, need_create_session);
	uxc_tracer_turn_off( tracer); 
	if ( rv < UX_SUCCESS ) {
		ux_log( UXL_MAJ, "Failed to process SIP message. "
				"(sess_id=%llu(%u), proto=%d, opcode=%d, %s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s)", 
				(unsigned long long)sess->id, uxc_sess_get_id(sess->uxcsess), proto, opcode,
				mobj->request ? "request" : "response", USIP_MOBJ_GET_CSEQMETHOD(mobj),
				mobj->status ? mobj->status->code : 0, USIP_MOBJ_GET_CALLID(mobj),
				USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
				USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj));
	}

	return rv;
}

/**
 * @brief UIMS HTTP action 함수
 * @param action action
 * @param worker worker
 * @param msg 수신된 message
 * @return 실행 결과
 */
int uims_plugin_handle_http( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	int rv, proto, is_req;
	int need_create_session;
	ux_mem_t *allocator;
	uxc_svcselinfo_t *ssinfo;
	uxc_service_t *service;
	uims_sess_t *sess;
	uhttp_req_t *req;
	uhttp_rsp_t *rsp;
	uhttp_hdrs_t *hdrs;
	uhttp_value_t *value;
	char *sessid = NULL;
	char temp[64];
	upa_httppa_t *httppa = (upa_httppa_t*)msg->paif;
	upa_httpmsg_t *httpmsg = (upa_httpmsg_t*)msg->data;
	uims_sessmgr_t *sessmgr = uxc_sessmgr_get_user_data( uxc_worker_get_sessmgr( worker));
	uxc_svcselmgr_t *svcselmgr = uxc_worker_get_svcselmgr( worker);
	uxc_tracer_t *tracer = uxc_worker_get_tracer( worker);

	is_req = uhttp_msg_is_req(httpmsg->msg);
	if( is_req ) {
		req = (uhttp_req_t*)httpmsg->msg;
		rsp = NULL;
	} else {
		rsp = (uhttp_rsp_t*)httpmsg->msg;
		req = NULL;
	}
	proto = 0;
	need_create_session = (is_req && strstr(uhttp_req_get_path(req), "/start") != NULL ? 1 : 0 );

	if( !need_create_session ) {
		if( httpmsg->sessid != UXC_SESSID_NULL ) {
			sess = uims_sessmgr_find( sessmgr, httpmsg->sessid);
		} else {
			sess = NULL;
		}
		if( sess == NULL ) {
			if( is_req) {
				ux_log( UXL_MAJ, "Can't find session for HTTP message. "
						"(sessid=%u, rid=%u, request=%s %s HTTP/%u.%u)",
						httpmsg->sessid, httpmsg->rid, uhttp_req_get_method_name(req),
						uhttp_req_get_path(req), UHTTP_VER_MAJOR(uhttp_req_get_version(req)),
						UHTTP_VER_MINOR(uhttp_req_get_version(req)));
			} else {
				ux_log( UXL_MAJ, "Can't find session for HTTP message. "
						"(sessid=%u, rid=%u, response=%u %s HTTP/%u.%u)",
						httpmsg->sessid, httpmsg->rid, uhttp_rsp_get_code(rsp),
						uhttp_rsp_get_phrase(rsp), UHTTP_VER_MAJOR(uhttp_rsp_get_version(rsp)),
						UHTTP_VER_MINOR(uhttp_rsp_get_version(rsp)));
			}
			return UX_ENOENT;
		}
	} else {
		ssinfo = svcselmgr ? uxc_svcselmgr_select( svcselmgr, proto, msg) : NULL;
		if( ssinfo == NULL ) {
			ux_log( UXL_MAJ, "Can't find service name. "
					"(proto=%d, svcselmgr=%p, request=%s %s HTTP/%u.%u)",
					proto, svcselmgr, uhttp_req_get_method_name(req),
					uhttp_req_get_path(req), UHTTP_VER_MAJOR(uhttp_req_get_version(req)),
					UHTTP_VER_MINOR(uhttp_req_get_version(req)));
			upa_httppa_reply( httppa, httpmsg, UHTTP_STATUS_503_SERVICE_UNAVAILABLE, "Colud not found service name");
			return UX_ENOENT;
		}
		service = uxc_svcselinfo_get_service(ssinfo);
		if( service == NULL ) {
			service = uims_sessmgr_find_service( sessmgr, uxc_svcselinfo_get_svcname(ssinfo));
			if ( service == NULL ) {
				ux_log( UXL_MAJ, "Can't find service for key. "
						"(svcname=%s, proto=%d, request=%s %s HTTP/%u.%u)",
						uxc_svcselinfo_get_svcname(ssinfo), proto, uhttp_req_get_method_name(req),
						uhttp_req_get_path(req), UHTTP_VER_MAJOR(uhttp_req_get_version(req)),
						UHTTP_VER_MINOR(uhttp_req_get_version(req)));
				upa_httppa_reply( httppa, httpmsg, UHTTP_STATUS_503_SERVICE_UNAVAILABLE, "Could not found serivce");
				return UX_ENOENT;
			}
			uxc_svcselinfo_set_service(ssinfo, service);
		}
		sess = uims_sessmgr_alloc( sessmgr, service, &rv);
		if ( sess == NULL ) {
			ux_log( UXL_MAJ, "Failed to create HTTP session."
						"(svcname=%s, proto=%d, request=%s %s HTTP/%u.%u)",
						uxc_svcselinfo_get_svcname(ssinfo), proto, uhttp_req_get_method_name(req),
						uhttp_req_get_path(req), UHTTP_VER_MAJOR(uhttp_req_get_version(req)),
						UHTTP_VER_MINOR(uhttp_req_get_version(req)));
			upa_httppa_reply( httppa, httpmsg, UHTTP_STATUS_500_INTERNAL_SERVER_ERROR, "Session memory is full");
			return UX_ENOENT;
		}
		allocator = uims_sess_get_allocator( sess);
		
		hdrs = uhttp_msg_get_hdrs( httpmsg->msg);
		value = uhttp_hdrs_get_value( hdrs, "SessionID", 0);
		if (value != NULL) {
			sessid = uhttp_value_get_str( value, temp, sizeof(temp), NULL);
			sess->http_sessid = (sessid) ? ux_str_dup(sessid, allocator) : NULL;
		}
		httpmsg->sessid = sess->id;
		httpmsg->rid = 0;

		ux_log( UXL_INFO, "uims_plugin_handle_http: sessid = %llu", (unsigned long long)httpmsg->sessid);
	}

	if( uxc_sess_is_trace_on( sess->uxcsess) ) {
		uxc_tracer_turn_on( tracer, uxc_sess_get_trckey(sess->uxcsess), uxc_sess_get_trclog(sess->uxcsess),
				uxc_sess_get_trclvl(sess->uxcsess)); 
	}
	rv = uims_sessmgr_run( sessmgr, sess, msg, need_create_session);
	uxc_tracer_turn_off( tracer); 
	if ( rv < UX_SUCCESS ) {
		if( is_req) {
			ux_log( UXL_MAJ, "Failed to process HTTP message. "
					"(sessid=%s/%llu,%u, rid=%u, proto=%d, request=%s %s HTTP/%u.%u)",
					sessid, (unsigned long long)httpmsg->sessid, uxc_sess_get_id(sess->uxcsess), 
					httpmsg->rid, proto,
					uhttp_req_get_method_name(req), uhttp_req_get_path(req),
					UHTTP_VER_MAJOR(uhttp_req_get_version(req)),
					UHTTP_VER_MINOR(uhttp_req_get_version(req)));
			upa_httppa_reply( httppa, httpmsg, UHTTP_STATUS_503_SERVICE_UNAVAILABLE, "Service Fail");
		} else {	
			ux_log( UXL_MAJ, "Failed to process HTTP message. "
					"(sessid=%s/%llu,%u, rid=%u, proto=%d, response=%u %s HTTP/%u.%u)",
					sessid, (unsigned long long)httpmsg->sessid, uxc_sess_get_id(sess->uxcsess), 
					httpmsg->rid, proto,
					uhttp_rsp_get_code(rsp), uhttp_rsp_get_phrase(rsp),
					UHTTP_VER_MAJOR(uhttp_rsp_get_version(rsp)),
					UHTTP_VER_MINOR(uhttp_rsp_get_version(rsp)));
		}	
	}

	return rv;
}

