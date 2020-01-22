#include "uxcpt/uxcpt_http.h"
#include <uxlib/ux_log.h>
#include <uxlib/ux_string.h>
#include <uxcutor/uxc_sessmgr.h>
#include <uxcutor/uxcutor.h>

/////////////////////////////////////////////////////////////////////////////////////////
// function to handle uxcpt_http_t
/////////////////////////////////////////////////////////////////////////////////////////
static void _uxcpt_http_destroy( uxc_plugin_t *pi);
static int _uxcpt_http_reload( uxc_plugin_t *pi, void *worker);
static int _uxcpt_http_reconf( uxc_plugin_t *pi);
static int _uxcpt_http_get_thrid( uxc_paif_t *paif, uxc_msg_t *msg);

static uxcpt_http_t* _g_uxcpt_http_instance = NULL;

/**
 * @brief uxcpt HTTP plugin singleton instance를 반환한다.
 * @return uxcpt HTTP plugin singleton instance
 */
uxcpt_http_t* uxcpt_http_instance( void )
{
	return _g_uxcpt_http_instance;
}

/**
 * @brief uxcpt HTTP plugin을 생성한다.
 * @param xcutor uxcutor instance
 * @param cfile configuration file
 * @return 생성된 plugin
 */
uxc_plugin_t* uxcpt_http_create( void *xcutor, const char *cfile)
{
	int rv;
	upa_httppa_t *httppa;

	uxcpt_http_t *pixcpt = ux_zalloc(ux_mem_default(), sizeof(uxcpt_http_t));
	if( pixcpt == NULL ) {
		ux_log(UXL_CRT, "Can't allocate memory for uxcpt_http_t");
		return NULL;
	}

	rv = uxcpt_httpconf_init( pixcpt->conf );
	if ( rv < UX_SUCCESS ) {
		ux_log( UXL_ERR, "uxcpt_httpconf_init() failed. err=[%d,%s]", rv, ux_errnostr(rv) );
		goto error;
	}
	rv = uxcpt_httpconf_load( pixcpt->conf, cfile );
	if ( rv < UX_SUCCESS ) {
		ux_log( UXL_ERR, "uxcpt_httpconf_load() failed. err=[%d,%s]", rv, ux_errnostr(rv) );
		goto error;
	}

	uxcpt_httpconf_print( pixcpt->conf);

	pixcpt->xcutor = (uxc_xcutor_t*)xcutor;
	httppa = (upa_httppa_t*) uxc_xcutor_get_paif( pixcpt->xcutor, pixcpt->conf->pa_name);
	if( httppa == NULL ) {
		ux_log(UXL_CRT, "Can't find Protocol Adapter %s", pixcpt->conf->pa_name);
		goto error;
	}

	rv = uxc_plugin_init( pixcpt->base, xcutor, cfile );
	if( rv < eUXC_SUCCESS ) {
		ux_log( UXL_ERR, "uxcpt_http_init() failed" );
		goto error;
	}

	pixcpt->base->destroy = _uxcpt_http_destroy;
	pixcpt->base->reconf = _uxcpt_http_reconf;
	pixcpt->base->reload = _uxcpt_http_reload;
	pixcpt->base->heartbeat = NULL;
	pixcpt->httppa = httppa;

	if ( pixcpt->conf->is_replace_get_thrid ) {
		pixcpt->get_thrid_org = uxc_paif_get_thrid_func( (uxc_paif_t*)pixcpt->httppa);
		uxc_paif_set_thrid_func((uxc_paif_t*)pixcpt->httppa, _uxcpt_http_get_thrid);
		ux_log( UXL_INFO, "PA_HTTP's get_thrid is replaced" );
	}

	ux_log( UXL_INFO, "uxcpt_http plugin created" );
	_g_uxcpt_http_instance = pixcpt;
	return pixcpt->base;

error:
	ux_free(ux_mem_default(), pixcpt);
	return NULL;
}

/**
 * @internal uxcpt HTTP plugin을 소멸한다.
 * @param pi plugin instance
 */
static void _uxcpt_http_destroy( uxc_plugin_t *pi)
{
	uxcpt_http_t *pixcpt = (uxcpt_http_t*)pi;
	uxc_plugin_final( pixcpt->base);
	ux_free( ux_mem_default(), pixcpt);
}

/**
 * @internal uxcpt HTTP plugin의 worker별 정보를 재로딩한다.
 * @param pi plugin instance
 * @param worker reload를 수행할 worker 함수 
 * @return 실행 결과
 */
static int _uxcpt_http_reload( uxc_plugin_t *pi, void *pworker)
{
	return eUXC_SUCCESS;
}

/**
 * @internal uxcpt HTTP plugin을 configureation 정보를 재로딩한다. 
 * @param pi plugin instance
 * @return 실행 결과
 */
static int _uxcpt_http_reconf( uxc_plugin_t *pi)
{
	return eUXC_SUCCESS;
}


/**
 * @internal HTTP PA에 등록 할 uxcpt HTTP get thread id 함수
 * @param paif protocol adapter
 * @param msg uxcutor message
 * @return worker(thread) id 
 */
static int _uxcpt_http_get_thrid( uxc_paif_t *paif, uxc_msg_t *msg)
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
		ux_log( UXL_INFO, "_uxcpt_http_get_thrid: SessionID HDR=%s", sessid );
	}

	value = uhttp_hdrs_get_value( hdrs, "GW-SessionID", 0);
	if (value != NULL) {
		gwsessid = uhttp_value_get_str( value, temp, sizeof(temp), NULL);
		ux_log( UXL_INFO, "_uxcpt_http_get_thrid: GW-SessionID HDR=%s", gwsessid );
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

/////////////////////////////////////////////////////////////////////////////////////////
// External action funcitons
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief uxcpt HTTP action 함수
 * @param action action
 * @param worker worker
 * @param msg 수신된 message
 * @return 실행 결과
 */
int uxcpt_handle_http( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg)
{
	int rv, proto, is_req;
	int need_create_session;
	uxc_svcselinfo_t *ssinfo;
	uxc_service_t *service;
	uxc_sess_t *sess;
	upa_httppa_t *httppa = (upa_httppa_t*)msg->paif;
	upa_httpmsg_t *httpmsg = (upa_httpmsg_t*)msg->data;
	uhttp_req_t *req;
	uhttp_rsp_t *rsp;
	uxc_sessmgr_t *sessmgr = uxc_worker_get_sessmgr( worker);
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
			sess = uxc_sessmgr_get( sessmgr, 0xFFFFFFFF & httpmsg->sessid);
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
			service = uxc_sessmgr_find_service( sessmgr, uxc_svcselinfo_get_svcname(ssinfo));
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
		sess = uxc_sessmgr_alloc( sessmgr, service);
		if ( sess == NULL ) {
			upa_httppa_reply( httppa, httpmsg, UHTTP_STATUS_500_INTERNAL_SERVER_ERROR, "Session memory is full");
			return UX_ENOENT;
		}
		httpmsg->sessid = uxc_sess_get_id(sess);
		httpmsg->rid = 0;

		ux_log( UXL_INFO, "uxcpt_handle_http: sessid = %u", httpmsg->sessid);
	}

	if( uxc_sess_is_trace_on( sess) ) {
		uxc_tracer_turn_on( tracer, uxc_sess_get_trckey(sess), uxc_sess_get_trclog(sess),
				uxc_sess_get_trclvl(sess)); 
	}
	rv = uxc_sessmgr_run( sessmgr, sess, msg );
	uxc_tracer_turn_off( tracer); 
	if ( rv < UX_SUCCESS ) {
		if( is_req) {
			ux_log( UXL_MAJ, "Failed to process HTTP message. "
					"(sessid=%u, rid=%u, proto=%d, request=%s %s HTTP/%u.%u)",
					uxc_sess_get_id(sess), httpmsg->rid, proto,
					uhttp_req_get_method_name(req), uhttp_req_get_path(req),
					UHTTP_VER_MAJOR(uhttp_req_get_version(req)),
					UHTTP_VER_MINOR(uhttp_req_get_version(req)));
			upa_httppa_reply( httppa, httpmsg, UHTTP_STATUS_503_SERVICE_UNAVAILABLE, "Service Fail");
		} else {	
			ux_log( UXL_MAJ, "Failed to process HTTP message. "
					"(sessid=%u, rid=%u, proto=%d, response=%u %s HTTP/%u.%u)",
					uxc_sess_get_id(sess), httpmsg->rid, proto,
					uhttp_rsp_get_code(rsp), uhttp_rsp_get_phrase(rsp),
					UHTTP_VER_MAJOR(uhttp_rsp_get_version(rsp)),
					UHTTP_VER_MINOR(uhttp_rsp_get_version(rsp)));
		}	
	}

	return rv;
}


