#ifndef __UIMS_UTIL_H__
#define __UIMS_UTIL_H__

#include <upa/upa_sippa.h>

/**
 * @ingroup uims_util
 * @file
 * @brief UIMS 유틸 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// UIMS utility functions 
////////////////////////////////////////////////////////////////////////////////
int uims_util_get_aor( usip_uri_t *uri, char *buffer, int bufsize);
usip_via_hdr_t *uims_util_get_ue_via( usip_mobj_t *mobj);
int uims_util_get_tport_str( char *protocol, char *buf, int nbuf);
int uims_util_sprint_str( char *buffer, int size, char *title, char *value, int dflt_len);
int uims_util_sprint_int( char *buffer, int size, char *title, int value, int dflt_len);
int uims_util_sprint_u64( char *buffer, int size, char *title, uint64_t value, int dflt_len);
int uims_util_sprint_ttl( char *buffer, int size, char *title);
int uims_util_sprint_ctt( char *buffer, int size, char *title, usip_contact_hdr_t *hdr, int dflt_len);
int uims_util_sprint_hdr( char *buffer, int size, char *title, usip_hdr_t *hdr, int dflt_len);

int uims_util_encode_rroutes( usip_route_hdr_t *rroutes, char *buffer, int buflen, int flags);

#ifdef __cplusplus
}
#endif

#endif //__UIMS_UTIL_H
