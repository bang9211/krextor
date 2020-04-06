#ifndef __SSWSIM_PLUGIN_H__
#define __SSWSIM_PLUGIN_H__

#include <upa/upa_sippa.h>

/**
 * @ingroup sswsim_plugin
 * @file
 * @brief SSWSIM plugin 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// SSWSIM Plugin types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief sswsim plugin 타입 */
typedef struct sswsim_plugin_s sswsim_plugin_t;

////////////////////////////////////////////////////////////////////////////////
// functions for iscf_plugin_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(sswsim_plugin_t*) sswsim_plugin_instance();
UX_DECLARE(uxc_plugin_t*) sswsim_plugin_create( void *xcutor, const char *cfile);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __SSWSIM_PLUGIN_H__
