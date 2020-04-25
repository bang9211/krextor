#ifndef __SIMCTRL_PROPS_H__
#define __SIMCTRL_PROPS_H__


#include <uxlib/ux_props.h>
#include "simctrl_def.h"


#ifdef __cplusplus 
extern "C" {
#endif 


/////////////////////////////////////////////////////////////////////////////////
// struct & define for simulator controller - properties 
/////////////////////////////////////////////////////////////////////////////////

/** @brief properites xml file structure
    + scenarios
    | + configuartion
    | | + define 
    | | | - name 
    | | | - value 
    | + class 
      | - name 
      | + scenario 
        | - name 
        | - description 
		| + rmi
		| | - command
		| | - display (true/false, default true)
        | + command 
          | - script 
          | - check
  	 	  | - check_type (seagull/sipp/none, default none)
          | - display (true/false, default true)
  		  | - report  (true/false, default true)
		  | - timeout (second, default 0)
		  | - sleep (second, default 0)
  	 	  | - serial  (true/false, default false)
		  | - remote_host (user@host, default null)
		  | - remote_passwd (user's password, default null)
 */

/** @brief properties type */
typedef ux_props_t simctrl_props_t;
/** @brief properties group type */
typedef ux_propgrp_t simctrl_propgrp_t;
/** @brief properties group(configuration) struct type */
typedef struct simctrl_propconf_s simctrl_propconf_t;
/** @brief properties group(class) struct type */
typedef struct simctrl_propclass_s simctrl_propclass_t;
/** @brief properties group(scenario) struct type */
typedef struct simctrl_propscen_s simctrl_propscen_t;
/** @brief properties group(rmi) struct type */
typedef struct simctrl_proprmi_s simctrl_proprmi_t;
/** @brief properties group(command) struct type */
typedef struct simctrl_propcmd_s simctrl_propcmd_t;


/** @brief properties group(configuration) struct */
struct simctrl_propconf_s {
	simctrl_propgrp_t *propgrp; /**< own property group instance */
	int display_trace_log; /**< ȭ�鿡 �ó����� ���� ���� ��� ���� (0:off, 1:on) */
	int realtime_display; /**< �ó����� ���� ���� �ǽð� ��� ���� (0:off, 1:on) */
	int file_report; /**< file�� �ó����� ���� ��� ��� ���� (0:off, 1:on) */ 
	int scenario_timeout; /**< �ó����� ���� Ÿ�Ӿƿ� �ð�(��) */
	int scenario_interval; /**< �ó����� ���� �� ���� �ó����� ���� �� ���� �ð�(��) */
	int scenario_type; /**< �ó����� Ÿ�� (seagull/sipp/none) */
	const char *rmi_host; /**< mmcd ���� ȣ��Ʈ�� �Ǵ� IP Address */
	const char *rmi_port; /**< mmcd ���� ��Ʈ ��ȣ */ 
	const char *report_plugin; /**< �ó����� ��� �м� �÷����� */
	const char *ignore_keyword[MAX_SIMCTRL_KEYWORD];
			/**< ��� �м� �� ���̰� �־ ������ ������ keyword ���� */
	int ignore_keyword_cnt; /**< ignore keyword ���� */
	const char *replace_keyword[MAX_SIMCTRL_KEYWORD];
			/**< ��� �м� �� ����� ������ �������� ������ keyword ���� */
	int replace_keyword_cnt;
	struct {
		const char *name; 
		const char *value;
	} defines[MAX_SIMCTRL_KEYWORD]; /**< rmi ���� �� ����� keyword�� ���� replace ���� ���� */
	int define_cnt; /**< defined keyword ���� */
};

/** @brief properties group(class) struct */
struct simctrl_propclass_s {
	simctrl_propgrp_t *propgrp; /**< own property group instance */
	const char *name; /**< class name */
};

/** @brief properties group(scenario) struct */
struct simctrl_propscen_s {
	simctrl_propgrp_t *propgrp; /**< own property group instance */
	const char *name; /**< scenario name*/ 
	const char *desc; /**< scenario description */
};

/** @brief properties group(rmi) struct */
struct simctrl_proprmi_s {
	simctrl_propgrp_t *propgrp; /**< own property group instance */
	const char *command; /**< rmi ��ɾ� */
	int display; /**< ȭ�鿡 ���� ������ ������� ���� �÷��� */
};

/** @brief properties group(command) struct */
struct simctrl_propcmd_s {
	simctrl_propgrp_t *propgrp; /**< own property group instance */
	const char *script; /**< �ó����� ���� ��ɾ� */ 
	const char *check; /**<�ó����� ��� ���� ���� */
	int check_type; /**< ���� ��� üũ Ÿ�� (�̼����� �ó�����Ÿ������ üũ) */
	int display; /**< ȭ�鿡 ���� ������ ������� ���� �÷��� */
	int report; /**< ȭ�鿡 ���� ��� ���� ������� ���� �÷��� */
	int timeout; /**< ���� ��ɾ� Ÿ�Ӿƿ� �ð� */
	int sleep; /**< ���� ��ɾ� ���� ���� ���޽ð��� �� �ð� */
	int serial; /**< �ش� ���� ����� ������ ���� ��ٸ��� ���� �÷��� */
	const char *remote_host; /**< remote host�� �����Ͽ� ��� ���� (user@host format) */
	const char *remote_passwd; /**< remote host�� �����Ҷ� ���� �н����� */
};


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - properties 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller properties open
          xml ������ ������ ���� �Ľ��ϰ� �Ľ� ������ props instance�� �����Ѵ�. */
int simctrl_props_open( simctrl_props_t *props, char *fname);
/** @brief simulator controller properties close */
void simctrl_props_close( simctrl_props_t *props );

/** @brief simulator controller properties���� configuration ��ȸ �Լ� */
int simctrl_props_get_configuration( simctrl_props_t *props, simctrl_propconf_t *conf);
/** @brief simulator controller properties���� ������ class ���� Ȯ�� �Լ� */
int simctrl_props_get_class_count( simctrl_props_t *props );
/** @brief simulator controller properties���� index��° class ��ȸ �Լ� */
int simctrl_props_get_class( simctrl_props_t *props, int idx, simctrl_propclass_t *class);

/** @brief simulator controller class properties���� ������ rmi ���� Ȯ�� �Լ� */
int simctrl_propclass_get_rmi_count( simctrl_propclass_t *class );
/** @brief simulator controller class properties���� index��° rmi ��ȸ �Լ� */
int simctrl_propclass_get_rmi( simctrl_propclass_t *class, int idx, simctrl_proprmi_t *rmi);

/** @brief simulator controller class properties���� ������ scenario ���� Ȯ�� �Լ� */
int simctrl_propclass_get_scenario_count( simctrl_propclass_t *class );
/** @brief simulator controller class properties���� index��° scenario ��ȸ �Լ� */
int simctrl_propclass_get_scenario( simctrl_propclass_t *class, int idx, simctrl_propscen_t *scen);

/** @brief simulator controller scenario properties���� ������ rmi ���� Ȯ�� �Լ� */
int simctrl_propscen_get_rmi_count( simctrl_propscen_t *scen );
/** @brief simulator controller scenario properties���� index��° rmi ��ȸ �Լ� */
int simctrl_propscen_get_rmi( simctrl_propscen_t *scen, int idx, simctrl_proprmi_t *rmi );

/** @brief simulator controller scenario properties���� ������ command ���� Ȯ�� �Լ� */
int simctrl_propscen_get_command_count( simctrl_propscen_t *scen );
/** @brief simulator controller scenario properties���� index��° command ��ȸ �Լ� */
int simctrl_propscen_get_command( simctrl_propscen_t *scen, int idx, simctrl_propcmd_t *cmd );

/** @brief properties group���� Ư�� field�� ���� ��(integer)�� ���ϴ� �Լ� */
int simctrl_propgrp_get_int( simctrl_propgrp_t *grp, const char *name, int dft );
/** @brief properties group���� Ư�� field�� ���� ��(string)�� ���ϴ� �Լ� */
char* simctrl_propgrp_get_str( simctrl_propgrp_t *grp, const char *name, 
		char *val, int len, const char *dft );
/** @brief properties group���� name field�� ���� ��(string)�� ���ϴ� �Լ� */
char *simctrl_propgrp_get_name( simctrl_propgrp_t *grp, char *val, int len );
/** @brief properties group���� description field�� ���� ��(string)�� ���ϴ� �Լ� */
char *simctrl_propgrp_get_desc( simctrl_propgrp_t *grp, char *val, int len );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_PROPS_H__
