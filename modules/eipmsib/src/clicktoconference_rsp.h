#ifndef __CLICKTOCONFERENCE_RSP_H__
#define __CLICKTOCONFERENCE_RSP_H__

#include <uxcutor/uxc_dbif.h>
#include <uxlib/ux_errno.h>
#include <uxlib/ux_log.h>
#include <uxcutor/uxcutor.h>
#include <upa/upa_tcp.h>
#include "skb_msg_info.h"

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Response Message Structure & Macro 
///////////////////////////////////////////////////////////////////////////////////

typedef struct clicktoconference_start_rsp_tcp_s clicktoconference_start_rsp_tcp_t;
struct clicktoconference_start_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char chairmanName[CHAIRMAN_NAME_LEN];
    char chairmanNumber[CALL_NUMBER_LEN];
	char chargingNumber[CHARGING_NUMBER_LEN];
    int8_t confType;
    int8_t recordingFilePath;
    int16_t filler1;
    char startTime[TIME_LEN];
    int8_t smsNoti;
    int8_t voiceNoti;
    int16_t filler2;
    int32_t recordingFileID;
    char recordingFileURL[RECORDING_FILE_URL_LEN];
    char recordingFileName[RECORDING_FILE_NAME_LEN];
    char password[PASSWORD_LEN];
    int32_t serviceMentID;
    int32_t callingCategoryType;
    char msp_id[4];
    char sp_id[4];
    char service_id[4];
};

typedef struct clicktoconference_add_party_rsp_tcp_s clicktoconference_add_party_rsp_tcp_t;
struct clicktoconference_add_party_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantList[MAX_PARTICIPANT_LIST_LEN];
};

typedef struct clicktoconference_remove_party_rsp_tcp_s clicktoconference_remove_party_rsp_tcp_t;
struct clicktoconference_remove_party_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantNumber[CALL_NUMBER_LEN];
};

typedef struct clicktoconference_change_party_media_rsp_tcp_s clicktoconference_change_party_media_rsp_tcp_t;
struct clicktoconference_change_party_media_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantNumber[CALL_NUMBER_LEN];
    int32_t mediaType;
};

typedef struct clicktoconference_change_option_rsp_tcp_s clicktoconference_change_option_rsp_tcp_t;
struct clicktoconference_change_option_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char password[PASSWORD_LEN];
};

typedef struct clicktoconference_get_number_of_party_rsp_tcp_s clicktoconference_get_number_of_party_rsp_tcp_t;
struct clicktoconference_get_number_of_party_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
	int32_t number;
};

typedef struct clicktoconference_stop_rsp_tcp_s clicktoconference_stop_rsp_tcp_t;
struct clicktoconference_stop_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char startTime[TIME_LEN];
    char endTime[TIME_LEN];
    int32_t recordingFileID;
};

typedef struct clicktoconference_play_ment_rsp_tcp_s clicktoconference_play_ment_rsp_tcp_t;
struct clicktoconference_play_ment_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
};

typedef struct clicktoconference_get_party_status_rsp_tcp_s clicktoconference_get_party_status_rsp_tcp_t;
struct clicktoconference_get_party_status_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantName[PARTICIPANT_NAME_LEN];
    char participantNumber[PARTICIPANT_NUMBER_LEN];
};

typedef struct clicktoconference_cancel_party_rsp_tcp_s clicktoconference_cancel_party_rsp_tcp_t;
struct clicktoconference_cancel_party_rsp_tcp_s {
	int32_t resultCode;
	char confID[CONF_ID_LEN];
    char participantNumber[PARTICIPANT_NUMBER_LEN];
};

///////////////////////////////////////////////////////////////////////////////////
// ClickToConference Response Message Functions 
///////////////////////////////////////////////////////////////////////////////////

int clicktoconference_start_rsp_tcp_init( clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp);
void clicktoconference_start_rsp_tcp_final( clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp);
int clicktoconference_start_rsp_encode_to_dbif_msg( clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp, char *sessionID, char *gwSessionID, uxc_dbif_t *dbif);
void clicktoconference_start_rsp_tcp_display(clicktoconference_start_rsp_tcp_t *clicktoconference_start_rsp);
void clicktoconference_start_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_add_party_rsp_tcp_init( clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp);
void clicktoconference_add_party_rsp_tcp_final( clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp);
int clicktoconference_add_party_rsp_encode_to_dbif_msg( clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp, uxc_dbif_t *dbif);
void clicktoconference_add_party_rsp_tcp_display(clicktoconference_add_party_rsp_tcp_t *clicktoconference_add_party_rsp);
void clicktoconference_add_party_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_remove_party_rsp_tcp_init( clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp);
void clicktoconference_remove_party_rsp_tcp_final( clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp);
int clicktoconference_remove_party_rsp_encode_to_dbif_msg( clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp, uxc_dbif_t *dbif);
void clicktoconference_remove_party_rsp_tcp_display(clicktoconference_remove_party_rsp_tcp_t *clicktoconference_remove_party_rsp);
void clicktoconference_remove_party_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_change_party_media_rsp_tcp_init( clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp);
void clicktoconference_change_party_media_rsp_tcp_final( clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp);
int clicktoconference_change_party_media_rsp_encode_to_dbif_msg( clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp, uxc_dbif_t *dbif);
void clicktoconference_change_party_media_rsp_tcp_display(clicktoconference_change_party_media_rsp_tcp_t *clicktoconference_change_party_media_rsp);
void clicktoconference_change_party_media_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_change_option_rsp_tcp_init( clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp);
void clicktoconference_change_option_rsp_tcp_final( clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp);
void clicktoconference_change_option_rsp_tcp_display(clicktoconference_change_option_rsp_tcp_t *clicktoconference_change_option_rsp);

int clicktoconference_get_number_of_party_rsp_tcp_init( clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp);
void clicktoconference_get_number_of_party_rsp_tcp_final( clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp);
int clicktoconference_get_number_of_party_rsp_encode_to_dbif_msg( clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp, uxc_dbif_t *dbif);
void clicktoconference_get_number_of_party_rsp_tcp_display(clicktoconference_get_number_of_party_rsp_tcp_t *clicktoconference_get_number_of_party_rsp);
void clicktoconference_get_number_of_party_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_stop_rsp_tcp_init( clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp);
void clicktoconference_stop_rsp_tcp_final( clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp);
int clicktoconference_stop_rsp_encode_to_dbif_msg( clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp, uxc_dbif_t *dbif);
void clicktoconference_stop_rsp_tcp_display(clicktoconference_stop_rsp_tcp_t *clicktoconference_stop_rsp);
void clicktoconference_stop_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_play_ment_rsp_tcp_init( clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp);
void clicktoconference_play_ment_rsp_tcp_final( clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp);
int clicktoconference_play_ment_rsp_encode_to_dbif_msg( clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp, uxc_dbif_t *dbif);
void clicktoconference_play_ment_rsp_tcp_display(clicktoconference_play_ment_rsp_tcp_t *clicktoconference_play_ment_rsp);
void clicktoconference_play_ment_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_get_party_status_rsp_tcp_init( clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp);
void clicktoconference_get_party_status_rsp_tcp_final( clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp);
int clicktoconference_get_party_status_rsp_encode_to_dbif_msg( clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp,  uxc_dbif_t *dbif);
void clicktoconference_get_party_status_rsp_tcp_display(clicktoconference_get_party_status_rsp_tcp_t *clicktoconference_get_party_status_rsp);
void clicktoconference_get_party_status_rsp_dbif_display(uxc_dbif_t *dbif);

int clicktoconference_cancel_party_rsp_tcp_init( clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp);
void clicktoconference_cancel_party_rsp_tcp_final( clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp);
int clicktoconference_cancel_party_rsp_encode_to_dbif_msg( clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp, uxc_dbif_t *dbif);
void clicktoconference_cancel_party_rsp_tcp_display(clicktoconference_cancel_party_rsp_tcp_t *clicktoconference_cancel_party_rsp);
void clicktoconference_cancel_party_rsp_dbif_display(uxc_dbif_t *dbif);


#endif // #ifndef __CLICKTOCONFERENCE_RSP_H__
