#ifndef __UIMS_CONF_L_H__
#define __UIMS_CONF_L_H__

#include "../uims/uims_conf.h"

////////////////////////////////////////////////////////////////////////////////
// UIMS configuration internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief uims configuration 구조체 */
struct uims_conf_s {
	uxc_xcutor_t *xcutor; /**< uxcutor instance */
	upa_sippa_t *sippa; /**< SIP Protocol Adapeter instance */
	upa_httppa_t *httppa; /**< HTTP Protocol Adapeter instance */
	int max_db; /**< 최대 접속 DB 정보 개수 */
	int sess_hash_bin_size; /**< session hash bin size */
	char sess_db[128]; /**< session db name */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_conf_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t uims_conf_init( uims_conf_t *conf, const char *cfile);
void uims_conf_final( uims_conf_t *conf);
ux_status_t uims_conf_load( uims_conf_t *conf, const char *cfile);
void uims_conf_print( uims_conf_t *conf);

#endif // #ifndef __UIMS_CONF_L_H__
