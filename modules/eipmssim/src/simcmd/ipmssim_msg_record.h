#ifndef __IPMSSIM_MSG_RECORD_H__
#define __IPMSSIM_MSG_RECORD_H__

#include <time.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <math.h>
#include <time.h>
#include <ua_log.h>
#include "ipmssim_util.h"
#include "../../../SKBGW/modules/eipmsib/src/skb_msg.h"

int ipmssim_proc_clicktocallrecording_binding_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktocallrecording_start_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktocallrecording_stop_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktocallrecording_service_status_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);

#endif  // #ifndef __IPMSSIM_MSG_RECORD_H__