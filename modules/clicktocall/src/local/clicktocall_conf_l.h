#ifndef __CLICKTOCALL_CONF_L_H__
#define __CLICKTOCALL_CONF_L_H__

#include "../clicktocall/clicktocall_conf.h"

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL configuration internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief clicktocall configuration ±¸Á¶Ã¼ */
struct clicktocall_conf_s {
	uxc_xcutor_t *xcutor; /**< uxcutor instance */
	upa_sippa_t *sippa; /**< SIP Protocol Adapeter instance */
	uint8_t dflt_privacy; /** Default Privacy mode */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for clicktocall_conf_t 
////////////////////////////////////////////////////////////////////////////////
ux_status_t clicktocall_conf_init( clicktocall_conf_t *conf, const char *cfile);
void clicktocall_conf_final( clicktocall_conf_t *conf);
ux_status_t clicktocall_conf_load( clicktocall_conf_t *conf, const char *cfile);
void clicktocall_conf_print( clicktocall_conf_t *conf);

#endif // #ifndef __CLICKTOCALL_CONF_L_H__
