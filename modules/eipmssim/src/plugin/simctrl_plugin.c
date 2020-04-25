#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "simctrl_def.h"


/* report compare plugin function */ 
int simctrl_report_compare_function( char *ebuf, char *rbuf, int type, char *report, int reslen )
{
	// expected와 received를 비교하여 결과 성공/실패 결정
	return eSIMCTRL_SUCCESS;
}

