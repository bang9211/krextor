#ifndef __UIMS_PLUGIN_L_H__
#define __UIMS_PLUGIN_L_H__

#include "../uims/uims_plugin.h"
#include "uims_conf_l.h"
#include "uims_sess_l.h"
#include "../uims/uims_db.h"
#include "../uims/uims_cdr.h"

////////////////////////////////////////////////////////////////////////////////
// UIMS Plugin internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief uims plugin ±¸Á¶Ã¼ */
struct uims_plugin_s {
	uxc_plugin_t base[1]; /**< base type */
	uims_conf_t conf[1]; /**< plugin configuration */
	uims_sesscdata_t sesscdata[1]; /**< session common data */
	uims_dbmgr_t *dbmgr; /**< db manager */
	uims_cdrmgr_t *cdrmgr; /**< cdr manager */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_plugin_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t uims_plugin_init( uims_plugin_t *plugin, uxc_xcutor_t *xcutor, const char *cfile);
void uims_plugin_final( uims_plugin_t *plugin);

#endif //#ifndef __UIMS_PLUGIN_L_H__
