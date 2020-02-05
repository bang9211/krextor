#include "local/uims_cdr_l.h"
#include <uxcutor/uxc_util.h>
#include <uxlib/ux_ini.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrpara_e 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_cdrpara_to_str( uims_cdrpara_e para)
{
	switch( para) {
		case UIMS_CDRPARA_UNKNOWN: return "unknown";
		case UIMS_CDRPARA_STIME: return "start_time";
		case UIMS_CDRPARA_ETIME: return "end_time";
		case UIMS_CDRPARA_DURATION: return "duration";
		case UIMS_CDRPARA_IN_PEER: return "in_peer";
		case UIMS_CDRPARA_OUT_PEER: return "out_peer";
		case UIMS_CDRPARA_FROM: return "from";
		case UIMS_CDRPARA_FROMTAG: return "from_tag";
		case UIMS_CDRPARA_TO: return "to";
		case UIMS_CDRPARA_TOTAG: return "to_tag";
		case UIMS_CDRPARA_CALLID: return "call_id";
		case UIMS_CDRPARA_STATUS: return "status";
		case UIMS_CDRPARA_REASON: return "reason";
		case UIMS_CDRPARA_PCHRG_VCT: return "pcharging_vector";
		case UIMS_CDRPARA_SIPREQ: return "reqmsg";
		case UIMS_CDRPARA_SIPRES: return "resmsg";
		case UIMS_CDRPARA_USER_FUNC: return "user_func";
		default: return "";
	}
}

UX_DECLARE(uims_cdrpara_e) uims_cdrpara_to_enum( char* str)
{
	char *pstr;

	if( str == NULL || str[0] == '\0') return UIMS_CDRPARA_UNKNOWN;

	pstr = strchr( str, '(');

	if( pstr == NULL) {
		switch( str[0]) {
			case 's': case 'S': 
				if( !strcasecmp( str, "start_time")) return UIMS_CDRPARA_STIME;
				else if( !strcasecmp( str, "status")) return UIMS_CDRPARA_STATUS;
				break;
			case 'e': case 'E':
				if( !strcasecmp( str, "end_time")) return UIMS_CDRPARA_ETIME;
				break;
			case 'd': case 'D':
				if( !strcasecmp( str, "duration")) return UIMS_CDRPARA_DURATION;
				break;
			case 'i': case 'I':
				if( !strcasecmp( str, "in_peer")) return UIMS_CDRPARA_IN_PEER;
				break;
			case 'o': case 'O':
				if( !strcasecmp( str, "out_peer")) return UIMS_CDRPARA_OUT_PEER;
				break;
			case 'f': case 'F':
				if( !strcasecmp( str, "from")) return UIMS_CDRPARA_FROM;
				else if( !strcasecmp( str, "from_tag")) return UIMS_CDRPARA_FROMTAG;
				break;
			case 't': case 'T':
				if( !strcasecmp( str, "to")) return UIMS_CDRPARA_TO;
				else if( !strcasecmp( str, "to_tag")) return UIMS_CDRPARA_TOTAG;
				break;
			case 'c': case 'C':
				if( !strcasecmp( str, "call_id")) return UIMS_CDRPARA_CALLID;
				break;
			case 'r': case 'R':
				if( !strcasecmp( str, "reason")) return UIMS_CDRPARA_REASON;
				break;
			case 'p': case 'P':
				if( !strcasecmp( str, "pcharging_vector")) return UIMS_CDRPARA_PCHRG_VCT;
				break;
			default: break;
		}
	} else {
		if( &pstr[1] == NULL || pstr[1] == '\0') {
			return UIMS_CDRPARA_UNKNOWN;
		}

		if( !strncasecmp( str, "reqmsg", 6)) return UIMS_CDRPARA_SIPREQ;
		else if( !strncasecmp( str, "resmsg", 6)) return UIMS_CDRPARA_SIPRES;
	}


	return UIMS_CDRPARA_UNKNOWN;
}


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrfieldtype_e 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_cdrfieldtype_to_str( uims_cdrfieldtype_e type)
{
	switch( type) {
		case UIMS_CDRFILDTYPE_PARA: return "param";
		case UIMS_CDRFILDTYPE_FUNC: return "func";
		case UIMS_CDRFILDTYPE_STRING: return "string";
		case UIMS_CDRFILDTYPE_INT: return "int";
		default: return "unknown";
	}
}

UX_DECLARE(uims_cdrfieldtype_e) uims_cdrfieldtype_to_enum( char *str)
{
	if( str == NULL || str[0] == '\0') return UIMS_CDRFILDTYPE_UNKNOWN;

	switch( str[0]) {
		case 'i': case 'I': 
			if( !strcasecmp( str, "int")) return UIMS_CDRFILDTYPE_INT;
			break;
		case 'p': case 'P': 
			if( !strcasecmp( str, "param")) return UIMS_CDRFILDTYPE_PARA;
			break;
		case 'f': case 'F': 
			if( !strcasecmp( str, "func")) return UIMS_CDRFILDTYPE_FUNC;
			break;
		case 's': case 'S': 
			if( !strcasecmp( str, "string")) return UIMS_CDRFILDTYPE_STRING;
			break;
	}

	return UIMS_CDRFILDTYPE_UNKNOWN;
}


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrufunc_e 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_cdrufunc_to_str( uims_cdrufunc_e func)
{
	switch( func) {
		case UIMS_CDRUFUNC_PRINT_TIME: return "PRINT_TIME";
		case UIMS_CDRUFUNC_PRINT_STIME: return "PRINT_STIME";
		case UIMS_CDRUFUNC_PRINT_IF: return "PRINT_IF";
		default: return "UNKNOWN";
	}

}

UX_DECLARE(uims_cdrufunc_e) uims_cdrufunc_to_enum( char *str)
{
	if( str == NULL || str[0] == '\0') return UIMS_CDRUFUNC_UNKNOWN;

	switch( str[0]) {
		case 'p': case 'P': 
			if( !strcasecmp( str, "print_time")) return UIMS_CDRUFUNC_PRINT_TIME;
			else if( !strcasecmp( str, "print_stime")) return UIMS_CDRUFUNC_PRINT_STIME;
			else if( !strcasecmp( str, "print_if")) return UIMS_CDRUFUNC_PRINT_IF;
			break;
	}
	return UIMS_CDRUFUNC_UNKNOWN;
}



////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrufunc_t 
////////////////////////////////////////////////////////////////////////////////

// $PRINT_TIME(start_time, %Y%m%d%H%m%S, 3)
static ux_status_t _uims_cdrufunc_print_time_load( uims_cdrufunc_t *func, char *buff);
static ux_status_t _uims_cdrufunc_print_time_print( uims_cdrufunc_t *func, uims_cdrbuff_t *cdrbuff, char *buff, int buflen);
static ux_status_t _uims_cdrufunc_print_time_conf( uims_cdrufunc_t *func, char *buff, int buflen);

uims_cdrufunc_t uims_cdrufunc_print_time[1] = {{
	"PRINT_TIME",
	UIMS_CDRUFUNC_PRINT_TIME,
	_uims_cdrufunc_print_time_load,
	_uims_cdrufunc_print_time_print,
	_uims_cdrufunc_print_time_conf,
}};

static ux_status_t _uims_cdrufunc_print_time_load( uims_cdrufunc_t *func, char *buff)
{
	int rv;
	char *datas[3];
	uims_cdrfield_t *para;

	enum { VALUE, FORMAT, USEC, TOTAL};

	rv = ux_ini_split_conf_data( buff, ',', datas, TOTAL, TOTAL);
	if( rv != 3) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (invalid param count(%d/%d)", uims_cdrufunc_print_time->name, rv, TOTAL);
		return UX_ERR_CONF;
	}

	// First
	para = uims_cdrfield_create_l( datas[VALUE]);
	if( para == NULL || para->type == UIMS_CDRFILDTYPE_UNKNOWN || para->type == UIMS_CDRFILDTYPE_FUNC) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (invalid first para)", uims_cdrufunc_print_time->name);
		if( para) uims_cdrfield_destroy( para);
		return UX_ERR_CONF;
	}
	ux_vector_append( func->paras, para);

	// Second
	para = uims_cdrfield_create_d( UIMS_CDRFILDTYPE_STRING, datas[FORMAT]);
	if( para == NULL) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (create second para)", uims_cdrufunc_print_time->name);
		return UX_ERR_CONF;
	}
	ux_vector_append( func->paras, para);

	// Third
	para = uims_cdrfield_create_d( UIMS_CDRFILDTYPE_INT, datas[USEC]);
	if( para == NULL) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (create third para)", uims_cdrufunc_print_time->name);
		return UX_ERR_CONF;
	}
	ux_vector_append( func->paras, para);

	return UX_SUCCESS;
}

static ux_status_t _uims_cdrufunc_print_time_print( uims_cdrufunc_t *func, uims_cdrbuff_t *cdrbuff,  char *buff, int buflen)
{
	int rv;
	char strusec[8];
	ux_time_t *ptime;
	uims_cdrfield_t *para1, *para2, *para3;

	enum { VALUE, FORMAT, USEC, TOTAL};

	para1 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, VALUE);
	para2 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, FORMAT);
	para3 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, USEC);
	if( para1 == NULL || para2 == NULL || para3 == NULL) {
		ux_log( UXL_MAJ, "Failed to print func '%s' for cdr. (invalid paras)", uims_cdrufunc_print_time->name);
		return UX_ERR_SYS;
	}

	ptime = uims_cdrbuff_get_time( cdrbuff, para1);
	if( ptime == NULL) {
		ux_log( UXL_MAJ, "Failed to print func '%s' for cdr. (not found first para)", uims_cdrufunc_print_time->name);
		return UX_ERR_SYS;
	}

	rv = ux_time_get_format_str( ptime, buff, buflen, para2->vstr);
	if( rv < 0) {
		ux_log( UXL_MAJ, "Failed to print func '%s' for cdr. (convert time)", uims_cdrufunc_print_time->name);
		return UX_ERR_SYS;
	}

	if( para3->vint > 0 && buflen > (rv+para3->vint+1)) {
		snprintf( strusec, sizeof( strusec), "%06ld", ptime->tv_usec);
		buff[rv] = '.';
		rv += 1;
		strncpy(&buff[rv], strusec, para3->vint);
		rv += para3->vint;
	}

	return rv;
}

static ux_status_t _uims_cdrufunc_print_time_conf( uims_cdrufunc_t *func, char *buff, int buflen)
{
	int rv;
	uims_cdrfield_t *para1, *para2, *para3;

	enum { VALUE, FORMAT, USEC, TOTAL};


	para1 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, VALUE);
	para2 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, FORMAT);
	para3 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, USEC);

	rv = snprintf( buff, buflen, "$%s ( %s, %s, %s )", func->name, uims_cdrfield_print_conf( para1), uims_cdrfield_print_conf( para2), uims_cdrfield_print_conf( para3));
	if( rv < 0) return rv;

	return UX_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////

// $PRINT_STIME(duration, 3)
static ux_status_t _uims_cdrufunc_print_stime_load( uims_cdrufunc_t *func, char *buff);
static ux_status_t _uims_cdrufunc_print_stime_print( uims_cdrufunc_t *func, uims_cdrbuff_t *cdrbuff, char *buff, int buflen);
static ux_status_t _uims_cdrufunc_print_stime_conf( uims_cdrufunc_t *func, char *buff, int buflen);

uims_cdrufunc_t uims_cdrufunc_print_stime[1] = {{
	"PRINT_STIME",
	UIMS_CDRUFUNC_PRINT_STIME,
	_uims_cdrufunc_print_stime_load,
	_uims_cdrufunc_print_stime_print,
	_uims_cdrufunc_print_stime_conf,
}};

static ux_status_t _uims_cdrufunc_print_stime_load( uims_cdrufunc_t *func, char *buff)
{
	int rv;
	char *datas[2];
	uims_cdrfield_t *para;

	enum { VALUE, USEC, TOTAL};

	rv = ux_ini_split_conf_data( buff, ',', datas, TOTAL, TOTAL);
	if( rv != 2) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (invalid param count(%d/%d)", uims_cdrufunc_print_stime->name, rv, TOTAL);
		return UX_ERR_CONF;
	}

	// First
	para = uims_cdrfield_create_l( datas[VALUE]);
	if( para == NULL || para->type == UIMS_CDRFILDTYPE_UNKNOWN || para->type == UIMS_CDRFILDTYPE_FUNC) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (invalid first para)", uims_cdrufunc_print_stime->name);
		if( para) uims_cdrfield_destroy( para);
		return UX_ERR_CONF;
	}
	ux_vector_append( func->paras, para);

	// Second
	para = uims_cdrfield_create_d( UIMS_CDRFILDTYPE_INT, datas[USEC]);
	if( para == NULL) {
		ux_log( UXL_MAJ, "Failed to load func '%s' for cdr. (create second para)", uims_cdrufunc_print_stime->name);
		return UX_ERR_CONF;
	}
	ux_vector_append( func->paras, para);

	return UX_SUCCESS;
}

static ux_status_t _uims_cdrufunc_print_stime_print( uims_cdrufunc_t *func, uims_cdrbuff_t *cdrbuff, char *buff, int buflen)
{
	int rv;
	char strusec[8];
	ux_time_t *ptime;
	uims_cdrfield_t *para1, *para2;

	enum { VALUE, USEC, TOTAL};

	para1 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, VALUE);
	para2 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, USEC);
	if( para1 == NULL || para2 == NULL) {
		ux_log( UXL_MAJ, "Failed to print func '%s' for cdr. (invalid paras)", uims_cdrufunc_print_stime->name);
		return UX_ERR_SYS;
	}

	ptime = uims_cdrbuff_get_time( cdrbuff, para1);
	if( ptime == NULL) {
		ux_log( UXL_MAJ, "Failed to print func '%s' for cdr. (not found first para)", uims_cdrufunc_print_stime->name);
		return UX_ERR_SYS;
	}

	rv = snprintf( buff, buflen, "%ld", ptime->tv_sec);
	if( rv < 0) {
		ux_log( UXL_MAJ, "Failed to print func '%s' for cdr. (invalid first para)", uims_cdrufunc_print_stime->name);
		return UX_ERR_SYS;
	}

	if( para2->vint > 0 && buflen > (rv+para2->vint+1)) {
		snprintf( strusec, sizeof( strusec), "%06ld", ptime->tv_usec);
		buff[rv] = '.';
		rv += 1;
		strncpy(&buff[rv], strusec, para2->vint);
		rv += para2->vint;
	}

	return rv;
}

static ux_status_t _uims_cdrufunc_print_stime_conf( uims_cdrufunc_t *func, char *buff, int buflen)
{
	int rv;
	uims_cdrfield_t *para1, *para2;

	enum { VALUE, USEC, TOTAL};

	para1 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, VALUE);
	para2 = (uims_cdrfield_t*) ux_vector_get_node( func->paras, USEC);

	rv = snprintf( buff, buflen, "$%s ( %s, %s )", func->name, uims_cdrfield_print_conf( para1), uims_cdrfield_print_conf( para2));
	if( rv <= 0) return rv;

	return UX_SUCCESS;
}



////////////////////////////////////////////////////////////////////////////////

uims_cdrufunc_t* g_uims_cdrufunc[] = {
	NULL,
	uims_cdrufunc_print_time,
	uims_cdrufunc_print_stime,
	NULL, // uims_cdrufunc_if,
	NULL
};

////////////////////////////////////////////////////////////////////////////////

uims_cdrufunc_t* uims_cdrufunc_create( int funcid)
{
	int rv;
	uims_cdrufunc_t *func;

	func = (uims_cdrufunc_t*) ux_zalloc( ux_mem_default(), sizeof( uims_cdrufunc_t));
	if( func == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_cdrufunc_t.");
		return NULL;
	}

	rv = uims_cdrufunc_init( func, funcid);
	if( rv < UX_SUCCESS) {
		ux_free( ux_mem_default(), func);
		return NULL;
	}

	return func;
}

ux_status_t uims_cdrufunc_init( uims_cdrufunc_t *cdrfunc, int funcid)
{
	uims_cdrufunc_t *gfunc;

	if( funcid <= UIMS_CDRUFUNC_UNKNOWN || funcid >= UIMS_CDRUFUNC_MAX) {
		ux_log( UXL_MAJ, "Failed to init ufunc for cdr. (invalid func id, %d)", funcid);
		return UX_ERR_CONF;
	}

	gfunc = g_uims_cdrufunc[funcid];
	if( gfunc == NULL) {
		ux_log( UXL_MAJ, "Failed to init ufunc for cdr. (invalid gfunc, %d)", funcid);
		return UX_ERR_SYS;
	}

	cdrfunc->name = gfunc->name;
	cdrfunc->id = funcid;
	cdrfunc->load = gfunc->load;
	cdrfunc->print = gfunc->print;
	cdrfunc->conf = gfunc->conf;

	ux_vector_init( cdrfunc->paras, 16, ux_mem_default());

	return UX_SUCCESS;
}

void uims_cdrufunc_final( uims_cdrufunc_t *cdrfunc)
{
	int i;
	uims_cdrfield_t *para;

	for( i = ux_vector_count( cdrfunc->paras)-1; i >= 0; i--) {
		para = (uims_cdrfield_t*) ux_vector_get_node( cdrfunc->paras, i);
		if( para) {
			ux_vector_remove_node( cdrfunc->paras, para);
			uims_cdrfield_destroy( para);
		}
	}
}

void uims_cdrufunc_destroy( uims_cdrufunc_t *cdrfunc)
{
	uims_cdrufunc_final( cdrfunc);
	ux_free( ux_mem_default(), cdrfunc);
}

uims_cdrufunc_t* uims_cdrufunc_create_l( char *config)
{
	int rv;
	char *name, *paras, *pstr;
	uims_cdrufunc_t *func;

	name = config;
	pstr = strchr( config, '(');
	if( pstr == NULL || strlen( pstr) <= 2) {
		ux_log( UXL_MAJ, "Not found braket for cdr func. (config=%s)", config);
		return NULL;
	}
	paras = pstr + 1;
	*pstr = '\0';

	pstr = strchr( paras, ')');
	if( pstr == NULL) {
		ux_log( UXL_MAJ, "Invalid braket for cdr func. (config=%s)", config);
		return NULL;
	}
	*pstr = '\0';

	func = uims_cdrufunc_create( uims_cdrufunc_to_enum( name));
	if( func == NULL) {
		return NULL;
	}

	rv = func->load( func, paras);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to load for cdr func. (config=%s)", config);
		uims_cdrufunc_destroy( func);
		return NULL;
	}

	return func;
}

char* uims_cdrufunc_print( uims_cdrufunc_t *cdrfunc, char *buff, int buflen)
{
	int rv;

	rv = cdrfunc->conf( cdrfunc, buff, buflen);
	if( rv < 0) {
		return NULL;
	}

	return buff;
}


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrfield_t 
////////////////////////////////////////////////////////////////////////////////
static int _uims_cdrfield_write_para(uims_cdrfield_t *cdrfield, uims_cdrbuff_t *cdrbuff, char *buff, int buflen);

uims_cdrfield_t* uims_cdrfield_create()
{
	int rv;
	uims_cdrfield_t *field;

	field = (uims_cdrfield_t*) ux_zalloc( ux_mem_default(), sizeof( uims_cdrfield_t));
	if( field == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_cdrfield_t.");
		return NULL;
	}

	rv = uims_cdrfield_init( field);
	if( rv < UX_SUCCESS) {
		ux_free( ux_mem_default(), field);
		return NULL;
	}

	return field;
}

ux_status_t uims_cdrfield_init( uims_cdrfield_t *cdrfield)
{
	return UX_SUCCESS;
}

void uims_cdrfield_final( uims_cdrfield_t *cdrfield)
{
	if( cdrfield->rfunc) uims_cdrufunc_destroy( cdrfield->rfunc);
	if( cdrfield->vstr) ux_free( ux_mem_default(), cdrfield->vstr);
}

void uims_cdrfield_destroy( uims_cdrfield_t *cdrfield)
{
	uims_cdrfield_final( cdrfield);
	ux_free( ux_mem_default(), cdrfield);
}

uims_cdrfield_t* uims_cdrfield_create_l( char *config)
{
	int rv;
	uims_cdrfield_t *field;

	field = uims_cdrfield_create();
	if( field == NULL) {
		return NULL;
	}

	rv = uims_cdrfield_load( field, config);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to load cdr field. (config=%s)", config);
		uims_cdrfield_destroy( field);
		return NULL;
	}

	return field;
}

uims_cdrfield_t* uims_cdrfield_create_d( uims_cdrfieldtype_e type, char *data)
{
	uims_cdrfield_t *field;

	field = uims_cdrfield_create();
	if( field == NULL) {
		return NULL;
	}

	if( data == NULL || data[0] == '\0') goto error;
	field->type = type;

	switch( type) {
		case UIMS_CDRFILDTYPE_STRING:
			field->vstr = ux_str_dup( data, ux_mem_default());
			break;
		case UIMS_CDRFILDTYPE_INT:
			if( ux_str_is_digit( data) == UX_FALSE) goto error;
			field->vstr = ux_str_dup( data, ux_mem_default());
			field->vint = strtol( data, NULL, 10);
			break;

		default:
			goto error;
	}

	return field;

error:
	uims_cdrfield_destroy( field);
	return NULL;
}

ux_status_t uims_cdrfield_load( uims_cdrfield_t *cdrfield, char *config)
{
	int conflen;
	char *pstr;
	uims_cdrpara_e para;
	uims_cdrufunc_t *rfunc;

	if( config == NULL || config[0] == '\0') {
		ux_log( UXL_MAJ, "Failed to load field. (field is null)");
		return UX_ERR_CONF;
	}

	if( config[0] == '$') {
		rfunc = uims_cdrufunc_create_l(&config[1]);
		if( rfunc == NULL) {
			ux_log( UXL_MAJ, "Failed to load field. (function, %s)", config);
			return UX_ERR_CONF;
		}
		cdrfield->type = UIMS_CDRFILDTYPE_FUNC;
		cdrfield->rfunc = rfunc;
	} else if( config[0] == '\'') {
		conflen = strlen( config);
		if( conflen <= 2 || config[conflen-1] != '\'') {
			ux_log( UXL_MAJ, "Failed to load field. (string, %s)", config);
			return UX_ERR_CONF;
		}
		cdrfield->type = UIMS_CDRFILDTYPE_STRING;
		config[conflen-1] = '\0';
		cdrfield->vstr = ux_str_dup( &config[1], ux_mem_default());
	} else {
		para = uims_cdrpara_to_enum( config);
		if( para == UIMS_CDRPARA_UNKNOWN) {
			ux_log( UXL_MAJ, "Failed to load field. (unknown para, %s)", config);
			return UX_ERR_CONF;
		}
		cdrfield->type = UIMS_CDRFILDTYPE_PARA;
		cdrfield->id = para;
		if( para == UIMS_CDRPARA_SIPREQ || para == UIMS_CDRPARA_SIPREQ) {
			pstr = strchr( config, '.');
			cdrfield->factorname = ux_str_dup( pstr, ux_mem_default());
		}
	}

	return UX_SUCCESS;
}

int uims_cdrfield_write( uims_cdrfield_t *cdrfield, uims_cdrbuff_t *cdrbuff, char *buff, int buflen)
{
	int rv;
	
	switch( cdrfield->type) {
		case UIMS_CDRFILDTYPE_PARA: 
			return _uims_cdrfield_write_para( cdrfield, cdrbuff, buff, buflen);
		case UIMS_CDRFILDTYPE_FUNC: 
			rv = cdrfield->rfunc->print( cdrfield->rfunc, cdrbuff, buff, buflen);
			if( rv < 0 || rv > buflen) {
				ux_log( UXL_WARN, "Failed to write field for cdr. (para=%s, funcid=%d, rv=%d)", uims_cdrfieldtype_to_str( cdrfield->type), cdrfield->id, rv);
				return UX_ERR_SYS;
			}
			return rv;
		case UIMS_CDRFILDTYPE_STRING:
			rv = snprintf( buff, buflen, "%s", cdrfield->vstr);
			if( rv < 0 || rv > buflen) {
				ux_log( UXL_WARN, "Failed to write field for cdr. (para=%s, str=%s, rv=%d)", uims_cdrfieldtype_to_str( cdrfield->type), cdrfield->vstr, rv);
				return UX_ERR_SYS;
			}
			return rv;
		case UIMS_CDRFILDTYPE_INT:
			rv = snprintf( buff, buflen, "%d", cdrfield->vint);
			if( rv < 0 || rv > buflen) {
				ux_log( UXL_WARN, "Failed to write field for cdr. (para=%s, int=%d, rv=%d)", uims_cdrfieldtype_to_str( cdrfield->type), cdrfield->vint, rv);
				return UX_ERR_SYS;
			}
			return rv;
		default :
			ux_log( UXL_WARN, "Failed to write field for cdr. (para=%s)", uims_cdrfieldtype_to_str( cdrfield->type));
			return UX_ERR_SYS;
	}
}

void uims_cdrfield_print( uims_cdrfield_t *cdrfield, int depth)
{
	char temp[1024];
	const char *pstr;

	if( cdrfield->type == UIMS_CDRFILDTYPE_FUNC) {
		pstr = uims_cdrufunc_print( cdrfield->rfunc, temp, sizeof( temp));
	} else {
		pstr = uims_cdrfield_print_conf( cdrfield);
	}

	ux_log( UXL_DEBUG, "%*s- type: %-6s | data: %s", depth*2, "", uims_cdrfieldtype_to_str( cdrfield->type), pstr);
}

char* uims_cdrfield_print_conf( uims_cdrfield_t *cdrfield)
{
	switch( cdrfield->type) {
		case UIMS_CDRFILDTYPE_PARA:
			return (char*)uims_cdrpara_to_str( cdrfield->id);
		case UIMS_CDRFILDTYPE_STRING:
		case UIMS_CDRFILDTYPE_INT:
			return cdrfield->vstr;
		default :
			return "";
	} 
}

static int _uims_cdrfield_write_para(uims_cdrfield_t *cdrfield, uims_cdrbuff_t *cdrbuff, char *buff, int buflen)
{
	int rv, tmplen;
	char tmpstr[1024];
	char *pstr;

	tmplen = sizeof( tmpstr);
	if( tmplen > buflen) tmplen = buflen;

	rv = 0;
	switch( cdrfield->id) {
		case UIMS_CDRPARA_STIME:
			rv = ux_time_get_format_str( cdrbuff->starttime, buff, buflen, "%Y%m%d%H%M%S");
			break;
		case UIMS_CDRPARA_ETIME:
			rv = ux_time_get_format_str( cdrbuff->endtime, buff, buflen, "%Y%m%d%H%M%S");
			break;
		case UIMS_CDRPARA_DURATION:
			rv = snprintf( buff, buflen, "%ld", cdrbuff->duration->tv_sec);
			break;
		case UIMS_CDRPARA_IN_PEER:
			rv = snprintf( buff, buflen, "%s", cdrbuff->in_peer? cdrbuff->in_peer:"" );
			break;
		case UIMS_CDRPARA_OUT_PEER:
			rv = snprintf( buff, buflen, "%s", cdrbuff->in_peer? cdrbuff->out_peer:"");
			break;
		case UIMS_CDRPARA_FROM:
			rv = snprintf( buff, buflen, "%s", cdrbuff->from? cdrbuff->from:"");
			break;
		case UIMS_CDRPARA_FROMTAG:
			rv = snprintf( buff, buflen, "%s", cdrbuff->fromtag? cdrbuff->fromtag:"");
			break;
		case UIMS_CDRPARA_TO:
			rv = snprintf( buff, buflen, "%s", cdrbuff->to? cdrbuff->to:"");
			break;
		case UIMS_CDRPARA_TOTAG:
			rv = snprintf( buff, buflen, "%s", cdrbuff->totag? cdrbuff->totag:"");
			break;
		case UIMS_CDRPARA_CALLID:
			rv = snprintf( buff, buflen, "%s", cdrbuff->callid? cdrbuff->callid:"");
			break;
		case UIMS_CDRPARA_STATUS:
			rv = snprintf( buff, buflen, "%s", cdrbuff->status? cdrbuff->status:"");
			break;
		case UIMS_CDRPARA_REASON:
			rv = snprintf( buff, buflen, "%d", cdrbuff->reason);
			break;
		case UIMS_CDRPARA_PCHRG_VCT:
			rv = snprintf( buff, buflen, "%s", cdrbuff->pcharging? cdrbuff->pcharging:"");
			break;
		case UIMS_CDRPARA_SIPREQ:
		case UIMS_CDRPARA_SIPRES:
			pstr = uims_cdrbuff_get_factor( cdrbuff, cdrfield);
			rv = snprintf( buff, buflen, "%s", pstr? pstr:"");
			break;

		default:
			ux_log( UXL_WARN, "Failed to write field for cdr. (not support para, para=%s, id=%d)", uims_cdrfieldtype_to_str( cdrfield->type), cdrfield->id);
			return UX_ERR_CONF;
	}

	if( rv < 0 || rv > buflen) {
		ux_log( UXL_WARN, "Failed to write field for cdr. (para=%s, id=%d, rv=%d)", uims_cdrfieldtype_to_str( cdrfield->type), cdrfield->id, rv);
		return UX_ERR_SYS;
	}

	return rv;
}


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_cdrinfo_t 
//////////////////////////////////////////////////////////////////////////////// 
static void _uims_cdrinfo_clear_fields( uims_cdrinfo_t *cdrinfo);

uims_cdrinfo_t* uims_cdrinfo_create( char *name, uims_cdrmgr_t *cdrmgr)
{
	int rv;
	uims_cdrinfo_t *cdrinfo;

	cdrinfo = (uims_cdrinfo_t*) ux_zalloc( ux_mem_default(), sizeof( uims_cdrinfo_t));
	if( cdrinfo == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_cdrinfo.");
		return NULL;
	}

	rv = uims_cdrinfo_init( cdrinfo, name, cdrmgr);
	if( rv < UX_SUCCESS) {
		ux_free( ux_mem_default(), cdrinfo);
		return NULL;
	}

	return cdrinfo;
}

ux_status_t uims_cdrinfo_init( uims_cdrinfo_t *cdrinfo, char *name, uims_cdrmgr_t *cdrmgr)
{
	if( name == NULL || name[0] == '\0' || strlen( name) > UIMS_CDR_MAX_NAMELEN ) {
		ux_log( UXL_MAJ, "invalid name for uims_cdrinfo_t.");
		return UX_ERR_CONF;
	}
	memset( cdrinfo, 0x00, sizeof( uims_cdrinfo_t));

	// cdrinfo->ucdr_conf = cdrmgr->ucdr_conf;

	// strlcpy( cdrinfo->name, name, sizeof( cdrinfo->name));
	cdrinfo->cdrmgr = cdrmgr;
	cdrinfo->name = ux_str_trimdup( name, ux_mem_default());
	ux_refcount_init( cdrinfo->refcnt);
	ux_vector_init( cdrinfo->fields, 32, ux_mem_default());
	cdrinfo->isalive = UX_FALSE;

	return UX_SUCCESS;
}

void uims_cdrinfo_final( uims_cdrinfo_t *cdrinfo)
{
	if( cdrinfo->name) {
		ux_free( ux_mem_default(), cdrinfo->name);
		cdrinfo->name = NULL;
	}
	_uims_cdrinfo_clear_fields( cdrinfo);
	ux_vector_final( cdrinfo->fields);
	// uims_cdrinfo_finish_file( cdrinfo);
	if( cdrinfo->ucdr_handle) uxc_cdr_destroy( cdrinfo->ucdr_handle);
	ux_refcount_final( cdrinfo->refcnt);
}

void uims_cdrinfo_destroy( uims_cdrinfo_t *cdrinfo)
{
	uims_cdrinfo_final( cdrinfo);
	ux_free( ux_mem_default(), cdrinfo);
}

uims_cdrinfo_t* uims_cdrinfo_create_l( char *name, const char* cfile, uims_cdrmgr_t *cdrmgr)
{
	int rv;
	uims_cdrinfo_t *cdrinfo;

	cdrinfo = uims_cdrinfo_create( name, cdrmgr);
	if( cdrinfo == NULL) {
		return NULL;
	}

	rv = uims_cdrinfo_load( cdrinfo, cfile);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to load for uims_cdrinfo_t. (name=%s, cfile=%s)", name, cfile);
		uims_cdrinfo_destroy( cdrinfo);
		return NULL;
	}

	return cdrinfo;
}

ux_status_t uims_cdrinfo_load( uims_cdrinfo_t *cdrinfo, const char* cfile)
{
	int rv;
	char *pstr, temp[4096], section[128];
	uims_cdrmgr_t *cdrmgr;

	cdrmgr = (uims_cdrmgr_t*) cdrinfo->cdrmgr;
	snprintf( section, sizeof( section), "[UCDR/UIMS_%s]", cdrinfo->name);
	pstr = uxc_get_conf_str( cfile, section, "DELIMITER", temp, 16, "");
	if( pstr[0] == '\0') {
		ux_log( UXL_MAJ, "Not found 'DELIMITER' on %s for uims_cdrinfo_t.", section);
		return UX_ERR_CONF;
	}
	strlcpy( cdrinfo->delim, pstr, sizeof( cdrinfo->delim));

	pstr = uxc_get_conf_str( cfile, section, "FIELDS", temp, sizeof( temp), "");
	if( pstr[0] == '\0') {
		ux_log( UXL_MAJ, "Not found 'FIELDS' on %s for uims_cdrinfo_t.", section);
		return UX_ERR_CONF;
	}

	rv = uims_cdrinfo_load_fields( cdrinfo, pstr);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to load 'FIELDS' on %s for uims_cdrinfo_t.", section);
		_uims_cdrinfo_clear_fields( cdrinfo);
		return UX_ERR_CONF;
	}

	cdrinfo->ucdr_handle = uxc_cdrmgr_get_cdr( cdrmgr->ucdr_mgr, "UIMS", cdrinfo->name);
	if( cdrinfo->ucdr_handle == NULL) {
		ux_log( UXL_MAJ, "Failed to load ucdr_info for %s.", cdrinfo->name);
		return UX_ERR_CONF;
	}

	return UX_SUCCESS;
}

ux_status_t _uims_cdrinfo_check_field_factor( uims_cdrinfo_t *cdrinfo, uims_cdrfield_t *field)
{
	int i;
	uims_cdrfield_t *factor;

	if( field->type != UIMS_CDRFILDTYPE_PARA || ( field->id != UIMS_CDRPARA_SIPREQ && field->id != UIMS_CDRPARA_SIPRES )) return UX_SUCCESS;

	for( i = 0; i < ux_vector_count( cdrinfo->msgfactors); i++) {
		factor = ( uims_cdrfield_t*) ux_vector_get_node( cdrinfo->msgfactors, i);
		if( factor) {
			if( factor->id == field->id && !strcmp( factor->factorname, field->factorname)) {
				return i;
			}
		}
	}

	factor = uims_cdrfield_create();
	if( factor == NULL) {
		ux_log( UXL_MAJ, "Failed to create msg factor for cdr. (cdr=%s, field=%s.%s)", cdrinfo->name, uims_cdrpara_to_str( field->id), field->factorname);
		return UX_ERR_SYS;
	}

	factor->type = field->type;
	factor->id = field->id;
	factor->factorname = ux_str_dup( field->factorname, ux_mem_default());

	ux_vector_append( cdrinfo->msgfactors, factor);
	return ux_vector_count( cdrinfo->msgfactors) -1;
}

ux_status_t uims_cdrinfo_load_fields( uims_cdrinfo_t *cdrinfo, char *buffer)
{
	int i, rv, cnt;
	char *datas[UIMS_CDR_MAX_FIELDS];
	uims_cdrfield_t *field, *rfield;

	cnt = ux_ini_split_conf_data( buffer, ';', datas, UIMS_CDR_MAX_FIELDS, 1);
	if( cnt < 1) {
		ux_log( UXL_MAJ, "Failed to parsing for fields. (rv=%d)", cnt);
		return UX_ERR_CONF;
	}

	ux_vector_resize( cdrinfo->fields, cnt);

	for( i = 0; i < cnt; ++i) {
		field = uims_cdrfield_create_l( datas[i]);
		if( field == NULL) {
			ux_log( UXL_MAJ, "Failed to create field. (field=%s)", datas[i]);
			return UX_ERR_CONF;
		}

		ux_vector_append( cdrinfo->fields, field);
	}

	// make msgfactor
	for( i = 0; i < ux_vector_count( cdrinfo->fields); i++) {
		field = (uims_cdrfield_t*) ux_vector_get_node( cdrinfo->fields, i);
		if( field) {
			switch( field->type) {
				case UIMS_CDRFILDTYPE_PARA:
					rv = _uims_cdrinfo_check_field_factor( cdrinfo, field);
					if( rv < UX_SUCCESS) return UX_ERR_CONF;
					field->factorid = rv;
					break;
				case UIMS_CDRFILDTYPE_FUNC:
					for( i = 0; i < ux_vector_count( field->rfunc->paras); i++) {
						rfield = (uims_cdrfield_t*) ux_vector_get_node( field->rfunc->paras, i);
						if( rfield) {
							rv = _uims_cdrinfo_check_field_factor( cdrinfo, rfield);
							if( rv < UX_SUCCESS) return UX_ERR_CONF;
							rfield->factorid = rv;
						}
					}
					break;
				default: break;
			}
		}
	}

	return UX_SUCCESS;
}

void uims_cdrinfo_print( uims_cdrinfo_t *cdrinfo, int depth)
{
	int i; 

	ux_log( UXL_DEBUG, "%*s+ %s (status:%s / ref:%d)", depth*2, "", cdrinfo->name, cdrinfo->isalive==UX_TRUE? "alive":"death", cdrinfo->refcnt->count);
	ux_log( UXL_DEBUG, "%*s  . delimiter  = %s", depth*2, "", cdrinfo->delim);
	ux_log( UXL_DEBUG, "%*s  . fields (cnt=%d)", depth*2, "", ux_vector_count( cdrinfo->fields));
	for( i = 0; i < ux_vector_count( cdrinfo->fields); ++i) {
		uims_cdrfield_print( (uims_cdrfield_t*) ux_vector_get_node( cdrinfo->fields, i), depth+2);
	}
}

UX_DECLARE(uims_cdrinfo_t*) uims_cdrinfo_ref( uims_cdrinfo_t *cdrinfo)
{
	return (uims_cdrinfo_t*)UX_OBJ_PREF(cdrinfo, refcnt);
}

UX_DECLARE(void) uims_cdrinfo_unref( uims_cdrinfo_t *cdrinfo)
{
	cdrinfo = UX_OBJ_PUNREF( cdrinfo, refcnt);
	if( cdrinfo) uims_cdrinfo_destroy( cdrinfo);
}

UX_DECLARE(void) uims_cdrinfo_write( uims_cdrinfo_t *cdrinfo, uims_cdrbuff_t *cdrbuff)
{
	int i, rv, cnt, tmplen, delen;
	char tmpbuff[UIMS_CDR_MAX_CDRBUFF];
	uims_cdrfield_t *field;

	tmplen = 0;
	delen = strlen( cdrinfo->delim);
	cnt = ux_vector_count( cdrinfo->fields);
	memset( tmpbuff, 0x00, sizeof( tmpbuff));

	for( i = 0; i < cnt; ++i) {
		if( tmplen >= UIMS_CDR_MAX_CDRBUFF-1) break;

		field = (uims_cdrfield_t*) ux_vector_get_node( cdrinfo->fields, i);
		if( field != NULL) {
			rv = uims_cdrfield_write( field, cdrbuff, &tmpbuff[tmplen], UIMS_CDR_MAX_CDRBUFF-tmplen);
			if( rv < 0) {
				ux_log( UXL_MAJ, "Failed to write %d field for cdr. (cdr=%s, fieldtype=%s, id=%d)", i+1, cdrinfo->name,  uims_cdrfieldtype_to_str( field->type), field->id);
				return;
			}
			tmplen += rv;
		}

		if( i >= cnt && tmplen+delen+1 >= UIMS_CDR_MAX_CDRBUFF) {
			break;
		}
		rv = snprintf( &tmpbuff[tmplen], UIMS_CDR_MAX_CDRBUFF-tmplen, "%s", cdrinfo->delim);
		if( rv <= 0) {
			ux_log( UXL_MAJ, "Failed to write %d field delimiter for cdr. (cdr=%s, fieldtype=%s, id=%d)", i+1, cdrinfo->name,  uims_cdrfieldtype_to_str( field->type), field->id);
			return;
		}
		tmplen += rv;
		ux_log( UXL_DBG2, "!!! info write(%d) buf=%s, len=%d", i+1, tmpbuff, tmplen);
	}

	rv = uxc_cdr_write( cdrinfo->ucdr_handle, tmpbuff, tmplen);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to write cdr. (cdr=%s, buflen=%d, buff=%s)", cdrinfo->name, tmplen, tmpbuff);
		return;
	}
}

static void _uims_cdrinfo_clear_fields( uims_cdrinfo_t *cdrinfo)
{
	int i;
	uims_cdrfield_t *field;

	for( i = ux_vector_count( cdrinfo->fields)-1; i >= 0; i--) {
		field = (uims_cdrfield_t*) ux_vector_get_node( cdrinfo->fields, i);
		if( field) {
			uims_cdrfield_destroy( field);
			ux_vector_remove_node( cdrinfo->fields, field);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrmgr_t 
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(uims_cdrmgr_t*) uims_cdrmgr_create( const char* cfile)
{
	int rv;
	uims_cdrmgr_t *cdrmgr;

	cdrmgr = (uims_cdrmgr_t*) ux_zalloc( ux_mem_default(), sizeof( uims_cdrmgr_t));
	if( cdrmgr == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_cdrmgr_t.");
		return NULL;
	}

	rv = uims_cdrmgr_init( cdrmgr, cfile);
	if( rv < UX_SUCCESS) {
		uims_cdrmgr_destroy( cdrmgr);
		return NULL;
	}

	return cdrmgr;
}

ux_status_t uims_cdrmgr_init( uims_cdrmgr_t *cdrmgr, const char* cfile)
{
	int rv;

	if( cfile == NULL || cfile[0] == '\0') {
		ux_log( UXL_MAJ, "invalid config file.");
		return UX_ERR_CONF;
	}

	cdrmgr->cfile = cfile;

	cdrmgr->ucdr_mgr = uxc_cdrmgr_create( cfile, NULL);
	if( cdrmgr->ucdr_mgr == NULL) {
		ux_log( UXL_MAJ, "Failed to init uxc_cdrmgr.");
		return UX_ERR_CONF;
	}

	rv = ux_hash_init( cdrmgr->cdrinfos, 16, ux_compare_str, ux_hash_str, NULL);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to init hash for cdrmgr.");
		return rv;
	}

	rv = uims_cdrmgr_load( cdrmgr);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to load config for cdrmgr.");
		return rv;
	}

	return UX_SUCCESS;
}

void uims_cdrmgr_final( uims_cdrmgr_t *cdrmgr)
{
	ux_hnode_t *hnode;
	uims_cdrinfo_t *cdrinfo;

	hnode = ux_hash_head( cdrmgr->cdrinfos);
	while( hnode) {
		cdrinfo = (uims_cdrinfo_t*)hnode;
		hnode = ux_hash_next( cdrmgr->cdrinfos, cdrinfo->hnode);
		ux_hash_remove_node( cdrmgr->cdrinfos, cdrinfo->hnode);

		uims_cdrinfo_unref( cdrinfo);
	}

	if( cdrmgr->ucdr_mgr) {
		uxc_cdrmgr_stop( cdrmgr->ucdr_mgr);
		uxc_cdrmgr_destroy( cdrmgr->ucdr_mgr);
	}

	ux_hash_final( cdrmgr->cdrinfos);
}

UX_DECLARE(void) uims_cdrmgr_destroy( uims_cdrmgr_t *cdrmgr)
{
	uims_cdrmgr_final( cdrmgr);
	ux_free( ux_mem_default(), cdrmgr);
}

UX_DECLARE(ux_status_t) uims_cdrmgr_load( uims_cdrmgr_t *cdrmgr)
{
	static const char *section = "[UCDR/COMMON]";
	int i, rv, ncdr;
	char *pstr, *datas[16], temp[2048];
	uims_cdrinfo_t *cdrinfo;

	pstr = uxc_get_conf_str( cdrmgr->cfile, section, "UIMS_LIST", temp, sizeof( temp), "");
	if( pstr[0] == '\0') {
		return UX_SUCCESS;
	}

	ncdr = uxc_split_conf_data( pstr, ";", datas, 15, 1);
	if( ncdr < 1) return UX_SUCCESS;

	for( i = 0; i < ncdr; i++) {
		cdrinfo = uims_cdrinfo_create_l( datas[i], cdrmgr->cfile, cdrmgr);
		if( cdrinfo == NULL) {
			ux_log( UXL_MAJ, "Failed to load cdr config for %s.", datas[i]);
			continue;
		}

		rv = ux_hash_insert( cdrmgr->cdrinfos, cdrinfo->name, cdrinfo->hnode);
		if( rv < UX_SUCCESS) {
			ux_log( UXL_MAJ, "Failed to insert cdr config for %s.", datas[i]);
			uims_cdrinfo_destroy( cdrinfo);
		}
	}

	uims_cdrmgr_print( cdrmgr);

	return UX_SUCCESS;
}

UX_DECLARE(uims_cdrinfo_t*) uims_cdrmgr_get_info( uims_cdrmgr_t *cdrmgr, char *name)
{
	return (uims_cdrinfo_t*)ux_hash_find( cdrmgr->cdrinfos, name);
}

UX_DECLARE(ux_status_t) uims_cdrmgr_start( uims_cdrmgr_t *cdrmgr)
{
	if( cdrmgr->ucdr_mgr) {
		return uxc_cdrmgr_start( cdrmgr->ucdr_mgr);
	}

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) uims_cdrmgr_stop( uims_cdrmgr_t *cdrmgr)
{
	if( cdrmgr->ucdr_mgr) {
		uxc_cdrmgr_stop( cdrmgr->ucdr_mgr);
	}

	return UX_SUCCESS;
}

UX_DECLARE(ux_status_t) uims_cdrmgr_join( uims_cdrmgr_t *cdrmgr)
{
	if( cdrmgr->ucdr_mgr) {
		uxc_cdrmgr_join( cdrmgr->ucdr_mgr);
	}

	return UX_SUCCESS;
}

void uims_cdrmgr_print( uims_cdrmgr_t *cdrmgr)
{
	ux_hnode_t *hnode;
	uims_cdrinfo_t *cdrinfo;

	ux_log( UXL_DEBUG, "-------------------------------------------------------");
	ux_log( UXL_DEBUG, " CDR Manager for UIMS");
	ux_log( UXL_DEBUG, "-------------------------------------------------------");
	ux_log( UXL_DEBUG, "- cfile = %s", cdrmgr->cfile);
	ux_log( UXL_DEBUG, "- count = %d", ux_hash_count( cdrmgr->cdrinfos));

	hnode = ux_hash_head( cdrmgr->cdrinfos);
	while( hnode) {
		cdrinfo = (uims_cdrinfo_t*)hnode;
		hnode = ux_hash_next( cdrmgr->cdrinfos, cdrinfo->hnode);

		uims_cdrinfo_print( cdrinfo, 1);
		ux_log( UXL_DEBUG, " -----------------------------------------------------");
	}
	ux_log( UXL_DEBUG, "-------------------------------------------------------");
}


////////////////////////////////////////////////////////////////////////////////
// functions for uims_cdrbuff_t 
////////////////////////////////////////////////////////////////////////////////
static void _uims_cdrbuff_set_str( char *pstr, char **dest);
static void _uims_cdrbuff_set_factor( uims_cdrbuff_t *cdrbuff, usip_msg_t *req, usip_msg_t *res);

UX_DECLARE(uims_cdrbuff_t*) uims_cdrbuff_create( uims_cdrinfo_t *cdrinfo)
{
	int rv;
	uims_cdrbuff_t *buff;

	buff = (uims_cdrbuff_t*) ux_zalloc( ux_mem_default(), sizeof( uims_cdrbuff_t));
	if( buff == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_cdrbuff_t.");
		return NULL;
	}

	rv = uims_cdrbuff_init( buff, cdrinfo);
	if( rv < UX_SUCCESS) {
		ux_free( ux_mem_default(), buff);
		return NULL;
	}

	return buff;
}

ux_status_t uims_cdrbuff_init( uims_cdrbuff_t *cdrbuff, uims_cdrinfo_t *cdrinfo)
{
	int i;

	cdrbuff->cdrinfo = cdrinfo;
	ux_vector_init( cdrbuff->buffs, UIMS_CDR_MAX_FIELDS, ux_mem_default());

	if( ux_vector_count( cdrinfo->msgfactors) > 0) {
		for( i = 0; i < ux_vector_count( cdrinfo->msgfactors); i++) {
			ux_vector_append( cdrbuff->buffs, NULL);
		}
	}


	return UX_SUCCESS;
}

void uims_cdrbuff_final( uims_cdrbuff_t *cdrbuff)
{
	int i;
	uims_cdrfield_t *para;

	for( i = ux_vector_count( cdrbuff->buffs)-1; i >= 0; i--) {
		para = (uims_cdrfield_t*) ux_vector_get_node( cdrbuff->buffs, i);
		if( para) {
			ux_vector_remove_node( cdrbuff->buffs, para);
			uims_cdrfield_destroy( para);
		}
	}
}

UX_DECLARE(void) uims_cdrbuff_destroy( uims_cdrbuff_t *cdrbuff)
{
	uims_cdrbuff_final( cdrbuff);
	ux_free( ux_mem_default(), cdrbuff);
}

UX_DECLARE(void) uims_cdrbuff_start( uims_cdrbuff_t *cdrbuff, usip_msg_t *msg, char *in_peer)
{usip_hdr_t *mhdr; usip_mobj_t *mobj;
	ux_time_get_current( cdrbuff->starttime);
	_uims_cdrbuff_set_str( in_peer, &cdrbuff->in_peer);

	if( msg != NULL) {
		mobj = (usip_mobj_t*) usip_msg_get_mobj( msg);
		_uims_cdrbuff_set_str( USIP_MOBJ_GET_FROMUSER( mobj), &cdrbuff->from);
		_uims_cdrbuff_set_str( USIP_MOBJ_GET_FROMTAG( mobj), &cdrbuff->fromtag);
		_uims_cdrbuff_set_str( USIP_MOBJ_GET_TOTAG( mobj), &cdrbuff->to);
		_uims_cdrbuff_set_str( USIP_MOBJ_GET_CALLID( mobj), &cdrbuff->callid);

		mhdr = usip_msg_get_hdr_n( msg, "P-Charging-Vector", 0);
		if( mhdr) {
			_uims_cdrbuff_set_str( (char*)usip_hdr_get_value( mhdr, NULL), &cdrbuff->pcharging);
		}
	}

	_uims_cdrbuff_set_factor( cdrbuff, msg, NULL);

	uims_cdrbuff_update_status( cdrbuff, msg, UX_FALSE);
}

UX_DECLARE(void) uims_cdrbuff_update( uims_cdrbuff_t *cdrbuff, usip_msg_t *reqmsg, usip_msg_t *resmsg, char *out_peer)
{
	usip_hdr_t *mhdr;
	usip_mobj_t *mobj;

	_uims_cdrbuff_set_str( out_peer, &cdrbuff->out_peer);

	if( resmsg != NULL) {
		mobj = (usip_mobj_t*) usip_msg_get_mobj( resmsg);
		if( cdrbuff->totag == NULL) _uims_cdrbuff_set_str( USIP_MOBJ_GET_TOTAG( mobj), &cdrbuff->totag);

		if( cdrbuff->pcharging == NULL) {
			mhdr = usip_msg_get_hdr_n( resmsg, "P-Charging-Vector", 0);
			if( mhdr) {
				_uims_cdrbuff_set_str( (char*)usip_hdr_get_value( mhdr, NULL), &cdrbuff->pcharging);
			}
		}
	}

	_uims_cdrbuff_set_factor( cdrbuff, reqmsg, resmsg);

	uims_cdrbuff_update_status( cdrbuff, resmsg, UX_FALSE);
}

UX_DECLARE(void) uims_cdrbuff_update_status( uims_cdrbuff_t *cdrbuff, usip_msg_t *msg, ux_status_t is_send)
{
	int rv;
	usip_mobj_t *mobj;

	if( msg == NULL) return;
	if( cdrbuff->statuslen >= UIMS_CDR_MAX_PATHLEN) return;

	mobj = (usip_mobj_t*) usip_msg_get_mobj( msg);
	rv = snprintf( &cdrbuff->status[cdrbuff->statuslen], UIMS_CDR_MAX_PATHLEN-cdrbuff->statuslen, "%s%c%c%03d", cdrbuff->status, is_send==UX_TRUE? '>':'<', usip_mobj_is_request( mobj)==UX_TRUE? 'R':'A', usip_mobj_is_request( mobj)==UX_TRUE? USIP_MOBJ_GET_METHOD( mobj):usip_mobj_get_status_code( mobj));

	if( rv > 0) cdrbuff->statuslen += rv;
}

UX_DECLARE(void) uims_cdrbuff_end( uims_cdrbuff_t *cdrbuff, usip_msg_t *msg, char *out_peer, ux_status_t is_send)
{
	usip_hdr_t *mhdr;
	usip_mobj_t *mobj;

	ux_time_get_current( cdrbuff->endtime);
	ux_time_subs( cdrbuff->duration, cdrbuff->endtime, cdrbuff->starttime);
	if( cdrbuff->out_peer == NULL && out_peer != NULL) _uims_cdrbuff_set_str( out_peer, &cdrbuff->out_peer);

	if( msg != NULL) {
		mobj = (usip_mobj_t*) usip_msg_get_mobj( msg);
		if( cdrbuff->totag == NULL) _uims_cdrbuff_set_str( USIP_MOBJ_GET_TOTAG( mobj), &cdrbuff->totag);
		cdrbuff->reason = usip_mobj_get_status_code( mobj);
		if( cdrbuff->pcharging == NULL) {
			mhdr = usip_msg_get_hdr_n( msg, "P-Charging-Vector", 0);
			if( mhdr) {
				_uims_cdrbuff_set_str( (char*)usip_hdr_get_value( mhdr, NULL), &cdrbuff->pcharging);
			}
		}
		_uims_cdrbuff_set_factor( cdrbuff, usip_mobj_is_request( mobj)? msg:NULL, usip_mobj_is_request( mobj)? NULL:msg);
	}

	uims_cdrbuff_update_status( cdrbuff, msg, is_send);
	uims_cdrinfo_write( cdrbuff->cdrinfo, cdrbuff);
}

UX_DECLARE(char*) uims_cdrbuff_get_factor( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *cdrfield)
{
	uims_cdrfield_t *field;

	if( cdrfield->factorid < 0 || cdrfield->factorid >= ux_vector_count( cdrbuff->buffs)) {
		return NULL;
	}

	field = (uims_cdrfield_t*) ux_vector_get_node( cdrbuff->buffs, cdrfield->factorid);
	if( field) {
		return field->vstr;
	}

	return NULL;
}

char* uims_cdrbuff_get_string( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *field)
{
	if( field->type == UIMS_CDRFILDTYPE_PARA) {
		switch( field->id) {
			case UIMS_CDRPARA_IN_PEER:
				return cdrbuff->in_peer;
			case UIMS_CDRPARA_OUT_PEER:
				return cdrbuff->out_peer;
			case UIMS_CDRPARA_FROM:
				return cdrbuff->from;
			case UIMS_CDRPARA_FROMTAG:
				return cdrbuff->fromtag;
			case UIMS_CDRPARA_TO:
				return cdrbuff->to;
			case UIMS_CDRPARA_TOTAG:
				return cdrbuff->totag;
			case UIMS_CDRPARA_CALLID:
				return cdrbuff->callid;
			case UIMS_CDRPARA_PCHRG_VCT:
				return cdrbuff->pcharging;
			case UIMS_CDRPARA_STATUS:
				return cdrbuff->status;
			case UIMS_CDRPARA_SIPREQ:
			case UIMS_CDRPARA_SIPRES:
				return uims_cdrbuff_get_factor( cdrbuff, field);
			default: break;
		}
	}

	ux_log( UXL_WARN, "not string type for cdr. ( para=%s, id=%d)", uims_cdrfieldtype_to_str( field->type), field->id);
	return NULL;
}

ux_time_t* uims_cdrbuff_get_time( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *field)
{
	if( field->type == UIMS_CDRFILDTYPE_PARA) {
		switch( field->id) {
			case UIMS_CDRPARA_STIME:
				return cdrbuff->starttime;
			case UIMS_CDRPARA_ETIME:
				return cdrbuff->endtime;
			case UIMS_CDRPARA_DURATION:
				return cdrbuff->duration;
			default: break;
		}
	}

	ux_log( UXL_WARN, "not time type for cdr. (para=%s, id=%d)", uims_cdrfieldtype_to_str( field->type), field->id);
	return NULL;
}

int uims_cdrbuff_get_int( uims_cdrbuff_t *cdrbuff, uims_cdrfield_t *field)
{
	if( field->type == UIMS_CDRFILDTYPE_PARA) {
		switch( field->id) {
			case UIMS_CDRPARA_STIME:
				return cdrbuff->starttime->tv_sec;
			case UIMS_CDRPARA_ETIME:
				return cdrbuff->endtime->tv_sec;
			case UIMS_CDRPARA_DURATION:
				return cdrbuff->duration->tv_sec;
			case UIMS_CDRPARA_REASON:
				return cdrbuff->reason;
			default: break;
		}
	}

	ux_log( UXL_WARN, "not int type for cdr. (para=%s, id=%d)", uims_cdrfieldtype_to_str( field->type), field->id);
	return UIMS_CDR_INVALID_INT;
}

static void _uims_cdrbuff_set_str( char *pstr, char **dest)
{
	if( pstr == NULL && pstr[0] == '\0') return;
	if( *dest) ux_free( ux_mem_default(), *dest);
	*dest = ux_str_dup( pstr, ux_mem_default());
}

static void _uims_cdrbuff_set_factor( uims_cdrbuff_t *cdrbuff, usip_msg_t *req, usip_msg_t *res)
{
	int i, err;
	const char *pstr;
	uims_cdrfield_t *field;

	for( i = 0; i < ux_vector_count( cdrbuff->cdrinfo->msgfactors); i++) {
		field = (uims_cdrfield_t*) ux_vector_get_node( cdrbuff->cdrinfo->msgfactors, i);
		if( field) {
			if( field->id == UIMS_CDRPARA_SIPREQ && req) {
				if( req) {
					pstr = usip_msg_get_str( req, field->factorname, &err);
					if( pstr != NULL) {
						if( field->vstr) ux_free( ux_mem_default(), field->vstr);
						field->vstr = ux_str_dup( pstr, ux_mem_default());
					}
				}
			}
			else {
				if( res) {
					pstr = usip_msg_get_str( res, field->factorname, &err);
					if( pstr != NULL) {
						if( field->vstr) ux_free( ux_mem_default(), field->vstr);
						field->vstr = ux_str_dup( pstr, ux_mem_default());
					}
				}
			}
		}
	}

}

