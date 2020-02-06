#ifndef __CLICKTOCALL_DLGSESS_L_H__
#define __CLICKTOCALL_DLGSESS_L_H__

#include "../clicktocall/clicktocall_dlgsess.h"
#include <uims/uims_db.h>
#include <uims/uims_sess.h>
#include <uxlib/ux_def.h>

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL dialog 관련 internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief CLICKTOCALL dialog session */
struct clicktocall_dlgsess_s {
	uims_sess_t *sess; /**< 관련 IMS session */
	upa_sipmsg_t *reqmsg; /**< request */
	uint64_t extime; /**< executed time-sec*/
	int32_t error; /**< 서비스 error code */
	uint32_t mscseq; /**< ms cseq */
	uint32_t ocseq; /**< originator cseq */
	uint32_t tcseq; /**< terminator cseq */
	uint8_t dlgstate; /**< dialog state */
	uint8_t prevstate; /**< preivios dialog state */
	uint8_t hasreq; /**< request 수신 여부 */
	uint8_t method; /**< dialog 생성 메시지 method */
	char *tpeer; /**< 착신 peer */
	char *ostag; /**< clicktocall originator session tag */
	char *tstag; /**< clicktocall terminator session tag */
	usip_nameaddr_t *ouser; /**< orignator user (From header) */
	usip_contact_hdr_t *ocontact; /**< originator contact */
	usip_route_hdr_t *oroute; /**< originator record-routes */
	usip_nameaddr_t *tuser; /**< terminator user. (To header) */
	usip_contact_hdr_t *tcontact; /**< terminator contact */
	usip_route_hdr_t *troute; /**< terminator record-routes */
	uint32_t thread_id;
	uint32_t conn_id;
	uint32_t stream_id;
	uint32_t version;
	char *sessionid;
	char *subscribername;
	char *callingnumber;
	char *callednumber;
	char *chargingnumber;
	uint8_t ringbacktonetype;
	char *watitngmentid;
	char *callmentid;
	char *callingcid;
	char *calledcid;
	uint8_t hostcode;
	void *data; /**< session 데이터 */
	void *user_data; /**< 해당 session에 연결할 사용자 데이터 정보 */
};

/** @brief dialog session DAO 타입 */
typedef struct clicktocall_dlgdao_s clicktocall_dlgdao_t; 
/** @brief diloag session DAO 구조체 */
struct clicktocall_dlgdao_s {
	uims_db_t *db; /**< 사용할 DB 타입 */
	uims_dbmgr_t *dbmgr; /** dbmgr */
};

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgsess_t
////////////////////////////////////////////////////////////////////////////////
ux_status_t clicktocall_dlgsess_final( clicktocall_dlgsess_t *dlgsess);

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_dlgdao_t
////////////////////////////////////////////////////////////////////////////////
ux_status_t clicktocall_dlgdao_init( clicktocall_dlgdao_t *dao, uims_dbmgr_t *dbmgr);
void clicktocall_dlgdao_final( clicktocall_dlgdao_t *dao);
clicktocall_dlgsess_t* clicktocall_dlgdao_find( clicktocall_dlgdao_t *dao,
						uims_sessmgr_t *sessmgr, uims_sessid_t sessid);
ux_status_t clicktocall_dlgdao_insert( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess);
ux_status_t clicktocall_dlgdao_remove( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess);
ux_status_t clicktocall_dlgdao_update( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess);
ux_status_t clicktocall_dlgdao_update_p( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess);
ux_status_t clicktocall_dlgdao_update_e( clicktocall_dlgdao_t *dao, clicktocall_dlgsess_t *dlgsess);

#endif // #ifndef __CLICKTOCALL_DLGSESS_L_H__
