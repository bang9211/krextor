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
	char command[SIMCTRL_COMMAND_SIZE]; /**< 시나리오 실행 명령어 */
	char check[SIMCTRL_FILE_SIZE]; /**< 시나리오 결과 검증 파일 */
	int check_type; /**< 실행 결과 체크 타입 (미설정시 시나리오타입으로 체크) */
	int display_flag; /**< 화면에 진행 정보를 출력할지 여부 플래그 */
	int report_flag; /**< 화면에 진행 결과 값을 출력할지 여부 플래그 */
	int timeout; /**< 실행 명령어 처리 타임아웃 시간 */
	int sleep_time; /**< 실행 명령어 시작 전에 유휴시간을 줄 시간 */
	int serial_flag; /**< 해당 실행 명령이 끝날때 까지 기다릴지 여부 플래그 */
	char remote_host[SIMCTRL_HOST_SIZE]; /**< remote host에 접속하여 명령 실행 (user@host format) */
	char remote_passwd[SIMCTRL_NAME_SIZE]; /**< remote host에 접속할때 사용될 패스워드 */
	FILE *pipe_fp; /**< popen에 의해 생성된 File pointer. scenario command 제어용 */
};

/** @brief scenario rmi struct */
struct simctrl_scenrmi_s {
	char command[SIMCTRL_COMMAND_SIZE]; /**< rmi 명령어 */
	int display_flag; /**< 화면에 진행 정보를 출력할지 여부 플래그 */
};

/** @brief scenario struct */
struct simctrl_scen_s {
	char name[SIMCTRL_NAME_SIZE]; /**< 시나리오 명 */
	char desc[SIMCTRL_DESC_SIZE]; /**< 시나리오 설명 */
	simctrl_scenrmi_t rmis[MAX_SIMCTRL_RMI*2]; /**< 시나리오 시작 전 수행할 MMC 명령어 */
	int nrmi; /**< 시나리오 시작 전 수행할 MMC 명령어 개수 */
	simctrl_scencmd_t cmds[MAX_SIMCTRL_COMMAND]; /**< 시나리오 상세 커맨드 */
	int ncmd; /**< 시나리오 상세 커맨드 개수 */ 
	simctrl_scengrp_t *grp; /**< 시나리오를 관리하고 있는 그룹 정보 */
	int idx; /**< 전체 시나리오 중 자신의 index 값 */
	char report[4096]; /**< 시나리오 수행 결과 이력 로그. 실패에 대한 상세 정보 기록 */
};

/** @brief scenario group struct */
struct simctrl_scengrp_s {
	char name[SIMCTRL_NAME_SIZE]; /**< 그룹 명 */
	simctrl_scenconf_t *conf; /**< 시나리오 매니저에서 관리하고 있는 config 정보 */
	simctrl_scen_t scens[MAX_SIMCTRL_SCENARIO]; /**< 시나리오 정보 */
	int nscen; /**< 시나리오 개수 */
};

/** @brief scenario configuration struct */
struct simctrl_scenconf_s {
	int display_flag; /**< 화면에 시나리오 실행내용 출력 여부 (0:off, 1:on) */
	int realtime_display; /**< 화면에 시나리오 실행내용 실시간 출력 여부 (0:off, 1:on) */
	int file_report_flag; /**< file에 시나리오 수행 결과 기록 여부 (0:off, 1:on) */ 
	int scen_timeout; /**< 시나리오 수행 타임아웃 시간(초) */
	int scen_interval; /**< 시나리오 수행 후 다음 시나리오 수행 전 유휴 시간(초) */
	int scen_type; /**< 시나리오 타입 (seagull/sipp/none) */
	char rmi_host[SIMCTRL_HOST_SIZE]; /**< mmc 연결 호스트명 또는 IP Address */
	char rmi_port[SIMCTRL_PORT_SIZE]; /**< mmc 연결 포트 번호 */
	char report_plugin[SIMCTRL_FILE_SIZE]; /**< 시나리오 결과 분석 플러그인 */ 
	char ignore_keyword[MAX_SIMCTRL_KEYWORD][SIMCTRL_KEYWORD_SIZE]; 
					/**< 결과 분석 시 차이가 있어도 무시할 내용의 keyword 정보 */
	int ignore_keyword_cnt; /**< ignore keyword 개수 */ 
	char replace_keyword[MAX_SIMCTRL_KEYWORD][SIMCTRL_KEYWORD_SIZE]; 
					/**< 결과 분석 시 예약된 값으로 변경해줄 내용의 keyword 정보 */
	int replace_keyword_cnt; /**< ignore keyword 개수 */ 
	struct {
		char name[SIMCTRL_NAME_SIZE]; 
		char value[SIMCTRL_KEYWORD_SIZE];
	} defines[MAX_SIMCTRL_KEYWORD]; /**< rmi 수행 시 예약된 keyword에 대해 replace 해줄 정보 */
	int define_cnt; /**< defined keyword 개수 */
};

/** @brief scenario manager struct */
struct simctrl_scenmgr_s {
	int load_flag; /**< 시나리오 로딩 여부 */
	simctrl_scenconf_t conf; /**< 시나리오 config 정보 */
	simctrl_scengrp_t grps[MAX_SIMCTRL_GROUP]; /**< 시나리오 그룹 정보 */
	int ngrp; /**< 로딩한 시나리오 그룹 개수 */
	int nscen; /**< 로딩한 시나리오 개수 */
	void *dll; /**< 시나리오 결과 분석 플러그인 포인터 */
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

/** @brief simulator controller scneario manager에 group을 추가한다. */
int simctrl_scenmgr_add_grp( simctrl_scenmgr_t *mgr, simctrl_scengrp_t *grp );
/** @brief simulator controller scneario manager에 group을 삭제한다. */
int simctrl_scenmgr_del_grp( simctrl_scenmgr_t *mgr, char *name );
/** @brief simulator controller scneario manager에서 특정 group을 조회한다. */
simctrl_scengrp_t* simctrl_scenmgr_get_grp( simctrl_scenmgr_t *mgr, char *name );
/** @brief simulator controller scneario manager에서 특정 group을 조회한다. */
simctrl_scengrp_t* simctrl_scenmgr_get_grp_i( simctrl_scenmgr_t *mgr, int idx );

/** @brief simulator controller scneario manager에 scenario를 추가한다. */
int simctrl_scenmgr_add_scen( simctrl_scenmgr_t *mgr, char *gname, simctrl_scen_t *scen );
/** @brief simulator controller scneario manager에 scenario를 삭제한다. */
int simctrl_scenmgr_del_scen( simctrl_scenmgr_t *mgr, char *gname, char *name );
/** @brief simulator controller scneario manager에서 특정 scenario를 조회한다. */
simctrl_scen_t* simctrl_scenmgr_get_scen( simctrl_scenmgr_t *mgr, char *gname, char *name );
/** @brief simulator controller scneario manager에서 특정 scenario를 조회한다. */
simctrl_scen_t* simctrl_scenmgr_get_scen_i( simctrl_scenmgr_t *mgr, int idx );

/** @brief simulator controller scenario manager의 정보를 buffer에 기록 */
int simctrl_scenmgr_sprintf( simctrl_scenmgr_t *mgr, char *buf, int buflen, int mode );
/** @brief simulator controller scenario manager의 정보를 화면에 출력 */
void simctrl_scenmgr_display( simctrl_scenmgr_t *mgr, int mode ) ;
/** @brief simulator controller scenario 수행 결과를 화면에 출력 */
void simctrl_scenmgr_display_result( simctrl_scenmgr_t *mgr, char *scens, struct timeval stime);


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - scenario group 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller scneario group에 scneario를 추가한다. */
int simctrl_scengrp_add_scen( simctrl_scengrp_t *grp, simctrl_scen_t *scen );
/** @brief simulator controller scneario group에 scneario를 삭제한다. */
int simctrl_scengrp_del_scen( simctrl_scengrp_t *grp, char *name );
/** @brief simulator controller scneario group에서 특정 scenario를 조회한다. */
simctrl_scen_t* simctrl_scengrp_get_scen( simctrl_scengrp_t *grp, char *name );
/** @brief simulator controller scneario group에서 특정 scenario를 조회한다. */
simctrl_scen_t* simctrl_scengrp_get_scen_i( simctrl_scengrp_t *grp, int idx );

/** @brief simulator controller scenario group의 정보를 buffer에 기록 */
int simctrl_scengrp_sprintf( simctrl_scengrp_t *grp, char *buf, int buflen, int mode );
/** @brief simulator controller scenario group의 정보를 화면에 출력 */
void simctrl_scengrp_display( simctrl_scengrp_t *grp, int mode );


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - scenario 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller scenario의 정보를 buffer에 기록 */
int simctrl_scen_sprintf( simctrl_scen_t *scen, char *buf, int buflen, int mode );
/** @brief simulator controller scenario의 정보를 화면에 출력 */
void simctrl_scen_display( simctrl_scen_t *scen, int mode );
 
/** @brief simulator controller scenario 실행 및 결과 리포트 함수 */
int simctrl_scen_proc( simctrl_scen_t *scen );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_SCEN_H__
