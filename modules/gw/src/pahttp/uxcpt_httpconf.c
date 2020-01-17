#include "uxcpt/uxcpt_httpconf.h"
#include <uxlib/ux_log.h>
#include <uxcutor/uxc_util.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////
// function to handle uxcpt_httpconf_t
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief uxcpt HTTP plugin configuration을 초기화한다.
 * @param conf uxcpt HTTP plugin configuration
 * @return 실행 결과
 */
ux_status_t uxcpt_httpconf_init( uxcpt_httpconf_t *conf)
{
	conf->is_replace_get_thrid = UX_TRUE;
	strcpy(conf->pa_name, "PA_HTTP");

	return UX_SUCCESS;
}

/**
 * @brief uxcpt HTTP plugin configuration의 사용을 종료한다.
 * @param conf uxcpt HTTP plugin configuration
 */
void uxcpt_httpconf_final( uxcpt_httpconf_t *conf )
{
	//Do Nothing
}

/**
 * @brief uxcpt HTTP plugin configuration의 값을 load 한다. 
 * @param conf uxcpt HTTP plugin configuration
 * @param cfile configuration file
 * @return 실행 결과
 */
ux_status_t uxcpt_httpconf_load( uxcpt_httpconf_t *conf, const char *cfile)
{
	conf->is_replace_get_thrid = 
		uxc_get_conf_bool( cfile, "[UXCUTOR/XCRIPT/HTTP]", "REPLACE_GET_THRID", UX_TRUE);
	uxc_get_conf_str( cfile, "[UXCUTOR/XCRIPT/HTTP]", "PA_NAME",
			conf->pa_name, sizeof(conf->pa_name), "PA_HTTP");

	return UHTTP_SUCCESS;
}

/**
 * @brief uxcpt HTTP plugin configuration의 값을 출력한다. 
 * @param conf uxcpt HTTP plugin configuration
 */
void uxcpt_httpconf_print( uxcpt_httpconf_t *conf)
{
	ux_log(UXL_INFO, "[UXCUTOR/XCRIPT/HTTP]");
	ux_log(UXL_INFO, "    REPLACE_GET_THRID : %s", conf->is_replace_get_thrid ? "TRUE" : "FALSE");
	ux_log(UXL_INFO, "    PA_NAME           : %s", conf->pa_name);
}
