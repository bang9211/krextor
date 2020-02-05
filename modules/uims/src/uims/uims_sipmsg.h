#ifndef __UIMS_SIPMSG_H__
#define __UIMS_SIPMSG_H__

#include <upa/upa_sippa.h>
#include "uims_constants.h"

/**
 * @ingroup uims_sipmsg
 * @file
 * @brief UIMS SIP MESSAGE handling ¸ðµâ
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// UIMS SIP MSG handling functions 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(int) uims_sipmsg_check_credential( usip_authorization_hdr_t *hdr,
						const char *realm);
UX_DECLARE(int) uims_sipmsg_read_impu_from_str( const char *str, char *buffer,
						int bufsize);
UX_DECLARE(char*) uims_sipmsg_get_impu_from_str( usip_mobj_t *mobj, const char *str,
						char *buffer, int bufsize, int *err);
UX_DECLARE(int) uims_sipmsg_read_impu( usip_uri_t *uri, char *buffer, int bufsize);
UX_DECLARE(char*) uims_sipmsg_get_impu( usip_mobj_t *mobj, usip_uri_t *uri,
						char *buffer, int bufsize, int *err);
UX_DECLARE(int) uims_sipmsg_read_impi_from_to( usip_mobj_t *mobj, char *buffer,
						int bufsize);
UX_DECLARE(int) uims_sipmsg_read_impi( usip_mobj_t *mobj, const char *realm,
						char *buffer, int bufsize);
UX_DECLARE(char*) uims_sipmsg_get_impi( usip_mobj_t *mobj, const char *realm,
						char *buffer, int bufsize, int *err);
UX_DECLARE(char*) uims_sipmsg_get_visited_network_id( usip_mobj_t *mobj);
UX_DECLARE(uint64_t) uims_sipmsg_get_expires( usip_mobj_t *mobj);
UX_DECLARE(char*) uims_sipmsg_get_hdrstr( usip_hdr_t *hdr, char *buffer,
						int bufsize, ux_mem_t *allocator, int *err);
UX_DECLARE(ux_status_t) uims_sipmsg_set_record_route( usip_mobj_t *mobj,
						usip_route_hdr_t *route);
UX_DECLARE(ux_status_t) uims_sipmsg_set_via( usip_mobj_t *mobj, usip_via_hdr_t *via);

#ifdef __cplusplus
}
#endif

#endif //__UIMS_SIPMSG_H__
