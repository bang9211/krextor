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
	uxc_get_conf_str( cfile, "[SKB]", "BINDING_USER_ID", conf->binding_user_id, sizeof( conf->binding_user_id), "");
	if ( conf->binding_user_id[0] == '\0') {
		ux_log(UXL_MAJ, "no exists BINDING_USER_ID");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, "BINDING_USER_ID : %s", conf->binding_user_id);

	uxc_get_conf_str( cfile, "[SKB]", "BINDING_PASSWORD", conf->binding_password, sizeof( conf->binding_password), "");
	if ( conf->binding_password[0] == '\0') {
		ux_log(UXL_MAJ, "no exists BINDING_PASSWORD");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, "BINDING_PASSWORD : %s", conf->binding_password);

	conf->heartbeat_interval = uxc_get_conf_int( cfile, "[SKB]", "HEARTBEAT_INTERVAL", 0);
	if ( conf->heartbeat_interval < 1) {
		ux_log(UXL_MAJ, "invalid HEARTBEAT_INTERVAL");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, "HEARTBEAT_INTERVAL : %d", conf->heartbeat_interval);

	conf->heartbeat_timeout = uxc_get_conf_int( cfile, "[SKB]", "HEARTBEAT_TIMEOUT", 0);
	if ( conf->heartbeat_timeout < 1) {
		ux_log(UXL_MAJ, "invalid HEARTBEAT_TIMEOUT");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, "HEARTBEAT_TIMEOUT : %d", conf->heartbeat_timeout);

	conf->request_timeout = uxc_get_conf_int( cfile, "[SKB]", "REQUEST_TIMEOUT", 0);
	if ( conf->request_timeout < 1) {
		ux_log(UXL_MAJ, "invalid REQUEST_TIMEOUT");
		return eUXC_INVALID_CONF;
	}
	ux_log(UXL_MAJ, "REQUEST_TIMEOUT : %d", conf->request_timeout);


	return eUXC_SUCCESS;
}

