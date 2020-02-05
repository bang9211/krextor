#include "local/uims_tntdb_l.h"
#include <uxlib/ux_string.h>
#include <uxlib/ux_util.h>
#include <uxlib/ux_socket.h>
#include <uxlib/ux_time.h>
#include <stdlib.h>
#include "local/msgpuck.h"
#include "local/pmatomic.h"

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntmptype_e
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal message pack type의 값을 문자열로 변환해 반환한다.
 * @param type message pack type
 * @return 해당 message pack type의 문자열
 */
const char* uims_tntmptype_to_str( int type)
{
	switch( type) {
		case MP_NIL : return "NIL";
		case MP_UINT : return "UINT";
		case MP_INT : return "INT";
		case MP_STR : return "STR";
		case MP_BIN : return "BIN";
		case MP_ARRAY : return "ARRAY";
		case MP_MAP : return "MAP";
		case MP_BOOL : return "BOOL";
		case MP_FLOAT : return "FLOAT";
		case MP_DOUBLE : return "DOUBLE";
		case MP_EXT : return "EXT";
		default : break;
	}
	return "UNKNOWN";
}

////////////////////////////////////////////////////////////////////////////////
// functions for tnt_err_t
////////////////////////////////////////////////////////////////////////////////

const char* uims_tntrcode_to_str( int rcode)
{
	switch( rcode) {
		case TNT_ER_UNKNOWN : return "TNT_ER_UNKNOWN"; 
		case TNT_ER_ILLEGAL_PARAMS : return "TNT_ER_ILLEGAL_PARAMS";
		case TNT_ER_MEMORY_ISSUE : return "TNT_ER_MEMORY_ISSUE";
		case TNT_ER_TUPLE_FOUND : return "TNT_ER_TUPLE_FOUND";
		case TNT_ER_TUPLE_NOT_FOUND : return "TNT_ER_TUPLE_NOT_FOUND";
		case TNT_ER_UNSUPPORTED : return "TNT_ER_UNSUPPORTED";
		case TNT_ER_NONMASTER : return "TNT_ER_NONMASTER";
		case TNT_ER_READONLY : return "TNT_ER_READONLY";
		case TNT_ER_INJECTION : return "TNT_ER_INJECTION";
		case TNT_ER_CREATE_SPACE : return "TNT_ER_CREATE_SPACE";
		case TNT_ER_SPACE_EXISTS : return "TNT_ER_SPACE_EXISTS";
		case TNT_ER_DROP_SPACE : return "TNT_ER_DROP_SPACE";
		case TNT_ER_ALTER_SPACE : return "TNT_ER_ALTER_SPACE";
		case TNT_ER_INDEX_TYPE : return "TNT_ER_INDEX_TYPE";
		case TNT_ER_MODIFY_INDEX : return "TNT_ER_MODIFY_INDEX";
		case TNT_ER_LAST_DROP : return "TNT_ER_LAST_DROP";
		case TNT_ER_TUPLE_FORMAT_LIMIT : return "TNT_ER_TUPLE_FORMAT_LIMIT";
		case TNT_ER_DROP_PRIMARY_KEY : return "TNT_ER_DROP_PRIMARY_KEY";
		case TNT_ER_KEY_PART_TYPE : return "TNT_ER_KEY_PART_TYPE";
		case TNT_ER_EXACT_MATCH : return "TNT_ER_EXACT_MATCH";
		case TNT_ER_INVALID_MSGPACK : return "TNT_ER_INVALID_MSGPACK";
		case TNT_ER_PROC_RET : return "TNT_ER_PROC_RET";
		case TNT_ER_TUPLE_NOT_ARRAY : return "TNT_ER_TUPLE_NOT_ARRAY";
		case TNT_ER_FIELD_TYPE : return "TNT_ER_FIELD_TYPE";
		case TNT_ER_FIELD_TYPE_MISMATCH : return "TNT_ER_FIELD_TYPE_MISMATCH";
		case TNT_ER_SPLICE : return "TNT_ER_SPLICE";
		case TNT_ER_ARG_TYPE : return "TNT_ER_ARG_TYPE";
		case TNT_ER_TUPLE_IS_TOO_LONG : return "TNT_ER_TUPLE_IS_TOO_LONG";
		case TNT_ER_UNKNOWN_UPDATE_OP : return "TNT_ER_UNKNOWN_UPDATE_OP";
		case TNT_ER_UPDATE_FIELD : return "TNT_ER_UPDATE_FIELD";
		case TNT_ER_FIBER_STACK : return "TNT_ER_FIBER_STACK";
		case TNT_ER_KEY_PART_COUNT : return "TNT_ER_KEY_PART_COUNT";
		case TNT_ER_PROC_LUA : return "TNT_ER_PROC_LUA";
		case TNT_ER_NO_SUCH_PROC : return "TNT_ER_NO_SUCH_PROC";
		case TNT_ER_NO_SUCH_TRIGGER : return "TNT_ER_NO_SUCH_TRIGGER";
		case TNT_ER_NO_SUCH_INDEX : return "TNT_ER_NO_SUCH_INDEX";
		case TNT_ER_NO_SUCH_SPACE : return "TNT_ER_NO_SUCH_SPACE";
		case TNT_ER_NO_SUCH_FIELD : return "TNT_ER_NO_SUCH_FIELD";
		case TNT_ER_SPACE_FIELD_COUNT : return "TNT_ER_SPACE_FIELD_COUNT";
		case TNT_ER_INDEX_FIELD_COUNT : return "TNT_ER_INDEX_FIELD_COUNT";
		case TNT_ER_WAL_IO : return "TNT_ER_WAL_IO";
		case TNT_ER_MORE_THAN_ONE_TUPLE : return "TNT_ER_MORE_THAN_ONE_TUPLE";
		case TNT_ER_ACCESS_DENIED : return "TNT_ER_ACCESS_DENIED";
		case TNT_ER_CREATE_USER : return "TNT_ER_CREATE_USER";
		case TNT_ER_DROP_USER : return "TNT_ER_DROP_USER";
		case TNT_ER_NO_SUCH_USER : return "TNT_ER_NO_SUCH_USER";
		case TNT_ER_USER_EXISTS : return "TNT_ER_USER_EXISTS";
		case TNT_ER_PASSWORD_MISMATCH : return "TNT_ER_PASSWORD_MISMATCH";
		case TNT_ER_UNKNOWN_REQUEST_TYPE : return "TNT_ER_UNKNOWN_REQUEST_TYPE";
		case TNT_ER_UNKNOWN_SCHEMA_OBJECT : return "TNT_ER_UNKNOWN_SCHEMA_OBJECT";
		case TNT_ER_CREATE_FUNCTION : return "TNT_ER_CREATE_FUNCTION";
		case TNT_ER_NO_SUCH_FUNCTION : return "TNT_ER_NO_SUCH_FUNCTION";
		case TNT_ER_FUNCTION_EXISTS : return "TNT_ER_FUNCTION_EXISTS";
		case TNT_ER_FUNCTION_ACCESS_DENIED : return "TNT_ER_FUNCTION_ACCESS_DENIED";
		case TNT_ER_FUNCTION_MAX : return "TNT_ER_FUNCTION_MAX";
		case TNT_ER_SPACE_ACCESS_DENIED : return "TNT_ER_SPACE_ACCESS_DENIED";
		case TNT_ER_USER_MAX : return "TNT_ER_USER_MAX";
		case TNT_ER_NO_SUCH_ENGINE : return "TNT_ER_NO_SUCH_ENGINE";
		case TNT_ER_RELOAD_CFG : return "TNT_ER_RELOAD_CFG";
		case TNT_ER_CFG : return "TNT_ER_CFG";
		case TNT_ER_SOPHIA : return "TNT_ER_SOPHIA";
		case TNT_ER_LOCAL_SERVER_IS_NOT_ACTIVE : return "TNT_ER_LOCAL_SERVER_IS_NOT_ACTIVE";
		case TNT_ER_UNKNOWN_SERVER : return "TNT_ER_UNKNOWN_SERVER";
		case TNT_ER_CLUSTER_ID_MISMATCH : return "TNT_ER_CLUSTER_ID_MISMATCH";
		case TNT_ER_INVALID_UUID : return "TNT_ER_INVALID_UUID";
		case TNT_ER_CLUSTER_ID_IS_RO : return "TNT_ER_CLUSTER_ID_IS_RO";
		case TNT_ER_SERVER_ID_IS_RESERVED : return "TNT_ER_SERVER_ID_IS_RESERVED";
		case TNT_ER_INVALID_ORDER : return "TNT_ER_INVALID_ORDER";
		case TNT_ER_MISSING_REQUEST_FIELD : return "TNT_ER_MISSING_REQUEST_FIELD";
		case TNT_ER_IDENTIFIER : return "TNT_ER_IDENTIFIER";
		case TNT_ER_DROP_FUNCTION : return "TNT_ER_DROP_FUNCTION";
		case TNT_ER_ITERATOR_TYPE : return "TNT_ER_ITERATOR_TYPE";
		case TNT_ER_REPLICA_MAX : return "TNT_ER_REPLICA_MAX";
		case TNT_ER_INVALID_XLOG : return "TNT_ER_INVALID_XLOG";
		case TNT_ER_INVALID_XLOG_NAME : return "TNT_ER_INVALID_XLOG_NAME";
		case TNT_ER_INVALID_XLOG_ORDER : return "TNT_ER_INVALID_XLOG_ORDER";
		case TNT_ER_NO_CONNECTION : return "TNT_ER_NO_CONNECTION";
		case TNT_ER_TIMEOUT : return "TNT_ER_TIMEOUT";
		case TNT_ER_ACTIVE_TRANSACTION : return "TNT_ER_ACTIVE_TRANSACTION";
		case TNT_ER_NO_ACTIVE_TRANSACTION : return "TNT_ER_NO_ACTIVE_TRANSACTION";
		case TNT_ER_CROSS_ENGINE_TRANSACTION : return "TNT_ER_CROSS_ENGINE_TRANSACTION";
		case TNT_ER_NO_SUCH_ROLE : return "TNT_ER_NO_SUCH_ROLE";
		case TNT_ER_ROLE_EXISTS : return "TNT_ER_ROLE_EXISTS";
		case TNT_ER_CREATE_ROLE : return "TNT_ER_CREATE_ROLE";
		case TNT_ER_INDEX_EXISTS : return "TNT_ER_INDEX_EXISTS";
		case TNT_ER_TUPLE_REF_OVERFLOW : return "TNT_ER_TUPLE_REF_OVERFLOW";
		case TNT_ER_ROLE_LOOP : return "TNT_ER_ROLE_LOOP";
		case TNT_ER_GRANT : return "TNT_ER_GRANT";
		case TNT_ER_PRIV_GRANTED : return "TNT_ER_PRIV_GRANTED";
		case TNT_ER_ROLE_GRANTED : return "TNT_ER_ROLE_GRANTED";
		case TNT_ER_PRIV_NOT_GRANTED : return "TNT_ER_PRIV_NOT_GRANTED";
		case TNT_ER_ROLE_NOT_GRANTED : return "TNT_ER_ROLE_NOT_GRANTED";
		case TNT_ER_MISSING_SNAPSHOT : return "TNT_ER_MISSING_SNAPSHOT";
		case TNT_ER_CANT_UPDATE_PRIMARY_KEY : return "TNT_ER_CANT_UPDATE_PRIMARY_KEY";
		case TNT_ER_UPDATE_INTEGER_OVERFLOW : return "TNT_ER_UPDATE_INTEGER_OVERFLOW";
		case TNT_ER_GUEST_USER_PASSWORD : return "TNT_ER_GUEST_USER_PASSWORD";
		case TNT_ER_TRANSACTION_CONFLICT : return "TNT_ER_TRANSACTION_CONFLICT";
		case TNT_ER_UNSUPPORTED_ROLE_PRIV : return "TNT_ER_UNSUPPORTED_ROLE_PRIV";
		case TNT_ER_LOAD_FUNCTION : return "TNT_ER_LOAD_FUNCTION";
		case TNT_ER_FUNCTION_LANGUAGE : return "TNT_ER_FUNCTION_LANGUAGE";
		case TNT_ER_RTREE_RECT : return "TNT_ER_RTREE_RECT";
		case TNT_ER_PROC_C : return "TNT_ER_PROC_C";
		case TNT_ER_UNKNOWN_RTREE_INDEX_DISTANCE_TYPE : return "TNT_ER_UNKNOWN_RTREE_INDEX_DISTANCE_TYPE";
		case TNT_ER_PROTOCOL : return "TNT_ER_PROTOCOL";
		case TNT_ER_UPSERT_UNIQUE_SECONDARY_KEY : return "TNT_ER_UPSERT_UNIQUE_SECONDARY_KEY";
		case TNT_ER_WRONG_INDEX_RECORD : return "TNT_ER_WRONG_INDEX_RECORD";
		case TNT_ER_WRONG_INDEX_PARTS : return "TNT_ER_WRONG_INDEX_PARTS";
		case TNT_ER_WRONG_INDEX_OPTIONS : return "TNT_ER_WRONG_INDEX_OPTIONS";
		case TNT_ER_WRONG_SCHEMA_VERSION : return "TNT_ER_WRONG_SCHEMA_VERSION";
		case TNT_ER_SLAB_ALLOC_MAX : return "TNT_ER_SLAB_ALLOC_MAX";
		default : break;
	}
return "UNKNOWN";
};



////////////////////////////////////////////////////////////////////////////////
// functions for uims_tnttkn_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal SQL문 lexer token을 생성한다.
 * @param allocator memory allocator
 * @param kind token 종류
 * @param text token text
 * @return 생성된 token 값
 */
uims_tnttkn_t* uims_tnttkn_create( ux_mem_t *allocator, int kind, const char *text)
{
	uims_tnttkn_t *tnttkn;

	tnttkn = (uims_tnttkn_t*)ux_zalloc( allocator, sizeof(uims_tnttkn_t));
	if( tnttkn == NULL) {
		ux_log( UXL_CRT, "Failed to allocate memory for uims_tnttkn_t.");
		return NULL;
	}

	tnttkn->kind = kind;
	tnttkn->text = text;

	return tnttkn;
}

/**
 * @internal SQL문 lexer token을 소멸한다. 
 * @param tnttkn 소멸할 token
 * @param allocator memory allocator
 */
void uims_tnttkn_destroy( uims_tnttkn_t *tnttkn, ux_mem_t *allocator)
{
	ux_free( allocator, tnttkn);
}

/**
 * @internal 전달된 token이 STRING token일 경우 해당 값을 문자열로 변환해 반환한다.
 * @param tnttkn token
 * @param allocator 문자열을 생성하기 위해 사용할 memory allocator
 * @param len 반환된 문자열의 크기 또는 에러 코드
 * @return 생성된 문자열. 실패 시 NULL 반환
 */
char* uims_tnttkn_get_str( uims_tnttkn_t *tnttkn, ux_mem_t *allocator, int *len)
{
	int i, slen, idx;
	const char *tk;
	char *buffer;

	tk = tnttkn->text;
	if(tk[0] != '\'') {
		if(*len) *len = UIMS_DBERR_INVALID_VALUE;
		return NULL;
	}
	slen = strlen(tk);
	if(tk[slen-1] != '\'') {
		if(*len) *len = UIMS_DBERR_INVALID_VALUE;
		return NULL;
	}

	buffer = ux_zalloc( allocator, slen+1);
	if( buffer == NULL) {
		if(*len) *len = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	idx = 0;
	tk++;
	slen -= 2;
	for( i=0; i < slen; i++) {
		if( tk[i]=='\'') {
			if( tk[i+1] !='\'') break; 
			buffer[idx++]='\'';
		} else {
			buffer[idx++]=tk[i];
		}
	}
	buffer[idx] = '\0';

	if(len) *len = idx;
	return buffer;
}

/**
 * @internal 전달된 token이 OCTET token일 경우 해당 값을 문자열로 변환해 반환한다.
 * @param tnttkn token
 * @param allocator OCTET 값을 생성하기 위해 사용할 memory allocator
 * @param len 반환된 OCTET 버퍼의 크기 또는 에러 코드
 * @return 생성된 OCTET 값. 실패 시 NULL 반환
 */
uint8_t* uims_tnttkn_get_oct( uims_tnttkn_t *tnttkn, ux_mem_t *allocator, int *len)
{
	int i, slen, olen, idx;
	const char *tk;
	uint8_t v, *buffer;

	tk = tnttkn->text;
	if(tk[0] != '0') {
		if(*len) *len = UIMS_DBERR_INVALID_VALUE;
		return NULL;
	}
	if(tk[1] != 'x' && tk[1] != 'X') {
		if(*len) *len = UIMS_DBERR_INVALID_VALUE;
		return NULL;
	}
	if(tk[2] != '\'') {
		if(*len) *len = UIMS_DBERR_INVALID_VALUE;
		return NULL;
	}
	slen = strlen(tk);
	if(tk[slen-1] != '\'') {
		if(*len) *len = UIMS_DBERR_INVALID_VALUE;
		return NULL;
	}

	slen -= 4;
	tk += 3;
	olen = (slen+1)/2;
	buffer = ux_zalloc( allocator, olen);
	if( buffer == NULL) {
		if(*len) *len = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	idx = 0;
	if( slen % 2 != 0) {
		if(tk[0] >= '0' && tk[0] <= '9') v = (tk[0]-'0');
		else if(tk[0] >= 'a' && tk[0] <= 'f') v = (tk[0]-'a' + 10);
		else if(tk[0] >= 'A' && tk[0] <= 'F') v = (tk[0]-'A' + 10);
		else {
			ux_free( allocator, buffer);
			if(*len) *len = UIMS_DBERR_INVALID_VALUE;
			return NULL;
		}
		buffer[idx++] = v;
		tk++;
		slen--;
	}
	for( i=0; i < slen; i++) {
		if(tk[i] >= '0' && tk[i] <= '9') v = (tk[i]-'0')*16;
		else if(tk[i] >= 'a' && tk[i] <= 'f') v = (tk[i]-'a' + 10)*16;
		else if(tk[i] >= 'A' && tk[i] <= 'F') v = (tk[i]-'A' + 10)*16;
		else {
			ux_free( allocator, buffer);
			if(*len) *len = UIMS_DBERR_INVALID_VALUE;
			return NULL;
		}
		i++;
		if(tk[i] >= '0' && tk[i] <= '9') v = (tk[i]-'0');
		else if(tk[i] >= 'a' && tk[i] <= 'f') v = (tk[i]-'a' + 10);
		else if(tk[i] >= 'A' && tk[i] <= 'F') v = (tk[i]-'A' + 10);
		else {
			ux_free( allocator, buffer);
			if(*len) *len = UIMS_DBERR_INVALID_VALUE;
			return NULL;
		}
		buffer[idx++] = v;
	}

	if(len) *len = idx;
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntval_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool statement 내부 value를 생성한다.
 * @param allocator 사용할 memory allocator
 * @param fldno field number
 * @param op operation code
 * @param token 생성시 사용할 token
 * @param err 실행 결과를 담을 포인터
 * @return 생성된 value
 */
uims_tntval_t* uims_tntval_create( ux_mem_t *allocator, uint32_t fldno, uint8_t op,
						uims_tnttkn_t *token, int *err)
{
	int rv;
	uims_tntval_t *tntval;

	tntval = (uims_tntval_t*)ux_zalloc( allocator, sizeof(uims_tntval_t));
	if( tntval == NULL) {
		ux_log( UXL_CRT, "Failed to allocate memory for uims_tntval_t.");
		if(err) *err = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	rv = uims_tntval_init( tntval, fldno, op, token);
	if( rv < UIMS_DB_SUCCESS) {
		ux_free( allocator, tntval);
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return tntval;
}

/**
 * @internal tarantool 내부 value를 소멸한다. 
 * @param tntval 소멸할 value
 * @param allocator 사용할 memory allocator
 */
void uims_tntval_destroy( uims_tntval_t *tntval, ux_mem_t *allocator)
{
	uims_tntval_final( tntval);
	ux_free( allocator, tntval);
}


/**
 * @internal tarantool statement 내부 value를 초기화한다. 
 * @param tntval 초기화할 value
 * @param fldno field number
 * @param op operation code
 * @param token 생성시 사용할 token
 * @return 실행 결과
 */
uims_dberr_e uims_tntval_init( uims_tntval_t *tntval, uint32_t fldno, uint8_t op,
						uims_tnttkn_t *token)
{
	int rv;

	tntval->fldno = fldno;
	tntval->op = op;
	switch(token->kind) {
		case '?' :
			tntval->type = UIMS_TNTVTYPE_PARAM;
			tntval->len = 0;
			tntval->v.str = NULL;
			break;
		case UIMS_TNTTK_NUMBER :
			tntval->type = UIMS_TNTVTYPE_INT;
			tntval->len = sizeof(int64_t); 
			tntval->v.i64 = strtoll(token->text, NULL, 0);
			break;
		case UIMS_TNTTK_FLOAT :
			tntval->type = UIMS_TNTVTYPE_INT;
			tntval->len = sizeof(int64_t); 
			tntval->v.d64 = strtod(token->text, NULL);
			break;
		case UIMS_TNTTK_STRING :
			tntval->type = UIMS_TNTVTYPE_STR;
			tntval->v.str = uims_tnttkn_get_str(token, ux_mem_default(), &rv);
			if( tntval->v.str == NULL) {
				ux_log(UXL_MAJ, "Failed to get string value. (token=%d:%s, err=%d,%s)",
						token->kind, token->text, rv, uims_dberr_to_str(rv));
				return rv;
			}
			tntval->len = rv;
			break;
		case UIMS_TNTTK_OCTSTR :
			tntval->type = UIMS_TNTVTYPE_OCT;
			tntval->v.oct = uims_tnttkn_get_oct(token, ux_mem_default(), &rv);
			if( tntval->v.oct == NULL) {
				ux_log(UXL_MAJ, "Failed to get octet value. (token=%d:%s, err=%d,%s)",
						token->kind, token->text, rv, uims_dberr_to_str(rv));
				return rv;
			}
			tntval->len = rv;
			break;
		default :
			ux_log(UXL_MAJ, "This token is not value type. (token=%d:%s)", token->kind, token->text);
			return UIMS_DBERR_INVALID_TYPE;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool 내부 value의 사용을 종료한다.
 * @param tntval value
 */
void uims_tntval_final( uims_tntval_t *tntval)
{
	switch( tntval->type) {
		case UIMS_TNTVTYPE_STR:
			if( tntval->v.str) {
				ux_free(ux_mem_default(), tntval->v.str);
				tntval->v.str = NULL;
			} break;
		case UIMS_TNTVTYPE_OCT:
			if( tntval->v.oct) {
				ux_free(ux_mem_default(), tntval->v.oct);
				tntval->v.oct = NULL;
			} break;
		default : break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntpara_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool parameter 초기화한다.
 * @param para tarantool parameter
 * @param buffer 초기에 사용할 외부 buffer
 * @param bufsize 초기에 사용할 외부 buffer의 크기
 * @return 실행 결과
 */
uims_dberr_e uims_tntpara_init( uims_tntpara_t *para, char *buffer, int bufsize)
{
	para->buffer = buffer;
	para->size = bufsize ? bufsize : 1024;
	para->start = para->curr = (buffer) ? buffer : ux_alloc( ux_mem_default(), para->size);
	para->end = para->start + para->size;
	if(para->start == NULL) return UIMS_DBERR_NO_MEMORY;
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter의 사용을 종료한다.
 * @param para tarantool parameter
 */
void uims_tntpara_final( uims_tntpara_t *para)
{
	if(para->start && para->buffer != para->start) {
		ux_free( ux_mem_default(), para->start);
	}
}

/**
 * @internal tarantool에 전달할 parameter stream을 반환한다.
 * @param para tarantool parameter
 * @return tarantool parameter stream
 */
struct tnt_stream* uims_tntpara_get_stream( uims_tntpara_t *para)
{
	para->stream->data = para->buf;
	para->buf->data = para->start;
	para->buf->size = para->curr - para->start;
	return para->stream;
}

/**
 * @internal tarantool parameter 내부 buffer를 재할당한다.
 * @param para tarantool parameter
 * @param need 요구되는 추가 buffer 크기
 * @return 실행 결과
 */
uims_dberr_e uims_tntpara_realloc( uims_tntpara_t *para, uint32_t need)
{
	int idx;
	char *newbuf;

	para->size += (need > 1024) ? need :  1024;
	newbuf = ux_alloc( ux_mem_default(), para->size);
	if( newbuf == NULL) return UIMS_DBERR_NO_MEMORY;

	idx = para->curr - para->start;
	memcpy( newbuf, para->start, idx);

	if(para->start && para->buffer != para->start) {
		ux_free( ux_mem_default(), para->start);
	}

	para->start = newbuf;
	para->curr = para->start + idx;
	para->end = para->start + para->size;
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 NIL 값을 추가한다.
 * @param para tarantool parameter
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_nil( uims_tntpara_t *para)
{
	int rv;

	if(para->curr + 2 > para->end) {
		rv = uims_tntpara_realloc( para, 2);
		if( rv < 0) return rv;
	}
	para->curr = mp_encode_nil(para->curr);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 integer 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 integer 값
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_int( uims_tntpara_t *para, int64_t value)
{
	int rv;

	if(para->curr + 10 > para->end) {
		rv = uims_tntpara_realloc( para, 10);
		if( rv < 0) return rv;
	}
	if( value >= 0) para->curr = mp_encode_uint(para->curr, value);
	else para->curr = mp_encode_int(para->curr, value);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 unsigned integer 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 unsigned integer 값
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_uint( uims_tntpara_t *para, uint64_t value)
{
	int rv;

	if(para->curr + 10 > para->end) {
		rv = uims_tntpara_realloc( para, 10);
		if( rv < 0) return rv;
	}
	para->curr = mp_encode_uint(para->curr, value);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 boolean 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 boolean 값
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_bool( uims_tntpara_t *para, char value)
{
	int rv;

	if(para->curr + 2 > para->end) {
		rv = uims_tntpara_realloc( para, 2);
		if( rv < 0) return rv;
	}
	para->curr = mp_encode_bool(para->curr, value);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 float 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 float 값
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_float( uims_tntpara_t *para, float value)
{
	int rv;

	if(para->curr + 6 > para->end) {
		rv = uims_tntpara_realloc( para, 6);
		if( rv < 0) return rv;
	}
	para->curr = mp_encode_float( para->curr, value);
	return 0;
}

/**
 * @internal tarantool parameter에 double 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 double 값
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_double( uims_tntpara_t *para, double value)
{
	int rv;

	if(para->curr + 10 > para->end) {
		rv = uims_tntpara_realloc( para, 10);
		if( rv < 0) return rv;
	}
	para->curr = mp_encode_double( para->curr, value);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 string 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 string 값
 * @param len string의 길이
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_str( uims_tntpara_t *para, const char *value, int len)
{
	int rv;

	if(para->curr + 6 + len > para->end) {
		rv = uims_tntpara_realloc( para, 6 + len);
		if( rv < 0) return rv;
	}
	if( value == NULL) {
		para->curr = mp_encode_nil( para->curr);
	} else {
		para->curr = mp_encode_str( para->curr, value, len);
	}
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 string 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 string 값 ('0'로 끝나는 string)
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_strz( uims_tntpara_t *para, const char *value)
{
	return uims_tntpara_add_str(para, value, strlen(value));
}

/**
 * @internal tarantool parameter에 binary 값을 추가한다.
 * @param para tarantool parameter
 * @param value 추가할 binary 값
 * @param len binary 값의 길이
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_bin( uims_tntpara_t *para, const void *value, int len)
{
	int rv;

	if(para->curr + 6 + len > para->end) {
		rv = uims_tntpara_realloc( para, 6 + len);
		if( rv < 0) return rv;
	}

	if( value == NULL) {
		para->curr = mp_encode_nil( para->curr);
	} else {
		para->curr = mp_encode_bin(para->curr, value, len);
	}
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 array 값을 추가한다.
 * @param para tarantool parameter
 * @param size array size - 하위에 들어갈 값들의 개수
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_array( uims_tntpara_t *para, uint32_t size)
{
	int rv;

	if(para->curr + 6 > para->end) {
		rv = uims_tntpara_realloc( para, 6);
		if( rv < 0) return rv;
	}

	para->curr = mp_encode_array(para->curr, size);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 map 값을 추가한다.
 * @param para tarantool parameter
 * @param size map size - 하위에 들어갈 값들의 개수
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_map( uims_tntpara_t *para, uint32_t size)
{
	int rv;

	if(para->curr + 6 > para->end) {
		rv = uims_tntpara_realloc( para, 6);
		if( rv < 0) return rv;
	}

	para->curr = mp_encode_map(para->curr, size);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool parameter에 전달된 DB data 값을 추가한다. 
 * @param para tarantool parameter
 * @param dabdata 추가할 DB data 값 
 * @reutrn 실행 결과
 */
uims_dberr_e uims_tntpara_add_dbdata( uims_tntpara_t *para, uims_dbdata_t *dbdata)
{
	int rv;

	switch(dbdata->type) {
		case UIMS_DBTYPE_NIL :
			rv = uims_tntpara_add_nil( para);
			break;
		case UIMS_DBTYPE_INT8 :
			rv = uims_tntpara_add_int( para, dbdata->v.i8);
			break;
		case UIMS_DBTYPE_UINT8 :
			rv = uims_tntpara_add_uint( para, dbdata->v.u8);
			break;
		case UIMS_DBTYPE_INT16 :
			rv = uims_tntpara_add_int( para, dbdata->v.i16);
			break;
		case UIMS_DBTYPE_UINT16 :
			rv = uims_tntpara_add_uint( para, dbdata->v.u16);
			break;
		case UIMS_DBTYPE_INT32 :
			rv = uims_tntpara_add_int( para, dbdata->v.i32);
			break;
		case UIMS_DBTYPE_UINT32 :
			rv = uims_tntpara_add_uint( para, dbdata->v.u32);
			break;
		case UIMS_DBTYPE_INT64 :
			rv = uims_tntpara_add_int( para, dbdata->v.i64);
			break;
		case UIMS_DBTYPE_UINT64 :
			rv = uims_tntpara_add_uint( para, dbdata->v.u64);
			break;
		case UIMS_DBTYPE_FLOAT :
			rv = uims_tntpara_add_float( para, dbdata->v.f32);
			break;
		case UIMS_DBTYPE_DOUBLE :
			rv = uims_tntpara_add_double( para, dbdata->v.d64);
			break;
		case UIMS_DBTYPE_STR :
			rv = uims_tntpara_add_str( para, dbdata->v.str, dbdata->len);
			break;
		case UIMS_DBTYPE_OCT :
			rv = uims_tntpara_add_bin( para, dbdata->v.oct, dbdata->len);
			break;
		default :
			rv = UIMS_DBERR_INVALID_TYPE;
			ux_log(UXL_MAJ, "Not suppoted data type.");
			break;
	}

	return rv;
}

/**
 * @internal 전달된 value 값과 parameter set을 이용해 key또는 tuple로 사용될 parameter를 로드한다.
 * @param para tarantool parameter
 * @param values 내부 value list
 * @param nvalues 내부 value list의 개수
 * @param paraset parameter set
 * @parma psidx parameter 위치를 담은 포인터
 * @return 실행 결과
 */
uims_dberr_e uims_tntpara_load_keys( uims_tntpara_t *para, uims_tntval_t **values,
						int nvalues, uims_dbdataset_t *paraset, int *psidx)
{
	int rv, idx;
	uims_dbdata_t *dbdata;
	uims_tntval_t *tntval;

	rv = uims_tntpara_add_array( para, nvalues);
	if( rv < UIMS_DB_SUCCESS) return rv;

	for( idx = 0; idx < nvalues; ++idx) {
		tntval = values[idx];
		switch( tntval->type) {
			case UIMS_TNTVTYPE_PARAM:
				dbdata = uims_dbdataset_get_data( paraset, *psidx);
				if( dbdata == NULL) {
					rv = UIMS_DBERR_INVALID_IDX;
					ux_log(UXL_MAJ, "Failed to set parameter cause of no data. (idx=%d, param=%d)",
								idx, *psidx);
				} else {
					rv = uims_tntpara_add_dbdata( para, dbdata);
				}
				*psidx += 1;
				break;
			case UIMS_TNTVTYPE_INT:
				rv = uims_tntpara_add_int( para, tntval->v.i64);
				break;
			case UIMS_TNTVTYPE_DOUBLE:
				rv = uims_tntpara_add_double( para, tntval->v.d64);
				break;
			case UIMS_TNTVTYPE_STR:
				rv = uims_tntpara_add_str( para, tntval->v.str, tntval->len);
				break;
			case UIMS_TNTVTYPE_OCT:
				rv = uims_tntpara_add_bin( para, tntval->v.oct, tntval->len);
				break;
			default :
				ux_log(UXL_MAJ, "Failed to set parameter cause of invalid type. (idx=%d, param=%d, type=%d)",
								idx, *psidx, tntval->type);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
		}

		if( rv < UIMS_DB_SUCCESS) return rv; 
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal 전달된 value 값과 parameter set을 이용해 operatio value 사용될 parameter를 로드한다.
 * @param para tarantool parameter
 * @param values 내부 value list
 * @param nvalues 내부 value list의 개수
 * @param paraset parameter set
 * @parma psidx parameter 위치를 담은 포인터
 * @return 실행 결과
 */
uims_dberr_e uims_tntpara_load_ops( uims_tntpara_t *para, uims_tntval_t **values,
						int nvalues, uims_dbdataset_t *paraset, int *psidx)
{
	int rv, idx;
	uims_dbdata_t *dbdata;
	uims_tntval_t *tntval;

	rv = uims_tntpara_add_array( para, nvalues);
	if( rv < UIMS_DB_SUCCESS) return rv;

	for( idx = 0; idx < nvalues; ++idx) {
		tntval = values[idx];
		rv = uims_tntpara_add_array( para, 3); 
		if( rv < UIMS_DB_SUCCESS) return rv;

		switch( tntval->op) {
			case TNT_UOP_ADDITION: rv = uims_tntpara_add_str( para, "+", 1); break;
			case TNT_UOP_SUBSTRACT: rv = uims_tntpara_add_str( para, "-", 1); break;
			case TNT_UOP_AND : rv = uims_tntpara_add_str( para, "&", 1); break;
			case TNT_UOP_XOR : rv = uims_tntpara_add_str( para, "^", 1); break;
			case TNT_UOP_OR : rv = uims_tntpara_add_str( para, "|", 1); break;
			case TNT_UOP_ASSIGN : rv = uims_tntpara_add_str( para, "=", 1); break;
			default :
				ux_log(UXL_MAJ, "Not supported operation. (idx=%d, param=%d, op=%d, fldno=%d)",
								idx, *psidx, tntval->op, tntval->fldno);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
		}
		if( rv < UIMS_DB_SUCCESS) return rv;

		rv = uims_tntpara_add_uint( para, tntval->fldno); 
		if( rv < UIMS_DB_SUCCESS) return rv;

		switch( tntval->type) {
			case UIMS_TNTVTYPE_PARAM:
				dbdata = uims_dbdataset_get_data( paraset, *psidx);
				if( dbdata == NULL) {
					rv = UIMS_DBERR_INVALID_IDX;
					ux_log(UXL_MAJ, "Failed to set parameter cause of no data. (idx=%d, param=%d)",
								idx, *psidx);
				} else {
					rv = uims_tntpara_add_dbdata( para, dbdata);
				}
				*psidx += 1;
				break;
			case UIMS_TNTVTYPE_INT:
				rv = uims_tntpara_add_int( para, tntval->v.i64);
				break;
			case UIMS_TNTVTYPE_DOUBLE:
				rv = uims_tntpara_add_double( para, tntval->v.d64);
				break;
			case UIMS_TNTVTYPE_STR:
				rv = uims_tntpara_add_str( para, tntval->v.str, tntval->len);
				break;
			case UIMS_TNTVTYPE_OCT:
				rv = uims_tntpara_add_bin( para, tntval->v.oct, tntval->len);
				break;
			default :
				rv = UIMS_DBERR_INVALID_TYPE;
				ux_log(UXL_MAJ, "Failed to set parameter cause of invalid type. (idx=%d, param=%d, type=%d)",
								idx, *psidx, tntval->type);
				break;
		}

		if( rv < UIMS_DB_SUCCESS) return rv; 
	}

	return UIMS_DB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntrset_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool result set을 생성해 반환한다.
 * @param bufsize result set을 담을 buffer의 초기 크기
 * @param err 실행 결과를 담음 포인터
 * @return 생성된 tarantool result set
 */
uims_tntrset_t* uims_tntrset_create( uint32_t bufsize, int *err)
{
	int rv;
	uims_tntrset_t *tntrset;

	tntrset = (uims_tntrset_t*)ux_zalloc( ux_mem_default(), sizeof(uims_tntrset_t));
	if( tntrset == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_tntrset_t. (size=%u)", bufsize);
		if(err) *err = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	rv = uims_tntrset_init( tntrset, bufsize);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize uims_tntrset_t. (size=%u, err=%d,%s)", 
				bufsize, rv, uims_dberr_to_str(rv));
		ux_free( ux_mem_default(), tntrset);
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return tntrset;
}

/**
 * @internal tarantool result set을 소멸한다. 
 * @param tntrset 소멸할 tarantool result set
 */
void uims_tntrset_destroy( uims_tntrset_t *tntrset)
{
	uims_tntrset_final( tntrset);
	ux_free( ux_mem_default(), tntrset);
}

/**
 * @internal tarantool result set을 초기화 한다.
 * @param rset result set
 * @param bufsize 초기에 사용할 외부 buffer의 크기
 * @return 실행 결과
 */
uims_dberr_e uims_tntrset_init( uims_tntrset_t *rset, uint32_t bufsize)
{
	tnt_reply_init( rset->reply);
	rset->ntuples = 0;
	rset->cdata = NULL;
	rset->buffer = NULL;
	rset->bufsize = 0;
	return uims_tntrset_resize( rset, bufsize); 
}

/**
 * @internal tarantool result set 사용을 종료한다.
 * @param rset result set
 */
void uims_tntrset_final( uims_tntrset_t *rset)
{
	if (rset->reply->buf) {
		if(rset->reply->buf != rset->buffer) tnt_mem_free((void *)rset->reply->buf);
		rset->reply->buf = NULL;
	}
	rset->reply->buf_size = 0;
	if( rset->buffer ) {
		ux_free( ux_mem_default(), rset->buffer);
		rset->buffer = NULL;
	}
	rset->bufsize = 0;
}

/**
 * @internal tarantool result set buffer 크기를 조정한다.
 * @param rset result set
 * @param bufsize 새로운 buffer 크기
 * @return 실행 결과
 */
uims_dberr_e uims_tntrset_resize( uims_tntrset_t *tntrset, uint32_t bufsize)
{
	char *buffer;

	if( bufsize <= tntrset->bufsize) return UIMS_DB_SUCCESS;

	buffer = ux_zalloc( ux_mem_default(), bufsize);
	if( buffer == NULL) {
		ux_log(UXL_CRT, "Failed to allocate TNT rest buffer. (size=%d)", bufsize);
		return UIMS_DBERR_NO_MEMORY;
	}
	if( tntrset->buffer ) ux_free( ux_mem_default(), tntrset->buffer);
	tntrset->bufsize = bufsize;
	tntrset->buffer = buffer;

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool result set에 응답 수신후 결과를 적용한다.
 * @param rset result set
 */
void uims_tntrset_apply( uims_tntrset_t *rset)
{
	if( rset->reply->code != 0) return;
	if( rset->reply->data == NULL) {
		ux_log(UXL_MAJ, "reply is empty");
		return;
	}

	rset->cdata = rset->reply->data;
	if( mp_typeof(*rset->cdata) != MP_ARRAY) {
		ux_log(UXL_MAJ, "Tarantool result set tuple is not array type(%d)", mp_typeof(*rset->cdata));
		rset->ntuples = 0;
	} else {
		rset->ntuples = mp_decode_array( &rset->cdata);
	}
}

/**
 * @internal tarantool result set결과를 reset한다.
 * @param rset result set
 */
void uims_tntrset_reset( uims_tntrset_t *rset)
{
	if (rset->reply->buf) {
		if(rset->reply->buf != rset->buffer) tnt_mem_free((void *)rset->reply->buf);
		rset->reply->buf = NULL;
	}
	rset->ntuples = 0;
	rset->cdata = NULL;
}

/**
 * @internal tarantool result set의 결과 code 값을 반환한다. (tnt_proto.h
 * @param rset result set
 * @return 수신된 result code 값
 */
enum tnt_errcode_t uims_tntrset_get_rcode( uims_tntrset_t *rset)
{
	return rset->reply->code;
}

/**
 * @internal 수신된 result code가 TNT_ER_UNKNOWN(0)이 아닐 경우 error 문자열을 반환한다.
 * @param rset result set
 * @return 수신된 error string
 */
const char* uims_tntrset_get_errstr( uims_tntrset_t *rset)
{
	return rset->reply->error ? rset->reply->error : "";
}

/**
 * @internal 수신된 result로 부터 array 값의 길이를 읽는다.
 * @param rset result set
 * @param err 실행 결과를 담을 포인터
 * @return 읽어 들인 array 값의 길이
 */
uint32_t uims_tntrset_read_array( uims_tntrset_t *rset, int *err)
{
	switch( mp_typeof(*rset->cdata)) {
		case MP_NIL :
			if(err) *err = UIMS_DB_SUCCESS;
			mp_decode_nil( &rset->cdata);
			return 0;
		case MP_ARRAY :
			if(err) *err = UIMS_DB_SUCCESS;
			return mp_decode_array( &rset->cdata);
		default : break;
	}
	ux_log(UXL_MAJ, "Failed to read data. current data is not ARRAY compliant type. (type=%s)",
			uims_tntmptype_to_str(mp_typeof(*rset->cdata)));
	if(err) *err = UIMS_DBERR_INVALID_PARAMETER;
	return 0;
}

/**
 * @internal 수신된 result로 부터 map 값의 길이를 읽는다.
 * @param rset result set
 * @param err 실행 결과를 담을 포인터
 * @return 읽어 들인 map 값의 길이
 */
uint32_t uims_tntrset_read_map( uims_tntrset_t *rset, int *err)
{
	switch( mp_typeof(*rset->cdata)) {
		case MP_NIL :
			if(err) *err = UIMS_DB_SUCCESS;
			mp_decode_nil( &rset->cdata);
			return 0;
		case MP_MAP :
			if(err) *err = UIMS_DB_SUCCESS;
			return mp_decode_map( &rset->cdata);
		default : break;
	}

	ux_log(UXL_MAJ, "Failed to read data. current data is not MAP compliant type. (type=%s)",
			uims_tntmptype_to_str(mp_typeof(*rset->cdata)));
	if(err) *err = UIMS_DBERR_INVALID_PARAMETER;
	return 0;
}

/**
 * @internal tarantool resultset의 한 tuple 값을 DB result set에 복사한다.
 * @param rset tarnatool result set
 * @param rsltset DB result set
 * @return 실행 결과
 */
uims_dberr_e uims_tntrset_write( uims_tntrset_t *rset, uims_dbdataset_t *rsltset)
{
	int rv, i;
	uint32_t nfields, len;
	uint8_t c;

	if( rset->cdata >= rset->reply->buf+rset->reply->buf_size) {
		return UIMS_DBERR_NO_DATA;
	}

	nfields = uims_tntrset_read_array( rset, &rv);
	if( rv < UIMS_DB_SUCCESS) return rv;

	for( i = 0; i < nfields; ++i) {
		c = mp_load_u8( &rset->cdata);
		switch (c) {
			case 0x00 ... 0x7f: //UINT8
				rv = uims_dbdataset_set_uint8( rsltset, i, c);
				break;
			case 0x80 ... 0x8f: //MAP
				len =  c & 0xf;
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0x90 ... 0x9f: //ARRAY
				len = c & 0xf;
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xa0 ... 0xbf: //STRING
				len = c & 0x1f;
				rv = uims_dbdataset_set_strn( rsltset, i, rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xc0: //NIL
				rv = uims_dbdataset_set_oct( rsltset, i, NULL, -1);
				break;
			case 0xc1: //Never used;
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xc2: //bool false;
				rv = uims_dbdataset_set_uint8( rsltset, i, 0);
				break;
			case 0xc3: //bool true;
				rv = uims_dbdataset_set_uint8( rsltset, i, 1);
				break;
			case 0xc4: //BIN
				len = mp_load_u8( &rset->cdata);
				rv = uims_dbdataset_set_oct( rsltset, i, (uint8_t*)rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xc5: //BIN
				len = mp_load_u16( &rset->cdata);
				rv = uims_dbdataset_set_oct( rsltset, i, (uint8_t*)rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xc6: //BIN
				len = mp_load_u32( &rset->cdata);
				rv = uims_dbdataset_set_oct( rsltset, i, (uint8_t*)rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xc7: //EXT
				len = mp_load_u8( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xc8: //EXT
				len = mp_load_u16( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xc9: //EXT
				len = mp_load_u32( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xca: //FLOAT
				rv = uims_dbdataset_set_float( rsltset, i, mp_load_float( &rset->cdata));
				break;
			case 0xcb: //DOUBLE
				rv = uims_dbdataset_set_double( rsltset, i, mp_load_double( &rset->cdata));
				break;
			case 0xcc:
				rv = uims_dbdataset_set_uint8( rsltset, i, mp_load_u8( &rset->cdata));
				break;
			case 0xcd:
				rv = uims_dbdataset_set_uint16( rsltset, i, mp_load_u16( &rset->cdata));
				break;
			case 0xce:
				rv = uims_dbdataset_set_uint32( rsltset, i, mp_load_u32( &rset->cdata));
				break;
			case 0xcf:
				rv = uims_dbdataset_set_uint64( rsltset, i, mp_load_u64( &rset->cdata));
				break;
			case 0xd0:
				rv = uims_dbdataset_set_int8( rsltset, i, (int8_t)mp_load_u8( &rset->cdata));
				break;
			case 0xd1:
				rv = uims_dbdataset_set_int16( rsltset, i, (int16_t)mp_load_u16( &rset->cdata));
				break;
			case 0xd2:
				rv = uims_dbdataset_set_int32( rsltset, i, (int32_t)mp_load_u32( &rset->cdata));
				break;
			case 0xd3:
				rv = uims_dbdataset_set_int64( rsltset, i, (int64_t)mp_load_u64( &rset->cdata));
				break;
			case 0xd4: //EXT
			case 0xd5: //EXT
			case 0xd6: //EXT
			case 0xd7: //EXT
			case 0xd8: //EXT
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xd9: //STR
				len = mp_load_u8( &rset->cdata);
				rv = uims_dbdataset_set_strn( rsltset, i, rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xda: //STR
				len = mp_load_u16( &rset->cdata);
				rv = uims_dbdataset_set_strn( rsltset, i, rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xdb: //STR
				len = mp_load_u32( &rset->cdata);
				rv = uims_dbdataset_set_strn( rsltset, i, rset->cdata, len);
				rset->cdata += len;
				break;
			case 0xdc: //ARRAY
				len = mp_load_u16( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xdd: //ARRAY
				len = mp_load_u32( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xde: //MAP
				len = mp_load_u16( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xdf: //MAP
				len = mp_load_u32( &rset->cdata);
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
			case 0xe0 ... 0xff:
				rv = uims_dbdataset_set_int8( rsltset, i, (int8_t)c);
				break;
			default :
				rv = UIMS_DBERR_INVALID_TYPE;
				break;
		}
		if( rv < UIMS_DB_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to read field. (idx=%d, type=%d, err=%d,%s)",
					i, mp_typeof(c), rv, uims_dberr_to_str(rv));
			return rv;
		}
	}

	return UIMS_DB_SUCCESS;

}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntdb_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool DB 를 생성한다.
 * @param db UIMS DB instance
 * @param confstr configuration string
 * @param err 실행 결과를 담을 포인터
 * @return 생성된 tarantool DB
 */
uims_tntdb_t* uims_tntdb_create( uims_db_t *db, const char *confstr, int *err)
{
	int rv;
	uims_tntdb_t *tntdb;

	tntdb = (uims_tntdb_t*)ux_zalloc( ux_mem_default(), sizeof(uims_tntdb_t));
	if( tntdb == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_tntdb_t. (confstr=%s)", confstr);
		if(err) *err = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	rv = uims_tntdb_init( tntdb, db, confstr);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize uims_tntdb_t. (confstr=%s, err=%d,%s)", 
				confstr, rv, uims_dberr_to_str(rv));
		ux_free( ux_mem_default(), tntdb);
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return tntdb;
}

/**
 * @internal tarantool DB 를 소멸한다. 
 * @param tntdb 소멸할 tarantool DB
 */
void uims_tntdb_destroy( uims_tntdb_t *tntdb)
{
	uims_tntdb_final( tntdb);
	ux_free( ux_mem_default(), tntdb);
}

/**
 * @internal tarantool DB 를 초기화 한다. 
 * @param tntdb 초기화할 tarantool DB
 * @param db UIMS DB instance
 * @param confstr configuration string
 * @return 실행 결과 
 */
uims_dberr_e uims_tntdb_init( uims_tntdb_t *tntdb, uims_db_t *db, const char *confstr)
{
	char *c, *v, *puri, *suri, *params;

	tntdb->buffer = ux_str_dup( confstr, ux_mem_default());
	if(tntdb->buffer == NULL) {
		ux_log( UXL_CRT, "Failed to duplicate configuration uims_tntdb_t. (confstr=%s)", confstr);
		return UIMS_DBERR_NO_MEMORY;
	}
	puri = tntdb->buffer;
	c = strchr( puri, ';');
	if( c ) {
		*c = '\0';
		suri = c+1;
	} else {
		suri = NULL;
	}
	c = (suri) ? strchr(suri, ';') : NULL; 
	if( c ) {
		*c = '\0';
		params = c+1;
	} else {
		params = NULL; 
	}

	tntdb->db = db;
	tntdb->uris[0] = ux_str_trim(puri);
	tntdb->uris[1] = suri ? ux_str_trim(suri) : NULL;
	tntdb->reconnect_period = 3;
	tntdb->error_wait_msec = 1000;
	tntdb->check_recv_size = 0;
	tntdb->max_rset_buf_size = 4096*1024;

	while( params) {
		c = strchr(suri, ';');
		if(c) *c = '\0';
		params = ux_str_trim(params);
		if( strncasecmp(params, "reconnect_period=", 17) == 0) {
			v = ux_str_trim(&params[17]);
			if(v && v[0]) tntdb->reconnect_period = strtoul(v, NULL, 0);
			if( tntdb->reconnect_period == 0) tntdb->reconnect_period = 3;
		} else if( strncasecmp(params, "error_wait_msec=", 16) == 0) {
			v = ux_str_trim(&params[16]);
			if(v && v[0]) tntdb->error_wait_msec = strtoul(v, NULL, 0);
			if( tntdb->error_wait_msec == 0) tntdb->error_wait_msec = 3;
		}
		params = (c) ? c+1 : NULL;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool의 사용을 종료한다.
 * @param tntdb 종료 할 tarantool DB
 */
void uims_tntdb_final( uims_tntdb_t *tntdb)
{
	if(tntdb->buffer) ux_free(ux_mem_default(), tntdb->buffer);
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntsql_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool SQL parser를 초기화한다.
 * @param tntsql 초기화할 SQL parser
 * @return 실행 결과
 */
uims_dberr_e uims_tntsql_init( uims_tntsql_t *tntsql)
{
	int rv;

	tntsql->tknmem = ux_fxmem_create( sizeof(uims_tnttkn_t), 0, 64, UX_FALSE);
	if(tntsql->tknmem == NULL) {
		ux_log(UXL_CRT, "Failed to create memory allocator for Tarantool Query Token");
		return UIMS_DBERR_NO_MEMORY;
	}

	tntsql->valmem = ux_fxmem_create( sizeof(uims_tntval_t), 0, 256, UX_FALSE);
	if(tntsql->valmem == NULL) {
		ux_log(UXL_CRT, "Failed to create memory allocator for Tarantool Query Value");
		ux_mem_destroy( tntsql->tknmem);
		return UIMS_DBERR_NO_MEMORY;
	}

	rv = ux_vector_init( tntsql->keys, 64, ux_mem_default());
	if( rv < UX_SUCCESS) {
		ux_log(UXL_CRT, "Failed to init tarantool SQL parser keys vector. (err=%d,%s)", rv, ux_errnostr(rv));
		ux_mem_destroy( tntsql->valmem);
		ux_mem_destroy( tntsql->tknmem);
		return UIMS_DBERR_NO_MEMORY;
	}

	rv = ux_vector_init( tntsql->ops, 64, ux_mem_default());
	if( rv < UX_SUCCESS) {
		ux_log(UXL_CRT, "Failed to init tarantool SQL parser operation vector. (err=%d,%s)", rv, ux_errnostr(rv));
		ux_vector_final( tntsql->keys);
		ux_mem_destroy( tntsql->valmem);
		ux_mem_destroy( tntsql->tknmem);
		return UIMS_DBERR_NO_MEMORY;
	}

	tntsql->ntokens = 0;
	UX_LIST_INIT( tntsql->tokens);

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool SQL parser의 사용을 종료한다.
 * @param tntsql 종료할 SQL parser
 */
void uims_tntsql_final( uims_tntsql_t *tntsql)
{
	uims_tntsql_clear(tntsql);
	uims_tntlex_final( tntsql->lexer);
	ux_vector_final( tntsql->ops);
	ux_vector_final( tntsql->keys);
	if(tntsql->valmem) ux_mem_destroy( tntsql->valmem);
	if(tntsql->tknmem) ux_mem_destroy( tntsql->tknmem);
}

/**
 * @internal tarantool SQL parser내 할당된 정보들을 모두 clear한다. 
 * @param tntsql SQL parser
 */
void uims_tntsql_clear( uims_tntsql_t *tntsql)
{
	int i;
	ux_lnode_t *lnode;
	uims_tntval_t *tntval;

	for( i = 0; i < ux_vector_count(tntsql->keys); ++i) {
		tntval = (uims_tntval_t*)ux_vector_get_node( tntsql->keys, i);
		if(tntval) uims_tntval_destroy( tntval, tntsql->valmem);
	}
	ux_vector_clear( tntsql->keys);

	for( i = 0; i < ux_vector_count(tntsql->ops); ++i) {
		tntval = (uims_tntval_t*)ux_vector_get_node( tntsql->ops, i);
		if(tntval) uims_tntval_destroy( tntval, tntsql->valmem);
	}
	ux_vector_clear( tntsql->ops);

	for( i = tntsql->ntokens*2; i >= 0; --i) {
		UX_LIST_POP( tntsql->tokens, lnode);
		if(lnode == NULL) break;
		uims_tnttkn_destroy( (uims_tnttkn_t*)lnode, tntsql->tknmem);
	}

	tntsql->ntokens = 0;
}

/**
 * @internal 전달된 statement의 SQL을 파싱하기 위한 초기화 작업을 수행한다.
 * @param tntsql SQL parser
 * @param stmt 파싱할 statement
 * @return 실행 결과
 * @detail lexer로 token들을 분리해 queue에 담는다.
 */
uims_dberr_e uims_tntsql_init_parsing( uims_tntsql_t *tntsql, uims_tntstmt_t *stmt)
{
	int rv, kind;

	tntsql->stmt = stmt;

	rv = uims_tntlex_init( tntsql->lexer);
	if( rv != 0) {
		rv = UX_ERROR();
		ux_log(UXL_MAJ, "Failed to prepare lexer. (stmtid=%s, err=%d,%s)",
				stmt->id, rv, ux_errnostr(rv));
		return UIMS_DBERR_ERROR;
	}

	uims_tntlex_set_scan_str( tntsql->lexer, stmt->query);
	for(;;) {
		kind = uims_tntlex_scan( tntsql->lexer);
		if( kind < 0) {
			ux_log(UXL_MAJ, "Invalid statement. (stmtid=%s, query=%s, err=%d,%s)",
					stmt->id, stmt->query, rv, uims_dberr_to_str(rv));
			uims_tntsql_clear( tntsql);
			uims_tntlex_final( tntsql->lexer);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
		if( kind == 0) break;

		rv = uims_tntsql_add_token( tntsql, kind, uims_tntlex_get_text( tntsql->lexer));
		if( rv < UIMS_DB_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to add token. (stmtid=%s, token=%d,%s, err=%d,%s)",
					stmt->id, kind, uims_tntlex_get_text(tntsql->lexer), rv, uims_dberr_to_str(rv));
			uims_tntsql_clear( tntsql);
			uims_tntlex_final( tntsql->lexer);
			return rv;
		}
	};

	return UIMS_DB_SUCCESS;
}

/**
 * @internal 전달된 statement의 SQL을 파싱후 종료 작업을 수행한다.
 * @param tntsql SQL parser
 * @detail lexer로 할당된 token들 및 중간 데이터들을 모두 삭제한다.
 */
void uims_tntsql_final_parsing( uims_tntsql_t *tntsql)
{
	tntsql->stmt = NULL;
	uims_tntsql_clear( tntsql);
	uims_tntlex_final( tntsql->lexer);
}

/**
 * @internal 전달된 값의 token을 추가한다.
 * @param tntsql SQL parser
 * @param kind token 종류
 * @param text token text 값
 * @return 실행 결과
 */
uims_dberr_e uims_tntsql_add_token( uims_tntsql_t *tntsql, int kind, const char *text)
{
	uims_tnttkn_t *tnttkn;

	tnttkn = uims_tnttkn_create( tntsql->tknmem, kind, text);
	if( tnttkn == NULL) {
		ux_log(UXL_CRT, "Failed to create token. (stmtid=%s, kind=%d, text=%s)",
				tntsql->stmt->id, kind, text);
		return UIMS_DBERR_NO_MEMORY;
	}

	UX_LIST_INSERT_TAIL( tntsql->tokens, tnttkn->lnode);
	tntsql->ntokens++;

	return UIMS_DB_SUCCESS;
}

/**
 * @internal 전달된 token의 다음 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @param prerr err출력 여부
 * @return 다음 token. 없으면 NULL을 반환한다.
 */ 
uims_tnttkn_t* uims_tntsql_get_next_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur,
						int prerr)
{
	ux_lnode_t *next;

	if(cur == NULL) next = UX_LIST_HEAD( tntsql->tokens);
	else next = UX_LIST_NEXT(cur->lnode);
	if( next == UX_LIST_END(tntsql->tokens)) {
		if(prerr) {
			if( cur) { 
				ux_log(UXL_MAJ, "Invalid query statement. near by '%s'. (stmtid=%s, err=No more token)",
						cur->text, tntsql->stmt->id);
			} else {
				ux_log(UXL_MAJ, "Invalid query statement. (stmtid=%s, err=No more token)", tntsql->stmt->id);
			}
		}
		return NULL;
	}
	return (uims_tnttkn_t*)next;
}

/**
 * @internal 전달된 token의 다음 token이 전달된 종류의 token일 경우에만 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @parma kind token type
 * @return 다음 token. 없거나 다른 종류의 token이 있으면 NULL을 반환한다.
 */
uims_tnttkn_t* uims_tntsql_get_expect_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur,
						int kind)
{
	uims_tnttkn_t *next;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;
	if( next->kind == kind) return next;

	ux_log(UXL_MAJ, "Invalid query statement. '%s' is not expected. (stmtid=%s)",
			next->text, tntsql->stmt->id);
	return NULL;
}

/**
 * @internal 전달된 token 다음의 space id로 사용 가능한 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return space id 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */ 
uims_tnttkn_t* uims_tntsql_get_space_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	long long ival;
	uims_tnttkn_t *next;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch(next->kind) {
		case UIMS_TNTTK_NUMBER :
			ival = strtoll( next->text, NULL, 10);
			if( ival < 0 || ival >= UINT32_MAX) {
				ux_log(UXL_MIN, "Invalid query statement. '%s' must be UINT32 or NAME.", next->text);
				return NULL;
			}
			tntsql->stmt->space = ival;
			break;
		case UIMS_TNTTK_ID :
			if( strncasecmp( next->text, "table", 5) == 0 && next->text[5] >= '0' && next->text[5] <= '9') {
				ival = strtoll( next->text+5, NULL, 10);
				if( ival < 0 || ival >= UINT32_MAX) {
					ux_log(UXL_MIN, "Invalid query statement. '%s' must be table$.", next->text);
					return NULL;
				}
				tntsql->stmt->space = ival;
				break;
			} else if( strncasecmp( next->text, "space", 5) == 0 && next->text[5] >= '0' && next->text[5] <= '9') {
				ival = strtoll( next->text+5, NULL, 10);
				if( ival < 0 || ival >= UINT32_MAX) {
					ux_log(UXL_MIN, "Invalid query statement. '%s' must be space$.", next->text);
					return NULL;
				}
				tntsql->stmt->space = ival;
				break;
			}

		default :
			ux_log(UXL_MIN, "Invalid query statement. '%s' must be UINT32 or table$ or space.", next->text);
			return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 index id로 사용 가능한 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return index id로 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */ 
uims_tnttkn_t* uims_tntsql_get_index_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	long long ival;
	uims_tnttkn_t *next;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch(next->kind) {
		case UIMS_TNTTK_NUMBER :
			ival = strtoll( next->text, NULL, 10);
			if( ival < 0 || ival >= UINT32_MAX) {
				ux_log(UXL_MIN, "Invalid query statement. '%s' must be UINT32 or NAME.", next->text);
				return NULL;
			}
			tntsql->stmt->idxid = ival;
			break;
		case UIMS_TNTTK_ID :
			if( strncasecmp( next->text, "index", 5) == 0 && next->text[5] >= '0' && next->text[5] <= '9') {
				ival = strtoll( next->text+5, NULL, 10);
				if( ival < 0 || ival >= UINT32_MAX) {
					ux_log(UXL_MIN, "Invalid query statement. '%s' must be index$.", next->text);
					return NULL;
				}
				tntsql->stmt->idxid = ival;
				break;
			}
		default :
			ux_log(UXL_MIN, "Invalid query statement. '%s' must be UINT32 or index$.", next->text);
			return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 field id로 사용 가능한 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @param fldno field number를 담을 포인터
 * @return field id로 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */ 
uims_tnttkn_t* uims_tntsql_get_field_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur, uint32_t *fldno)
{
	long long ival;
	uims_tnttkn_t *next;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch(next->kind) {
		case UIMS_TNTTK_NUMBER :
			ival = strtoll( next->text, NULL, 10);
			if( ival < 0 || ival >= UINT32_MAX) {
				ux_log(UXL_MIN, "Invalid query statement. '%s' must be UINT32 or NAME.", next->text);
				return NULL;
			}
			*fldno = ival;
			break;
		case UIMS_TNTTK_ID :
			if( strncasecmp( next->text, "column", 6) == 0 && next->text[6] >= '0' && next->text[6] <= '9') {
				ival = strtoll( next->text+6, NULL, 10);
				if( ival < 0 || ival >= UINT32_MAX) {
					ux_log(UXL_MIN, "Invalid query statement. '%s' must be column$.", next->text);
					return NULL;
				}
				*fldno = ival;
				break;
			} else if( strncasecmp( next->text, "field", 5) == 0 && next->text[5] >= '0' && next->text[5] <= '9') {
				ival = strtoll( next->text+5, NULL, 10);
				if( ival < 0 || ival >= UINT32_MAX) {
					ux_log(UXL_MIN, "Invalid query statement. '%s' must be filed$.", next->text);
					return NULL;
				}
				*fldno = ival;
				break;
			}
		default :
			ux_log(UXL_MIN, "Invalid query statement. '%s' must be UINT32 or column$ or field$.", next->text);
			return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 iterator로 사용 가능한 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return iterator로 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */ 
uims_tnttkn_t* uims_tntsql_get_iter_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	uims_tnttkn_t *next;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch( next->kind) {
		case UIMS_TNTTK_EQ : tntsql->stmt->iter = TNT_ITER_EQ; break;
		case UIMS_TNTTK_NE : tntsql->stmt->iter = TNT_ITER_REQ; break; 
		case UIMS_TNTTK_LT : tntsql->stmt->iter = TNT_ITER_LT; break;
		case UIMS_TNTTK_LE : tntsql->stmt->iter = TNT_ITER_LE; break;
		case UIMS_TNTTK_GT : tntsql->stmt->iter = TNT_ITER_GT; break;
		case UIMS_TNTTK_GE : tntsql->stmt->iter = TNT_ITER_GE; break;
		case '&' : tntsql->stmt->iter = TNT_ITER_BITS_ALL_SET; break;
		case '|' : tntsql->stmt->iter = TNT_ITER_BITS_ANY_SET; break;
		case '^' : tntsql->stmt->iter = TNT_ITER_BITS_ALL_NOT_SET; break;
		case '+' : tntsql->stmt->iter = TNT_ITER_OVERLAP; break;
		case '-' : tntsql->stmt->iter = TNT_ITER_NEIGHBOR; break;
		default :
			ux_log(UXL_MAJ, "Invalid query statement. '%s' is not iterator. (stmtid=%s)",
						next->text, tntsql->stmt->id);
			return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 operation으로 사용 가능한 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @param op 획득한 operation code를 담을 포인터
 * @return operation으로 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */ 
uims_tnttkn_t* uims_tntsql_get_op_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur,
						uint8_t *op)
{
	uims_tnttkn_t *next;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch( next->kind) {
		case '+' : *op = TNT_UOP_ADDITION; break;
		case '-' : *op = TNT_UOP_SUBSTRACT; break;
		case '&' : *op = TNT_UOP_AND; break;
		case '^' : *op = TNT_UOP_XOR; break;
		case '|' : *op = TNT_UOP_OR; break;
		case UIMS_TNTTK_EQ : *op = TNT_UOP_ASSIGN; break;
		default :
			ux_log(UXL_MAJ, "Invalid query statement. '%s' is not operation. (stmtid=%s)",
						next->text, tntsql->stmt->id);
			return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 key 또는 tuple로 사용 가능한 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return key 또는 tuple로 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */
uims_tnttkn_t* uims_tntsql_get_key_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	int rv;
	uims_tnttkn_t *next;
	uims_tntval_t *tntval;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch( next->kind) {
		case '?' : 
		case UIMS_TNTTK_NUMBER :
		case UIMS_TNTTK_FLOAT :
		case UIMS_TNTTK_STRING :
		case UIMS_TNTTK_OCTSTR : break;
		default :
			ux_log(UXL_MAJ, "Invalid query statement. '%s' is not value. (stmtid=%s)",
					next->text, tntsql->stmt->id);
			return NULL;
	}

	tntval = uims_tntval_create( tntsql->valmem, 0, 0, next, &rv);
	if( tntval == NULL) {
		ux_log(UXL_MAJ, "Faied to create value '%s'. (stmtid=%s)", next->text, tntsql->stmt->id);
		return NULL;
	}

	rv = ux_vector_append( tntsql->keys, tntval);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_MAJ, "Faied to add value '%s'. (stmtid=%s, err=%d,%s)",
				next->text, tntsql->stmt->id, rv, ux_errnostr(rv));
		uims_tntval_destroy( tntval, tntsql->valmem);
		return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 fldno,op,value로 구성된 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return fldno, op, value 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */
uims_tnttkn_t* uims_tntsql_get_opval_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	int rv;
	uint32_t fldno;
	uint8_t op;
	uims_tnttkn_t *next;
	uims_tntval_t *tntval;

	next = uims_tntsql_get_field_token( tntsql, cur, &fldno);
	if( next == NULL) return NULL;

	next = uims_tntsql_get_op_token( tntsql, next, &op);
	if( next == NULL) return NULL;

	next = uims_tntsql_get_next_token( tntsql, next, UX_TRUE);
	if( next == NULL) return NULL;

	switch( next->kind) {
		case '?' : 
		case UIMS_TNTTK_NUMBER :
		case UIMS_TNTTK_FLOAT :
		case UIMS_TNTTK_STRING :
		case UIMS_TNTTK_OCTSTR : break;
		default :
			ux_log(UXL_MAJ, "Invalid query statement. '%s' is not value. (stmtid=%s)",
					next->text, tntsql->stmt->id);
			return NULL;
	}

	tntval = uims_tntval_create( tntsql->valmem, fldno, op, next, &rv);
	if( tntval == NULL) {
		ux_log(UXL_MAJ, "Faied to create value '%s'. (stmtid=%s)", next->text, tntsql->stmt->id);
		return NULL;
	}

	rv = ux_vector_append( tntsql->ops, tntval);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_MAJ, "Faied to add value '%s'. (stmtid=%s, err=%d,%s)",
				next->text, tntsql->stmt->id, rv, ux_errnostr(rv));
		uims_tntval_destroy( tntval, tntsql->valmem);
		return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token 다음의 operatin integer 값으로 구성된 token을 반환한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return operation integer 사용 가능한 다음 token. 없으면 NULL을 반환한다.
 */
uims_tnttkn_t* uims_tntsql_get_opint_token( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	int rv;
	uims_tnttkn_t *next;
	uims_tntval_t *tntval;

	next = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
	if( next == NULL) return NULL;

	switch( next->kind) {
		case '?' : 
		case UIMS_TNTTK_NUMBER : break;
		default :
			ux_log(UXL_MAJ, "Invalid query statement. '%s' is not int value. (stmtid=%s)",
					next->text, tntsql->stmt->id);
			return NULL;
	}

	tntval = uims_tntval_create( tntsql->valmem, 0, 0, next, &rv);
	if( tntval == NULL) {
		ux_log(UXL_MAJ, "Faied to create value '%s'. (stmtid=%s)", next->text, tntsql->stmt->id);
		return NULL;
	}

	rv = ux_vector_append( tntsql->ops, tntval);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_MAJ, "Faied to add value '%s'. (stmtid=%s, err=%d,%s)",
				next->text, tntsql->stmt->id, rv, ux_errnostr(rv));
		uims_tntval_destroy( tntval, tntsql->valmem);
		return NULL;
	}

	return next;
}

/**
 * @internal 전달된 token이 statement의 끝을 나타내는지 확인한다. 
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_check_eos( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	if( cur == NULL) return UIMS_DB_SUCCESS;
	if( cur->kind != ';') {
		ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
		return UIMS_DBERR_INVALID_STATEMENT;
	}
	cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
	if( cur) {
		ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
		return UIMS_DBERR_INVALID_STATEMENT;
	}
	return UIMS_DB_SUCCESS;
}

/**
 * @internal select 문을 파싱한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse_select( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	tntsql->stmt->command = TNT_OP_SELECT;
	//SELECT * FROM {space} WHERE {index}{interator}?,? [LIMIT {limit}|{offset},{limit}] ;
	
	cur = uims_tntsql_get_expect_token( tntsql, cur, '*');
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_FROM);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_space_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_WHERE);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_index_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_iter_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	for(;;) {
		cur = uims_tntsql_get_key_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
		if( cur == NULL) break;
		else if( cur->kind == UIMS_TNTTK_LIMIT) break;
		else if( cur->kind == ';') break;
		else if( cur->kind != ',') {
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'. (stmtid=%s)",
					cur->text, tntsql->stmt->id);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}
	if( cur && cur->kind == UIMS_TNTTK_LIMIT) {
		cur = uims_tntsql_get_opint_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
		if( cur ) {
			if( cur->kind == ',') {
				cur = uims_tntsql_get_opint_token( tntsql, cur);
				if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

				cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
			} else if( cur->kind != ';') {
				ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
				return UIMS_DBERR_INVALID_STATEMENT;
			}
		}
	}

	return uims_tntsql_check_eos( tntsql, cur);
}

/**
 * @internal insert 또는 replace 문을 파싱한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse_insert( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	if( cur->kind == UIMS_TNTTK_REPLACE) {
		tntsql->stmt->command = TNT_OP_REPLACE;
		//INSERT INTO {space} VALUES (?,?,?);
	} else {
		tntsql->stmt->command = TNT_OP_INSERT;
		//REPLACE INTO {space} VALUES (?,?,?);
	}

	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_INTO);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_space_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_VALUES);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, '(');
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	for(;;) {
		cur = uims_tntsql_get_key_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
		if( cur == NULL) { 
			return UIMS_DBERR_INVALID_STATEMENT;
		} else if(cur->kind == ')') {
			cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
			break;
		} else if( cur->kind != ',') { 
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	return uims_tntsql_check_eos( tntsql, cur);
}

/**
 * @internal delete 문을 파싱한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse_delete( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	tntsql->stmt->command = TNT_OP_DELETE;
	//DELETE FROM {space} WHERE {index}=?,?;

	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_FROM);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_space_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_WHERE);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_index_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_EQ);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	for(;;) {
		cur = uims_tntsql_get_key_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
		if( cur == NULL) break;
		else if(cur->kind == ';') break;
		else if(cur->kind != ',') {
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	return uims_tntsql_check_eos( tntsql, cur);
}

/**
 * @internal update 문을 파싱한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse_update( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	tntsql->stmt->command = TNT_OP_UPDATE;
	//UPDATE {space} SET {fld}{op}?, {fld}{op}? WHERE {index}=?,?;

	cur = uims_tntsql_get_space_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_SET);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

	for(;;) {
		cur = uims_tntsql_get_opval_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
		else if(cur->kind == UIMS_TNTTK_WHERE) break; 
		else if(cur->kind != ',') {
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	cur = uims_tntsql_get_index_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_EQ);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	for(;;) {
		cur = uims_tntsql_get_key_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
		if( cur == NULL) break;
		else if(cur->kind == ';') break;
		else if(cur->kind != ',') {
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	return uims_tntsql_check_eos( tntsql, cur);
}

/**
 * @internal upsert 문을 파싱한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse_upsert( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	tntsql->stmt->command = TNT_OP_UPDATE;
	//UPSERT {space} VALUES (?,?) SET {fld}{op}?, {fld}{op}?;

	cur = uims_tntsql_get_space_token( tntsql, cur);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_VALUES);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	cur = uims_tntsql_get_expect_token( tntsql, cur, '(');
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	for(;;) {
		cur = uims_tntsql_get_key_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
		if( cur == NULL) { 
			return UIMS_DBERR_INVALID_STATEMENT;
		} else if(cur->kind == ')') {
			break;
		} else if( cur->kind != ',') { 
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_SET);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

	for(;;) {
		cur = uims_tntsql_get_opval_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
		if( cur == NULL) break;
		else if(cur->kind == ';') break; 
		else if(cur->kind != ',') {
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	return uims_tntsql_check_eos( tntsql, cur);
}

/**
 * @internal call 또는 eval 문을 파싱한다.
 * @param tntsql SQL parser
 * @param cur 현재 token 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse_call( uims_tntsql_t *tntsql, uims_tnttkn_t *cur)
{
	if( cur->kind == UIMS_TNTTK_EVAL) {
		tntsql->stmt->command = TNT_OP_EVAL;
		//CALL {proc}(?,?);
	} else {
		tntsql->stmt->command = TNT_OP_CALL;
		//EVAL {proc}(?,?);
	}

	cur = uims_tntsql_get_expect_token( tntsql, cur, UIMS_TNTTK_ID);
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	tntsql->stmt->proc = ux_str_dup( cur->text, ux_mem_default());
	if( tntsql->stmt->proc == NULL) {
		ux_log(UXL_CRT, "Failed duplcaite procedue name. (name=%s, stmtid=%s)",
				cur->text, tntsql->stmt->id);
		return UIMS_DBERR_NO_MEMORY;
	}
	cur = uims_tntsql_get_expect_token( tntsql, cur, '(');
	if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;
	for(;;) {
		cur = uims_tntsql_get_key_token( tntsql, cur);
		if( cur == NULL) return UIMS_DBERR_INVALID_STATEMENT;

		cur = uims_tntsql_get_next_token( tntsql, cur, UX_TRUE);
		if( cur == NULL) { 
			return UIMS_DBERR_INVALID_STATEMENT;
		} else if(cur->kind == ')') {
			cur = uims_tntsql_get_next_token( tntsql, cur, UX_FALSE);
			break;
		} else if( cur->kind != ',') { 
			ux_log(UXL_MIN, "Invalid query statement. near by '%s'.", cur->text);
			return UIMS_DBERR_INVALID_STATEMENT;
		}
	}

	return uims_tntsql_check_eos( tntsql, cur);
}

/**
 * @internal 전달된 statement의 query를 파싱한다.
 * @param tntsql SQL parser
 * @param stmt parsing할 statement 
 * @return 실행 결과 
 */
uims_dberr_e uims_tntsql_parse( uims_tntsql_t *tntsql, uims_tntstmt_t *stmt)
{
	int rv, i;
	uims_tnttkn_t *cur;

	rv = uims_tntsql_init_parsing( tntsql, stmt);
	if( rv < UIMS_DB_SUCCESS) return rv;

	cur = uims_tntsql_get_next_token( tntsql, NULL, UX_TRUE);
	if( cur == NULL) {
		rv = UIMS_DBERR_INVALID_STATEMENT;
	} else {
		switch(cur->kind) {
			case UIMS_TNTTK_SELECT :
				rv = uims_tntsql_parse_select( tntsql, cur);
				break;
			case UIMS_TNTTK_INSERT :
			case UIMS_TNTTK_REPLACE :
				rv = uims_tntsql_parse_insert( tntsql, cur);
				break;
			case UIMS_TNTTK_UPDATE :
				rv = uims_tntsql_parse_update( tntsql, cur);
				break;
			case UIMS_TNTTK_DELETE :
				rv = uims_tntsql_parse_delete( tntsql, cur);
				break;
			case UIMS_TNTTK_UPSERT :
				rv = uims_tntsql_parse_upsert( tntsql, cur);
				break;
			case UIMS_TNTTK_CALL :
			case UIMS_TNTTK_EVAL :
				rv = uims_tntsql_parse_call( tntsql, cur);
				break;
			case UIMS_TNTTK_PING :
				rv = UIMS_DB_SUCCESS;
				break;
			default :
				ux_log(UXL_MAJ, "Invalid statement. command '%s' is not supported. (stmtid=%s, query=%s, err=%d,%s)",
						cur->text, stmt->id, stmt->query, rv, uims_dberr_to_str(rv));
				rv = UIMS_DBERR_INVALID_STATEMENT;
				break;
		}
	}

	if( ux_vector_count( tntsql->keys) > 0) {
		stmt->nkeys = ux_vector_count( tntsql->keys);
		stmt->keys = ux_zalloc( ux_mem_default(), sizeof(uims_tntval_t*)*stmt->nkeys);
		if( stmt->keys) {
			for( i = 0; i < stmt->nkeys; ++i) {
				stmt->keys[i] = ux_vector_get_node( tntsql->keys, i);
			}
			ux_vector_clear( tntsql->keys);
		} else {
			ux_log(UXL_MAJ, "Failed to create key list. (stmtid=%s, count=%d)",
						stmt->id, stmt->nkeys);
			rv = UIMS_DBERR_INVALID_STATEMENT;
		}
	} else {
		stmt->nkeys = 0;
		stmt->keys = 0; 
	}
	if( ux_vector_count( tntsql->ops) > 0) {
		stmt->nops = ux_vector_count( tntsql->ops);
		stmt->ops = ux_zalloc( ux_mem_default(), sizeof(uims_tntval_t*)*stmt->nops);
		if( stmt->ops) {
			for( i = 0; i < stmt->nops; ++i) {
				stmt->ops[i] = ux_vector_get_node( tntsql->ops, i);
			}
			ux_vector_clear( tntsql->ops);
		} else {
			ux_log(UXL_MAJ, "Failed to create operation list. (stmtid=%s, count=%d)",
						stmt->id, stmt->nops);
			rv = UIMS_DBERR_INVALID_STATEMENT;
		}
	} else {
		stmt->nops = 0;
		stmt->ops = 0; 
	}

	uims_tntsql_final_parsing( tntsql);
	return rv;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntdbc_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool DB connector를 생성한다.
 * @param tntdb tarantool DB
 * @param err 실행 결과를 담을 포인터
 * @return 생성된 tarantool DB connector
 */
uims_tntdbc_t* uims_tntdbc_create( uims_tntdb_t *tntdb, int *err)
{
	int rv;
	uims_tntdbc_t *tntdbc;

	tntdbc = (uims_tntdbc_t*)ux_zalloc( ux_mem_default(), sizeof(uims_tntdbc_t));
	if( tntdbc == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_tntdbc_t.");
		if(err) *err = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	rv = uims_tntdbc_init( tntdbc, tntdb);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize uims_tntdbc_t. (err=%d,%s)", 
				rv, uims_dberr_to_str(rv));
		ux_free( ux_mem_default(), tntdbc);
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return tntdbc;
}

/**
 * @internal tarantool DB connector을 생성한다.
 * @param tntdbc tarantool DB connector
 */
void uims_tntdbc_destroy( uims_tntdbc_t *tntdbc)
{
	uims_tntdbc_final( tntdbc);
	ux_free( ux_mem_default(), tntdbc);
}

/**
 * @internal tarantool DB connector를 초기화한다.
 * @param tntdbc tarantool DB connector
 * @param tntdb tranantool DB 정보
 * @return 실행 결과
 */
uims_dberr_e uims_tntdbc_init( uims_tntdbc_t *tntdbc, uims_tntdb_t *tntdb)
{
	int rv;
	struct tnt_stream *net;

	rv = ux_rbtree_init( tntdbc->stmts, ux_compare_str);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_CRT, "Failed to initialze tarantool statement map. (id=%u, name=%s, err=%d,%s)",
				uims_db_get_id(tntdb->db), uims_db_get_name(tntdb->db), rv, ux_errnostr(rv)); 
		return UIMS_DBERR_INVALID_PARAMETER;
	}

	rv = uims_tntsql_init( tntdbc->tntsql);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_CRT, "Failed to initialze tarantool SQL parser. (id=%u, name=%s, err=%d,%s)",
				uims_db_get_id(tntdb->db), uims_db_get_name(tntdb->db), rv, uims_dberr_to_str(rv)); 
		ux_rbtree_final( tntdbc->stmts);
		return rv;
	}

	net = tnt_net( &tntdbc->tnts[0]);
	if(net == NULL) {
		ux_log(UXL_CRT, "Failed to alloc tarantool connection stream[0]."); 
		uims_tntsql_final( tntdbc->tntsql);
		ux_rbtree_final( tntdbc->stmts);
		return UIMS_DBERR_NO_MEMORY;
	}

	net = tnt_net( &tntdbc->tnts[1]);
	if(net == NULL) {
		ux_log(UXL_CRT, "Failed to alloc tarantool connection stream[1]."); 
		tnt_stream_free( &tntdbc->tnts[0]);
		uims_tntsql_final( tntdbc->tntsql);
		ux_rbtree_final( tntdbc->stmts);
		return UIMS_DBERR_NO_MEMORY;
	}

	tntdbc->tntdb = tntdb; 
	uims_tntdbc_load( tntdbc);

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool connector의 사용을 종료한다.
 * @param conn tarantool connector
 * @return 실행 결과
 */
void uims_tntdbc_final( uims_tntdbc_t *tntdbc)
{
	ux_rbtree_iterator_t it;
	ux_rbnode_t* rbnode;
	uims_tntrset_t *tntrset;
	uims_tntstmt_t *tntstmt;
	
	rbnode = ux_rbtree_head( tntdbc->stmts, &it);
	while( rbnode != ux_rbtree_end( tndbc->stmts) ) {
		tntstmt = (uims_tntstmt_t*)rbnode;
		rbnode = ux_rbtree_next( tntdbc->stmts, &it);
		ux_rbtree_remove_node( tntdbc->stmts, tntstmt->rbnode);
		uims_tntstmt_destroy( tntstmt);
	}
	while( tntdbc->freerset) {
		tntrset = tntdbc->freerset;
		tntdbc->freerset = tntrset->next;
		tntrset->next = NULL;
		uims_tntrset_destroy( tntrset);
	}

	tnt_close( &tntdbc->tnts[1]);
	tnt_close( &tntdbc->tnts[0]);
	tnt_stream_free( &tntdbc->tnts[1]);
	tnt_stream_free( &tntdbc->tnts[0]);
	uims_tntsql_final( tntdbc->tntsql);
	ux_rbtree_final( tntdbc->stmts);
}

/**
 * @internal tarantool connector 정보를 다시 로딩한다.
 * @param tntdbc tarantool DB connector
 * @return 실행 결과
 */
uims_dberr_e uims_tntdbc_load( uims_tntdbc_t *tntdbc)
{
	int rv, i;
	char *uri, errstr[512];

	for( i = 0; i < 2; ++i) {
		tntdbc->reconn_time[i] = 0;
		uri = tntdbc->tntdb->uris[i];
		if( uri && uri[0]) {
			if( TNT_SNET_CAST(&tntdbc->tnts[i])->connected) {
				tnt_reload_schema( &tntdbc->tnts[i]);
			} else {
				tnt_set( &tntdbc->tnts[i], TNT_OPT_URI, uri);
				rv = tnt_connect( &tntdbc->tnts[i]);
				if( rv == -1) {
					snprintf(errstr, sizeof(errstr), "connect to %s", uri);
					uims_tntdbc_check_error( tntdbc, &tntdbc->tnts[i], errstr, rv);
				} else {
					ux_log(UXL_INFO, "Succeed to connect to tarantool 'IDX=%d, URI=%s'.", i, uri);
				}
			}
		} else {
			if( TNT_SNET_CAST(&tntdbc->tnts[i])->connected) {
				tnt_close( &tntdbc->tnts[i]);
			}
		}
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal 전달된 tarantool network에서 pending error 발생 시 실제 에러 값을 반환한다.
 * @param tntdbc tarantool DB connector
 * @param net tarantool network
 * @return tarantool error code
  */
int uims_tntdbc_get_error( uims_tntdbc_t *tntdbc, struct tnt_stream_net *net)
{
	int rv, opt;
	socklen_t len;
	fd_set fds;
	ux_time_t start, curr, tmout;

	ux_time_get_current( &start);
	ux_time_set_msec( &tmout, tntdbc->tntdb->error_wait_msec);
	while (1) {
		FD_ZERO( &fds);
		FD_SET( net->fd, &fds);
		rv = select( net->fd + 1, NULL, &fds, NULL, &tmout);
		if (rv == -1) {
			if (errno == EINTR || errno == EAGAIN) {
				ux_time_get_current( &curr);
				ux_time_add_msec( &tmout, &start, tntdbc->tntdb->error_wait_msec);
				if( ux_time_compare( &tmout, &curr) <= 0) return TNT_ETMOUT;
				ux_time_subs( &tmout, &tmout, &curr);
			} else {
				net->errno_ = errno;
				return TNT_ESYSTEM;
			}
		} else if (rv == 0) {
			/* timeout */
			return TNT_ETMOUT;
		} else {
			break;
		}
	}

	opt = 0;
	len = sizeof(opt);
	if( (getsockopt( net->fd, SOL_SOCKET, SO_ERROR, &opt, &len) == -1) || opt) {
		net->errno_ = (opt) ? opt : errno;
		return TNT_ESYSTEM;
	}

	return net->error;
}

/**
 * @internal 전달된 tarantool 함수 호출시 발생한 error를 적절한 error code를 반환한다.
 * @param tntdbc tarantool DB connector
 * @param net tarantool network
 * @return 실행 결과
 */
uims_dberr_e uims_tntdbc_check_error( uims_tntdbc_t *tntdbc, struct tnt_stream *tnt,
						const char *cmdstr, int rslt)
{
	struct tnt_stream_net *net;

	net = TNT_SNET_CAST(tnt);
	if( net->error == TNT_ESYSTEM && net->errno_ == EINPROGRESS) {
		net->error = uims_tntdbc_get_error( tntdbc, net);
	}
	if( net->error != TNT_EOK) {
		ux_log(UXL_MAJ, "Failed to %s. (rslt=%d, error=%d,%s, errno=%d,%s, connected=%s)",
				cmdstr, rslt, net->error, tnt_strerror(tnt), net->errno_, strerror(net->errno_),
				net->connected ? "true" : "false");
	}
	switch( net->error) {
		case TNT_EOK      : return (rslt == -1) ? UIMS_DBERR_INVALID_PARAMETER : UIMS_DB_SUCCESS; 
		case TNT_EFAIL    : return UIMS_DBERR_ERROR;// fail
		case TNT_EMEMORY  : return UIMS_DBERR_NO_MEMORY; // memory allocation failed
		case TNT_ESYSTEM  :
			if(net->connected) {
				tnt_close( tnt);
				return UIMS_DBERR_DISCONNECTED;
			}
			return UIMS_DBERR_SYSTEM;
		case TNT_EBIG     : return UIMS_DBERR_TOO_BIG;     // buffer is too big
		case TNT_ESIZE    : return UIMS_DBERR_INVALID_SIZE;  // buffer size
		case TNT_ERESOLVE : return UIMS_DBERR_INVALID_ADDR; // gethostbyname(2) failed
		case TNT_ETMOUT   : return UIMS_DBERR_TIMEOUT; // operation timeout
		case TNT_EBADVAL  : return UIMS_DBERR_INVALID_VALUE;    // Invalid value
		case TNT_ELOGIN   : return UIMS_DBERR_CANT_ACCESS;    // failed to login
		default : break;
	}

	return UIMS_DBERR_ERROR;
}

/**
 * @internal 우선 순위에 따른 tarantool connection stream을 반환한다.
 * @param tntdbc tarantool DB connector
 * @param err 에러코드를 담을 포인터
 * @return 유효한 taranto connection stream
 */
struct tnt_stream* uims_tntdbc_get_tnt( uims_tntdbc_t *tntdbc, int *err)
{
	int rv, i, idx;
	time_t now;
	char *uri, errstr[512];

	for( i = 0; i < 2; ++i) {
		idx = tntdbc->curidx;
		if( TNT_SNET_CAST(&tntdbc->tnts[idx])->connected) {
			tntdbc->curidx = idx;
			if(err) *err = UIMS_DB_SUCCESS;
			return &tntdbc->tnts[idx];
		}
		now = time(NULL);
		if( tntdbc->reconn_time[idx] + tntdbc->tntdb->reconnect_period < now ) {
			uri = tntdbc->tntdb->uris[idx];
			if( uri && uri[0] ) {
				tnt_set( &tntdbc->tnts[idx], TNT_OPT_URI, uri);
				rv = tnt_connect( &tntdbc->tnts[idx]);
				if( rv == -1) {
					tntdbc->reconn_time[idx] = now;
					snprintf(errstr, sizeof(errstr), "connect to %s", uri);
					uims_tntdbc_check_error( tntdbc, &tntdbc->tnts[idx], errstr, rv);
				} else {
					ux_log(UXL_INFO, "Succeed to connect to tarantool '%s'.", uri);
					tntdbc->curidx = idx;
					if(err) *err = UIMS_DB_SUCCESS;
					return &tntdbc->tnts[idx];
				}
			}
		} else {
			tntdbc->reconn_time[idx] = now;
		}

		idx = (idx == 0) ? 1 : 0;
	}
	if(err) *err = UIMS_DBERR_CANT_CONNECT; 
	return NULL;
}

/**
 * @internal 새로운 result set을 할당해 반환한다.
 * @param tntdbc tarantool DB connector
 * @param err 실행결과를 담을 포인터.
 * @return 할당된 result set. 할당 실패 시 NULL 반환
 */
uims_tntrset_t* uims_tntdbc_alloc_rset( uims_tntdbc_t *tntdbc, int *err)
{
	uims_tntrset_t *tntrset;

	if( tntdbc->freerset) {
		tntrset = tntdbc->freerset;
		tntdbc->freerset = tntrset->next;
		tntrset->next = NULL;
		if(err) *err = UIMS_DB_SUCCESS;
	} else {
		tntrset = uims_tntrset_create( 8192, err);
	}

	return tntrset;
}

/**
 * @internal 전달된 result set를 해제한다.
 * @param tntdbc tarantool DB connector
 * @param tntrset 해제할 tarantool result set
 */
void uims_tntdbc_free_rset( uims_tntdbc_t *tntdbc, uims_tntrset_t *tntrset)
{
	uims_tntrset_reset( tntrset);
	tntrset->next = tntdbc->freerset;
	tntdbc->freerset = tntrset;
}

/**
 * @internal 전달된 statement id와 query문의 statment instace를 반환한다. 
 * @param tntdbc tarantool DB connector
 * @param stmtid statement id
 * @param query statement query 문 
 * @param err 실행결과를 담을 포인터.
 * @return 해당 id의 tarantool statement 
 */
uims_tntstmt_t* uims_tntdbc_get_stmt( uims_tntdbc_t *tntdbc, const char *stmtid,
						const char *query, int *err)
{
	int rv;
	uims_tntstmt_t *tntstmt;

	tntstmt = (uims_tntstmt_t*)ux_rbtree_find( tntdbc->stmts, (void*)stmtid);
	if( tntstmt) {
		if(err) *err = UIMS_DB_SUCCESS;
		return tntstmt;
	}

	tntstmt = uims_tntstmt_create( tntdbc, stmtid, query, &rv);
	if( tntstmt == NULL) {
		if(err) *err = rv;
		return NULL; 
	}

	rv = uims_tntsql_parse( tntdbc->tntsql, tntstmt);
	if( rv < UIMS_DB_SUCCESS) {
		uims_tntstmt_destroy( tntstmt);
		if(err) *err = rv;
		return NULL;
	}

	rv = ux_rbtree_insert( tntdbc->stmts, tntstmt->id, tntstmt->rbnode);
	if( rv < UX_SUCCESS) {
		uims_tntstmt_destroy( tntstmt);
		if(err) *err = UIMS_DBERR_ALREADY_EXISTS;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return tntstmt;
}

/**
 * @internal tarantool socket으로 부터 result set buffer로 값을 읽어 들인다. 
 * @param tntdbc tarantool DB connector
 * @param tnt tarantool connection stream
 * @param rset 수행 결과를 담을 result set
 * @return 실행 결과
 */
int uims_tntdbc_read_reply( uims_tntdbc_t *tntdbc, struct tnt_stream *tnt,
						uims_tntrset_t *rset)
{
	int rv;
	size_t size, hdrlen;
	const char *data;
	struct tnt_reply *r;
	struct tnt_stream_net *tntsn;
	char length[TNT_REPLY_IPROTO_HDR_SIZE];

	if (pm_atomic_load(&tnt->wrcnt) == 0) return 1;
	pm_atomic_fetch_sub(&tnt->wrcnt, 1);

	/* reading iproto header */
	r = rset->reply;
	memset( r, 0 , sizeof(struct tnt_reply));
	data = (const char *)length;
	tntsn = TNT_SNET_CAST(tnt);

	if( tnt_io_recv(tntsn, length, sizeof(length)) == -1) goto rollback;
	if( mp_typeof(*length) != MP_UINT) goto rollback;

	size = mp_decode_uint(&data);
	if( tntdbc->tntdb->check_recv_size && size > tntdbc->tntdb->check_recv_size ) {
		ux_log(UXL_MAJ, "Too big reply message. (rsize=%u, max=%u)",
				(uint32_t)size, tntdbc->tntdb->check_recv_size);
		tntsn->error = TNT_ESYSTEM;
		goto rollback;
	}
	if(size > rset->bufsize && size < tntdbc->tntdb->max_rset_buf_size ) {
		rv = uims_tntrset_resize( rset, size); 
		if( rv < UIMS_DB_SUCCESS) {
			tntsn->error = TNT_ESYSTEM;
			goto rollback;
		}
	}

	r->buf = (size > rset->bufsize) ? tnt_mem_alloc(size) : rset->buffer;
	r->buf = rset->buffer;
	r->buf_size = size;
	if( r->buf == NULL) {
		tntsn->error = TNT_ESYSTEM;
		goto rollback;
	}

	if( tnt_io_recv(tntsn, (char*)r->buf, size) == -1) goto rollback;

	if (tnt_reply_hdr0(r, r->buf, r->buf_size, &hdrlen) != 0)
		goto rollback;
	if (size == hdrlen)
		return 0; /* no body */
	if (tnt_reply_body0(r, r->buf + hdrlen, r->buf_size - hdrlen, NULL) != 0)
		goto rollback;

	return 0;

rollback:
	if( r->buf && rset->buffer != r->buf) {
		tnt_mem_free((void *)r->buf);
		r->buf = NULL;
	}
	memset(r, 0, sizeof(struct tnt_reply));
	return -1;
}

/**
 * @internal request 메시지를 보내고 result를 수신한다.
 * @param tntdbc tarantool DB connector
 * @param tnt tarantool connection stream
 * @param rset 수행 결과를 담을 result set
 * @return 실행 결과
 */
uims_dberr_e uims_tntdbc_execute( uims_tntdbc_t *tntdbc, struct tnt_stream *tnt,
						uims_tntrset_t *rset)
{
	int rv, rcode;

	rv = tnt_flush( tnt);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntdbc, tnt, "flush", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	rv = uims_tntdbc_read_reply( tntdbc, tnt, rset);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntdbc, tnt, "read_reply", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	rcode = uims_tntrset_get_rcode(rset);
	if( rcode != 0) {
		if( rcode == TNT_ER_TUPLE_FOUND) {
			return UIMS_DBERR_ALREADY_EXISTS;
		} else if( rcode == TNT_ER_TUPLE_NOT_FOUND) {
			return UIMS_DBERR_NO_DATA;
		}
		ux_log(UXL_MIN, "tarantool is reply error. (rcode=%d,%s)",
				rcode, uims_tntrcode_to_str(rcode));
		return UIMS_DBERR_ERROR;
	}

	uims_tntrset_apply( rset);

	return UIMS_DB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_tntdbstmt_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool statement를 생성해 반환한다.
 * @param tntdbc 소속된 tarantool db connection
 * @param stmtid statement id
 * @param query statment query
 * @param err 실행 결과를 담을 포인터
 * @return 생성된 tarantool statement
 */
uims_tntstmt_t* uims_tntstmt_create( uims_tntdbc_t *tntdbc, const char *stmtid,
						const char *query, int *err)
{
	int rv;
	uims_tntstmt_t *tntstmt;

	tntstmt = (uims_tntstmt_t*)ux_zalloc( ux_mem_default(), sizeof(uims_tntstmt_t));
	if( tntstmt == NULL) {
		ux_log( UXL_CRT, "Can't allocate memory for uims_tntstmt_t. (stmtid=%s, query=%s)",
				stmtid, query);
		if(err) *err = UIMS_DBERR_NO_MEMORY;
		return NULL;
	}

	rv = uims_tntstmt_init( tntstmt, tntdbc, stmtid, query);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log( UXL_CRT, "Failed to initialize uims_tntstmt_t. (stmtid=%s, query=%s, err=%d,%s)", 
				stmtid, query, rv, uims_dberr_to_str(rv));
		ux_free( ux_mem_default(), tntstmt);
		if(err) *err = rv;
		return NULL;
	}

	if(err) *err = UIMS_DB_SUCCESS;
	return tntstmt;
}

/**
 * @internal tarantool statement를 소멸한다. 
 * @param tntstmt 소멸할 statement
 */ 
void uims_tntstmt_destroy( uims_tntstmt_t *tntstmt)
{
	uims_tntstmt_final( tntstmt);
	ux_free( ux_mem_default(), tntstmt);
}

/**
 * @internal tarantool statement를 초기화 한다. 
 * @param tntstmt 초기화할 tarantool statement
 * @param tntdbc 소속된 tarantool db connection
 * @param stmtid statement id
 * @param query statment query
 * @return 실행 결과 
 */
uims_dberr_e uims_tntstmt_init( uims_tntstmt_t *tntstmt, uims_tntdbc_t *tntdbc,
						const char *stmtid, const char *query)
{
	int ilen, qlen;
	char *buffer;
	
	ilen = strlen(stmtid)+1;
	qlen = strlen(query)+1;
	buffer = ux_alloc( ux_mem_default(), ilen+qlen);
	if( buffer == NULL) {
		return UIMS_DBERR_NO_MEMORY;
	}

	tntstmt->tntdbc = tntdbc;
	tntstmt->id = buffer;
	tntstmt->query = buffer+ilen;
	memcpy( tntstmt->id, stmtid, ilen);
	memcpy( tntstmt->query, query, qlen);

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool statement의 사용을 종료한다.
 * @param tntstmt 초기화할 tarantool statement
 */
void uims_tntstmt_final( uims_tntstmt_t *tntstmt)
{
	int i;

	if( tntstmt->proc) {
		ux_free( ux_mem_default(), tntstmt->proc);
		tntstmt->proc = NULL;
	}
	if( tntstmt->keys) {
		for( i = 0; i < tntstmt->nkeys; ++i) {
			if(tntstmt->keys[i]) uims_tntval_destroy( tntstmt->keys[i], tntstmt->tntdbc->tntsql->valmem);
		}
		ux_free( ux_mem_default(), tntstmt->keys);
		tntstmt->keys = NULL;
	}
	if( tntstmt->ops) {
		for( i = 0; i < tntstmt->nops; ++i) {
			if(tntstmt->ops[i]) uims_tntval_destroy( tntstmt->ops[i], tntstmt->tntdbc->tntsql->valmem);
		}
		ux_free( ux_mem_default(), tntstmt->ops);
		tntstmt->ops = NULL;
	}
	if(tntstmt->id) {
		ux_free(ux_mem_default(), tntstmt->id);
		tntstmt->id = NULL;
		tntstmt->query = NULL;
	}
}

/**
 * @internal tarantool select command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_select( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt,
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara)
{
	int rv, i, psidx, nops;
	uint32_t limit, offset;
	int64_t opval[2];
	struct tnt_stream *key;
	uims_tntval_t *tntval;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set key. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}
	nops = (tntstmt->nops > 2) ? 2 : tntstmt->nops;
	for( i = 0; i < nops; ++i) {
		tntval = tntstmt->ops[i];
		if( tntval ) {
			switch( tntval->type) {
				case UIMS_TNTVTYPE_PARAM:
					opval[i] = uims_dbdataset_get_int( paraset, psidx++, &rv);
					break;
				case UIMS_TNTVTYPE_INT:
					opval[i] = tntval->v.i64;
					rv = UIMS_DB_SUCCESS;
					break;
				default:
					rv = UIMS_DBERR_INVALID_TYPE;
					break;
			}
		} else {
			rv = UIMS_DBERR_INVALID_IDX;
		}
		if( rv < UIMS_DB_SUCCESS) break;
	}
	if( rv < UIMS_DB_SUCCESS) return rv;

	if( nops == 2) {
		offset = opval[0];
		limit = opval[1];
	} else if( nops == 1) {
		offset = 0;
		limit = opval[0];
	} else {
		offset = 0;
		limit = 0x7FFFFFFFU;
	}
	key = uims_tntpara_get_stream( tntpara);

	rv = tnt_select( tnt, tntstmt->space, tntstmt->idxid, limit, offset, tntstmt->iter, key);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, "select", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool insert command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_insert( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt,
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara)
{
	int rv, psidx;
	struct tnt_stream *tuples;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set tuples. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	tuples = uims_tntpara_get_stream( tntpara);

	rv = tnt_insert( tnt, tntstmt->space, tuples);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, "insert", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool replace command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_replace( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara)
{
	int rv, psidx;
	struct tnt_stream *tuples;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set keys. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	tuples = uims_tntpara_get_stream( tntpara);

	rv = tnt_replace( tnt, tntstmt->space, tuples);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, "replace", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool delete command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_delete( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara)
{
	int rv, psidx;
	struct tnt_stream *key;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set keys. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	key = uims_tntpara_get_stream( tntpara);

	rv = tnt_delete( tnt, tntstmt->space, tntstmt->idxid, key);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, "delete", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool update command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @param tntops tarantool operations
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_update( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara,
						uims_tntpara_t *tntops)
{
	int rv, psidx;
	struct tnt_stream *key, *ops;

	psidx = 0;
	rv = uims_tntpara_load_ops( tntops, tntstmt->ops, tntstmt->nops, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set operations. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set keys. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	key = uims_tntpara_get_stream( tntpara);
	ops = uims_tntpara_get_stream( tntops);

	rv = tnt_update( tnt, tntstmt->space, tntstmt->idxid, key, ops);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, "update", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool upsert command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @param tntops tarantool operations
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_upsert( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara,
						uims_tntpara_t *tntops)
{
	int rv, psidx;
	struct tnt_stream *tuple, *ops;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set tuples. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}
	rv = uims_tntpara_load_ops( tntops, tntstmt->ops, tntstmt->nops, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set operations. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	tuple = uims_tntpara_get_stream( tntpara);
	ops = uims_tntpara_get_stream( tntops);

	rv = tnt_upsert( tnt, tntstmt->space, tuple, ops);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, "upsert", rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool call command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_call( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara)
{
	int rv, psidx;
	struct tnt_stream *args;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set arguments. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	args = uims_tntpara_get_stream( tntpara);

	rv = tnt_call( tnt, tntstmt->proc, strlen(tntstmt->proc), args);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, tntstmt->proc, rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool eval command를 수행한다.
 * @param tntstmt tarantool statement
 * @param tnt tarantool connection stream
 * @parma paraset DB parameter set
 * @param tntpara tarantool parameter
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_eval( uims_tntstmt_t *tntstmt, struct tnt_stream *tnt, 
						uims_dbdataset_t *paraset, uims_tntpara_t *tntpara)
{
	int rv, psidx;
	struct tnt_stream *args;

	psidx = 0;
	rv = uims_tntpara_load_keys( tntpara, tntstmt->keys, tntstmt->nkeys, paraset, &psidx);
	if( rv < UIMS_DB_SUCCESS) {
		ux_log(UXL_MAJ, "Failed to set arguments. (stmtid=%s, err=%d,%s)",
				tntstmt->id, rv, uims_dberr_to_str(rv));
		return rv;
	}

	args = uims_tntpara_get_stream( tntpara);

	rv = tnt_eval( tnt, tntstmt->proc, strlen(tntstmt->proc), args);
	if( rv == -1) {
		rv = uims_tntdbc_check_error( tntstmt->tntdbc, tnt, tntstmt->proc, rv);
		if( rv < UIMS_DB_SUCCESS) return rv;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal 전달된 statement를 실행한다.
 * @param tntstmt tarantool statement
 * @param mode 실행 mode
 * @parma paraset DB parameter set
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_execute( uims_tntstmt_t *tntstmt, uint32_t mode,
						uims_dbdataset_t *paraset)
{
	int rv, trycnt;
	struct tnt_stream *tnt;
	uims_tntpara_t tntpara[1], tntops[1];
	char kbuf[8192], obuf[8192];

	for( trycnt = 0; trycnt < 2; ++trycnt) {
		tnt = uims_tntdbc_get_tnt( tntstmt->tntdbc, &rv);
		if( tnt == NULL) return rv;

		uims_tntpara_init( tntpara, kbuf, sizeof(kbuf));
		uims_tntpara_init( tntops, obuf, sizeof(obuf));

		switch( tntstmt->command) {
			case TNT_OP_SELECT : 
				rv = uims_tntstmt_select( tntstmt, tnt, paraset, tntpara);
				break;
			case TNT_OP_INSERT :
				rv = uims_tntstmt_insert( tntstmt, tnt, paraset, tntpara);
				break;
			case TNT_OP_REPLACE :
				rv = uims_tntstmt_replace( tntstmt, tnt, paraset, tntpara);
				break;
			case TNT_OP_UPDATE :
				rv = uims_tntstmt_update( tntstmt, tnt, paraset, tntpara, tntops);
				break;
			case TNT_OP_DELETE :
				rv = uims_tntstmt_delete( tntstmt, tnt, paraset, tntpara);
				break;
			case TNT_OP_EVAL :
				rv = uims_tntstmt_eval( tntstmt, tnt, paraset, tntpara);
				break;
			case TNT_OP_UPSERT :
				rv = uims_tntstmt_upsert( tntstmt, tnt, paraset, tntpara, tntops);
				break;
			case TNT_OP_CALL :
				rv = uims_tntstmt_call( tntstmt, tnt, paraset, tntpara);
				break;
			case TNT_OP_PING :
				rv = tnt_ping( tnt);
				break;
			default :
				ux_log(UXL_MAJ, "Unknown command id. (stmtid=%s, command=%d)", tntstmt->id, tntstmt->command); 
				rv = UIMS_DBERR_INVALID_STATEMENT;
				break;
		}
		if( rv < UIMS_DB_SUCCESS) {
			uims_tntpara_final( tntpara);
			uims_tntpara_final( tntops);
			if( rv == UIMS_DBERR_DISCONNECTED && trycnt == 0) continue;
			return rv; 
		}

		if( tntstmt->tntrset == NULL) {
			tntstmt->tntrset = uims_tntdbc_alloc_rset( tntstmt->tntdbc, &rv);
			if( tntstmt->tntrset == NULL) {
				ux_log(UXL_MAJ, "Failed to allocate result set. (stmtid=%s, err=%d,%s)",
						tntstmt->id, rv, uims_dberr_to_str(rv));
				uims_tntpara_final( tntpara);
				uims_tntpara_final( tntops);
				return rv;
			}
		}

		rv = uims_tntdbc_execute( tntstmt->tntdbc, tnt, tntstmt->tntrset);
		uims_tntpara_final( tntpara);
		uims_tntpara_final( tntops);
		if( rv < UIMS_DB_SUCCESS) {
			if( rv == UIMS_DBERR_DISCONNECTED && trycnt == 0) continue;
			return rv;
		}

		return UIMS_DB_SUCCESS;
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal 전달된 statement의 실행 결과로 부터 하나의 record를 읽어들인다.
 * @param tntstmt tarantool statement
 * @parma rsltset DB result set
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_fetch( uims_tntstmt_t *tntstmt, uims_dbdataset_t *rsltset)
{
	if( tntstmt->tntrset == NULL) {
		ux_log(UXL_MAJ, "Faield to fecth becaue this statement doesn' have result set. (stmtid=%s)",
				tntstmt->id);
		return UIMS_DBERR_INVALID_STATEMENT;
	}

	return uims_tntrset_write( tntstmt->tntrset, rsltset);
}

/**
 * @internal 전달된 statement의 실행 결과로 부터 영향받은 record의 개수를 반환한다. 
 * @param tntstmt tarantool statement
 * @return 실행 결과
 */
ssize_t uims_tntstmt_get_affected_row_count( uims_tntstmt_t *tntstmt)
{
	if( tntstmt->tntrset == NULL) return 0;
	return tntstmt->tntrset->ntuples;
}

/**
 * @internal 전달된 statement의 실행 결과를 삭제한다. 
 * @param tntstmt tarantool statement
 * @return 실행 결과
 */
uims_dberr_e uims_tntstmt_free_result( uims_tntstmt_t *tntstmt)
{
	if( tntstmt->tntrset ) {
		uims_tntdbc_free_rset( tntstmt->tntdbc, tntstmt->tntrset);
		tntstmt->tntrset = NULL;
	}
	return UIMS_DB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// functions for tarantool db module
////////////////////////////////////////////////////////////////////////////////

/**
 * @internal tarantool db를 open 한다.
 * @param db UIMS DB
 * @return 실행 결과
 */
static uims_dberr_e _uims_tntmodule_open_db( uims_db_t *db)
{
	int rv;
	uims_tntdb_t *tntdb;
	const char *confstr;

	confstr = uims_db_get_confstr( db);
	tntdb = uims_tntdb_create( db, confstr, &rv);
	if( tntdb == NULL) {
		ux_log( UXL_CRT, "Failed to create TARANTOOL DB handle. (id=%u, name=%s, confstr=%s, err=%d,%s)", 
				uims_db_get_id(db), uims_db_get_name(db), confstr, rv, uims_dberr_to_str(rv));
		return rv;
	}

	uims_db_set_handle( db, tntdb);
	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool db를 close 한다.
 * @param db UIMS DB
 * @return 실행 결과
 */
static void _uims_tntmodule_close_db( uims_db_t *db)
{
	uims_tntdb_t *tntdb;

	tntdb = (uims_tntdb_t*)uims_db_get_handle( db);

	if(tntdb) uims_tntdb_destroy( tntdb);
}

/**
 * @internal tarantool DB connector를 open 한다.
 * @param dbc UIMS DB connector
 * @return 실행 결과
 */
static uims_dberr_e _uims_tntmodule_open_conn( uims_dbc_t *dbc)
{
	uims_dberr_e rv;
	uims_db_t *db;
	uims_tntdb_t *tntdb;
	uims_tntdbc_t *tntdbc;

	db = (uims_db_t*)uims_dbc_get_db( dbc);
	tntdbc = (uims_tntdbc_t*)uims_dbc_get_handle( dbc);
	tntdb = (uims_tntdb_t*)uims_db_get_handle( db);

	if( tntdbc == NULL) {
		tntdbc = uims_tntdbc_create( tntdb, &rv);
		if( tntdbc == NULL ) {
			ux_log(UXL_CRT, "Failed to uims_tntdbc_create. (id=%u, name=%s, confstr=%s)",
					uims_db_get_id(db), uims_db_get_name(db), uims_db_get_confstr(db));
			return UIMS_DBERR_NO_MEMORY;
		}
		uims_dbc_set_handle( dbc, tntdbc);
	}

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool DB connector를 close 한다.
 * @param dbc UIMS DB connector
 * @return 실행 결과
 */
static void _uims_tntmodule_close_conn( uims_dbc_t *dbc)
{
	uims_tntdbc_t *tntdbc;

	tntdbc = (uims_tntdbc_t*)uims_dbc_get_handle( dbc);

	if(tntdbc) uims_tntdbc_destroy( tntdbc);
}

/**
 * @internal tarantool DB statement를 open 한다.
 * @param dbstmt UIMS DB statement
 * @param stmtid statement id
 * @param query query문
 * @return 실행 결과
 */
static uims_dberr_e _uims_tntmodule_open_stmt( uims_dbstmt_t *dbstmt,
						const char *stmtid, const char *query)
{
	int rv;
	uims_tntdbc_t *tntdbc;
	uims_tntstmt_t *tntstmt;

	tntdbc = (uims_tntdbc_t*)uims_dbstmt_get_dbc_handle( dbstmt);

	tntstmt = uims_tntdbc_get_stmt( tntdbc, stmtid, query, &rv);
	if( tntstmt == NULL) {
		return rv;
	}

	uims_dbstmt_set_handle( dbstmt, tntstmt);

	return UIMS_DB_SUCCESS;
}

/**
 * @internal tarantool DB statement를 close 한다.
 * @param dbstmt UIMS DB statement
 * @return 실행 결과
 */
static void _uims_tntmodule_close_stmt( uims_dbstmt_t *dbstmt)
{
	uims_tntstmt_t *tntstmt;

	tntstmt = (uims_tntstmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( tntstmt == NULL) return;

	uims_tntstmt_free_result( tntstmt);
	uims_dbstmt_set_handle( dbstmt, NULL);
}

/**
 * @internal tarantool DB statement를 실행한다.
 * @param dbstmt UIMS DB statement
 * @return 실행 결과
 */
static uims_dberr_e _uims_tntmodule_execute_stmt( uims_dbstmt_t *dbstmt)
{
	uims_tntstmt_t *tntstmt;

	tntstmt = (uims_tntstmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( tntstmt == NULL) {
		ux_log(UXL_MAJ, "this statement is not open. (handle is NULL)");
		return UIMS_DBERR_INVALID_STATEMENT;
	}

	return uims_tntstmt_execute( tntstmt, uims_dbstmt_get_mode(dbstmt),
					uims_dbstmt_get_paraset( dbstmt));
}

/**
 * @internal tarantool DB statement의 실행결과로 부터 하나의 record 값을 읽어들인다.
 * @param dbstmt UIMS DB statement
 * @return 실행 결과
 */
static uims_dberr_e _uims_tntmodule_fetch_stmt( uims_dbstmt_t *dbstmt)
{
	uims_tntstmt_t *tntstmt;

	tntstmt = (uims_tntstmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( tntstmt == NULL) {
		ux_log(UXL_MAJ, "this statement is not open. (handle is NULL)");
		return UIMS_DBERR_INVALID_STATEMENT;
	}

	return uims_tntstmt_fetch( tntstmt, uims_dbstmt_get_rsltset(dbstmt));
}

/**
 * @internal tarantool DB statement의 실행결과로 영향받은 record의 개수를 반환한다. 
 * @param dbstmt UIMS DB statement
 * @return 실행 결과
 */
static ssize_t _uims_tntmodule_get_affected_row_count( uims_dbstmt_t *dbstmt)
{
	uims_tntstmt_t *tntstmt;

	tntstmt = (uims_tntstmt_t*)uims_dbstmt_get_handle( dbstmt);
	if( tntstmt == NULL) {
		ux_log(UXL_MAJ, "this statement is not open. (handle is NULL)");
		return UIMS_DBERR_INVALID_STATEMENT;
	}

	return uims_tntstmt_get_affected_row_count( tntstmt);
}

/**
 * @internal UIMS tarantool DB module을 반환한다.
 * @return UIMS tarantool DB module을 반환한다.
 */
UX_DECLARE(uims_dbmodule_t*) uims_tntdb_get_module()
{
	static uims_dbmodule_t _g_uims_tntmodule = {
		_uims_tntmodule_open_db,
		_uims_tntmodule_close_db,
		_uims_tntmodule_open_conn,
		_uims_tntmodule_close_conn,
		_uims_tntmodule_open_stmt,
		_uims_tntmodule_close_stmt,
		_uims_tntmodule_execute_stmt,
		_uims_tntmodule_fetch_stmt,
		_uims_tntmodule_get_affected_row_count
	};

	return &_g_uims_tntmodule; 
}

