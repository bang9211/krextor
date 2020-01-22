#ifndef __GW_PLUGIN_H__
#define __GW_PLUGIN_H__

#include <upa/upa_sippa.h>

/**
 * @ingroup gw_plugin
 * @file
 * @brief GW plugin 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// GW Plugin types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief gw plugin 타입 */
typedef struct gw_plugin_s gw_plugin_t;

////////////////////////////////////////////////////////////////////////////////
// functions for iscf_plugin_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(gw_plugin_t*) gw_plugin_instance();
UX_DECLARE(uxc_plugin_t*) gw_plugin_create( void *xcutor, const char *cfile);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __GW_PLUGIN_H__
