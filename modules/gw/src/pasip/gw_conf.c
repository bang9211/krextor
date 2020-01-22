#include "local/gw_conf_l.h"
#include <uxcutor/uxc_util.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////
// function to handle gw_conf_t
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief plugin configuration을 초기화한다.
 * @param conf plugin configuration
 * @param cfile configuration file
 * @return 실행 결과
 */
ux_status_t gw_conf_init( gw_conf_t *conf, const char *cfile)
{
	return gw_conf_load( conf, cfile);
}

/**
 * @brief plugin configuration의 사용을 종료한다.
 * @param conf plugin configuration
 */
void gw_conf_final( gw_conf_t *conf )
{
	if( conf->thig_confstr) ux_free( ux_mem_default(), conf->thig_confstr);
}

/**
 * @brief plugin configuration의 값을 load 한다. 
 * @param conf plugin configuration
 * @param cfile configuration file
 * @return 실행 결과
 */
ux_status_t gw_conf_load( gw_conf_t *conf, const char *cfile)
{
/*
	char buf[512];

	conf->use_thig = uxc_get_conf_bool( cfile, "[UXCUTOR/GW/COMMON]", "USE_THIG", UX_FALSE);

	uxc_get_conf_str( cfile, "[UXCUTOR/GW/COMMON]", "DEFAULT_PRIVACY", buf, sizeof(buf), "");
	if( ux_str_zcasecmp( buf, "id") == 0) conf->dflt_privacy = GW_PRIVACY_ID;
	else if( ux_str_zcasecmp( buf, "user") == 0) conf->dflt_privacy = GW_PRIVACY_USER;
	else if( ux_str_zcasecmp( buf, "header") == 0) conf->dflt_privacy = GW_PRIVACY_HEADER;
	else if( ux_str_zcasecmp( buf, "session") == 0) conf->dflt_privacy = GW_PRIVACY_SESSION;
	else conf->dflt_privacy = GW_PRIVACY_NONE;

	if( conf->use_thig) {
		uxc_get_conf_str( cfile, "[UXCUTOR/GW/COMMON]", "THIG_HOST",
				conf->thig_host, sizeof(conf->thig_host), "uangel.com");
		uxc_get_conf_str( cfile, "[UXCUTOR/GW/COMMON]", "THIG_KEY",
				conf->thig_key, sizeof(conf->thig_key), NULL);
		if( conf->thig_key[0] == '\0') return UX_ERR_CONF;
		// TODO : 임시 설정. 관리방안 필요
		conf->thig_aes_type = GW_TINYAES_AES128;
		conf->thig_cipher_mode = GW_THIG_CIPHER_CBC;
		uxc_get_conf_str( cfile, "[UXCUTOR/GW/COMMON]", "THIG_INDOMAIN", buf, sizeof(buf), "");
		if( strlen(buf) > 0) conf->thig_confstr = ux_str_dup( buf, ux_mem_default());
	}

	uxc_get_conf_str( cfile, "[UXCUTOR/GW/COMMON]", "RTPP_LOG",
			conf->rtpp_log, sizeof(conf->rtpp_log), NULL);
*/

	return UX_SUCCESS;
}

/**
 * @brief plugin configuration의 값을 출력한다. 
 * @param conf plugin configuration
 */
void gw_conf_print( gw_conf_t *conf)
{
	ux_log(UXL_INFO, "[UXCUTOR/GW/COMMON]");
/*
	ux_log(UXL_INFO, "    DEFAULT_PRIVACY : %s", gw_util_privacy_to_str(conf->dflt_privacy));
	ux_log(UXL_INFO, "    USE_THIG        : %s", conf->use_thig ? "YES" : "NO");
	if( conf->use_thig) {
		ux_log(UXL_INFO, "    THIG_HOST       : %s", conf->thig_host);
		ux_log(UXL_INFO, "    THIG_KEY        : %s", conf->thig_key);
	}
*/
}

