#ifndef __UXCPT_HTTPCONF_H__
#define __UXCPT_HTTPCONF_H__

#include <uxlib/ux_def.h>
#include <uxlib/ux_hash.h>
#include <uhttp/uhttp_msg.h>

/**
 * @ingroup uxcpt_httpconf
 * @file
 * @brief HTTP XCRIPT Configuration
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// HTTP XCRIPT Configuration types & macros
////////////////////////////////////////////////////////////////////////////////

/** @brief HTTP XCRIPT Configuration 타입 */
typedef struct uxcpt_httpconf_s uxcpt_httpconf_t; 
/** @brief HTTP XCRIPT Configuration 구조체 */
struct uxcpt_httpconf_s {
	int is_replace_get_thrid; /**< get_thrid의 replace 여부 */
	char pa_name[64]; /**< HTTP XCRIPT에서 사용할 PA 이름 */
};

////////////////////////////////////////////////////////////////////////////////
// functions for upxy_httpconf_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t uxcpt_httpconf_init( uxcpt_httpconf_t *conf);
void uxcpt_httpconf_final( uxcpt_httpconf_t *conf);
ux_status_t uxcpt_httpconf_load( uxcpt_httpconf_t *conf, const char *cfile);
void uxcpt_httpconf_print( uxcpt_httpconf_t *conf);

#ifdef __cplusplus
}
#endif

#endif
