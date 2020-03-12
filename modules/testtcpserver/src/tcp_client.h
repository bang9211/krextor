#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include <upa/upa_dbif.h>
#include "tcp_msg.h"
#include "tcp_idprsp.h"
#include "tcp_acrsp.h"
#include "tcp_cirrsp.h"

///////////////////////////////////////////////////////////////////////////////////
// TCP Client Service Manager(plugin) Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_client_s tcp_client_t; 
struct tcp_client_s {
	uxc_plugin_t base[1];
	uxc_xcutor_t *xcutor;
	tcp_conf_t conf[1];
	upa_dbif_t *padbif;
	upa_tcp_t *patcp;
};

///////////////////////////////////////////////////////////////////////////////////
// TCP Client Service Manager(plugin) functions 
///////////////////////////////////////////////////////////////////////////////////

uxc_plugin_t *tcp_client_create( void *xcutor, const char* cfile);

tcp_client_t* tcp_client_instance();

int  tcp_client_init( tcp_client_t *client, uxc_xcutor_t *xcutor, const char *cfile);
void tcp_client_final( tcp_client_t *client);

int tcp_client_handle_dbifreq( tcp_client_t *client, uxc_worker_t *worker, uxc_ipcmsg_t *ipcmsg);
int tcp_client_handle_dbifrsp( tcp_client_t *client, uxc_worker_t *worker, upa_tcpmsg_t *tcpmsg);
int tcp_client_send_ipcmsg( tcp_client_t *client,  tcp_msg_t* msg, int rv);

#endif // #ifndef __TCP_CLIENT_H__
