#ifndef __IPMSSIM_MSG_H__
#define __IPMSSIM_MSG_H__


#define IPMSSIM_VERSION 				0x00000001
#define IPMSSIM_SYSID					"ipmssim"

#define IPMSSIM_MSGID_STS_INQ			1
#define	IPMSSIM_MSGID_STS_INQ_ACK		2
#define IPMSSIM_MSGID_USG_REP			3
#define IPMSSIM_MSGID_USG_REP_ACK		4
#define IPMSSIM_MSGID_BAL_REP			7
#define IPMSSIM_MSGID_BAL_REP_ACK		8
#define IPMSSIM_MSGID_STS_INQ_STR		"STATUS_INQUERY"
#define IPMSSIM_MSGID_STS_INQ_ACK_STR	"STATUS_INQUERY_ACK"
#define IPMSSIM_MSGID_USG_REP_STR		"USAGE_REPORT"
#define IPMSSIM_MSGID_USG_REP_ACK_STR	"USAGE_REPORT_ACK"
#define IPMSSIM_MSGID_BAL_REP_STR		"BALANCE_REPORT"
#define IPMSSIM_MSGID_BAL_REP_ACK_STR	"BALANCE_REPORT_ACK"
#define ipmssim_msgid_str( _msgid ) ( 											\
		(_msgid) == IPMSSIM_MSGID_STS_INQ ? IPMSSIM_MSGID_STS_INQ_STR : 			\
		(_msgid) == IPMSSIM_MSGID_STS_INQ_ACK ? IPMSSIM_MSGID_STS_INQ_ACK_STR : 	\
		(_msgid) == IPMSSIM_MSGID_USG_REP ? IPMSSIM_MSGID_USG_REP_STR : 			\
		(_msgid) == IPMSSIM_MSGID_USG_REP_ACK ? IPMSSIM_MSGID_USG_REP_ACK_STR : 	\
		(_msgid) == IPMSSIM_MSGID_BAL_REP ? IPMSSIM_MSGID_BAL_REP_STR : 			\
		(_msgid) == IPMSSIM_MSGID_BAL_REP_ACK ? IPMSSIM_MSGID_BAL_REP_ACK_STR : 	\
		"Unknown ID" )
#define ipmssim_msgid_str_maxlen() 		(strlen(IPMSSIM_MSGID_BAL_REP_ACK_STR))

// usage type for ibrsib
#define IPMSSIM_USGTYPE_CDMA			1
#define IPMSSIM_USGTYPE_WCDMA			2
#define IPMSSIM_USGTYPE_ROAMING_VOICE	3
#define IPMSSIM_USGTYPE_ROAMING_DATA	4
#define IPMSSIM_USGTYPE_SMS				5
#define IPMSSIM_USGTYPE_PPS				6
#define IPMSSIM_USGTYPE_VOLTE			7
#define IPMSSIM_USGTYPE_RECEIVER		15
// usage type for mvrsib
#define IPMSSIM_USGTYPE_MVRS_SENDER		8	
#define IPMSSIM_USGTYPE_MVRS_RECEIVER	IPMSSIM_USGTYPE_RECEIVER	
#define IPMSSIM_USGTYPE_MVRS_VOICE		17
#define IPMSSIM_USGTYPE_MVRS_SMS		18
#define IPMSSIM_USGTYPE_MVRS_DAILY		19
#define IPMSSIM_USGTYPE_MVRS_RECHARGE	20
// usage type for srggwib
#define IPMSSIM_USGTYPE_LTEDATA_START	100
#define IPMSSIM_USGTYPE_LTEDATA_END		199	
#define ipmssim_usgtype_is_ltedata( _usgtype ) (						\
		((_usgtype) >= IPMSSIM_USGTYPE_LTEDATA_START &&					\
		 (_usgtype) <= IPMSSIM_USGTYPE_LTEDATA_END))  
#define ipmssim_usgtype_str( _usgtype ) (								\
		(_usgtype) == IPMSSIM_USGTYPE_CDMA ? "CDMA" :					\
		(_usgtype) == IPMSSIM_USGTYPE_WCDMA ? "WCDMA" :					\
		(_usgtype) == IPMSSIM_USGTYPE_ROAMING_VOICE ? "ROAMING VOICE" :	\
		(_usgtype) == IPMSSIM_USGTYPE_ROAMING_DATA ? "ROAMING DATA" :	\
		(_usgtype) == IPMSSIM_USGTYPE_SMS ? "SMS" :						\
		(_usgtype) == IPMSSIM_USGTYPE_PPS ? "PPS" :						\
		(_usgtype) == IPMSSIM_USGTYPE_VOLTE ? "VOLTE" :					\
		(_usgtype) == IPMSSIM_USGTYPE_RECEIVER ? "RECEIVER" :			\
		(_usgtype) == IPMSSIM_USGTYPE_MVRS_SENDER ? "MVRS SENDER" :		\
		(_usgtype) == IPMSSIM_USGTYPE_MVRS_VOICE ? "MVRS VOICE" :		\
		(_usgtype) == IPMSSIM_USGTYPE_MVRS_SMS ? "MVRS SMS" :			\
		(_usgtype) == IPMSSIM_USGTYPE_MVRS_DAILY ? "MVRS DAILY" : 		\
		(_usgtype) == IPMSSIM_USGTYPE_MVRS_RECHARGE ? "MVRS RECHARGE" :	\
		ipmssim_usgtype_is_ltedata((_usgtype)) ? "LTE DATA" : "Unknown Type" )

#define eIPMSSIM_NO_ERROR			 	0 
#define eIPMSSIM_INVALID_VERSION		-1
#define eIPMSSIM_INVALID_MESSAGE_ID		-2
#define eIPMSSIM_XML_PARSE_ERR			-3
#define eIPMSSIM_INVALID_SERVICE		-4
#define eIPMSSIM_INVALID_ARGUMENT		-5
#define eIPMSSIM_SYSTEM_FAIL			-6
#define eIPMSSIM_TIMEOUT				-100 // ���� ���� ���� �ڵ�
#define eIPMSSIM_INVALID_STATUS			-101 // ���� ���� ���� �ڵ�
#define eIPMSSIM_DUPLICATE_MESSAGE		-102 // ���� ���� ���� �ڵ�
#define ipmssim_result_str( _err ) (										\
		(_err) == eIPMSSIM_NO_ERROR? "No error" :						\
		(_err) == eIPMSSIM_INVALID_VERSION ? "Invalid version" : 		\
		(_err) == eIPMSSIM_INVALID_MESSAGE_ID ? "Invalid message id" : 	\
		(_err) == eIPMSSIM_XML_PARSE_ERR ? "XML parsing error" : 		\
		(_err) == eIPMSSIM_INVALID_SERVICE ? "Invalid service" : 		\
		(_err) == eIPMSSIM_INVALID_ARGUMENT ? "Invalid argment" : 		\
		(_err) == eIPMSSIM_SYSTEM_FAIL ? "System fail" : 				\
		(_err) == eIPMSSIM_TIMEOUT ? "Timeout" :							\
		(_err) == eIPMSSIM_INVALID_STATUS ? "Invalid status" : 			\
		(_err) == eIPMSSIM_DUPLICATE_MESSAGE ? "Duplicate message" :		\
		"Unknown" )


#define IPMSSIM_SYSID_SIZE		8
#define IPMSSIM_TRCID_SIZE		12
#define IPMSSIM_DATA_SIZE		40960
#define IPMSSIM_SEQ_MIN			0
#define IPMSSIM_SEQ_MAX			99999999

typedef struct ipmssim_msg_hdr_s ipmssim_msg_hdr_t;
struct ipmssim_msg_hdr_s {
	unsigned int version; /**< message version id */
	char sysid[IPMSSIM_SYSID_SIZE]; /**< system id */
	char trcid[IPMSSIM_TRCID_SIZE]; /**< transaction id */
	unsigned int msgid; /**< message id */
	unsigned int dummy; /**< dummy */
	time_t time; /**< time 8byte */
	unsigned int usgtype; /**< usage type */
	unsigned int result; /**< result code */
	unsigned int reserved; /**< reserved field */
	unsigned int length; /**< message length */
};

typedef struct ipmssim_msg_s ipmssim_msg_t;
struct ipmssim_msg_s {
	ipmssim_msg_hdr_t header; /**< message header */
	unsigned char data[IPMSSIM_DATA_SIZE]; /**< message body */
};


#define IPMSSIM_BALREP_MAX_USER		120	
#define IPMSSIM_BALREP_MAX_COUNTER	110

typedef struct ipmssim_msg_balrep_info_s ipmssim_msg_balrep_info_t;
struct ipmssim_msg_balrep_info_s {
	char month[16];
	char nuser;
	struct {
		char id[32];
		char mdn[32];
		char svc[32];
		char ncounter;
		struct {
			char name[32];
			long value;
			int scale;
			char pps_item_flag;
			struct {
				char chrg_time[32];
				int cdr_type;
				long chrg_amt;
				int scale;
			} pps_item;
		} counters[IPMSSIM_BALREP_MAX_COUNTER];
	} users[IPMSSIM_BALREP_MAX_USER];
};


void ipmssim_msg_cvt_order_ntoh( ipmssim_msg_t *msg );
void ipmssim_msg_cvt_order_hton( ipmssim_msg_t *msg );
int ipmssim_msg_enc_header( ipmssim_msg_t *msg, char *trcid, unsigned int msgid, time_t timeval, 
		unsigned int usgtype, unsigned int result, unsigned int reserved, unsigned int length );
int ipmssim_msg_enc_status( ipmssim_msg_t *msg, char *trcid, unsigned int usgtype, 
		unsigned int length );
int ipmssim_msg_enc_status_ack( ipmssim_msg_t *msg, ipmssim_msg_t *rcv, unsigned int result );
int ipmssim_msg_enc_usgrep_ack( ipmssim_msg_t *msg, ipmssim_msg_t *rcv, unsigned int result );
int ipmssim_msg_enc_balrep( ipmssim_msg_t *msg, char *trcid, unsigned int usgtype, 
		unsigned int length, const char *data );
int ipmssim_msg_enc_balrep_info( ipmssim_msg_t *msg, char *trcid, unsigned int usgtype, 
		ipmssim_msg_balrep_info_t *info );
char* ipmssim_msg_dec_header_sysid( ipmssim_msg_t *msg, char *sysid, size_t len );
char* ipmssim_msg_dec_header_trcid( ipmssim_msg_t *msg, char *trcid, size_t len );
char* ipmssim_msg_dec_header_time( ipmssim_msg_t *msg, const char *fmt, char *timestr, size_t len );
void ipmssim_msg_display( ipmssim_msg_t *msg );


#endif // #ifndef __IPMSSIM_MSG_H__
