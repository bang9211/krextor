#ifndef __IPMSSIM_TCPIP_H__
#define __IPMSSIM_TCPIP_H__


#include "ipmssim_msg.h"
#include "ipmssim_proc.h"
#include "../../../SKBGW/modules/eipmsib/src/skb_msg.h"


int ipmssim_tcpip_start( const char *ipaddr, unsigned short port[] );
int ipmssim_tcpip_stop();
void ipmssim_tcpip_display();
int ipmssim_tcpip_read( skb_msg_t *msg );
int ipmssim_tcpip_write( int idx, skb_msg_t *msg );
int ipmssim_tcpip_write2( int idx, char *msg, int body_len );
int ipmssim_tcpip_write_all( skb_msg_t *msg );
void ipmssim_tcpip_keepalive( int interval, int (func)(int idx) );

void turn_heartbeat_timer_on(int chnl_idx);
void turn_heartbeat_timer_off(int chnl_idx);


#endif // #ifndef __IPMSSIM_TCPIP_H__
