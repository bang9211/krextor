#ifndef __eipmsib_conf_H__
#define __eipmsib_conf_H__

#include <uxlib/ux_def.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

#define PROCESS_NAME_SIZE 64

typedef struct eipmsib_conf_s eipmsib_conf_t; 
struct eipmsib_conf_s {
	int mqid;
	char call_binding_user_id[BINDING_USER_ID_LEN];
	char call_binding_password[BINDING_PASSWORD_LEN];
	char recording_binding_user_id[BINDING_USER_ID_LEN];
	char recording_binding_password[BINDING_PASSWORD_LEN];
	char conference_binding_user_id[BINDING_USER_ID_LEN];
	char conference_binding_password[BINDING_PASSWORD_LEN];
	int heartbeat_interval;
	int heartbeat_timeout;
	int request_timeout;
	int heartbeat_display;
	char dbif_gw_process_name[PROCESS_NAME_SIZE];
};

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Functions 
///////////////////////////////////////////////////////////////////////////////////

int eipmsib_conf_init( eipmsib_conf_t *conf, uxc_xcutor_t *xcutor, const char* cfile);
void eipmsib_conf_final( eipmsib_conf_t *conf);
int eipmsib_conf_load( eipmsib_conf_t *conf, const char* cfile);

#endif // #ifndef __eipmsib_conf_H__
