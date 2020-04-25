#ifndef __SIMCTRL_RMI_H__
#define __SIMCTRL_RMI_H__


#include <time.h>
#include <sys/types.h>
#include "simctrl_def.h"


#ifdef __cplusplus 
extern "C" {
#endif 


/////////////////////////////////////////////////////////////////////////////////
// struct & define for simulator controller - rmi 
/////////////////////////////////////////////////////////////////////////////////

/** @brief rmi struct type */
typedef struct simctrl_rmi_s simctrl_rmi_t;

/** @brief rmi struct */
struct simctrl_rmi_s {
	char init_flag; /**< 초기화 여부 플래그 */
	char host[SIMCTRL_HOST_SIZE]; /**< mmc 연동 호스트명 혹은 IP Address */
	short port; /**< mmc 연동 포트 번호 */
	struct timeval timeout; /**< result message 수신 대기 시간 */
	int sd; /**< mmc 연결 socket descriptor.  -1: disconnect, 0이상: socket descriptor */
	char login_result_flag; /**< 처음 연결 후 인증한 결과를 받았는지 여부 플래그 
	                             인증 결과를 화면에 출력하지 않기 위해 필요 */
	int proc_flag; /**< scenario에서 rmi 수행 중인지 확인을 위한 플래그 */
};


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - rmi 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller rmi instance */
simctrl_rmi_t* simctrl_rmi_instance();
/** @brief simulator controller rmi instance 초기화 */
int simctrl_rmi_init(simctrl_rmi_t *rmi, char *host, short port, int msec );
/** @brief simulator controller rmi intance final */
void simctrl_rmi_final(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi mmcd에 연결 시도 */
int simctrl_rmi_connect(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi mmcd와 연결 해제 */
int simctrl_rmi_close(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi 정보 화면에 출력 */
void simctrl_rmi_display(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi mmc명령어를 mmcd에 전송 */
int simctrl_rmi_send_request(simctrl_rmi_t *rmi, char *cmd);
/** @brief simulator controller rmi mmc명령어 처리 결과를 수신 */
int simctrl_rmi_recv_result(simctrl_rmi_t *rmi, int display_type);
/** @brief simulator controller rmi mmc명령어 처리 결과를 mmcd로 부터 수신 */ 
int simctrl_rmi_run(simctrl_rmi_t *rmi, char *cmd, int display_type);


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_RMI_H__
