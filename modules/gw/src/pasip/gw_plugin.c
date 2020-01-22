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


