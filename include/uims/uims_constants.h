#ifndef __UIMS_CONSTANTS_H__
#define __UIMS_CONSTANTS_H__

/**
 * @ingroup uims_constants
 * @file
 * @brief UIMS 상수 값들
 */

#ifdef __cplusplus
extern "C" {
#endif

/* @brief User-Authorization-Type AVP 값 */
typedef enum uims_authtype_e {
    UIMS_AUTHTYPE_REG = 0, /**< REGISTRATION  */
    UIMS_AUTHTYPE_DEREG = 1, /**< DE_REGISTRATION */
    UIMS_AUTHTYPE_REG_AND_CAPS = 2 /**< REGISTRATION_AND_CAPABILITIES */
} uims_authtype_e;

////////////////////////////////////////////////////////////////////////////////
// UIMS
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // #ifndef __UIMS_CONSTANTS_H__
