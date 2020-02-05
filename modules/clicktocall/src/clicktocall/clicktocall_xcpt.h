#ifndef __CLICKTOCALL_XCPT_H__
#define __CLICKTOCALL_XCPT_H__

#include "clicktocall_plugin.h"

/**
 * @ingroup clicktocall_xcpt
 * @file
 * @brief CLICKTOCALL dialog service xcript call 
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL INVITE dialog servcie xcript functions 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(int) clicktocall_xcpt_on_send_outgoing_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __CLICKTOCALL_XCPT_H__