#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "tcp_msg.h"
#include "../../eipmsib/src/skb_msg.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////
// TCP Server Service Manager(plugin) Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct tcp_server_s tcp_server_t; 
struct tcp_server_s {
	uxc_plugin_t base[1];
	uxc_xcutor_t *xcutor;
	upa_tcp_t *patcp;
};

///////////////////////////////////////////////////////////////////////////////////
// TCP Server Service Manager(plugin) functions 
///////////////////////////////////////////////////////////////////////////////////

uxc_plugin_t* tcp_server_create( void *xcutor, const char *cfile);

tcp_server_t* tcp_server_instance();

int tcp_server_handle_svrreq( tcp_server_t *server, uxc_worker_t* worker, upa_tcpmsg_t *tcpmsg);

#endif // #ifndef __TCP_SERVER_H__
