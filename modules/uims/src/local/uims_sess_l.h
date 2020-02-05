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
// UIMS session ���� internal type �� macro 
////////////////////////////////////////////////////////////////////////////////

/** @brief UIMS session ����ü */ 
struct uims_sess_s {
	ux_hnode_t hnode[1]; /**< session id hash node */
	ux_hnode_t snode[1]; /**< sip session id hash node */
	uims_sessid_t id; /**< session id */
	uxc_sess_t *uxcsess; /**< uxcutor session */
	uims_sessmgr_t *sessmgr; /**< �ڽ��� ������ UIMS session manager */ 
	uims_cdrbuff_t *cdrbuff; /**< cdr �ۼ��� ���� buffer */
	char *http_sessid; /**< http sesssion id */
	char *call_id; /**< call-id */
	char *ltag; /**< local tag */
	char *rtag; /**< remote tag */
	void *data; /**< Ȯ�� data */
};

/** @brief UIMS session common data ����ü */
struct uims_sesscdata_s {
	uims_conf_t *conf; /**< UIMS configuration */
	uims_sessdb_t *sessdb; /**< session DB */
};

/** @brief UIMS session manager */
struct uims_sessmgr_s {
	uims_sesscdata_t *cdata; /**< UIMS session common data */
	uxc_sessmgr_t *uxcsessmgr; /**< UXCUTOR session manager */
	uint16_t host_id; /**< �ش� ����� HOST ID */
	uint16_t thread_id; /**< �ش� session manager�� ���� thread�� id */
	uint32_t seqnum; /**< session id�Ҵ�� ����� sequnece number */
	uims_sess_t *current; /**< ���� ���񽺰� ����ǰ� �ִ� session */
	ux_hash_t sessmap[1]; /**< session �˻� map */
	ux_hash_t diamap[1]; /**< diameter session �˻� */
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
