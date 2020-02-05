#include "local/uims_conf_l.h"
#include <uxcutor/uxc_util.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////
// function to handle uims_conf_t
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief plugin configuration을 초기화한다.
 * @param conf plugin configuration
 * @param cfile configuration file
 * @return 실행 결과
 */
ux_status_t uims_conf_init( uims_conf_t *conf, const char *cfile)
{
	return uims_conf_load( conf, cfile);
}

/**
 * @brief plugin configuration의 사용을 종료한다.
 * @param conf plugin configuration
 */
void uims_conf_final( uims_conf_t *conf )
{
	//Do Nothing
}

/**
 * @brief plugin configuration의 값을 load 한다. 
 * @param conf plugin configuration
 * @param cfile configuration file
 * @return 실행 결과
 */
ux_status_t uims_conf_load( uims_conf_t *conf, const char *cfile)
{
	static char *section = "[UXCUTOR/PLUGIN/UIMS/COMMON]";

	conf->max_db = uxc_get_conf_int( cfile, section, "max_db", 16);
	conf->sess_hash_bin_size = uxc_get_conf_int( cfile, section, "sess_hash_bin_size", 81920);
	uxc_get_conf_str( cfile, section, "sess_db", conf->sess_db, sizeof( conf->sess_db), "");

	return UX_SUCCESS;
}

/**
 * @brief plugin configuration의 값을 출력한다. 
 * @param conf plugin configuration
 */
void uims_conf_print( uims_conf_t *conf)
{
	ux_log(UXL_INFO, "[UXCUTOR/PLUGIN/UIMS/COMMON]");
	ux_log(UXL_INFO, "max_db = %d", conf->max_db);
	ux_log(UXL_INFO, "sess_hash_bin_size = %d", conf->sess_hash_bin_size);
	if( conf->sess_db[0]) ux_log(UXL_INFO, "sess_db = %s", conf->sess_db);
}

