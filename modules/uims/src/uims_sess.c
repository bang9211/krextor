#include "local/uims_sess_l.h"
#include <uxlib/ux_string.h>
#include "uims/uims_plugin.h"

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessid_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 새로운 session id를 생성한다.
 * @param host_id 시스템 host id
 * @param thread_id thread id
 * @param svc_id service id
 * @param tval 현재 시간
 * @param seq 현재 sequence number
 * @return 생성된 unique session id
 */
UX_DECLARE(uims_sessid_t) uims_sessid_create( uint16_t host_id, uint16_t thread_id,
								uint16_t svc_id, time_t tval, uint32_t seq)
{
	uims_sessid_t sess_id;

	sess_id = ((uint64_t)(host_id & UXC_SESSID_HOST_BITMASK)) << (UXC_SESSID_HOST_BITIDX+32);
	sess_id += ((uint64_t)(thread_id & UXC_SESSID_THREAD_BITMASK)) << (UXC_SESSID_THREAD_BITIDX+32);
	sess_id += ((uint64_t)(svc_id & 0xFFF)) << 40;
	sess_id += ((uint64_t)(tval & 0xFFFFFF)) << 16;
	sess_id += (seq & 0xFFFF);

	return sess_id;
}

/**
 * @brief session id 포인터 값을 비교한다.
 * @param a 비교할 포인터
 * @param b 비교할 포인터
 * @return 두 값의 차이
 */
UX_DECLARE(int) uims_sessid_compare( uims_sessid_t* a, uims_sessid_t* b)
{ 
	if( *a == *b) return 0;
	return (*a < *b) ? -1 : 1;
}

/**
 * @brief session id 포인터 값을 비교한다.
 * @param a 비교할 포인터
 * @param b 비교할 포인터
 * @return 두 값의 차이
 */
UX_DECLARE(int) uims_sessid_hash( uims_sessid_t* sessid)
{ 
	return (int)(((uint32_t)(*sessid)) & 0x7FFFFFFF);
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sess_t
////////////////////////////////////////////////////////////////////////////////

ux_status_t uims_sess_final( uims_sess_t *sess)
{
	ux_log(UXL_ALWAYS, "!!!!! IMS session(s=%llu, i=%u, mgr=%p) is destroyed",
			(unsigned long long)sess->id, uxc_sess_get_id(sess->uxcsess), sess->sessmgr);

	if( sess->sessmgr) uims_sessmgr_free( sess->sessmgr, sess);

	return UX_SUCCESS;
}

/**
 * @brief UIMS session의 memory allocator를 반환한다.
 * @param sess UIMS session
 * @return 해당 session의 memory allocator 
 */
UX_DECLARE(ux_mem_t*) uims_sess_get_allocator( uims_sess_t *sess)
{
	return uxc_sess_get_allocator( sess->uxcsess);
}

/**
 * @brief UIMS session의 uxcutor session을 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 uxcutor session 
 */
UX_DECLARE(uxc_sess_t*) uims_sess_get_uxcsess( uims_sess_t *sess)
{
	return sess->uxcsess;
}

/**
 * @brief UIMS session의 uxcutor session header를 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 uxcutor session header 
 */
UX_DECLARE(uxc_sesshdr_t*) uims_sess_get_sesshdr( uims_sess_t *sess)
{
	return uxc_sess_get_hdr(sess->uxcsess);
}

/**
 * @brief UIMS session의 session manager 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 session manager 
 */
UX_DECLARE(uims_sessmgr_t*) uims_sess_get_sessmgr( uims_sess_t *sess)
{
	return sess->sessmgr;
}

/**
 * @brief UIMS session의 session manager 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 session manager 
 */
UX_DECLARE(ux_evtmgr_t*) uims_sess_get_evtmgr( uims_sess_t *sess)
{
	return uxc_sessmgr_get_evtmgr( sess->sessmgr->uxcsessmgr);
}

/**
 * @brief UIMS session의 id 를 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 id 
 */
UX_DECLARE(uims_sessid_t) uims_sess_get_id( uims_sess_t *sess)
{
	return sess->id;
}

/**
 * @brief UIMS session이 속한 host의 id 를 반환한다.
 * @param sess UIMS session
 * @return UIMS session이 속한 host의 id 
 */
UX_DECLARE(uint16_t) uims_sess_get_host_id( uims_sess_t *sess)
{
	return uims_sessid_get_host_id(sess->id);
}

/**
 * @brief UIMS session이 속한 thread의 id 를 반환한다.
 * @param sess UIMS session
 * @return UIMS session이 속한 thread의 id 
 */
UX_DECLARE(uint16_t) uims_sess_get_thread_id( uims_sess_t *sess)
{
	return uims_sessid_get_thread_id(sess->id);
}

/**
 * @brief UIMS session의 service id 를 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 service id 
 */
UX_DECLARE(uint16_t) uims_sess_get_service_id( uims_sess_t *sess)
{
	return uims_sessid_get_service_id(sess->id);
}

/**
 * @brief UIMS session의 call id 값을 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 call id 값 
 */
UX_DECLARE(const char*) uims_sess_get_call_id( uims_sess_t *sess)
{
	return sess->call_id;
}

/**
 * @brief UIMS session의 call id 값을 설정한다. 
 * @param sess UIMS session
 * @param call_id 설정할 call_id 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) uims_sess_set_call_id( uims_sess_t *sess, const char *call_id)
{
	char *value;

	if( call_id == NULL) {
		if( sess->call_id) ux_free( uxc_sess_get_allocator( sess->uxcsess), sess->call_id);
		sess->call_id = NULL;
		return UX_SUCCESS;
	}

	value = ux_str_dup( call_id, uxc_sess_get_allocator( sess->uxcsess));
	if( value == NULL) return UX_ENOMEM;

	if( sess->call_id) ux_free( uxc_sess_get_allocator( sess->uxcsess), sess->call_id);
	sess->call_id = value;
	return UX_SUCCESS;
}

/**
 * @brief UIMS session의 local tag 값을 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 local tag 값 
 */
UX_DECLARE(const char*) uims_sess_get_ltag( uims_sess_t *sess)
{
	return sess->ltag;
}

/**
 * @brief UIMS session의 local tag 값을 설정한다. 
 * @param sess UIMS session
 * @param ltag 설정할 local tag 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) uims_sess_set_ltag( uims_sess_t *sess, const char *ltag)
{
	char *value;

	if( ltag == NULL) {
		if( sess->ltag) ux_free( uxc_sess_get_allocator( sess->uxcsess), sess->ltag);
		sess->ltag = NULL;
		return UX_SUCCESS;
	}

	value = ux_str_dup( ltag, uxc_sess_get_allocator( sess->uxcsess));
	if( value == NULL) return UX_ENOMEM;

	if( sess->ltag) ux_free( uxc_sess_get_allocator( sess->uxcsess), sess->ltag);
	sess->ltag = value;
	return UX_SUCCESS;
}

/**
 * @brief UIMS session의 remote tag 값을 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 remote tag 값 
 */
UX_DECLARE(const char*) uims_sess_get_rtag( uims_sess_t *sess)
{
	return sess->rtag;
}

/**
 * @brief UIMS session의 remote tag 값을 설정한다. 
 * @param sess UIMS session
 * @param rtag 설정할 remote tag 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) uims_sess_set_rtag( uims_sess_t *sess, const char *rtag)
{
	char *value;

	if( rtag == NULL) {
		if( sess->rtag) ux_free( uxc_sess_get_allocator( sess->uxcsess), sess->rtag);
		sess->rtag = NULL;
		return UX_SUCCESS;
	}

	value = ux_str_dup( rtag, uxc_sess_get_allocator( sess->uxcsess));
	if( value == NULL) return UX_ENOMEM;

	if( sess->rtag) ux_free( uxc_sess_get_allocator( sess->uxcsess), sess->rtag);
	sess->rtag = value;
	return UX_SUCCESS;
}

/**
 * @brief UIMS session의 확장 data를 반환한다.
 * @param sess UIMS session
 * @return UIMS session의 확장 data 
 */
UX_DECLARE(void*) uims_sess_get_data( uims_sess_t *sess)
{
	return sess->data;
}

/**
 * @brief UIMS session의 확장 data를 설정한다. 
 * @param sess UIMS session
 * @param data 설정할 data 
 */
UX_DECLARE(void) uims_sess_set_data( uims_sess_t *sess, void *data)
{
	sess->data = data;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessmgr_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief UIMS session manager를 생성한다.
 * @param cdata UIMS session common data
 * @param uxcsessmgr UXCUTOR session manager
 * @return 생성된 UIMS session manager
 */
UX_DECLARE(uims_sessmgr_t*) uims_sessmgr_create( uims_sesscdata_t *cdata, uxc_sessmgr_t *uxcsessmgr)
{
	int rv;
	uims_sessmgr_t *sessmgr;

	sessmgr = ( uims_sessmgr_t*) ux_zalloc( ux_mem_default(), sizeof( uims_sessmgr_t));
	if( sessmgr == NULL) {
		usip_log( USL_CRT, "Can't allocate memory for uims_sessmgr_t.");
		return NULL;
	}

	rv = uims_sessmgr_init( sessmgr, cdata, uxcsessmgr);
	if( rv < USIP_SUCCESS) {
		usip_log( USL_CRT, "Failed to uims_sessmgr_init(). (err=%d,%s)", rv, ux_errnostr(rv));
		ux_free( ux_mem_default(), sessmgr);
		return NULL;
	}

	return sessmgr;	
}

/**
 * @brief UIMS session manager를 소멸한다. 
 * @param sessmgr UIMS session manager
 */
UX_DECLARE(void) uims_sessmgr_destroy( uims_sessmgr_t *sessmgr)
{
	uims_sessmgr_final( sessmgr);
	ux_free( ux_mem_default(), sessmgr);
}

/**
 * @internal UIMS session manager를 초기화한다. 
 * @param cdata UIMS session common data
 * @param uxcsessmgr UXCUTOR session manager
 * @return 생성된 UIMS session manager
 */
ux_status_t uims_sessmgr_init( uims_sessmgr_t *sessmgr, uims_sesscdata_t *cdata,
						uxc_sessmgr_t *uxcsessmgr)
{
	int rv;
	ux_time_t cur[1];

	ux_time_get_current(cur);
	sessmgr->cdata = cdata;
	sessmgr->uxcsessmgr = uxcsessmgr;
	sessmgr->host_id = uxc_sessmgr_get_host_id( uxcsessmgr);
	sessmgr->thread_id = uxc_sessmgr_get_thread_id( uxcsessmgr);
	sessmgr->seqnum = cur->tv_sec / 1000;
	if(sessmgr->seqnum == 0) sessmgr->seqnum++;
	rv = ux_hash_init( sessmgr->sessmap, sessmgr->cdata->conf->sess_hash_bin_size,
			(ux_compare_f)uims_sessid_compare, (ux_hash_f)uims_sessid_hash, ux_mem_default());
	if( rv < UX_SUCCESS) {
		usip_log( USL_MAJ, "Failed to session map for uxc_sessmgr_t. (err=%d,%s)", rv, ux_errnostr(rv));
		return rv;
	}
	rv = ux_hash_init( sessmgr->diamap, sessmgr->cdata->conf->sess_hash_bin_size,
			ux_compare_str, ux_hash_str, ux_mem_default());
	if( rv < UX_SUCCESS) {
		usip_log( USL_MAJ, "Failed to session map for uxc_sessmgr_t. (err=%d,%s)", rv, ux_errnostr(rv));
		return rv;
	}


	uxc_sessmgr_set_user_data( uxcsessmgr, sessmgr); 

	return UX_SUCCESS;
}

/**
 * @internal UIMS session manager의 사용을 종료한다.
 * @param sessmgr UIMS session manager
 */
void uims_sessmgr_final( uims_sessmgr_t *sessmgr)
{
	uims_sessmgr_clear( sessmgr);
	ux_hash_final( sessmgr->sessmap);
	ux_hash_final( sessmgr->diamap);
}

/**
 * @brief UIMS session manager 하위의 session들을 모두 종료한다.
 * @param sessmgr UIMS session manager
 */
UX_DECLARE(void) uims_sessmgr_clear( uims_sessmgr_t *sessmgr)
{
	ux_hnode_t *hnode;
	uims_sess_t *sess;

	ux_hash_clear( sessmgr->diamap);
	hnode = ux_hash_head( sessmgr->sessmap);
	while( hnode) {
		sess = (uims_sess_t*)hnode;
		hnode = ux_hash_next( sessmgr->sessmap, hnode);
		uims_sessmgr_free( sessmgr, sess);
	}
}

/**
 * @brief UIMS session manager로부터 새로운 session을 생성한다.
 * @param sessmgr UIMS session manager
 * @param service 새로운 service
 * @param err 실행 결과를 담을 포인터
 * @return 생성된 session
 */
UX_DECLARE(uims_sess_t*) uims_sessmgr_alloc( uims_sessmgr_t *sessmgr, uxc_service_t *service,
						int *err)
{
	int rv;
	ux_mem_t *allocator;
	uxc_sesshdr_t *sesshdr;
	uxc_sess_t *uxcsess;
	uims_sess_t *sess;

	uxcsess = uxc_sessmgr_alloc( sessmgr->uxcsessmgr, service);
	if ( uxcsess == NULL ) {
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	sesshdr = uxc_sess_get_hdr( uxcsess);
	allocator = uxc_sess_get_allocator( uxcsess);

	sess = ux_zalloc( allocator, sizeof(uims_sess_t));
	if( sess == NULL) {
		uxc_sessmgr_free( sessmgr->uxcsessmgr, uxcsess);
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	sess->uxcsess = uxcsess;
	uxc_sess_set_user_data( uxcsess, sess);
	sess->id = uims_sessid_create( sessmgr->host_id, sessmgr->thread_id,
					sesshdr->svc_id, time(NULL), sessmgr->seqnum++); 
	if(sessmgr->seqnum == 0) sessmgr->seqnum++;

	rv = ux_hash_insert( sessmgr->sessmap, &sess->id, sess->hnode);
	if( rv < USIP_SUCCESS) {
		if(err) *err = rv;
		uxc_sessmgr_free( sessmgr->uxcsessmgr, uxcsess);
		return NULL;
	}
	sess->sessmgr = sessmgr;

	ux_mpool_register_cleanup((ux_mpool_t*)allocator, sess, (ux_mpool_cleanup_f)uims_sess_final);

	if(err) *err = UX_SUCCESS;
	return sess;
}

/**
 * @brief UIMS session manager로부터 전달된 session id의 새로운 session을 생성한다.
 * @param sessmgr UIMS session manager
 * @param service 새로운 service
 * @param sessid 사용할 session id
 * @param err 실행 결과를 담을 포인터
 * @return 생성된 session
 */
UX_DECLARE(uims_sess_t*) uims_sessmgr_alloc_with_id( uims_sessmgr_t *sessmgr,
						uxc_service_t *service, uims_sessid_t sessid, int *err)
{
	int rv;
	ux_mem_t *allocator;
	uxc_sesshdr_t *sesshdr;
	uxc_sess_t *uxcsess;
	uims_sess_t *sess;

	uxcsess = uxc_sessmgr_alloc( sessmgr->uxcsessmgr, service);
	if ( uxcsess == NULL ) {
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	sesshdr = uxc_sess_get_hdr( uxcsess);
	allocator = uxc_sess_get_allocator( uxcsess);

	sess = ux_zalloc( allocator, sizeof(uims_sess_t));
	if( sess == NULL) {
		uxc_sessmgr_free( sessmgr->uxcsessmgr, uxcsess);
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	sess->uxcsess = uxcsess;
	uxc_sess_set_user_data( uxcsess, sess);
	sess->id = sessid;

	rv = ux_hash_insert( sessmgr->sessmap, &sess->id, sess->hnode);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_MAJ, "Aleady exist sesssion. (sessid=%llu, err=%d,%s)",
				(unsigned long long)sess->id, rv, ux_errnostr(rv));
		if(err) *err = rv;
		uxc_sessmgr_free( sessmgr->uxcsessmgr, uxcsess);
		return NULL;
	}
	sess->sessmgr = sessmgr;

	ux_mpool_register_cleanup((ux_mpool_t*)allocator, sess, (ux_mpool_cleanup_f)uims_sess_final);

	if(err) *err = UX_SUCCESS;
	return sess;
}

/**
 * @brief UIMS session manager로부터 session을 소멸한다. 
 * @param sessmgr UIMS session manager
 * @param sess 소멸할 세션 
 */
UX_DECLARE(void) uims_sessmgr_free( uims_sessmgr_t *sessmgr, uims_sess_t *sess)
{
	if( uxc_sessmgr_is_ha_active(sess->sessmgr->uxcsessmgr)) {
		uims_sessdb_remove( sessmgr->cdata->sessdb, sessmgr, sess);
	}
	if( UX_LIST_IS_LINKED(&sess->snode->lnode)) {
		ux_hash_remove_node(sessmgr->diamap, sess->snode);
	}
	ux_hash_remove_node(sessmgr->sessmap, sess->hnode);
	sess->sessmgr->current = NULL;
	sess->sessmgr = NULL;
}

/**
 * @brief UIMS session manager로부터 session을 검색한다. 
 * @param sessmgr UIMS session manager
 * @param sessid 검색할 session id
 * @return 검색된 sesssion
 */
UX_DECLARE(uims_sess_t*) uims_sessmgr_find( uims_sessmgr_t *sessmgr, uims_sessid_t sessid)
{
	uims_sess_t *sess;

	sess = (uims_sess_t*)ux_hash_find( sessmgr->sessmap, &sessid); 
	if( sess ) return sess;

	return uims_sessdb_find( sessmgr->cdata->sessdb, sessmgr, sessid);
}

/**
 * @brief UIMS session manager로부터 전달된 DIAMETER session id의 session을 검색한다. 
 * @param sessmgr UIMS session manager
 * @param dsid 검색할 DIAMETER session id
 * @return 검색된 sesssion
 */
 /*
UX_DECLARE(uims_sess_t*) uims_sessmgr_find_dia( uims_sessmgr_t *sessmgr, const char *dsid)
{
	ux_hnode_t *hnode;

	hnode = ux_hash_find( sessmgr->sessmap, (void*)dsid); 
	if( hnode ) return UX_HASH_ENTRY(hnode, uims_sess_t, snode);

	return uims_sessdb_find_dia( sessmgr->cdata->sessdb, sessmgr, dsid);
}
*/

/**
 * @brief UIMS session manager에 DIAMETER session id로 해당 session을 저장한다.
 * @param sessmgr UIMS session manager
 * @param dsid DIAMETER session id
 * @arapm sess 저장할 session
 * @return 실행 결과 
 */
 /*
UX_DECLARE(ux_status_t) uims_sessmgr_register_dia( uims_sessmgr_t *sessmgr, const char *dsid,
						uims_sess_t *sess)
{
	int rv;

	sess->dsid = ux_str_dup( dsid, uxc_sess_get_allocator(sess->uxcsess));
	if( sess->dsid == NULL) {
		ux_log(UXL_CRT, "Failed to dupliate DIAMETER session id. (dsid=%s)", dsid);
		return UX_ENOMEM;
	}

	rv = ux_hash_insert( sessmgr->diamap, sess->dsid, sess->snode);
	if( rv < USIP_SUCCESS) {
		ux_log(UXL_CRT, "Failed to register DIAMETER session id. (dsid=%s)", dsid);
		return rv;
	}

	return UX_SUCCESS;
}
*/

/**
 * @brief 전달된 id의 service 객체를 반환한다.
 * @param sessmgr UIMS session manager
 * @param svcid service id 
 * @return 해당 service 객체
 */
UX_DECLARE(uxc_service_t*) uims_sessmgr_get_service( uims_sessmgr_t *sessmgr, uint16_t svcid)
{
	return uxc_sessmgr_get_service( sessmgr->uxcsessmgr, svcid);
}

/**
 * @brief 전달된 이름의 service 객체를 반환한다.
 * @param sessmgr UIMS session manager
 * @param svcname service 이름
 * @return 해당 service 객체
 */
UX_DECLARE(uxc_service_t*) uims_sessmgr_find_service( uims_sessmgr_t *sessmgr, const char *svcname)
{
	return uxc_sessmgr_find_service( sessmgr->uxcsessmgr, svcname);
}

/**
 * @brief UIMS session의 전달된 메시지에 대한 서비스 로직을 수행한다. 
 * @param sessmgr UIMS session manager
 * @param sess 실행할 session 
 * @param msg 수신된 메시지
 * @param is_new_sess 새로 생성된 session인지 여부
 * @return 검색된 sesssion
 */
UX_DECLARE(ux_status_t) uims_sessmgr_run( uims_sessmgr_t *sessmgr, uims_sess_t *sess,
						uxc_msg_t *msg, int is_new_sess)
{
	int rv;

	sessmgr->current = sess;

	rv = uxc_sessmgr_run( sessmgr->uxcsessmgr, sess->uxcsess, msg);

	if( sessmgr->current == sess) { 
		if( is_new_sess) uims_sessdb_insert( sessmgr->cdata->sessdb, sessmgr, sess);
		else uims_sessdb_update( sessmgr->cdata->sessdb, sessmgr, sess);
	}
	sessmgr->current = NULL;

	return rv;
}

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessdb_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 전달된 DB로 부터 session을 검색해 반환한다.
 * @param sessdb session DB
 * @param sessmgr 관련 session manager
 * @param sessid 검색할 session id
 * @return 검색 된 session. 없을 경우 NULL 반환
 */
UX_DECLARE(uims_sess_t*) uims_sessdb_find( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sessid_t sessid)
{
	if( sessdb == NULL || sessdb->find == NULL) return NULL;
	return sessdb->find( sessdb, sessmgr, sessid);
}

/**
 * @brief 전달된 DB로 부터 DIAMETER id의 session을 검색해 반환한다.
 * @param sessdb session DB
 * @param sessmgr 관련 session manager
 * @param dsid 검색할 DIAMETER session id
 * @return 검색 된 session. 없을 경우 NULL 반환
 */
UX_DECLARE(uims_sess_t*) uims_sessdb_find_dia( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						const char *dsid)
{
	if( sessdb == NULL || sessdb->find_dia == NULL) return NULL;
	return sessdb->find_dia( sessdb, sessmgr, dsid);
}

/**
 * @brief 전달된 DB에 session을 저장한다.
 * @param sessdb session DB
 * @param sessmgr 관련 session manager
 * @param sess 저장할 session
 * @return 실행 결과 
 */
UX_DECLARE(ux_status_t) uims_sessdb_insert( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sess_t *sess)
{
	if( sessdb == NULL || sessdb->insert == NULL) return UX_SUCCESS;
	return sessdb->insert( sessdb, sessmgr, sess);
}

/**
 * @brief 전달된 DB로부터 session을 삭제한다. 
 * @param sessdb session DB
 * @param sessmgr 관련 session manager
 * @param sess 삭제할 session
 * @return 실행 결과 
 */
UX_DECLARE(ux_status_t) uims_sessdb_remove( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sess_t *sess)
{
	if( sessdb == NULL || sessdb->remove == NULL) return UX_SUCCESS;
	return sessdb->remove( sessdb, sessmgr, sess);
}

/**
 * @brief 전달된 DB에 session을 갱신한다. 
 * @param sessdb session DB
 * @param sessmgr 관련 session manager
 * @param sess 갱신할 session
 * @return 실행 결과 
 */
UX_DECLARE(ux_status_t) uims_sessdb_update( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sess_t *sess)
{
	if( sessdb == NULL || sessdb->update == NULL) return UX_SUCCESS;
	return sessdb->update( sessdb, sessmgr, sess);
}



