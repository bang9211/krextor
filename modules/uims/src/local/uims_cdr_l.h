#ifndef __UIMS_CDR_L_H__
#define __UIMS_CDR_L_H__

#include "../uims/uims_cdr.h"

////////////////////////////////////////////////////////////////////////////////
// UIMS cdr internal types & definitions
////////////////////////////////////////////////////////////////////////////////

struct uims_cdrufunc_s {
	char *name; // cdr name for configuration
	uims_cdrufunc_e id;
	ux_status_t (*load)( uims_cdrufunc_t *func, char *buff);
	ux_status_t (*print)( uims_cdrufunc_t *func, uims_cdrbuff_t *cdrbuff, char *buff, int buflen);
	ux_status_t (*conf)( uims_cdrufunc_t *func, char *buff, int buflen);
	ux_vector_t paras[1];
};

struct uims_cdrfield_s {
	uims_cdrfieldtype_e type;
	uims_cdrpara_e id;
	int vint;
	char *vstr;
	
	int factorid;
	char *factorname;
	uims_cdrufunc_t *rfunc;
};

// for manager
struct uims_cdrinfo_s {
	ux_hnode_t hnode[1];
	ux_refcount_t refcnt[1]; /**< 참조 계수 */
	ux_vector_t fields[1]; // meta info
	char *name; // cdr name for configuration
	char delim[16]; // cdr delimeter

	ux_vector_t msgfactors[1]; // 각 상태별로 메시지에서 읽어야 하는 메시지 요소 define 
	ux_status_t isalive; // cdr status ??

	uxc_cdr_t *ucdr_handle;
	void *cdrmgr;
};

/** @brief uims cdr manager 구조체 */
struct uims_cdrmgr_s {
	const char* cfile; /**< configuration file */
	ux_hash_t cdrinfos[1]; // cdr info list
	uxc_cdrmgr_t *ucdr_mgr;
};

// field buff for session 
struct uims_cdrbuff_s {
	uims_cdrinfo_t *cdrinfo;
	ux_vector_t buffs[1];

	ux_time_t starttime[1];
	ux_time_t endtime[1];
	ux_time_t duration[1];
	char *in_peer;
	char *out_peer;
	char *from;
	char *fromtag;
	char *to;
	char *totag;
	char *callid;
	char *pcharging;
	int statuslen;
	int reason;
	char status[UIMS_CDR_MAX_PATHLEN];
};


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrmgr_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t uims_cdrmgr_init( uims_cdrmgr_t *cdrmgr, const char* cfile);
void uims_cdrmgr_final( uims_cdrmgr_t *cdrmgr);
// ux_status_t uims_cdrmgr_load( uims_cdrmgr_t *cdrmgr);
// void uims_cdrmgr_check( uims_cdrmgr_t *cdrmgr);

void uims_cdrmgr_print( uims_cdrmgr_t *cdrmgr);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrinfo_t 
////////////////////////////////////////////////////////////////////////////////
uims_cdrinfo_t* uims_cdrinfo_create( char *name, uims_cdrmgr_t *cdrmgr);
ux_status_t uims_cdrinfo_init( uims_cdrinfo_t *cdrinfo, char *name, uims_cdrmgr_t *cdrmgr);
void uims_cdrinfo_final( uims_cdrinfo_t *cdrinfo);
void uims_cdrinfo_destroy( uims_cdrinfo_t *cdrinfo);

uims_cdrinfo_t* uims_cdrinfo_create_l( char *name, const char* cfile, uims_cdrmgr_t *cdrmgr);

ux_status_t uims_cdrinfo_load( uims_cdrinfo_t *cdrinfo, const char* cfile);
ux_status_t uims_cdrinfo_load_fields( uims_cdrinfo_t *cdrinfo, char *buffer);
// ux_status_t uims_cdrinfo_check_file( uims_cdrinfo_t *cdrinfo);
// ux_status_t uims_cdrinfo_create_file( uims_cdrinfo_t *cdrinfo);
// ux_status_t uims_cdrinfo_finish_file( uims_cdrinfo_t *cdrinfo);

void uims_cdrinfo_print( uims_cdrinfo_t *cdrinfo, int depth);


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrfield_t 
////////////////////////////////////////////////////////////////////////////////
uims_cdrfield_t* uims_cdrfield_create();
ux_status_t uims_cdrfield_init( uims_cdrfield_t *cdrfield);
void uims_cdrfield_final( uims_cdrfield_t *cdrfield);
void uims_cdrfield_destroy( uims_cdrfield_t *cdrfield);

uims_cdrfield_t* uims_cdrfield_create_l( char *config);
uims_cdrfield_t* uims_cdrfield_create_d( uims_cdrfieldtype_e type, char *data);

ux_status_t uims_cdrfield_load( uims_cdrfield_t *cdrfield, char *config);
int uims_cdrfield_write( uims_cdrfield_t *cdrfield, uims_cdrbuff_t *cdrbuff, char *buff, int buflen);

void uims_cdrfield_print( uims_cdrfield_t *cdrfield, int depth);
char* uims_cdrfield_print_conf( uims_cdrfield_t *cdrfield);


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrufunc_t 
////////////////////////////////////////////////////////////////////////////////
uims_cdrufunc_t* uims_cdrufunc_create( int funcid);
ux_status_t uims_cdrufunc_init( uims_cdrufunc_t *cdrfunc, int funcid);
void uims_cdrufunc_final( uims_cdrufunc_t *cdrfunc);
void uims_cdrufunc_destroy( uims_cdrufunc_t *cdrfunc);

uims_cdrufunc_t* uims_cdrufunc_create_l( char *config);

char* uims_cdrufunc_print( uims_cdrufunc_t *cdrfunc, char *buff, int buflen);


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrbuff_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t uims_cdrbuff_init( uims_cdrbuff_t *cdrbuff, uims_cdrinfo_t *cdrinfo);
void uims_cdrbuff_final( uims_cdrbuff_t *cdrbuff);

char* uims_cdrbuff_get_string( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *field);
ux_time_t* uims_cdrbuff_get_time( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *field);
int uims_cdrbuff_get_int( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *field);

	

#endif // #ifndef __UIMS_CDR_L_H__
