#ifndef __UIMS_CDR_H__
#define __UIMS_CDR_H__

#include <uxlib/ux_log.h>
#include <uxlib/ux_string.h>
#include <uxlib/ux_hash.h>
#include <uxlib/ux_memory.h>
#include <uxlib/ux_time.h>
#include <uxcutor/uxcutor.h>
#include <uxcutor/uxc_cdrmgr.h>
#include <uxcutor/uxc_cdrworker.h>
#include <uxcutor/uxc_cdrconf.h>
#include <upa/upa_sippa.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @ingroup uims_cdr
 * @file
 * @brief UIMS cdr
 */

// $PRINT_TIME(value, format, usec) / $PRINT_TIME(start_time, %Y%m%d%H%m%S, 3)
// $PRINT_STIME(value, usec)        / $PRINT_STIME(duration, 3)
// $STRING(define)        			 / $PRINT_STIME(duration, 3)

/*
[UCDR/COMMON]
THREAD_COUNT = 1 		# mandatory
CDR_MAX_SIZE = 10240 	# default
QUEUE_SIZE = 10240 		# default
USE_SEQ_NUM = OFF 		# default

// 추가 //
UIMS_LIST=IBCF;PCSCF;SCSCF


[UCDR/UIMS_IBCF]
THREAD_IDX = 0 			# mandatory
MAX_FILE_SIZE = 1024	# default (byte??)
MAX_TIME_PERIOD = 86400	# default (second??)
FILE_FLAGS = NULL # default (etc : BINARY)
FILE_FORMAT = NULL # default
DIR_MODE = 0755 # default
FILE_MODE = 0666 # default
CDR_DATA_DIR = ./uims/ibcf # mandatory
CDR_TEMP_DIR = ./uims/ibcf # mandatory
CDR_SEQ_FILE = NULL # condition with use_seq for seq_path
CDR_MAPPER = NULL # default

// 추가 //
DELIMITER = ;  # mandatory
FIELDS = 	start_time; end_time; duration; in_peer; out_peer; from; from_tag; to; to_tag; call_id; status;
			reason; pcharging_id; pcharging_idgen; pcharging_oioi; pcharging_tioi; sip.p-asserted-identity
FIELDS = 	$PRINT_TIME(start_time, %Y%m%d%H%m%S, 3); $PRINT_TIME(end_time, %Y%m%d%H%m%S, 3); 
			$PRINT_STIME(duration, 3); in_peer; out_peer; from; from_tag; to; to_tag; call_id; status; 
			reason; pcharging_id; pcharging_idgen; pcharging_oioi; pcharging_tioi; sip.p-asserted-identity; reqmsg.a[0].b.c


[UXCUTOR/PLUGIN/UIMS/CDR]
LIST=IBCF;PCSCF;SCSCF

[UXCUTOR/PLUGIN/UIMS/CDR/IBCF]
FILE_NAME = $(UASYS_HOME)/cdr/ibcf/cdrs  # mandatory final( $(UASYS_HOME)/cdr/ibcf/cdrs_YYYYMMDDhhmm_????.cdr)
DELIMITER = ;  # mandatory
FILE_SIZE = 0  # default 0 (unlimit)
FILE_LIFECYCLE = 0  # default 3600, unit : second
FIELDS = 	start_time; end_time; duration; in_peer; out_peer; from; from_tag; to; to_tag; call_id; status;
			reason; pcharging_id; pcharging_idgen; pcharging_oioi; pcharging_tioi; sip.p-asserted-identity
FIELDS = 	$PRINT_TIME(start_time, %Y%m%d%H%m%S, 3); $PRINT_TIME(end_time, %Y%m%d%H%m%S, 3); 
			$PRINT_STIME(duration, 3); in_peer; out_peer; from; from_tag; to; to_tag; call_id; status; 
			reason; pcharging_id; pcharging_idgen; pcharging_oioi; pcharging_tioi; sip.p-asserted-identity
*/

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// UIMS cdr types & macros
////////////////////////////////////////////////////////////////////////////////
#define UIMS_CDR_MAX_NAMELEN 	64
#define UIMS_CDR_MAX_PATHLEN 	256
#define UIMS_CDR_MAX_FIELDS 	128
#define UIMS_CDR_MAX_CDRBUFF 	4096
#define UIMS_CDR_INVALID_INT 	-9876

typedef enum uims_cdrpara_e {
	UIMS_CDRPARA_UNKNOWN = 0,
	UIMS_CDRPARA_STIME,			// start
	UIMS_CDRPARA_ETIME,			// end
	UIMS_CDRPARA_DURATION,		// end
	UIMS_CDRPARA_IN_PEER,		// start
	UIMS_CDRPARA_OUT_PEER,		// update, end
	UIMS_CDRPARA_FROM,			// start
	UIMS_CDRPARA_FROMTAG,		// start
	UIMS_CDRPARA_TO, 			// start
	UIMS_CDRPARA_TOTAG,			// update, end
	UIMS_CDRPARA_CALLID, 		// start
	UIMS_CDRPARA_STATUS,		// all
	UIMS_CDRPARA_REASON,		// end
	UIMS_CDRPARA_PCHRG_VCT,		// start, end
	UIMS_CDRPARA_SIPREQ,		// all
	UIMS_CDRPARA_SIPRES,		// all
	UIMS_CDRPARA_USER_FUNC,

	UIMS_CDRPARA_OPER_EQUAL,
	UIMS_CDRPARA_OPER_NEQUAL,
	UIMS_CDRPARA_OPER_BIGGER,
	UIMS_CDRPARA_OPER_EBIGGER,
	UIMS_CDRPARA_OPER_SMALL,
	UIMS_CDRPARA_OPER_ESMALL,

	UIMS_CDRPARA_MAX
} uims_cdrpara_e;

typedef enum uims_cdrfieldtype_e {
	UIMS_CDRFILDTYPE_UNKNOWN = 0,
	UIMS_CDRFILDTYPE_PARA,
	UIMS_CDRFILDTYPE_FUNC,
	UIMS_CDRFILDTYPE_STRING,
	UIMS_CDRFILDTYPE_INT,
	UIMS_CDRFILDTYPE_MAX
} uims_cdrfieldtype_e;

typedef enum uims_cdrufunc_e {
	UIMS_CDRUFUNC_UNKNOWN = 0,
	UIMS_CDRUFUNC_PRINT_TIME,
	UIMS_CDRUFUNC_PRINT_STIME,
	UIMS_CDRUFUNC_PRINT_IF,
	UIMS_CDRUFUNC_MAX
} uims_cdrufunc_e;

typedef struct uims_cdrufunc_s uims_cdrufunc_t; 
typedef struct uims_cdrfield_s uims_cdrfield_t; 
typedef struct uims_cdrinfo_s uims_cdrinfo_t; 
typedef struct uims_cdrmgr_s uims_cdrmgr_t; 
typedef struct uims_cdrbuff_s uims_cdrbuff_t; 


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrpara_e 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_cdrpara_to_str( uims_cdrpara_e para);
UX_DECLARE(uims_cdrpara_e) uims_cdrpara_to_enum( char *str);


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrfieldtype_e 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_cdrfieldtype_to_str( uims_cdrfieldtype_e type);
UX_DECLARE(uims_cdrfieldtype_e) uims_cdrfieldtype_to_enum( char *str);


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrufunc_e 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_cdrufunc_to_str( uims_cdrufunc_e func);
UX_DECLARE(uims_cdrufunc_e) uims_cdrufunc_to_enum( char *str);


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrmgr_t 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_cdrmgr_t*) uims_cdrmgr_create( const char *cfile);
UX_DECLARE(void) uims_cdrmgr_destroy( uims_cdrmgr_t *cdrmgr);

UX_DECLARE(ux_status_t) uims_cdrmgr_load( uims_cdrmgr_t *cdrmgr);
UX_DECLARE(uims_cdrinfo_t*) uims_cdrmgr_get_info( uims_cdrmgr_t *cdrmgr, char *name);

UX_DECLARE(ux_status_t) uims_cdrmgr_start( uims_cdrmgr_t *cdrmgr);
UX_DECLARE(ux_status_t) uims_cdrmgr_stop( uims_cdrmgr_t *cdrmgr);
UX_DECLARE(ux_status_t) uims_cdrmgr_join( uims_cdrmgr_t *cdrmgr);


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrinfo_t 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_cdrinfo_t*) uims_cdrinfo_ref( uims_cdrinfo_t *cdrinfo);
UX_DECLARE(void) uims_cdrinfo_unref( uims_cdrinfo_t *cdrinfo);
UX_DECLARE(void) uims_cdrinfo_write( uims_cdrinfo_t *cdrinfo, uims_cdrbuff_t *cdrbuff);


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrbuff_t 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_cdrbuff_t*) uims_cdrbuff_create( uims_cdrinfo_t *cdrinfo);
UX_DECLARE(void) uims_cdrbuff_destroy( uims_cdrbuff_t *cdrbuff);

UX_DECLARE(void) uims_cdrbuff_start( uims_cdrbuff_t *cdrbuff, usip_msg_t *msg, char *in_peer);
UX_DECLARE(void) uims_cdrbuff_update( uims_cdrbuff_t *cdrbuff, usip_msg_t *reqmsg, usip_msg_t *resmsg, char *out_peer);
UX_DECLARE(void) uims_cdrbuff_update_status( uims_cdrbuff_t *cdrbuff, usip_msg_t *msg, ux_status_t is_send);
UX_DECLARE(void) uims_cdrbuff_end( uims_cdrbuff_t *cdrbuff, usip_msg_t *msg, char *out_peer, ux_status_t is_send);
UX_DECLARE(char*) uims_cdrbuff_get_factor( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *cdrfield);


#ifdef __cplusplus
}
#endif

#endif	// #ifndef __UIMS_CONF_H__
