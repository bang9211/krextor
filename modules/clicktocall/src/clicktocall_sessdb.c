#include "local/clicktocall_sessdb_l.h"
#include "uims/uims_plugin.h"

////////////////////////////////////////////////////////////////////////////////
// functions for clicktocall_sessdb_t
////////////////////////////////////////////////////////////////////////////////
static uims_sess_t* _clicktocall_sessdb_find( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sessid_t sessid);
static ux_status_t _clicktocall_sessdb_insert( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sess_t *sess);
static ux_status_t _clicktocall_sessdb_remove( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sess_t *sess);
static ux_status_t _clicktocall_sessdb_update( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sess_t *sess);

ux_status_t clicktocall_sessdb_init( clicktocall_sessdb_t *sessdb, clicktocall_conf_t *conf)
{
	int rv;
	sessdb->base->find = _clicktocall_sessdb_find;
	sessdb->base->insert = _clicktocall_sessdb_insert;
	sessdb->base->remove = _clicktocall_sessdb_remove;
	sessdb->base->update = _clicktocall_sessdb_update;
	sessdb->conf = conf;

	sessdb->dbmgr = uims_plugin_get_dbmgr( uims_plugin_instance());
	if( sessdb->dbmgr == NULL) {
		ux_log( UXL_ERR, "Failed to get DB manager from IMS COMMON plugin.");
		return UX_ENOENT;
	}

	rv = clicktocall_dlgdao_init( sessdb->dlgdao, sessdb->dbmgr, conf);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to initialize Call Session DAO. (err=%d,%s)", rv, uxc_errnostr(rv));
		return rv;
	}

	return UX_SUCCESS;
}

void clicktocall_sessdb_final( clicktocall_sessdb_t *sessdb)
{
	clicktocall_dlgdao_final( sessdb->dlgdao);
}

static uims_sess_t* _clicktocall_sessdb_find( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sessid_t sessid)
{
	int svcid;
	uims_sess_t *sess;
	clicktocall_dlgsess_t *dlgsess;
	
	svcid = uims_sessid_get_service_id( sessid);
	ux_log(UXL_INFO, "CLICKTOCALL SESSDB find. (sessid=%llu, svcid=%d)", (unsigned long long) sessid, svcid);
	switch( svcid ) {
		case 0: 
			dlgsess = clicktocall_dlgdao_find( ((clicktocall_sessdb_t*)sessdb)->dlgdao, sessmgr, sessid);
			sess = (dlgsess) ? dlgsess->sess : NULL;
			break;
		default :
			sess = NULL;
			break;
	}

	return sess;
}

static ux_status_t _clicktocall_sessdb_insert( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sess_t *sess)
{
	int svcid;
	uims_sessid_t sessid;
	clicktocall_dlgsess_t *dlgsess;

	sessid = uims_sess_get_id( sess);
	svcid = uims_sessid_get_service_id( sessid);
	ux_log(UXL_INFO, "CLICKTOCALL SESSDB insert. (sessid=%llu, svcid=%d)", (unsigned long long) sessid, svcid);
	switch( svcid ) {
		case 0: 
			dlgsess = (clicktocall_dlgsess_t*)uims_sess_get_data( sess);
			return clicktocall_dlgdao_insert( ((clicktocall_sessdb_t*)sessdb)->dlgdao, dlgsess); 
		default : break;
	}

	return UX_SUCCESS;
}

static ux_status_t _clicktocall_sessdb_remove( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sess_t *sess)
{
	int svcid;
	uims_sessid_t sessid;
	clicktocall_dlgsess_t *dlgsess;

	sessid = uims_sess_get_id( sess);
	svcid = uims_sessid_get_service_id( sessid);
	ux_log(UXL_INFO, "CLICKTOCALL SESSDB remove. (sessid=%llu, svcid=%d)", (unsigned long long) sessid, svcid);
	switch( svcid ) {
		case 0 :
			dlgsess = (clicktocall_dlgsess_t*)uims_sess_get_data( sess);
			return clicktocall_dlgdao_remove( ((clicktocall_sessdb_t*)sessdb)->dlgdao, dlgsess); 
		default : break;
	}

	return UX_SUCCESS;
}

static ux_status_t _clicktocall_sessdb_update( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
		                        uims_sess_t *sess)
{
	int svcid;
	uims_sessid_t sessid;
	clicktocall_dlgsess_t *dlgsess;

	sessid = uims_sess_get_id( sess);
	svcid = uims_sessid_get_service_id( sessid);
	ux_log(UXL_INFO, "CLICKTOCALL SESSDB update. (sessid=%llu, svcid=%d)", (unsigned long long) sessid, svcid);
	switch( svcid ) {
		case 0 :
			dlgsess = (clicktocall_dlgsess_t*)uims_sess_get_data( sess);
			return clicktocall_dlgdao_update( ((clicktocall_sessdb_t*)sessdb)->dlgdao, dlgsess); 
		default : break;
	}

	return UX_SUCCESS;
}



