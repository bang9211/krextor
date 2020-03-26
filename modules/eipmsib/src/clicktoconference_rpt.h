#ifndef __CLICKTOCONFERENCE_RPT_H__
#define __CLICKTOCONFERENCE_RPT_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Report Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktoconference_add_party_rpt_tcp_s clicktoconference_add_party_rpt_tcp_t;
struct clicktoconference_add_party_rpt_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantName[PARTICIPANT_NAME_LEN];
    char participantNumber[PARTICIPANT_NUMBER_LEN];
};

typedef struct clicktoconference_remove_party_rpt_tcp_s clicktoconference_remove_party_rpt_tcp_t;
struct clicktoconference_remove_party_rpt_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantNumber[CALL_NUMBER_LEN];
};

typedef struct clicktoconference_change_party_media_rpt_tcp_s clicktoconference_change_party_media_rpt_tcp_t;
struct clicktoconference_change_party_media_rpt_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantNumber[CALL_NUMBER_LEN];
    int32_t mediaType;
};

typedef struct clicktoconference_change_option_rpt_tcp_s clicktoconference_change_option_rpt_tcp_t;
struct clicktoconference_change_option_rpt_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char password[PASSWORD_LEN];
};

typedef struct clicktoconference_stop_rpt_tcp_s clicktoconference_stop_rpt_tcp_t;
struct clicktoconference_stop_rpt_tcp_s {
    int32_t returnCode;
	char confID[CONF_ID_LEN];
    char startTime[TIME_LEN];
    char endTime[TIME_LEN];
    int32_t isRecorded;
};

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Report Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_add_party_rpt_tcp_init( clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt);
void clicktoconference_add_party_rpt_tcp_final( clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt);
int clicktoconference_add_party_rpt_encode_to_dbif_msg( clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt, uxc_dbif_t *dbif);
void clicktoconference_add_party_rpt_tcp_display(clicktoconference_add_party_rpt_tcp_t *clicktoconference_add_party_rpt);
void clicktoconference_add_party_rpt_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_remove_party_rpt_tcp_init( clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt);
void clicktoconference_remove_party_rpt_tcp_final( clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt);
int clicktoconference_remove_party_rpt_encode_to_dbif_msg( clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt, uxc_dbif_t *dbif);
void clicktoconference_remove_party_rpt_tcp_display(clicktoconference_remove_party_rpt_tcp_t *clicktoconference_remove_party_rpt);
void clicktoconference_remove_party_rpt_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_change_party_media_rpt_tcp_init( clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt);
void clicktoconference_change_party_media_rpt_tcp_final( clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt);
int clicktoconference_change_party_media_rpt_encode_to_dbif_msg( clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt, uxc_dbif_t *dbif);
void clicktoconference_change_party_media_rpt_tcp_display(clicktoconference_change_party_media_rpt_tcp_t *clicktoconference_change_party_media_rpt);
void clicktoconference_change_party_media_rpt_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_change_option_rpt_tcp_init( clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt);
void clicktoconference_change_option_rpt_tcp_final( clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt);
void clicktoconference_change_option_rpt_tcp_display(clicktoconference_change_option_rpt_tcp_t *clicktoconference_change_option_rpt);

int clicktoconference_stop_rpt_tcp_init( clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt);
void clicktoconference_stop_rpt_tcp_final( clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt);
int clicktoconference_stop_rpt_encode_to_dbif_msg( clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt, uxc_dbif_t *dbif);
void clicktoconference_stop_rpt_tcp_display(clicktoconference_stop_rpt_tcp_t *clicktoconference_stop_rpt);
void clicktoconference_stop_rpt_dbif_display(uxc_dbif_t *dbif);

#endif // #ifndef __CLICKTOCONFERENCE_RPT_H__
