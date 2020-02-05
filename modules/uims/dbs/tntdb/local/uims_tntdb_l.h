#ifndef __UIMS_TNTDB_L_H__
#define __UIMS_TNTDB_L_H__

#include <uxlib/ux_memory.h>
#include <uxlib/ux_log.h>
#include <uxlib/ux_list.h>
#include <uxlib/ux_vector.h>
#include <tarantool/tarantool.h>
#include <tarantool/tnt_net.h>
#include <tarantool/tnt_opt.h>
#include <tarantool/tnt_io.h>
#include <tarantool/tnt_io.h>
#include <uims/uims_db.h>
#include "uims_tntlex_l.h"

////////////////////////////////////////////////////////////////////////////////
// UIMS TARANTOOL DB internal types & definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum uims_tntvtype_e {
	UIMS_TNTVTYPE_UNKNOWN = 0,
	UIMS_TNTVTYPE_PARAM,
	UIMS_TNTVTYPE_INT,
	UIMS_TNTVTYPE_DOUBLE,
	UIMS_TNTVTYPE_STR,
	UIMS_TNTVTYPE_OCT,
	UIMS_TNTVTYPE_MAX
} uims_tntvtype_e;

/** @brief UIMS tarantool DB 타입 */
typedef struct uims_tntdb_s uims_tntdb_t; 
/** @brief tarantool DB connection 타입 */
typedef struct uims_tntdbc_s uims_tntdbc_t;
/** @brief tarantool statement 타입 */
typedef struct uims_tntstmt_s uims_tntstmt_t;

/** @brief tarantool query token 타입 */
typedef struct uims_tnttkn_s uims_tnttkn_t; 
/** @brief tarantool query token 타입 */
struct uims_tnttkn_s {
	ux_lnode_t lnode[1]; /**< list node */
	int kind; /**< token kind */
	const char *text; /**< token text */
};

/** @brief tarantool internal value 타입 */
typedef struct uims_tntval_s uims_tntval_t; 
/** @brief tarantool internal value 구조체 */
struct uims_tntval_s {
	uint32_t fldno:24; /**< field number */
	uint32_t op:8; /**< operation */
	uint32_t type:8; /**< value type */
	uint32_t len:24; /**< value length */
	union {
		int64_t i64; /**< integer value */
		double d64; /**< double value */
		char *str; /**< string value */
		uint8_t *oct; /**< octet value */
	} v; /**< value */
};

/** @brief tarantool query parameter 타입 */
typedef struct uims_tntpara_s uims_tntpara_t;
/** @brief tarantool query parameter 구조체 */
struct uims_tntpara_s {
    struct tnt_stream stream[1]; /**< parameter stream */
    struct tnt_stream_buf buf[1]; /**< parameter stream buffer */
    int size; /**< 내부 버퍼의 크기 */
    char *start; /**< 내부 버퍼 시작 포인터 */
    char *curr; /**< 현재까지 쓰여진 데이터의 마지막 포인터 */
    char *end; /**< 내부 버퍼의 마지막 포인터 */
    const char *buffer; /**< 초기에 설정된 외부 버퍼 */
};

/** @brief tarantool result set 타입 */
typedef struct uims_tntrset_s uims_tntrset_t;
/** @brief tarantool result set 구조체 */
struct uims_tntrset_s {
	uims_tntrset_t *next; /**< next result set */
	struct tnt_reply reply[1]; /**< tarantoo reply */
	uint32_t ntuples; /**< 성공 시 반환된 tuple의 개수 */
	int bufsize; /**< 초기 설정 외부 버퍼 size */
	const char *cdata; /**< current step data */
	char *buffer; /**< 초기 설정 외부 버퍼 */
};

/** @brief UIMS tarantool DB 구조체 */
struct uims_tntdb_s {
	uims_db_t *db; /**< 관련 DB module */
	char *buffer; /** configuration buffer */
	char *uris[2]; /**< connection uris */
	uint32_t reconnect_period; /**< DB connection 연결 실패시 다음까지 기다릴 시간 */ 
	uint32_t error_wait_msec; /**< DB error pending시 기다릴 에러를 얻기 위해 기다릴 시간 */ 
	uint32_t check_recv_size; /**< 수신 될 수 있는 최대 데이터 크기 */ 
	uint32_t max_rset_buf_size; /**< result set에 할당될 수 있는 최대 buffer 크기 */ 
};

/** @brief tarantool sql parser 타입 */
typedef struct uims_tntsql_s uims_tntsql_t; 
/** @brief tarantool sql parser 구조체 */
struct uims_tntsql_s {
	ux_mem_t *tknmem; /**< token memory allocator */
	ux_mem_t *valmem; /**< value memory allocator */
	uint32_t ntokens; /**< token count */
	ux_list_t tokens[1]; /**< token list */
	ux_vector_t keys[1]; /**< temporary keys */
	ux_vector_t ops[1]; /**< temporary keys */
	uims_tntlex_t lexer[1]; /**< lexer */
	uims_tntstmt_t *stmt; /**< current parsing statement */
};

/** @brief tarantool DB connection 구조체  */
struct uims_tntdbc_s {
	uims_tntdb_t *tntdb; /**< 관련 tarantool DB */
	struct tnt_stream tnts[2]; /**< tarantool connection streams */
	uint32_t reconn_time[2]; /**< connection 재연결 시도 실패 시간 */
	uint32_t curidx; /** 현재 사용되고 있는 connection index */ 
	uims_tntrset_t *freerset; /**< free result sets */
	ux_rbtree_t stmts[1]; /**< statements */
	uims_tntsql_t tntsql[1]; /**< tarantool sql parser */
};

/** @brief tarantool statement 구조체 */
struct uims_tntstmt_s {
	ux_rbnode_t rbnode[1]; /**< statment 검색 rbnode */
	uims_tntdbc_t *tntdbc; /**< tarantool db connection */
	uims_tntrset_t *tntrset; /**< result set */
	char *id; /**< statment id */
	char *query; /**< query string */
	uint8_t command; /**< tarantool command */
	uint8_t iter; /**< iterator value  */
	uint32_t space; /**< tarantool space id */
	uint32_t idxid; /**< tarantool index id */
	uint16_t nkeys; /**< main keys count  */
	uint16_t nops; /**< operation count  */
	char *proc; /**< procedure name */
	uims_tntval_t** keys; /**< query values */
	uims_tntval_t** ops; /**< query operations */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tnttkn_t
////////////////////////////////////////////////////////////////////////////////
uims_tnttkn_t* uims_tnttkn_create( ux_mem_t *allocator, int kind, const char *text);
void uims_tnttkn_destroy( uims_tnttkn_t *tnttkn, ux_mem_t *allocator);
char* uims_tnttkn_get_str( uims_tnttkn_t *tnttkn, ux_mem_t *allocator, int *len);
uint8_t* uims_tnttkn_get_oct( uims_tnttkn_t *tnttkn, ux_mem_t *allocator, int *len);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntval_t
////////////////////////////////////////////////////////////////////////////////
uims_tntval_t* uims_tntval_create( ux_mem_t *allocator, uint32_t fldno, uint8_t op,
						uims_tnttkn_t *token, int *err);
void uims_tntval_destroy( uims_tntval_t *tntval, ux_mem_t *allocator);
uims_dberr_e uims_tntval_init( uims_tntval_t *tntval, uint32_t fldno, uint8_t op,
						uims_tnttkn_t *token);
void uims_tntval_final( uims_tntval_t *tntval);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntpara_t
////////////////////////////////////////////////////////////////////////////////
uims_dberr_e uims_tntpara_init( uims_tntpara_t *para, char *buffer, int bufsize);
void uims_tntpara_final( uims_tntpara_t *para);
struct tnt_stream* uims_tntpara_get_stream( uims_tntpara_t *para);
uims_dberr_e uims_tntpara_realloc( uims_tntpara_t *para, uint32_t need);
uims_dberr_e uims_tntpara_add_nil( uims_tntpara_t *para);
uims_dberr_e uims_tntpara_add_int( uims_tntpara_t *para, int64_t value);
uims_dberr_e uims_tntpara_add_uint( uims_tntpara_t *para, uint64_t value);
uims_dberr_e uims_tntpara_add_bool( uims_tntpara_t *para, char value);
uims_dberr_e uims_tntpara_add_float( uims_tntpara_t *para, float value);
uims_dberr_e uims_tntpara_add_double( uims_tntpara_t *para, double value);
uims_dberr_e uims_tntpara_add_str( uims_tntpara_t *para, const char *value, int len);
uims_dberr_e uims_tntpara_add_strz( uims_tntpara_t *para, const char *value);
uims_dberr_e uims_tntpara_add_bin( uims_tntpara_t *para, const void *value, int len);
uims_dberr_e uims_tntpara_add_array( uims_tntpara_t *para, uint32_t size);
uims_dberr_e uims_tntpara_add_map( uims_tntpara_t *para, uint32_t size);
uims_dberr_e uims_tntpara_add_dbdata( uims_tntpara_t *para, uims_dbdata_t *dbdata);
uims_dberr_e uims_tntpara_load_keys( uims_tntpara_t *para, uims_tntval_t **values,
						int nvalues, uims_dbdataset_t *paraset, int *psidx);
uims_dberr_e uims_tntpara_load_ops( uims_tntpara_t *para, uims_tntval_t **values,
						int nvalues, uims_dbdataset_t *paraset, int *psidx);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntrset_t
////////////////////////////////////////////////////////////////////////////////
uims_tntrset_t* uims_tntrset_create( uint32_t bufsize, int *err);
void uims_tntrset_destroy( uims_tntrset_t *tntrset);
uims_dberr_e uims_tntrset_init( uims_tntrset_t *rset, uint32_t bufsize);
void uims_tntrset_final( uims_tntrset_t *rset);
uims_dberr_e uims_tntrset_resize( uims_tntrset_t *tntrset, uint32_t bufsize);
void uims_tntrset_apply( uims_tntrset_t *rset);
void uims_tntrset_reset( uims_tntrset_t *rset);
enum tnt_errcode_t uims_tntrset_get_rcode( uims_tntrset_t *rset);
const char* uims_tntrset_get_errstr( uims_tntrset_t *rset);
uint32_t uims_tntrset_read_array( uims_tntrset_t *rset, int *err);
uint32_t uims_tntrset_read_map( uims_tntrset_t *rset, int *err);
uims_dberr_e uims_tntrset_write( uims_tntrset_t *rset, uims_dbdataset_t *rsltset);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntdb_t
////////////////////////////////////////////////////////////////////////////////
uims_tntdb_t* uims_tntdb_create( uims_db_t *db, const char *confstr, int *err);
void uims_tntdb_destroy( uims_tntdb_t *tntdb);
uims_dberr_e uims_tntdb_init( uims_tntdb_t *tntdb, uims_db_t *db, const char *confstr);
void uims_tntdb_final( uims_tntdb_t *tntdb);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntsql_t
////////////////////////////////////////////////////////////////////////////////
uims_dberr_e uims_tntsql_init( uims_tntsql_t *tntsql);
void uims_tntsql_final( uims_tntsql_t *tntsql);
void uims_tntsql_clear( uims_tntsql_t *tntsql);
uims_dberr_e uims_tntsql_init_parsing( uims_tntsql_t *tntsql, uims_tntstmt_t *stmt);
void uims_tntsql_final_parsing( uims_tntsql_t *tntsql);
uims_dberr_e uims_tntsql_add_token( uims_tntsql_t *tntsql, int kind, const char *text);
uims_tnttkn_t* uims_tntsql_get_next_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur,
						int prerr);
uims_tnttkn_t* uims_tntsql_get_expect_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur,
						int kind);
uims_tnttkn_t* uims_tntsql_get_space_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_tnttkn_t* uims_tntsql_get_index_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_tnttkn_t* uims_tntsql_get_iter_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_tnttkn_t* uims_tntsql_get_op_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur,
						uint8_t *op);
uims_tnttkn_t* uims_tntsql_get_key_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_tnttkn_t* uims_tntsql_get_opval_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_tnttkn_t* uims_tntsql_get_opint_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_check_eos( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse_select( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse_insert( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse_delete( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse_update( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse_upsert( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse_call( uims_tntsql_t *tntsql, uims_tnttkn_t *cur);
uims_dberr_e uims_tntsql_parse( uims_tntsql_t *tntsql, uims_tntstmt_t *stmt);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntdbc_t
////////////////////////////////////////////////////////////////////////////////
uims_tntdbc_t* uims_tntdbc_create( uims_tntdb_t *tntdb, int *err);
void uims_tntdbc_destroy( uims_tntdbc_t *tntdbc);
uims_dberr_e uims_tntdbc_init( uims_tntdbc_t *tntdbc, uims_tntdb_t *tntdb);
void uims_tntdbc_final( uims_tntdbc_t *tntdbc);
uims_dberr_e uims_tntdbc_load( uims_tntdbc_t *tntdbc);
int uims_tntdbc_get_error( uims_tntdbc_t *tntdbc, struct tnt_stream_net *net);
uims_dberr_e uims_tntdbc_check_error( uims_tntdbc_t *tntdbc, struct tnt_stream *tnt,
						const char *cmdstr, int rslt);

struct tnt_stream* uims_tntdbc_get_tnt( uims_tntdbc_t *tntdbc, int *rr);
uims_tntrset_t* uims_tntdbc_alloc_rset( uims_tntdbc_t *tntdbc, int *err);
void uims_tntdbc_free_rset( uims_tntdbc_t *tntdbc, uims_tntrset_t *tntrset);
uims_tntstmt_t* uims_tntdbc_get_stmt( uims_tntdbc_t *tntdbc, const char *stmtid,
						const char *query, int *err);
int uims_tntdbc_read_reply(uims_tntdbc_t *tntdbc, struct tnt_stream *tnt,
						uims_tntrset_t *rset);
uims_dberr_e uims_tntdbc_execute( uims_tntdbc_t *tntdbc, struct tnt_stream *tnt,
						uims_tntrset_t *rset);

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntstmt_t
////////////////////////////////////////////////////////////////////////////////
uims_tntstmt_t* uims_tntstmt_create( uims_tntdbc_t *tntdbc, const char *stmtid,
						const char *query, int *err);
void uims_tntstmt_destroy( uims_tntstmt_t *tntstmt);
uims_dberr_e uims_tntstmt_init( uims_tntstmt_t *tntstmt, uims_tntdbc_t *tntdbc,
						const char *stmtid, const char *query);
void uims_tntstmt_final( uims_tntstmt_t *tntstmt);
uims_dberr_e uims_tntstmt_select( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt,
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara);
uims_dberr_e uims_tntstmt_insert( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt,
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara);
uims_dberr_e uims_tntstmt_replace( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara);
uims_dberr_e uims_tntstmt_delete( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara);
uims_dberr_e uims_tntstmt_update( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara,
						uims_tntpara_t *tntops);
uims_dberr_e uims_tntstmt_upsert( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara,
						uims_tntpara_t *tntops);
uims_dberr_e uims_tntstmt_call( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara);
uims_dberr_e uims_tntstmt_eval( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara);
uims_dberr_e uims_tntstmt_execute( uims_tntstmt_t *tntstmt, uint32_t mode,
						uims_dbdataset_t *paraset);
uims_dberr_e uims_tntstmt_fetch( uims_tntstmt_t *tntstmt, uims_dbdataset_t *rsltset);
ssize_t uims_tntstmt_get_affected_row_count( uims_tntstmt_t *tntstmt);
uims_dberr_e uims_tntstmt_free_result( uims_tntstmt_t *tntstmt);


#endif
