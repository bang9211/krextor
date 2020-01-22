#ifndef __GW_DLGSVC_H__
#define __GW_DLGSVC_H__

#include "gw_plugin.h"

/**
 * @ingroup gw_dlgsvc
 * @file
 * @brief GW dialog service xcript call 관련 모듈
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// GW INVITE dialog servcie xcript functions 
////////////////////////////////////////////////////////////////////////////////
UX_DECLARE(int) gw_dlgsvc_on_send_outgoing_req( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __GW_DLGSVC_H__