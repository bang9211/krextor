#ifndef __SIMCMD_REG_H__
#define __SIMCMD_REG_H__


/** @brief �ִ� 8���� register�� ������ �� �ִ�. */
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


/** @brief simulator commmand register main index�� �����Ѵ�. 
           �ϳ��� simulator���� state ���� �ٸ� command list�� ����� �� �־� 
		   register�� index(0~9)�� �������� ������ ������ �� �ֵ��� �Ѵ�. */
int simcmd_reg_set_mainidx( int idx );

/** @brief simulator command ��� �Լ� */
int simcmd_reg_register( char *name, simcmd_reg_t *reg );

/** @brief simulator command ��� ���� ��ȸ �Լ� */
int simcmd_reg_count();
/** @brief index�� �������� ��ϵ� simulator command�� name�� ��ȸ�ϴ� �Լ� */
char* simcmd_reg_get_name( int idx );
/** @brief name�� �������� ��ϵ� simulator command�� help�� ��ȸ�ϴ� �Լ� */
char* simcmd_reg_get_help( char *name );
/** @brief name�� �������� ��ϵ� simulator command�� ó���� callback function�� ��ȸ�ϴ� �Լ� */
simcmd_proc_f simcmd_reg_get_func( char *name );
/** @brief name�� �������� ��ϵ� simulator command�� group name�� ��ȸ�ϴ� �Լ� */
char* simcmd_reg_get_grpname( char *name );

/** @brief simulator command group ��� ���� ��ȸ �Լ� */
int simcmd_reg_grp_count();
/** @brief index�� �������� ��ϵ� simulator command group�� name�� ��ȸ�ϴ� �Լ� */
char* simcmd_reg_get_grpname_i( int idx );


#endif //#ifndef __SIMCMD_REG_H__
