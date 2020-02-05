#ifndef __UIMS_SESS_L_H__
#define __UIMS_SESS_L_H__

#include "../uims/uims_sess.h"
#include <uxlib/ux_hash.h>
#include <uxcutor/uxc_sessmgr.h>
#include <upa/upa_sippa.h>
#include <uxlib/ux_hash.h>
#include "uims_conf_l.h"
#include "../uims/uims_cdr.h"

////////////////////////////////////////////////////////////////////////////////
// UIMS session 관련 internal type 및 macro 
////////////////////////////////////////////////////////////////////////////////

/** @brief UIMS session 구조체 */ 
struct uims_sess_s {
	ux_hnode_t hnode[1]; /**< session id hash node */
	ux_hnode_t snode[1]; /**< sip session id hash node */
	uims_sessid_t id; /**< session id */
	uxc_sess_t *uxcsess; /**< uxcutor session */
	uims_sessmgr_t *sessmgr; /**< 자신을 소유한 UIMS session manager */ 
	uims_cdrbuff_t *cdrbuff; /**< cdr 작성을 위한 buffer */
	char *http_sessid; /**< http sesssion id */
	char *call_id; /**< call-id */
	char *ltag; /**< local tag */
	char *rtag; /**< remote tag */
	void *data; /**< 확장 data */
};

/** @brief UIMS session common data 구조체 */
struct uims_sesscdata_s {
	uims_conf_t *conf; /**< UIMS configuration */
	uims_sessdb_t *sessdb; /**< session DB */
};

/** @brief UIMS session manager */
struct uims_sessmgr_s {
	uims_sesscdata_t *cdata; /**< UIMS session common data */
	uxc_sessmgr_t *uxcsessmgr; /**< UXCUTOR session manager */
	uint16_t host_id; /**< 해당 장비의 HOST ID */
	uint16_t thread_id; /**< 해당 session manager가 속한 thread의 id */
	uint32_t seqnum; /**< session id할당시 사용할 sequnece number */
	uims_sess_t *current; /**< 현재 서비스가 수행되고 있는 session */
	ux_hash_t sessmap[1]; /**< session 검색 map */
	ux_hash_t diamap[1]; /**< diameter session 검색 */
};

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sess_t
////////////////////////////////////////////////////////////////////////////////
ux_status_t uims_sess_final( uims_sess_t *sess);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessmgr_t; 
////////////////////////////////////////////////////////////////////////////////
ux_status_t uims_sessmgr_init( uims_sessmgr_t *sessmgr, uims_sesscdata_t *cdata,
						uxc_sessmgr_t *uxcsessmgr);
void uims_sessmgr_final( uims_sessmgr_t *sessmgr);


#endif // #ifndef __UIMS_SESS_L_H__
