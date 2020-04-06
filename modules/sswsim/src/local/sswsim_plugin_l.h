#ifndef __SSWSIM_PLUGIN_L_H__
#define __SSWSIM_PLUGIN_L_H__

#include "../sswsim/sswsim_plugin.h"
#include "sswsim_conf_l.h"

////////////////////////////////////////////////////////////////////////////////
// SSWSIM Plugin internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief sswsim plugin ±¸Á¶Ã¼ */
struct sswsim_plugin_s {
	uxc_plugin_t base[1]; /**< base type */
	sswsim_conf_t conf[1]; /**< plugin configuration */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for sswsim_plugin_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t sswsim_plugin_init( sswsim_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile);
void sswsim_plugin_final( sswsim_plugin_t *plugin);

#endif // #ifndef __SSWSIM_PLUGIN_L_H__
