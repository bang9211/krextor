#ifndef __SIMCTRL_REPORT_H__
#define __SIMCTRL_REPORT_H__


#include "simctrl_def.h"


#ifdef __cplusplus 
extern "C" {
#endif 


/////////////////////////////////////////////////////////////////////////////////
// struct & define for simulator controller - report 
/////////////////////////////////////////////////////////////////////////////////

/** @brief report check function type */
typedef int (*simctrl_report_chk_f)(char *ebuf, char *rbuf, int type, char *result, int reslen);
/** @brief report compare function type */
typedef int (*simctrl_report_cmp_f)(char *a, char *b);


/////////////////////////////////////////////////////////////////////////////////
// functions for simulator controller - report 
/////////////////////////////////////////////////////////////////////////////////

/** @brief simulator controller report check ó�� callback �Լ� ���� */
void simctrl_report_set_chk_callback( simctrl_report_chk_f func );
/** @brief simulator controller report ó�� �� �߰� line compare ó�� �Լ� ���� */
void simctrl_report_set_cmp_callback( simctrl_report_cmp_f func );
/** @brief simulator controller report ó�� �Լ� */
int simctrl_report_check( char *expected, char *received, int type, char *result, int reslen );


#ifdef __cplusplus 
}
#endif


#endif //#ifndef __SIMCTRL_REPORT_H__
