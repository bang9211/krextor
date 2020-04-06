#ifndef __SSWSIM_CONF_L_H__
#define __SSWSIM_CONF_L_H__

#include "../sswsim/sswsim_conf.h"

////////////////////////////////////////////////////////////////////////////////
// SSWSIM configuration internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief sswsim configuration ±¸Á¶Ã¼ */
struct sswsim_conf_s {
	uxc_xcutor_t *xcutor; /**< uxcutor instance */
	upa_sippa_t *sippa; /**< SIP Protocol Adapeter instance */
	uint8_t dflt_privacy; /** Default Privacy mode */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for sswsim_conf_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t sswsim_conf_init( sswsim_conf_t *conf, const char *cfile);
void sswsim_conf_final( sswsim_conf_t *conf);
ux_status_t sswsim_conf_load( sswsim_conf_t *conf, const char *cfile);
void sswsim_conf_print( sswsim_conf_t *conf);

#endif // #ifndef __SSWSIM_CONF_L_H__
