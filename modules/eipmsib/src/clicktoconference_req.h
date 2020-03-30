#ifndef __CLICKTOCONFERENCE_REQ_H__
#define __CLICKTOCONFERENCE_REQ_H__

#include <string.h>
#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Request Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktoconference_binding_req_tcp_s clicktoconference_binding_req_tcp_t;
struct clicktoconference_binding_req_tcp_s {
	char userID[BINDING_USER_ID_LEN];
	char password[BINDING_PASSWORD_LEN];
};

typedef struct clicktoconference_start_req_tcp_s clicktoconference_start_req_tcp_t;
struct clicktoconference_start_req_tcp_s {
	char subject[SUBSCRIBER_NAME_LEN];
    char chairmanName[CHAIRMAN_NAME_LEN];
    char chairmanNumber[CALL_NUMBER_LEN];
	char chargingNumber[CHARGING_NUMBER_LEN];
    int8_t confType;
	int8_t participantScenario;
    int8_t filler1;
    int8_t cidType;
    char chairmanCID[CID_LEN];
    char startTime[TIME_LEN];
    int8_t smsNoti;
    int8_t voiceNoti;
    int8_t isRecording;
    int8_t hostingCode;
    char password[PASSWORD_LEN];
    int8_t serviceMentGroupID;
	int8_t wirelessTimeout;
	int8_t wiredTimeout;
    int8_t filler2;
    int32_t callingCategoryType;
    char msp_id[4];
    char sp_id[4];
    char service_id[4];
    char participantList[MAX_PARTICIPANT_LIST_LEN];
};

typedef struct clicktoconference_add_party_req_tcp_s clicktoconference_add_party_req_tcp_t;
struct clicktoconference_add_party_req_tcp_s {
	char confID[CONF_ID_LEN];
    char participantList[MAX_PARTICIPANT_LIST_LEN];
};

typedef struct clicktoconference_remove_party_req_tcp_s clicktoconference_remove_party_req_tcp_t;
struct clicktoconference_remove_party_req_tcp_s {
	char confID[CONF_ID_LEN];
    char participantNumber[CALL_NUMBER_LEN];
};

typedef struct clicktoconference_change_party_media_req_tcp_s clicktoconference_change_party_media_req_tcp_t;
struct clicktoconference_change_party_media_req_tcp_s {
	char confID[CONF_ID_LEN];
    char participantNumber[CALL_NUMBER_LEN];
    int32_t mediaType;
};

typedef struct clicktoconference_change_option_req_tcp_s clicktoconference_change_option_req_tcp_t;
struct clicktoconference_change_option_req_tcp_s {
	char confID[CONF_ID_LEN];
    char password[PASSWORD_LEN];
};

typedef struct clicktoconference_get_number_of_party_req_tcp_s clicktoconference_get_number_of_party_req_tcp_t;
struct clicktoconference_get_number_of_party_req_tcp_s {
	char confID[CONF_ID_LEN];
};

typedef struct clicktoconference_stop_req_tcp_s clicktoconference_stop_req_tcp_t;
struct clicktoconference_stop_req_tcp_s {
	char confID[CONF_ID_LEN];
};

typedef struct clicktoconference_play_ment_req_tcp_s clicktoconference_play_ment_req_tcp_t;
struct clicktoconference_play_ment_req_tcp_s {
	char confID[CONF_ID_LEN];
    char aupkg[AUPKG_LEN];
};

typedef struct clicktoconference_get_party_status_req_tcp_s clicktoconference_get_party_status_req_tcp_t;
struct clicktoconference_get_party_status_req_tcp_s {
	char confID[CONF_ID_LEN];
    char participantName[PARTICIPANT_NAME_LEN];
    char participantNumber[PARTICIPANT_NUMBER_LEN];
};

typedef struct clicktoconference_cancel_party_req_tcp_s clicktoconference_cancel_party_req_tcp_t;
struct clicktoconference_cancel_party_req_tcp_s {
	char confID[CONF_ID_LEN];
    char participantNumber[PARTICIPANT_NUMBER_LEN];
};

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Request Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_binding_req_tcp_init( clicktoconference_binding_req_tcp_t *clicktoconference_binding_req);
void clicktoconference_binding_req_tcp_final( clicktoconference_binding_req_tcp_t *clicktoconference_binding_req);
void clicktoconference_binding_req_tcp_display(clicktoconference_binding_req_tcp_t *clicktoconference_binding_req);

int clicktoconference_start_req_tcp_init( clicktoconference_start_req_tcp_t *clicktoconference_start_req);
void clicktoconference_start_req_tcp_final( clicktoconference_start_req_tcp_t *clicktoconference_start_req);
int clicktoconference_start_req_decode_dbif_msg( clicktoconference_start_req_tcp_t *clicktoconference_start_req, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void clicktoconference_start_req_tcp_display(clicktoconference_start_req_tcp_t *clicktoconference_start_req);
void clicktoconference_start_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_add_party_req_tcp_init( clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req);
void clicktoconference_add_party_req_tcp_final( clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req);
int clicktoconference_add_party_req_decode_dbif_msg( clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req, uxc_dbif_t *dbif);
void clicktoconference_add_party_req_tcp_display(clicktoconference_add_party_req_tcp_t *clicktoconference_add_party_req);
void clicktoconference_add_party_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_remove_party_req_tcp_init( clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req);
void clicktoconference_remove_party_req_tcp_final( clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req);
int clicktoconference_remove_party_req_decode_dbif_msg( clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req, uxc_dbif_t *dbif);
void clicktoconference_remove_party_req_tcp_display(clicktoconference_remove_party_req_tcp_t *clicktoconference_remove_party_req);
void clicktoconference_remove_party_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_change_party_media_req_tcp_init( clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req);
void clicktoconference_change_party_media_req_tcp_final( clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req);
int clicktoconference_change_party_media_req_decode_dbif_msg( clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req, uxc_dbif_t *dbif);
void clicktoconference_change_party_media_req_tcp_display(clicktoconference_change_party_media_req_tcp_t *clicktoconference_change_party_media_req);
void clicktoconference_change_party_media_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_change_option_req_tcp_init( clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req);
void clicktoconference_change_option_req_tcp_final( clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req);
void clicktoconference_change_option_req_tcp_display(clicktoconference_change_option_req_tcp_t *clicktoconference_change_option_req);

int clicktoconference_get_number_of_party_req_tcp_init( clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req);
void clicktoconference_get_number_of_party_req_tcp_final( clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req);
int clicktoconference_get_number_of_party_req_decode_dbif_msg( clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req, uxc_dbif_t *dbif);
void clicktoconference_get_number_of_party_req_tcp_display(clicktoconference_get_number_of_party_req_tcp_t *clicktoconference_get_number_of_party_req);
void clicktoconference_get_number_of_party_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_stop_req_tcp_init( clicktoconference_stop_req_tcp_t *clicktoconference_stop_req);
void clicktoconference_stop_req_tcp_final( clicktoconference_stop_req_tcp_t *clicktoconference_stop_req);
int clicktoconference_stop_req_decode_dbif_msg( clicktoconference_stop_req_tcp_t *clicktoconference_stop_req, uxc_dbif_t *dbif);
void clicktoconference_stop_req_tcp_display(clicktoconference_stop_req_tcp_t *clicktoconference_stop_req);
void clicktoconference_stop_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_play_ment_req_tcp_init( clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req);
void clicktoconference_play_ment_req_tcp_final( clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req);
int clicktoconference_play_ment_req_decode_dbif_msg( clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req, uxc_dbif_t *dbif);
void clicktoconference_play_ment_req_tcp_display(clicktoconference_play_ment_req_tcp_t *clicktoconference_play_ment_req);
void clicktoconference_play_ment_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_get_party_status_req_tcp_init( clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req);
void clicktoconference_get_party_status_req_tcp_final( clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req);
int clicktoconference_get_party_status_req_decode_dbif_msg( clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req,  uxc_dbif_t *dbif);
void clicktoconference_get_party_status_req_tcp_display(clicktoconference_get_party_status_req_tcp_t *clicktoconference_get_party_status_req);
void clicktoconference_get_party_status_req_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_cancel_party_req_tcp_init( clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req);
void clicktoconference_cancel_party_req_tcp_final( clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req);
int clicktoconference_cancel_party_req_decode_dbif_msg( clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req, uxc_dbif_t *dbif);
void clicktoconference_cancel_party_req_tcp_display(clicktoconference_cancel_party_req_tcp_t *clicktoconference_cancel_party_req);
void clicktoconference_cancel_party_req_dbif_display(uxc_dbif_t *dbif);


#endif // #ifndef __CLICKTOCONFERENCE_REQ_H__
