#ifndef __UIMS_XCPT_H__
#define __UIMS_XCPT_H__

#include "uims_plugin.h"

/**
 * @ingroup uims_xcpt
 * @file
 * @brief UIMS xcript user function ¸ðµâ
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// UIM xcript user functions 
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(int) uims_xcpt_init_diamsg( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params);
UX_DECLARE(int) uims_xcpt_cdr_start( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params);
UX_DECLARE(int) uims_xcpt_cdr_update( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params);
UX_DECLARE(int) uims_xcpt_cdr_end( uxc_sfcall_t *sfcall, uxc_sdmvars_t *params);

#ifdef __cplusplus
}
#endif


#endif // #ifndef __UIMS_XCPT_H__
