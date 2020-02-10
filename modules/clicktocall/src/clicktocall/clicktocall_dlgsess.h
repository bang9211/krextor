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
	CLICKTOCALL_DLGSTATE_CALLED_PROCEEDING, 
	CLICKTOCALL_DLGSTATE_CALLED_ACCEPTED,
	CLICKTOCALL_DLGSTATE_CALLED_REJECTED,
	CLICKTOCALL_DLGSTATE_ESTABLISHED,
	CLICKTOCALL_DLGSTATE_TERMINATING,
	CLICKTOCALL_DLGSTATE_TERMINATED
} clicktocall_dlgstate_e;

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
UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_ssw_outgoing_req( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *reqmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_initial_res( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *resmsg);

UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ocall_id( clicktocall_dlgsess_t *dlgsess, const char *call_id);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ofrom( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_oto( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);

UX_DECLARE(int) clicktocall_dlgsess_sprint( clicktocall_dlgsess_t *dlgsess, char *buffer, int buflen);

UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_ouser( clicktocall_dlgsess_t *dlgsess);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ouser( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);
UX_DECLARE(usip_contact_hdr_t*) clicktocall_dlgsess_get_ocontact( clicktocall_dlgsess_t *dlgsess);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_ocontact( clicktocall_dlgsess_t *dlgsess, usip_contact_hdr_t *value);
UX_DECLARE(usip_route_hdr_t*) clicktocall_dlgsess_get_oroute( clicktocall_dlgsess_t *dlgsess);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_oroute( clicktocall_dlgsess_t *dlgsess, usip_route_hdr_t *value);
UX_DECLARE(usip_nameaddr_t*) clicktocall_dlgsess_get_tuser( clicktocall_dlgsess_t *dlgsess);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tuser( clicktocall_dlgsess_t *dlgsess, usip_nameaddr_t *value);
UX_DECLARE(usip_contact_hdr_t*) clicktocall_dlgsess_get_tcontact( clicktocall_dlgsess_t *dlgsess);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_tcontact( clicktocall_dlgsess_t *dlgsess, usip_contact_hdr_t *value);
UX_DECLARE(usip_route_hdr_t*) clicktocall_dlgsess_get_troute( clicktocall_dlgsess_t *dlgsess);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_set_troute( clicktocall_dlgsess_t *dlgsess, usip_route_hdr_t *value);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_load_from_initial_req( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *reqmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_load_from_initial_rsp( clicktocall_dlgsess_t *dlgsess, upa_sipmsg_t *rspmsg);

UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_initial_req( clicktocall_dlgsess_t *dlgsess,
						uims_dbmgr_t *dbmgr, upa_sipmsg_t *reqmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_forward_initial_req( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *fwdmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_initial_rsp( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *rspmsg, int *status);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_forward_initial_rsp( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *rspmsg, upa_sipmsg_t *fwdmsg);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_handle_notify_req( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *reqmsg, int *is_terminated);
UX_DECLARE(ux_status_t) clicktocall_dlgsess_make_forward_msg( clicktocall_dlgsess_t *dlgsess,
						upa_sipmsg_t *srcmsg, upa_sipmsg_t *fwdmsg);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __CLICKTOCALL_DLGSESS_H__
