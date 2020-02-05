#ifndef __CLICKTOCALL_PLUGIN_L_H__
#define __CLICKTOCALL_PLUGIN_L_H__

#include "../clicktocall/clicktocall_plugin.h"
#include "clicktocall_sessdb_l.h"
#include "clicktocall_conf_l.h"
#include "uims/uims_db.h"

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL Plugin internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief clicktocall plugin ±¸Á¶Ã¼ */
struct clicktocall_plugin_s {
	uxc_plugin_t base[1]; /**< base type */
	clicktocall_conf_t conf[1]; /**< plugin configuration */
	clicktocall_sessdb_t sessdb[1]; /**< session DB */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for clicktocall_plugin_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t clicktocall_plugin_init( clicktocall_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile);
void clicktocall_plugin_final( clicktocall_plugin_t *plugin);

#endif // #ifndef __CLICKTOCALL_PLUGIN_L_H__
