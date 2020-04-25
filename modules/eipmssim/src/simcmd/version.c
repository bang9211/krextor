#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static char _g_version_str[] = {
	/* 최신 버전을 작성해주세요. */
	"Release version 1.0.0\n"
};
static char _g_history_str[] = {
	/* 버전 업데이트 이력을 작성해주세요. 기존 이력 아래 추가해주세요. */
	"2017.03.14 Release version 1.0.0 - IBRS Interface(Receiver/Sender) Simulator 최초 버전\n"
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
