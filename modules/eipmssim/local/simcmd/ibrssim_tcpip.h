#ifndef __IPMSSIM_TCPIP_H__
#define __IPMSSIM_TCPIP_H__


#include "ipmssim_msg.h"


int ipmssim_tcpip_start( const char *ipaddr, unsigned short port );
int ipmssim_tcpip_stop();
void ipmssim_tcpip_display();
int ipmssim_tcpip_read( ipmssim_msg_t *msg );
int ipmssim_tcpip_write( int idx, ipmssim_msg_t *msg );
int ipmssim_tcpip_write_all( ipmssim_msg_t *msg );
void ipmssim_tcpip_keepalive( int interval, int (func)(int idx) );


#endif // #ifndef __IPMSSIM_TCPIP_H__
