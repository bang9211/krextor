#ifndef __CLICKTOCALL_PLUGIN_H__
#define __CLICKTOCALL_PLUGIN_H__

#include <upa/upa_sippa.h>

/**
 * @ingroup clicktocall_plugin
 * @file
 * @brief CLICKTOCALL plugin 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL Plugin types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief clicktocall plugin 타입 */
typedef struct clicktocall_plugin_s clicktocall_plugin_t;

////////////////////////////////////////////////////////////////////////////////
// functions for iscf_plugin_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(clicktocall_plugin_t*) clicktocall_plugin_instance();
UX_DECLARE(uxc_plugin_t*) clicktocall_plugin_create( void *xcutor, const char *cfile);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __CLICKTOCALL_PLUGIN_H__
