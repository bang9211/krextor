#ifndef __SMSCIB_CONF_H__
#define __SMSCIB_CONF_H__

#include <uxlib/ux_def.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// TCP Configuration Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

#define PROCESS_NAME_SIZE 64

typedef struct smscib_conf_s smscib_conf_t; 
struct smscib_conf_s {
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

int smscib_conf_init( smscib_conf_t *conf, uxc_xcutor_t *xcutor, const char* cfile);
void smscib_conf_final( smscib_conf_t *conf);
int smscib_conf_load( smscib_conf_t *conf, const char* cfile);

#endif // #ifndef __SMSCIB_CONF_H__
