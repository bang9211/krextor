#ifndef __SIMCTRL_SCEN_H__
#define __SIMCTRL_SCEN_H__


#include <stdio.h>
#include <sys/types.h>
#include "simctrl_def.h"


#ifdef __cplusplus 
extern "C" {
#endif 


/////////////////////////////////////////////////////////////////////////////////
// struct & define for simulator controller - scenario manager
/////////////////////////////////////////////////////////////////////////////////

/** @brief define command buffer size */
#define SIMCTRL_COMMAND_SIZE	SIMCTRL_FILE_SIZE	

/** @brief define display mode */
#define SIMCTRL_DISPLAY_IDX		0x0001
#define SIMCTRL_DISPLAY_CLASS 	0x0002
#define SIMCTRL_DISPLAY_NAME	0x0004
#define SIMCTRL_DISPLAY_DESC	0x0008
#define SIMCTRL_DISPLAY_RMI		0x0010
#define SIMCTRL_DISPLAY_CMD		0x0020
#define SIMCTRL_DISPLAY_CR		0x0100 // Carriage Return
#define SIMCTRL_DISPLAY_ALL		0xffff
#define SIMCTRL_DISPLAY_DEFAULT	(SIMCTRL_DISPLAY_IDX | SIMCTRL_DISPLAY_CLASS | \
		SIMCTRL_DISPLAY_NAME | SIMCTRL_DISPLAY_DESC | SIMCTRL_DISPLAY_CR)
#define SIMCTRL_DISPLAY_RESULT (SIMCTRL_DISPLAY_IDX | SIMCTRL_DISPLAY_CLASS | \
		SIMCTRL_DISPLAY_NAME)


/** @brief scenario manager struct type */
typedef struct simctrl_scenmgr_s simctrl_scenmgr_t;
/** @brief scenario group struct type */
typedef struct simctrl_scengrp_s simctrl_scengrp_t;
/** @brief scenario struct type */
typedef struct simctrl_scen_s simctrl_scen_t;
/** @brif scenario rmi struct type */
typedef struct simctrl_scenrmi_s simctrl_scenrmi_t;
/** @brief scenario command struct type */
typedef struct simctrl_scencmd_s simctrl_scencmd_t;
/** @brief scenario configuration struct type */
typedef struct simctrl_scenconf_s simctrl_scenconf_t;

/** @brief scenario command struct */
struct simctrl_scencmd_s {
	char command[SIMCTRL_COMMAND_SIZE]; /**< �ó����� ���� ��ɾ� */
	char check[SIMCTRL_FILE_SIZE]; /**< �ó����� ��� ���� ���� */
	int check_type; /**< ���� ��� üũ Ÿ�� (�̼����� �ó�����Ÿ������ üũ) */
	int display_flag; /**< ȭ�鿡 ���� ������ ������� ���� �÷��� */
	int report_flag; /**< ȭ�鿡 ���� ��� ���� ������� ���� �÷��� */
	int timeout; /**< ���� ��ɾ� ó�� Ÿ�Ӿƿ� �ð� */
	int sleep_time; /**< ���� ��ɾ� ���� ���� ���޽ð��� �� �ð� */
	int serial_flag; /**< �ش� ���� ����� ������ ���� ��ٸ��� ���� �÷��� */
	char remote_host[SIMCTRL_HOST_SIZE]; /**< remote host�� �����Ͽ� ��� ���� (user@host format) */
	char remote_passwd[SIMCTRL_NAME_SIZE]; /**< remote host�� �����Ҷ� ���� �н����� */
	FILE *pipe_fp; /**< popen�� ���� ������ File pointer. scenario command ����� */
};

/** @brief scenario rmi struct */
struct simctrl_scenrmi_s {
	char command[SIMCTRL_COMMAND_SIZE]; /**< rmi ��ɾ� */
	int display_flag; /**< ȭ�鿡 ���� ������ ������� ���� �÷��� */
};

/** @brief scenario struct */
struct simctrl_scen_s {
	char name[SIMCTRL_NAME_SIZE]; /**< �ó����� �� */
	char desc[SIMCTRL_DESC_SIZE]; /**< �ó����� ���� */
	simctrl_scenrmi_t rmis[MAX_SIMCTRL_RMI*2]; /**< �ó����� ���� �� ������ MMC ��ɾ� */
	int nrmi; /**< �ó����� ���� �� ������ MMC ��ɾ� ���� */
	simctrl_scencmd_t cmds[MAX_SIMCTRL_COMMAND]; /**< �ó����� �� Ŀ�ǵ� */
	int ncmd; /**< �ó����� �� Ŀ�ǵ� ���� */ 
	simctrl_scengrp_t *grp; /**< �ó������� �����ϰ� �ִ� �׷� ���� */
	int idx; /**< ��ü �ó����� �� �ڽ��� index �� */
	char report[4096]; /**< �ó����� ���� ��� �̷� �α�. ���п� ���� �� ���� ��� */
};

/** @brief scenario group struct */
struct simctrl_scengrp_s {
	char name[SIMCTRL_NAME_SIZE]; /**< �׷� �� */
	simctrl_scenconf_t *conf; /**< �ó����� �Ŵ������� �����ϰ� �ִ� config ���� */
	simctrl_scen_t scens[MAX_SIMCTRL_SCENARIO]; /**< �ó����� ���� */
	int nscen; /**< �ó����� ���� */
};

/** @brief scenario configuration struct */
struct simctrl_scenconf_s {
	int display_flag; /**< ȭ�鿡 �ó����� ���೻�� ��� ���� (0:off, 1:on) */
	int realtime_display; /**< ȭ�鿡 �ó����� ���೻�� �ǽð� ��� ���� (0:off, 1:on) */
	int file_report_flag; /**< file�� �ó����� ���� ��� ��� ���� (0:off, 1:on) */ 
	int scen_timeout; /**< �ó����� ���� Ÿ�Ӿƿ� �ð�(��) */
	int scen_interval; /**< �ó����� ���� �� ���� �ó����� ���� �� ���� �ð�(��) */
	int scen_type; /**< �ó����� Ÿ�� (seagull/sipp/none) */
	char rmi_host[SIMCTRL_HOST_SIZE]; /**< mmc ���� ȣ��Ʈ�� �Ǵ� IP Address */
	char rmi_port[SIMCTRL_PORT_SIZE]; /**< mmc ���� ��Ʈ ��ȣ */
	char report_plugin[SIMCTRL_FILE_SIZE]; /**< �ó����� ��� �м� �÷����� */ 
	char ignore_keyword[MAX_SIMCTRL_KEYWORD][SIMCTRL_KEYWORD_SIZE]; 
					/**< ��� �м� �� ���̰� �־ ������ ������ keyword ���� */
	int ignore_keyword_cnt; /**< ignore keyword ���� */ 
	char replace_keyword[MAX_SIMCTRL_KEYWORD][SIMCTRL_KEYWORD_SIZE]; 
					/**< ��� �м� �� ����� ������ �������� ������ keyword ���� */
	int replace_keyword_cnt; /**< ignore keyword ���� */ 
	struct {
		char name[SIMCTRL_NAME_SIZE]; 
		char value[SIMCTRL_KEYWORD_SIZE];
	} defines[MAX_SIMCTRL_KEYWORD]; /**< rmi ���� �� ����� keyword�� ���� replace ���� ���� */
	int define_cnt; /**< defined keyword ���� */
};

/** @brief scenario manager struct */
struct simctrl_scenmgr_s {
	int load_flag; /**< �ó����� �ε� ���� */
	simctrl_scenconf_t conf; /**< �ó����� config ���� */
	simctrl_scengrp_t grps[MAX_SIMCTRL_GROUP]; /**< �ó����� �׷� ���� */
	int ngrp; /**< �ε��� �ó����� �׷� ���� */
	int nscen; /**< �ε��� �ó����� ���� */
	void *dll; /**< �ó����� ��� �м� �÷����� ������ */
};


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - scenario manager 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller scenario manager instance */
simctrl_scenmgr_t* simctrl_scenmgr_instance();
/** @brief simulator controller scenario manager load */
int simctrl_scenmgr_load( simctrl_scenmgr_t *mgr, char *xml );
/** @brief simulator controller scenario manager unload */
int simctrl_scenmgr_unload( simctrl_scenmgr_t *mgr, char *gname, char *name );

/** @brief simulator controller scneario manager�� group�� �߰��Ѵ�. */
int simctrl_scenmgr_add_grp( simctrl_scenmgr_t *mgr, simctrl_scengrp_t *grp );
/** @brief simulator controller scneario manager�� group�� �����Ѵ�. */
int simctrl_scenmgr_del_grp( simctrl_scenmgr_t *mgr, char *name );
/** @brief simulator controller scneario manager���� Ư�� group�� ��ȸ�Ѵ�. */
simctrl_scengrp_t* simctrl_scenmgr_get_grp( simctrl_scenmgr_t *mgr, char *name );
/** @brief simulator controller scneario manager���� Ư�� group�� ��ȸ�Ѵ�. */
simctrl_scengrp_t* simctrl_scenmgr_get_grp_i( simctrl_scenmgr_t *mgr, int idx );

/** @brief simulator controller scneario manager�� scenario�� �߰��Ѵ�. */
int simctrl_scenmgr_add_scen( simctrl_scenmgr_t *mgr, char *gname, simctrl_scen_t *scen );
/** @brief simulator controller scneario manager�� scenario�� �����Ѵ�. */
int simctrl_scenmgr_del_scen( simctrl_scenmgr_t *mgr, char *gname, char *name );
/** @brief simulator controller scneario manager���� Ư�� scenario�� ��ȸ�Ѵ�. */
simctrl_scen_t* simctrl_scenmgr_get_scen( simctrl_scenmgr_t *mgr, char *gname, char *name );
/** @brief simulator controller scneario manager���� Ư�� scenario�� ��ȸ�Ѵ�. */
simctrl_scen_t* simctrl_scenmgr_get_scen_i( simctrl_scenmgr_t *mgr, int idx );

/** @brief simulator controller scenario manager�� ������ buffer�� ��� */
int simctrl_scenmgr_sprintf( simctrl_scenmgr_t *mgr, char *buf, int buflen, int mode );
/** @brief simulator controller scenario manager�� ������ ȭ�鿡 ��� */
void simctrl_scenmgr_display( simctrl_scenmgr_t *mgr, int mode ) ;
/** @brief simulator controller scenario ���� ����� ȭ�鿡 ��� */
void simctrl_scenmgr_display_result( simctrl_scenmgr_t *mgr, char *scens, struct timeval stime);


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - scenario group 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller scneario group�� scneario�� �߰��Ѵ�. */
int simctrl_scengrp_add_scen( simctrl_scengrp_t *grp, simctrl_scen_t *scen );
/** @brief simulator controller scneario group�� scneario�� �����Ѵ�. */
int simctrl_scengrp_del_scen( simctrl_scengrp_t *grp, char *name );
/** @brief simulator controller scneario group���� Ư�� scenario�� ��ȸ�Ѵ�. */
simctrl_scen_t* simctrl_scengrp_get_scen( simctrl_scengrp_t *grp, char *name );
/** @brief simulator controller scneario group���� Ư�� scenario�� ��ȸ�Ѵ�. */
simctrl_scen_t* simctrl_scengrp_get_scen_i( simctrl_scengrp_t *grp, int idx );

/** @brief simulator controller scenario group�� ������ buffer�� ��� */
int simctrl_scengrp_sprintf( simctrl_scengrp_t *grp, char *buf, int buflen, int mode );
/** @brief simulator controller scenario group�� ������ ȭ�鿡 ��� */
void simctrl_scengrp_display( simctrl_scengrp_t *grp, int mode );


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - scenario 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller scenario�� ������ buffer�� ��� */
int simctrl_scen_sprintf( simctrl_scen_t *scen, char *buf, int buflen, int mode );
/** @brief simulator controller scenario�� ������ ȭ�鿡 ��� */
void simctrl_scen_display( simctrl_scen_t *scen, int mode );
 
/** @brief simulator controller scenario ���� �� ��� ����Ʈ �Լ� */
int simctrl_scen_proc( simctrl_scen_t *scen );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_SCEN_H__
