#ifndef __IPMSSIM_CMD_H__
#define __IPMSSIM_CMD_H__


int ipmssim_cmd_init( char *prompt, int sleep_time );
int ipmssim_cmd_start();
void ipmssim_cmd_stop();
int ipmssim_cmd_run( int argc, char *argv[] );


#endif // #ifndef __IPMSSIM_CMD_H__
