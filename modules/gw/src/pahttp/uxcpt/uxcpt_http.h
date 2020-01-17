#ifndef __UXCPT_HTTP_H__
#define __UXCPT_HTTP_H__

#include "uxcpt_httpconf.h"
#include <uxcutor/uxc_svcsel.h>
#include <upa/upa_httppa.h>

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// HTTP XCRIPT plugin types & macros
////////////////////////////////////////////////////////////////////////////////

/** @brief uxcpt httpmeter plugin 타입 */
typedef struct uxcpt_http_s uxcpt_http_t;
/** @brief uxcpt httpmeter plugin 구조체 */
struct uxcpt_http_s {
	uxc_plugin_t base[1]; /**< base type */
	uxc_xcutor_t *xcutor; /**< uxcutor instance */
	upa_httppa_t *httppa; /**< HTTP Protocol Adapter */
	uxc_paif_get_thrid_f get_thrid_org; /**< original get thread id function */
	uxcpt_httpconf_t conf[1]; /**< configuration */
};

////////////////////////////////////////////////////////////////////////////////
// HTTP XCRIPT plugin function 
////////////////////////////////////////////////////////////////////////////////
uxc_plugin_t* uxcpt_http_create( void *xcutor, const char *cfile);

////////////////////////////////////////////////////////////////////////////////
// HTTP XCRIPT handle function 
////////////////////////////////////////////////////////////////////////////////
int uxcpt_handle_http( uxc_action_t *action, uxc_worker_t* worker, uxc_msg_t *msg);

#ifdef __cplusplus
}
#endif

#endif
