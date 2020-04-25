#ifndef __IPMSSIM_MSG_CONF_H__
#define __IPMSSIM_MSG_CONF_H__

#include <time.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <math.h>
#include <time.h>
#include <ua_log.h>
#include "ipmssim_util.h"
#include "../../../SKBGW/modules/eipmsib/src/skb_msg.h"

#define CHANNEL_SIZE 3

int ipmssim_proc_clicktoconference_binding_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_start_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_add_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_remove_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_change_party_media_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_change_option_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_get_number_of_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_stop_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_play_ment_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_get_party_status_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);
int ipmssim_proc_clicktoconference_cancel_party_req(int idx, skb_msg_t *rcvmsg, skb_msg_t *sndmsg);

#endif  // #ifndef __IPMSSIM_MSG_CONF_H__