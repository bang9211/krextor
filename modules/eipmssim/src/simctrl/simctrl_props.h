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
	int display_trace_log; /**< 화면에 시나리오 실행 내용 출력 여부 (0:off, 1:on) */
	int realtime_display; /**< 시나리오 실행 내용 실시간 출력 여부 (0:off, 1:on) */
	int file_report; /**< file에 시나리오 수행 결과 기록 여부 (0:off, 1:on) */ 
	int scenario_timeout; /**< 시나리오 수행 타임아웃 시간(초) */
	int scenario_interval; /**< 시나리오 수행 후 다음 시나리오 수행 전 유휴 시간(초) */
	int scenario_type; /**< 시나리오 타입 (seagull/sipp/none) */
	const char *rmi_host; /**< mmcd 연결 호스트명 또는 IP Address */
	const char *rmi_port; /**< mmcd 연결 포트 번호 */ 
	const char *report_plugin; /**< 시나리오 결과 분석 플러그인 */
	const char *ignore_keyword[MAX_SIMCTRL_KEYWORD];
			/**< 결과 분석 시 차이가 있어도 무시할 내용의 keyword 정보 */
	int ignore_keyword_cnt; /**< ignore keyword 개수 */
	const char *replace_keyword[MAX_SIMCTRL_KEYWORD];
			/**< 결과 분석 시 예약된 값으로 변경해줄 내용의 keyword 정보 */
	int replace_keyword_cnt;
	struct {
		const char *name; 
		const char *value;
	} defines[MAX_SIMCTRL_KEYWORD]; /**< rmi 수행 시 예약된 keyword에 대해 replace 해줄 정보 */
	int define_cnt; /**< defined keyword 개수 */
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
	const char *command; /**< rmi 명령어 */
	int display; /**< 화면에 진행 정보를 출력할지 여부 플래그 */
};

/** @brief properties group(command) struct */
struct simctrl_propcmd_s {
	simctrl_propgrp_t *propgrp; /**< own property group instance */
	const char *script; /**< 시나리오 실행 명령어 */ 
	const char *check; /**<시나리오 결과 검증 파일 */
	int check_type; /**< 실행 결과 체크 타입 (미설정시 시나리오타입으로 체크) */
	int display; /**< 화면에 진행 정보를 출력할지 여부 플래그 */
	int report; /**< 화면에 진행 결과 값을 출력할지 여부 플래그 */
	int timeout; /**< 실행 명령어 타임아웃 시간 */
	int sleep; /**< 실행 명령어 시작 전에 유휴시간을 줄 시간 */
	int serial; /**< 해당 실행 명령이 끝날때 까지 기다릴지 여부 플래그 */
	const char *remote_host; /**< remote host에 접속하여 명령 실행 (user@host format) */
	const char *remote_passwd; /**< remote host에 접속할때 사용될 패스워드 */
};


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - properties 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller properties open
          xml 형식의 파일을 열어 파싱하고 파싱 내역을 props instance에 설정한다. */
int simctrl_props_open( simctrl_props_t *props, char *fname);
/** @brief simulator controller properties close */
void simctrl_props_close( simctrl_props_t *props );

/** @brief simulator controller properties에서 configuration 조회 함수 */
int simctrl_props_get_configuration( simctrl_props_t *props, simctrl_propconf_t *conf);
/** @brief simulator controller properties에서 설정된 class 개수 확인 함수 */
int simctrl_props_get_class_count( simctrl_props_t *props );
/** @brief simulator controller properties에서 index번째 class 조회 함수 */
int simctrl_props_get_class( simctrl_props_t *props, int idx, simctrl_propclass_t *class);

/** @brief simulator controller class properties에서 설정된 rmi 개수 확인 함수 */
int simctrl_propclass_get_rmi_count( simctrl_propclass_t *class );
/** @brief simulator controller class properties에서 index번째 rmi 조회 함수 */
int simctrl_propclass_get_rmi( simctrl_propclass_t *class, int idx, simctrl_proprmi_t *rmi);

/** @brief simulator controller class properties에서 설정된 scenario 개수 확인 함수 */
int simctrl_propclass_get_scenario_count( simctrl_propclass_t *class );
/** @brief simulator controller class properties에서 index번째 scenario 조회 함수 */
int simctrl_propclass_get_scenario( simctrl_propclass_t *class, int idx, simctrl_propscen_t *scen);

/** @brief simulator controller scenario properties에서 설정된 rmi 개수 확인 함수 */
int simctrl_propscen_get_rmi_count( simctrl_propscen_t *scen );
/** @brief simulator controller scenario properties에서 index번째 rmi 조회 함수 */
int simctrl_propscen_get_rmi( simctrl_propscen_t *scen, int idx, simctrl_proprmi_t *rmi );

/** @brief simulator controller scenario properties에서 설정된 command 개수 확인 함수 */
int simctrl_propscen_get_command_count( simctrl_propscen_t *scen );
/** @brief simulator controller scenario properties에서 index번째 command 조회 함수 */
int simctrl_propscen_get_command( simctrl_propscen_t *scen, int idx, simctrl_propcmd_t *cmd );

/** @brief properties group에서 특정 field에 대한 값(integer)을 구하는 함수 */
int simctrl_propgrp_get_int( simctrl_propgrp_t *grp, const char *name, int dft );
/** @brief properties group에서 특정 field에 대한 값(string)을 구하는 함수 */
char* simctrl_propgrp_get_str( simctrl_propgrp_t *grp, const char *name, 
		char *val, int len, const char *dft );
/** @brief properties group에서 name field에 대한 값(string)을 구하는 함수 */
char *simctrl_propgrp_get_name( simctrl_propgrp_t *grp, char *val, int len );
/** @brief properties group에서 description field에 대한 값(string)을 구하는 함수 */
char *simctrl_propgrp_get_desc( simctrl_propgrp_t *grp, char *val, int len );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_PROPS_H__
