#include "local/gw_plugin_l.h"

static gw_plugin_t *_g_plugin = NULL;

static void _gw_plugin_destroy( uxc_plugin_t *pi);
static int _gw_plugin_reload( uxc_plugin_t *pi, void *pworker);
static int _gw_plugin_reconf( uxc_plugin_t *pi);
static int _gw_plugin_stop( uxc_plugin_t *pi);

//////////////////////////////////////////////////////////////////////////////////////////////
// functions for icsf_plugin_t 
//////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief GW plugin singleton instance를 반환한다.
 * @return GW plugin singleton instance
 */
UX_DECLARE(gw_plugin_t*) gw_plugin_instance()
{
	return _g_plugin;
}

/**
 * @brief GW plugin을 생성한다.
 * @param xcutor uxcutor instance
 * @param cfile configuration file
 * @return 생성된 GW plugin
 */
UX_DECLARE(uxc_plugin_t*) gw_plugin_create( void *xcutor, const char *cfile)
{
	int rv;
	gw_plugin_t *plugin;

	plugin = (gw_plugin_t*)ux_zalloc( ux_mem_default(), sizeof(gw_plugin_t));
	if( plugin == NULL) {
		ux_log(UXL_CRT, "Can't allocate memory for gw_plugin_t.");
		return NULL;
	}

	rv = gw_plugin_init( plugin, (uxc_xcutor_t*)xcutor, cfile);
	if( rv < UX_SUCCESS) {
		ux_free( ux_mem_default(), plugin);
		return NULL;
	}

	ux_log( UXL_INFO, "gw plugin created." );

	_g_plugin = plugin;

	return plugin->base;
}

/**
 * @internal GW plugin을 초기화 한다.
 * @param plugin GW plugin
 * @param xcutor uxcutor instance
 * @param cfile configuration file
 * @return 실행 결과
 */
ux_status_t  gw_plugin_init( gw_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile)
{
	int rv, nworkers;
	upa_sippa_t *sippa;

	sippa = (upa_sippa_t*)uxc_xcutor_get_paif( xcutor, "PA_SIP");
	if( sippa == NULL) {
		ux_log(UXL_CRT, "Can't find Protocol Adapter PA_SIP.");
		return UX_ENOENT;
	}

	rv = uxc_plugin_init( plugin->base, xcutor, cfile);
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_MAJ, "Failed to uxc_plugin_init. (err=%d,%s)", rv, ux_errnostr(rv));
		return rv;
	}

	rv = gw_conf_init( plugin->conf, cfile);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to gw_conf_init. (err=%d,%s)", rv, ux_errnostr(rv));
		uxc_plugin_final( plugin->base);
		return rv;
	}
	gw_conf_print( plugin->conf);

	plugin->conf->xcutor = xcutor;
	plugin->conf->sippa = sippa;

	nworkers = uxc_xcutor_get_worker_count( xcutor)+1;
/*
	rv = gw_routermgr_init( plugin->routermgr, plugin->conf, nworkers, plugin);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to initialize GW router manager. (err=%d,%s)", rv, ux_errnostr(rv));
		gw_conf_final( plugin->conf);
		uxc_plugin_final( plugin->base);
	}

	rv = gw_sessdb_init( plugin->sessdb, plugin->routermgr);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_ERR, "Failed to initialize GW session DB. (err=%d,%s)", rv, ux_errnostr(rv));
		gw_routermgr_final( plugin->routermgr);
		gw_conf_final( plugin->conf);
		uxc_plugin_final( plugin->base);
		return rv;
	}

	rv = gw_thig_init( plugin->thid, plugin->conf);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_ERR, "Failed to initialize GW THIG data. (err=%d,%s)", rv, ux_errnostr(rv));
		gw_sessdb_final( plugin->sessdb);
		gw_routermgr_final( plugin->routermgr);
		gw_conf_final( plugin->conf);
		uxc_plugin_final( plugin->base);
		return rv;
	}
*/

	plugin->base->destroy = _gw_plugin_destroy;
	plugin->base->reconf = _gw_plugin_reconf;
	plugin->base->reload = _gw_plugin_reload;
	plugin->base->heartbeat = NULL;
	plugin->base->stop = _gw_plugin_stop;
	//uims_plugin_set_sessdb( uims_plugin_instance(), plugin->sessdb->base);

/*
	rv = gw_rtppmgr_init( plugin->rtppmgr, GW_RTPPIF_MAX, plugin);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_ERR, "Failed to initialize GW RTPP manager. (err=%d,%s)", rv, ux_errnostr(rv));
		gw_thig_final( plugin->thid);
		gw_sessdb_final( plugin->sessdb);
		gw_routermgr_final( plugin->routermgr);
		gw_conf_final( plugin->conf);
		uxc_plugin_final( plugin->base);
		return rv;
	}

	gw_rtppmgr_load( plugin->rtppmgr, cfile, "[UXCUTOR/PLUGIN/GW/RTPP]");
*/

	return UX_SUCCESS;
}

/**
 * @internal GW plugin의 사용을 종료한다.
 * @param plugin GW plugin
 */
void gw_plugin_final( gw_plugin_t *plugin)
{
/*
	gw_rtppmgr_final( plugin->rtppmgr);
	gw_thig_final( plugin->thid);
	gw_sessdb_final( plugin->sessdb);
	gw_routermgr_final( plugin->routermgr);
*/
	gw_conf_final( plugin->conf);
	uxc_plugin_final( plugin->base);
}

/**
 * @internal GW plugin을 소멸시 호출될 함수
 * @param pi plugin
 */
static void _gw_plugin_destroy( uxc_plugin_t *pi)
{
	gw_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _gw_plugin_destroy.");

	plugin = (gw_plugin_t*)pi;
	gw_plugin_final( plugin);
	ux_free( ux_mem_default(), plugin);
}

/**
 * @internal GW plugin과 관련해 worker별 정보 재로드시 호출될 함수
 * @param pi plugin
 * @param pworker worker
 * @return 실행 결과
 */
static int _gw_plugin_reload( uxc_plugin_t *pi, void *pworker)
{
	uxc_worker_t *worker;
	gw_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _gw_plugin_reload.");
	plugin = (gw_plugin_t*)pi;
	worker = (uxc_worker_t*)pworker;

	//return gw_routermgr_reload( plugin->routermgr, worker); 
	return UX_SUCCESS;
}

/**
 * @internal GW plugin과 관련해 configuration 로딩시 호출될 함수
 * @param pi plugin
 * @return 실행 결과
 */
static int _gw_plugin_reconf( uxc_plugin_t *pi)
{
	gw_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _gw_plugin_reconf.");
	plugin = (gw_plugin_t*)pi;

	return UX_SUCCESS;
	//return gw_routermgr_reconf( plugin->routermgr); 
}

/**
 * @internal GW plugin 서비스 종료 시 호출될 함수
 * @param pi plugin
 */
static int _gw_plugin_stop( uxc_plugin_t *pi)
{
	ux_log( UXL_DEBUG, "Process _gw_plugin_stop.");
	//uims_plugin_set_sessdb( uims_plugin_instance(), NULL);
	return UX_SUCCESS;
}


/**
 * @brief GW SIP action 함수
 * @param action action
 * @param worker worker
 * @param msg 수신된 message
 * @return 실행 결과
 */
int gw_plugin_handle_sip( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	int rv, thread_id, opcode, proto, need_create_session;
	ux_mem_t *allocator;
	uxc_tracer_t *tracer;
	uxc_svcselinfo_t *ssinfo;
	uxc_service_t *service;
	uxc_svcselmgr_t *svcselmgr;
	usip_mobj_t *mobj;
	upa_sippa_trace_f msg_trace_f;
	upa_sippa_t *sippa;
	upa_sipmsg_t *sipmsg;
	uxc_sess_t *sess;
	uxc_sessmgr_t *sessmgr;

	sippa = (upa_sippa_t*)msg->paif;
	sipmsg = (upa_sipmsg_t*)msg->data;
	mobj = sipmsg->mobj;
	svcselmgr = uxc_worker_get_svcselmgr( worker);
	tracer = uxc_worker_get_tracer( worker);
	thread_id = uxc_worker_get_thread_id( worker);
	sessmgr = uxc_worker_get_sessmgr( worker);

	opcode = sipmsg->mobj->cseq ? sipmsg->mobj->cseq->method : USIP_METHOD_UNKNOWN;
	proto = 0;
	need_create_session = (mobj->request && sipmsg->sessinfo->sessid == UPA_SIPSESSID_NULL);
	if( !need_create_session ) {
		if( sipmsg->sessinfo->sessid != UPA_SIPSESSID_NULL ) {
			sess = uxc_sessmgr_get( sessmgr, 0xFFFFFFFF & sipmsg->sessinfo->sessid);
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
			if( !sipmsg->sessinfo->is_trace && uxc_sess_is_trace_on( sess) &&
					uxc_sess_get_trckey(sess) && (msg_trace_f = upa_sippa_get_on_trace_f( sippa)))
			{
				sipmsg->sessinfo->is_trace = USIP_TRUE;
				sipmsg->sessinfo->key = ux_str_dup( uxc_sess_get_trckey(sess), usip_mobj_get_allocator( mobj));
				sipmsg->sessinfo->log_on = uxc_sess_get_trclog( sess);
				sipmsg->sessinfo->level = uxc_sess_get_trclvl( sess);
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
			service = uxc_sessmgr_find_service( sessmgr, uxc_svcselinfo_get_svcname(ssinfo));
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

		sess = uxc_sessmgr_alloc( sessmgr, service);
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

		
		allocator = uxc_sess_get_allocator( sess);
		uxc_sess_set_str_n(sess, "call_id", (mobj->call_id && mobj->call_id->id) ? ux_str_dup(mobj->call_id->id, allocator) : NULL)
		uxc_sess_set_str_n(sess, "ltag", (mobj->from && mobj->from->tag) ? ux_str_dup(mobj->from->tag, allocator) : NULL)

		sipmsg->sessinfo->sessid = uxc_sess_get_id(sess);
		sipmsg->sessinfo->did = 0;
		if( sipmsg->sessinfo->is_trace && sipmsg->sessinfo->key ) {
			uxc_sess_set_trckey( sess, ux_str_dup( sipmsg->sessinfo->key, uxc_sess_get_allocator(sess)));
			if( uxc_sess_get_trckey( sess)) {
				uxc_sess_set_trclvl( sess, sipmsg->sessinfo->level);
				uxc_sess_set_trclog( sess, sipmsg->sessinfo->log_on);
				uxc_sess_set_trace_on( sess, sipmsg->sessinfo->is_trace ? UX_TRUE : UX_FALSE);
			}
		}
	}

	if( uxc_sess_is_trace_on( sess) ) {
		uxc_tracer_turn_on( tracer, uxc_sess_get_trckey(sess), uxc_sess_get_trclog(sess),
				uxc_sess_get_trclvl(sess)); 
	}

	rv = uxc_sessmgr_run( sessmgr, sess, msg );
	uxc_tracer_turn_off( tracer); 
	if ( rv < UX_SUCCESS ) {
		ux_log( UXL_MAJ, "Failed to process SIP message. "
				"(sess_id=%llu(%u), proto=%d, opcode=%d, %s=%s/%u, call_id=%s, from=%s:%s, to=%s:%s)", 
				(unsigned long long) uxc_sess_get_id(sess), uxc_sess_get_id(sess), proto, opcode,
				mobj->request ? "request" : "response", USIP_MOBJ_GET_CSEQMETHOD(mobj),
				mobj->status ? mobj->status->code : 0, USIP_MOBJ_GET_CALLID(mobj),
				USIP_MOBJ_GET_FROMUSER(mobj), USIP_MOBJ_GET_FROMTAG(mobj),
				USIP_MOBJ_GET_TOUSER(mobj), USIP_MOBJ_GET_TOTAG(mobj));
	}

	return rv;
}