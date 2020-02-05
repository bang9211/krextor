#ifndef __UIMS_PLUGIN_H__
#define __UIMS_PLUGIN_H__

#include <upa/upa_sippa.h>
#include <upa/upa_httppa.h>
#include "uims_conf.h"
#include "uims_sipmsg.h"
#include "uims_sess.h"
#include "uims_db.h"

/**
 * @ingroup uims_plugin
 * @file
 * @brief UIMS plugin 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// UIMS Plugin types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief uims plugin 타입 */
typedef struct uims_plugin_s uims_plugin_t;

////////////////////////////////////////////////////////////////////////////////
// functions for uims_plugin_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_plugin_t*) uims_plugin_instance();
UX_DECLARE(uxc_plugin_t*) uims_plugin_create( void *xcutor, const char *cfile);

uims_dbmgr_t* uims_plugin_get_dbmgr( uims_plugin_t *plugin);
uims_sessdb_t* uims_plugin_get_sessdb( uims_plugin_t *plugin);
void uims_plugin_set_sessdb( uims_plugin_t *plugin, uims_sessdb_t *sessdb);
const char* uims_plugin_get_dia_host( uims_plugin_t *plugin);
const char* uims_plugin_get_dia_realm( uims_plugin_t *plugin);

////////////////////////////////////////////////////////////////////////////////
// UIMS handle functions
////////////////////////////////////////////////////////////////////////////////
int uims_handle_sip( uxc_action_t *action, void *pworker, uxc_msg_t *uxcmsg);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __UIMS_PLUGIN_H__
