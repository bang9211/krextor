#ifndef __CLICKTOCALL_DLGSESS_L_H__
#define __CLICKTOCALL_DLGSESS_L_H__

#include "../clicktocall/clicktocall_dlgsess.h"
#include <uims/uims_db.h>
#include <uims/uims_sess.h>
#include <uxlib/ux_def.h>

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL dialog ���� internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief CLICKTOCALL dialog session */
struct clicktocall_dlgsess_s {
	uims_sess_t *sess; /**< ���� IMS session */
	uint64_t extime; /**< executed time-sec*/
	int32_t error; /**< ���� error code */
	uint8_t dlgstate; /**< dialog state */
	uint8_t prevstate; /**< preivios dialog state */
	uint8_t hasreq; /**< request ���� ���� */
	uint8_t method; /**< dialog ���� �޽��� method */
	char *tpeer; /**< ���� peer */

	/* http */
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

	/* sip */
	uint32_t ocseq; /**< originator cseq */
	char *ostag; /**< clicktocall originator session tag */
	char *ocall_id; /**< originator call-id */
	usip_nameaddr_t *ofrom; /**< originator From header */
	usip_nameaddr_t *oto; /**< originator To header */
	uint32_t tcseq; /**< terminator cseq */
	char *tstag; /**< clicktocall terminator session tag */
	char *tcall_id; /**< terminator call-id */
	usip_nameaddr_t *tfrom; /**< terminator From header */
	usip_nameaddr_t *tto; /**< terminator To header */
	uint32_t mscseq; /**< ms cseq */
	char *msstag; /**< clicktocall ms session tag */
	char *mscall_id; /**< ms call-id */
	usip_nameaddr_t *msfrom; /**< ms From header */
	usip_nameaddr_t *msto; /**< ms To header */
	
	void *data; /**< session ������ */
	void *user_data; /**< �ش� session�� ������ ����� ������ ���� */
};

/** @brief dialog session DAO Ÿ�� */
typedef struct clicktocall_dlgdao_s clicktocall_dlgdao_t; 
/** @brief diloag session DAO ����ü */
struct clicktocall_dlgdao_s {
	uims_db_t *db; /**< ����� DB Ÿ�� */
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
