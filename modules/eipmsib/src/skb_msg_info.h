#ifndef __SKB_MSG_INFO_H__
#define __SKB_MSG_INFO_H__

/** @brief define TCP channel info */
#define TCP_CHANNEL_CALL 										0
#define TCP_CHANNEL_RECORDING 									1
#define TCP_CHANNEL_CONFERENCE 									2

/** @brief define DBIF(GW) message ID */
#define DBIF_CALL_START_REQUEST									100
#define DBIF_CALL_STOP_REQUEST									101
#define DBIF_CALL_START_RECORDING_REQUEST						102
#define DBIF_CALL_STOP_RECORDING_REQUEST						103

#define DBIF_CALL_START_RESPONSE								200
#define DBIF_CALL_STOP_RESPONSE									201
#define DBIF_CALL_START_RECORDING_RESPONSE						202
#define DBIF_CALL_STOP_RECORDING_RESPONSE						203
#define DBIF_CALL_SERVICE_STATUS_REPORT							204
#define DBIF_CALL_END_REPORT									205

#define DBIF_CALL_RECORDING_START_REQUEST						300
#define DBIF_CALL_RECORDING_STOP_REQUEST						301

#define DBIF_CALL_RECORDING_START_RESPONSE						400
#define DBIF_CALL_RECORDING_STOP_RESPONSE						401
#define DBIF_RECORDING_CALL_SERVICE_STATUS_REPORT				402
#define DBIF_RECORDING_CALL_END_REPORT							403

#define DBIF_CONFERENCE_START_REQUEST							500
#define DBIF_CONFERENCE_STOP_REQUEST							501
#define DBIF_ADD_PARTY_REQUEST									502
#define DBIF_REMOVE_PARTY_REQUEST								503
#define DBIF_CANCEL_PARTY_REQUEST								504
#define DBIF_CHANGE_PARTY_MEDIA_REQUEST							505
#define DBIF_PLAY_MENT_REQUEST									506
#define DBIF_GET_NUMBER_OF_PARTY_REQUEST						507
#define DBIF_GET_PARTY_STATUS_REQUEST							508

#define DBIF_CONFERENCE_START_RESPONSE							600
#define DBIF_CONFERENCE_STOP_RESPONSE							601
#define DBIF_ADD_PARTY_RESPONSE								602
#define DBIF_REMOVE_PARTY_RESPONSE								603
#define DBIF_CANCEL_PARTY_RESPONSE								604
#define DBIF_CHANGE_PARTY_MEDIA_RESPONSE						605
#define DBIF_PLAY_MENT_RESPONSE								606
#define DBIF_GET_NUMBER_OF_PARTY_RESPONSE						607
#define DBIF_GET_PARTY_STATUS_RESPONSE							608
#define DBIF_ADD_PARTY_REPORT									609
#define DBIF_REMOVE_PARTY_REPORT								610
#define DBIF_CHANGE_PARTY_MEDIA_REPORT							611
#define DBIF_CLOSE_CONF_REPORT									612


/** @brief define TCP(eIPMS) message ID */
#define HEARTBEAT_REQUEST									0x00000000
#define HEARTBEAT_RESPONSE									0x10000000
#define BINDING_REQUEST										0x00000001
#define BINDING_RESPONSE									0x10000001
#define START_REQUEST										0x00000010
#define START_RESPONSE										0x10000010
#define START_REPORT										0x20000010
#define STOP_REQUEST										0x00000011
#define STOP_RESPONSE										0x10000011
#define STOP_REPORT											0x20000011
#define START_RECORDING_REQUEST								0x00000020
#define START_RECORDING_RESPONSE							0x10000020
#define START_RECORDING_REPORT								0x20000020
#define STOP_RECORDING_REQUEST								0x00000021
#define STOP_RECORDING_RESPONSE								0x10000021
#define STOP_RECORDING_REPORT								0x20000021
#define SERVICE_STATUS_REQUEST								0x00000030
#define SERVICE_STATUS_RESPONSE								0x10000030
#define SERVICE_STATUS_REPORT								0x20000030
#define START_CONFERENCE_REQUEST                            0x00000010
#define START_CONFERENCE_RESPONSE                           0x10000010
#define ADD_PARTY_REQUEST                                   0x00000020
#define ADD_PARTY_RESPONSE                                  0x10000020
#define ADD_PARTY_REPORT                                    0x20000020
#define REMOVE_PARTY_REQUEST                                0x00000030
#define REMOVE_PARTY_RESPONSE                               0x10000030
#define REMOVE_PARTY_REPORT                                 0x20000030
#define CHANGE_PARTY_MEDIA_REQUEST                          0x00000040
#define CHANGE_PARTY_MEDIA_RESPONSE                         0x10000040
#define CHANGE_PARTY_MEDIA_REPORT                           0x20000040
#define CHANGE_OPTION_REQUEST                               0x00000050
#define CHANGE_OPTION_RESPONSE                              0x10000050
#define CHANGE_OPTION_REPORT                                0x20000050
#define GET_NUMBER_OF_PARTY_REQUEST                         0x00000060
#define GET_NUMBER_OF_PARTY_RESPONSE                        0x10000060
#define STOP_CONFERENCE_REQUEST                             0x00000070
#define STOP_CONFERENCE_RESPONSE                            0x10000070
#define STOP_CONFERENCE_REPORT                              0x20000070
#define PLAY_MENT_REQUEST                                   0x00000080
#define PLAY_MENT_RESPONSE                                  0x10000080
#define GET_PARTY_STATUS_REQUEST                            0x00000090
#define GET_PARTY_STATUS_RESPONSE                           0x10000090
#define CANCEL_PARTY_REQUEST                                0x000000A0
#define CANCEL_PARTY_RESPONSE                               0x100000A0


/** @brief SKB MAX Message Length */
#define SKB_MSG_MAX_LEN										64*1024
#define SESSION_ID_LEN 										32
#define GW_SESSION_ID_LEN 									32
#define SUBSCRIBER_NAME_LEN									32
#define CALL_NUMBER_LEN										24
#define CHARGING_NUMBER_LEN									24
#define SERVICE_ID_LEN										32
#define CID_LEN												16
#define RECORDING_FILE_URL_LEN								64
#define RECORDING_FILE_NAME_LEN								64
#define TIME_LEN	        								16
#define CALLER_INFO_LEN                                     24
#define CALLEE_INFO_LEN                                     24
#define SUBJECT_LEN                                         64
#define CHAIRMAN_NAME_LEN                                   20
#define PASSWORD_LEN                                        16
#define PARTICIPANT_NAME_LEN                                32
#define MAX_PARTICIPANT_LEN                                 32
// v0.4에서 참여자 리스트의 최대 크기는 32명으로 제한한다. 추가로 회의에 참여시킬 참여자가 있는 경우는, 회의가 생성되고 주관자가 회의에 참여하여 완전히 회의가 개시된 이후에 참여자 추가 요청을 사용한다.
// 참여자 간의 구분은 ’|’ 문자를 사용한다.
// 참여자 정보는 이름, 전화번호로 이루어 지며, 이 같은 참여자 정보 간의 구분은 ‘^’ 문자를 사용한다. 전화 번호는 유선 전화 번호와 무선 전화번호 중 하나의 정보를 반드시 입력한다. 리스트 중 마지막 참여자의 정보 뒤에는 Null Termination 처리한다.
#define MAX_PARTICIPANT_LIST_LEN                            (MAX_PARTICIPANT_LEN+1) * (CALL_NUMBER_LEN + PARTICIPANT_NAME_LEN + 1) - 1 + 1
#define CONF_ID_LEN                                         32
#define AUPKG_LEN                                           128
#define PARTICIPANT_NUMBER_LEN                              32
#define BINDING_USER_ID_LEN                                 16
#define BINDING_PASSWORD_LEN                                16

#endif // #ifndef __SKB_MSG_INFO_H__