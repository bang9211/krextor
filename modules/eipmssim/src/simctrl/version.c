#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static char _g_version_str[] = {
	/* 최신 버전을 작성해주세요. */
	"Release version 1.3.2\n"
};
static char _g_history_str[] = {
	/* 버전 업데이트 이력을 작성해주세요. 기존 이력 아래 추가해주세요. */
	"2014.04.17 Release version 1.0.0 - Common Simulator Controller 최초 버전 배포\n"
    "                                 - Scenario load / unload / display / run\n"
	"                                 - Simulation result report.\n"
	"2014.04.23 Release version 1.0.1 - command 기능 추가 (remote host, sleep)\n"
	"                                 - report result 기능 개선 (display scenario index)\n"
	"2014.05.09 Release version 1.0.2 - Control signals 추가 (signal 수신할때까지 진행한 결과 리포팅 후 종료)\n"
	"2014.05.29 Release version 1.0.3 - report plugin 기능 추가 (등록된 플러그인에서 report comapre function 호출)\n"
	"2014.06.03 Release version 1.1.0 - RMI 연동 기능 추가 (console 혹은 scenario 에서 rmi 수행 가능)\n"
	"2014.06.12 Release version 1.1.1 - report result 기능 개선 (비교결과 차이나는 부분을 리포트할때 라인번호도 출력)\n"
	"2014.06.12 Release version 1.1.2 - configuration file-report 추가 (시나리오 수행 결과를 파일에 추가 기록할지 여부 관리)\n"
	"2015.06.08 Release version 1.1.3 - configuration define 추가 (rmi,command에서 예약된 keyword에 대해 replace 처리)\n"
	"2015.10.06 Release version 1.1.4 - configuration scenario-timeout, command timeout 추가 (시나리오 수행 타임아웃 관리)\n"
	"2015.11.04 Release version 1.1.5 - configuration replace-keyword 추가 (결과 분석 시 예약된 값으로 변경해줄 내용의 keyword 정보 관리)\n"
	"2016.01.11 Release version 1.1.6 - max size 변경 (group : 20 -> 30, scenario : 100, command : 8 -> 16, rmi : 10)\n"
	"2016.01.21 Release version 1.1.7 - sleep field max value 변경 (60 -> 360 sec)\n"
	"2016.07.22 Release version 1.1.8 - configuration realtime-display 추가 (시나리오 실행 내용 동시 출력 기능)\n"
	"2016.08.05 Release version 1.2.0 - scenario 실행 기능 추가 (prompt 상에서 실행하는 기존 기능 외에 simctrl 실행과 동시에 수행되는 기능 추가)\n"
	"2016.08.11 Release version 1.2.1 - command 기능 추가 (remote passwd) > 설정된 경우 remote cmd 처리 시 expect를 이용하여 password 처리함\n" 
	"2016.09.26 Release version 1.2.2 - scenario load 기능 추가 (simctrl 실행과 동시에 scenario load 수행)\n"
	"2016.11.15 Release version 1.2.3 - report result 기능 개선 (sipp/diameter 메시지 분석 기능 개선 - 메시지 단위 분석, 헤더/AVP 기반 분석)\n"
	"2018.03.27 Release version 1.2.4 - report result 기능 개선 (diff 내용 개선)\n"
	"2018.07.11 Release version 1.2.5 - report result 기능 개선 (ignore keyword 비교 오류 보완)\n"
	"2018.11.02 Release version 1.2.6 - rmi 결과 수신 데이터가 클 경우 수신 처리 도중 실패 처리 현상 보완\n"
	"2018.11.23 Release version 1.2.7 - report result 기능 개선 (diff 내용 개선 - sip \\r\\n 화면 출력시 \\r 제외)\n"
	"2019.04.03 Release version 1.2.8 - command 기능 개선 (script field의 path / command를 나누는 기능 보완)\n"
	"                                 - config 기능 개선 (define, replace-keyword 에 SYSTEM(\"cmd\") 등록 시 system call 수행 결과로 변환해줌)\n"
	"2019.04.23 Release version 1.2.9 - max size 변경 (command : 16 -> 30, rmi : 10 -> 20)\n"
	"2019.05.28 Release version 1.3.0 - command 기능 추가 (scen-reload command 추가 - 기존에 로딩된 시나리오 unload 한 다음 새로운 시나리오 로딩)\n"
	"2019.07.23 Release version 1.3.1 - sipp 시나리오 실행 후 결과 출력 내용 잘림 현상 보완\n"
	"2020.02.24 Release version 1.3.2 - report result 기능 보완 (replace-keyword 이후 ignore keyword 등록한 경우 replace 동작하지 않는 현상 보완)\n"
	//" TODO - 항시기동해야할 보조 프로세스(대개 외부연동 노드 시물레이터) 관리 기능
};


void __version_check__( int argc, char *argv[] ) 
{
	int i, vflag = 0, hflag = 0;

	if( argc >= 2 ) {
		for( i = 1 ; i < argc ; i ++ ) {
			if( !strcasecmp(argv[i], "--version") ) vflag = 1;
			else if( !strcasecmp(argv[i], "--history") ) hflag = 1;
		}
		if( vflag ) printf(_g_version_str);
		if( hflag ) printf(_g_history_str);
		if( vflag || hflag ) exit(0);
	}
}
