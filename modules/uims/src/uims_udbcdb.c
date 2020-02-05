#include "local/uims_udbcdb_l.h"

uims_dberr_e uims_udbcerr_to_err( udbc_error_t err)
{
	switch( err ) {
		case UDBC_SUCCESS : return UIMS_DB_SUCCESS;
		case UDBC_ERROR : return UIMS_DBERR_ERROR;
		case UDBC_CANNOT_CONNECT : return UIMS_DBERR_CANT_CONNECT;
		case UDBC_DISCONNECTED : return UIMS_DBERR_DISCONNECTED;
		case UDBC_INVALID_PARAMETER : return UIMS_DBERR_INVALID_PARAMETER;
		case UDBC_CANNOT_BIND : return UIMS_DBERR_CANT_BIND;
		case UDBC_ALREADY_EXISTS : return UIMS_DBERR_ALREADY_EXISTS;
		case UDBC_NO_DATA : return UIMS_DBERR_NO_DATA;
		case UDBC_CANNOT_OPEN_DLL : return UIMS_DBERR_CANT_OPEN_DLL;
		case UDBC_NOT_FOUND_FUNC : return UIMS_DBERR_NOT_FOUND_FUNC;
		case UDBC_NO_MEMORY : return UIMS_DBERR_NO_MEMORY;
		case UDBC_INVALID_STATEMENT : return UIMS_DBERR_INVALID_STATEMENT;
		case UDBC_NOT_SUPPORTED : return UIMS_DBERR_NOT_SUPPORTED;
		default : break;
	}
	return UIMS_DBERR_ERROR; 
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_udbcdb_t
////////////////////////////////////////////////////////////////////////////////

uims_udbcdb_t* uims_udbcdb_create( uims_db_t *db, const char *dllname,
						const char *confstr, int *err)
{
	int rv;
	uims_udbcdb_t *udbcdb;

	udbcdb = (uims_udbcdb_t*)ux_zalloc( ux_mem_default(), sizeof(uims_udbcdb_t));
	if( udbcdb == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_udbcdb_t. (dll=%s, confstr=%s)",
				dllname, confstr)
		if(err) *err = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	rv = uims_udbcdb_init( udbcdb, db, dllname, confstr);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize uims_udbcdb_t. (dll=%s, confstr=%s, err=%d,%s)", 
				dllname, confstr, rv, uims_dberr_to_str(rv))
		ux_free( ux_mem_default(), udbcdb);
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return udbcdb;
}

void uims_udbcdb_destroy( uims_udbcdb_t *udbcdb)
{
	uims_udbcdb_final( udbcdb);
	ux_free( ux_mem_default(), udbcdb);
}

uims_dberr_e uims_udbcdb_init( uims_udbcdb_t *udbcdb, uims_db_t *db, const char *dllname,
						const char *confstr)
{
	uims_dberr_e rv;

	ux_log(UXL_ALWAYS, "!!!! UDBCDB_INIT : dllname=%s, confstr=%s", dllname, confstr);
	udbcdb->conf = udbc_conf_create( confstr, strlen(confstr));
	if( udbcdb->conf == NULL ) {
		ux_log(UXL_MAJ, "Failed to udbc_conf_create");
		return UIMS_DBERR_NO_MEMORY;
	}

	rv = udbc_module_open( dllname, &udbcdb->module);
	if( rv != UDBC_SUCCESS ) {
		rv = uims_udbcerr_to_err(rv);
		ux_log(UXL_INFO, "Failed to udbc_module_open. (err=%d,%s)",
				(int)rv, uims_dberr_to_str(rv));
		udbc_conf_destroy( udbcdb->conf);
		return rv;
	}

	udbcdb->db = db;

	return UIMS_DB_SUCCESS;
}

void uims_udbcdb_final( uims_udbcdb_t *udbcdb)
{
	if(udbcdb->conf) udbc_conf_destroy(udbcdb->conf);
	if(udbcdb->module) udbc_module_close(udbcdb->module);
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_dbmodule_t
////////////////////////////////////////////////////////////////////////////////

static uims_dberr_e _uims_udbcmodule_open_db( uims_db_t *db)
{
	int rv, len;
	uims_udbcdb_t *udbcdb;
	const char *confstr;
	char *cstr, dllname[256];

	confstr = uims_db_get_confstr( db);
	cstr = strchr(confstr, ';');
	if( cstr == NULL) {
		ux_log( UXL_MAJ, "Invalid UDBC DB configuration string. (id=%u, name=%s, confstr=%s)", 
				uims_db_get_id(db), uims_db_get_name(db), confstr);
		return UIMS_DBERR_INVALID_CONF;
	}

	len = cstr - confstr;
	cstr++;
	memcpy( dllname, confstr, len);
	dllname[len] = '\0';

	udbcdb = uims_udbcdb_create( db, dllname, cstr, &rv);
	if( udbcdb == NULL) {
		ux_log( UXL_CRT, "Failed to create UDBC DB handle. (id=%u, name=%s, confstr=%s, err=%d,%s)", 
				uims_db_get_id(db), uims_db_get_name(db), confstr, rv, uims_dberr_to_str(rv));
		return rv;
	}

	uims_db_set_handle( db, udbcdb);
	return UIMS_DB_SUCCESS;
}

static void _uims_udbcmodule_close_db( uims_db_t *db)
{
	uims_udbcdb_t *udbcdb;

	udbcdb = (uims_udbcdb_t*)uims_db_get_handle( db);

	if(udbcdb) uims_udbcdb_destroy( udbcdb);
}

static uims_dberr_e _uims_udbcmodule_open_conn( uims_dbc_t *dbc)
{
	uims_dberr_e rv;
	udbc_t *udbc;
	uims_db_t *db;
	uims_udbcdb_t *udbcdb;

	db = (uims_db_t*)uims_dbc_get_db( dbc);
	udbc = (udbc_t*)uims_dbc_get_handle( dbc);
	udbcdb = (uims_udbcdb_t*)uims_db_get_handle( db);

	if( udbc == NULL) {
		udbc = udbc_create( udbcdb->module);
		if( udbc == NULL ) {
			ux_log(UXL_CRT, "Failed to udbc_create. (id=%u, name=%s, confstr=%s)",
					uims_db_get_id(db), uims_db_get_name(db), uims_db_get_confstr(db));
			return UIMS_DBERR_NO_MEMORY;
		}
		uims_dbc_set_handle( dbc, udbc);
	}

	if( !udbc_is_connected( udbc) ) {
		rv = udbc_connect( udbc, udbcdb->conf);
		if( rv != UDBC_SUCCESS ) {
			rv = uims_udbcerr_to_err(rv);
			ux_log(UXL_MAJ, "Failed to udbc_connect. (id=%u, name=%s, confstr=%s, err=%d,%s)",
					uims_db_get_id(db), uims_db_get_name(db), uims_db_get_confstr(db),
					(int)rv, uims_dberr_to_str(rv));
			return rv;
		}
	}

	return UIMS_DB_SUCCESS;
}

static void _uims_udbcmodule_close_conn( uims_dbc_t *dbc)
{
	udbc_t *udbc;

	udbc = (udbc_t*)uims_dbc_get_handle( dbc);

	if(udbc) udbc_destroy( udbc);
}

static uims_dberr_e _uims_udbcmodule_open_stmt( uims_dbstmt_t *dbstmt,
						const char *stmtid, const char *query)
{
	int rv;
	udbc_t *dbc;
	uims_udbcdb_t *udbcdb;
	udbc_stmt_t *stmt;

	dbc = (udbc_t*)uims_dbstmt_get_dbc_handle( dbstmt);

	if( !udbc_is_connected( dbc) ) {
		udbcdb = (uims_udbcdb_t*)uims_dbstmt_get_db_handle( dbstmt);
		rv = udbc_connect( dbc, udbcdb->conf);
		if( rv != UDBC_SUCCESS ) {
			rv = uims_udbcerr_to_err(rv);
			ux_log(UXL_MAJ, "Failed to udbc_connect. (id=%u, name=%s, confstr=%s, err=%d,%s)",
					uims_db_get_id(udbcdb->db), uims_db_get_name(udbcdb->db), uims_db_get_confstr(udbcdb->db),
					(int)rv, uims_dberr_to_str(rv));
			return rv;
		}
	}

	rv = udbc_get_prepared_statement( dbc, stmtid, query, &stmt);
	if( rv < UDBC_SUCCESS) {
		rv = uims_udbcerr_to_err(rv);
		return rv;
	}

	uims_dbstmt_set_handle( dbstmt, stmt);

	return UIMS_DB_SUCCESS;
}

static void _uims_udbcmodule_close_stmt( uims_dbstmt_t *dbstmt)
{
	udbc_stmt_t *stmt;

	stmt = (udbc_stmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( stmt == NULL) return;

	udbc_stmt_free_result( stmt);
	uims_dbstmt_set_handle( dbstmt, NULL);
}

static uims_dberr_e _uims_udbcmodule_execute_stmt( uims_dbstmt_t *dbstmt)
{
	int i, pcnt, inout;
	uims_dberr_e rv;
	udbc_stmt_t *stmt;
	uims_dbdata_t *data;
	uims_dbdataset_t *paraset;

	stmt = (udbc_stmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( stmt == NULL) {
		ux_log(UXL_MAJ, "this statement is not open. (handle is NULL)");
		return UIMS_DBERR_INVALID_STATEMENT;
	}

	paraset = uims_dbstmt_get_paraset( dbstmt);
	pcnt = uims_dbdataset_get_count( paraset);
	for( i = 0; i < pcnt; ++i) {
		data = uims_dbdataset_get_data( paraset, i);
		if(data->flags & (UIMS_DBFLAG_IO_IN|UIMS_DBFLAG_IO_OUT)) {
			if(data->flags & UIMS_DBFLAG_IO_IN) {
				inout = (data->flags & UIMS_DBFLAG_IO_OUT) ? UDBC_IO_INOUT : UDBC_IO_IN;
			} else {
				inout = UDBC_IO_OUT;
			}
		} else {
			inout = UDBC_IO_DEFAULT;
		}
	
		switch( data->type) {
			case UIMS_DBTYPE_INT8 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_CHAR, (char*)&data->v.i8, sizeof(int8_t));
				break;
			case UIMS_DBTYPE_UINT8 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_CHAR, (char*)&data->v.u8, sizeof(uint8_t));
				break;
			case UIMS_DBTYPE_INT16 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_SHORT, (char*)&data->v.i16, sizeof(int16_t));
				break;
			case UIMS_DBTYPE_UINT16 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_SHORT, (char*)&data->v.u16, sizeof(uint16_t));
				break;
			case UIMS_DBTYPE_INT32 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_LONG, (char*)&data->v.i32, sizeof(int32_t));
				break;
			case UIMS_DBTYPE_UINT32 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_LONG, (char*)&data->v.u32, sizeof(uint32_t));
				break;
			case UIMS_DBTYPE_INT64 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_LONGLONG, (char*)&data->v.i64, sizeof(int64_t));
				break;
			case UIMS_DBTYPE_UINT64 : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_LONGLONG, (char*)&data->v.u64, sizeof(uint64_t));
				break;
			case UIMS_DBTYPE_FLOAT : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_FLOAT, (char*)&data->v.f32, sizeof(float));
				break;
			case UIMS_DBTYPE_DOUBLE : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_DOUBLE, (char*)&data->v.d64, sizeof(double));
				break;
			case UIMS_DBTYPE_STR : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_STRING, (char*)data->v.str, data->len + 1);
				break;
			case UIMS_DBTYPE_OCT : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_BLOB, (char*)data->v.oct, data->len);
				break;
			case UIMS_DBTYPE_NIL : 
				rv = udbc_stmt_bind_parameter_ex( stmt, i, inout, UDBC_STRING, "", -1);
				break;
			default:
				rv = UDBC_INVALID_PARAMETER;
				break;
		}
		if( rv < UDBC_SUCCESS) {
			rv = uims_udbcerr_to_err(rv);
			ux_log(UXL_MIN, "Failed to bind parameter. (stmtid=%s, idx=%d, type=%s, err=%d,%s)",
					uims_dbstmt_get_stmtid(dbstmt), i, uims_dbtype_to_str(data->type),
					rv, uims_dberr_to_str( rv));
			return rv;
		}
	}

	rv = udbc_stmt_apply_parameters(stmt);
	if( rv < UDBC_SUCCESS ) {
		rv = uims_udbcerr_to_err(rv);
		ux_log(UXL_MAJ, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
					 uims_dbstmt_get_stmtid(dbstmt), rv, uims_dberr_to_str( rv));
		return rv;
	}

	rv = udbc_stmt_execute( stmt, uims_dbstmt_get_mode(dbstmt));
	if( rv < UDBC_SUCCESS) {
		rv = uims_udbcerr_to_err(rv);
		if( rv != UIMS_DBERR_NO_DATA) {
			ux_log(UXL_MAJ, "Failed to execute statement. (stmtid=%s, err=%d,%s)",
						 uims_dbstmt_get_stmtid(dbstmt), rv, uims_dberr_to_str( rv));
		}
	}

	return rv;
}

static uims_dberr_e _uims_udbcmodule_fetch_stmt( uims_dbstmt_t *dbstmt)
{
	int rv, i, rcnt, type;
	int64_t ival;
	double dval;
	udbc_blob_t *blob;
	udbc_rset_t *rset;
	udbc_stmt_t *stmt;
	uims_dbdataset_t *dbrsltset;

	stmt = (udbc_stmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( stmt == NULL) {
		ux_log(UXL_MAJ, "this statement is not open. (handle is NULL)");
		return UIMS_DBERR_INVALID_STATEMENT;
	}
	
	rset = udbc_stmt_get_rset( stmt);
	if( rset == NULL) {
		ux_log(UXL_MAJ, "Failed to create result set.");
		return UIMS_DBERR_NO_MEMORY;
	}

	rv = udbc_rset_bind( rset);
	if( rv < UDBC_SUCCESS) {
		rv = uims_udbcerr_to_err(rv);
		ux_log(UXL_MAJ, "Failed to bind result set. (stmtid=%s, err=%d,%s)",
					 uims_dbstmt_get_stmtid(dbstmt), rv, uims_dberr_to_str( rv));
		return rv;
	}

	rv = udbc_rset_next( rset);
	if( rv < UDBC_SUCCESS) {
		rv = uims_udbcerr_to_err(rv);
		ux_log(UXL_MAJ, "Failed to fetch statement. (stmtid=%s, err=%d,%s)",
					 uims_dbstmt_get_stmtid(dbstmt), rv, uims_dberr_to_str( rv));
		return rv;
	}

	dbrsltset = uims_dbstmt_get_rsltset( dbstmt);
	uims_dbdataset_reset_count( dbrsltset);
	rcnt = udbc_rset_get_count( rset);

	for( i = 0; i < rcnt; ++i) {
		type = udbc_rset_get_type(rset, i);
		switch( type) {
			case UDBC_CHAR:
				ival = udbc_rset_get_int( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_int8( dbrsltset, i, ival);
				} break;
			case UDBC_SHORT:
				ival = udbc_rset_get_int( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_int16( dbrsltset, i, ival);
				} break;
			case UDBC_LONG:
				ival = udbc_rset_get_int( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_int32( dbrsltset, i, ival);
				} break;
			case UDBC_LONGLONG:
				ival = udbc_rset_get_int( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_int64( dbrsltset, i, ival);
				} break;
			case UDBC_FLOAT:
				dval = udbc_rset_get_float( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_float( dbrsltset, i, dval);
				} break;
			case UDBC_DOUBLE:
				dval = udbc_rset_get_float( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_double( dbrsltset, i, dval);
				} break;
			case UDBC_DATETIME:
			case UDBC_STRING:
				blob = udbc_rset_get_blob( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_strn( dbrsltset, i, blob->data, blob->size);
				} break;
			case UDBC_BLOB:
				blob = udbc_rset_get_blob( rset, i, &rv);
				if( rv < UDBC_SUCCESS) {
					rv = uims_udbcerr_to_err(rv);
				} else {
					rv = uims_dbdataset_set_oct( dbrsltset, i, (uint8_t*)blob->data, blob->size);
				} break;
			default:
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
		}
		if( rv < UIMS_DB_SUCCESS) {
			ux_log(UXL_MIN, "Failed to fetch result. (stmtid=%s, idx=%d, type=%s, err=%d,%s)",
					uims_dbstmt_get_stmtid(dbstmt), i, udbc_type_str(type),
					rv, udbc_error_str( rv));
			return rv;
		}
	}

	return UIMS_DB_SUCCESS;
}

static ssize_t _uims_udbcmodule_get_affected_row_count( uims_dbstmt_t *dbstmt)
{
	udbc_stmt_t *stmt;

	stmt = (udbc_stmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( stmt == NULL) {
		ux_log(UXL_MAJ, "this statement is not open. (handle is NULL)");
		return UIMS_DBERR_INVALID_STATEMENT;
	}

	return (ssize_t)udbc_stmt_get_affected_row_count(stmt);
}

UX_DECLARE(uims_dbmodule_t*) uims_udbcdb_get_module()
{
	static uims_dbmodule_t _g_uims_udbcmodule = {
		_uims_udbcmodule_open_db,
		_uims_udbcmodule_close_db,
		_uims_udbcmodule_open_conn,
		_uims_udbcmodule_close_conn,
		_uims_udbcmodule_open_stmt,
		_uims_udbcmodule_close_stmt,
		_uims_udbcmodule_execute_stmt,
		_uims_udbcmodule_fetch_stmt,
		_uims_udbcmodule_get_affected_row_count
	};

	return &_g_uims_udbcmodule; 
}

