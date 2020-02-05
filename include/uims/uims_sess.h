#ifndef __UIMS_SESS_H__
#define __UIMS_SESS_H__

#include "uims_def.h"
#include "uims_conf.h"
#include <uxlib/ux_hash.h>
#include <uxcutor/uxc_sessmgr.h>
#include <upa/upa_sippa.h>

/**
 * @ingroup uims_sess
 * @file
 * @brief ICSCF session 관련 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// ICSCF session 관련 type 및 macro 
////////////////////////////////////////////////////////////////////////////////

/** @brief NULL IMS session id  */
#define UIMS_SESSID_NULL 0 
/** @brief session id로 부터 host-id를 반환한다. */
#define uims_sessid_get_host_id(id) (((id) >> (UXC_SESSID_HOST_BITIDX+32)) & UXC_SESSID_HOST_BITMASK)
/** @brief session id로 부터 thread-id를 반환한다. */
#define uims_sessid_get_thread_id(id) (((id) >> (UXC_SESSID_THREAD_BITIDX+32)) & UXC_SESSID_THREAD_BITMASK)
/** @brief session id로 부터 service-id를 반환한다. */
#define uims_sessid_get_service_id(id) (((id) >> 40) & 0xFFF)
/** @brief session id로 부터 time-id를 반환한다. */
#define uims_sessid_get_time_id(id) (((id) >> 16) & 0xFFFFFF)
/** @brief session id로 부터 sequnce number를 반환한다. */
#define uims_sessid_get_seqnum(id) ((id) & 0xFFFF)

/** @brief UIMS session id 타입 */
typedef uint64_t uims_sessid_t;

/** @brief UIMS session 타입 */ 
typedef struct uims_sess_s uims_sess_t; 

/** @brief UIMS session common data 타입 */
typedef struct uims_sesscdata_s uims_sesscdata_t;

/** @brief UIMS session manager 타입 */
typedef struct uims_sessmgr_s uims_sessmgr_t; 

/** @brief UIMS session DB interface 타입 */
typedef struct uims_sessdb_s uims_sessdb_t; 
/** @brief UIMS session DB interface 구조체 */
struct uims_sessdb_s {
	uims_sess_t* (*find)( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr, uims_sessid_t sessid);
	uims_sess_t* (*find_dia)( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr, const char *dsid);
	ux_status_t (*insert)( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr, uims_sess_t *sess);
	ux_status_t (*remove)( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr, uims_sess_t *sess);
	ux_status_t (*update)( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr, uims_sess_t *sess);
};

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessid_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_sessid_t) uims_sessid_create( uint16_t host_id, uint16_t thread_id,
								uint16_t svc_id, time_t tval, uint32_t seq);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sess_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(ux_mem_t*) uims_sess_get_allocator( uims_sess_t *sess);
UX_DECLARE(uxc_sess_t*) uims_sess_get_uxcsess( uims_sess_t *sess);
UX_DECLARE(uxc_sesshdr_t*) uims_sess_get_sesshdr( uims_sess_t *sess);
UX_DECLARE(uims_sessmgr_t*) uims_sess_get_sessmgr( uims_sess_t *sess);
UX_DECLARE(ux_evtmgr_t*) uims_sess_get_evtmgr( uims_sess_t *sess);
UX_DECLARE(uims_sessid_t) uims_sess_get_id( uims_sess_t *sess);
UX_DECLARE(uint16_t) uims_sess_get_host_id( uims_sess_t *sess);
UX_DECLARE(uint16_t) uims_sess_get_thread_id( uims_sess_t *sess);
UX_DECLARE(uint16_t) uims_sess_get_service_id( uims_sess_t *sess);
UX_DECLARE(const char*) uims_sess_get_call_id( uims_sess_t *sess);
UX_DECLARE(ux_status_t) uims_sess_set_call_id( uims_sess_t *sess, const char *call_id);
UX_DECLARE(const char*) uims_sess_get_ltag( uims_sess_t *sess);
UX_DECLARE(ux_status_t) uims_sess_set_ltag( uims_sess_t *sess, const char *ltag);
UX_DECLARE(const char*) uims_sess_get_rtag( uims_sess_t *sess);
UX_DECLARE(ux_status_t) uims_sess_set_rtag( uims_sess_t *sess, const char *rtag);
UX_DECLARE(void*) uims_sess_get_data( uims_sess_t *sess);
UX_DECLARE(void) uims_sess_set_data( uims_sess_t *sess, void *data);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessmgr_t; 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_sessmgr_t*) uims_sessmgr_create( uims_sesscdata_t *cdata, uxc_sessmgr_t *uxcsessmgr);
UX_DECLARE(void) uims_sessmgr_destroy( uims_sessmgr_t *sessmgr);
UX_DECLARE(void) uims_sessmgr_clear( uims_sessmgr_t *sessmgr);

UX_DECLARE(uims_sess_t*) uims_sessmgr_alloc( uims_sessmgr_t *sessmgr, uxc_service_t *service,
						int *err);
UX_DECLARE(uims_sess_t*) uims_sessmgr_alloc_with_id( uims_sessmgr_t *sessmgr,
						uxc_service_t *service, uims_sessid_t sessid, int *err);
UX_DECLARE(void) uims_sessmgr_free( uims_sessmgr_t *sessmgr, uims_sess_t *sess);
UX_DECLARE(uims_sess_t*) uims_sessmgr_find( uims_sessmgr_t *sessmgr, uims_sessid_t sessid);
UX_DECLARE(uims_sess_t*) uims_sessmgr_find_dia( uims_sessmgr_t *sessmgr, const char *dsid);
UX_DECLARE(ux_status_t) uims_sessmgr_register_dia( uims_sessmgr_t *sessmgr, const char *dsid,
						uims_sess_t *sess);
UX_DECLARE(uxc_service_t*) uims_sessmgr_get_service( uims_sessmgr_t *sessmgr, uint16_t svcid);
UX_DECLARE(uxc_service_t*) uims_sessmgr_find_service( uims_sessmgr_t *sessmgr, const char *svcname);
UX_DECLARE(ux_status_t) uims_sessmgr_run( uims_sessmgr_t *sessmgr, uims_sess_t *sess,
						uxc_msg_t *msg, int is_new_sess);

////////////////////////////////////////////////////////////////////////////////
// functions for uims_sessdb_t
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(uims_sess_t*) uims_sessdb_find( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sessid_t sessid);
UX_DECLARE(uims_sess_t*) uims_sessdb_find_dia( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						const char *dsid);
UX_DECLARE(ux_status_t) uims_sessdb_insert( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sess_t *sess);
UX_DECLARE(ux_status_t) uims_sessdb_remove( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sess_t *sess);
UX_DECLARE(ux_status_t) uims_sessdb_update( uims_sessdb_t *sessdb, uims_sessmgr_t *sessmgr,
						uims_sess_t *sess);

#ifdef __cplusplus
}
#endif


#endif
