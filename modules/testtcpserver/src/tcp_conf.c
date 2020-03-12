#include "tcp_conf.h"

int tcp_conf_init( tcp_conf_t *conf, uxc_xcutor_t *xcutor, const char* cfile)
{
	conf->mqid = uxc_xcutor_get_mqid( xcutor);
	return tcp_conf_load( conf, cfile);
}

void tcp_conf_final( tcp_conf_t *conf)
{
	//nothing
}

int tcp_conf_load( tcp_conf_t *conf, const char* cfile)
{
	uxc_get_conf_str( cfile, "[CTF]", "ORIGIN_HOST", conf->origin_host, sizeof( conf->origin_host), "");
	if ( conf->origin_host[0] == '\0') {
		ux_log(UXL_MAJ, "no exits origin host");
		return eUXC_INVALID_CONF;
	}

	uxc_get_conf_str( cfile, "[CTF]", "ORIGIN_REALM", conf->origin_realm, sizeof( conf->origin_realm), "");
	if ( conf->origin_realm[0] == '\0') {
		ux_log(UXL_MAJ, "no exits origin realm");
		return eUXC_INVALID_CONF;
	}

	uxc_get_conf_str( cfile, "[CTF]", "DESTINATION_REALM", conf->dest_realm, sizeof( conf->dest_realm), "");
	if ( conf->dest_realm[0] == '\0') {
		ux_log(UXL_MAJ, "no exits destination realm");
		return eUXC_INVALID_CONF;
	}

	uxc_get_conf_str( cfile, "[CTF]", "DESTINATION_HOST", conf->dest_host, sizeof( conf->dest_host), "");
	if ( conf->dest_host[0] == '\0') {
		ux_log(UXL_MAJ, "no exits destination host");
		return eUXC_INVALID_CONF;
	}

	return eUXC_SUCCESS;
}

