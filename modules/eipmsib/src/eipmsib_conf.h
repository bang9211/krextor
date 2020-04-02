#ifndef __eipmsib_conf_H__
#define __eipmsib_conf_H__

#include <uxlib/ux_def.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct eipmsib_conf_s eipmsib_conf_t; 
struct eipmsib_conf_s {
	int mqid;
	char binding_user_id[BINDING_USER_ID_LEN];
	char binding_password[BINDING_PASSWORD_LEN];
	int heartbeat_interval;
	int heartbeat_timeout;
};

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Functions 
///////////////////////////////////////////////////////////////////////////////////

int eipmsib_conf_init( eipmsib_conf_t *conf, uxc_xcutor_t *xcutor, const char* cfile);
void eipmsib_conf_final( eipmsib_conf_t *conf);
int eipmsib_conf_load( eipmsib_conf_t *conf, const char* cfile);

#endif // #ifndef __eipmsib_conf_H__
