#ifndef __GW_PLUGIN_L_H__
#define __GW_PLUGIN_L_H__

#include "../gw/gw_plugin.h"
#include "gw_conf_l.h"

////////////////////////////////////////////////////////////////////////////////
// GW Plugin internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief gw plugin ±¸Á¶Ã¼ */
struct gw_plugin_s {
	uxc_plugin_t base[1]; /**< base type */
	gw_conf_t conf[1]; /**< plugin configuration */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for gw_plugin_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t gw_plugin_init( gw_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile);
void gw_plugin_final( gw_plugin_t *plugin);

#endif // #ifndef __GW_PLUGIN_L_H__
