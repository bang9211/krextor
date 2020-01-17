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

/** @brief HTTP XCRIPT Configuration Ÿ�� */
typedef struct uxcpt_httpconf_s uxcpt_httpconf_t; 
/** @brief HTTP XCRIPT Configuration ����ü */
struct uxcpt_httpconf_s {
	int is_replace_get_thrid; /**< get_thrid�� replace ���� */
	char pa_name[64]; /**< HTTP XCRIPT���� ����� PA �̸� */
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
