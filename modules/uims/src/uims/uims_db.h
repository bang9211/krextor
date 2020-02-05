#ifndef __UIMS_DB_H__
#define __UIMS_DB_H__

#include <uxlib/ux_def.h>
#include <uxlib/ux_rbtree.h>

/**
 * @ingroup uims_db
 * @file
 * @brief UIMS DB module
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// macro & types for UIMS DB 
////////////////////////////////////////////////////////////////////////////////

#define UIMS_DBFLAG_IO_IN  0x01
#define UIMS_DBFLAG_IO_OUT 0x02

/**@brief UIMS DB ERROR Type */
typedef enum uims_dberr_e {
	UIMS_DB_SUCCESS              = 0,   /**< SUCCESS */
	UIMS_DBERR_ERROR             = -1,  /**< Normal Error */
	UIMS_DBERR_CANT_CONNECT      = -2,  /**< Can't connect Error */
	UIMS_DBERR_DISCONNECTED      = -3,  /**< Disconnnected Error */
	UIMS_DBERR_CANT_BIND         = -4,  /**< Can't bind Error */
	UIMS_DBERR_ALREADY_EXISTS    = -5,  /**< Already exists Error  */
	UIMS_DBERR_NO_DATA           = -6,  /**< No data Error */
	UIMS_DBERR_INVALID_PARAMETER = -7,  /**< Invalid parameter Error */
	UIMS_DBERR_NO_MEMORY         = -8,  /**< No Memory Error */
	UIMS_DBERR_CANT_OPEN_DLL     = -9,  /**< Can't Open DLL Error */
	UIMS_DBERR_NOT_FOUND_FUNC    = -10, /**< Not Found Function Error */
	UIMS_DBERR_INVALID_STATEMENT = -11, /**< Invalid Statement Error */
	UIMS_DBERR_NOT_SUPPORTED     = -12, /**< Not Supported Error */
	UIMS_DBERR_INVALID_IDX       = -20, /**< Ivalid index */
	UIMS_DBERR_INVALID_TYPE      = -21, /**< Ivalid data type */
	UIMS_DBERR_INVALID_VALUE     = -22, /**< Ivalid data value */
	UIMS_DBERR_INVALID_LEN       = -23, /**< Ivalid data length */
	UIMS_DBERR_INVALID_SIZE      = -24, /**< Ivalid data size */
	UIMS_DBERR_INVALID_CONF      = -25, /**< Ivalid configuration data */
	UIMS_DBERR_INVALID_ADDR      = -26, /**< Ivalid address */
	UIMS_DBERR_OUT_OF_RANGE      = -27, /**< Out of range */
	UIMS_DBERR_TOO_BIG           = -28, /**< Too big size */
	UIMS_DBERR_SYSTEM            = -30, /**< System error */
	UIMS_DBERR_TIMEOUT           = -31, /**< Timeout */
	UIMS_DBERR_CANT_ACCESS       = -32  /**< Can't access */
} uims_dberr_e;

/** @brief UIMS DB type */
typedef enum uims_dbtype_e {
	UIMS_DBTYPE_UNKNOWN = 0, /**< UNKNOWN type */
	UIMS_DBTYPE_NIL, /**< NIL 타입 */
	UIMS_DBTYPE_INT8, /**< int8_t 타입 */
	UIMS_DBTYPE_UINT8, /**< uint8_t 타입 */
	UIMS_DBTYPE_INT16, /**< int16_t 타입 */
	UIMS_DBTYPE_UINT16, /**< uint16_t 타입 */
	UIMS_DBTYPE_INT32, /**< int32_t 타입 */
	UIMS_DBTYPE_UINT32, /**< uint32_t 타입 */
	UIMS_DBTYPE_INT64, /**< int64_t 타입 */
	UIMS_DBTYPE_UINT64, /**< uint64_t 타입 */
	UIMS_DBTYPE_FLOAT, /**< 32bit float 타입 */
	UIMS_DBTYPE_DOUBLE, /**< 64bit double 타입 */
	UIMS_DBTYPE_STR, /**< string 타입 */
	UIMS_DBTYPE_OCT, /**< octet 타입 */
	UIMS_DBTYPE_MAX /**< MAX 값 */
} uims_dbtype_e;


/** @brief UIMS DB data 타입 */
typedef struct uims_dbdata_s uims_dbdata_t; 
/** @brief UIMS DB data 구조체 */
struct uims_dbdata_s {
	uint32_t type:8; /**< DB data type - uims_dbtype_e */
	uint32_t len:24; /**< data length */
	uint32_t flags:8; /**< data flags */
	uint32_t size:24; /**< data buffer size */
	union {
		int8_t i8; /**< int8_t 타입 value */
		uint8_t u8; /**< uint8_t 타입 value */
		int16_t i16; /**< int16_t 타입 value */
		uint16_t u16; /**< uint16_t 타입 value */
		int32_t i32; /**< int32_t 타입 value */
		uint32_t u32; /**< uint32_t 타입 value */
		int64_t i64; /**< int64_t 타입 value */
		uint64_t u64; /**< int8_t 타입 value */
		float f32; /**< float 타입 value */
		double d64; /**< double 타입 value */
		char *str; /**< string 타입 value */
		uint8_t *oct; /**< octet 타입 value */
	} v; /**< data value */
};

/** UIMS DB data set 타입 */
typedef struct uims_dbdataset_s uims_dbdataset_t; 
/** UIMS DB statement 타입 */
typedef struct uims_dbstmt_s uims_dbstmt_t;
/** @brief UIMS DB connection 타입 */
typedef struct uims_dbc_s uims_dbc_t;
/** @brief UIMS DB 타입 */
typedef struct uims_db_s uims_db_t;
/** @brief UIMS DB manager 타입 */
typedef struct uims_dbmgr_s uims_dbmgr_t; 
/** @brief UIMD DB module interface 타입 */
typedef struct uims_dbmodule_s uims_dbmodule_t; 
/** @brief UIMD DB module interface 타입 */
struct uims_dbmodule_s {
	uims_dberr_e (*open_db)( uims_db_t *db);
	void (*close_db)( uims_db_t *db);

	uims_dberr_e (*open_conn)( uims_dbc_t *dbc);
	void (*close_conn)( uims_dbc_t *dbc);

	uims_dberr_e (*open_stmt)( uims_dbstmt_t *dbstmt, const char *stmtid,
						const char *query);
	void (*close_stmt)( uims_dbstmt_t *dbstmt);
	uims_dberr_e (*execute_stmt)( uims_dbstmt_t *dbstmt);
	uims_dberr_e (*fetch_stmt)( uims_dbstmt_t *dbstmt);
	ssize_t (*get_affected_row_count)( uims_dbstmt_t *dbstmt);
};

typedef uims_dbmodule_t* (*uims_db_get_module_f)();

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dberr_e
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_dberr_to_str(uims_dberr_e err);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbtype_e
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_dbtype_to_str( uims_dbtype_e type);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbdataset_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_dberr_e) uims_dbdataset_next( uims_dbdataset_t *dbdataset);
UX_DECLARE(uint32_t) uims_dbdataset_get_count( uims_dbdataset_t *dbdataset);
UX_DECLARE(void) uims_dbdataset_reset_count( uims_dbdataset_t *dbdataset);
UX_DECLARE(uims_dbdata_t*) uims_dbdataset_get_data( uims_dbdataset_t *dbdataset,
						uint32_t idx);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_data( uims_dbdataset_t *dbdataset,
						uint32_t idx, uims_dbdata_t *dbdata);
UX_DECLARE(uims_dbtype_e) uims_dbdataset_get_type( uims_dbdataset_t *dbdataset,
						uint32_t idx);
UX_DECLARE(uint32_t) uims_dbdataset_get_len( uims_dbdataset_t *dbdataset,
						uint32_t idx);
UX_DECLARE(uint8_t) uims_dbdataset_get_flags( uims_dbdataset_t *dbdataset,
						uint32_t idx);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_flags( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint8_t flags);
UX_DECLARE(uint32_t) uims_dbdataset_get_size( uims_dbdataset_t *dbdataset,
						uint32_t idx);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_size( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint32_t size);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_value( uims_dbdataset_t *dbdataset,
						uint32_t idx, uims_dbtype_e type, void *vptr, int len);
UX_DECLARE(int64_t) uims_dbdataset_get_int( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *err);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int( uims_dbdataset_t *dbdataset,
						uint32_t idx, int64_t val);
UX_DECLARE(uint64_t) uims_dbdataset_get_uint( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *err);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint64_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int8( uims_dbdataset_t *dbdataset,
						uint32_t idx, int8_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint8( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint8_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int16( uims_dbdataset_t *dbdataset,
						uint32_t idx, int16_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint16( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint16_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int32( uims_dbdataset_t *dbdataset,
						uint32_t idx, int32_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint32( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint32_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int64( uims_dbdataset_t *dbdataset,
						uint32_t idx, int64_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint64( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint64_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_int( uims_dbdataset_t *dbdataset,
						uint32_t idx, int64_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_uint( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint64_t val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_float( uims_dbdataset_t *dbdataset,
						uint32_t idx, float val);
UX_DECLARE(double) uims_dbdataset_get_double( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *err);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_double( uims_dbdataset_t *dbdataset,
						uint32_t idx, double val);
UX_DECLARE(const char*) uims_dbdataset_get_str( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *len);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_str( uims_dbdataset_t *dbdataset,
						uint32_t idx, const char *val);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_strn( uims_dbdataset_t *dbdataset,
						uint32_t idx, const char *val, int len);
UX_DECLARE(uint8_t*) uims_dbdataset_get_oct( uims_dbdataset_t *dbdataset,
						uint32_t idx, int *len);
UX_DECLARE(uims_dberr_e) uims_dbdataset_set_oct( uims_dbdataset_t *dbdataset,
						uint32_t idx, uint8_t *val, int len);

UX_DECLARE(uims_dberr_e) uims_dbdataset_writev( uims_dbdataset_t *dbdataset,
						int count, va_list args);
UX_DECLARE(uims_dberr_e) uims_dbdataset_write( uims_dbdataset_t *dbdataset,
						int count, ...);
UX_DECLARE(int) uims_dbdataset_readv( uims_dbdataset_t *dbdataset, int count,
						va_list args);
UX_DECLARE(int) uims_dbdataset_read( uims_dbdataset_t *dbdataset, int count, ...);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbstmt_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(const char*) uims_dbstmt_get_stmtid( uims_dbstmt_t *dbstmt);
UX_DECLARE(const char*) uims_dbstmt_get_query( uims_dbstmt_t *dbstmt);
UX_DECLARE(uint32_t) uims_dbstmt_get_mode( uims_dbstmt_t *dbstmt);
UX_DECLARE(void) uims_dbstmt_set_mode( uims_dbstmt_t *dbstmt, uint32_t mode);
UX_DECLARE(uims_dbc_t*) uims_dbstmt_get_dbc( uims_dbstmt_t *dbstmt);
UX_DECLARE(void*) uims_dbstmt_get_dbc_handle( uims_dbstmt_t *dbstmt);
UX_DECLARE(uims_db_t*) uims_dbstmt_get_db( uims_dbstmt_t *dbstmt);
UX_DECLARE(void*) uims_dbstmt_get_db_handle( uims_dbstmt_t *dbstmt);
UX_DECLARE(void*) uims_dbstmt_get_handle( uims_dbstmt_t *dbstmt);
UX_DECLARE(void) uims_dbstmt_set_handle( uims_dbstmt_t *dbstmt, void *handle);
UX_DECLARE(uims_dbdataset_t*) uims_dbstmt_get_paraset( uims_dbstmt_t *dbstmt);
UX_DECLARE(uims_dbdataset_t*) uims_dbstmt_get_rsltset( uims_dbstmt_t *dbstmt);
UX_DECLARE(uims_dberr_e) uims_dbstmt_execute( uims_dbstmt_t *dbstmt);
UX_DECLARE(uims_dberr_e) uims_dbstmt_fetch( uims_dbstmt_t *dbstmt);
UX_DECLARE(ssize_t) uims_dbstmt_affected_row_count( uims_dbstmt_t *dbstmt);
UX_DECLARE(void) uims_dbstmt_close( uims_dbstmt_t *dbstmt);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbc_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_db_t*) uims_dbc_get_db( uims_dbc_t *dbc);
UX_DECLARE(void*) uims_dbc_get_db_handle( uims_dbc_t *dbc);
UX_DECLARE(void*) uims_dbc_get_handle( uims_dbc_t *dbc);
UX_DECLARE(void) uims_dbc_set_handle( uims_dbc_t *dbc, void *handle);
UX_DECLARE(uims_dbstmt_t*) uims_dbc_open_stmt( uims_dbc_t *dbc, const char *stmtid,
							const char *query, int *err);
UX_DECLARE(uims_dbstmt_t*) uims_dbc_open_extra_stmt( uims_dbc_t *dbc, const char *stmtid,
							const char *query, int *err);
UX_DECLARE(uims_dberr_e) uims_dbc_close_extra_stmt( uims_dbc_t *dbc, uims_dbstmt_t *dbstmt);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_db_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_db_t*) uims_db_create( uint32_t id, const char *name, const char *dll,
						const char *func, const char *confstr, int *err);
UX_DECLARE(void) uims_db_destroy( uims_db_t *db);
UX_DECLARE(uims_db_t*) uims_db_ref( uims_db_t *db);
UX_DECLARE(void) uims_db_unref( uims_db_t *db);
UX_DECLARE(uint32_t) uims_db_get_id( uims_db_t *db);
UX_DECLARE(const char*) uims_db_get_name( uims_db_t *db);
UX_DECLARE(const char*) uims_db_get_dll( uims_db_t *db);
UX_DECLARE(const char*) uims_db_get_func( uims_db_t *db);
UX_DECLARE(const char*) uims_db_get_confstr( uims_db_t *db);
UX_DECLARE(uims_dbmodule_t*) uims_db_get_module( uims_db_t *db);
UX_DECLARE(void*) uims_db_get_handle( uims_db_t *db);
UX_DECLARE(void) uims_db_set_handle( uims_db_t *db, void *handle);
UX_DECLARE(uims_dbc_t*) uims_db_get_conn( uims_db_t *db, int *err);
UX_DECLARE(uims_dbstmt_t*) uims_db_open_stmt( uims_db_t *db, const char *stmtid,
						const char *query, int *err);
UX_DECLARE(uims_dbstmt_t*) uims_db_open_extra_stmt( uims_db_t *db, const char *stmtid,
						const char *query, int *err);
UX_DECLARE(uims_dberr_e) uims_db_close_extra_stmt( uims_db_t *db, uims_dbstmt_t *dbstmt);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbmgr_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_dbmgr_t*) uims_dbmgr_create( uint32_t maxdb);
UX_DECLARE(void) uims_dbmgr_destroy( uims_dbmgr_t *dbmgr);
UX_DECLARE(uims_dberr_e) uims_dbmgr_load( uims_dbmgr_t *dbmgr, const char *cfile, const char *section);
UX_DECLARE(uims_dberr_e) uims_dbmgr_add( uims_dbmgr_t *dbmgr, uims_db_t *db);
UX_DECLARE(uims_db_t*) uims_dbmgr_get( uims_dbmgr_t *dbmgr, uint32_t id);
UX_DECLARE(uims_db_t*) uims_dbmgr_get_n( uims_dbmgr_t *dbmgr, const char *name);
UX_DECLARE(uims_dberr_e) uims_dbmgr_remove( uims_dbmgr_t *dbmgr, uint32_t id);
UX_DECLARE(uims_dberr_e) uims_dbmgr_remove_n( uims_dbmgr_t *dbmgr, const char *name);
UX_DECLARE(uims_dbc_t*) uims_dbmgr_get_conn( uims_dbmgr_t *dbmgr, uint32_t id);
UX_DECLARE(uims_dbc_t*) uims_dbmgr_get_conn_n( uims_dbmgr_t *dbmgr, const char* name);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __UIMS_DB_H__

