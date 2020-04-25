
1. 초간단 컴파일 방법 설명

 SIMCTRL_SRC_HOME 환경 변수 생성 (ex> setenv SIMCTRL_SRC_HOME "$HOME/SIMCTRL"

 cd config
 ./configure
 cd ..
 make

 ** configure 옵션 **
 --enable-64bit 64bit binary로 컴파일한다.
 --enable-xopen xopen 확장 OS 함수들을 사용할 수 있도록 컴파일 옵션을 설정한다.
 --disable-debug  debug 옵션 없이 binary를 컴파일한다.
 --disable-optimize 최적화 옵션 없이 binary를 컴파일한다.
 --disable-warnging 컴파일시 warnging을 출력하지 않는다.

 ** 플랫폼별 configure 실행 방법 **
    1) HP-UX IA-64(force장비) :
	     // 기본적인 cc 옵션으로 a.out이 안나와서 명시적으로 CFLAGS를 정의해야함. 
         $> ./configure --enable-64bit CFLAGS='-Ae +DD64' CXXFLAGS='-AA +DD64'

    2) SUN, LINUX 등 특별히 문제없이 그냥 configure 실행하면됨.
