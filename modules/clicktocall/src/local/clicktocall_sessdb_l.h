#ifndef __CLICKTOCALL_SESSDB_L_H__
#define __CLICKTOCALL_SESSDB_L_H__

#include <uims/uims_sess.h>
#include "clicktocall_dlgsess_l.h"
#include "clicktocall/clicktocall_conf.h"

////////////////////////////////////////////////////////////////////////////////
// CLICKTOCALL session DB internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief clicktocall session DB 구조체 */
typedef struct clicktocall_sessdb_s clicktocall_sessdb_t; 
/** @brief clicktocall session DB 구조체 */
struct clicktocall_sessdb_s {
	uims_sessdb_t base[1]; /**< IMS session base */
	clicktocall_conf_t *conf; /**< plugin configuration */
	uims_dbmgr_t *dbmgr; /**< uims DB manager */
	clicktocall_dlgdao_t dlgdao[1]; /**< dialog session dao */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for clicktocall_sessdb_t
////////////////////////////////////////////////////////////////////////////////
ux_status_t clicktocall_sessdb_init( clicktocall_sessdb_t *sessdb, clicktocall_conf_t *conf);
void clicktocall_sessdb_final( clicktocall_sessdb_t *sessdb);

#endif // #ifndef __CLICKTOCALL_SESSDB_L_H__
