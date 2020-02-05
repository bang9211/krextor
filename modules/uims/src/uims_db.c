#include "local/uims_db_l.h"
#include <uxlib/ux_log.h>
#include <uxlib/ux_util.h>
#include <uxlib/ux_ini.h>
#include <limits.h>
#include <dlfcn.h>

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dberr_e
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief error code를 문자열로 변환하여 반환한다.
 * @param err error code
 * @return 전달된 error code에 대한 문자열
 */
UX_DECLARE(const char*) uims_dberr_to_str(uims_dberr_e err)
{
    switch( err ) {
        case UIMS_DB_SUCCESS : return "SUCCESS";
        case UIMS_DBERR_ERROR : return "Error";
        case UIMS_DBERR_CANT_CONNECT : return "Can't connect";
        case UIMS_DBERR_DISCONNECTED : return "Disconnected";
        case UIMS_DBERR_INVALID_PARAMETER : return "Invalid parameter";
        case UIMS_DBERR_CANT_BIND : return "Can't bind";
        case UIMS_DBERR_ALREADY_EXISTS : return "Aleady exists data";
        case UIMS_DBERR_NO_DATA : return "No data";
        case UIMS_DBERR_CANT_OPEN_DLL : return dlerror();
        case UIMS_DBERR_NOT_FOUND_FUNC : return dlerror();
        case UIMS_DBERR_NO_MEMORY : return "No memory";
        case UIMS_DBERR_INVALID_STATEMENT : return "Invalid statement";
        case UIMS_DBERR_NOT_SUPPORTED : return "Not supporeted";
        case UIMS_DBERR_INVALID_IDX : return "Invalid index";
        case UIMS_DBERR_INVALID_TYPE : return "Invalid data type";
        case UIMS_DBERR_INVALID_VALUE : return "Invalid data value";
        case UIMS_DBERR_INVALID_LEN : return "Invalid data length";
        case UIMS_DBERR_INVALID_SIZE : return "Invalid data buffer size";
        case UIMS_DBERR_INVALID_CONF : return "Invalid configuration data";
        case UIMS_DBERR_INVALID_ADDR : return "Invalid address";
        case UIMS_DBERR_OUT_OF_RANGE : return "Out of range";
        case UIMS_DBERR_TOO_BIG : return "Too big size";
        case UIMS_DBERR_SYSTEM : return "System error";
        case UIMS_DBERR_TIMEOUT : return "Timeout error";
        case UIMS_DBERR_CANT_ACCESS : return "Can't access";
		default : break;
    }
    return "UNKNOWN";
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbtype_e
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief DB data type을 문자열로 변환하여 반환한다.
 * @param type data type
 * @return 전달된 DB data type에 대한 문자열
 */
UX_DECLARE(const char*) uims_dbtype_to_str( uims_dbtype_e type)
{
	switch( type) {
		case UIMS_DBTYPE_NIL : return "NIL";
		case UIMS_DBTYPE_INT8 : return "INT8";
		case UIMS_DBTYPE_UINT8 : return "UINT8";
		case UIMS_DBTYPE_INT16 : return "INT16";
		case UIMS_DBTYPE_UINT16 : return "UINT16";
		case UIMS_DBTYPE_INT32 : return "INT32";
		case UIMS_DBTYPE_UINT32 : return "UINT32";
		case UIMS_DBTYPE_INT64 : return "INT64";
		case UIMS_DBTYPE_UINT64 : return "UINT64";
		case UIMS_DBTYPE_FLOAT : return "FLOAT";
		case UIMS_DBTYPE_DOUBLE : return "DOUBLE";
		case UIMS_DBTYPE_STR : return "STRING";
		case UIMS_DBTYPE_OCT : return "OCTET";
		default : break;
	}
	return "UNKNOWN";
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbdataset_t
////////////////////////////////////////////////////////////////////////////////

uims_dberr_e uims_dbdataset_init( uims_dbdataset_t *dbdataset, uims_dbstmt_t *dbstmt,
						uint32_t size)
{
	dbdataset->datas = ux_zalloc( ux_mem_default(), sizeof(uims_dbdata_t) * size);
	if(dbdataset->datas == NULL) {
		ux_log(UXL_CRT, "Failed to allocate data set memory. (size=%lu)", sizeof(uims_dbdata_t)*size);
		return UIMS_DBERR_NO_MEMORY;
	}

	dbdataset->stmt = dbstmt;
	dbdataset->count = 0;
	dbdataset->size = size;

	return UIMS_DB_SUCCESS;
}

void uims_dbdataset_final( uims_dbdataset_t *dbdataset)
{
	if( dbdataset->datas ) ux_free( ux_mem_default(), dbdataset->datas);
}

uims_dberr_e uims_dbdataset_resize( uims_dbdataset_t *dbdataset, uint32_t size)
{
	uims_dbdata_t *datas;

	if( size <= dbdataset->size) return UIMS_DB_SUCCESS;

	datas = ux_zalloc( ux_mem_default(), sizeof(uims_dbdata_t) * size);
	if( datas == NULL) {
		ux_log(UXL_CRT, "Failed to allocate DB parameter data memory. (size=%lu)", sizeof(uims_dbdata_t) * size);
		return UIMS_DBERR_NO_MEMORY;
	}
	if( dbdataset->datas ) {
		memcpy( datas, dbdataset->datas, sizeof(uims_dbdata_t) * dbdataset->count);
		ux_free( ux_mem_default(), dbdataset->datas);
	}
	dbdataset->size = size;
	dbdataset->datas = datas;

	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_next( uims_dbdataset_t *dbdataset)
{
	return uims_dbstmt_fetch( dbdataset->stmt);
}


UX_DECLARE(uint32_t) uims_dbdataset_get_count( uims_dbdataset_t *dbdataset)
{
	return dbdataset->count; 
}

UX_DECLARE(void) uims_dbdataset_reset_count( uims_dbdataset_t *dbdataset)
{
	dbdataset->count = 0; 
}

UX_DECLARE(uims_dbdata_t*) uims_dbdataset_get_data( uims_dbdataset_t *dbdataset,
						uint32_t idx)
{
	if( idx >= dbdataset->count) return NULL;

	return &dbdataset->datas[idx];
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_data( uims_dbdataset_t *dbdataset,
						uint32_t idx, uims_dbdata_t *dbdata)
{
	int rv;

	if( idx > dbdataset->count) {
		ux_log(UXL_MIN, "Failed to get data to set value. (idx=%u, count=%u)", idx, dbdataset->count); 
		return UIMS_DBERR_INVALID_IDX;
	}
	if( idx == dbdataset->size) {
		rv = uims_dbdataset_resize( dbdataset, idx+16);
		if( rv < UIMS_DB_SUCCESS) return rv; 
	}

	memcpy( &dbdataset->datas[idx], dbdata, sizeof(uims_dbdata_t));
	if( idx == dbdataset->count) dbdataset->count++;
	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dbtype_e) uims_dbdataset_get_type( uims_dbdataset_t *dbdataset,
						uint32_t idx)
{
	if( idx > dbdataset->count) return UIMS_DBTYPE_UNKNOWN;
	return dbdataset->datas[idx].type;
}

UX_DECLARE(uint32_t) uims_dbdataset_get_len( uims_dbdataset_t *dbdataset,
						uint32_t idx)
{
	if( idx > dbdataset->count) return 0;
	return dbdataset->datas[idx].len;
}

UX_DECLARE(uint8_t) uims_dbdataset_get_flags( uims_dbdataset_t *dbdataset,
						uint32_t idx)
{
	if( idx > dbdataset->count) return UIMS_DBERR_INVALID_IDX;
	return dbdataset->datas[idx].flags;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_flags( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint8_t flags)
{
	if( idx >= dbdataset->count) {
		ux_log(UXL_MIN, "Failed to set flags. (idx=%u, count=%u)", idx, dbdataset->count); 
		return UIMS_DBERR_INVALID_IDX;
	}

	dbdataset->datas[idx].flags = flags;
	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uint32_t) uims_dbdataset_get_size( uims_dbdataset_t *dbdataset,
						uint32_t idx)
{
	if( idx > dbdataset->count) return 0;
	return dbdataset->datas[idx].size;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_size( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint32_t size)
{
	if( idx >= dbdataset->count) {
		ux_log(UXL_MIN, "Failed to set flags. (idx=%u, count=%u)", idx, dbdataset->count); 
		return UIMS_DBERR_INVALID_IDX;
	}

	dbdataset->datas[idx].size = size;
	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_value( uims_dbdataset_t *dbdataset,
						uint32_t idx, uims_dbtype_e type, void *vptr, int len)
{
	int rv;
	uims_dbdata_t *dbdata;

	if( idx > dbdataset->count) {
		ux_log(UXL_MIN, "Failed to get data to set value. (idx=%u, count=%u)", idx, dbdataset->count); 
		return UIMS_DBERR_INVALID_IDX;
	}
	if( idx == dbdataset->size) {
		rv = uims_dbdataset_resize( dbdataset, idx+16);
		if( rv < UIMS_DB_SUCCESS) return rv; 
	}

	dbdata = &dbdataset->datas[idx];
	dbdata->type = type;
	switch( type) {
		case UIMS_DBTYPE_INT8 :
			dbdata->v.i8 = *((int8_t*)(vptr));
			dbdata->len = sizeof(int8_t);
			break;
		case UIMS_DBTYPE_UINT8 :
			dbdata->v.u8 = *((uint8_t*)(vptr));
			dbdata->len = sizeof(uint8_t);
			break;
		case UIMS_DBTYPE_INT16 :
			dbdata->v.i16 = *((int16_t*)(vptr));
			dbdata->len = sizeof(int16_t);
			break;
		case UIMS_DBTYPE_UINT16 :
			dbdata->v.u16 = *((uint16_t*)(vptr));
			dbdata->len = sizeof(uint16_t);
			break;
		case UIMS_DBTYPE_INT32 :
			dbdata->v.i32 = *((int32_t*)(vptr));
			dbdata->len = sizeof(int32_t);
			break;
		case UIMS_DBTYPE_UINT32 :
			dbdata->v.u32 = *((uint32_t*)(vptr));
			dbdata->len = sizeof(uint32_t);
			break;
		case UIMS_DBTYPE_INT64 :
			dbdata->v.i64 = *((int64_t*)(vptr));
			dbdata->len = sizeof(int64_t);
			break;
		case UIMS_DBTYPE_UINT64 :
			dbdata->v.u64 = *((uint64_t*)(vptr));
			dbdata->len = sizeof(uint64_t);
			break;
		case UIMS_DBTYPE_FLOAT :
			dbdata->v.f32 = *((float*)(vptr));
			dbdata->len = sizeof(float);
			break;
		case UIMS_DBTYPE_DOUBLE :
			dbdata->v.d64 = *((double*)(vptr));
			dbdata->len = sizeof(double);
			break;
		case UIMS_DBTYPE_STR :
			if( vptr == NULL || len < 0) {
				dbdata->type = UIMS_DBTYPE_NIL;
				dbdata->v.str = NULL;
				dbdata->len = 0;
			} else {
				dbdata->v.str = (char*)vptr;
				dbdata->len = len;
			} break;
		case UIMS_DBTYPE_OCT :
			if( vptr == NULL || len < 0) {
				dbdata->type = UIMS_DBTYPE_NIL;
				dbdata->v.str = NULL;
				dbdata->len = 0;
			} else {
				dbdata->v.oct = (uint8_t*)vptr;
				dbdata->len = len;
			} break;
		default :
			ux_log(UXL_MIN, "Failed to set int value at [%d]. because %s type can't convert int.",
					idx, uims_dbtype_to_str(type));
			return UIMS_DBERR_INVALID_TYPE;
	}

	if( idx == dbdataset->count) dbdataset->count++;
	return UIMS_DB_SUCCESS;
}

UX_DECLARE(int64_t) uims_dbdataset_get_int( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *err)
{
	uims_dbdata_t *dbdata;

	if( idx > dbdataset->count) {
		if(err) *err = UIMS_DBERR_INVALID_IDX;
		return 0;
	}

	dbdata = &dbdataset->datas[idx];
	if(err) *err = UIMS_DB_SUCCESS;
	switch( dbdata->type) {
		case UIMS_DBTYPE_INT8 : return dbdata->v.i8;
		case UIMS_DBTYPE_UINT8 : return dbdata->v.u8;
		case UIMS_DBTYPE_INT16 : return dbdata->v.i16;
		case UIMS_DBTYPE_UINT16 : return dbdata->v.u16;
		case UIMS_DBTYPE_INT32 : return dbdata->v.i32;
		case UIMS_DBTYPE_UINT32 : return dbdata->v.u32;
		case UIMS_DBTYPE_INT64 : return dbdata->v.i64;
		case UIMS_DBTYPE_UINT64 : return dbdata->v.u64;
		case UIMS_DBTYPE_FLOAT : return (int64_t)dbdata->v.f32;
		case UIMS_DBTYPE_DOUBLE : return (int64_t)dbdata->v.d64;
		default : break;	
	}
	if(err) *err = UIMS_DBERR_INVALID_TYPE;
	return 0;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int( uims_dbdataset_t *dbdataset,
						uint32_t idx, int64_t val)
{
	if( val <= 0x7FLL) {
		return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT8, &val, sizeof(int8_t));
	} else if( val <= 0x7FFFLL) {
		return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT16, &val, sizeof(int16_t));
	} else if( val <= 0x7FFFFFFFLL) {
		return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT32, &val, sizeof(int32_t));
	}
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT64, &val, sizeof(int64_t));
}

UX_DECLARE(uint64_t) uims_dbdataset_get_uint( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *err)
{
	uims_dbdata_t *dbdata;

	if( idx > dbdataset->count) {
		if(err) *err = UIMS_DBERR_INVALID_IDX;
		return 0;
	}

	dbdata = &dbdataset->datas[idx];
	if(err) *err = UIMS_DB_SUCCESS;
	switch( dbdata->type) {
		case UIMS_DBTYPE_INT8 : return dbdata->v.i8;
		case UIMS_DBTYPE_UINT8 : return dbdata->v.u8;
		case UIMS_DBTYPE_INT16 : return dbdata->v.i16;
		case UIMS_DBTYPE_UINT16 : return dbdata->v.u16;
		case UIMS_DBTYPE_INT32 : return dbdata->v.i32;
		case UIMS_DBTYPE_UINT32 : return dbdata->v.u32;
		case UIMS_DBTYPE_INT64 : return dbdata->v.i64;
		case UIMS_DBTYPE_UINT64 : return dbdata->v.u64;
		case UIMS_DBTYPE_FLOAT : return (uint64_t)dbdata->v.f32;
		case UIMS_DBTYPE_DOUBLE : return (uint64_t)dbdata->v.d64;
		default : break;	
	}
	if(err) *err = UIMS_DBERR_INVALID_TYPE;
	return 0;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint64_t val)
{
	if( val <= 0xFFUL) {
		return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT8, &val, sizeof(uint8_t));
	} else if( val <= 0xFFFFUL) {
		return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT16, &val, sizeof(uint16_t));
	} else if( val <= 0xFFFFFFFFUL) {
		return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT32, &val, sizeof(uint32_t));
	}
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT64, &val, sizeof(uint64_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int8( uims_dbdataset_t *dbdataset,
						uint32_t idx, int8_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT8, &val, sizeof(int8_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint8( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint8_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT8, &val, sizeof(uint8_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int16( uims_dbdataset_t *dbdataset,
						uint32_t idx, int16_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT16, &val, sizeof(int16_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint16( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint16_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT16, &val, sizeof(uint16_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int32( uims_dbdataset_t *dbdataset,
						uint32_t idx, int32_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT32, &val, sizeof(int32_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint32( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint32_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT32, &val, sizeof(uint32_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int64( uims_dbdataset_t *dbdataset,
						uint32_t idx, int64_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_INT64, &val, sizeof(int64_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint64( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint64_t val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_UINT64, &val, sizeof(uint64_t));
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_float( uims_dbdataset_t *dbdataset,
						uint32_t idx, float val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_FLOAT, &val, sizeof(float));
}

UX_DECLARE(double) uims_dbdataset_get_double( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *err)
{
	uims_dbdata_t *dbdata;

	if( idx > dbdataset->count) {
		if(err) *err = UIMS_DBERR_INVALID_IDX;
		return 0;
	}

	dbdata = &dbdataset->datas[idx];
	if(err) *err = UIMS_DB_SUCCESS;
	switch( dbdata->type) {
		case UIMS_DBTYPE_INT8 : return dbdata->v.i8;
		case UIMS_DBTYPE_UINT8 : return dbdata->v.u8;
		case UIMS_DBTYPE_INT16 : return dbdata->v.i16;
		case UIMS_DBTYPE_UINT16 : return dbdata->v.u16;
		case UIMS_DBTYPE_INT32 : return dbdata->v.i32;
		case UIMS_DBTYPE_UINT32 : return dbdata->v.u32;
		case UIMS_DBTYPE_INT64 : return dbdata->v.i64;
		case UIMS_DBTYPE_UINT64 : return dbdata->v.u64;
		case UIMS_DBTYPE_FLOAT : return (double)dbdata->v.f32;
		case UIMS_DBTYPE_DOUBLE : return (double)dbdata->v.d64;
		default : break;	
	}
	if(err) *err = UIMS_DBERR_INVALID_TYPE;
	return 0;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_double( uims_dbdataset_t *dbdataset,
						uint32_t idx, double val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_DOUBLE, &val, sizeof(double));
}

UX_DECLARE(const char*) uims_dbdataset_get_str( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *len)
{
	uims_dbdata_t *dbdata;

	if( idx > dbdataset->count) {
		if(len) *len = UIMS_DBERR_INVALID_IDX;
		return NULL;
	}

	dbdata = &dbdataset->datas[idx];
	switch( dbdata->type) {
		case UIMS_DBTYPE_NIL :
			if(len) *len = 0;
			return NULL;
		case UIMS_DBTYPE_STR :
			if(len) *len = dbdata->len;
			if( dbdata->v.str[dbdata->len]) dbdata->v.str[dbdata->len] = '\0';
			return dbdata->v.str;
		case UIMS_DBTYPE_OCT : 
			if(len) *len = dbdata->len;
			return (char*)dbdata->v.oct;
		default : break;	
	}
	if(len) *len = UIMS_DBERR_INVALID_TYPE;
	return NULL;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_str( uims_dbdataset_t *dbdataset,
						uint32_t idx, const char *val)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_STR, (void*)val, strlen(val)+1);
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_strn( uims_dbdataset_t *dbdataset,
						uint32_t idx, const char *val, int len)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_STR, (void*)val, len);
}

UX_DECLARE(uint8_t*) uims_dbdataset_get_oct( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *len)
{
	uims_dbdata_t *dbdata;

	if( idx > dbdataset->count) {
		if(len) *len = UIMS_DBERR_INVALID_IDX;
		return NULL;
	}

	dbdata = &dbdataset->datas[idx];
	switch( dbdata->type) {
		case UIMS_DBTYPE_NIL :
			if(len) *len = 0;
			return NULL;
		case UIMS_DBTYPE_STR :
			if(len) *len = dbdata->len;
			return (uint8_t*)dbdata->v.str;
		case UIMS_DBTYPE_OCT : 
			if(len) *len = dbdata->len;
			return dbdata->v.oct;
		default : break;	
	}
	if(len) *len = UIMS_DBERR_INVALID_TYPE;
	return NULL;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_set_oct( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint8_t *val, int len)
{
	return uims_dbdataset_set_value( dbdataset, idx, UIMS_DBTYPE_OCT, (void*)val, len);
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_writev( uims_dbdataset_t *dbdataset,
						int count, va_list args)
{
	int rv, i;
	const char *name;
	uims_dbdata_t *dbdata;

	if( count > dbdataset->size) {
		rv = uims_dbdataset_resize( dbdataset, count);
		if( rv < UIMS_DB_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to write value. (err=%d,%s)", rv, uims_dberr_to_str(rv)); 
			return rv;
		}
	}

	for( i = 0; i < count; ++i) {
		dbdata = &dbdataset->datas[i];

		name = va_arg( args, const char*);
		dbdata->type = va_arg( args, int);
		switch( dbdata->type) {
			case UIMS_DBTYPE_NIL :
				dbdata->v.str = va_arg( args, char*);
				dbdata->v.str = NULL;
				dbdata->len = 0; 
				break;
			case UIMS_DBTYPE_INT8 :
				dbdata->v.i8 = va_arg( args, int);
				dbdata->len = sizeof(int8_t);
				break;
			case UIMS_DBTYPE_UINT8 :
				dbdata->v.u8 = va_arg( args, unsigned int);
				dbdata->len = sizeof(uint8_t);
				break;
			case UIMS_DBTYPE_INT16 :
				dbdata->v.i16 = va_arg( args, int);
				dbdata->len = sizeof(int16_t);
				break;
			case UIMS_DBTYPE_UINT16 :
				dbdata->v.u16 = va_arg( args, unsigned int);
				dbdata->len = sizeof(uint16_t);
				break;
			case UIMS_DBTYPE_INT32 :
				dbdata->v.i32 = va_arg( args, int);
				dbdata->len = sizeof(int32_t);
				break;
			case UIMS_DBTYPE_UINT32 :
				dbdata->v.u32 = va_arg( args, int);
				dbdata->len = sizeof(uint32_t);
				break;
			case UIMS_DBTYPE_INT64 :
				dbdata->v.i64 = va_arg( args, int64_t);
				dbdata->len = sizeof(int64_t);
				break;
			case UIMS_DBTYPE_UINT64 :
				dbdata->v.u64 = va_arg( args, uint64_t);
				dbdata->len = sizeof(uint64_t);
				break;
			case UIMS_DBTYPE_FLOAT :
				dbdata->v.f32 = va_arg( args, double);
				dbdata->len = sizeof(float);
				break;
			case UIMS_DBTYPE_DOUBLE :
				dbdata->v.d64 = va_arg( args, double);
				dbdata->len = sizeof(double);
				break;
			case UIMS_DBTYPE_STR :
				dbdata->v.str = va_arg( args, char*);
				if(dbdata->v.str == NULL) {
					dbdata->len = 0;
					dbdata->type = UIMS_DBTYPE_NIL;
				} else {
					dbdata->len = strlen(dbdata->v.str);
				}
				break;
			case UIMS_DBTYPE_OCT :
				dbdata->v.oct = va_arg( args, uint8_t*);
				dbdata->len = va_arg( args, unsigned int);
				if(dbdata->v.oct == NULL || dbdata->len < 0) {
					dbdata->v.oct = NULL;
					dbdata->len = 0;
					dbdata->type = UIMS_DBTYPE_NIL;
				}
				break;
			default :
				dbdataset->count = i;
				ux_log(UXL_MAJ, "Invalid data value. (name=%s, index=%d, type=%s)",
						name? name : "", i, uims_dbtype_to_str(dbdata->type)); 
				return UIMS_DBERR_INVALID_TYPE;
		}
	}

	dbdataset->count = count;
	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dberr_e) uims_dbdataset_write( uims_dbdataset_t *dbdataset,
						int count, ...)
{
	int rv;
	va_list args;

	va_start(args, count);
	rv = uims_dbdataset_writev( dbdataset, count, args);
	va_end(args);

	return rv;
}

UX_DECLARE(int) uims_dbdataset_readv( uims_dbdataset_t *dbdataset, int count,
						va_list args)
{
	int rv, i, type, *plen;
	void *ptr;
	const char *name;
	uims_dbdata_t *dbdata;

	for( i = 0; i < count; ++i) {
		if( i >= dbdataset->count) return i;

		dbdata = &dbdataset->datas[i]; 
		name = va_arg( args, const char*);
		type = va_arg( args, int);
		ptr = va_arg( args, void*);

		switch( type) {
			case UIMS_DBTYPE_INT8 :
				*((int8_t*)ptr) = uims_dbdataset_get_int( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_UINT8 :
				*((uint8_t*)ptr) = uims_dbdataset_get_uint( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_INT16 :
				*((int16_t*)ptr) = uims_dbdataset_get_int( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_UINT16 :
				*((uint16_t*)ptr) = uims_dbdataset_get_uint( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_INT32 :
				*((int32_t*)ptr) = uims_dbdataset_get_int( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_UINT32 :
				*((uint32_t*)ptr) = uims_dbdataset_get_uint( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_INT64 :
				*((int64_t*)ptr) = uims_dbdataset_get_int( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_UINT64 :
				*((uint64_t*)ptr) = uims_dbdataset_get_uint( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_FLOAT :
				*((float*)ptr) = uims_dbdataset_get_double( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_DOUBLE :
				*((double*)ptr) = uims_dbdataset_get_double( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_STR :
				*((char**)ptr) = (char*)uims_dbdataset_get_str( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				break;
			case UIMS_DBTYPE_OCT :
				*((uint8_t**)ptr) = (uint8_t*)uims_dbdataset_get_oct( dbdataset, i, &rv);
				if( rv < UX_SUCCESS) goto error;
				plen = va_arg( args, int*);
				*plen = rv;
				break;
			default :
				ux_log(UXL_MAJ, "Invalid read data value. (name=%s, index=%d, type=%s)",
						name? name : "", i, uims_dbtype_to_str(type)); 
				return UIMS_DBERR_INVALID_TYPE;
		}
	}

	return i;

error:
	ux_log(UXL_MAJ, "Failed to read value. (name=%s, index=%d, type=%s, err=%d,%s)",
			name? name : "", i, uims_dbtype_to_str(type), rv, uims_dberr_to_str(rv)); 
	return rv;
}

UX_DECLARE(int) uims_dbdataset_read( uims_dbdataset_t *dbdataset, int count, ...)
{
	int rv;
	va_list args;

	va_start(args, count);
	rv = uims_dbdataset_readv( dbdataset, count, args);
	va_end(args);

	return rv;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbstmt_t
////////////////////////////////////////////////////////////////////////////////

uims_dbstmt_t* uims_dbstmt_create( uims_dbc_t *dbc, int *err)
{
	int rv;
	uims_dbstmt_t *dbstmt;

	dbstmt = (uims_dbstmt_t*)ux_zalloc( ux_mem_default(), sizeof(uims_dbstmt_t));
	if( dbstmt == NULL) {
		ux_log( UXL_CRT, "Failed to allocate memory for uims_dbstmt_t. (id=%u, name=%s)",
				dbc->db->id, dbc->db->name ? dbc->db->name : "NULL")
		return NULL;
	}

	rv = uims_dbstmt_init( dbstmt, dbc);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize memory for uims_dbstmt_t. (id=%u, name=%s, err=%d,%s)", 
				dbc->db->id, dbc->db->name ? dbc->db->name : "NULL", rv, uims_dberr_to_str(rv))
		ux_free( ux_mem_default(), dbstmt);
		return NULL;
	}

	return dbstmt;
}

void uims_dbstmt_destroy( uims_dbstmt_t *dbstmt)
{
	uims_dbstmt_final( dbstmt);
	ux_free( ux_mem_default(), dbstmt);
}

uims_dberr_e uims_dbstmt_init( uims_dbstmt_t *dbstmt, uims_dbc_t *dbc)
{
	dbstmt->dbc = dbc;
	uims_dbdataset_init( dbstmt->paraset, dbstmt, 64);
	uims_dbdataset_init( dbstmt->rsltset, dbstmt, 64);
	return UIMS_DB_SUCCESS;
}

void uims_dbstmt_final( uims_dbstmt_t *dbstmt)
{
	uims_dbstmt_close( dbstmt);
	uims_dbdataset_final( dbstmt->rsltset);
	uims_dbdataset_final( dbstmt->paraset);
}

UX_DECLARE(const char*) uims_dbstmt_get_stmtid( uims_dbstmt_t *dbstmt)
{
	return dbstmt->stmtid;
}

UX_DECLARE(const char*) uims_dbstmt_get_query( uims_dbstmt_t *dbstmt)
{
	return dbstmt->query;
}

UX_DECLARE(uint32_t) uims_dbstmt_get_mode( uims_dbstmt_t *dbstmt)
{
	return dbstmt->mode;
}

UX_DECLARE(void) uims_dbstmt_set_mode( uims_dbstmt_t *dbstmt, uint32_t mode)
{
	dbstmt->mode = mode;
}

UX_DECLARE(uims_dbc_t*) uims_dbstmt_get_dbc( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc;
}

UX_DECLARE(void*) uims_dbstmt_get_dbc_handle( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc->handle;
}

UX_DECLARE(uims_db_t*) uims_dbstmt_get_db( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc->db;
}

UX_DECLARE(void*) uims_dbstmt_get_db_handle( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc->db->handle;
}

UX_DECLARE(void*) uims_dbstmt_get_handle( uims_dbstmt_t *dbstmt)
{
	return dbstmt->handle;
}

UX_DECLARE(void) uims_dbstmt_set_handle( uims_dbstmt_t *dbstmt, void *handle)
{
	dbstmt->handle = handle;
}

UX_DECLARE(uims_dbdataset_t*) uims_dbstmt_get_paraset( uims_dbstmt_t *dbstmt)
{
	return dbstmt->paraset;
}

UX_DECLARE(uims_dbdataset_t*) uims_dbstmt_get_rsltset( uims_dbstmt_t *dbstmt)
{
	return dbstmt->rsltset;
}

uims_dberr_e uims_dbstmt_open( uims_dbstmt_t *dbstmt, const char *stmtid, const char *query)
{
	int rv;

	dbstmt->stmtid = stmtid;
	dbstmt->query = query;
	dbstmt->mode = 0;
	dbstmt->handle = NULL;
	dbstmt->paraset->count = 0;
	dbstmt->rsltset->count = 0;

	rv = dbstmt->dbc->db->module->open_stmt( dbstmt, stmtid, query);
	if( rv < UIMS_DB_SUCCESS) {
		dbstmt->stmtid = NULL;
		dbstmt->query = NULL;
		return rv;
	}

	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dberr_e) uims_dbstmt_execute( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc->db->module->execute_stmt( dbstmt);
}

UX_DECLARE(uims_dberr_e) uims_dbstmt_fetch( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc->db->module->fetch_stmt( dbstmt);
}

UX_DECLARE(ssize_t) uims_dbstmt_affected_row_count( uims_dbstmt_t *dbstmt)
{
	return dbstmt->dbc->db->module->get_affected_row_count( dbstmt);
}

UX_DECLARE(void) uims_dbstmt_close( uims_dbstmt_t *dbstmt)
{
	dbstmt->dbc->db->module->close_stmt( dbstmt);
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_db_t
////////////////////////////////////////////////////////////////////////////////

uims_dbc_t* uims_dbc_create( uims_db_t *db, int *err)
{
	int rv;
	uims_dbc_t *dbc;

	dbc = (uims_dbc_t*)ux_zalloc( ux_mem_default(), sizeof(uims_dbc_t));
	if( dbc == NULL) {
		ux_log( UXL_CRT, "Failed to allocate memory for uims_dbc_t. (id=%u, name=%s)",
				db->id, db->name ? db->name : "NULL")
		return NULL;
	}

	rv = uims_dbc_init( dbc, db);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize memory for uims_dbc_t. (id=%u, name=%s, err=%d,%s)", 
				db->id, db->name ? db->name : "NULL", rv, uims_dberr_to_str(rv))
		ux_free( ux_mem_default(), dbc);
		return NULL;
	}

	return dbc;
}

void uims_dbc_destroy( uims_dbc_t *dbc)
{
	uims_dbc_final( dbc);
	ux_free( ux_mem_default(), dbc);
}

uims_dberr_e uims_dbc_init( uims_dbc_t *dbc, uims_db_t *db)
{
	dbc->db = db;
	dbc->handle = NULL;
	dbc->freestmt = NULL;
	uims_dbstmt_init( dbc->stmt, dbc);
	return UIMS_DB_SUCCESS;
}

void uims_dbc_final( uims_dbc_t *dbc)
{
	uims_dbstmt_t *dbstmt;

	while( dbc->freestmt) {
		dbstmt = dbc->freestmt;
		dbc->freestmt = dbstmt->next;
		dbstmt->next = NULL;
		uims_dbstmt_destroy( dbstmt);
	}
	uims_dbstmt_final( dbc->stmt);
	uims_dbc_close( dbc);
}

uims_dberr_e uims_dbc_open( uims_dbc_t *dbc)
{
	return dbc->db->module->open_conn( dbc); 
}

void uims_dbc_close( uims_dbc_t *dbc)
{
	return dbc->db->module->close_conn( dbc); 
}

uims_dbstmt_t* uims_dbc_alloc_extra_stmt( uims_dbc_t *dbc, int *err)
{
	uims_dbstmt_t *dbstmt;

	if( dbc->freestmt) {
		dbstmt = dbc->freestmt;
		dbc->freestmt = dbstmt->next;
		dbstmt->next = NULL;
	} else {
		dbstmt = uims_dbstmt_create( dbc, err);
	}

	return dbstmt;
}

void uims_dbc_free_extra_stmt( uims_dbc_t *dbc, uims_dbstmt_t *dbstmt)
{
	dbstmt->next = dbc->freestmt;
	dbc->freestmt = dbstmt;
}

UX_DECLARE(uims_db_t*) uims_dbc_get_db( uims_dbc_t *dbc)
{
	return dbc->db;
}

UX_DECLARE(void*) uims_dbc_get_db_handle( uims_dbc_t *dbc)
{
	return dbc->db->handle;
}

UX_DECLARE(void*) uims_dbc_get_handle( uims_dbc_t *dbc)
{
	return dbc->handle;
}

UX_DECLARE(void) uims_dbc_set_handle( uims_dbc_t *dbc, void *handle)
{
	dbc->handle = handle;
}

UX_DECLARE(uims_dbstmt_t*) uims_dbc_open_stmt( uims_dbc_t *dbc, const char *stmtid,
							const char *query, int *err)
{
	int rv;

	if( dbc->stmt->handle != NULL) {
		rv = UX_EEXIST; 
		ux_log(UXL_MAJ, "DB Statement is aleady opened. (stmtid=%s, query=%s, err=%d,%s)",
				stmtid, query, rv, ux_errnostr(rv));
		if( err) *err = rv; 
		return NULL;
	}

	rv = uims_dbstmt_open( dbc->stmt, stmtid, query);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to open extra statement. (stmtid=%s, query=%s, err=%d,%s)",
				stmtid, query, rv, ux_errnostr(rv));
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return dbc->stmt;
}

UX_DECLARE(uims_dbstmt_t*) uims_dbc_open_extra_stmt( uims_dbc_t *dbc, const char *stmtid,
							const char *query, int *err)
{
	int rv;
	uims_dbstmt_t* dbstmt;
	
	dbstmt = uims_dbc_alloc_extra_stmt( dbc, &rv);
	if( dbstmt == NULL) {
		rv = UX_ENOMEM;
		ux_log(UXL_CRT, "Failed to allocate extra statement. (stmtid=%s, query=%s, err=%d,%s)",
				stmtid, query, rv, ux_errnostr(rv));
		if(err) *err = rv;
		return NULL;
	}

	rv = uims_dbstmt_open( dbstmt, stmtid, query);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MIN, "Failed to open extra statement. (stmtid=%s, query=%s, err=%d,%s)",
				stmtid, query, rv, ux_errnostr(rv));
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return dbc->stmt;

}

UX_DECLARE(uims_dberr_e) uims_dbc_close_extra_stmt( uims_dbc_t *dbc, uims_dbstmt_t *dbstmt)
{
	if( dbstmt == NULL) {
		ux_log(UXL_MIN, "Closing extra statement is NULL.");
		return UX_EINVAL;
	}
	if( dbc != dbstmt->dbc) {
		ux_log(UXL_MIN, "Extra statement's db connection is invalid. (dbc=%p, owner=%p)", dbc, dbstmt->dbc);
		return UX_EINVAL;
	}

	uims_dbstmt_close( dbstmt);
	uims_dbc_free_extra_stmt( dbc, dbstmt);

	return UX_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_db_t
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(uims_db_t*) uims_db_create( uint32_t id, const char *name, const char *dll,
					const char *func, const char *confstr, int *err)
{
	int rv;
	uims_db_t *db;

	db = (uims_db_t*)ux_zalloc( ux_mem_default(), sizeof(uims_db_t));
	if( db == NULL) {
		ux_log( UXL_CRT, "Failed to allocate memory for uims_db_t. (id=%u, name=%s)",
				id, name ? name : "NULL")
		return NULL;
	}

	rv = uims_db_init( db, id, name, dll, func, confstr);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize memory for uims_db_t. (id=%u, name=%s, err=%d,%s)", 
				id, name ? name : "NULL", rv, uims_dberr_to_str(rv))
		ux_free( ux_mem_default(), db);
		return NULL;
	}

	return db;
}

UX_DECLARE(void) uims_db_destroy( uims_db_t *db)
{
	uims_db_final( db);
	ux_free( ux_mem_default(), db);
}

uims_dberr_e uims_db_init( uims_db_t *db, uint32_t id, const char *name, const char *dll,
						const char *func, const char *confstr)
{
	int rv;
	void *handle;
	char *dlerrstr;
	uims_db_get_module_f getf;

	if( name == NULL || name[0] == '\0' || func == NULL || func[0] == '\0') {
		ux_log(UXL_MAJ, "Invalid name or function. (id=%u, name=%s, func=%s)",
				id, name ? name : "NULL", func ? func : "NULL");
		return UIMS_DBERR_INVALID_PARAMETER;
	}

	ux_refcount_init( db->refcnt);

	db->id = id;
	db->name = ux_str_dup( name, ux_mem_default());
	db->dll = dll ? ux_str_dup( dll, ux_mem_default()) : NULL;
	db->func = func ? ux_str_dup( func, ux_mem_default()) : NULL;
	db->confstr = confstr ? ux_str_dup( confstr, ux_mem_default()) : NULL;
	if( db->name == NULL || (dll && db->dll == NULL) ||
		db->func == NULL || (confstr && db->confstr == NULL))
	{
		ux_log(UXL_CRT, "Failed to dupicate name. (id=%u, name=%s, dll=%s, func=%s, confstr=%s)",
				id, name ? name : "NULL", dll ? dll : "NULL", func ? func : "NULL",
				confstr ? confstr : "NULL");
		uims_db_final( db);
		return UIMS_DBERR_NO_MEMORY;
	}

	handle = dlopen( db->dll && db->dll[0] ? db->dll : NULL, RTLD_GLOBAL | RTLD_NOW);
	if( handle == NULL ) {
		dlerrstr = dlerror();
		ux_log(UXL_MAJ, "Failed to dlopen for uims_db_t. (id=%d, name=%s, dll=%s, err=%s)",
				id, name, db->dll ? db->dll : "", dlerrstr ? dlerrstr : "unknown");
		uims_db_final( db);
		return UIMS_DBERR_CANT_OPEN_DLL;
	}
	getf = (uims_db_get_module_f)dlsym( handle, db->func);
	if( getf == NULL) {
		dlerrstr = dlerror();
		ux_log(UXL_MAJ, "Failed to get function by dlsym. (id=%d, name=%s, dll=%s, func=%s, err=%s)",
				id, name, db->dll ? db->dll : "", db->func, dlerrstr ? dlerrstr : "unknown");
		uims_db_final( db);
		return UIMS_DBERR_NOT_FOUND_FUNC;
	}
	db->module = getf();
	if( db->module == NULL) {
		ux_log(UXL_MAJ, "Failed to get DB module. (id=%d, name=%s, dll=%s, func=%s)",
				id, name, db->dll ? db->dll : "", db->func);
		return UIMS_DBERR_NO_DATA;
	}

	rv = db->module->open_db( db);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to open DB. (id=%d, name=%s, dll=%s, func=%s, confstr=%s)",
				id, name, db->dll ? db->dll : "", db->func, db->confstr ? db->confstr : "");
		return rv;
	}

	rv = pthread_key_create( &db->tskey, (void (*)(void *))uims_dbc_destroy);
	if( rv != 0 ) {
		rv = UX_ERROR();
		ux_log(UXL_MAJ, "Failed to pthread_key_create for uims_db_t. (id=%d, name=%s, err=%d,%s)",
				id, name, rv, ux_errnostr(rv));
		return UIMS_DBERR_NO_MEMORY;
	}

	return UIMS_DB_SUCCESS;
}

void uims_db_final( uims_db_t *db)
{
	if( db->tskey) pthread_key_delete( db->tskey);
	if( db->module) db->module->close_db( db);
	if( db->confstr) ux_free( ux_mem_default(), db->confstr);
	if( db->func) ux_free( ux_mem_default(), db->func);
	if( db->dll) ux_free( ux_mem_default(), db->dll);
	if( db->name) ux_free( ux_mem_default(), db->name);
	ux_refcount_final( db->refcnt);
}

UX_DECLARE(uims_db_t*) uims_db_ref( uims_db_t *db)
{
	return (uims_db_t*)UX_OBJ_PREF(db, refcnt);
}

UX_DECLARE(void) uims_db_unref( uims_db_t *db)
{
	db = UX_OBJ_PUNREF(db, refcnt);
	if(db) uims_db_destroy(db);
}

UX_DECLARE(uint32_t) uims_db_get_id( uims_db_t *db)
{
	return db->id;
}

UX_DECLARE(const char*) uims_db_get_name( uims_db_t *db)
{
	return db->name;
}

UX_DECLARE(const char*) uims_db_get_dll( uims_db_t *db)
{
	return db->dll;
}

UX_DECLARE(const char*) uims_db_get_func( uims_db_t *db)
{
	return db->func;
}

UX_DECLARE(const char*) uims_db_get_confstr( uims_db_t *db)
{
	return db->confstr;
}

UX_DECLARE(uims_dbmodule_t*) uims_db_get_module( uims_db_t *db)
{
	return db->module;
}

UX_DECLARE(void*) uims_db_get_handle( uims_db_t *db)
{
	return db->handle;
}

UX_DECLARE(void) uims_db_set_handle( uims_db_t *db, void *handle)
{
	db->handle = handle;
}

UX_DECLARE(uims_dbc_t*) uims_db_get_conn( uims_db_t *db, int *err)
{
	int rv;
	uims_dbc_t *dbc;

	dbc = (uims_dbc_t*)pthread_getspecific( db->tskey);
	if( dbc == NULL) {
		dbc = uims_dbc_create( db, &rv);
		if( dbc == NULL ) {
			ux_log(UXL_CRT, "Failed to create DB uims_dbc_t. (err=%d,%s)", rv, uims_dberr_to_str(rv));
			if(err) *err = rv;
			return NULL;
		}
		pthread_setspecific( db->tskey, dbc);
	}

	rv = uims_dbc_open( dbc);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to DB connection open (err=%d%s)", rv, uims_dberr_to_str(rv));
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return dbc;
}


UX_DECLARE(uims_dbstmt_t*) uims_db_open_stmt( uims_db_t *db, const char *stmtid,
						const char *query, int *err)
{
	int rv;
	uims_dbc_t *dbc;

	dbc = uims_db_get_conn( db, &rv);
	if( dbc == NULL) {
		if(err) *err = rv;
		return NULL; 
	}

	return uims_dbc_open_stmt( dbc, stmtid, query, err);
}

UX_DECLARE(uims_dbstmt_t*) uims_db_open_extra_stmt( uims_db_t *db, const char *stmtid,
						const char *query, int *err)
{
	int rv;
	uims_dbc_t *dbc;

	dbc = uims_db_get_conn( db, &rv);
	if( dbc == NULL) {
		if(err) *err = rv;
		return NULL; 
	}

	return uims_dbc_open_extra_stmt( dbc, stmtid, query, err);
}

UX_DECLARE(uims_dberr_e) uims_db_close_extra_stmt( uims_db_t *db, uims_dbstmt_t *dbstmt) 
{
	int rv;
	uims_dbc_t *dbc;

	rv = UIMS_DB_SUCCESS;
	dbc = uims_db_get_conn( db, &rv);
	if( dbc == NULL) return rv; 

	return uims_dbc_close_extra_stmt( dbc, dbstmt);
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbmgr_t
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(uims_dbmgr_t*) uims_dbmgr_create( uint32_t maxdb)
{
	int rv;
	uims_dbmgr_t *dbmgr;

	dbmgr = (uims_dbmgr_t*)ux_zalloc( ux_mem_default(), sizeof(uims_dbmgr_t));
	if( !dbmgr) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_dbmgr_t.")
		return NULL;
	}

	rv = uims_dbmgr_init( dbmgr, maxdb);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize memory for uims_dbmgr_t.")
		ux_free( ux_mem_default(), dbmgr);
		return NULL;
	}

	return dbmgr;
}

UX_DECLARE(void) uims_dbmgr_destroy( uims_dbmgr_t *dbmgr)
{
	uims_dbmgr_final( dbmgr);
	ux_free( ux_mem_default(), dbmgr);
}

uims_dberr_e uims_dbmgr_init( uims_dbmgr_t *dbmgr, uint32_t maxdb)
{
	int rv;

	if( maxdb <= 0 || maxdb >= INT_MAX) maxdb = 16;

	dbmgr->nodes = ux_zalloc( ux_mem_default(), sizeof(uims_dbnode_t)*maxdb);
	if( dbmgr->nodes == NULL) {
		ux_log( UXL_MAJ, "Failed to allocate DB array of uims_dbmgr_t. (maxdb=%u)", maxdb);
		return UIMS_DBERR_NO_MEMORY;
	}

	rv = ux_rbtree_init( dbmgr->map, ux_compare_str);
	if( rv < UX_SUCCESS) {
		ux_log( UXL_MAJ, "Failed to DB name map of uims_dbmgr_t. (rv=%d,%s)", 
				rv, ux_errnostr( rv));
		ux_free( ux_mem_default(), dbmgr->nodes);
		return UIMS_DBERR_INVALID_PARAMETER;
	}
	dbmgr->maxdb = maxdb;

	return UIMS_DB_SUCCESS;
}

void uims_dbmgr_final( uims_dbmgr_t *dbmgr)
{
	uims_dbmgr_clear( dbmgr);
	ux_rbtree_final( dbmgr->map);
	ux_free( ux_mem_default(), dbmgr->nodes);
	dbmgr->nodes = NULL;
}

void uims_dbmgr_clear( uims_dbmgr_t *dbmgr)
{
	int i;

	for( i = 0; i < dbmgr->maxdb; ++i) {
		if( dbmgr->nodes[i].db == NULL) continue;
		ux_rbtree_remove_node( dbmgr->map, dbmgr->nodes[i].rbnode);
		uims_db_unref( dbmgr->nodes[i].db);
		dbmgr->nodes[i].db = NULL;
	}
}

UX_DECLARE(uims_dberr_e) uims_dbmgr_load( uims_dbmgr_t *dbmgr, const char *cfile, const char *section)
{
	int i, rv;
	FILE *fp;
	uims_db_t *db;
	char *c, *id, *name, *dll, *func, *confstr;
	char *lstr, buf[8192];

	fp = ux_ini_open_section(cfile, section, &rv);
	if( fp == NULL || rv < UX_SUCCESS ) {
		ux_log( UXL_MAJ, "Failed to open section. (section='%s', file='%s')", section, cfile);
		return UIMS_DBERR_INVALID_CONF;
	}

	rv = UIMS_DB_SUCCESS;
	i = 0;
	while( (lstr = ux_ini_read_next_line( fp, buf, sizeof(buf))) ) {
		//ID; NAME; DLL; FUNC; CONFSTR
		id = lstr;
		c = strchr( id, ';');
		if( c ) { *c='\0'; name = ux_str_ltrim(c+1); }
		else { name = NULL; }
		c = (name) ? strchr( name, ';') : NULL;
		if( c ) { *c='\0'; dll = ux_str_ltrim(c+1); }
		else { dll = NULL; }
		c = (dll) ? strchr( dll, ';') : NULL;
		if( c ) { *c='\0'; func = ux_str_ltrim(c+1); }
		else { func = NULL; }
		c = (func) ? strchr( func, ';') : NULL;
		if( c ) { *c='\0'; confstr = ux_str_ltrim(c+1); }
		else { confstr = NULL; }

		if( id == NULL || id[0] == '\0' || name == NULL || name[0] == '\0'||
			dll == NULL || func == NULL || func[0] == '\0'|| confstr == NULL)
		{
			ux_log(UXL_MIN, "Invalid DB configuration. (idx=%d, head=%s)", i, lstr);
			continue;
		}

		db = uims_db_create( strtoul(id, NULL, 0), name, dll, func, confstr, &rv);
		if( db == NULL) {
			ux_log(UXL_MAJ, "Failed to create DB module. "
					"(idx=%d, id='%s', name='%s', dll='%s', func='%s', confstr='%s', err=%d,%s)",
					i, id, name, dll, func, confstr, rv, uims_dberr_to_str(rv));
			goto final;
		}

		rv = uims_dbmgr_add( dbmgr, db);
		if( rv < UIMS_DB_SUCCESS ) {
			ux_log(UXL_MAJ, "Failed to add DB module. "
					"(idx=%d, id='%s', name='%s', dll='%s', func='%s', confstr='%s', err=%d,%s)",
					i, id, name, dll, func, confstr, rv, uims_dberr_to_str(rv));
			goto final;
		}
	}

final:
	fclose(fp);
	return rv;
}

UX_DECLARE(uims_dberr_e) uims_dbmgr_add( uims_dbmgr_t *dbmgr, uims_db_t *db)
{
	int rv;
	uims_db_t *fdb;
	uims_dbnode_t *dbnode;

	if( db->id >= dbmgr->maxdb) {
		ux_log(UXL_MAJ, "Failed to add DB cause of invalid id. (id=%u, max=%u)", db->id, dbmgr->maxdb);
		return UIMS_DBERR_INVALID_IDX;
	}

	dbnode = &dbmgr->nodes[db->id];
	if( dbnode->db ) {
		ux_log(UXL_MAJ, "Aleady exist DB id. (id=%u)", db->id);
		return UIMS_DBERR_ALREADY_EXISTS;
	}

	fdb = uims_dbmgr_get_n( dbmgr, db->name);
	if( fdb ) {
		ux_log(UXL_MAJ, "Aleady exist DB name. (name=%s)", db->name);
		return UIMS_DBERR_ALREADY_EXISTS;
	}
	rv = ux_rbtree_insert( dbmgr->map, db->name, dbnode->rbnode); 
	if( rv < UX_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to insert DB to map. (name=%s, err=%d,%s)", db->name, rv, ux_errnostr(rv));
		return UIMS_DBERR_ALREADY_EXISTS;
	}

	dbnode->db = uims_db_ref(db);
	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_db_t*) uims_dbmgr_get( uims_dbmgr_t *dbmgr, uint32_t id)
{
	if( id >= dbmgr->maxdb) return NULL; 

	return dbmgr->nodes[id].db;
}

UX_DECLARE(uims_db_t*) uims_dbmgr_get_n( uims_dbmgr_t *dbmgr, const char *name)
{
	uims_dbnode_t *dbnode;

	dbnode = (uims_dbnode_t*)ux_rbtree_find( dbmgr->map, (void*)name);
	if(dbnode == NULL) return NULL;

	return dbnode->db;
}

UX_DECLARE(uims_dberr_e) uims_dbmgr_remove( uims_dbmgr_t *dbmgr, uint32_t id)
{
	uims_dbnode_t *dbnode;

	if( id >= dbmgr->maxdb) return UIMS_DB_SUCCESS; 

	dbnode = &dbmgr->nodes[id];

	if( dbnode->db) {
		ux_rbtree_remove_node( dbmgr->map, dbnode->rbnode);
		uims_db_unref( dbnode->db);
		dbnode->db = NULL;
	}

	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dberr_e) uims_dbmgr_remove_n( uims_dbmgr_t *dbmgr, const char *name)
{
	uims_dbnode_t *dbnode;

	dbnode = (uims_dbnode_t*)ux_rbtree_find( dbmgr->map, (void*)name);
	if( dbnode == NULL) return UIMS_DB_SUCCESS;

	if( dbnode->db) {
		ux_rbtree_remove_node( dbmgr->map, dbnode->rbnode);
		uims_db_unref( dbnode->db);
		dbnode->db = NULL;
	}

	return UIMS_DB_SUCCESS;
}

UX_DECLARE(uims_dbc_t*) uims_dbmgr_get_conn( uims_dbmgr_t *dbmgr, uint32_t id)
{
	int rv;
	uims_db_t *db;

	db = uims_dbmgr_get( dbmgr, id);
	if( db == NULL) return NULL;

	return uims_db_get_conn( db, &rv);
}

UX_DECLARE(uims_dbc_t*) uims_dbmgr_get_conn_n( uims_dbmgr_t *dbmgr, const char* name)
{
	int rv;
	uims_db_t *db;

	db = uims_dbmgr_get_n( dbmgr, name);
	if( db == NULL) return NULL;

	return uims_db_get_conn( db, &rv);
}
