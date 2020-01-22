#ifndef __GW_ERROR_H__
#define __GW_ERROR_H__

#include <uxlib/ux_def.h>

/**
 * @ingroup gw_error
 * @file
 * @brief GW error
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// GW service error types & macros
////////////////////////////////////////////////////////////////////////////////

#define GW_ERR_SUCCESS                     0 

#define GW_ERR_BAD_REQUEST                 -40000
#define GW_ERR_NO_IMPI                     -40001 
#define GW_ERR_NO_IMPU                     -40002
#define GW_ERR_NO_VNETID                   -40003 
#define GW_ERR_NO_MALFORMED_CONTACT        -40004

#define GW_ERR_INVALID_SCSCF_URI           -40301
#define GW_ERR_RCV_DIA_ERROR               -40302
#define GW_ERR_AUTHORIZATION_REJECTED      -40303
#define GW_ERR_UNABLE_TO_COMPLY            -40304
#define GW_ERR_RCV_3GPP_ERROR              -40310
#define GW_ERR_USER_UNKNOWN                -40311
#define GW_ERR_IDENTITIES_DONT_MATCH       -40312 
#define GW_ERR_ROAMING_NOT_ALLOWED         -40314
#define GW_ERR_IDENTITY_NOT_REGISTERED     -40315
#define GW_ERR_IDENTITY_ALREADY_REGISTERED -40317 
#define GW_ERR_AUTH_SCHEME_NOT_SUPPORTED   -40318
#define GW_ERR_IN_ASSIGNMENT_TYPE          -40319
#define GW_ERR_TOO_MUCH_DATA               -40320
#define GW_ERR_NOT_SUPPORTED_USER_DATA     -40321
#define GW_ERR_FEATURE_UNSUPPORTED         -40322

#define GW_ERR_DIA_MISSING_AVP             -48001
#define GW_ERR_DIA_TIMEOUT                 -48002
#define GW_ERR_DIA_ERROR                   -48003
#define GW_ERR_DIA_SEND_FAIL               -48004
#define GW_ERR_UNREGISTERED_SERVICE        -48005
#define GW_ERR_SERVER_NAME_NOT_STORED      -48006

#define GW_ERR_NO_MEMORY                   -50000
#define GW_ERR_SERVICE_LOGIC               -50001
#define GW_ERR_SET_AVP                     -50002

#define GW_ERR_TIMEOUT                     -50401

#define GW_ERR_NO_SCSCF                    -60001

#define GW_SIPPHRASE_400_BAD_REQUEST "Bad Request"
#define GW_SIPPHRASE_400_NO_IMPI "Bad Request - Private Identity is missing"
#define GW_SIPPHRASE_400_NO_IMPU "Bad Request - Public Identity is missing"
#define GW_SIPPHRASE_400_NO_VNETID "Bad Request - P-Visited-Network-ID is missing"
#define GW_SIPPHRASE_400_MALFORMED_CONTACT "Bad Request - Malformed Contactr"

#define GW_SIPPHRASE_403_INVALID_SCSCF_URI "Forbidden - Invalid S-CSCF URI"
#define GW_SIPPHRASE_403_RCV_DIA_ERROR "Forbidden - HSS respond DIAMETER Error"
#define GW_SIPPHRASE_403_AUTHORIZATION_REJECTED "Forbidden - HSS Authorization rejected"
#define GW_SIPPHRASE_403_UNABLE_TO_COMPLY "Forbidden - HSS Unable to comply"
#define GW_SIPPHRASE_403_RCV_3GPP_ERROR "Forbidden - HSS respond 3GPP Error result code"
#define GW_SIPPHRASE_403_USER_UNKNOWN "Forbidden - HSS User Unknown"
#define GW_SIPPHRASE_403_IDENTITIES_DONT_MATCH "Forbidden - HSS Identities don't match"
#define GW_SIPPHRASE_403_IDENTITY_NOT_REGISTERED "Forbidden - HSS Identity not registered"
#define GW_SIPPHRASE_403_ROAMING_NOT_ALLOWED "Forbidden - HSS Roaming not allowed"
#define GW_SIPPHRASE_403_IDENTITY_ALREADY_REGISTERED "Forbidden - HSS Identity already registered"
#define GW_SIPPHRASE_403_AUTH_SCHEME_NOT_SUPPORTED "Forbidden - HSS Auth scheme not supported"
#define GW_SIPPHRASE_403_IN_ASSIGNMENT_TYPE "Forbidden - HSS In-Assignment-Type error"
#define GW_SIPPHRASE_403_TOO_MUCH_DATA "Forbidden - HSS Too Much Data"
#define GW_SIPPHRASE_403_NOT_SUPPORTED_USER_DATA "Forbidden - HSS Not supported user data"
#define GW_SIPPHRASE_403_FEATURE_UNSUPPORTED "Forbidden - HSS Feature unsupported"

#define GW_SIPPHRASE_480_DIA_MISSING_AVP "Temporarily Unavailable - DIAMETE Message missing AVP"
#define GW_SIPPHRASE_480_DIA_TIMEOUT "Temporarily Unavailable - DIAMETER Request Timeout"
#define GW_SIPPHRASE_480_DIA_ERROR "Temporarily Unavailable - DIAMETER error"
#define GW_SIPPHRASE_480_DIA_SEND_FAIL "Temporarily Unavailable - DIAMETER sending failed"
#define GW_SIPPHRASE_480_UNREGISTERED_SERVICE "Temporarily Unavailable - Unregisterd service"
#define GW_SIPPHRASE_480_SERVER_NAME_NOT_STORED "Temporarily Unavailable - Server name not stored"

#define GW_SIPPHRASE_500_NO_MEMORY "Server Error - Out of memory"
#define GW_SIPPHRASE_500_SERVICE_LOGIC "Server Error - Failed to run Service Logic"
#define GW_SIPPHRASE_500_SET_AVP "Server Error - Failed to set DIAMETER AVP"

#define GW_SIPPHRASE_504_TIMEOUT "Server Timeout - Timeout message to S-CSCF "

#define GW_SIPPHRASE_600_NO_SCSCF "Busy Everywhere - S-CSCF name is missing"

////////////////////////////////////////////////////////////////////////////////
// GW error functions 
////////////////////////////////////////////////////////////////////////////////

UX_DECLARE(uint32_t) gw_err_to_rcode( int err); 
UX_DECLARE(const char*) gw_err_to_phrase( int err); 


#ifdef __cplusplus
}
#endif

#endif // #ifndef __GW_ERROR_H__
