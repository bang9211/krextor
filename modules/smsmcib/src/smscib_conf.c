#include "eipmsib_conf.h"

int eipmsib_conf_init( eipmsib_conf_t *conf, uxc_xcutor_t *xcutor, const char* cfile)
{
	conf->mqid = uxc_xcutor_get_mqid( xcutor);
	return eipmsib_conf_load( conf, cfile);
}

void eipmsib_conf_final( eipmsib_conf_t *conf)
{
	//nothing
}

int eipmsib_conf_load( eipmsib_conf_t *conf, const char* cfile)
{
	uxc_get_conf_str( cfile, "[SKB]", "CALL_BINDING_USER_ID", conf->call_binding_user_id, sizeof( conf->call_binding_user_id), "");
	if ( conf->call_binding_user_id[0] == '\0') {
		ux_log(UXL_MAJ, "no exists CALL_BINDING_USER_ID");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - CALL_BINDING_USER_ID : %s", conf->call_binding_user_id);

	uxc_get_conf_str( cfile, "[SKB]", "CALL_BINDING_PASSWORD", conf->call_binding_password, sizeof( conf->call_binding_password), "");
	if ( conf->call_binding_password[0] == '\0') {
		ux_log(UXL_MAJ, "no exists CALL_BINDING_PASSWORD");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - CALL_BINDING_PASSWORD : %s", conf->call_binding_password);

	uxc_get_conf_str( cfile, "[SKB]", "RECORDING_BINDING_USER_ID", conf->recording_binding_user_id, sizeof( conf->recording_binding_user_id), "");
	if ( conf->recording_binding_user_id[0] == '\0') {
		ux_log(UXL_MAJ, "no exists RECORDING_BINDING_USER_ID");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - RECORDING_BINDING_USER_ID : %s", conf->recording_binding_user_id);

	uxc_get_conf_str( cfile, "[SKB]", "RECORDING_BINDING_PASSWORD", conf->recording_binding_password, sizeof( conf->recording_binding_password), "");
	if ( conf->recording_binding_password[0] == '\0') {
		ux_log(UXL_MAJ, "no exists RECORDING_BINDING_PASSWORD");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - RECORDING_BINDING_PASSWORD : %s", conf->recording_binding_password);

	uxc_get_conf_str( cfile, "[SKB]", "CONFERENCE_BINDING_USER_ID", conf->conference_binding_user_id, sizeof( conf->conference_binding_user_id), "");
	if ( conf->conference_binding_user_id[0] == '\0') {
		ux_log(UXL_MAJ, "no exists CONFERENCE_BINDING_USER_ID");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - CONFERENCE_BINDING_USER_ID : %s", conf->conference_binding_user_id);

	uxc_get_conf_str( cfile, "[SKB]", "CONFERENCE_BINDING_PASSWORD", conf->conference_binding_password, sizeof( conf->conference_binding_password), "");
	if ( conf->conference_binding_password[0] == '\0') {
		ux_log(UXL_MAJ, "no exists CONFERENCE_BINDING_PASSWORD");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - CONFERENCE_BINDING_PASSWORD : %s", conf->conference_binding_password);

	conf->heartbeat_interval = uxc_get_conf_int( cfile, "[SKB]", "HEARTBEAT_INTERVAL", 0);
	if ( conf->heartbeat_interval < 1) {
		ux_log(UXL_MAJ, "invalid HEARTBEAT_INTERVAL");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - HEARTBEAT_INTERVAL : %d", conf->heartbeat_interval);

	conf->heartbeat_timeout = uxc_get_conf_int( cfile, "[SKB]", "HEARTBEAT_TIMEOUT", 0);
	if ( conf->heartbeat_timeout < 1) {
		ux_log(UXL_MAJ, "invalid HEARTBEAT_TIMEOUT");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - HEARTBEAT_TIMEOUT : %d", conf->heartbeat_timeout);

	conf->request_timeout = uxc_get_conf_int( cfile, "[SKB]", "REQUEST_TIMEOUT", 0);
	if ( conf->request_timeout < 1) {
		ux_log(UXL_MAJ, "invalid REQUEST_TIMEOUT");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - REQUEST_TIMEOUT : %d", conf->request_timeout);

	conf->heartbeat_display = uxc_get_conf_bool(cfile, "[SKB]", "HEARTBEAT_DISPLAY", 1);
	ux_log(UXL_MAJ, " - HEARTBEAT_DISPLAY : %s", conf->heartbeat_display ? "TRUE" : "FALSE");
	
	uxc_get_conf_str( cfile, "[SKB]", "DBIF_GW_PROCESS_NAME", conf->dbif_gw_process_name, sizeof( conf->dbif_gw_process_name), "");
	if ( conf->dbif_gw_process_name[0] == '\0') {
		ux_log(UXL_MAJ, "no exists DBIF_GW_PROCESS_NAME");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, " - DBIF_GW_PROCESS_NAME : %s", conf->dbif_gw_process_name);

	return eUXC_SUCCESS;
}

