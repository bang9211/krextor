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

/** @brief simulator controller command 초기화 함수 */
int simctrl_cmd_init( char *prompt, int sleep_time );
/** @brief simulator controller command 실행 함수
          stop 호출 전까지 명령어 입력을 기다리고 입력한 명령어를 실행. */
int simctrl_cmd_start();
/** @brief simulator controller command 종료 함수 */
void simctrl_cmd_stop();
/** @brief simulator controller command 실행 함수*/
int simctrl_cmd_run( int argc, char *argv[] );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_CMD_H__
