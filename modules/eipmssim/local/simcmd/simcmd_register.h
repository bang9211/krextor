#ifndef __SIMCMD_REG_H__
#define __SIMCMD_REG_H__


/** @brief 최대 8개의 register를 관리할 수 있다. */
#define MAX_SIMCMD_REG_COUNT	10	


/** @brief simulator command register type */
typedef struct simcmd_reg_s simcmd_reg_t;
/** @brief simulator command processing callback function type */ 
typedef int (*simcmd_proc_f)(int, char**); // argc, argv

/** @brief simulator command register struct */
struct simcmd_reg_s {
	char *name; /**< simulator command name */
	simcmd_proc_f func; /**< simulator command processing callback funciton */
	char *help; /**< simulator command help */
};


/** @brief simulator commmand register main index를 설정한다. 
           하나의 simulator에서 state 별로 다른 command list를 사용할 수 있어 
		   register를 index(0~9)를 기준으로 복수개 관리할 수 있도록 한다. */
int simcmd_reg_set_mainidx( int idx );

/** @brief simulator command 등록 함수 */
int simcmd_reg_register( char *name, simcmd_reg_t *reg );

/** @brief simulator command 등록 개수 조회 함수 */
int simcmd_reg_count();
/** @brief index를 기준으로 등록된 simulator command의 name을 조회하는 함수 */
char* simcmd_reg_get_name( int idx );
/** @brief name을 기준으로 등록된 simulator command의 help를 조회하는 함수 */
char* simcmd_reg_get_help( char *name );
/** @brief name을 기준으로 등록된 simulator command를 처리할 callback function을 조회하는 함수 */
simcmd_proc_f simcmd_reg_get_func( char *name );
/** @brief name을 기준으로 등록된 simulator command의 group name을 조회하는 함수 */
char* simcmd_reg_get_grpname( char *name );

/** @brief simulator command group 등록 개수 조회 함수 */
int simcmd_reg_grp_count();
/** @brief index를 기준으로 등록된 simulator command group의 name을 조회하는 함수 */
char* simcmd_reg_get_grpname_i( int idx );


#endif //#ifndef __SIMCMD_REG_H__
