#ifndef __GW_CONF_L_H__
#define __GW_CONF_L_H__

#include "../gw/gw_conf.h"

////////////////////////////////////////////////////////////////////////////////
// GW configuration internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief gw configuration ±¸Á¶Ã¼ */
struct gw_conf_s {
	uxc_xcutor_t *xcutor; /**< uxcutor instance */
	upa_sippa_t *sippa; /**< SIP Protocol Adapeter instance */
	uint8_t dflt_privacy; /** Default Privacy mode */
	uint8_t use_thig; /**< Usage of THIG */
	uint8_t thig_cipher_mode; /**< Block cipher mode(ECB or CBC) */
	uint8_t thig_aes_type; /**< AES128/192/256 */
	char *thig_confstr; /**< in-domain, address buffer */
	char thig_host[64]; /**< Hostname for tokenized-by */
	char thig_keyfile[1024]; /**< AES key file name for THIG */
	char thig_key[64]; /**< AES key string */
	char rtpp_log[64]; /**< rtpp logger name */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for gw_conf_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t gw_conf_init( gw_conf_t *conf, const char *cfile);
void gw_conf_final( gw_conf_t *conf);
ux_status_t gw_conf_load( gw_conf_t *conf, const char *cfile);
void gw_conf_print( gw_conf_t *conf);

#endif // #ifndef __GW_CONF_L_H__
