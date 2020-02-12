#ifndef __CLICKTOCALL_DLGSESS_H__
#define __CLICKTOCALL_DLGSESS_H__

#include <uxlib/ux_def.h>
#include <uims/uims_sess.h>
#include <uims/uims_db.h>
#include <upa/upa_httppa.h>

/**
 * @ingroup clicktocall_dlgsess
 * @file
 * @brief CLICKTOCALL dialog session 관련 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL dialog session 관련 type 및 macro 
////////////////////////////////////////////////////////////////////////////////

typedef enum clicktocall_dlgstate_e {
	CLICKTOCALL_DLGSTATE_INIT = 0,
	CLICKTOCALL_DLGSTATE_PROCEEDING, 
	CLICKTOCALL_DLGSTATE_ACCEPTED,
	CLICKTOCALL_DLGSTATE_REJECTED,
	CLICKTOCALL_DLGSTATE_CALLING_PROCEEDING, 
	CLICKTOCALL_DLGSTATE_CALLING_ACCEPTED,
	CLICKTOCALL_DLGSTATE_CALLING_REJECTED,
	CLICKTOCALL_DLGSTATE_CALLED_PROCEEDING, 
	CLICKTOCALL_DLGSTATE_CALLED_ACCEPTED,
	CLICKTOCALL_DLGSTATE_CALLED_REJECTED,
	CLICKTOCALL_DLGSTATE_ESTABLISHED,
	CLICKTOCALL_DLGSTATE_TERMINATING,
	CLICKTOCALL_DLGSTATE_TERMINATED
} clicktocall_dlgstate_e;

typedef enum clicktocall_callto_e {
	CALL_TO_CALLING = 0,
	CALL_TO_CALLED,
	CALL_TO_MS_CALLING,
	CALL_TO_MS_CALLED
} clicktocall_callto_e;

/** @brief clicktocall dialog session 타입 */
typedef struct clicktocall_dlgsess_s clicktocall_dlgsess_t; 

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgstate_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) clicktocall_dlgstate_to_str( clicktocall_dlgstate_e dlgstate);

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgsess_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(clicktocall_dlgsess_t*) clicktocall_dlgsess_create( uims_sess_t *imssess, int *err);

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_http_start_req( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *reqmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_http_start_res( clicktocall_dlgsess_t *dlgsess, upa_httpmsg_t *resmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_sip_invite_req( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *reqmsg, clicktocall_callto_e callto);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_sip_invite_res( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *resmsg, clicktocall_callto_e callto);


UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ocall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ofrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_oto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);


UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tcall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tfrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);

UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_mscall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_msfrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_msto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);



UX_DECLARE(int) clicktocall_dlgsess_sprint( clicktocall_dlgsess_t *dlgsess, char *buffer, int buflen);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __CLICKTOCALL_DLGSESS_H__
