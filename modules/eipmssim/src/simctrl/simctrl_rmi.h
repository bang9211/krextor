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
	char init_flag; /**< �ʱ�ȭ ���� �÷��� */
	char host[SIMCTRL_HOST_SIZE]; /**< mmc ���� ȣ��Ʈ�� Ȥ�� IP Address */
	short port; /**< mmc ���� ��Ʈ ��ȣ */
	struct timeval timeout; /**< result message ���� ��� �ð� */
	int sd; /**< mmc ���� socket descriptor.  -1: disconnect, 0�̻�: socket descriptor */
	char login_result_flag; /**< ó�� ���� �� ������ ����� �޾Ҵ��� ���� �÷��� 
	                             ���� ����� ȭ�鿡 ������� �ʱ� ���� �ʿ� */
	int proc_flag; /**< scenario���� rmi ���� ������ Ȯ���� ���� �÷��� */
};


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - rmi 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller rmi instance */
simctrl_rmi_t* simctrl_rmi_instance();
/** @brief simulator controller rmi instance �ʱ�ȭ */
int simctrl_rmi_init(simctrl_rmi_t *rmi, char *host, short port, int msec );
/** @brief simulator controller rmi intance final */
void simctrl_rmi_final(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi mmcd�� ���� �õ� */
int simctrl_rmi_connect(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi mmcd�� ���� ���� */
int simctrl_rmi_close(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi ���� ȭ�鿡 ��� */
void simctrl_rmi_display(simctrl_rmi_t *rmi);
/** @brief simulator controller rmi mmc��ɾ mmcd�� ���� */
int simctrl_rmi_send_request(simctrl_rmi_t *rmi, char *cmd);
/** @brief simulator controller rmi mmc��ɾ� ó�� ����� ���� */
int simctrl_rmi_recv_result(simctrl_rmi_t *rmi, int display_type);
/** @brief simulator controller rmi mmc��ɾ� ó�� ����� mmcd�� ���� ���� */ 
int simctrl_rmi_run(simctrl_rmi_t *rmi, char *cmd, int display_type);


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_RMI_H__
