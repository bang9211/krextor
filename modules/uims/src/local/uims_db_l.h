#ifndef __UIMS_DB_L_H__
#define __UIMS_DB_L_H__

#include "../uims/uims_db.h"
#include <uxlib/ux_rbtree.h>
#include <uxlib/ux_string.h>

////////////////////////////////////////////////////////////////////////////////
// UIMS DB internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief UIMS DB data set 구조체 */
struct uims_dbdataset_s {
	uims_dbstmt_t *stmt; /**< 관련 statement */
	uint32_t size; /**< 메모리에 할당된 parameter에 크기 */ 
	uint32_t count; /**< 사용되는 parameter의 개수 */
	uims_dbdata_t *datas; /**< parameter data */ 
};

/** @brief UIMS DB statment 구조체 */
struct uims_dbstmt_s {
	uims_dbstmt_t *next; /**< next statement node */
	uims_dbc_t *dbc; /**< 해당 statement 관련 DB connection */
	const char *stmtid; /**< statement id */
	const char *query; /**< query */
	uint32_t mode; /**< statement mode */
	void *handle; /**< statement handle */
	uims_dbdataset_t paraset[1]; /**< DB parameter set */
	uims_dbdataset_t rsltset[1]; /**< DB result set */
};

/** @brief UIMS DB connection 구조체 */
struct uims_dbc_s {
	uims_db_t *db; /**< owner DB */
	void *handle; /**< DB connection handle */
	uims_dbstmt_t *freestmt; /**< free statement node */
	uims_dbstmt_t stmt[1]; /**< default statement */
};

/** @brief UIMS DB 구조체 */
struct uims_db_s {
	ux_refcount_t refcnt[1]; /**< 참조 계수 */
	uint32_t id; /**< DB id */
	char *name; /**< DB 이름 */
	char *dll; /**< DLL 이름 */
	char *func; /**< DB module 획득 함수 이름 */
	char *confstr; /**< DB 연결을 위한 configuration 문자열 */
	uims_dbmodule_t *module; /**< 해당 DB에서 사용할 module */
	void *handle; /**< 실제 DB handle */
	pthread_key_t tskey; /**< thread specific key */
};

/** @brief UIMS DB 검색 node 타입 */
typedef struct uims_dbnode_s uims_dbnode_t; 
/** @brief UIMS DB 검색 node 구조체 */
struct uims_dbnode_s {
	ux_rbnode_t rbnode[1]; /**< rbtree name 검색 node */
	uims_db_t *db; /**< 관련 DB */
};

/** @brief UIMS DB manager 구조체 */
struct uims_dbmgr_s {
	uint32_t maxdb; /**< 최대 등록 가능한 DB 정보, default 16 */
	uims_dbnode_t *nodes; /**< DB node list */
	ux_rbtree_t map[1]; /**< DB 검색 map */
};

// uims_db_t *db = uims_dbmgr_get_db("pcscf-tntdb");
// //Tarnatool Qyery : CMD space_id input output limit offset iterator 
// uims_dbstmt_t *stmt = uims_db_open_stmt( db, "sess_insert" "INSERT(0, 5, 0);");
//
// uims_db_t *db = uims_dbmgr_get_db("pcscf-dbms");
// uims_dbstmt_t *stmt = uims_db_open_stmt( db, "sess_insert" "INSERT INTO PCALLSESS VALUES(?,?,?,?,?);"); 
//
// uims_dbdataset_t* pset = uims_stmt_get_paraset( stmt);
// uims_dbdataset_set_int( pset, 0, sess_id); 
// uims_dbdataset_set_int( pset, 1, expire_time); 
// uims_dbdataset_set_int( pset, 2, values); 
// rv = uims_stmt_execute( stmt);
// usim_dbrsltset_t* rset = uims_stmt_get_rsltset( stmt);
// int i0 = uims_dbrsltset_get_int( rsltset, 0, &rv); 
// const char* s0 = uims_dbrsltset_get_str( rsltset, 0, &len); 
//
// rv = uims_stmt_close( stmt);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_dbdataset_t
////////////////////////////////////////////////////////////////////////////////
uims_dberr_e uims_dbdataset_init( uims_dbdataset_t *dbdataset, uims_dbstmt_t *dbstmt,
						uint32_t size);
void uims_dbdataset_final( uims_dbdataset_t *dbdataset);
uims_dberr_e uims_dbdataset_resize( uims_dbdataset_t *dbdataset, uint32_t size);


////////////////////////////////////////////////////////////////////////////////
// local functions for uims_dbstmt_t
////////////////////////////////////////////////////////////////////////////////
uims_dbstmt_t* uims_dbstmt_create( uims_dbc_t *dbc, int *err);
void uims_dbstmt_destroy( uims_dbstmt_t *dbstmt);
uims_dberr_e uims_dbstmt_init( uims_dbstmt_t *dbstmt, uims_dbc_t *dbc);
void uims_dbstmt_final( uims_dbstmt_t *dbstmt);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_dbc_t
////////////////////////////////////////////////////////////////////////////////
uims_dbc_t* uims_dbc_create( uims_db_t *db, int *err);
void uims_dbc_destroy( uims_dbc_t *dbc);
uims_dberr_e uims_dbc_init( uims_dbc_t *dbc, uims_db_t *db);
void uims_dbc_final( uims_dbc_t *dbc);
uims_dberr_e uims_dbc_open( uims_dbc_t *dbc);
void uims_dbc_close( uims_dbc_t *dbc);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_db_t
////////////////////////////////////////////////////////////////////////////////
uims_dberr_e uims_db_init( uims_db_t *db, uint32_t id, const char *name, const char *dll,
						const char *func, const char *confstr);
void uims_db_final( uims_db_t *db);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_dbmgr_t
////////////////////////////////////////////////////////////////////////////////
uims_dberr_e uims_dbmgr_init( uims_dbmgr_t *dbmgr, uint32_t maxdb);
void uims_dbmgr_final( uims_dbmgr_t *dbmgr);
void uims_dbmgr_clear( uims_dbmgr_t *dbmgr);

#endif // #ifndef __UIMS_DB_L_H__
