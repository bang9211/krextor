#include "local/clicktocall_plugin_l.h"
#include "uims/uims_plugin.h"

static clicktocall_plugin_t *_g_plugin = NULL;

static void _clicktocall_plugin_destroy( uxc_plugin_t *pi);
static int _clicktocall_plugin_reload( uxc_plugin_t *pi, void *pworker);
static int _clicktocall_plugin_reconf( uxc_plugin_t *pi);
static int _clicktocall_plugin_stop( uxc_plugin_t *pi);

//////////////////////////////////////////////////////////////////////////////////////////////
// functions for icsf_plugin_t 
//////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief GW plugin singleton instance를 반환한다.
 * @return GW plugin singleton instance
 */
UX_DECLARE(clicktocall_plugin_t*) clicktocall_plugin_instance()
{
	return _g_plugin;
}

/**
 * @brief GW plugin을 생성한다.
 * @param xcutor uxcutor instance
 * @param cfile configuration file
 * @return 생성된 GW plugin
 */
UX_DECLARE(uxc_plugin_t*) clicktocall_plugin_create( void *xcutor, const char *cfile)
{
	int rv;
	clicktocall_plugin_t *plugin;

	plugin = (clicktocall_plugin_t*)ux_zalloc( ux_mem_default(), sizeof(clicktocall_plugin_t));
	if( plugin == NULL) {
		ux_log(UXL_CRT, "Can't allocate memory for clicktocall_plugin_t.");
		return NULL;
	}

	rv = clicktocall_plugin_init( plugin, (uxc_xcutor_t*)xcutor, cfile);
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
ux_status_t  clicktocall_plugin_init( clicktocall_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile)
{
	int rv;
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

	rv = clicktocall_conf_init( plugin->conf, cfile);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to clicktocall_conf_init. (err=%d,%s)", rv, ux_errnostr(rv));
		uxc_plugin_final( plugin->base);
		return rv;
	}
	clicktocall_conf_print( plugin->conf);

	plugin->conf->xcutor = xcutor;
	plugin->conf->sippa = sippa;

	rv = clicktocall_sessdb_init( plugin->sessdb, plugin->conf);
	if( rv < eUXC_SUCCESS) {
		ux_log( UXL_ERR, "Failed to initialize GW session DB. (err=%d,%s)", rv, ux_errnostr(rv));
		clicktocall_conf_final( plugin->conf);
		uxc_plugin_final( plugin->base);
		return rv;
	}

	plugin->base->destroy = _clicktocall_plugin_destroy;
	plugin->base->reconf = _clicktocall_plugin_reconf;
	plugin->base->reload = _clicktocall_plugin_reload;
	plugin->base->heartbeat = NULL;
	plugin->base->stop = _clicktocall_plugin_stop;
	uims_plugin_set_sessdb( uims_plugin_instance(), plugin->sessdb->base);

	return UX_SUCCESS;
}

/**
 * @internal GW plugin의 사용을 종료한다.
 * @param plugin GW plugin
 */
void clicktocall_plugin_final( clicktocall_plugin_t *plugin)
{
	clicktocall_sessdb_final( plugin->sessdb);
	clicktocall_conf_final( plugin->conf);
	uxc_plugin_final( plugin->base);
}

/**
 * @internal GW plugin을 소멸시 호출될 함수
 * @param pi plugin
 */
static void _clicktocall_plugin_destroy( uxc_plugin_t *pi)
{
	clicktocall_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _clicktocall_plugin_destroy.");

	plugin = (clicktocall_plugin_t*)pi;
	clicktocall_plugin_final( plugin);
	ux_free( ux_mem_default(), plugin);
}

/**
 * @internal GW plugin과 관련해 worker별 정보 재로드시 호출될 함수
 * @param pi plugin
 * @param pworker worker
 * @return 실행 결과
 */
static int _clicktocall_plugin_reload( uxc_plugin_t *pi, void *pworker)
{
	//uxc_worker_t *worker;
	//clicktocall_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _clicktocall_plugin_reload.");
	//plugin = (clicktocall_plugin_t*)pi;
	//worker = (uxc_worker_t*)pworker;

	return UX_SUCCESS;
}

/**
 * @internal GW plugin과 관련해 configuration 로딩시 호출될 함수
 * @param pi plugin
 * @return 실행 결과
 */
static int _clicktocall_plugin_reconf( uxc_plugin_t *pi)
{
	//clicktocall_plugin_t *plugin;

	ux_log( UXL_DEBUG, "Process _clicktocall_plugin_reconf.");
	//plugin = (clicktocall_plugin_t*)pi;

	return UX_SUCCESS;
}

/**
 * @internal GW plugin 서비스 종료 시 호출될 함수
 * @param pi plugin
 */
static int _clicktocall_plugin_stop( uxc_plugin_t *pi)
{
	ux_log( UXL_DEBUG, "Process _clicktocall_plugin_stop.");
	uims_plugin_set_sessdb( uims_plugin_instance(), NULL);
	return UX_SUCCESS;
}