#ifndef __TCP_CONF_H__
#define __TCP_CONF_H__

#include <uxlib/ux_def.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_conf_s tcp_conf_t; 
struct tcp_conf_s {
	int mqid;
	char origin_host[64];
	char origin_realm[64];
	char dest_realm[64];
	char dest_host[64];
};

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Functions 
///////////////////////////////////////////////////////////////////////////////////

int tcp_conf_init( tcp_conf_t *conf, uxc_xcutor_t *xcutor, const char* cfile);
void tcp_conf_final( tcp_conf_t *conf);

int tcp_conf_load( tcp_conf_t *conf, const char* cfile);

#endif // #ifndef __TCP_CONF_H__
