#ifndef __SMSC_MSG_H__
#define __SMSC_MSG_H__

#include <string.h>
#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// SMSC Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

//BIND
typedef struct bind_msg_tcp_s bind_msg_tcp_t;
struct bind_msg_tcp_s {
	char userID[BINDING_USER_ID_LEN];
	char password[BINDING_PASSWORD_LEN];
};

//BIND_ACK
typedef struct bind_ack_msg_tcp_s bind_ack_msg_tcp_t;
struct bind_ack_msg_tcp_s {
	int result;
	char prefix[PREFIX_LEN];
};

//DELIVER
typedef struct deliver_msg_tcp_s deliver_msg_tcp_t;
struct deliver_msg_tcp_s {
	int teleServiceID;
	char callingNumber[CALL_NUMBER_LEN];
	char calledNumber[CALL_NUMBER_LEN];
	char callbackNumber[CALL_NUMBER_LEN];
	char message[MESSAGE_LEN];
	int serialNumber;
};

//deliver_ack_ACK
typedef struct deliver_ack_msg_tcp_s deliver_ack_msg_tcp_t;
struct deliver_ack_msg_tcp_s {
	int result;
	char callingNumber[CALL_NUMBER_LEN];
	char calledNumber[CALL_NUMBER_LEN];
	int serialNumber;
};

//REPORT
typedef struct report_msg_tcp_s report_msg_tcp_t;
struct report_msg_tcp_s {
	int result;
	char callingNumber[CALL_NUMBER_LEN];
	char calledNumber[CALL_NUMBER_LEN];
	int serialNumber;
	long deliverTime;
};

//REPORT_ACK
typedef struct report_ack_msg_tcp_s report_ack_msg_tcp_t;
struct report_ack_msg_tcp_s {
	int result;
};

///////////////////////////////////////////////////////////////////////////////////
// SMSC Message Functions 
///////////////////////////////////////////////////////////////////////////////////
int bind_msg_tcp_init( bind_msg_tcp_t *bind_msg);
void bind_msg_tcp_final( bind_msg_tcp_t *bind_msg);
void bind_msg_tcp_display(char *headerStr, bind_msg_tcp_t *bind_msg);

int bind_ack_msg_tcp_init( bind_ack_msg_tcp_t *bind_ack_msg);
void bind_ack_msg_tcp_final( bind_ack_msg_tcp_t *bind_ack_msg);
void bind_ack_msg_tcp_display(char *headerStr, bind_ack_msg_tcp_t *bind_ack_msg);

int deliver_msg_tcp_init( deliver_msg_tcp_t *deliver_msg);
void deliver_msg_tcp_final( deliver_msg_tcp_t *deliver_msg);
int deliver_msg_decode_dbif_msg( deliver_msg_tcp_t *deliver_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
int deliver_msg_encode_to_dbif_msg( deliver_msg_tcp_t *deliver_msg, uxc_dbif_t *dbif);
void deliver_msg_tcp_display(char *headerStr, deliver_msg_tcp_t *deliver_msg);
void deliver_msg_dbif_display(uxc_dbif_t *dbif);

int deliver_ack_msg_tcp_init( deliver_ack_msg_tcp_t *deliver_ack_msg);
void deliver_ack_msg_tcp_final( deliver_ack_msg_tcp_t *deliver_ack_msg);
int deliver_ack_msg_decode_dbif_msg( deliver_ack_msg_tcp_t *deliver_ack_msg, uxc_dbif_t *dbif);
int deliver_ack_msg_encode_to_dbif_msg( deliver_ack_msg_tcp_t *deliver_ack_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void deliver_ack_msg_tcp_display(char *headerStr, deliver_ack_msg_tcp_t *deliver_ack_msg);
void deliver_ack_msg_dbif_display(uxc_dbif_t *dbif);

int report_msg_tcp_init( report_msg_tcp_t *report_msg);
void report_msg_tcp_final( report_msg_tcp_t *report_msg);
int report_msg_decode_dbif_msg( report_msg_tcp_t *report_msg, uxc_dbif_t *dbif);
int report_msg_encode_to_dbif_msg( report_msg_tcp_t *report_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void report_msg_tcp_display(char *headerStr, report_msg_tcp_t *report_msg);
void report_msg_dbif_display(uxc_dbif_t *dbif);

int report_ack_msg_tcp_init( report_ack_msg_tcp_t *report_ack_msg);
void report_ack_msg_tcp_final( report_ack_msg_tcp_t *report_ack_msg);
int report_ack_msg_decode_dbif_msg( report_ack_msg_tcp_t *report_ack_msg, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
int report_ack_msg_encode_to_dbif_msg( report_ack_msg_tcp_t *report_ack_msg, uxc_dbif_t *dbif);
void report_ack_msg_tcp_display(char *headerStr, report_ack_msg_tcp_t *report_ack_msg);
void report_ack_msg_dbif_display(uxc_dbif_t *dbif);


#endif // #ifndef __SMSC_MSG_H__
