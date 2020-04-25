#ifndef __SIMCTRL_CMD_H__
#define __SIMCTRL_CMD_H__


#ifdef __cplusplus 
extern "C" {
#endif 


/////////////////////////////////////////////////////////////////////////////////
// struct & define for simulator controller - command
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - command
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller command �ʱ�ȭ �Լ� */
int simctrl_cmd_init( char *prompt, int sleep_time );
/** @brief simulator controller command ���� �Լ�
          stop ȣ�� ������ ��ɾ� �Է��� ��ٸ��� �Է��� ��ɾ ����. */
int simctrl_cmd_start();
/** @brief simulator controller command ���� �Լ� */
void simctrl_cmd_stop();
/** @brief simulator controller command ���� �Լ�*/
int simctrl_cmd_run( int argc, char *argv[] );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_CMD_H__
