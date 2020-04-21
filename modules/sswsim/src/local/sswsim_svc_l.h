#ifndef __SSWSIM_SVC_L_H__
#define __SSWIM_SVC_L_H__

#include <uims/uims_db.h>
#include <uims/uims_sess.h>
#include <uxlib/ux_def.h>

/** @brief dialog session DAO Ÿ�� */
typedef struct sswsim_dao_s sswsim_dao_t; 
/** @brief diloag session DAO ����ü */
struct sswsim_dao_s {
	uims_db_t *db; /**< �����? DB Ÿ�� */
	uims_dbmgr_t *dbmgr; /** dbmgr */
};

typedef struct sswsim_regist_s sswsim_regist_t;
struct sswsim_regist_s {
    char publicuid[256];
    char privateuid[256];
    char homenetdomain[64];
    uint64_t regdate;
    uint8_t regstatus;
    uint32_t expire;
    char contact1[256];
    char contact2[256];
    char contact3[256];
    char contact4[256];
    char contact5[256];
};

////////////////////////////////////////////////////////////////////////////////
// functions for sswsim_dao_t
////////////////////////////////////////////////////////////////////////////////
ux_status_t sswsim_dao_init( sswsim_dao_t *dao, uims_dbmgr_t *dbmgr);
void sswsim_dao_final( sswsim_dao_t *dao);
sswsim_regist_t* sswsim_dao_find( sswsim_dao_t *dao, char *publicuid);
ux_status_t sswsim_dao_insert( sswsim_dao_t *dao, sswsim_regist_t *regist);
ux_status_t sswsim_dao_remove( sswsim_dao_t *dao, sswsim_regist_t *regist);

#endif // __SSWSIM_SVC_L_H__