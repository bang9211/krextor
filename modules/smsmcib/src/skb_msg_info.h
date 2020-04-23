#ifndef __SKB_MSG_INFO_H__
#define __SKB_MSG_INFO_H__

/** @brief define TCP channel info */
#define TCP_CHANNEL_SMS_CLI 									0
#define TCP_CHANNEL_SMS_SVR 									1

/** @brief define DBIF(GW) message ID */
#define NONE_DBIF_MESSAGE                                       0

#define DBIF_GW_DELIVER									        100
#define DBIF_GW_REPORT_ACK									    101

#define DBIF_SMSC_DELIVER_ACK								    200
#define DBIF_SMSC_REPORT									    201

#define DBIF_SMSC_DELIVER						                300
#define DBIF_SMSC_REPORT_ACK						            301

#define DBIF_GW_DELIVER_ACK						                400
#define DBIF_GW_REPORT						                    401

/** @brief define TCP(SMSC) message ID */
#define BIND									                0
#define BIND_ACK									            1
#define DELIVER										            2
#define DELIVER_ACK									            3
#define REPORT										            4
#define REPORT_ACK										        5
#define ALIVE										            8
#define ALIVE_ACK										        9


/** @brief SKB MAX Message Length */
#define SKB_MSG_HEADER_LEN                                      8
#define SKB_MSG_MAX_BODY_LEN                                    264
#define SKB_MSG_MAX_LEN										    SKB_MSG_MAX_BODY_LEN+SKB_MSG_HEADER_LEN
#define SESSION_ID_LEN 										    32
#define GW_SESSION_ID_LEN 									    32
#define CALL_NUMBER_LEN										    32
#define MESSAGE_LEN									            160
#define BINDING_USER_ID_LEN                                     16
#define BINDING_PASSWORD_LEN                                    16
#define PREFIX_LEN                                              16

#define SKB_HEADER_DISPLAY_SIZE                                 256

#endif // #ifndef __SKB_MSG_INFO_H__