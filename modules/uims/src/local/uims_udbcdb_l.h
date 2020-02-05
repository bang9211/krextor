#ifndef __UIMS_UDBCDB_L_H__
#define __UIMS_UDBCDB_L_H__

#include "../uims/uims_db.h"
#include <uxlib/ux_memory.h>
#include <uxlib/ux_log.h>
#include <udbc.h>

////////////////////////////////////////////////////////////////////////////////
// UIMS UDBC DB internal types & definitions
////////////////////////////////////////////////////////////////////////////////

/** @brief UIMS UDBC DB 타입 */
typedef struct uims_udbcdb_s uims_udbcdb_t; 
/** @brief UIMS UDBC DB 구조체 */
struct uims_udbcdb_s {
	uims_db_t *db; /**< UIMS DB */
	const char *confstr; /**< configuration string */
	udbc_module_t *module; /**< UDBC module */
	udbc_conf_t *conf; /**< UDBC configuration */
};

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_udbcdb_t
////////////////////////////////////////////////////////////////////////////////
uims_dberr_e uims_udbcdb_init( uims_udbcdb_t *udbcdb, uims_db_t *db, const char *dllname,
						const char *confstr);
void uims_udbcdb_final( uims_udbcdb_t *udbcdb);


#endif
